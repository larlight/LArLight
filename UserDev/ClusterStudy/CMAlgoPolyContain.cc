#ifndef CMALGOPOLYCONTAIN_CC
#define CMALGOPOLYCONTAIN_CC

#include "CMAlgoPolyContain.hh"

namespace cluster {

  CMAlgoPolyContain::CMAlgoPolyContain()
  {
    // Nothing to be done in the base class
    this->reconfigure();
  }


  void CMAlgoPolyContain::reconfigure(){

    //not sure what needs to be reset/reconfigured for this algo
    
  }//end reconfigure function

  
  bool CMAlgoPolyContain::Bool(const ClusterParamsAlgNew &cluster1,
			       const ClusterParamsAlgNew &cluster2)
  {

    if ( (cluster1.GetParams().PolyObject.Size() < 3) or (cluster2.GetParams().PolyObject.Size() < 3) )
      return false;

    //if either polygon is fully contained in other
    //then return true! --> MERGE!
    if ( (cluster1.GetParams().PolyObject.Contained(cluster2.GetParams().PolyObject)) or
	 (cluster2.GetParams().PolyObject.Contained(cluster1.GetParams().PolyObject)) )
      return true;
    else
      return false;
  }
  

}

#endif
