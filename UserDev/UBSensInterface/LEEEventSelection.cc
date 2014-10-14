#ifndef LEEEVENTSELECTION_CC
#define LEEEVENTSELECTION_CC

#include "LEEEventSelection.hh"

namespace larlight {

  bool LEEEventSelection::initialize() {

    return true;
  }
  
  bool LEEEventSelection::analyze(storage_manager* storage) {

    //grab the MC showers
    larlight::event_mcshower* ev_mcshower = (event_mcshower*)storage->get_data(DATA::MCShower);
    if(!ev_mcshower) {
      print(larlight::MSG::ERROR,__FUNCTION__,Form("Did not find specified data product, MCShower!"));
      return false;
    }

    //If *any* of the MCShowers have vertex (conversion point, in the case
    //of gammas) outside of fiducial volume, filter out this event
    for( auto const & mcs : *ev_mcshower) {
      
      auto _daughtpos = mcs.DaughterPosition();

      //DistToBoxWall uses units of cm, but mcshower stuff is in mm
      for(size_t i = 0; i < _daughtpos.size(); ++i)
	_daughtpos.at(i) = _daughtpos.at(i)/10.;

      double dist_to_wall = _dtbw.DistanceToWall(_daughtpos);
      
      // If point is outside TPC, filter out this event
      if ( dist_to_wall < 0 ) return false;
      
      // If point is outside active volume, filter out this event
      if ( dist_to_wall < _fid_cut_dist ) return false;
      
    }

    // If you've reached here, don't filter out the event
    return true;

  }

  bool LEEEventSelection::finalize() {

    return true;

  }

}
#endif
