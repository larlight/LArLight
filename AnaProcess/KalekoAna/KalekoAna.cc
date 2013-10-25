#include "KalekoAna.hh"

#include <TSystem.h>
#include <TVector3.h>
#include <TH1.h>
#include <TGraph.h>
#include <TMath.h>
#include <storage_manager.hh>
#include <constants.hh>


KalekoAna::KalekoAna(){
  //
  // Constructor, clears histograms
  //

  RecoTrackAngleHist = 0;
  MCTrackAngleHist = 0;
  NRecoTracksHist = 0;
  
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

  //Initialize the histograms with bins and such
  InitializeHistograms(MCTheta, n_bins_histo);

  //
  // Let's loop over our events
  //

  int evt_counter = 0;
  while(my_storage.next_event() && (evt_counter < max_evts || max_evts == -1)){

    double mctheta_i = -1;

    //MC stuff
    event_mc* my_event_mc = (event_mc*)(my_storage.get_data(DATA::MCTruth));

    if(!my_event_mc) continue;

    else{

      const std::vector<part_mc> my_part_mc(my_event_mc->GetParticleCollection());
      
      mctheta_i = my_part_mc.at(0).step_momentum().at(0).Theta();

      //Fill MCTheta histo
      MCTrackAngleHist->Fill(mctheta_i);

    }

    //Reco stuff
    event_track* my_event_track = (event_track*)(my_storage.get_data(DATA::Kalman3DSPS));

    if(!my_event_track) continue;

    else{
      
      const std::vector<track> my_track(my_event_track->GetTrackCollection());
      std::cout << Form("Found %zu tracks for event %d...",
			my_track.size(),
      			my_event_track->event_id())
      		<< std::endl;
      
      //studying the # of reconstructed tracks per event
      NRecoTracksHist->Fill((int)my_track.size());

      for(size_t i=0; i<my_track.size(); i++){
	
	// Fill angular distribution histo
	RecoTrackAngleHist->Fill(my_track.at(0).direction_at(0).Theta());

      }
    }


    if( max_evts != -1 ) evt_counter++;

  } // end loop over events
  
  my_storage.close();
  return 1;
}

TGraph* KalekoAna::GenWindowEfficiencyGraph(const int max_window_radius){

  TGraph *WindowEfficiencyGraph = new TGraph();

  TH1D* myRecoTrackAngleHist = GetRecoTrackAngleHist();

  int center_bin = GetMCTrackAngleHist()->GetMaximumBin();
  int total_evts = GetMCTrackAngleHist()->GetEntries();
  double radians_per_bin = GetMCTrackAngleHist()->GetBinWidth(center_bin);

  double window_integral, window_efficiency = -1;
  
  int pt_ctr = 0;
  for (int window_width = max_window_radius;
       window_width > 0;
       --window_width){

    window_integral = myRecoTrackAngleHist->
      Integral(center_bin-window_width,
	       center_bin+window_width);
    
    window_efficiency = (double)window_integral/(double)total_evts;

    WindowEfficiencyGraph->
      SetPoint(pt_ctr, 
	       (double)window_width*radians_per_bin,
	       window_efficiency);    

    pt_ctr++;
  }

  return WindowEfficiencyGraph;
 
}
 
void KalekoAna::InitializeHistograms(const int MCTheta, const int n_bins_histo){
  char hname[500];
  char htitle[500];

  // Initialize RecoTrackAngleHist
  sprintf(hname,"RecoTrackAngleHist_MCTheta%d",MCTheta);
  sprintf(htitle,"Kalman3DSPS Single Muon Recon Track ThetaYZ: MCTheta = %d",MCTheta);
  RecoTrackAngleHist = new TH1D(hname,htitle,
				n_bins_histo, 0, TMath::Pi());
  //    RecoTrackAngleHist->Reset();

  // Initialize MCTrackAngleHist with same bins, etc  
  sprintf(hname,"MCTrackAngleHist_MCTheta%d",MCTheta);
  sprintf(htitle,"MC Single Muon Track ThetaYZ: MCTheta = %d",MCTheta);
  MCTrackAngleHist = new TH1D(hname,htitle,
			      n_bins_histo, 0, TMath::Pi());
  
  // Initialize NRecoTracksHist
  sprintf(hname,"NRecoTracksHist_MCTheta%d",MCTheta);
  sprintf(htitle,"Number Kalman3DSPS Reconstructed Tracks per Event: MCTheta = %d",MCTheta);
  NRecoTracksHist = new TH1D(hname,htitle,
			     5,-0.5,4.5);

}
