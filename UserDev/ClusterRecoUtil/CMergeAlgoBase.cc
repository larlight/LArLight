#ifndef CMERGEALGOBASE_CC
#define CMERGEALGOBASE_CC

#include "CMergeAlgoBase.hh"

namespace cluster {

  CMergeAlgoBase::CMergeAlgoBase()
  {
    // Nothing to be done in the base class
  }
  
  bool CMergeAlgoBase::Merge(const cluster_params &cluster1,
				     const cluster_params &cluster2)
  {
    // Return false for the stupid base class
    return false;
  }


}

#endif
