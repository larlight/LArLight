#ifndef CBALGOPOLYCONTAIN_CXX
#define CBALGOPOLYCONTAIN_CXX

#include "CBAlgoPolyContain.hh"

namespace cmtool {

  CBAlgoPolyContain::CBAlgoPolyContain() : CBoolAlgoBase()
  {
    // Nothing to be done in the base class
    this->reconfigure();
  }


  void CBAlgoPolyContain::reconfigure(){

    //not sure what needs to be reset/reconfigured for this algo
    
  }//end reconfigure function

  
  bool CBAlgoPolyContain::Bool(const ::cluster::ClusterParamsAlgNew &cluster1,
			       const ::cluster::ClusterParamsAlgNew &cluster2)
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
