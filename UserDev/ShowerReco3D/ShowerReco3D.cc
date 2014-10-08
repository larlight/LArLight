#ifndef SHOWERRECO3D_CXX
#define SHOWERRECO3D_CXX

#include "ShowerReco3D.hh"

namespace larlight {

  ShowerReco3D::ShowerReco3D() : ana_base(), fShowerAlgo(nullptr), fMatchMgr(nullptr)
  {
    _name="ShowerReco3D";
    fClusterType = DATA::Cluster;

    auto geom = ::larutil::Geometry::GetME();
    if(!fMatchMgr) fMatchMgr = new ::cmtool::CMatchManager(geom->Nplanes());
  }
  
  bool ShowerReco3D::initialize() {
    _mgr = 0;
    if(!fShowerAlgo) {
      
      throw ::cluster::CRUException("Shower reco algorithm not attached... aborting.");
      return false;

    }
    // Make sure cluster type is a valid one
    if( fClusterType != DATA::Cluster &&
	fClusterType != DATA::MCShowerCluster &&
	fClusterType != DATA::DBCluster &&
        fClusterType != DATA::FuzzyCluster &&
	fClusterType != DATA::CrawlerCluster &&
	fClusterType != DATA::ShowerAngleCluster &&
	fClusterType != DATA::RyanCluster)

      throw ::cluster::CRUException(Form("Not supported cluster type: %s",
					 DATA::DATA_TREE_NAME[fClusterType].c_str())
				    );
    
    return true;
  }
  
  bool ShowerReco3D::analyze(storage_manager* storage) {

    _mgr = storage;
    // Re-initialize tools
    fShowerAlgo->Reset();
    fMatchMgr->Reset();

    // Retrieve clusters and fed into the algorithm
    std::vector<std::vector<larutil::PxHit> > local_clusters;

    fCRUHelper.GeneratePxHit(storage,fClusterType,local_clusters);
    
    fMatchMgr->SetClusters(local_clusters);

    local_clusters.clear();

    // Run matching & retrieve matched cluster indices
    try{
      fMatchMgr->Process();
    }catch( ::cmtool::CMTException &e){
      e.what();
      return false;
    }

    auto const& matched_pairs = fMatchMgr->GetBookKeeper().GetResult();

    //
    // Run shower reco algorithm and store output
    //

    // Prepare output storage
    auto shower_v = (event_shower*)(storage->get_data(DATA::Shower));
    shower_v->clear();
    shower_v->reserve(matched_pairs.size());
    shower_v->set_event_id(storage->get_data(fClusterType)->event_id());
    shower_v->set_run(storage->get_data(fClusterType)->run());
    shower_v->set_subrun(storage->get_data(fClusterType)->subrun());

    // Create association holder
    std::vector<std::vector<unsigned int> > ass_index_v;
    ass_index_v.reserve(matched_pairs.size());
    
    // Loop over matched pairs
    for(auto const& pair : matched_pairs) {
      
      // Create an input data holder
      std::vector< ::cluster::ClusterParamsAlg> clusters;
      clusters.reserve(pair.size());      

      // Create an association vector
      std::vector<unsigned int> ass_index;
      ass_index.reserve(pair.size());

      for(auto const& cluster_index : pair) {

	ass_index.push_back(cluster_index);
	
	clusters.push_back(fMatchMgr->GetInputClusters().at(cluster_index));
	
      }

      // Temporarily store association
      ass_index_v.push_back(ass_index);

      // Append input to shower reco algorithm
      fShowerAlgo->AppendInputClusters(clusters);

    }

    // Run shower reco
    auto result_v = fShowerAlgo->Reconstruct();

    // Make sure result has the same size 
    if(result_v.size() != ass_index_v.size())
      throw ::showerreco::ShowerRecoException("Mismatch in # of showers from algorithm's return!");

    for(size_t index = 0; index < result_v.size(); ++index) {

      auto& result = result_v.at(index);

      auto const& ass = ass_index_v.at(index);

      // Add association
      result.add_association(fClusterType,ass);

      // Set ID
      result.set_id(shower_v->size());

      // Store
      shower_v->push_back(result);

    } // done looping over matched cluster pairs

    return true;
  }
  
  bool ShowerReco3D::finalize() {

    return true;
  }
  
}
#endif
