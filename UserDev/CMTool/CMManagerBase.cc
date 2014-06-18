#ifndef CMMANAGERBASE_CXX
#define CMMANAGERBASE_CXX

#include "CMManagerBase.hh"

namespace cmtool {

  CMManagerBase::CMManagerBase()
  {
    _fout = 0;
    _debug_mode = kNone;
    _priority_algo = nullptr;
    _min_nhits = 0;
    _merge_till_converge = false;
    Reset();
  }

  void CMManagerBase::Reset()
  {
    _planes.clear();
    _in_clusters.clear();
    if(_priority_algo) _priority_algo->Reset();
  }

  void CMManagerBase::SetClusters(const std::vector<std::vector<larutil::PxHit> > &clusters)
  {

    // Clear & fill cluster info

    _in_clusters.clear();

    _in_clusters.reserve(clusters.size());

    ::cluster::ClusterParamsAlgNew tmp_alg;
    tmp_alg.SetMinNHits(_min_nhits);
    tmp_alg.SetVerbose(false);

    for(auto const &c : clusters) {
      
      _in_clusters.push_back(tmp_alg);
      (*_in_clusters.rbegin()).Initialize();

      if((*_in_clusters.rbegin()).SetHits(c) < 1) continue;
      (*_in_clusters.rbegin()).DisableFANN();
      (*_in_clusters.rbegin()).FillParams(true,true,true,true,true,false);
      (*_in_clusters.rbegin()).FillPolygon();

    }
    
  }

  void CMManagerBase::SetClusters(const std::vector<cluster::ClusterParamsAlgNew> &clusters)
  {
    _in_clusters = clusters;
  }

  void CMManagerBase::Process()
  {
    
    if(!(_in_clusters.size()))

      return;
    
    EventBegin();

    bool keep_going = true;

    while(keep_going) {

      IterationBegin();

      keep_going = IterationProcess();

      IterationEnd();

      if(!_merge_till_converge)

	break;
    }

    if(!keep_going && _debug_mode >= kPerIteration)
      
      std::cout << "\033[93m  Iterative approach = OFF ... exiting from iteration loop. \033[00m" << std::endl;

    EventEnd();

  }

  void CMManagerBase::ComputePriority(const std::vector<cluster::ClusterParamsAlgNew> &clusters) {

    _priority.clear();
    _planes.clear();

    if(!clusters.size()) return;

    // Priority is computed cluster-by-cluster. In case of two clusters having the same priority 
    // value the one with lower cluster index gets the priority. Also, clusters with priority < 0
    // are not logged (assumed not to be used)

    for(size_t i=0; i<clusters.size(); ++i) {

      size_t c_index = clusters.size() - i - 1;

      float priority = clusters.at(c_index).GetNHits();
      
      if(_priority_algo) {
	
	priority = _priority_algo->Priority(clusters.at(c_index));

      }
	
      if(priority>0) {
	
	_priority.insert(std::make_pair(priority,c_index));

	if( _planes.find(clusters.at(c_index).Plane()) == _planes.end() )

	  _planes.insert(clusters.at(c_index).Plane());

      }
      
    }
    
  }


}

#endif
