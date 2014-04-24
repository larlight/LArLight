#ifndef CMERGEMANAGER_CC
#define CMERGEMANAGER_CC

#include "CMergeManager.hh"

namespace cluster {

  CMergeManager::CMergeManager(CMergePriority_t priority)
  {
    _debug_mode = kNone;
    _merge_till_converge = false;
    _priority = priority;
    Reset();
  }

  void CMergeManager::Reset()
  {
    _merge_algo.clear();
    _ask_and_in_algos.clear();
    _ask_and_in_sets.clear();
    _in_clusters.clear();
    _out_clusters.clear();
    _book_keeper.Reset();
  }

  void CMergeManager::SetClusters(const std::vector<std::vector<larutil::PxHit> > &clusters)
  {

    // Clear & fill cluster info

    _in_clusters.clear();
    _out_clusters.clear();

    _in_clusters.reserve(clusters.size());

    ClusterParamsAlgNew tmp_alg;
    tmp_alg.SetVerbose(false);

    for(auto const &c : clusters) {
      
      _in_clusters.push_back(tmp_alg);
      (*_in_clusters.rbegin()).Initialize();

      if((*_in_clusters.rbegin()).SetHits(c) < 1) continue;
      (*_in_clusters.rbegin()).DisableFANN();
      (*_in_clusters.rbegin()).FillParams(true,true,true,true,true,false);
      (*_in_clusters.rbegin()).FillPolygon();

    }    

    // Clear book keeper
    
    _book_keeper.Reset(clusters.size());

  }
  
  void CMergeManager::Process()
  {
    if(!(_merge_algo.size())) throw RecoUtilException("No algorithm to run!");

    size_t ctr=0;
    bool keep_going=true;
    std::vector<CBookKeeper> book_keepers;
    std::vector<std::vector<unsigned short> > tmp_merged_indexes;
    std::vector<cluster::ClusterParamsAlgNew> tmp_merged_clusters;
    while(keep_going){
      
      // Configure input for RunMerge
      CBookKeeper bk;

      if(!ctr) tmp_merged_clusters = _in_clusters;
      else tmp_merged_clusters = _out_clusters;
      _out_clusters.clear();

      if(_debug_mode <= kPerIteration)
	
	std::cout 
	  << std::endl
	  << Form("  Merging iteration %zu: processing %zu clusters...",ctr,tmp_merged_clusters.size()) 
	  << std::endl;

      std::vector<bool> merge_switch(tmp_merged_clusters.size(),true);
      for(size_t i=0; i<tmp_merged_indexes.size(); ++i)
	
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
	  
	  for(auto const& index : indexes_v) {
	    for(auto const& hit : tmp_merged_clusters.at(index).GetHitVector())
	      tmp_hits.push_back(hit);
	  }
	  _out_clusters.push_back(ClusterParamsAlgNew());
	  (*_out_clusters.rbegin()).SetVerbose(false);
	  (*_out_clusters.rbegin()).DisableFANN();

	  if((*_out_clusters.rbegin()).SetHits(tmp_hits) < 1) continue;
	  (*_out_clusters.rbegin()).FillParams(true,true,true,true,true,false);
	  (*_out_clusters.rbegin()).FillPolygon();
	}
      }

      book_keepers.push_back(bk);

      if(_debug_mode <= kPerIteration) {

	std::cout
	  << Form("  Input / Output cluster length: %zu/%zu",tmp_merged_clusters.size(),_out_clusters.size())
	  << std::endl;

	if(tmp_merged_clusters.size() == _out_clusters.size())

	  std::cout << "  Did not find any newly merged clusters..." <<std::endl;
	
	if(_out_clusters.size()==1)
	  
	  std::cout << "  Output cluster length is 1 (= no more merging needed)" << std::endl;

	if(!_merge_till_converge) 

	  std::cout << "  Non-iterative option: terminating..." << std::endl;

      }

      // Break if necessary
      if(!_merge_till_converge || tmp_merged_clusters.size() == _out_clusters.size() || _out_clusters.size()==1)

	break;

