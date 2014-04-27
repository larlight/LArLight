/**
 * \file CBoolAlgoBase.hh
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class CAlgoBase, CBoolAlgoBase and CSeparateAlgoBase
 *
 * @author kazuhiro
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef CBOOLALGOBASE_HH
#define CBOOLALGOBASE_HH

#include <iostream>
#include "ClusterParamsAlgNew.hh"

namespace cluster {

  /**
     \class CBoolAlgoBase
     An abstract base class for merging/mathcing algorithm. Having this base class helps
     to have a better overall design of various merging for iterative approach.
     The algorithms are run through CMergeManager.
  */
  class CBoolAlgoBase {
    
  public:
    
    /// Default constructor
    CBoolAlgoBase(){}
    
    /// Default destructor
    virtual ~CBoolAlgoBase(){}

    /// Function to reset the algorithm instance ... maybe implemented via child class
    virtual void Reset(){}
 
    /**
       Optional function: called at the beggining of iterating over all pairs of clusters. 
       This provides all clusters' information in case the algorithm need them 
     */
    virtual void Prepare(const std::vector<cluster::ClusterParamsAlgNew> &clusters)
    { return; }

    /**
       Core function: given the CPAN input, return whether a cluster should be
       merged or not.
    */
    virtual bool Bool(const ClusterParamsAlgNew &cluster1,
		      const ClusterParamsAlgNew &cluster2)
    {return false;}

    /**
       Optional function: called after each Merge() function call by CMergeManager IFF
       CMergeManager is run with verbosity level kPerMerging. Maybe useful for debugging.
     */
    virtual void Report() {return;}

  };

}

#endif
/** @} */ // end of doxygen group 

