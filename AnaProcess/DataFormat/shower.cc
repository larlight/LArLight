#ifndef SHOWER_CC
#define SHOWER_CC

#include "shower.hh"

namespace larlight {

  //*******************************************
  void shower::clear_data()
  //******************************************* 
  {
    data_base::clear_data();

    fID = -1;
    fDCosStart[0] = fDCosStart[1] = fDCosStart[2] = -1;
    fSigmaDCosStart[0] = fSigmaDCosStart[1] = fSigmaDCosStart[2] = -1;
    fMaxWidthX = fMaxWidthY = -1;
    fDistanceMaxWidth=-1;
    fTotalCharge=-1;
    fClusters_v.clear();
  }

}

#endif
