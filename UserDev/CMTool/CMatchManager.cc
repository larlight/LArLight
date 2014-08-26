#ifndef CMATCHMANAGER_CXX
#define CMATCHMANAGER_CXX

#include "CMatchManager.hh"

namespace cmtool {

  CMatchManager::CMatchManager() : CMManagerBase()
  {
    throw CMTException("Default ctor needs # planes as an argument!");
  }

  CMatchManager::CMatchManager(size_t nplanes) : CMManagerBase()
  {
    _match_algo = nullptr;
    _nplanes    = nplanes;
    Reset();
  }

  void CMatchManager::Reset()
  {
    CMManagerBase::Reset();
    if(_match_algo) _match_algo->Reset();
    if(_priority_algo) _priority_algo->Reset();
    _book_keeper.Reset();
  }

  void CMatchManager::EventBegin()
  {
    if(_debug_mode <= kPerMerging) {
      if(_match_algo)    _match_algo->SetVerbose(true);
      //if(_priority_algo) _priority_algo->SetVerbose(true);
      _priority_algo->SetVerbose(true);
    }

    _match_algo->IterationBegin(_in_clusters);
    if(_priority_algo) _priority_algo->IterationBegin(_in_clusters);    
  }
  
  void CMatchManager::IterationBegin()
  {
    _match_algo->IterationBegin(_in_clusters);
    if(_priority_algo) _priority_algo->IterationBegin(_in_clusters);
  }
  
  void CMatchManager::IterationEnd()
  {
    _match_algo->IterationEnd();
    if(_priority_algo) _priority_algo->IterationEnd();
  }

  void CMatchManager::EventEnd()
  {
    _match_algo->EventEnd();
    if(_priority_algo) _priority_algo->EventEnd();
  }

  bool CMatchManager::IterationProcess()
  {

    //
    // Create plane-by-plane vectors
    //
    ComputePriority(_in_clusters);

    if(_planes.size()<2) return false;

    if(_planes.size() > _nplanes)

      throw CMTException("Found more plane IDs than specified number of planes!");

    // Index array of clusters per plane
    std::vector<std::vector<size_t> > cluster_array;

    cluster_array.resize( _planes.size() );
    
    for(auto riter = _priority.rbegin();
	riter != _priority.rend();
	++riter) 

      cluster_array.at( _in_clusters.at((*riter).second).Plane()).push_back((*riter).second);


    // Loop over possible combinations
    std::vector<size_t> ctr(cluster_array.size(),0);
    std::vector<unsigned int> tmp_index_v(cluster_array.size(),0);
    std::vector<const cluster::ClusterParamsAlg*> ptr_v;

    while(1) {
      
      ptr_v.clear();
      ptr_v.reserve(_nplanes);

      auto plane_id_iter = _planes.begin();
      for(size_t plane=0; plane<ctr.size(); ++plane) {

	size_t cluster_index = cluster_array.at(plane).at(ctr.at(plane));

	tmp_index_v.at(plane) = (unsigned int)cluster_index;

	while( plane_id_iter != _planes.end() && (*plane_id_iter) > ptr_v.size()) {

	  ptr_v.push_back(nullptr);
	  ++plane_id_iter;
	}

	ptr_v.push_back( (&(_in_clusters.at(cluster_index))) );

      }
      while( plane_id_iter != _planes.end() && ptr_v.size() < _nplanes) {
	
	ptr_v.push_back(nullptr);
	++plane_id_iter;
      }

      float score = _match_algo->Float(ptr_v);

      if(score>0)
	
	_book_keeper.Match(tmp_index_v,score);

      for(size_t i=0; i<ctr.size(); ++i) {

	size_t index = (size_t)(ctr.size()-i-1);

	ctr.at(index) +=1;

	if(ctr.at(index) < cluster_array.at(index).size())

	  break;
	    
	ctr.at(index) = 0;
      }

      bool abort = true;
      for(auto const& value : ctr)

	abort = abort && (!(value));
      
      if(abort) break;
    }

    if(_debug_mode <= kPerIteration) {
      if(_match_algo) _match_algo->Report();
      if(_priority_algo) _priority_algo->Report();
    }
    
    return false;
  }

}

#endif
