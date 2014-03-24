#ifndef CLUSTERMERGEALGNEW_CC
#define CLUSTERMERGEALGNEW_CC

#include "ClusterMergeAlgNew.hh"

namespace cluster {

  ClusterMergeAlgNew::ClusterMergeAlgNew() : fAlgoSwitch(kAlgoMax,false)
  {
    
    fAlgoSwitch.at(kStartPoint) = true;
    fAlgoSwitch.at(kPolygonCollision) = true;

  }

  bool ClusterMergeAlgNew::Merge(const cluster::cluster_params &param_a, 
				 const cluster::cluster_params &param_b)
  {    
    
    bool merge = true;

    if(merge && fAlgoSwitch.at(kStartPoint))

      merge = TestStartPoint(param_a, param_b);

    if(merge && fAlgoSwitch.at(kPolygonCollision))

      merge = TestPolygonCollision(param_a, param_b);
    
    return merge;
  }

}

#endif
