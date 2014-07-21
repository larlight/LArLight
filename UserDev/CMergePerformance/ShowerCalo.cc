#ifndef SHOWERCALO_CC
#define SHOWERCALO_CC

#include "ShowerCalo.hh"

namespace larlight {

  bool ShowerCalo::initialize() {

    w2cm = larutil::GeometryUtilities::GetME()->WireToCm();
    t2cm = larutil::GeometryUtilities::GetME()->TimeToCm();

    if (_debug) { std::cout << "Preparing Histograms..." << std::endl; }
    PrepareHistos();
    if (_debug) { std::cout << "Histograms Done..." << std::endl; }
    _evt_num = 0;

    Ab = 0.8;
    Wion = 23.6; // [eV]
    kB = 0.0486; // [kV/cm]
    E = 128.0; // [kV]

    //Initialize Box Model, Brick Model and LAr Properties:
    _Efield = 0.5;     //kV/cm
    _Wion   = 1000./4.237e7; //23.6 eV = 1 electron
    _rho    = 1.43;    //g/cm^3
    _beta   = 0.212;
    _alpha  = 0.93;
    //Converting from Hit Q to number of electrons
    _Qtoe_Induction = 0.0165;
    _Qtoe_Collection = 0.035;
    return true;

  }
  
  bool ShowerCalo::analyze(storage_manager* storage) {

    _evt_num += 1;

    //grab the reconstructed clusters
    larlight::event_cluster* ev_cluster = (larlight::event_cluster*)storage->get_data(_cluster_type);
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

    _mcslb.SetCutoffEnergy(_MCShower_mother_energy_cutoff);
    //fill some std::maps that are constant through the whole event
    //_shower_idmap is (G4trackid => MCShower index in ev_mcshower)
    //map does NOT include trackIDs from particles belonging to 
    //MCShowers with less than cutoff energy
    _shower_idmap.clear();
    _mcslb.FillShowerMap(ev_mcshower,_shower_idmap);

    //_simch_map is (channel => larlight::simch)
    _simch_map.clear();
    _mcslb.FillSimchMap(ev_simch,_simch_map);


    //first start with the raw fuzzyclusters and find their CPAN params
    if (_debug) { std::cout << "Finding CPAN values..." << std::endl; }
    //FillClusterParamsVector(ev_cluster,ev_hits);
    if (_debug) { std::cout << "CPAN Done..." << std::endl; }
    //Now Find best MCShower and fill histograms with their info
    if (_debug) { std::cout << "Find Best Cluster..." << std::endl; }
    std::vector< std::vector<int> > bestC = FindBestClusters(ev_mcshower, ev_cluster, ev_hits, _clusterparams);   
    if (_debug) { std::cout << "Event Analysis Done..." << std::endl; }


    for (unsigned int pl=0; pl < bestC.size(); pl++){
      //figure out if the two MCShowers have been reconstructed as the same cluster
      if ( bestC.at(pl).size() == 2 ) {
	if ( bestC.at(pl).at(0) != bestC.at(pl).at(1) ) {
	  std::vector<float> angles;
	  std::vector<float> charge;
	  for (unsigned int cl=0; cl < bestC.at(pl).size(); cl++){

	    GetHits( &(ev_cluster->at(bestC.at(pl).at(cl))), ev_hits, hit_type);
	    GetParams( _clusHits );
	    if ( _params.GetNHits() < 100 )
	      continue;

	    dEdx( _dEdx_vec, _dE_vec );

	    angles.push_back( (_params.GetParams().angle_2d)*3.14/180. );
	    charge.push_back( GetEnergy( _clusHits ) );

	    int plane = _params.Plane();
	    /*	    
	    TH1D* dedx = new TH1D( Form("Cal_ev_%d_clusterIdx_%d_plane_%d", _evt_num, bestC.at(pl).at(cl), pl),
				   "dEdx for all hits in cluster",
				   100, 0, 10000);
	    TH2D* dx   = new TH2D( Form("Dx_ev_%d_clusterIdx_%d_plane_%d", _evt_num, bestC.at(pl).at(cl), pl),
				   "dx vs dist from start point; Dist [cm]",
				   L_max*3, 0, L_max, 100, 0, 10000);
	    */
	    for (unsigned int hit=0; hit < L_max; hit++){
	      //only first few centimeters for dedx histogram
	      if ( (hit < 10) and (_dEdx_vec.at(hit) != 0) ) { 
		hdEdx.at(plane)->Fill( _dEdx_vec.at(hit) ); 
		//dedx->Fill(_dEdx_vec.at(hit) );
	      }
	      hdx.at(plane)->Fill( L_max-hit , _dEdx_vec.at(hit) );
	      //dx->Fill( L_max-hit, _dEdx_vec.at(hit) );
	    }
	    /*dx->Write();
	    dedx->Write();
	    delete dx;
	    delete dedx;*/
	  }//if two clusters have separate angles
	  if ( angles.size() == 2 ){
	    float pi0m = Pi0Mass( angles.at(0), charge.at(0), angles.at(1), charge.at(1) );
	    hPi0Mass.at(_params.Plane())->Fill(pi0m);
	  }
	}

	    //dEdx( &(ev_cluster->at(bestC.at(pl).at(cl))), ev_hits, hit_type, _dEdx_vec);


      }
    }

	

    return true;
  }


