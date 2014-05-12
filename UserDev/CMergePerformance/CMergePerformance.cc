#ifndef CMERGEPERFORMANCE_CC
#define CMERGEPERFORMANCE_CC

#include "CMergePerformance.hh"

namespace larlight {
  
  bool CMergePerformance::initialize() {
    
    ana_tree=0;
    PrepareTTree();
    
    PrepareHistos();
    
    w2cm = larutil::GeometryUtilities::GetME()->WireToCm();
    t2cm = larutil::GeometryUtilities::GetME()->TimeToCm();
    
    return true;
  }
  
  bool CMergePerformance::analyze(storage_manager* storage) {
          
    //grab the reconstructed clusters
    larlight::event_cluster* ev_cluster = (larlight::event_cluster*)storage->get_data(larlight::DATA::FuzzyCluster);
    if(!ev_cluster) {
      print(larlight::MSG::ERROR,__FUNCTION__,Form("Did not find specified data product, FuzzyCluster!"));
      return false;
    }
    
    //grab the reconstructed hits
    hit_type = ev_cluster->get_hit_type();
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

    //fill some std::maps that are constant through the whole event
    //_shower_idmap is (G4trackid => MCShower index in ev_mcshower)
    _shower_idmap.clear();
    _mcslb.FillShowerMap(ev_mcshower,_shower_idmap);

    //_simch_map is (channel => larlight::simch)
    _simch_map.clear();
    _mcslb.FillSimchMap(ev_simch,_simch_map);


    if(_run_before_merging){
      //first start with the raw fuzzyclusters and find their CPAN params
      FillClusterParamsVector(ev_cluster,ev_hits);
      
      //once that is done, _clusterparams is filled, calculate figure
      //of merit and fill histos with it
      _after_merging = false;
      FillFOMHistos(_after_merging,ev_mcshower,ev_cluster,ev_hits,_clusterparams);
    }

    if(_run_merging){
      //now run merging with whatever algos you want
      //this overwites _clusterparams with the new (merged) clusters
      RunMerging(ev_cluster,ev_hits);
      
      //now fill the other FOM histos
      _after_merging = true;
      FillFOMHistos(_after_merging,ev_mcshower,ev_cluster,ev_hits,_clusterparams);    
    }

    return true;
  }

