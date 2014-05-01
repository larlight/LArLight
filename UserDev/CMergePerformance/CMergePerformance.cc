#ifndef CMERGEPERFORMANCE_CC
#define CMERGEPERFORMANCE_CC

#include "CMergePerformance.hh"

namespace larlight {

  bool CMergePerformance::initialize() {
    
    PrepareHistos();
    
    w2cm = larutil::GeometryUtilities::GetME()->WireToCm();
    t2cm = larutil::GeometryUtilities::GetME()->TimeToCm();

    return true;
  }
  
  bool CMergePerformance::analyze(storage_manager* storage) {
    //this function is called once per event

    //grab the reconstructed clusters
    larlight::event_cluster* ev_cluster = (larlight::event_cluster*)storage->get_data(larlight::DATA::FuzzyCluster);
    if(!ev_cluster) {
      print(larlight::MSG::ERROR,__FUNCTION__,Form("Did not find specified data product, FuzzyCluster!"));
      return false;
    }
    
    //grab the reconstructed hits
    const larlight::DATA::DATA_TYPE hit_type = ev_cluster->get_hit_type();
    larlight::event_hit* ev_hits = (larlight::event_hit*)storage->get_data(hit_type);
    if(!ev_hits) {
      print(larlight::MSG::ERROR,__FUNCTION__,Form("Did not find specified data product, Hits!"));
      return false;
    }
      
    //grab the MC showers
    larlight::event_mcshower* ev_mcshower = (larlight::event_mcshower*)storage->get_data(larlight::DATA::MCShower);
    if(!ev_mcshower) {
      print(larlight::MSG::ERROR,__FUNCTION__,Form("Did not find specified data product, MCShower!"));
      return false;
    }

    //grab the simchannels
    larlight::event_simch* ev_simch = (larlight::event_simch*)storage->get_data(larlight::DATA::SimChannel);
    if(!ev_simch) {
      print(larlight::MSG::ERROR,__FUNCTION__,Form("Did not find specified data product, SimChannel!"));
      return false;
    }

    //first start with the raw fuzzyclusters and find their CPAN params
    FillClusterParamsVector(ev_cluster,ev_hits);

    //once that is done, _clusterparams is filled, calculate figure
    //of merit and fill histos with it
    bool after_merging = false;
    FillFOMHistos(after_merging,ev_mcshower,ev_simch,ev_hits,_clusterparams);

    //now run merging with whatever algos you want
    //this overwites _clusterparams with the new (merged) clusters
    RunMerging(ev_cluster,ev_hits);

    //now fill the other FOM histos
    after_merging = true;
    FillFOMHistos(after_merging,ev_mcshower,ev_simch,ev_hits,_clusterparams);

    return true;
  }

