/**
 * \file CBAlgoFake.hh
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class CBAlgoFake
 *
 * @author kazuhiro
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef CBALGOFAKE_HH
#define CBALGOFAKE_HH

#include <iostream>
#include "CBoolAlgoBase.hh"

namespace cmtool {
  /**
     \class CBAlgoFake
     An abstract fake class for merging algorithm. Having this fake class helps
     to have a better overall design of various merging for iterative approach.
     The algorithms are run through CMergeManager.
  */
  class CBAlgoFake : public CBoolAlgoBase {
    
  public:
    
    /// Default constructor
    CBAlgoFake();
    
    /// Default destructor
    virtual ~CBAlgoFake(){};
 
    /**
       Core function: given the ClusterParamsAlgNew input, return whether a cluster should be
       merged or not.
    */
    virtual bool Bool(const ::cluster::ClusterParamsAlgNew &cluster1,
		      const ::cluster::ClusterParamsAlgNew &cluster2);

    /// Function to reset the algorithm instance ... maybe implemented via child class
    virtual void Reset(){}

  protected:

    bool _flip;
    int _ctr;
  };
}

#endif
/** @} */ // end of doxygen group 

