#ifndef CLUSTERPARAMSEXECUTOR_CC
#define CLUSTERPARAMSEXECUTOR_CC

#include "ClusterParamsExecutor.hh"

namespace cluster {

  bool ClusterParamsExecutor::initialize()
  {
    return true;
  }

  bool ClusterParamsExecutor::analyze(larlight::storage_manager* storage)
  {
    return true;
  }


  bool ClusterParamsExecutor::finalize()
  {
    return true;
  }


}

#endif
