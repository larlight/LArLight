/**
 * \file CBoolAlgoBase.hh
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for algorithm classes for CMergeManager
 *
 * @author kazuhiro
 */

/** \addtogroup CMTool

    @{*/
#ifndef CBOOLALGOBASE_HH
#define CBOOLALGOBASE_HH

#include "CMAlgoBase.hh"

namespace cmtool {

  /**
     \class CBoolAlgoBase
     An abstract base class for merging algorithm. Having this base class helps
     to have a better overall design of various merging for iterative approach.
     The algorithms are run through CMergeManager.
  */
  class CBoolAlgoBase : public CMAlgoBase {
    
  public:
    
    /// Default constructor
    CBoolAlgoBase(){}
    
    /// Default destructor
    virtual ~CBoolAlgoBase(){}

    /**
       Core function: given the CPAN input, return whether a cluster should be
       merged or not.
    */
    virtual bool Bool(const ::cluster::ClusterParamsAlgNew &cluster1,
		      const ::cluster::ClusterParamsAlgNew &cluster2)
    { if(cluster1.Plane() != cluster2.Plane()) return false;
      else return true;
    }

  };

}

#endif
/** @} */ // end of doxygen group 

