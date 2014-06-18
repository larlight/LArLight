#ifndef CMATCHMANAGER_CXX
#define CMATCHMANAGER_CXX

#include "CMatchManager.hh"

namespace cmtool {

  CMatchManager::CMatchManager() : CMManagerBase()
  {
    _match_algo = nullptr;
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

    if(_planes.size() != ((*_planes.rbegin())+1))
 
      throw CMTException("Some planes do not contain any cluster!");

    std::vector<std::vector<size_t> > cluster_array;

    cluster_array.resize( _planes.size() );
    
    for(auto riter = _priority.rbegin();
	riter != _priority.rend();
	++riter) 

      cluster_array.at( _in_clusters.at((*riter).second).Plane()).push_back((*riter).second);


    // Loop over possible combinations
    std::vector<size_t> ctr(cluster_array.size(),0);
    std::vector<unsigned int> tmp_index_v(cluster_array.size(),0);
    std::vector<const cluster::ClusterParamsAlgNew*> ptr_v;

    while(1) {
      
      ptr_v.clear();
      ptr_v.reserve(cluster_array.size());

      for(size_t plane=0; plane<ctr.size(); ++plane) {

	size_t cluster_index = cluster_array.at(plane).at(ctr.at(plane));

	tmp_index_v.at(plane) = (unsigned int)cluster_index;
	ptr_v.push_back( (&(_in_clusters.at(cluster_index))) );

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

    return false;
  }

}

#endif
