#ifndef HITBACKTRACKER_CC
#define HITBACKTRACKER_CC

#include "HitBackTracker.hh"

namespace larlight {


  double HitBackTracker::FindElectrons( larlight::hit h ){

    //if simchannelMap not filled then return -1
    if ( _simch_map.size() == 0 )
      return -1;

    //Find the simchannels on the hit's channel number
    auto simch_iter = _simch_map.find( h.Channel() );
    if( simch_iter == _simch_map.end() ) 
      std::cerr<<"Hit has no matched simchannel!!"<<std::endl;
    
    if (_verbosity) { std::cout << "Hit time boundaries: [" << h.StartTime() << ", " << h.EndTime() << "]" << std::endl; }
    
    //Find IDEs on this simch
      double sigmaTime = h.PeakTime() - h.StartTime();
      std::vector<larlight::ide> matchedIDEs((*simch_iter).second.TrackIDsAndEnergies(h.PeakTime()-sigmaTime*2,h.PeakTime()+sigmaTime*2));
      
      //if matchedIDEs is empty no IDEs in the time-range of the hit on that channel
      if ( matchedIDEs.size() == 0 ){
	std::cerr << "Hit with no associated IDEs..." << std::endl;
	return 0;
      }
      
      //else loop through found IDEs and and add charge in them      
      double numElectrons = 0;
      for ( auto const &IDE : matchedIDEs )
	numElectrons += IDE.numElectrons;

      return numElectrons;
      
  }

  void HitBackTracker::MakeSimchMap( larlight::event_simch *evt_simch ){
    
    //Make a map for the simchannels and the LAr Chs
    _simch_map.clear();
    for(auto simCh : *evt_simch)
      _simch_map.insert(std::pair<UShort_t,larlight::simch>(simCh.Channel(),simCh));

  }


}
#endif
