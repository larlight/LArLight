#ifndef TRIGGERDATA_CXX
#define TRIGGERDATA_CXX

#include "TriggerData.h"

namespace trigdata {

  //****************
  Trigger::Trigger()
  //****************
  {
    fTriggerNumber  = std::numeric_limits<unsigned int>::max();
    fTriggerSample  = std::numeric_limits<unsigned int>::max();
    fTriggerFrame   = std::numeric_limits<unsigned int>::max();
    fBeamGateSample = std::numeric_limits<unsigned int>::max();
    fBeamGateFrame  = std::numeric_limits<unsigned int>::max();
    fTriggerBits    = std::numeric_limits<unsigned int>::min();
  }
  
  //****************************************************
  bool Trigger::Triggered(const unsigned char bit) const
  //****************************************************
  {

    if(bit>32) {
      throw std::invalid_argument("\n\nCannot access bit higher than 32!\n");
      std::exit(1);
    }

    return ( (fTriggerBits >> bit) & 0x1);

  }

}

#endif
