#ifndef CMERGEMANAGER_CC
#define CMERGEMANAGER_CC

#include "CMergeManager.hh"

namespace cluster {

  CMergeManager::CMergeManager(CMergePriority_t priority)
  {
    _debug_mode = false;
    _merge_till_converge = true;
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
  
  void CMergeManager::Process()
  {

    size_t ctr=0;
    bool keep_going=true;
    std::vector<CBookKeeper> book_keepers;
    std::vector<std::vector<unsigned short> > tmp_merged_indexes;
    std::vector<cluster::ClusterParamsAlgNew> tmp_merged_clusters;
    while(keep_going){

      // Clear output
      _out_clusters.clear();

      // Configure input for RunMerge
      CBookKeeper bk;

      if(!ctr) tmp_merged_clusters = _in_clusters;
      else tmp_merged_clusters = _out_clusters;

      std::vector<bool> merge_switch(tmp_merged_indexes.size(),true);
      for(size_t i=0; (ctr) && i<tmp_merged_indexes.size(); ++i)
	
	if(tmp_merged_indexes.at(i).size()==1)

	  merge_switch.at(i) = false;

      bk.Reset(tmp_merged_clusters.size());

      // Run algorithm
      RunMerge(tmp_merged_clusters,
	       merge_switch,
	       bk);

      // Save output
      bk.PassResult(tmp_merged_indexes);
      if(bk.size() == tmp_merged_indexes.size())
	_out_clusters = tmp_merged_clusters;
      else {
	_out_clusters.reserve(tmp_merged_indexes.size());
	for(auto const& indexes_v : tmp_merged_indexes) {
	  
	  if(indexes_v.size()==1) {
	    _out_clusters.push_back(tmp_merged_clusters.at(indexes_v.at(0)));
	    continue;
	  }
	      
	  size_t tmp_hit_counts=0;
	  for(auto const& index : indexes_v) 
	    tmp_hit_counts += tmp_merged_clusters.at(index).GetHitVector().size();
	  std::vector<larutil::PxHit> tmp_hits;
	  tmp_hits.reserve(tmp_hit_counts);
	  
	  for(auto const& index : indexes_v)
	    for(auto const& hit : tmp_merged_clusters.at(index).GetHitVector())
	      tmp_hits.push_back(hit);
	  _out_clusters.push_back(ClusterParamsAlgNew());
	  (*tmp_merged_clusters.rbegin()).Initialize();
	  (*tmp_merged_clusters.rbegin()).SetHits(tmp_hits);
	  (*tmp_merged_clusters.rbegin()).FillParams();
	  (*tmp_merged_clusters.rbegin()).FillPolygon();
	}
      }

      book_keepers.push_back(bk);

      // Break if necessary
      if(!_merge_till_converge || tmp_merged_clusters.size() == _out_clusters.size())

	break;

      ctr++;
    }
    
  }

  void CMergeManager::RunMerge(const std::vector<cluster::ClusterParamsAlgNew> &in_clusters,
			       CBookKeeper &book_keeper) const
  {
    RunMerge(in_clusters,
	     std::vector<bool>(in_clusters.size(),true),
	     book_keeper);
  }

  void CMergeManager::RunMerge(const std::vector<cluster::ClusterParamsAlgNew> &in_clusters,
			       const std::vector<bool> &merge_flag,
			       CBookKeeper &book_keeper) const
  {
    if(merge_flag.size() != in_clusters.size())
      throw RecoUtilException("in_clusters and merge_flag vectors must be of same length!");

    // Figure out ordering of clusters to process
    std::multimap<double,size_t> prioritized_index;
    for(size_t i=0; i<in_clusters.size(); ++i) {

      if(in_clusters.at(i).GetHitVector().size()<1) continue;

      switch(_priority) {
      case ::cluster::CMergeManager::kIndex:
	prioritized_index.insert(std::pair<double,size_t>(((double)(in_clusters.size()) - (double)i),i));
	break;
      case ::cluster::CMergeManager::kPolyArea:
	prioritized_index.insert(std::pair<double,size_t>(in_clusters.at(i).GetParams().PolyObject.Area(),i));
	break;
      case ::cluster::CMergeManager::kChargeSum:
	prioritized_index.insert(std::pair<double,size_t>(in_clusters.at(i).GetParams().sum_charge,i));
	break;
      case ::cluster::CMergeManager::kNHits:
	prioritized_index.insert(std::pair<double,size_t>((double)(in_clusters.at(i).GetParams().N_Hits),i));
	break;
      }
    }
    
    // Run over clusters and execute merging algorithms
    for(auto citer1 = prioritized_index.rbegin();
	citer1 != prioritized_index.rend();
	++citer1) {
      
      auto citer2 = citer1;
      while(1) {
	citer2++;
	if(citer2 == prioritized_index.rend()) break;

	// Skip if this combination is not meant to be compared
	if(!(merge_flag.at((*citer2).second))) continue;

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
