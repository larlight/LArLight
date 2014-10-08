#ifndef CBALGOMERGEALL_CC
#define CBALGOMERGEALL_CC

#include "CBAlgoMergeAll.hh"

namespace cmtool {

  //----------------------------------------
  CBAlgoMergeAll::CBAlgoMergeAll() : CBoolAlgoBase()
  //----------------------------------------
  {

  }

  //--------------------------------------------------------
  bool CBAlgoMergeAll::Bool(const ::cluster::ClusterParamsAlg &cluster1,
			    const ::cluster::ClusterParamsAlg &cluster2)
  //--------------------------------------------------------
  {
    if(cluster1.GetNHits() && cluster2.GetNHits()) return true;
    else return false;
  }
  
}

#endif
