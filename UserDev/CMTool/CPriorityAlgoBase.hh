/**
 * \file CPriorityAlgoBase.hh
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class CAlgoBase, CPriorityAlgoBase and CSeparateAlgoBase
 *
 * @author kazuhiro
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef CPRIORITYALGOBASE_HH
#define CPRIORITYALGOBASE_HH

#include "CMAlgoBase.hh"

namespace cmtool {

  /**
     \class CPriorityAlgoBase
     An abstract base class for CMatchManager and CMergeManager to determine 
     cluster "priority" for matching and merging action respectively.
  */
  class CPriorityAlgoBase : public CMAlgoBase {
    
  public:
    
    /// Default constructor
    CPriorityAlgoBase(){}
    
    /// Default destructor
    virtual ~CPriorityAlgoBase(){}

    /**
       Core function: given the CPAN input, return whether a cluster should be
       merged or not.
    */
    virtual float Priority(const cluster::ClusterParamsAlgNew &cluster)
    { 
      if(cluster.GetNHits()) return 1.;
      return 0.1;
    }

  };

}

#endif
/** @} */ // end of doxygen group 

