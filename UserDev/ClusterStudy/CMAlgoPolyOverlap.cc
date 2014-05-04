#ifndef CMALGOPOLYOVERLAP_CC
#define CMALGOPOLYOVERLAP_CC

#include "CMAlgoPolyOverlap.hh"

namespace cluster {

  CMAlgoPolyOverlap::CMAlgoPolyOverlap()
  {
    // Nothing to be done in the base class
    SetDebug(true);
    SetVerbose(true);
    this->reconfigure();
  }


  void CMAlgoPolyOverlap::reconfigure(){

    //not sure what needs to be reset/reconfigured for this algo
    
  }//end reconfigure function

  
  bool CMAlgoPolyOverlap::Bool(const ClusterParamsAlgNew &cluster1,
			       const ClusterParamsAlgNew &cluster2)
  {
    //if either has < 4 hits do not merge!
    if ( (cluster1.GetParams().PolyObject.Size() < 3) or
	 (cluster2.GetParams().PolyObject.Size() < 3) ){
      if (_debug) { std::cout << "either cluster is too small!" << std::endl; }
      return false;
    }
    if (_verbose) {
      std::cout << "Cluster 1:" << std::endl;
      std::cout << "\tN_Hits: " << cluster1.GetParams().N_Hits << std::endl;
      std::cout << "\tN Sides:" << cluster1.GetParams().PolyObject.Size() << std::endl;
      for (unsigned int n=0; n < cluster1.GetParams().PolyObject.Size(); n++)
	std::cout << "\t\t\t(" << cluster1.GetParams().PolyObject.Point(n).first << ", "
		  << cluster1.GetParams().PolyObject.Point(n).second << " )" << std::endl;
      std::cout << "Cluster 2:" << std::endl;
      std::cout << "\tN_Hits: " << cluster2.GetParams().N_Hits << std::endl;
      std::cout << "\tN Sides:" << cluster2.GetParams().PolyObject.Size() << std::endl;
      for (unsigned int n=0; n < cluster2.GetParams().PolyObject.Size(); n++)
	std::cout << "\t\t\t(" << cluster2.GetParams().PolyObject.Point(n).first << ", "
		  << cluster2.GetParams().PolyObject.Point(n).second << " )" << std::endl;
    }

    //if the two polygons overlap even partially
    //then return true! --> MERGE!
    if ( cluster1.GetParams().PolyObject.PolyOverlapSegments(cluster2.GetParams().PolyObject) ){
      if (_verbose) { std::cout << "Overlap...merging!" << std::endl; }
      return true;
    }
    if (_debug) { std::cout << "No overlap..." << std::endl; }
    return false;

  }


}

#endif
