#include "KalekoAna.hh"


KalekoAna::KalekoAna(){
  //
  // Constructor, clears histograms, clears tree
  //

  RecoTrackAngleHist = 0;
  MCTrackAngleHist = 0;
  MCTrackBoxAngleHist = 0;
  NRecoTracksHist = 0;
  DataTree = 0;

};


int KalekoAna::EventLoop(std::string filename, const int MCTheta, const int max_evts, const int n_bins_histo){

  //
  // A simple routine to read a data file and perform an event loop.
  // This is a test routine for storage_manager class which interacts
  // decoder output root file. 
  //

  storage_manager my_storage;

  // If you wish, change the message level to DEBUG.
 
  my_storage.set_verbosity(MSG::DEBUG);
  
  // Step 0: Set I/O mode: we are reading in, so "READ"
  my_storage.set_io_mode(storage_manager::READ);

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
  
  // Loop over our events  
  int evt_counter = 0;
  
  while(my_storage.next_event() && (evt_counter < max_evts || max_evts == -1)){
    
    double track_xmin = 0, track_xmax = 0,
      track_ymin = 0, track_ymax = 0,
      track_zmin = 0, track_zmax = 0;
    //one tracklength per reconstructed track
    std::vector<double> crude_reco_tracklengths;
    //length of mc track within reco box
    std::vector<double> crude_MC_tracklengths;
    
    

    //Reconstructed objects stuff
    event_track* my_event_track = (event_track*)(my_storage.get_data(fDataType));
    if(!my_event_track){
      std::cout<<"DID NOT FIND SPECIFIED DATA PRODUCT!"<<std::endl;
      break;
    }
    else{    
            
      //MC stuff
      event_mc* my_event_mc = (event_mc*)(my_storage.get_data(DATA::MCTruth));
      
      //only loop over this event if mc object and track reco object exists
      //note, it still enters loop even if the reco object is an empty vector
      //(IE no tracks were reconstructed). This is good.
      if(!my_event_track || !my_event_mc) continue;
      
      else{
	
	const std::vector<track> my_track(my_event_track->GetTrackCollection());
	const std::vector<part_mc> my_part_mc(my_event_mc->GetParticleCollection());

	//reset the vectors that are going into my tree
	mydRecoTrackAngle.clear();
	mydMCBoxTheta.clear();
		
	//initialize track lengths to zero before each track
	crude_reco_tracklengths.clear();
	crude_MC_tracklengths.clear();
	
	//finding max # of reconstructed tracks for histogram boundaries
	if(max_nRecoTracks < (int)my_track.size()) max_nRecoTracks = (int)my_track.size();

	//one of the DataTree variables
	myMCTheta = (double)MCTheta;

	//loop over all of the reconstructed tracks in the event
	for(size_t i=0; i<my_track.size(); i++){
	  
	  //initial guess for ranges, will be altered by get_axis_range()
	  track_xmin = track_xmax = my_track.at(i).vertex_at(0).X();
	  track_ymin = track_ymax = my_track.at(i).vertex_at(0).Y();
	  track_zmin = track_zmax = my_track.at(i).vertex_at(0).Z();
	  //updates the track_xmin, etc variables to be true mins and maxes
	  //(min and max for this reconstructed track)
	  my_track.at(i).get_axis_range(track_xmax, track_xmin,
					track_ymax, track_ymin,
					track_zmax, track_zmin);
	  
	  
	  double tmplength = 0;
	  //for each track, loop over the reconstructed points in it
	  for(size_t j=0; j < my_track.at(i).n_point()-1; j++){
	    tmplength += 
	      (my_track.at(i).vertex_at(j) - my_track.at(i).vertex_at(j+1)).Mag();
	  }//end loop over points in individual track
	  crude_reco_tracklengths.push_back(tmplength);
	  

	  //now loop over the mc points and calculate a MC tracklength that
	  //is inside of this track's "box"
	  //loop over each MC step the parent particle takes
	  bool HaventFilledHistoYet = true;
	  
	  double dMCBoxTheta_i = -99.;
	  
	  tmplength = 0;
	  for(size_t j=0; j < my_part_mc.at(0).step_vertex().size()-1; j++){
	    TVector3 vtx_j = my_part_mc.at(0).step_vertex().at(j);	
	    TVector3 vtx_jp1 = my_part_mc.at(0).step_vertex().at(j+1);
	    
	    //if the current vertex point is inside of the box
	    if(vtx_j.X() > track_xmin && vtx_j.X() < track_xmax &&
	       vtx_j.Y() > track_ymin && vtx_j.Y() < track_ymax &&
	       vtx_j.Z() > track_zmin && vtx_j.Z() < track_zmax){
	      
	      tmplength += ( vtx_jp1 - vtx_j ).Mag();
	      //only store the angle for the "first" point inside the box
	      //(safe to assume I'm looping from start to finish?)
	      if(HaventFilledHistoYet){
		dMCBoxTheta_i = (vtx_jp1-vtx_j).Theta() * UtilFunctions().DegreesPerRadian - MCTheta;
		mydMCBoxTheta.push_back(dMCBoxTheta_i);
		HaventFilledHistoYet=false;
	      }
	      
	    } //end if step vertex is inside of reconstructed track's box limits
	  } // end looping over each step the parent particle takes
	  crude_MC_tracklengths.push_back(tmplength);


	  //do i want to plot difference from the muon-generated angle?
	  //	  double dRecoTrackAngle_i = (my_track.at(i).direction_at(0).Theta()
	  //				      * UtilFunctions().DegreesPerRadian ) - MCTheta;
	    
	  //or, difference from the angle of muon as it enters the reconstructed "box"?
	  double dRecoTrackAngle_i = (my_track.at(i).direction_at(0).Theta()
				      * UtilFunctions().DegreesPerRadian ) - (dMCBoxTheta_i+MCTheta);

	  //one of the DataTree variables
       	  mydRecoTrackAngle.push_back( dRecoTrackAngle_i );

	  //check to see if I need to re-define max and min histogram range
	  if(max_mydRecoTrackAngle == -1 || max_mydRecoTrackAngle < dRecoTrackAngle_i)
	    max_mydRecoTrackAngle = dRecoTrackAngle_i;

	  if(min_mydRecoTrackAngle == -1 || min_mydRecoTrackAngle > dRecoTrackAngle_i)
	    min_mydRecoTrackAngle = dRecoTrackAngle_i;



	}//end loop over each reconstructed track
	
      } // end else
      
      if( max_evts != -1 ) evt_counter++;
      
      /*      
      std::cout<<Form("tracklengths: ");
      for(size_t jason = 0; jason < crude_MC_tracklengths.size(); jason++)
	std::cout<<Form("MC = %f, ",crude_MC_tracklengths[jason]);
      for(size_t david = 0; david < crude_reco_tracklengths.size(); david++)
	std::cout<<Form("reco = %f, ",crude_reco_tracklengths[david]);
      std::cout<<std::endl;
      */
   
      //Fill DataTree
      DataTree->Fill();
    }      
      
  } // end loop over events to make ttree


  //Initialize the histograms with max/min bins from first event loop
  InitializeHistograms(MCTheta, n_bins_histo);
  
  //Loop over DataTree and make histograms
  LoopOverTree();

  my_storage.close();

  return 1;

} //end EventLoop function