  bool CMergePerformance::finalize() {

    if(_fout) { 
      _fout->cd(); 
      for(int before_after = 0; before_after < 2; ++before_after){
	hPurity.at(before_after)->Write();
	hNClusOverNMCShowers.at(before_after)->Write();

	delete hPurity.at(before_after);
	delete hNClusOverNMCShowers.at(before_after);
      }
      
    }
    else 
      print(MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
    

      
    return true;
  }

  void CMergePerformance::PrepareHistos(){
    
    hPurity.push_back(
		      new TH1D("hPurity_before_merging",
			       "Clus Purity before Merging",
			       100,0,1)
		      );

    hPurity.push_back(
		      new TH1D("hPurity_after_merging",
			   "Clus Purity after Merging",
			   100,0,1)
		      );

    hNClusOverNMCShowers.push_back(
				   new TH1D("hNClusOverNMCShowers_before_merging",
					    "NClusOverNMCShowers before Merging",
					    50,-0.5,49.5)
				   );
    
    hNClusOverNMCShowers.push_back(
				   new TH1D("hNClusOverNMCShowers_after_merging",
					    "NClusOverNMCShowers after Merging",
					    50,-0.5,49.5)
				   );
  }


  void CMergePerformance::FillClusterParamsVector(event_cluster* ev_cluster,
						  event_hit* ev_hits){

    // Now we make ClusterParamsAlgNew instance per cluster
    _clusterparams.clear();
    //    _clusterparams.reserve(ev_cluster->size());

    //loop over the reconstructed clusters
    const larlight::DATA::DATA_TYPE hit_type = ev_cluster->get_hit_type();

    for(auto const i_cluster: *ev_cluster){
      
      const std::vector<unsigned short> ass_index(i_cluster.association(hit_type));
      if(ass_index.size()<15) continue;

      const UChar_t plane = larutil::Geometry::GetME()->ChannelToPlane(ev_hits->at((*ass_index.begin())).Channel());

      // Compute cluster parameters (ClusterParamsAlgNew) and store (_clusterparams)
      std::vector<larutil::PxHit> tmp_hits;
      tmp_hits.reserve(ass_index.size());

      for(auto const index : ass_index) {	
	larutil::PxHit h;
	h.w = ev_hits->at(index).Wire() * w2cm;
	h.t = ev_hits->at(index).PeakTime() * t2cm;
	h.plane = plane;
	h.charge = ev_hits->at(index).Charge();
	tmp_hits.push_back(h);
      }
      

      _clusterparams.push_back(::cluster::ClusterParamsAlgNew());
	
      try {
	(*_clusterparams.rbegin()).setNeuralNetPath(myNeuralNetPath);
	(*_clusterparams.rbegin()).SetHits(tmp_hits);
	(*_clusterparams.rbegin()).FillPolygon();
	(*_clusterparams.rbegin()).FillParams();
      }catch( ::cluster::RecoUtilException) {
	
	print(larlight::MSG::ERROR,__FUNCTION__,Form("Cluster %d too bad to run ClusterParamsAlgNew!",i_cluster.ID()));
      }
      
    } //end loop over ev_cluster
        

    return;
  }//end FillClusterParamsVector
  
  void CMergePerformance::FillFOMHistos(bool after_merging,
					event_mcshower* ev_mcshower,
					event_simch* ev_simch,
					event_hit* ev_hits,
					const std::vector<::cluster::ClusterParamsAlgNew> &_clusterparams){

    //shower_idmap is (G4trackid => MCShower index in ev_mcshower)
    std::map<UInt_t,UInt_t> shower_idmap;
    _mcslb.FillShowerMap(ev_mcshower,shower_idmap);

    //now simch_map is (channel => larlight::simch)
    std::map<UShort_t, larlight::simch> simch_map;
    _mcslb.FillSimchMap(ev_simch,simch_map);
    
    //make a vector of MCShower indices (cutting out MCShowers with
    //super-low-energy mothers... i only want the big-uns
    std::vector<UInt_t> MCShower_indices;
    for(UInt_t i=0; i < ev_mcshower->size(); ++i){
      //      if(ev_mcshower->at(i).MotherMomentum().at(3) > 0.01)
      MCShower_indices.push_back(i);
    }

    //loop over hits, for each: compute the % of its charge that belongs to each MCShower
    for(auto const i_hit: *ev_hits){
      
      std::vector<float> hit_charge_frac = 
	_mcslb.MatchHitsAll(i_hit,
			    simch_map,
			    shower_idmap,
			    MCShower_indices);
      //debug
      //      std::cout<<"This hit is made up of (";
      //      for(int i=0; i<hit_charge_frac.size()-1; ++i)
      //	std::cout<<hit_charge_frac.at(i)<<", ";
      //      std::cout<<") fractional charge from "<<
      //	hit_charge_frac.size()-1<<" known MCShowers and "<<
      //	(float)hit_charge_frac.back()<<" from unknown ones."<<std::endl;
      //end debug

    }
    //debug: print contents of shower_idmap map
    //    for(std::map<UInt_t,UInt_t>::iterator it=shower_idmap.begin(); it!=shower_idmap.end(); ++it)
    //      std::cout<<"viewing whoer_idmap: "<<it->first << " => "<<it->second <<std::endl;

    

    ////////////////////////////////////////////
    //now we're done with getting all the data//
    //let's fill some histograms w/ it        //
    ////////////////////////////////////////////

    double NClusOverNMCShowers = 
      (double)_clusterparams.size()/(double)MCShower_indices.size();
    hNClusOverNMCShowers.at(after_merging)->Fill(NClusOverNMCShowers);


    //fill hPurity plot here... need to decide what purity means...


    
  }//end fillFOMhistos


  void CMergePerformance::RunMerging(event_cluster* ev_cluster,
				     event_hit* ev_hits)
  {

    std::vector<std::vector<larutil::PxHit> > local_clusters(ev_cluster->size(),
							     std::vector<larutil::PxHit>()
							     );

    DATA::DATA_TYPE hit_type = ev_cluster->get_hit_type();

    
    for(size_t i=0; i<ev_cluster->size(); ++i) {
      
      std::vector<unsigned short> hit_indexes(ev_cluster->at(i).association(hit_type));
      
      local_clusters.at(i).reserve(hit_indexes.size());
      
      for(auto index : hit_indexes) {
	larutil::PxHit h;
	h.w = ev_hits->at(index).Wire() * w2cm;
	h.t = ev_hits->at(index).PeakTime() * t2cm;
	h.plane = larutil::Geometry::GetME()->ChannelToPlane(ev_hits->at(index).Channel());
	h.charge = ev_hits->at(index).Charge();
	local_clusters.at(i).push_back(h);
      }

    }

    //do the actual merging with those algos
    //user needs to add algos etc w/ python, via GetManager() function
    _mgr.SetClusters(local_clusters);
    _mgr.Process();

    //clear the clusterparams vector (which was CPAN params from before merging)
    _clusterparams.clear();

    //overwite the clusterparams vector w/ the merged CPAN params
    _clusterparams=_mgr.GetClusters();
    
  }//end RunMerging


}
#endif
