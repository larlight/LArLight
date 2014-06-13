#ifndef CMALGOPROHIBITBIGCLUSTERS_CXX
#define CMALGOPROHIBITBIGCLUSTERS_CXX

#include "CMAlgoProhibitBigClusters.hh"

namespace cluster {

  //-------------------------------------------------------
  CMAlgoProhibitBigClusters::CMAlgoProhibitBigClusters() : CBoolAlgoBase()
  //-------------------------------------------------------
  {
    SetMinHits(9999);

  }

  //-----------------------------
  void CMAlgoProhibitBigClusters::Reset()
  //-----------------------------
  {

  }

  //----------------------------------------------------------------
  bool CMAlgoProhibitBigClusters::Bool(const ClusterParamsAlgNew &cluster1,
			       const ClusterParamsAlgNew &cluster2)
  //----------------------------------------------------------------
  {
    //if both clusters are big enough, prohibit merging
    if(cluster1.GetNHits() > _min_hits && cluster2.GetNHits() > _min_hits)
      return true;
    else
      return false;
  }

  //------------------------------
  void CMAlgoProhibitBigClusters::Report()
  //------------------------------
  {

  }
    
}
#endif
