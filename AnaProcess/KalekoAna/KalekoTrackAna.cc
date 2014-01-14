#ifndef KALEKOTRACKANA_CC
#define KALEKOTRACKANA_CC

#include "KalekoTrackAna.hh"

namespace kaleko{
  
  KalekoTrackAna::KalekoTrackAna(){
    //
    // Constructor, clears histograms, clears tree
    //
    
    RecoTrackAngleHist = 0;
    MCTrackAngleHist = 0;
    MCTrackBoxAngleHist = 0;
    NRecoTracksHist = 0;
    DataTree = 0;
    
  };
  
  
  int KalekoTrackAna::EventLoop(std::string filename, const int MCTheta, const int max_evts, const int n_bins_histo){
    
    //
    // A simple routine to read a data file and perform an event loop.
    // This is a test routine for storage_manager class which interacts
    // decoder output root file. 
    //
    
    larlight::storage_manager my_storage;
    
    // If you wish, change the message level to DEBUG.
    
    my_storage.set_verbosity(larlight::MSG::DEBUG);
    
    // Step 0: Set I/O mode: we are reading in, so "READ"
    my_storage.set_io_mode(larlight::storage_manager::READ);
    
    // Step 0.5: Specify data type to readout 
    // this is actually redundant, all set to "true" by default, 
    // better to set ones you don't want to false
    // DATA:: types found in $MAKE_TOP_DIR/AnaProcess/Base/constants.hh
    //  my_storage.set_data_to_read(DATA::Kalman3DSPS);
    
    // Step 1: Add input file from user input
    my_storage.add_in_filename(filename);
    
    // Step 1.5 make sure you're looking in the "scanner" directory
    my_storage.set_in_rootdir("scanner");
    
    // Step 2: Open a file.
    my_storage.open();
    
    // Step 3: Check if a file is opened.
    if(!my_storage.is_open()) {
      std::cerr << "File open failed!" << std::endl;
      return 0;
    }
    
    // Step 4: Check if it's ready to perform I/O
    if(!my_storage.is_ready_io()) {
      std::cerr << "I/O preparation failed!" << std::endl;
    }
    
    //Prepare the data tree
    PrepareDataTree();
    
    //Initialize a few variables
    //  max_mydMCBoxTheta = min_mydMCBoxTheta = -1.;
    max_mydRecoTrackAngle = min_mydRecoTrackAngle = -1.;
    max_nRecoTracks = -1;
    nRecoTracks = -1;
    double track_xmin = 0, track_xmax = 0,
      track_ymin = 0, track_ymax = 0,
      track_zmin = 0, track_zmax = 0;
    
    //count how many times there are no MC points inside of a reconstructed "box"
    int badboxcounter = 0;
    
    // Loop over our events  
    int evt_counter = 0;
    
    while(my_storage.next_event() && (evt_counter < max_evts || max_evts == -1)){
      
      if(evt_counter%100==0) std::cout<<"Event counter = "<<evt_counter<<std::endl;
      //std::cout<<"Event counter = "<<evt_counter<<std::endl;


      //reset the vectors that are going into my tree
      mydRecoTrackAngle.clear();
      myRecoTrackAngle.clear();
      myAverageRecoTrackAngle.clear();
      myRecoStartingZ.clear();
      mydMCBoxTheta.clear();
      nTrackPoints.clear();
      E_dEdx_pair.clear();
      
      //initialize track lengths to zero before each track
      crude_reco_tracklengths.clear();
      crude_MC_tracklengths.clear();
      straightline_reco_tracklengths.clear();
      
      //Reconstructed objects stuff ... fDataType doesn't work now, idfk why... think it's a problem w/ root on my laptop
      //    event_track* my_event_track = (event_track*)(my_storage.get_data(fDataType));
      larlight::event_track* my_event_track = (larlight::event_track*)(my_storage.get_data(larlight::DATA::Kalman3DSPS));
      
      if(!my_event_track){
	std::cout<<"DID NOT FIND SPECIFIED DATA PRODUCT!"<<std::endl;
	break;
      }
      
      else{    
	myEventNumber = my_event_track->event_id();
	
	//spacepoint stuff
	larlight::event_sps* my_event_sps = (larlight::event_sps*)(my_storage.get_data(larlight::DATA::SpacePoint));
	
	//MC stuff
	larlight::event_mc* my_event_mc = (larlight::event_mc*)(my_storage.get_data(larlight::DATA::MCTruth));
	
	//only loop over this event if mc object AND track reco object exists
	//note, it still enters loop even if the reco object is an empty vector
	//(IE no tracks were reconstructed). This is good.
	if(!my_event_track || !my_event_mc) continue;
	
	else{
	  
	  nSpacePoints = (int)my_event_sps->size();
	  
	  //finding max # of reconstructed tracks (over all events) for histogram boundaries
	  if(max_nRecoTracks < (int)my_event_track->size()) max_nRecoTracks = (int)my_event_track->size();
	  
	  //some of the DataTree variables
	  myMCTheta = (double)MCTheta;
	  nRecoTracks = (int)my_event_track->size();
	  
	  //loop over all of the reconstructed tracks in the event
	  for(size_t i=0; i<my_event_track->size(); i++){
	    
	    nTrackPoints.push_back( (int)my_event_track->at(i).n_point() );
	    
	    //initial guess for ranges, will be altered by get_axis_range()
	    track_xmin = track_xmax = my_event_track->at(i).vertex_at(0).X();
	    track_ymin = track_ymax = my_event_track->at(i).vertex_at(0).Y();
	    track_zmin = track_zmax = my_event_track->at(i).vertex_at(0).Z();
	    //updates the track_xmin, etc variables to be true mins and maxes
	    //(min and max for this reconstructed track)
	    my_event_track->at(i).get_axis_range(track_xmax, track_xmin,
					  track_ymax, track_ymin,
					  track_zmax, track_zmin);
	    
	    //	  std::cout<<"in this track there were "<<my_event_track->at(i).n_point()<<" reco points, and "<<my_event_mc->at(0).step_vertex().size()<<" MC points."<<std::endl;	  
	    
	    double tmplength = 0;
	    //for each track, loop over the reconstructed points in it
	    for(size_t j=0; j < my_event_track->at(i).n_point()-1; j++){
	      //	    std::cout<<Form("Reco point: (%f,%f,%f)\n",
	      //			    my_event_track->at(i).vertex_at(j).X(),
	      //			    my_event_track->at(i).vertex_at(j).Y(),
	      //			    my_event_track->at(i).vertex_at(j).Z());
	      tmplength += 
		(my_event_track->at(i).vertex_at(j) - my_event_track->at(i).vertex_at(j+1)).Mag();
	    }//end loop over points in individual track
	    crude_reco_tracklengths.push_back(tmplength);
	    double straightline_reco_tkln = (my_event_track->at(i).vertex_at(0)-
					     my_event_track->at(i).vertex_at(my_event_track->at(i).n_point()-1)).Mag();
	    straightline_reco_tracklengths.push_back( straightline_reco_tkln );
	    //	  std::cout<<"straight line reco track length = "<< straightline_reco_tkln << std::endl;
	    //	  std::cout<<"crude reco track length = "<< tmplength << std::endl;
	    //now loop over the MC points and calculate a MC tracklength
	    //loop over each MC step the parent particle takes
	    bool HaventFilledHistoYet = true;
	    
	    //initialize this to something huge so it doesn't mimic something real
	    //if there is a bug in the code or something
	    double dMCBoxTheta_i = -9999.;
	    
	    double energy_lost = -1.;
	    double step_length = -1.;
	    double de_dx = -1.;
	    
	    tmplength = 0;
	    for(size_t j=0; j < my_event_mc->at(0).step_vertex().size()-1; j++){
	      TVector3 vtx_j = my_event_mc->at(0).step_vertex().at(j);	
	      TVector3 vtx_jp1 = my_event_mc->at(0).step_vertex().at(j+1);
	      TVector3 mom_j = my_event_mc->at(0).step_momentum().at(j);
	      TVector3 mom_jp1 = my_event_mc->at(0).step_momentum().at(j+1);
	      
	      //only add to MC tracklength, calculate dedx, etc
	      //only if both points j and j+1 of muon are within the detector volume
	      if( UtilFunctions().IsInDetectorVolume(vtx_j) && UtilFunctions().IsInDetectorVolume(vtx_jp1) ){
		tmplength += ( vtx_jp1 - vtx_j ).Mag();
		
		//	    std::cout<<Form("MC Point: (%f,%f,%f)\n",vtx_j.X(),vtx_j.Y(),vtx_j.Z());
		//	    std::cout<<Form("MC momentum: (%f,%f,%f)\n",mom_j.X(),mom_j.Y(),mom_j.Z());
		
		energy_lost = mom_jp1.Mag() - mom_j.Mag();
		step_length = (vtx_jp1 - vtx_j).Mag();
		de_dx = -energy_lost/step_length;
		
		E_dEdx_pair.push_back(std::make_pair(mom_jp1.Mag(),de_dx));
		
		//if the current MC point is inside of the "reconstructed" box
		if(vtx_j.X() > track_xmin && vtx_j.X() < track_xmax &&
		   vtx_j.Y() > track_ymin && vtx_j.Y() < track_ymax &&
		   vtx_j.Z() > track_zmin && vtx_j.Z() < track_zmax){
		  
		  //only store the angle for the "first" point inside the box
		  //(safe to assume I'm looping from start to finish?)
		  if(HaventFilledHistoYet){
		    dMCBoxTheta_i = (mom_j.Theta() * UtilFunctions().DegreesPerRadian) - MCTheta;
		    mydMCBoxTheta.push_back(dMCBoxTheta_i);
		    HaventFilledHistoYet=false;
		  }
		  
		} //end if step vertex is inside of reconstructed track's box limits
	      }//end requirement of muon being inside the detector volume
	    } // end looping over each step the parent particle takes
	    crude_MC_tracklengths.push_back(tmplength);
	    //	  std::cout<<"meanwhile, MC tracklength is calculated as "<<tmplength<<std::endl;
	    
	    //if you want to look at reco track angle b/t first two points...
	    //	  myRecoTrackAngle.push_back(my_event_track->at(i).direction_at(0).Theta() * UtilFunctions().DegreesPerRadian);
	    //if you want to look @ length-weighted average reco track angle
	    myRecoTrackAngle.push_back( UtilFunctions().CalculateWeightedAvgTheta( my_event_track->at(i) ) 
					* UtilFunctions().DegreesPerRadian );
	    myRecoStartingZ.push_back(my_event_track->at(i).vertex_at(0).Z());
	    
	    //do i want to plot difference from the muon-generated angle?
	    //double dRecoTrackAngle_i = (my_event_track->at(i).direction_at(0).Theta()
	    //* UtilFunctions().DegreesPerRadian ) - MCTheta;
	    
	    //or, difference from the angle of muon as it enters the reconstructed "box"?
	    //	  double dRecoTrackAngle_i = (my_event_track->at(i).direction_at(0).Theta()
	    //				      * UtilFunctions().DegreesPerRadian ) - (dMCBoxTheta_i+MCTheta);
	    
	    
	    //and, do i want to look at weighted average track angle? or direction from first point? this is weighted:
	    double dRecoTrackAngle_i = (UtilFunctions().CalculateWeightedAvgTheta( my_event_track->at(i) ) 
					* UtilFunctions().DegreesPerRadian) - MCTheta;
	    
	    //temp: calculate average reco track angle and push it back
	    double avgang=0;
	    for(size_t j=0; j<my_event_track->at(i).n_point()-1; j++)
	      avgang+=my_event_track->at(i).direction_at(j).Theta();
	    avgang/=(my_event_track->at(i).n_point()-1);
	    myAverageRecoTrackAngle.push_back(avgang*UtilFunctions().DegreesPerRadian);
	    
	    
	    //if there are no MC points inside of the reconstructed box, then
	    //dRecoTrackAngle_i is something like 9999 (it's initialized value)
	    //for now, only push_back if dRecoTrackAngle_i is within [-180,180]
	    //should fix this for real later... not sure how though?
	    //this effect is maximized at 0 and 90 degree generated tracks, because
	    //even if the track is reconstructed well, the "box" is very long and skinny
	    //(tall and skinny, for 90), so it is less likely any MC points will fall
	    //inside of the box
	    if(dRecoTrackAngle_i > -181 && dRecoTrackAngle_i < 181){
	      mydRecoTrackAngle.push_back( dRecoTrackAngle_i );
	      
	      //check to see if I need to re-define max and min histogram range
	      if(max_mydRecoTrackAngle == -1 || max_mydRecoTrackAngle < dRecoTrackAngle_i)
		max_mydRecoTrackAngle = dRecoTrackAngle_i;
	      
	      if(min_mydRecoTrackAngle == -1 || min_mydRecoTrackAngle > dRecoTrackAngle_i)
		min_mydRecoTrackAngle = dRecoTrackAngle_i;
	      
	    }//end if dRecoTrackAngle is reasonable, between -180 and 180
	    else
	      badboxcounter++;
	    
	  }//end loop over each reconstructed track
	  
	} // end else
	
	if( max_evts != -1 ) evt_counter++;
	
	/*
	  if((int)crude_reco_tracklengths.size() > 1) {
	  std::cout << Form("%d tracks. Angle = %d, evt = %d",(int)crude_reco_tracklengths.size(),MCTheta,my_event_track->event_id())<<std::endl;
	  std::cout<<Form("tracklengths: ");
	  for(size_t jason = 0; jason < crude_MC_tracklengths.size(); jason++)
	  std::cout<<Form("MC = %f, ",crude_MC_tracklengths[jason]);
	  for(size_t david = 0; david < crude_reco_tracklengths.size(); david++)
	  std::cout<<Form("reco = %f, ",crude_reco_tracklengths[david]);
	  std::cout<<std::endl;
	  }
	*/
	//Fill DataTree
	DataTree->Fill();
      }      
      
    } // end loop over events to make ttree
    
    
    //Initialize the histograms with max/min bins from first event loop
    InitializeHistograms(MCTheta, n_bins_histo);
    
    //Loop over DataTree and make histograms
    LoopOverTree();
    
    std::cout<<"badboxcounter = "<<badboxcounter<<std::endl;
    
    my_storage.close();
    
    return 1;
    
  } //end EventLoop function
  
