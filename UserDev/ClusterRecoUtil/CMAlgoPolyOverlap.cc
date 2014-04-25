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

  
  bool CMAlgoPolyOverlap::Merge(const ClusterParamsAlgNew &cluster1,
				const ClusterParamsAlgNew &cluster2)
  {
    //if either has < 4 hits do not merge!
    if ( (cluster1.GetParams().PolyObject.Size() < 3) or
	 (cluster2.GetParams().PolyObject.Size() < 3) ){
      return false;
    }
    //if the two polygons overlap even partially
    //then return true! --> MERGE!
    if ( cluster1.GetParams().PolyObject.PolyOverlapSegments(cluster2.GetParams().PolyObject) ){
      std::cout << "merging!" << std::endl;
      return true;
    }
    else
      return false;
  }


}

#endif
