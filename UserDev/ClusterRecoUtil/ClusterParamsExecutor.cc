#ifndef CLUSTERPARAMSEXECUTOR_CC
#define CLUSTERPARAMSEXECUTOR_CC

#include "ClusterParamsExecutor.hh"

namespace cluster {

  ClusterParamsExecutor::ClusterParamsExecutor(){
    
    _in_cluster_type = larlight::DATA::ShowerAngleCluster;

  }

  bool ClusterParamsExecutor::initialize()
  {
    return true;
  }

  bool ClusterParamsExecutor::analyze(larlight::storage_manager* storage)
  {
    
    auto clusters = (larlight::event_cluster*)(storage->get_data(_in_cluster_type));

    larlight::DATA::DATA_TYPE hit_type = clusters->get_hit_type();
    auto hits = (larlight::event_hit*)(storage->get_data(hit_type));

    for(auto const& h : *hits) {

      ;

    }

    return true;
  }


  bool ClusterParamsExecutor::finalize()
  {
    return true;
  }


}

#endif
