#ifndef TEST_XDIRRECO_FILTER_CC
#define TEST_XDIRRECO_FILTER_CC

#include "test_xdirreco_filter.hh"

namespace kaleko {

  bool test_xdirreco_filter::initialize() {
    
    return true;
  }
  
  bool test_xdirreco_filter::analyze(larlight::storage_manager* storage) {
    
    //return true to keep the event, return false ot filter it out
    
    //grab the reconstructed showers
    larlight::event_shower* ev_shower = (larlight::event_shower*)storage->get_data(larlight::DATA::Shower);
    if(!ev_shower) {
      print(larlight::MSG::ERROR,__FUNCTION__,Form("Did not find specified data product, Shower!"));
      return false;
    }
    
    //grab the MC showers
    larlight::event_mcshower* ev_mcshower = (larlight::event_mcshower*)storage->get_data(larlight::DATA::MCShower);
    if(!ev_mcshower) {
      print(larlight::MSG::ERROR,__FUNCTION__,Form("Did not find specified data product, MCShower!"));
      return false;
    }
    
    if(ev_mcshower->size() != 1) {
      std::cout<<"Why are there "<<ev_mcshower->size()<<" mcshowers? For elecrons, should only have 1. Skipping this event."<<std::endl;
      return false;
    }
    if(ev_shower->size() != 1) {
      std::cout<<"Why are there "<<ev_shower->size()<<" showers? For electrons, should only have 1. Skipping this event."<<std::endl;
      return false;
    }
    
    
    double mom_dir_magnitude = 
      pow(ev_mcshower->at(0).DaughterMomentum().at(0),2)+
      pow(ev_mcshower->at(0).DaughterMomentum().at(1),2)+
      pow(ev_mcshower->at(0).DaughterMomentum().at(2),2);
    mom_dir_magnitude = pow(mom_dir_magnitude,0.5);
    
    double mc_x_dir = ev_mcshower->at(0).DaughterMomentum().at(0);
    double reco_x_dir = ev_shower->at(0).Direction()[0];
    
    //I want to "filter out" the events with good x-direction reconstruction
    //and *keep* the events with bad ones (so i can look @ them in viewer)
    if(mc_x_dir-reco_x_dir < .15 && mc_x_dir-reco_x_dir > -.15)
      return false;
    else
      return true;
    
  }
  
  bool test_xdirreco_filter::finalize() {
    
    // This function is called at the end of event loop.
    // Do all variable finalization you wish to do here.
    // If you need, you can store your ROOT class instance in the output
    // file. You have an access to the output file through "_fout" pointer.
    //
    // Say you made a histogram pointer h1 to store. You can do this:
    //
    // if(_fout) { _fout->cd(); h1->Write(); }
    //
    // else 
    //   print(larlight::MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
    //
  
    return true;
  }

}
#endif
