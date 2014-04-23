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
    
  public:
    
    /// Default constructor
    CMergeManager(CMergePriority_t priority = kNHits);
    
    /// Default destructor
    virtual ~CMergeManager(){};

    /// Method to reset itself
    void Reset();

    /// A simple method to add a one merging step
    void AddAlgo(CMergeAlgoBase* alg) 
    { 
      // By default we ask AND condition
      AddAlgo(std::vector<cluster::CMergeAlgoBase*>(1,alg),true);
    }

    /// A simple method to add a group of algorithms
    void AddAlgo(std::vector<cluster::CMergeAlgoBase* > alg_v, bool ask_and)
    {
      _merge_algo.push_back(alg_v);
      _ask_and.push_back(ask_and);
    }

    /// A simple method to add a cluster
    void SetClusters(const std::vector<std::vector<larutil::PxHit> > &clusters);

    /// A method to execute merging algorithms
    void Process(bool merge_till_converge=false);

  protected:

    void RunMerge(const std::vector<cluster::ClusterParamsAlgNew > &in_clusters,
		  CBookKeeper &book_keeper) const;

  protected:

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
       for the specified set of algorithms in _merge_algo.
     */
    std::vector<bool> _ask_and;

    /// Merging priority type
    CMergePriority_t _priority;

  };
}

#endif
/** @} */ // end of doxygen group 