      ctr++;
    }
    
    // Gather the full book keeping result
    for(auto const& bk : book_keepers)

      _book_keeper.Combine(bk);
    
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
      throw RecoUtilException(Form("in_clusters (%zu) and merge_flag (%zu) vectors must be of same length!",
				   in_clusters.size(),
				   merge_flag.size()
				   )
			      );
    if(_debug_mode <= kPerIteration){
      
      std::cout
	<< Form("  Calling %s with %zu clusters...",__FUNCTION__,in_clusters.size())
	<<std::endl;

    }

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

    // Call prepare functions
    for(auto &algo_set : _merge_algo)
      for(auto &algo : algo_set)
	algo->Prepare(in_clusters);
    
    // Run over clusters and execute merging algorithms
    for(auto citer1 = prioritized_index.rbegin();
	citer1 != prioritized_index.rend();
	++citer1) {
      
      auto citer2 = citer1;

      UChar_t plane1 = in_clusters.at((*citer1).second).Plane();

      while(1) {
	citer2++;
	if(citer2 == prioritized_index.rend()) break;

	// Skip if not on the same plane
	UChar_t plane2 = in_clusters.at((*citer2).second).Plane();
	if(plane1 != plane2) continue;

	// Skip if this combination is not meant to be compared
	if(!(merge_flag.at((*citer2).second))) continue;

	bool merge=true;

	for(size_t i=0; merge && (i<_merge_algo.size()); ++i) {

	  if(_debug_mode <= kPerAlgoSet){

	    std::cout
	      << Form("    \033[93mInspecting a pair (%zu, %zu) ... \033[00m",(*citer1).second, (*citer2).second)
	      << std::endl;
	  }

	  // In case this is not i==0
	  if(!i) {

	    // If this set of algos are to be asked in AND condition with the previous... BREAK
	    if(!merge &&  _ask_and_in_sets.at(i)) break;
	    // If this set of algos are to be asked in OR condition with the previous... CONTINUE
	    if(merge  && !_ask_and_in_sets.at(i)) continue;
	    
	  }

	  bool merge_local = true;

	  for(auto algo : _merge_algo.at(i)) {
	    
	    // for 1st algorithm, simply assign merge_local
	    if(algo == (*_merge_algo.at(i).begin()))

	      merge_local = algo->Merge(in_clusters.at((*citer1).second).GetParams(),
					in_clusters.at((*citer2).second).GetParams());

	    else if(_ask_and_in_algos.at(i))

	      merge_local = merge_local && algo->Merge(in_clusters.at((*citer1).second).GetParams(),
						       in_clusters.at((*citer2).second).GetParams());
	      
	    else
	      
	      merge_local = merge_local || algo->Merge(in_clusters.at((*citer1).second).GetParams(),
						       in_clusters.at((*citer2).second).GetParams());
	    
	    if(_debug_mode <= kPerMerging)
	      algo->Report();

	    // AND condition, and this is already false... BREAK
	    if( !merge_local && _ask_and_in_algos.at(i)  ) break;
	    // OR condition, and this is already true... BREAK
	    if(  merge_local && !_ask_and_in_algos.at(i) ) break;

	  } // end looping over algorithms in this set

	  if(!i) merge = merge_local;

	  else if(_ask_and_in_sets.at(i))

	    merge = merge && merge_local;

	  else

	    merge = merge || merge_local;

	}

	if(_debug_mode <= kPerAlgoSet) {
	  
	  if(merge) 
	    std::cout << "    \033[93mfound to be merged!\033[00m " 
		      << std::endl
		      << std::endl;
	  
	  else 
	    std::cout << "    \033[93mfound NOT to be merged...\033[00m" 
		      << std::endl
		      << std::endl;

	} // end looping over all sets of algorithms
	
	if(merge)
	  book_keeper.Merge((*citer1).second,(*citer2).second);
      } // end looping over all cluster pairs for citer1
    } // end looping over clusters
    
    if(_debug_mode <= kPerIteration && book_keeper.GetResult().size() != in_clusters.size()) {

      std::cout << "  Found following clusters to be merged..." << std::endl;
      for(auto const &indexes_v : book_keeper.GetResult()) {

	if(indexes_v.size()==1) continue;
	std::cout<<"    ";
	for(auto index : indexes_v)

	  std::cout<<index<<" ";
	std::cout<<" ... indexes to be merged!"<<std::endl;

      }

    }

  }

}

#endif
