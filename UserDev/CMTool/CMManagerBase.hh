/**
 * \file CMManagerBase.hh
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CMManagerBase
 *
 * @author kazuhiro
 */

/** \addtogroup CMTool

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
    void AddPriorityAlgo(CPriorityAlgoBase* algo) { _priority_algo = algo; }

    /// Switch to continue merging till converges
    void MergeTillConverge(bool doit=true) {_merge_till_converge = doit;}

    /// A simple method to add a cluster
    void SetClusters(const std::vector<std::vector<larutil::PxHit> > &clusters);

    /// A simple method to add a cluster
    void SetClusters(const std::vector<cluster::ClusterParamsAlgNew> &clusters);

    /// A getter for input clusters
    const std::vector<cluster::ClusterParamsAlgNew>&  GetInputClusters() const 
    { return _in_clusters; }

    /// A setter for minimum # of hits ... passed onto ClusterParamsAlg
    void SetMinNHits(unsigned int n) { _min_nhits = n; }

    /// A method to execute the main action, to be called per event
    void Process();

    /// A setter for an analysis output file
    void SetAnaFile(TFile* fout) { _fout = fout; }

  protected:

    /// Function to compute priority
    void ComputePriority(const std::vector<cluster::ClusterParamsAlgNew>& clusters);

    /// FMWK function called @ beginning of Process()
    virtual void EventBegin(){}

    /// FMWK function called @ beginning of iterative loop inside Process()
    virtual void IterationBegin(){}

    /// FMWK function called @ iterative loop inside Process()
    virtual bool IterationProcess()=0;

    /// FMWK function called @ end of iterative loop inside Process()
    virtual void IterationEnd(){}

    /// FMWK function called @ end of Process()
    virtual void EventEnd(){}

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

    /// Iteration loop switch
    bool _merge_till_converge;

    /// A holder for # of unique planes in the clusters, computed in ComputePriority() function
    std::set<UChar_t> _planes;

  };
}

#endif
/** @} */ // end of doxygen group 

