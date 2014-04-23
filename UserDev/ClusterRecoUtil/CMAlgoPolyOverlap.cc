#ifndef CMALGOPOLYOVERLAP_CC
#define CMALGOPOLYOVERLAP_CC

#include "CMAlgoPolyOverlap.hh"

namespace cluster {

  CMAlgoPolyOverlap::CMAlgoPolyOverlap()
  {
    // Nothing to be done in the base class
    this->reconfigure();
  }


  void CMAlgoPolyOverlap::reconfigure(){

    //not sure what needs to be reset/reconfigured for this algo
    
  }//end reconfigure function

  
  bool CMAlgoPolyOverlap::Merge(const cluster_params &cluster1,
				     const cluster_params &cluster2)
  {
    //if the two polygons overlap even partially
    //then return true! --> MERGE!
    if ( cluster1.PolyObject.PolyOverlapSegments(cluster2.PolyObject) )
      return true;
    else
      return false;
  }


}

#endif
