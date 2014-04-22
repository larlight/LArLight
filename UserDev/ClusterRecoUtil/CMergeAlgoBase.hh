/**
 * \file CMergeAlgoBase.hh
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class CMergeAlgoBase
 *
 * @author kazuhiro
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef CMERGEALGOBASE_HH
#define CMERGEALGOBASE_HH

#include <iostream>
#include "ClusterParams.hh"

namespace cluster {
  /**
     \class CMergeAlgoBase
     An abstract base class for merging algorithm. Having this base class helps
     to have a better overall design of various merging for iterative approach.
     The algorithms are run through CMergeManager.
  */
  class CMergeAlgoBase{
    
  public:
    
    /// Default constructor
    CMergeAlgoBase();
    
    /// Default destructor
    virtual ~CMergeAlgoBase(){};
 
    /**
       Core function: given the cluster_params input, return whether a cluster should be
       merged or not.
    */
    virtual bool Merge(const cluster_params &cluster1,
		       const cluster_params &cluster2);

    /// Function to reset the algorithm instance ... maybe implemented via child class
    virtual void Reset(){}

  };
}

#endif
/** @} */ // end of doxygen group 

