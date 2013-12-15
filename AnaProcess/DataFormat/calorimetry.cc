#ifndef CALORIMETRY_CC
#define CALORIMETRY_CC

#include "calorimetry.hh"

namespace larlight {

  //*************************************
  void calorimetry::clear_data() 
  //************************************
  {
    data_base::clear_data();
    fdEdx.clear();
    fdQdx.clear();
    fResidualRange.clear();
    fDeadWireResR.clear();
    fTrkPitch.clear();
    fRange = -1;
  }

}

#endif
