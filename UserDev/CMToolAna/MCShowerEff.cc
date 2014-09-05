#ifndef MCSHOWEREFF_CC
#define MCSHOWEREFF_CC

#include "MCShowerEff.hh"

namespace larlight {

  bool MCShowerEff::initialize() {

    auto geom = ::larutil::Geometry::GetME();

    vMergeQEff.clear();

    for(size_t i=0; i<geom->Nplanes(); ++i) {

      vMergeQEff.push_back(new TH1D(Form("hMergeQEffPlane%zu",vMergeQEff.size()),
				    Form("Merging Charge Efficiency for Plane %zu; Efficiency; # MCShower",vMergeQEff.size()),
				    101,-0.005,1.005)
			   );

      vMergeQPur.push_back(new TH1D(Form("hMergeQPurPlane%zu",vMergeQPur.size()),
				    Form("Merging Charge Purity for Plane %zu; Purity; # MCShower",vMergeQPur.size()),
				    101,-0.005,1.005)
			   );
    }   
    return true;
  }
  
  bool MCShowerEff::analyze(storage_manager* storage) {

    auto geom = ::larutil::Geometry::GetME();

    // Get data products from storage
    //auto cluster_v  = (event_cluster*  )( storage->get_data(DATA::FuzzyCluster)  );
    auto cluster_v  = (event_cluster*  )( storage->get_data(fClusterType)  );

    // Check data exists
    if( !cluster_v ) {
      print(MSG::ERROR,__FUNCTION__,Form("Missing %s! not doing anything...",DATA::DATA_TREE_NAME[fClusterType].c_str()));
      return false;
    }
    if(!(cluster_v->size())) {
      print(MSG::ERROR,__FUNCTION__,Form("Cluster %s is empty! not doing anything...",DATA::DATA_TREE_NAME[fClusterType].c_str()));
      return false;
    }

    auto const hit_type = cluster_v->get_hit_type();

    auto hit_v      = (event_hit*      )( storage->get_data(hit_type) );

    // Check data exists
    if( !hit_v ){
      print(MSG::ERROR,__FUNCTION__,Form("Missing %s is empty! not doing anything...",DATA::DATA_TREE_NAME[hit_type].c_str()));
      return false;
    }
    if( !(hit_v->size()) ){
      print(MSG::ERROR,__FUNCTION__,Form("Hit %s is empty! not doing anything...",DATA::DATA_TREE_NAME[hit_type].c_str()));
      return false;
    }

    fBTAlg.Prepare(storage);

    // Get relevant MCShower index array
    auto const& mcs_index_v = fBTAlg.ShowerIDs();

    if(!mcs_index_v.size()) return false;

    // Get truth charge per MCShower
    std::vector<std::vector<const larlight::hit*> >all_hits(geom->Nplanes(),std::vector<const larlight::hit*>());
    for(auto& hits : all_hits) hits.reserve(hit_v->size());
    for(auto& h : *hit_v)

      all_hits.at(h.View()).push_back(&h);

    std::vector<std::vector<float> > mcs_q_v(geom->Nplanes(),std::vector<float>());
    for(size_t i=0; i<geom->Nplanes(); ++i)

      mcs_q_v.at(i) = fBTAlg.MCShowerQ(all_hits.at(i));

    // Create & fill a vector of MC charge for each MCShower per cluster
    std::vector<std::vector<float> > cluster_mcq_v;
    std::vector<size_t> cluster_plane_v;
    cluster_mcq_v.reserve(cluster_v->size());
    cluster_plane_v.reserve(cluster_v->size());
    for(auto const& c : *cluster_v) {

      auto const hit_index = c.association(hit_type);

      std::vector<const larlight::hit*> c_hits;
      c_hits.reserve(hit_index.size());

      size_t plane = 0;

      for(auto const& i : hit_index) {

	c_hits.push_back(&(hit_v->at(i)));
	plane = hit_v->at(i).View();

      }

      cluster_plane_v.push_back(plane);
      cluster_mcq_v.push_back(fBTAlg.MCShowerQ(c_hits));

    }

    // Per MCShower, compute the maximum charge efficiency
    std::vector<std::vector<size_t> > rep_cluster_index(geom->Nplanes(),std::vector<size_t>(mcs_q_v.at(0).size(),1e4));

    std::cout<<std::endl<<"event "<<cluster_v->event_id()<<std::endl;
    for(size_t mcs_index=0; mcs_index < mcs_q_v.at(0).size(); ++mcs_index) {
      std::cout<<"mcshower "<<mcs_index<<std::endl;
      
      std::vector<float>  max_q(geom->Nplanes(),0);
      std::vector<size_t> rep_cluster_index(geom->Nplanes(),1e4);
      
      for(size_t c_index=0; c_index<cluster_mcq_v.size(); ++c_index) {

	auto const& plane = cluster_plane_v.at(c_index);

	//std::cout << "cluster "<<c_index<<" ... "<<"plane "<<plane<<" ... "<<cluster_mcq_v.at(c_index).at(mcs_index)<<std::endl;

	if( max_q.at(plane) < cluster_mcq_v.at(c_index).at(mcs_index) ) {
	  
	  max_q.at(plane) = cluster_mcq_v.at(c_index).at(mcs_index);

	  rep_cluster_index.at(plane) = c_index;
	}
      }

      for(size_t plane=0; plane<geom->Nplanes(); ++plane) {

	if(rep_cluster_index.at(plane) == 1e4) continue;
	
	vMergeQEff.at(plane)->Fill( max_q.at(plane) / mcs_q_v.at(plane).at(mcs_index) );

	float qsum = 0;

	for(auto const& q : cluster_mcq_v.at(rep_cluster_index.at(plane)))
	  
	  qsum += q;

	std::cout<<"plane "<<plane<<" "<<max_q.at(plane)<<"/"<<qsum<<std::endl;
	vMergeQPur.at(plane)->Fill( max_q.at(plane) / qsum );
      }
    }

    return true;
  }

  bool MCShowerEff::finalize() {

    if(_fout) {
      
      for(auto& h : vMergeQEff) h->Write();
      for(auto& h : vMergeQPur) h->Write();

    }

    return true;
  }

}
#endif
