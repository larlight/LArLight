#ifndef CMALGOMERGEALL_CC
#define CMALGOMERGEALL_CC

#include "CMAlgoMergeAll.hh"

namespace cluster {

  //----------------------------------------
  CMAlgoMergeAll::CMAlgoMergeAll() : CBoolAlgoBase()
  //----------------------------------------
  {

  }

  //--------------------------------------------------------
  bool CMAlgoMergeAll::Bool(const ClusterParamsAlgNew &cluster1,
			const ClusterParamsAlgNew &cluster2)
  //--------------------------------------------------------
  {
    return true;
  }

  //-----------------------
  void CMAlgoMergeAll::Report()
  //-----------------------
  {

  }

}

#endif
