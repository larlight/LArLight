#ifndef CMALGOPOLYHITOVERLAP_CC
#define CMALGOPOLYHITOVERLAP_CC

#include "CMAlgoPolyHitOverlap.hh"

namespace cluster {

  CMAlgoPolyHitOverlap::CMAlgoPolyHitOverlap()
  {
    // Nothing to be done in the base class
    this->reconfigure();
  }


  void CMAlgoPolyHitOverlap::reconfigure(){

    //not sure what needs to be reset/reconfigured for this algo
    
  }//end reconfigure function

  
  bool CMAlgoPolyHitOverlap::Bool(const ClusterParamsAlgNew &cluster1,
				  const ClusterParamsAlgNew &cluster2)
  {

    //Check and see if a certain fraction of hits of a cluster
    //lie within polygon boundary of other cluster
    

    return false;
  }


}

#endif