  void KalekoTrackAna::InitializeHistograms(const int MCTheta, const int n_bins_histo){
    
    char hname[500];
    char htitle[500];
    
    int lowbin = min_mydRecoTrackAngle - (max_mydRecoTrackAngle-min_mydRecoTrackAngle)*0.1; 
    int highbin = max_mydRecoTrackAngle + (max_mydRecoTrackAngle-min_mydRecoTrackAngle)*0.1; 
    
    // Initialize RecoTrackangleHist with range determined from first event loop
    sprintf(hname,"RecoTrackAngleHist_MCTheta%d",MCTheta);
    sprintf(htitle,"Muon Reco Track [ThetaYZ - MCTheta]: MCTheta = %d",MCTheta);
    RecoTrackAngleHist = new TH1D(hname,htitle,
				  n_bins_histo, lowbin, highbin);
    
    
    // Initialize MCTrackAngleHist with same bins, etc  
    sprintf(hname,"MCTrackAngleHist_MCTheta%d",MCTheta);
    sprintf(htitle,"MC Muon Track ThetaYZ - MCTheta: MCTheta = %d",MCTheta);
    MCTrackAngleHist = new TH1D(hname,htitle,
				n_bins_histo, lowbin, highbin);
    
    // MCTrackBoxAngleHist (using initial MC angle from points inside of reconstructed track boundaries)
    sprintf(hname,"MCTrackBoxAngleHist_MCTheta%d",MCTheta);
    sprintf(htitle,"MC Muon Track ThetaYZ - MCTheta [inside Reco Track Limits]: MCTheta = %d",MCTheta);
    MCTrackBoxAngleHist = new TH1D(hname,htitle,
				   n_bins_histo, lowbin, highbin);
    
    
    // Initialize NRecoTracksHist
    sprintf(hname,"NRecoTracksHist_MCTheta%d",MCTheta);
    sprintf(htitle,"Number Reconstructed Tracks per Event: MCTheta = %d",MCTheta);
    NRecoTracksHist = new TH1D(hname,htitle,
			       max_nRecoTracks,-0.5,(double)max_nRecoTracks-0.5);
    
    
    
  }
  
