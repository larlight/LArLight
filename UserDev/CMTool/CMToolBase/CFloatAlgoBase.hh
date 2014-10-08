/**
 * \file CFloatAlgoBase.hh
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class CAlgoBase, CFloatAlgoBase and CSeparateAlgoBase
 *
 * @author kazuhiro
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef CFLOATALGOBASE_HH
#define CFLOATALGOBASE_HH

#include "CMAlgoBase.hh"

namespace cmtool {

  /**
     \class CFloatAlgoBase
     An abstract base class for mathcing algorithm. Having this base class helps
     to have a better overall design of various merging for iterative approach.
     The algorithms are run through CMergeManager.
  */
  class CFloatAlgoBase : public CMAlgoBase {
    
  public:
    
    /// Default constructor
    CFloatAlgoBase(){}
    
    /// Default destructor
    virtual ~CFloatAlgoBase(){}

    /**
       Core function: given the CPAN input, return whether a cluster should be
       merged or not.
    */
    virtual float Float(const std::vector<const cluster::ClusterParamsAlg*> &clusters)
    { 
      if(clusters.size()) return 1;
      else return -1;
    }

  };

}

#endif
/** @} */ // end of doxygen group 

