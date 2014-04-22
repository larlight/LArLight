#ifndef CMERGEMANAGER_CC
#define CMERGEMANAGER_CC

#include "CMergeManager.hh"

namespace cluster {

  CMergeManager::CMergeManager(CMergePriority_t priority)
  {
    _priority = priority;
    Reset();
  }

  void CMergeManager::Reset()
  {
    _merge_algo.clear();
    _ask_and.clear();
    _clusters.clear();
    _book_keeper.Reset();
  }

  void CMergeManager::SetClusters(const std::vector<std::vector<larutil::PxHit> > &clusters)
  {

    // Clear & fill cluster info

    _clusters.clear();
    _prioritized_index.clear();

    // Clear book keeper
    
    _book_keeper.Reset(clusters.size());

  }
  
  void CMergeManager::Process()
  {
    
  }

}

#endif
