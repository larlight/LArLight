/**
 * \file CMManagerBase.hh
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class CMManagerBase
 *
 * @author kazuhiro
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef CMMANAGERBASE_HH
#define CMMANAGERBASE_HH

#include <iostream>

#include "ClusterParamsAlgNew.hh"
#include "CPriorityAlgoBase.hh"

namespace cmtool {

  /**
     \class CMManagerBase
     A class that instantiates merging algorithm(s) and run.
     The book-keeping of merged cluster sets are done by CMergeBookKeeper.
  */
  class CMManagerBase{

  public:

    /// Enum to specify message output level
    enum CMMSGLevel_t {
      /// Extremely verbose (cout per individual algorithm execution)
      kPerMerging,
      /// Somewhat verbose (cout per merging iteration)
      kPerIteration,
      /// Bit verbose (cout per event)
      kPerEvent,
      /// Normal
      kNone
    };
    
  public:
    
    /// Default constructor
    CMManagerBase();
    
    /// Default destructor
    virtual ~CMManagerBase(){};

    /// Method to enable debug mode (lots of couts)
    void DebugMode(CMMSGLevel_t level) {_debug_mode=level;}

    /// Method to reset itself
    void Reset();

    /// Setter to add an algorithm for priority determination
    void AddPriorityAlgo(CPriorityAlgoBase* algo);

    /// A simple method to add a cluster
    void SetClusters(const std::vector<std::vector<larutil::PxHit> > &clusters);

    /// A simple method to add a cluster
    void SetClusters(const std::vector<cluster::ClusterParamsAlgNew> &clusters);

    /// A setter for minimum # of hits ... passed onto ClusterParamsAlg
    void SetMinNHits(unsigned int n) { _min_nhits = n; }

    /// A method to execute the main action
    virtual void Process()=0;

    /// A setter for an analysis output file
    void SetAnaFile(TFile* fout) { _fout = fout; }

  protected:

    /// Function to compute priority
    void ComputePriority(const std::vector<cluster::ClusterParamsAlgNew> &clusters);

  protected:

    /// Minimum number of hits: the limit set for ClusterParamsAlgNew
    unsigned int _min_nhits;

    /// Debug mode switch
    CMMSGLevel_t _debug_mode;

    /// Input clusters
    std::vector<cluster::ClusterParamsAlgNew> _in_clusters;

    /// Priority algorithm
    ::cmtool::CPriorityAlgoBase* _priority_algo;

    /// Output analysis plot TFile
    TFile* _fout;

    /// Priority record 
    std::multimap<float,size_t> _priority;

  };
}

#endif
/** @} */ // end of doxygen group 

