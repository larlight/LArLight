#ifndef CMERGEALGOBASE_CC
#define CMERGEALGOBASE_CC

#include "CMergeAlgoBase.hh"

namespace cluster {

  CMergeAlgoBase::CMergeAlgoBase()
  {
    // Nothing to be done in the base class
  }
  
  bool CMergeAlgoBase::Merge(const ClusterParamsAlgNew &cluster1,
			     const ClusterParamsAlgNew &cluster2)
  {
    // Return false for the stupid base class
    return false;
  }


}

#endif