  void KalekoTrackAna::PrepareDataTree(){
    
    DataTree = new TTree();
    DataTree->Branch("myEventNumber", &myEventNumber, "myEventNumber/I");
    DataTree->Branch("nRecoTracks", &nRecoTracks, "nRecoTracks/I");
    DataTree->Branch("nSpacePoints",&nSpacePoints, "nSpacePoints/I");
    DataTree->Branch("myMCTheta", &myMCTheta, "myMCTheta/D");
    DataTree->Branch("mydMCBoxTheta","std::vector<double>",&mydMCBoxTheta);
    DataTree->Branch("mydRecoTrackAngle","std::vector<double>",&mydRecoTrackAngle);
    DataTree->Branch("myRecoTrackAngle","std::vector<double>",&myRecoTrackAngle);
    DataTree->Branch("myRecoStartingZ","std::vector<double>",&myRecoStartingZ);
    DataTree->Branch("myAverageRecoTrackAngle","std::vector<double>",&myAverageRecoTrackAngle);
    DataTree->Branch("crude_reco_tracklengths","std::vector<double>",&crude_reco_tracklengths);
    DataTree->Branch("crude_MC_tracklengths","std::vector<double>",&crude_MC_tracklengths);
    DataTree->Branch("straightline_reco_tracklengths","std::vector<double>",&straightline_reco_tracklengths);
    DataTree->Branch("nTrackPoints","std::vector<int>",&nTrackPoints);
    DataTree->Branch("E_dEdx_pair","std::vector<std::pair<double,double>>",&E_dEdx_pair);
    
  }
  
  void KalekoTrackAna::LoopOverTree(){
    
    //loop over data tree I created in the main event loop and make histograms from its contents
    for(int ientry=0; ientry<DataTree->GetEntries(); ++ientry){
      
      DataTree->GetEntry(ientry);
      NRecoTracksHist->Fill(nRecoTracks);
      
      //temp
      if(nRecoTracks == 1){
	
	for(std::vector<double>::iterator iter = mydRecoTrackAngle.begin();
	    iter != mydRecoTrackAngle.end();
	    ++iter)
	  RecoTrackAngleHist->Fill(*iter);
	
	
	//filling this with zero... since I centered it at zero...
	MCTrackAngleHist->Fill(myMCTheta - myMCTheta);
	
	for(std::vector<double>::iterator iter = mydMCBoxTheta.begin();
	    iter != mydMCBoxTheta.end();
	    ++iter)
	  MCTrackBoxAngleHist->Fill(*iter); 
      }//end temp
      
    } // end for-loop over tree entries
  }//end LoopOverTree()
}

#endif
