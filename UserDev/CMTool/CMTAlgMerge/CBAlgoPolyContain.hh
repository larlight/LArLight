/**
 * \file CMalgoPolyContain.hh
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class CBAlgoPolyContain
 *
 * @author David Caratelli
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef CBALGOPOLYCONTAIN_HH
#define CBALGOPOLYCONTAIN_HH

#include <iostream>
#include "CBoolAlgoBase.hh"
#include "GeometryUtilities.hh"

namespace cmtool {
  /**
     \class CMalgoPolyContain
     Merge Polygons if one is completely inside the other
  */
  class CBAlgoPolyContain : public CBoolAlgoBase{
    
  public:
    
    /// Default constructor
    CBAlgoPolyContain();
    
    /// Default destructor
    virtual ~CBAlgoPolyContain(){};
 
    /**
       Core function: given the ClusterParamsAlg input, return whether a cluster should be
       merged or not.
    */
    virtual bool Bool(const ::cluster::ClusterParamsAlg &cluster1,
		      const ::cluster::ClusterParamsAlg &cluster2);

    /// Method to re-configure the instance
    void reconfigure();

  };
}

#endif
/** @} */ // end of doxygen group 

