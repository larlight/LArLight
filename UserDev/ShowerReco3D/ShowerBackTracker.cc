#ifndef SHOWERBACKTRACKER_CC
#define SHOWERBACKTRACKER_CC

#include "ShowerBackTracker.hh"

namespace larlight {

  ShowerBackTracker::ShowerBackTracker()
  {
    fBTAlg.SetMinEnergyCut(0.03); // 30 MeV minimum energy cut
  }

  bool ShowerBackTracker::Prepare(storage_manager* storage)
  {

    auto ev_mcshower = (const event_mcshower*)(storage->get_data(DATA::MCShower));

    auto ev_simch    = (const event_simch*)(storage->get_data(DATA::SimChannel));

    if(!ev_mcshower || !ev_simch) {

      std::cerr<<"MCShower/SimCh data product not found!"<<std::endl;

      return false;
      
    }

    fShowerIDMap.clear();
    fSimChMap.clear();

    fBTAlg.FillShowerMap(ev_mcshower, fShowerIDMap);

    fBTAlg.FillSimchMap(ev_simch, fSimChMap);

    fBTAlg.UniqueShowerID(fShowerIDMap, fUniqueShowerID);

    return true;
  }

}
#endif
