/**
 * \file CBAlgoMergeAll.hh
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class CBAlgoMergeAll
 *
 * @author david caratelli
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef CBALGOMERGEALL_HH
#define CBALGOMERGEALL_HH

#include <iostream>
#include "CBoolAlgoBase.hh"

namespace cmtool {
  /**
     \class CBAlgoMergeAll
     Merges all clusters: maybe useful to test how well a cluster-separating
     algorithm has performed
  */
  class CBAlgoMergeAll: public CBoolAlgoBase {
    
  public:
    
    /// Default constructor
    CBAlgoMergeAll();
    
    /// Default destructor
    virtual ~CBAlgoMergeAll(){};
 
    /**
       Core function: given the ClusterParamsAlg input, return whether a cluster should be
       merged or not.
    */
    virtual bool Bool(const ::cluster::ClusterParamsAlg &cluster1,
		      const ::cluster::ClusterParamsAlg &cluster2);

    /// Function to reset the algorithm instance ... maybe implemented via child class
    virtual void Reset(){}

  protected:

  };
}

#endif
/** @} */ // end of doxygen group 

