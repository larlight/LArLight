#ifndef CMERGEPERFORMANCE_CC
#define CMERGEPERFORMANCE_CC

#include "CMergePerformance.hh"

namespace larlight {

  bool CMergePerformance::initialize() {
    
    PrepareHistos();
    
    w2cm = larutil::GeometryUtilities::GetME()->WireToCm();
    t2cm = larutil::GeometryUtilities::GetME()->TimeToCm();


    ////////////////////////////
    //Merging algorithm stuff://
    ////////////////////////////

    _mgr.MergeTillConverge(false);

    _shortest_dist_algo.SetSquaredDistanceCut(200.);
    _shortest_dist_algo.SetVerbose(false);

    //define what algos you want to use for merging, etc
    //algos should be initialized in intialize function
    //IMPORTANT INHERITANCE IDEA THAT I REALLY SHOULD HAVE KNOWN:
    //addmergealgo() wants to receive a pointer to CBoolAlgoBase object
    //but I am passing it a pointer to CMAlgoShortestDist object
    //wtf? it CASTS it as a CBoolAlgoBase. it COULD NOT DO THIS
    //had i passed it a CMAlgoShortestDist object instead of a pointer to one.
    _mgr.AddMergeAlgo(&_shortest_dist_algo);

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

    //first start with the raw fuzzyclusters and find their CPAN params
    FillClusterParamsVector(ev_cluster,ev_hits);

    //once that is done, _clusterparams is filled, calculate figure
    //of merit and fill histos with it
    FillFOMHistos(true,ev_mcshower,_clusterparams);

    //now run merging with whatever algos you want
    //this overwite _clusterparams with the new (merged) clusters
    RunMerging(ev_cluster,ev_hits);

    //now fill the other FOM histos
    FillFOMHistos(false,ev_mcshower,_clusterparams);

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
					    100,0,100)
				   );
    
    hNClusOverNMCShowers.push_back(
				   new TH1D("hNClusOverNMCShowers_after_merging",
					    "NClusOverNMCShowers after Merging",
					    100,0,100)
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
  
  void CMergePerformance::FillFOMHistos(bool before_merging,
					event_mcshower* ev_mcshower,
					const std::vector<::cluster::ClusterParamsAlgNew> &_clusterparams){

    unsigned int histo_index = before_merging ? 0 : 1;



    //This is where stuff needs to be coded up

    //loop over the MC Showers
    //implement bryce stuff here
    int n_viable_mcshowers = 0;
    for(auto i_mcshower: *ev_mcshower){

      //sometimes there are more than one shower, cut out those
      if(i_mcshower.MotherMomentum().at(3) > 0.01)
	n_viable_mcshowers++;

    }

    double NClusOverNMCShowers = 
      (double)_clusterparams.size()/(double)n_viable_mcshowers;
    hNClusOverNMCShowers.at(histo_index)->Fill(NClusOverNMCShowers);
    

    
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
    std::cout<<Form("Processing event %d...",ev_cluster->event_id())<<std::endl;


    //do the actual merging with those algos
    _mgr.SetClusters(local_clusters);
    _mgr.Process();

    //clear the clusterparams vector (which was CPAN params from before merging)
    _clusterparams.clear();

    //overwite the clusterparams vector w/ the merged CPAN params
    _clusterparams=_mgr.GetClusters();
    
  }//end RunMerging


}
#endif
