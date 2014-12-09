#ifndef LEEEVENTSELECTION_CC
#define LEEEVENTSELECTION_CC

#include "LEEEventSelection.hh"

namespace larlight {

  bool LEEEventSelection::initialize() {

    total_event_counter = 0;
    passed_event_counter = 0;

    generated_x = new TH1F("generated_x","generated_x",1000,-1000,1000);
    generated_y = new TH1F("generated_y","generated_y",1000,-1000,1000);
    generated_z = new TH1F("generated_z","generated_z",1000,-100,2000);
    selected_x = new TH1F("selected_x","selected_x",1000,-1000,1000);
    selected_y = new TH1F("selected_y","selected_y",1000,-1000,1000);
    selected_z = new TH1F("selected_z","selected_z",1000,-100,2000);

    return true;
  }
  
  bool LEEEventSelection::analyze(storage_manager* storage) {
    total_event_counter++;
    
    //grab the MC showers
    larlight::event_mcshower* ev_mcshower = (event_mcshower*)storage->get_data(DATA::MCShower);
    if(!ev_mcshower) {
      print(larlight::MSG::ERROR,__FUNCTION__,Form("Did not find specified data product, MCShower!"));
      return false;
    }

    //If *any* of the MCShowers have vertex (conversion point, in the case
    //of gammas) outside of fiducial volume, filter out this event
    for( auto const & mcs : *ev_mcshower) {
      
      // If point is outside of fiducial volume, filter out event
      // (note: mcshower is in units of cm)
      return isInFidVol(mcs.DaughterPosition());
    }

    // If you've reached here, don't filter out the event
    // (though you should never reach here, currently)
    return true;

  }

  bool LEEEventSelection::finalize() {

    std::cout<<std::endl;
    std::cout<<"Total Events Counted : "<<total_event_counter<<std::endl;
    std::cout<<"Total Events Pass Cut: "<<passed_event_counter<<std::endl;
    std::cout<<std::endl;

    
    if(_fout) {
      _fout->cd();
      generated_x->Write();
      generated_y->Write();
      generated_z->Write();
      selected_x->Write();
      selected_y->Write();
      selected_z->Write();
    }
       

    return true;

  }

  bool LEEEventSelection::isInFidVol(const std::vector<double> &vertex_in_cm){


    generated_x->Fill(vertex_in_cm.at(0));
    generated_y->Fill(vertex_in_cm.at(1));
    generated_z->Fill(vertex_in_cm.at(2));
    

    //////////////////////////////////////////////////////////
    // If you're using custom defined fid volume boundaries:
    //////////////////////////////////////////////////////////
    /// Generated events say that cryostat has bounds roughly:
    /// x = -50 -> 320
    /// y = -190 -> 190
    /// z = 0 -> 1080

    /// TPC boundaries (according to Ariana) in this coord system are:
    /// x = 0 --> 256.35 
    /// y = -116.5 --> 116.5
    /// z = 0 --> 1036.8

    /// Custom fid vol is: 25cm from TPC walls, but
    /// 30 cm from upstream Z-wall, 50cm from downstream Z-wall
    /*
    double xmin = 25.;
    double xmax = 231.35;
    double ymin = -91.5;
    double ymax = 91.5;
    double zmin = 30.;
    double zmax = 986.8;
    */

    /// Custom fid vol: 17cm from all TPC walls
    double xmin = 0. + 17.;
    double xmax = 256.35-17.;
    double ymin = -116.5 + 17.;
    double ymax = 116.5 - 17.;
    double zmin = 0. + 17.;
    double zmax = 1036.8 - 17.;





    if( vertex_in_cm.at(0) < xmin || vertex_in_cm.at(0) > xmax)
      return false;

    if( vertex_in_cm.at(1) < ymin || vertex_in_cm.at(1) > ymax)
      return false;
    
    if( vertex_in_cm.at(2) < zmin || vertex_in_cm.at(2) > zmax)
      return false;
    
    passed_event_counter++;
    
    selected_x->Fill(vertex_in_cm.at(0));
    selected_y->Fill(vertex_in_cm.at(1));
    selected_z->Fill(vertex_in_cm.at(2));
    return true;

  }
}
#endif
