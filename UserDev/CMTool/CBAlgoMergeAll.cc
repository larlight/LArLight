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
  bool CBAlgoMergeAll::Bool(const ::cluster::ClusterParamsAlgNew &cluster1,
			    const ::cluster::ClusterParamsAlgNew &cluster2)
  //--------------------------------------------------------
  {
    if(cluster1.GetNHits() && cluster2.GetNHits()) return true;
    else return false;
  }
  
}

#endif