void KalekoAna::InitializeHistograms(const int MCTheta, const int n_bins_histo){

  char hname[500];
  char htitle[500];

  // Initialize RecoTrackangleHist with range determined from first event loop
  sprintf(hname,"RecoTrackAngleHist_MCTheta%d",MCTheta);
  sprintf(htitle,"Muon Reco Track [ThetaYZ - MCTheta]: MCTheta = %d",MCTheta);
  RecoTrackAngleHist = new TH1D(hname,htitle,
				n_bins_histo, min_mydRecoTrackAngle, max_mydRecoTrackAngle);
  

  // Initialize MCTrackAngleHist with same bins, etc  
  sprintf(hname,"MCTrackAngleHist_MCTheta%d",MCTheta);
  sprintf(htitle,"MC Muon Track ThetaYZ - MCTheta: MCTheta = %d",MCTheta);
  MCTrackAngleHist = new TH1D(hname,htitle,
			      n_bins_histo, min_mydRecoTrackAngle, max_mydRecoTrackAngle);

  // MCTrackBoxAngleHist (using initial MC angle from points inside of reconstructed track boundaries)
  sprintf(hname,"MCTrackBoxAngleHist_MCTheta%d",MCTheta);
  sprintf(htitle,"MC Muon Track ThetaYZ - MCTheta [inside Reco Track Limits]: MCTheta = %d",MCTheta);
  MCTrackBoxAngleHist = new TH1D(hname,htitle,
  				 n_bins_histo, min_mydRecoTrackAngle, max_mydRecoTrackAngle);


  // Initialize NRecoTracksHist
  sprintf(hname,"NRecoTracksHist_MCTheta%d",MCTheta);
  sprintf(htitle,"Number Reconstructed Tracks per Event: MCTheta = %d",MCTheta);
  NRecoTracksHist = new TH1D(hname,htitle,
			     max_nRecoTracks,-0.5,(double)max_nRecoTracks-0.5);

}

void KalekoAna::PrepareDataTree(){

  DataTree = new TTree();
  DataTree->Branch("myMCTheta", &myMCTheta, "myMCTheta/D");
  DataTree->Branch("mydMCBoxTheta","std::vector<double>",&mydMCBoxTheta);
  DataTree->Branch("mydRecoTrackAngle","std::vector<double>",&mydRecoTrackAngle);

}

void KalekoAna::LoopOverTree(){

  //loop over data tree I created in the main event loop and make histograms from its contents
  for(int ientry=0; ientry<DataTree->GetEntries(); ++ientry){

    DataTree->GetEntry(ientry);
  
    NRecoTracksHist->Fill((int)mydRecoTrackAngle.size());
    
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

  }
 
}
