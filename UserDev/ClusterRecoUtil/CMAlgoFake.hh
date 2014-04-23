/**
 * \file CMAlgoFake.hh
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class CMAlgoFake
 *
 * @author kazuhiro
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef CMALGOFAKE_HH
#define CMALGOFAKE_HH

#include <iostream>
#include "CMergeAlgoBase.hh"

namespace cluster {
  /**
     \class CMAlgoFake
     An abstract fake class for merging algorithm. Having this fake class helps
     to have a better overall design of various merging for iterative approach.
     The algorithms are run through CMergeManager.
  */
  class CMAlgoFake : public CMergeAlgoBase {
    
  public:
    
    /// Default constructor
    CMAlgoFake();
    
    /// Default destructor
    virtual ~CMAlgoFake(){};
 
    /**
       Core function: given the cluster_params input, return whether a cluster should be
       merged or not.
    */
    virtual bool Merge(const cluster_params &cluster1,
		       const cluster_params &cluster2);

    /// Function to reset the algorithm instance ... maybe implemented via child class
    virtual void Reset(){}

  protected:

    bool _flip;

  };
}

#endif
/** @} */ // end of doxygen group 

