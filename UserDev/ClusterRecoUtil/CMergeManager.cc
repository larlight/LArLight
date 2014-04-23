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
    _in_clusters.clear();
    _out_clusters.clear();
    _book_keeper.Reset();
  }

  void CMergeManager::SetClusters(const std::vector<std::vector<larutil::PxHit> > &clusters)
  {

    // Clear & fill cluster info

    _in_clusters.clear();

    _in_clusters.reserve(clusters.size());

    ClusterParamsAlgNew tmp_alg;

    for(auto const &c : clusters) {
      
      _in_clusters.push_back(tmp_alg);

      (*_in_clusters.rbegin()).Initialize();

      if((*_in_clusters.rbegin()).SetHits(c) < 1) continue;

      (*_in_clusters.rbegin()).FillParams();
      (*_in_clusters.rbegin()).FillPolygon();

    }    

    // Clear book keeper
    
    _book_keeper.Reset(clusters.size());

  }
  
  void CMergeManager::Process(bool merge_till_converge)
  {

    size_t ctr=0;
    bool keep_going=true;
    std::vector<CBookKeeper> book_keepers;
    while(keep_going){
      
      CBookKeeper bk;
      if(!ctr) {
	bk.Reset(_in_clusters.size());
	RunMerge(_in_clusters, bk);
      }

      if(!merge_till_converge) {

	keep_going = false;
	
	book_keepers.push_back(bk);

	ctr++;

	break;

      }

      else if((*book_keepers.rbegin()).GetResult().size() == bk.GetResult().size())

	keep_going=false;
      
      else

	book_keepers.push_back(bk);

      ctr++;
    }
    
  }

  void CMergeManager::RunMerge(const std::vector<cluster::ClusterParamsAlgNew> &in_clusters,
			       CBookKeeper &book_keeper) const
  {
    // Figure out ordering of clusters to process
    std::map<double,size_t> prioritized_index;
    double key = 0;
    
    for(size_t i=0; i<in_clusters.size(); ++i) {

      if(in_clusters.at(i).GetHitVector().size()<1) continue;

      switch(_priority) {
      case ::cluster::CMergeManager::kIndex:
	key = (double)(in_clusters.size()) - (double)i;
	break;
      case ::cluster::CMergeManager::kPolyArea:
	key = in_clusters.at(i).GetParams().PolyObject.Area();
	break;
      case ::cluster::CMergeManager::kChargeSum:
	key = in_clusters.at(i).GetParams().sum_charge;
	break;
      case ::cluster::CMergeManager::kNHits:
	key = (double)(in_clusters.at(i).GetParams().N_Hits);
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

	bool merge=true;

	for(size_t i=0; merge && (i<_merge_algo.size()); ++i) {

	  bool merge_local = false;
	  for(auto algo : _merge_algo.at(i)) {
	    
	    // for 1st algorithm, simply assign merge_local
	    if(algo == (*_merge_algo.at(i).begin()))

	      merge_local = algo->Merge(in_clusters.at((*citer1).second).GetParams(),
					in_clusters.at((*citer2).second).GetParams());

	    else if(_ask_and.at(i))

	      merge_local = merge_local && algo->Merge(in_clusters.at((*citer1).second).GetParams(),
						       in_clusters.at((*citer2).second).GetParams());
	    
	    else
	      
	      merge_local = merge_local || algo->Merge(in_clusters.at((*citer1).second).GetParams(),
						       in_clusters.at((*citer2).second).GetParams());
	    
	  } // end looping over algorithms in this set
	  
	  merge = merge && merge_local;
	  
	} // end looping over all sets of algorithms

	if(merge)
	  book_keeper.Merge((*citer1).second,(*citer2).second);
      } // end looping over all cluster pairs for citer1
    } // end looping over clusters
  }

}

#endif