  //-------------------------------
  void ShowerCalo::PrepareHistos(){
    
    for(int i_view = 0; i_view < 3; i_view++){
      
      hCharge_toQ.push_back( new TH2D( Form("Charge vs Hit Q_%d", i_view),
				       "e- Vs Hit Q; Number of e-; Q of Hit",
				       100, 0, 100000, 100, 0, 700) );//out to 3 m

      hdEdx.push_back( new TH1D( Form("CAL_Histo_%d", i_view),
				 "dEdx for all hits in cluster; Charge",
				 100, 0, 100) );

      hdx.push_back( new TH2D( Form("Dx_Histo_%d", i_view),
			       "dx vs dist from start point; Dist [cm]",
			       1000, 0, 300, 100, 0, 100) );//out to 3 meters

      hPi0Mass.push_back( new TH1D( Form("Pi0Mass_%d", i_view),
				    "Pi0 Mass", 30, 0, 1000 ) );

      hPurityPerMCShower.push_back(
				   new TH1D(Form("hPurityPerMCShower_view%d",i_view),
					    Form("For *best* Cluster: Q in Clus from MCShower / Q of Cluster  [before merge], view %d",i_view),
					    100,-0.1,3.1)
				   );
    
      hEffPerMCShower.push_back(
				new TH1D(Form("hEffPerMCShower_view%d",i_view),
					 Form("Largest Frac of Shower Q in single Cluster, before, view %d",i_view), 
					 300,-0.1,3.1)
				);
    
      hClusQ_vs_ShowerQ.push_back(
				  new TH2D(Form("hClusQ_vs_ShowerQ_view%d",i_view),
					   Form("Cluster Q from MCShower vs MCShower Q from Hits, view %d; MCShower Q; Cluster Q", i_view),
					   30, 0, 500000,
					   30, 0, 500000)
				  );

      hClusQ_vs_ShowerMCQ.push_back(
				    new TH2D(Form("hClusQ_vs_ShowerMCQ_view%d",i_view),
					     Form("Cluster Q from MCShower vs MCShower MC Q from Hits, view %d; MCShower MC Q; Cluster Q", i_view),
					     100, 0, 500000,
					     100, 0, 500000)
				    );

      hClusE_vs_ShowerMCE.push_back(
				    new TH2D(Form("hClusE_vs_ShowerMCE_view%d",i_view),
					     Form("Cluster E from MCShower vs MCShower MotherMomentum, view %d; MCShower Energy [MeV]; Cluster Energy [MeV]", i_view),
					     100, 0, 1000,
					     100, 0, 1000)
				    );

      hClusTOTQ_vs_ShowerQ.push_back(
				     new TH2D(Form("hClusTOTQ_vs_ShowerQ_view%d",i_view),
					      Form("TOT Cluster Q vs MCShower Q from Hits, view %d; MCShower Q; Cluster Q TOT", i_view),
					      100, 0, 500000,
					      100, 0, 500000)
				     );


      hClusPur_vs_ShowerQ.push_back(
				  new TH2D(Form("hClusPur_vs_ShowerQ_view%d",i_view),
					   Form("Cluster Purity vs MCShower Q from Hits, view %d; MCShower Q; Cluster Purity", i_view),
					   100, 0, 500000,
					   100, 0, 1)
				  );

      hClusEff_vs_ShowerQ.push_back(
				  new TH2D(Form("hClusEff_vs_ShowerQ_view%d",i_view),
					   Form("Cluster Efficiency vs MCShower Q from Hits, view %d; MCShower Q; Cluster Efficiency", i_view),
					   100, 0, 500000,
					   100, 0, 1)
				  );
    }

  }

