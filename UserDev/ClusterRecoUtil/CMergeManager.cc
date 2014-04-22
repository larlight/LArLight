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

    _clusters.reserve(clusters.size());

    ClusterParamsAlgNew tmp_alg;

    for(auto const &c : clusters) {
      
      _clusters.push_back(tmp_alg);

      (*_clusters.rbegin()).Initialize();

      if((*_clusters.rbegin()).SetHits(c) < 1) continue;

      (*_clusters.rbegin()).FillParams();
      (*_clusters.rbegin()).FillPolygon();

    }    

    // Clear book keeper
    
    _book_keeper.Reset(clusters.size());

  }
  
  void CMergeManager::Process()
  {
    // Figure out ordering of clusters to process
    std::map<double,size_t> prioritized_index;
    double key = 0;
    
    for(size_t i=0; i<_clusters.size(); ++i) {

      if(_clusters.at(i).GetHitVector().size()<1) continue;

      switch(_priority) {
      case ::cluster::CMergeManager::kIndex:
	key = (double)(_clusters.size()) - (double)i;
	break;
      case ::cluster::CMergeManager::kPolyArea:
	key = _clusters.at(i).GetParams().PolyObject.Area();
	break;
      case ::cluster::CMergeManager::kChargeSum:
	key = _clusters.at(i).GetParams().sum_charge;
	break;
      case ::cluster::CMergeManager::kNHits:
	key = (double)(_clusters.at(i).GetParams().N_Hits);
	break;
      }

      while(prioritized_index.find(key) != prioritized_index.end())

	key-=1.e-9;

      prioritized_index.insert(std::pair<double,size_t>(key,i));
      
    }


    // Run over clusters and execute merging algorithms
    for(auto citer1 = prioritized_index.rbegin();
	citer1 != prioritized_index.rend();
	++citer1) {

      auto citer2 = citer1;
      while(1) {
	citer2++;
	if(citer2 == prioritized_index.rend()) break;


	for(size_t i=0; i<_merge_algo.size(); ++i) {
	  
	  bool status = true;
	  
	  for(auto algo : _merge_algo.at(i)) {
	    
	    
	    
	  } // end looping over algorithms in this set
	} // end looping over all sets of algorithms
      } // end looping over all cluster pairs for citer1
    } // end looping over clusters
  }

}

#endif
