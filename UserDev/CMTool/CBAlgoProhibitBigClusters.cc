#ifndef CBALGOPROHIBITBIGCLUSTERS_CXX
#define CBALGOPROHIBITBIGCLUSTERS_CXX

#include "CBAlgoProhibitBigClusters.hh"

namespace cmtool {

  //-------------------------------------------------------
  CBAlgoProhibitBigClusters::CBAlgoProhibitBigClusters() : CBoolAlgoBase()
  //-------------------------------------------------------
  {
    SetMinHits(9999);

  }

  //-----------------------------
  void CBAlgoProhibitBigClusters::Reset()
  //-----------------------------
  {

  }

  //----------------------------------------------------------------
  bool CBAlgoProhibitBigClusters::Bool(const ::cluster::ClusterParamsAlgNew &cluster1,
			       const ::cluster::ClusterParamsAlgNew &cluster2)
  //----------------------------------------------------------------
  {
    //if both clusters are big enough, prohibit merging
    if(cluster1.GetNHits() > _min_hits && cluster2.GetNHits() > _min_hits)
      return true;
    else
      return false;
  }

  //------------------------------
  void CBAlgoProhibitBigClusters::Report()
  //------------------------------
  {

  }
    
}
#endif