  bool ShowerCalo::finalize() {

    if(_fout) { 
      _fout->cd(); 
      for(int nview = 0; nview < 3; ++nview){
	hCharge_toQ.at(nview)->Write();
	hdEdx.at(nview)->Write();
	hdx.at(nview)->Write();
	hPi0Mass.at(nview)->Write();
	hEffPerMCShower.at(nview)->Write();
	hPurityPerMCShower.at(nview)->Write();
	hClusQ_vs_ShowerQ.at(nview)->Write();
	hClusQ_vs_ShowerMCQ.at(nview)->Write();
	hClusE_vs_ShowerMCE.at(nview)->Write();
	hClusTOTQ_vs_ShowerQ.at(nview)->Write();
	hClusPur_vs_ShowerQ.at(nview)->Write();
	hClusEff_vs_ShowerQ.at(nview)->Write();

	delete hCharge_toQ.at(nview);
	delete hdEdx.at(nview);
	delete hdx.at(nview);
	delete hPi0Mass.at(nview);
	delete hPurityPerMCShower.at(nview);
	delete hEffPerMCShower.at(nview);
	delete hClusQ_vs_ShowerQ.at(nview);
	delete hClusQ_vs_ShowerMCQ.at(nview);
	delete hClusE_vs_ShowerMCE.at(nview);
	delete hClusTOTQ_vs_ShowerQ.at(nview);
	delete hClusPur_vs_ShowerQ.at(nview);
	delete hClusEff_vs_ShowerQ.at(nview);
      }

    /*
      if(ana_tree){
	ana_tree->Write();
	delete ana_tree;
      }
    */
    }
    
    else 
      print(MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
    
    return true;
  }

  
  //--------------------------------------------------
  void ShowerCalo::GetHits(cluster* clus, event_hit* ev_hits,
			    larlight::DATA::DATA_TYPE hit_type){
    
    _clusHits.clear();
    //Get the hits from each cluster and loop over them
    std::vector<unsigned short> ass_index(clus->association(hit_type));
    // Compute cluster parameters (ClusterParamsAlgNew) and store (_clusterparams)
    
    _clusHits.reserve(ass_index.size());
    const UChar_t plane = larutil::Geometry::GetME()->ChannelToPlane(ev_hits->at((*ass_index.begin())).Channel());
    
    for(auto const index : ass_index) {	
      larutil::PxHit h;
      h.w = ev_hits->at(index).Wire() * w2cm;
      h.t = ev_hits->at(index).PeakTime() * t2cm;
      h.plane = plane;
      h.charge = ev_hits->at(index).Charge();
      _clusHits.push_back(h);
    }

  }

  //--------------------------------------------------
  float ShowerCalo::GetEnergy( std::vector<larutil::PxHit> clusHits ){

    float E=0;
    float _Qtoe = 0;
    //look at plane of first hit to find out if Induction or Collection
    if ( _clusHits.at(0).plane == 2 )
      _Qtoe = _Qtoe_Collection;
    if ( (_clusHits.at(0).plane == 0) or (_clusHits.at(0).plane == 1) )
      _Qtoe = _Qtoe_Induction;

    for ( unsigned int hit = 0; hit < clusHits.size(); hit++)
      E += (clusHits.at(hit).charge/_Qtoe)*_Wion;

    return E;
  }

  //--------------------------------------------------
  void ShowerCalo::GetParams( std::vector<larutil::PxHit> clusHits ){

    ::cluster::ClusterParamsAlgNew params = ::cluster::ClusterParamsAlgNew();// = ClusterParamsAlgNew(tmp_hits);  
    _params.SetVerbose(false);
    _params.SetHits(clusHits);
    _params.FillParams(true, true, true, true, true, true);

  }

  //--------------------------------------------------
  float ShowerCalo::dEdxBox( float dQdx ){

    if (_debug) { std::cout << "Initial dQdx: " << dQdx << std::endl; }
    
    double beta = ( _beta / _rho * _Efield );
    
    double dEdx = ( exp(beta*_Wion*dQdx) - _alpha ) / beta;
    
    if (_debug) { std::cout << "Converted dEdx: " << dEdx << std::endl; }

    return dEdx;
  }


  //--------------------------------------------------
  float ShowerCalo::dQdEBirks( float dQdx ){

    float dEdx = dQdx / ( Ab/Wion - (kB*dQdx)/E );
    return dEdx;
    
  }


  //Get PI0 Mass from photon directions and momenta
  float ShowerCalo::Pi0Mass( float angle1, float Q1, float angle2, float Q2){

    float m = 0;
    _debug = true;
    if ( _debug ) { std::cout << "Photon 1: angle: " << angle1 << " momentum: " << Q1 << std::endl; }
    if ( _debug ) { std::cout << "Photon 2: angle: " << angle2 << " momentum: " << Q2 << std::endl; }
    float totMomentumAngle = atan( ( Q1*sin(angle1)+Q2*sin(angle2) ) / ( Q1*cos(angle1)+Q2*sin(angle2) ) );
    if ( _debug ) { std::cout << "Tot Angle: " << totMomentumAngle << std::endl; }
    angle1 = angle1-totMomentumAngle;
    angle2 = angle2-totMomentumAngle;
    if ( _debug ) { std::cout << "New Angles...angle1: " << angle1 << " angle2: " << angle2 << std::endl; }

    m = sqrt( 2*Q1*Q2*(1-cos(angle1)*cos(angle2)-sin(angle1)*sin(angle2) ) );

    if ( _debug ) { std::cout << "Pion Mass: " << m << " [MeV]" << std::endl; }

    _debug = false;

    return m;

  }

  //--------------------------------------------------
  //Take a cluster, its hits and make a vector of dEdx
  //for the hits in that cluster up to some distance
  void  ShowerCalo::dEdx( std::vector<float> & dEdx_vec,
			   std::vector<float> & dE_vec ){

    dEdx_vec.clear();
    dEdx_vec.resize(1000);//reserve up to 3 meters (1000 3 mm points)
    dE_vec.clear();
    dE_vec.resize(1000);//reserve up to 3 meters (1000 3 mm points)

    //cluster's direction:
    double direction = (_params.GetParams().angle_2d)*3.14/180.;
    double cosdir = cos(direction);
    if ( cosdir < 0 ) { cosdir = - cosdir; }
    double pitch = larutil::Geometry::GetME()->WirePitch(0,1,0);
    if (_debug) { std::cout << "Direction: " << direction << std::endl; }
    double start_w = _params.GetParams().start_point.w;
    double start_t = _params.GetParams().start_point.t;

    //keep track of maximum distance
    L_max = 0;

    //Now scan down the cluster from the start point to calculate the 
    for( unsigned int hit=0; hit < _clusHits.size() ; hit++ ) {	
      double dist = sqrt( (_clusHits.at(hit).w - start_w)*(_clusHits.at(hit).w - start_w) +
			  (_clusHits.at(hit).t - start_t)*(_clusHits.at(hit).t - start_t) );
      //tick: = (int)( distance/0.3 cm)
      int tick = (int)( dist/0.3 );
      if ( tick > L_max ) { L_max = tick; }
      if ( _clusHits.at(hit).plane == 2 ){
	dE_vec.at(tick) += ((_clusHits.at(hit).charge)/_Qtoe_Collection)*_Wion;
	dEdx_vec.at(tick) += ( ((_clusHits.at(hit).charge)/_Qtoe_Collection) / (pitch*cosdir) ) * 3.333;
      }
      if ( (_clusHits.at(hit).plane == 0) or
	   (_clusHits.at(hit).plane == 1) ){
	dE_vec.at(tick) += ((_clusHits.at(hit).charge)/_Qtoe_Induction)*_Wion;
	//multiply by 3.33 to account for the fact that we are going from ticks of 0.3 cm to dEdx in 1 cm units
	dEdx_vec.at(tick) += ( ((_clusHits.at(hit).charge)/_Qtoe_Induction) / (pitch*cosdir) ) * 3.333;
      }
    }

    //now convert dEdx_vec from dQdx to dEdx
    for ( unsigned int dx=0; dx < _dEdx_vec.size(); dx++)
      if ( dEdx_vec.at(dx) > 0 )
	dEdx_vec.at(dx) = dEdxBox( dEdx_vec.at(dx) );


    return;
  }


  //--------------------------------------------------
  //Take a cluster, its hits and make a vector of dEdx
  //for the hits in that cluster up to some distance
  void  ShowerCalo::dEdx( cluster* clus, event_hit* ev_hits,
			   larlight::DATA::DATA_TYPE hit_type,
			   std::vector<float> & dEdx_vec ){

    _dEdx_vec.clear();
    _dEdx_vec.resize(1000);//reserve up to 3 meters (1000 3 mm points)
    //Make clusterparams for this cluster
    //FillClusterParams( clus, ev_hits);
    //Get the hits from each cluster and loop over them
    std::vector<unsigned short> ass_index(clus->association(hit_type));
    // Compute cluster parameters (ClusterParamsAlgNew) and store (_clusterparams)
    std::vector<larutil::PxHit> tmp_hits;
    tmp_hits.reserve(ass_index.size());
    const UChar_t plane = larutil::Geometry::GetME()->ChannelToPlane(ev_hits->at((*ass_index.begin())).Channel());

    for(auto const index : ass_index) {	
      larutil::PxHit h;
      h.w = ev_hits->at(index).Wire() * w2cm;
      h.t = ev_hits->at(index).PeakTime() * t2cm;
      h.plane = plane;
      h.charge = ev_hits->at(index).Charge();
      tmp_hits.push_back(h);
    }

    ::cluster::ClusterParamsAlgNew params = ::cluster::ClusterParamsAlgNew();// = ClusterParamsAlgNew(tmp_hits);  
    params.SetVerbose(false);
    params.SetHits(tmp_hits);
    params.FillParams();

    //cluster's direction:
    double direction = params.GetParams().opening_angle;
    double cosdir = cos(direction);
    double pitch = larutil::Geometry::GetME()->WirePitch(0,1,0);
    if (_debug) { std::cout << "Direction: " << direction << std::endl; }
    double start_w = params.GetParams().start_point.w;
    double start_t = params.GetParams().start_point.t;

    //Now scan down the cluster from the start point to calculate the 
    for( unsigned int hit=0; hit < tmp_hits.size() ; hit++ ) {	
      double dist = sqrt( (tmp_hits.at(hit).w - start_w)*(tmp_hits.at(hit).w - start_w) +
			  (tmp_hits.at(hit).t - start_t)*(tmp_hits.at(hit).t - start_t) );
      //tick: = (int)( distance/0.3 cm)
      int tick = (int)( dist/0.3 );
      dEdx_vec.at(tick) += ( tmp_hits.at(hit).charge / (pitch*cosdir) );
    }

    return;
  }
  
  //------------------------------------------------------------------------
  void ShowerCalo::FillClusterParamsVector(event_cluster* ev_cluster,
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
  
  

  //--------------------------------------------------------------
  std::vector< std::vector<int> > ShowerCalo::FindBestClusters(event_mcshower* ev_mcshower,
								event_cluster* ev_cluster,
								   event_hit* ev_hits,
								const std::vector< ::cluster::ClusterParamsAlgNew> &_clusterparams){
    
    //make a vector of MCShower indices 
    //McshowerLookback has a default 20MeV cut on these energies, don't have to do it here
    _MCShower_indices.clear();
    for(UInt_t i=0; i < ev_mcshower->size(); ++i){
      _MCShower_indices.push_back(i);
    }
    if (_debug) { std::cout << "Number of MCShowers in event: " << _MCShower_indices.size() <<  std::endl; }
    
    clus_idx_vec.clear();
    for(UInt_t i = 0; i < ev_cluster->size(); ++i){
      std::vector<unsigned short> temporary(1,i);
      clus_idx_vec.push_back(temporary);
    }

    if (_debug) { std::cout << "Prepare vectors for best shower info: " << std::endl; }

    int cluster_params_idx = 0;

    //Hold max fraction of each MCShower's charge in a single cluster
    std::vector< std::vector<double> >  MCShower_best_clus(3, std::vector<double>(_MCShower_indices.size()+1,0) );
    //Try and Keep track of Each MCShower's charge across the clusters
    std::vector< std::vector<double> >  MCShower_Qs(3, std::vector<double>(_MCShower_indices.size()+1,0) );
    //Keep track of total Q of the cluster that has largest fract of MCShower Q
    std::vector< std::vector<double> > BestClus_Qs(3, std::vector<double>(_MCShower_indices.size()+1,0) );
    //Keep track of the index in ev_clusters for best cluster
    std::vector< std::vector<int> > BestClus_Idx(3, std::vector<int>(_MCShower_indices.size(),0) );


    //--------------------------------
    //Loop over reconstructed clusters

    if (_debug) { std::cout << "Start Looping over clusters..." << std::endl; }
    if (_debug) { std::cout << "Number of Clusters: " << clus_idx_vec.size() << std::endl; }

    for(unsigned int outer_clus_idx=0; outer_clus_idx<clus_idx_vec.size(); ++outer_clus_idx){

      //holders for cluster Q, Q from MCShowers and plane info
      _tot_clus_charge = 0;
      _tot_clus_charge_fromKnownMCS = 0;
      _plane = ev_cluster->at(clus_idx_vec.at(outer_clus_idx).at(0)).View();

      //charge from each MCShower object... last element is from unknown MCShower
      //or from MCShowers that were too low energy to pass to McshowerLookback
      //Looks like this: [ Qfrom_MCShower_1, Qfrom_MCShower_2, ..., Qfrom_UnknownMCShower ]
      std::vector<double> part_clus_charge(_MCShower_indices.size()+1,0);

      //container to hold the fractional hit charge returned by McshowerLookback
      std::vector<float> hit_charge_frac;

      //Get the hits from each cluster and loop over them
      //if after merging, this may be hits from multiple clusters
      //combine the assiciations from all merged clusters into one vector of hit indices to loop over
      //container to hold the final list of hit indices for this(these) cluster(s)
      std::vector<unsigned short> ass_index;
      ass_index.clear();
      //loop over this(these) cluster(s) index(indices)
      for(unsigned int in_clus_idx = 0; in_clus_idx < clus_idx_vec.at(outer_clus_idx).size(); ++in_clus_idx){
	//grab the hit associations for each cluster
	const std::vector<unsigned short> tmp_ass_index(ev_cluster->at(clus_idx_vec.at(outer_clus_idx).at(in_clus_idx)).association(hit_type));
	// and concatenate it to ass_index
	ass_index.insert( ass_index.end(), tmp_ass_index.begin(), tmp_ass_index.end() );
      }


      //-----------------------------
      //Loop over hits in this cluster 
    if (_debug) { std::cout << "Loop over hits in cluster..." << std::endl; }
    if (_debug) { std::cout << "Number of Hits in cluster: " << ass_index.size() << std::endl; }
      for(auto const hit_index: ass_index){
	
	_tot_clus_charge += ev_hits->at(hit_index).Charge();
	
	hit_charge_frac.clear();

	//Using MCShower Lookback to determine, for Q of hit, what fraction of it comes from each MCShower
	//last element in vector is assigned to unknonw MCShower
	hit_charge_frac =  _mcslb.MatchHitsAll(ev_hits->at(hit_index),
					       _simch_map,
					       _shower_idmap,
					       _MCShower_indices);


	//ADDITION: MAP Charge on Hit to actual Charge:
	auto simch_iter = _simch_map.find(ev_hits->at(hit_index).Channel());
	float tot_ides_charge = 0;
	std::vector<larlight::ide> matchedides((*simch_iter).second.TrackIDsAndEnergies(ev_hits->at(hit_index).StartTime(),
											ev_hits->at(hit_index).EndTime()));
	if ( matchedides.size() > 0 ){
	  for(auto const &this_ide : matchedides){
	    tot_ides_charge+=this_ide.numElectrons;
	  }
	  hCharge_toQ.at(_plane)->Fill(tot_ides_charge,
				       ev_hits->at(hit_index).Charge() );
	}
	//sometimes mcslb returns a null vector if the reco hit couldn't be matched with an IDE at all... this rarely occurs
	//in this case, add 100% of this hit's charge to the "unknown" index
	if(hit_charge_frac.size() == 0){
	  part_clus_charge.back() += (1.)*ev_hits->at(hit_index).Charge();
	}
	else{
	  //mcslb has found an IDE, whether it be in an MCShower, or unknown
	  for(unsigned int i = 0; i < hit_charge_frac.size(); ++i)
	    part_clus_charge.at(i) += hit_charge_frac.at(i)*ev_hits->at(hit_index).Charge();
	  //count total charge only from known MCShowers (hence -1)
	  for(unsigned int i = 0; i < hit_charge_frac.size() - 1; ++i)
	    _tot_clus_charge_fromKnownMCS += hit_charge_frac.at(i)*ev_hits->at(hit_index).Charge();
	}  
	  
      } //end looping over hits in this cluster
    if (_debug) { std::cout << "End loop over hits..." << std::endl; }	
      
      //--------------------------------------------------------
      // Fill histograms/tree that need once-per-cluster filling
      
	//if all of the charge in the cluster is from unknown MCShower, skip this cluster   
      if(_tot_clus_charge_fromKnownMCS == 0) continue;
      
      //calculate _nhits for ttree
      _nhits = (int)ass_index.size();
      
      //if _nhits is less than 15, don't fill this clusters' info into histos and ttree
      if(_nhits<15) continue;
      

      //Loop over the Cluster's MCShower-components
      //Add up the charge from each component to find the total MCShower's Q
      //Find the cluster that contains most of the MCShower's Q
      //ignore unknown MCShowers
      for (unsigned int i=0; i < part_clus_charge.size() - 1; i++){
	//std::cout << "Cluster's Charge from MC Shower " << i << " : " << part_clus_charge.at(i) << std::endl;
	MCShower_Qs.at(_plane).at(i) += part_clus_charge.at(i);
	if ( part_clus_charge.at(i) > MCShower_best_clus.at(_plane).at(i) ) {
	  //keep track of this cluster
	  BestClus_Idx.at(_plane).at(i) = outer_clus_idx;
	  MCShower_best_clus.at(_plane).at(i) = part_clus_charge.at(i);
	  BestClus_Qs.at(_plane).at(i) = (double)(part_clus_charge.at(i)/_tot_clus_charge_fromKnownMCS);
	}
      }
      
      
      _clusQfrac_from_unknown = part_clus_charge.back()/_tot_clus_charge;
      
      //get this from the clusterparams
      //_opening_angle = _clusterparams.at(cluster_params_idx).GetParams().opening_angle;
      
      //Fill ana TTree once per cluster
      //if(ana_tree)
      //	ana_tree->Fill();
      
      //make sure to only increment cluster_params_idx (where in the _clusterparams vector this cluster is)
      //after ditching clusters with <15 hits
      cluster_params_idx++;
      
      
    }//end loop over clusters
    if (_debug) { std::cout << "End loop over Clusters..." << std::endl; }    
    
    //---------------------------------
    // Per MCShower Efficiency & Purity

    if (_debug) { std::cout << "Fill histo values..." << std::endl; }    
    for (int iplane = 0; iplane < 3; ++iplane){
      for (unsigned int i=0; i<_MCShower_indices.size(); i++){
	if (_debug) { std::cout << "Charge of MCShower in Cluster - max: " << MCShower_best_clus.at(iplane).at(i) << std::endl; }
	if (_debug) { std::cout << "Charge of MCShower Total: " << MCShower_Qs.at(iplane).at(i) << std::endl; }
	MCShower_best_clus.at(iplane).at(i) /= MCShower_Qs.at(iplane).at(i);
	if (_debug) { std::cout << "Fraction of MCShower's Q in that Cluster: " << MCShower_best_clus.at(iplane).at(i) << std::endl; }
	//BestClus_Qs_pl0.at(i) /= MCShower_Qs_pl0.at(i);
	hEffPerMCShower.at(iplane)->Fill(MCShower_best_clus.at(iplane).at(i));
	//std::cout << "Fraction of Cluster's Q made up by the Q of that MCShower " << BestClus_Qs_pl0.at(i) << std::endl;      
	hPurityPerMCShower.at(iplane)->Fill(BestClus_Qs.at(iplane).at(i));
	hClusQ_vs_ShowerMCQ.at(iplane)->Fill( ev_mcshower->at(i).Charge(larutil::Geometry::GetME()->PlaneToView(iplane)), 
					      MCShower_best_clus.at(iplane).at(i)*MCShower_Qs.at(iplane).at(i) );
	if ( iplane == 2 )
	  hClusE_vs_ShowerMCE.at(iplane)->Fill( ev_mcshower->at(i).MotherMomentum().at(0)*1000, 
						(((MCShower_best_clus.at(iplane).at(i)*MCShower_Qs.at(iplane).at(i))/BestClus_Qs.at(iplane).at(i))/_Qtoe_Collection)*_Wion );
	if ( (iplane == 0) or (iplane == 1) )
	  hClusE_vs_ShowerMCE.at(iplane)->Fill( ev_mcshower->at(i).MotherMomentum().at(0)*1000, 
						(((MCShower_best_clus.at(iplane).at(i)*MCShower_Qs.at(iplane).at(i))/BestClus_Qs.at(iplane).at(i))/_Qtoe_Induction)*_Wion );
	hClusQ_vs_ShowerQ.at(iplane)->Fill( MCShower_Qs.at(iplane).at(i),
					    MCShower_best_clus.at(iplane).at(i)*MCShower_Qs.at(iplane).at(i) );
	hClusTOTQ_vs_ShowerQ.at(iplane)->Fill( MCShower_Qs.at(iplane).at(i),
					       (MCShower_best_clus.at(iplane).at(i)*MCShower_Qs.at(iplane).at(i))/BestClus_Qs.at(iplane).at(i) );
	hClusPur_vs_ShowerQ.at(iplane)->Fill( MCShower_Qs.at(iplane).at(i), BestClus_Qs.at(iplane).at(i) );
	hClusEff_vs_ShowerQ.at(iplane)->Fill( MCShower_Qs.at(iplane).at(i), MCShower_best_clus.at(iplane).at(i) );
      }
    }
  
    return BestClus_Idx;
  }//end of function  
  
}
#endif
