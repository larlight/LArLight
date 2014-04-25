/**
 * \file CMergeManager.hh
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class CMergeManager
 *
 * @author kazuhiro
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef CMERGEMANAGER_HH
#define CMERGEMANAGER_HH

#include <iostream>

#include "ClusterParamsAlgNew.hh"
#include "CBookKeeper.hh"
#include "CMergeAlgoBase.hh"

namespace cluster {

  /**
     \class CMergeManager
     A class that instantiates merging algorithm(s) and run.
     The book-keeping of merged cluster sets are done by CBookKeeper.
  */
  class CMergeManager{

  public:

    /// Enum to specify the priority for running a merging.
    enum CMergePriority_t {
      /// Merge clusters along the vector index (from low to high)
      kIndex,
      /// Merge clusters with large areas first 
      kPolyArea,
      /// Merge clusters with high charge first
      kChargeSum,
      /// Merge clusters with many hits first
      kNHits
    };

    /// Enum to specify message output level
    enum CMergeMSGLevel_t {
      /// Extremely verbose (cout per individual algorithm execution)
      kPerMerging,
      /// Very verbose (cout per set of algorithms execution)
      kPerAlgoSet,
      /// Somewhat verbose (cout per merging iteration)
      kPerIteration,
      /// Bit verbose (cout per event)
      kPerEvent,
      /// Normal
      kNone
    };
    
  public:
    
    /// Default constructor
    CMergeManager(CMergePriority_t priority = kNHits);
    
    /// Default destructor
    virtual ~CMergeManager(){};

    /// Method to enable debug mode (lots of couts)
    void DebugMode(CMergeMSGLevel_t level) {_debug_mode=level;}

    /// Switch to continue merging till converges
    void MergeTillConverge(bool doit=true) {_merge_till_converge = doit;}

    /// Choose ordering for clusters to be merged
    void SetMergePriority(CMergePriority_t level) { _priority=level; }

    /// Method to reset itself
    void Reset();

    /// A simple method to add a one merging step
    void AddAlgo(CMergeAlgoBase* alg,
		 bool ask_and_in_algos = true,
		 bool ask_and_in_sets  = true) 
    { 
      // By default we ask AND condition
      AddAlgo(std::vector<cluster::CMergeAlgoBase*>(1,alg),
	      ask_and_in_algos,
	      ask_and_in_sets);
    }

    /// A simple method to add a group of algorithms
    void AddAlgo(std::vector<cluster::CMergeAlgoBase* > alg_v, 
		 bool ask_and_in_algos = true,
		 bool ask_and_in_sets = true)
    {
      _merge_algo.push_back(alg_v);
      _ask_and_in_algos.push_back(ask_and_in_algos);
      _ask_and_in_sets.push_back(ask_and_in_sets);
    }

    /// A simple method to add a cluster
    void SetClusters(const std::vector<std::vector<larutil::PxHit> > &clusters);

    /// A method to execute merging algorithms
    void Process();

    /// A method to obtain output clusters
    const std::vector<cluster::ClusterParamsAlgNew>& GetClusters() const { return _out_clusters; }

    /// A method to obtain book keeper
    const CBookKeeper& GetBookKeeper() const { return _book_keeper; }

  protected:

    void RunMerge(const std::vector<cluster::ClusterParamsAlgNew > &in_clusters,
		  CBookKeeper &book_keeper) const;

    void RunMerge(const std::vector<cluster::ClusterParamsAlgNew > &in_clusters,
		  const std::vector<bool> &merge_flag,
		  CBookKeeper &book_keeper) const;

  protected:

    /// Iterative approach for merging
    bool _merge_till_converge;

    /// Debug mode switch
    CMergeMSGLevel_t _debug_mode;

    /// Input clusters
    std::vector<cluster::ClusterParamsAlgNew> _in_clusters;

    /// Output clusters
    std::vector<cluster::ClusterParamsAlgNew> _out_clusters;

    /// Book keeper instance
    CBookKeeper _book_keeper;

    /**
       A list of algorithms to be run over.
       It's a vector of vector of algorithms.
       Each of the inner vector contents can be applied with either AND or OR condition
       which is set in _ask_and vector. 
     */
    std::vector<std::vector<cluster::CMergeAlgoBase*> > _merge_algo;

    /**
       A boolean vector that holds user configuration of whether asking AND or OR condition
       among algorithm in the specified set in _merge_algo
     */
    std::vector<bool> _ask_and_in_algos;

    /**
       A boolean vector that holds user configuration of whether asking AND or OR condition
       w.r.t. the previous set of algorithms in _merge_algo
     */
    std::vector<bool> _ask_and_in_sets;

    /// Merging priority type
    CMergePriority_t _priority;

  };
}

#endif
/** @} */ // end of doxygen group 

