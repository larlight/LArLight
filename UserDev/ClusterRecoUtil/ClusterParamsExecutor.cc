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
    const larutil::Geometry* geo = larutil::Geometry::GetME();
    
    auto clusters = (larlight::event_cluster*)(storage->get_data(_in_cluster_type));
    
    larlight::DATA::DATA_TYPE hit_type = clusters->get_hit_type();
    auto hits = (larlight::event_hit*)(storage->get_data(hit_type));

    if(!clusters || !hits) {
      throw cluster::RecoUtilException("Invalid cluster/hit (didn't find data!)");
      return false;
    }

    for(auto const &i_cluster : *clusters) {

      std::vector<larutil::PxHit> pxhits;
      const std::vector<UShort_t> &hit_index_v = i_cluster.association(hit_type);
      pxhits.reserve(hit_index_v.size());
      for(auto const index : hit_index_v) {
	
	pxhits.push_back(larutil::PxHit());
	(*pxhits.rbegin()).w = hits->at(index).Wire();
	(*pxhits.rbegin()).t = hits->at(index).PeakTime();
	(*pxhits.rbegin()).plane = geo->ChannelToPlane(hits->at(index).Channel());
      }
      
      fCParAlg.Initialize();
      fCParAlg.SetHits(pxhits);
      
    }

    return true;
  }


  bool ClusterParamsExecutor::finalize()
  {
    return true;
  }


}

#endif