  bool CMergePerformance::finalize() {

    if(_fout) { 
      _fout->cd(); 
      for(int before_after = 0; before_after < 2; ++before_after){
	for(int nview = 0; nview < 3; ++nview){
	  hPurity.at(before_after).at(nview)->Write();
	  hEff.at(before_after).at(nview)->Write();
	  hClusQoverMCQ.at(before_after).at(nview)->Write();

	  delete hPurity.at(before_after).at(nview);
	  delete hEff.at(before_after).at(nview);
	  delete hClusQoverMCQ.at(before_after).at(nview);
	}
      }

      if(ana_tree){
	ana_tree->Write();
	delete ana_tree;
      }
      
    }
    else 
      print(MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
    
    
    
    return true;
  }
  
  void CMergePerformance::PrepareHistos(){
    
    std::vector<TH1D*> tmp;
    tmp.clear();
    for(int i_view = 0; i_view < 3; i_view++)
      tmp.push_back(
		    new TH1D(Form("hPurity_before_merging_view%d",i_view),
			     Form("Clus Purity (Clus Charge Fraction from Dominant MCShower) [before Merge], view %d",i_view),
			     100,-0.1,1.1)
		    );
    hPurity.push_back(tmp);
    
    tmp.clear();
    for(int i_view = 0; i_view < 3; i_view++)
      tmp.push_back(
		    new TH1D(Form("hPurity_after_merging_view%d",i_view),
			     Form("Clus Purity (Clus Charge Fraction from Dominant MCShower) [after Merge], view %d",i_view),
			     100,-0.1,1.1)
		    );
    hPurity.push_back(tmp);
    
    tmp.clear();
    for(int i_view = 0; i_view < 3; i_view++)
      tmp.push_back(
		    new TH1D(Form("hEff_before_merging_view%d",i_view),
			     Form("# MCShowers / # Clusters per evt [before Merging], view %d",i_view), 
			     300,-0.1,3.1)
		    );
    hEff.push_back(tmp);
    
    tmp.clear();
    for(int i_view = 0; i_view < 3; i_view++)
      tmp.push_back(
		    new TH1D(Form("hEff_after_merging_view%d",i_view),
			     Form("# MCShowers / # Clusters per evt [after Merging], view %d",i_view), 
			     300,-0.1,3.1)
		    );
    hEff.push_back(tmp);
    
    tmp.clear();
    for(int i_view = 0; i_view < 3; i_view++)
      tmp.push_back(
		    new TH1D(Form("hClusQoverMCQ_before_merging_view%d",i_view),
			     Form("Cluster Charge / Dominant MCShower Charge [before Merging], view %d",i_view),
			     100,-0.1,10)
		    );
    hClusQoverMCQ.push_back(tmp);
   
    tmp.clear();
    for(int i_view = 0; i_view < 3; i_view++)
      tmp.push_back(
		    new TH1D(Form("hClusQoverMCQ_after_merging_view%d",i_view),
			     Form("Cluster Charge / Dominant MCShower Charge [after Merging], view %d",i_view),
			     100,-0.1,10)
		    );
    hClusQoverMCQ.push_back(tmp);
    
    std::vector<TH2D*> tmp2D;
    tmp2D.clear();
    for(int i_view = 0; i_view < 3; i_view++)
      tmp2D.push_back(
		    new TH2D(Form("hPi0_photonanglediff_vs_Eff_view%d",i_view),
			     Form("Pi0 Photon 2D Angle Difference vs. Efficiency [before merging], view %d",i_view),
			     300,-0.1,3.1,
			     100,0,180)
		    );
    hPi0_photonanglediff_vs_Eff.push_back(tmp2D);

    tmp2D.clear();
    for(int i_view = 0; i_view < 3; i_view++)
      tmp2D.push_back(
		    new TH2D(Form("hPi0_photonanglediff_vs_Eff_view%d",i_view),
			     Form("Pi0 Photon 2D Angle Difference vs. Efficiency [after merging], view %d",i_view),
			     300,-0.1,3.1,
			     100,0,180)
		    );
    hPi0_photonanglediff_vs_Eff.push_back(tmp2D);

  }
  
  void CMergePerformance::PrepareTTree(){
    
    if(!ana_tree) {
      
      ana_tree = new TTree("ana_tree","");
 
      ana_tree->Branch("clusQfrac_over_totclusQ",&_clusQfrac_over_totclusQ,"clusQfrac_over_totclusQ/D");
      ana_tree->Branch("clusQ_over_MCQ",&_clusQ_over_MCQ,"clusQ_over_MCQ/D");
      ana_tree->Branch("tot_clusQ",&_tot_clus_charge,"tot_clus_charge/D");
      ana_tree->Branch("frac_clusQ",&_frac_clusQ,"frac_clusQ/D");
      ana_tree->Branch("dom_MCS_Q",&_dom_MCS_Q,"dom_MCS_Q/D");
      ana_tree->Branch("plane",&_plane,"plane/I");
      ana_tree->Branch("mother_energy",&_mother_energy,"mother_energy/D");
      ana_tree->Branch("nhits",&_nhits,"nhits/I");
      ana_tree->Branch("opening_angle",&_opening_angle,"opening_angle/D");
      ana_tree->Branch("after_merging",&_after_merging,"after_merging/O");
    }

  }

  void CMergePerformance::FillClusterParamsVector(event_cluster* ev_cluster,
						  event_hit* ev_hits){

    // Now we make ClusterParamsAlgNew instance per cluster
    _clusterparams.clear();

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
	(*_clusterparams.rbegin()).SetVerbose(false);
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
					event_cluster* ev_cluster,
					event_hit* ev_hits,
					const std::vector< ::cluster::ClusterParamsAlgNew> &_clusterparams){

    //make a vector of MCShower indices 
    //McshowerLookback has a default 20MeV cut on these energies, don't have to do it here
    std::vector<UInt_t> MCShower_indices;
    for(UInt_t i=0; i < ev_mcshower->size(); ++i)
      MCShower_indices.push_back(i);

    //debug: view MCShower_indices
    /*
    std::cout<<"MCShower_indices = { ";
    for(int a = 0; a < MCShower_indices.size(); ++a)
      std::cout<<MCShower_indices.at(a)<<", ";
    std::cout<<"}"<<std::endl;
    */

    //Here we try to loop over clusters based on the _clusterparams vector, *not* ev_cluster vector,
    //because the merging algorithm returns an altered _clusterparams vector but it does *not*
    //modify the actual ev_cluster vector. 
    //Before merging, we make a vector of vectors like { {1}, {2}, {3}, ...}
    //where each entry is a vector of length 1, corresponding to the index in ev_cluster we are looping on
    //After merging, we retrieve this vector from CBookKeeper and it will look more like
    // { {1, 2}, {3}, {4, 5, 6, 7}, ...} showing which ev_cluster elements should be combined
    //That way, when we can easily combine the hits in different clusters to loop over,
    //as if the clusters were really merged

    //solution: get the bookkeeper instance and use the GetResult() function
    //this returns a vector of vector whose length = # of output clusters
    //(index number matches with the std::vector<CPAN> you get from CMergeManager::GetClusters()
    //the internal vector is the input clusters indices that were merged

    
    clus_idx_vec.clear();
    if(!after_merging){
      for(int i = 0; i < ev_cluster->size(); ++i){
	std::vector<unsigned short> temporary(1,i);
     	clus_idx_vec.push_back(temporary);
      }
    }
    else{
      clus_idx_vec = _mgr.GetBookKeeper().GetResult();
    }

    int cluster_params_idx = 0;
    //debug, viewing idx vec
    /*
    std::cout<<"after_merging is equal to "<<after_merging<<std::endl;
    std::cout<<"clus_idx_vec = { ";
    for(int a = 0; a < clus_idx_vec.size(); a++){
      std::cout<< "{ ";
      for(int b = 0; b < clus_idx_vec.at(a).size(); b++)
	std::cout<< clus_idx_vec.at(a).at(b) << ", ";
      std::cout<<" }, ";
    }
    std::cout<<" } "<<std::endl;
    */

    ///////////////////////////////////////////////
    //Loop over reconstructed clusters
    ///////////////////////////////////////////////

    //    std::cout<<"after_merging? = "<<after_merging
    //	     <<". This event has "<<clus_idx_vec.size()
    //	     <<" clusters and "<<MCShower_indices.size()
    //	     <<" viable MCShowers"<<std::endl;

    //    for(auto const i_cluster: *ev_cluster) {
    for(int outer_clus_idx=0; outer_clus_idx<clus_idx_vec.size(); ++outer_clus_idx){
      //total cluster charge
      _tot_clus_charge = 0;
      _plane = ev_cluster->at(clus_idx_vec.at(outer_clus_idx).at(0)).View();

      //charge from each MCShower object... last element is from unknown MCShower
      //or from MCShowers that were too low energy to pass to McshowerLookback
      std::vector<double> part_clus_charge(MCShower_indices.size()+1,0);

      //container to hold the fractional hit charge returned by McshowerLookback
      std::vector<float> hit_charge_frac;

      //Get the hits from each cluster and loop over them
      //if after merging, this may be hits from multiple clusters
      //combine the assiciations from all merged clusters into one vector of hit indices to loop over
      //container to hold the final list of hit indices for this(these) cluster(s)
      std::vector<unsigned short> ass_index;
      ass_index.clear();
      //loop over this(these) cluster(s) index(indices)
      for(int in_clus_idx = 0; in_clus_idx < clus_idx_vec.at(outer_clus_idx).size(); ++in_clus_idx){
	//grab the hit associations for each cluster
	const std::vector<unsigned short> tmp_ass_index(ev_cluster->at(clus_idx_vec.at(outer_clus_idx).at(in_clus_idx)).association(hit_type));
	// and concatenate it to ass_index
	ass_index.insert( ass_index.end(), tmp_ass_index.begin(), tmp_ass_index.end() );
      }


      //debug: view ass_index
      /*
      std::cout<<"for this cluster, ass_index is {";
      for(int a = 0; a < ass_index.size(); a++)
	std::cout<<ass_index.at(a)<<",";
      std::cout<< "}"<<std::endl;
      */

      ///////////////////////////////////////////////
      //Loop over hits in this cluster 
      ///////////////////////////////////////////////
      //      std::cout<<"looping over ass_index. n_hits is "<<ass_index.size()<<std::endl;
      for(auto const hit_index: ass_index){
	//	std::cout<<"hit_index is "<<hit_index<<std::endl;
	_tot_clus_charge += ev_hits->at(hit_index).Charge();

	//	std::cout<<"adding "<<ev_hits->at(hit_index).Charge()<<" to tot_clus_charge"<<std::endl;

	hit_charge_frac.clear();
	
	hit_charge_frac = 
	  _mcslb.MatchHitsAll(ev_hits->at(hit_index),
			      _simch_map,
			      _shower_idmap,
			      MCShower_indices);


	//debug
	/*
	std::cout<<"Debug: just got back hit_charge_frac as (";
	for(int j = 0; j < hit_charge_frac.size(); ++j)
	  std::cout<<hit_charge_frac.at(j)<<", ";
	std::cout<<")."<<std::endl;
	*/

	
	//sometimes mcslb returns a null vector if the reco hit couldn't be matched with an IDE
	//in this case, add 100% of this hit's charge to the "unknown" index
	if(hit_charge_frac.size() == 0)
	  part_clus_charge.back() += (1.)*ev_hits->at(hit_index).Charge();
	else
	  for(int i = 0; i < hit_charge_frac.size(); ++i)
	    part_clus_charge.at(i) += hit_charge_frac.at(i)*ev_hits->at(hit_index).Charge();


	//debug
	/*
	  std::cout<<"This hit is made up of (";
	  for(int i=0; i<hit_charge_frac.size()-1; ++i)
	  std::cout<<hit_charge_frac.at(i)<<", ";
	  std::cout<<") fractional charge from "<<
	  hit_charge_frac.size()-1<<" known MCShowers and "<<
	  (float)hit_charge_frac.back()<<" from unknown ones."<<std::endl;
	*/
	  //end debug
	
      } //end looping over hits in this cluster
      

      //debug: view part_clus_charge vector
      /*
      std::cout<<"part_clus_charge vector = { ";
      for(int a = 0; a < part_clus_charge.size(); ++a)
	std::cout<<part_clus_charge.at(a)<<", ";
      std::cout<< "}"<<std::endl;
      */

      //debug: print contents of _shower_idmap map
      //    for(std::map<UInt_t,UInt_t>::iterator it=_shower_idmap.begin(); it!=_shower_idmap.end(); ++it)
      //      std::cout<<"viewing whoer_idmap: "<<it->first << " => "<<it->second <<std::endl;
      
    
      ///////////////////////////////////////////////
      // Fill histograms/tree that need once-per-cluster filling
      ///////////////////////////////////////////////

      //calculate _nhits for ttree
      _nhits = (int)ass_index.size();

      //if _nhits is less than 15, don't fill this clusters' info into histos and ttree
      if(_nhits<15) continue;

      //purity of a cluster: highest charge fraction belonging to an MCShower
      //find the MCShower that most of this cluster belongs to & fill some TTree vars
      //don't count "unknown" showers. looks like lots of clusters are 100% pure from
      //"unknown" showers, but many "unknown" things all combine into this one object
      int dominant_MCshower_index = 0;
      _clusQfrac_over_totclusQ = 0;
      _frac_clusQ = 0;
      for(int i = 0; i < part_clus_charge.size() - 1; ++i){	
	if( (double)(part_clus_charge.at(i)/_tot_clus_charge) > _clusQfrac_over_totclusQ){
	  _clusQfrac_over_totclusQ = (double)(part_clus_charge.at(i)/_tot_clus_charge);
	  _frac_clusQ = part_clus_charge.at(i);
	  dominant_MCshower_index = i;
	}
      }
      hPurity.at(after_merging).at(_plane)->Fill(_clusQfrac_over_totclusQ);
      

      //cluster charge divided by charge of dominant MC shower
      //if dominant_index points to the "unknown" element, set ratio to -1

      if(dominant_MCshower_index == ev_mcshower->size()){
	
	_dom_MCS_Q = -1;
	
	_clusQ_over_MCQ = -1;
      }
      else{
	//total charge of the dominant MC Shower for this cluster
	_dom_MCS_Q = ev_mcshower->at(dominant_MCshower_index).Charge(_plane);

	_clusQ_over_MCQ = 
	  _tot_clus_charge / _dom_MCS_Q;

      }
      hClusQoverMCQ.at(after_merging).at(_plane)->Fill(_clusQ_over_MCQ);

      //take mother energy (ttree var) to be max of MCShower mother energies
      _mother_energy = 0;
      for(auto this_mcshow : *ev_mcshower)
	if(this_mcshow.MotherMomentum().at(3) > _mother_energy)
	  _mother_energy = this_mcshow.MotherMomentum().at(3);

      //std::cout<<"after_merging is "<<after_merging<<", outer_clus_idx is "<<outer_clus_idx<<", size of _clusterparms is "<<_clusterparams.size()<<", size of clud_idx_vec is "<<clus_idx_vec.size()<<std::endl;
      //calculate opening_angle for ttree
      //get this from the clusterparams
      _opening_angle = _clusterparams.at(cluster_params_idx).GetParams().opening_angle;
    
      //Fill ana TTree once per cluster
      if(ana_tree)
	ana_tree->Fill();

      //make sure to only increment cluster_params_idx (where in the _clusterparams vector this cluster is)
      //after ditching clusters with <15 hits
      cluster_params_idx++;

    
    }//end loop over clusters


    ///////////////////////////////////////////////
    // Fill histograms/tree that need once-per-event filling
    ///////////////////////////////////////////////
    
    ///////////
    //efficiency of clustering for the event, per plane
    ///////////
    //count how many final clusters there are in each plane, using the clus_idx_vec vector
    int n_clusters_in_plane[3] = { 0, 0, 0 };
    for (int d = 0; d < (int)clus_idx_vec.size(); ++d){
      int this_view = ev_cluster->at(clus_idx_vec.at(d).at(0)).View();
      n_clusters_in_plane[this_view]++;
    }
    //count how many MCShowers are above ~20 MeV.
    double n_viable_MCShowers = 0;
    for(auto this_mcshow : *ev_mcshower)
      if(this_mcshow.MotherMomentum().at(3) > 0.02)
	n_viable_MCShowers++; 

    //then loop over 3 planes and fill hEff with the right fraction
    for (int iplane = 0; iplane < 3; ++iplane){

      double NMCSoverNClus = 
	n_viable_MCShowers/(double)n_clusters_in_plane[iplane];
      //      std::cout<<Form("Merged yet? %o. In plane %d there were %d MCShowers and %d clusters, which makes the ratio %f\n",
      //		      after_merging,iplane,(int)MCShower_indices.size(),n_clusters_in_plane[iplane],NMCSoverNClus);
      hEff.at(after_merging).at(iplane)->Fill(NMCSoverNClus);
      
      //if there are exactly 2 MCShowers in the event, fill some pi0 relevant stuff per plane
      //this needs debugging
      double angle2D[2] = {999.,999.};
      if(n_viable_MCShowers == 2){
	int blah = 0;
	for(auto this_mcshow : *ev_mcshower)
	  if(this_mcshow.MotherMomentum().at(3) > 0.02){
	    angle2D[blah]=this_mcshow.MotherAngle2D((larlight::GEO::View_t)iplane);
	    //	    std::cout<<"blah "<<this_mcshow.MotherAngle2D((larlight::GEO::View_t)iplane)<<std::endl;
	    blah++;
	  }
	double angle2Ddiff = std::abs(angle2D[0]-angle2D[1]);
	hPi0_photonanglediff_vs_Eff.at(after_merging).at(iplane)->Fill(NMCSoverNClus,angle2Ddiff);
	//	std::cout<<"filling 2d shit with "<<NMCSoverNClus<<", "<<angle2Ddiff<<std::endl;
      }
    }//end loop over planes
    
  }//end fillFOMhistos
    

  void CMergePerformance::RunMerging(event_cluster* ev_cluster,
				     event_hit* ev_hits)
  {

    std::vector<std::vector<larutil::PxHit> > local_clusters(ev_cluster->size(),
							     std::vector<larutil::PxHit>()
							     );

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
