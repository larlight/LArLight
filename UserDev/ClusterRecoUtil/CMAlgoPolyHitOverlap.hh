/**
 * \file CMalgoPolyHitOverlap.hh
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class CMAlgoPolyHitOverlap
 *
 * @author David Caratelli
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef CMALGOPOLYHITOVERLAP_HH
#define CMALGOPOLYHITOVERLAP_HH

#include <iostream>
#include "CMergeAlgoBase.hh"
#include "GeometryUtilities.hh"

namespace cluster {
  /**
     \class CMalgoPolyContain
     Merge Polygons if one is completely inside the other
  */
  class CMAlgoPolyHitOverlap : public CMergeAlgoBase{
    
  public:
    
    /// Default constructor
    CMAlgoPolyHitOverlap();
    
    /// Default destructor
    virtual ~CMAlgoPolyHitOverlap(){};
 
    /**
       Core function: given the ClusterParamsAlgNew input, return whether a cluster should be
       merged or not.
    */
    virtual bool Merge(const ClusterParamsAlgNew &cluster1,
		       const ClusterParamsAlgNew &cluster2);

    /// Method to re-configure the instance
    void reconfigure();

  };
}

#endif
/** @} */ // end of doxygen group 

