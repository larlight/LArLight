/**
 * \file CMalgoPolyOverlap.hh
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class CMAlgoPolyOverlap
 *
 * @author David Caratelli
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef CMALGOPOLYOVERLAP_HH
#define CMALGOPOLYOVERLAP_HH

#include <iostream>
#include "CMergeAlgoBase.hh"
#include "GeometryUtilities.hh"


namespace cluster {
  /**
     \class CMalgoPolyContain
     Merge Polygons if the two overlap even partially
  */
  class CMAlgoPolyOverlap : public CMergeAlgoBase{
    
  public:
    
    /// Default constructor
    CMAlgoPolyOverlap();
    
    /// Default destructor
    virtual ~CMAlgoPolyOverlap(){};
 
    /**
       Core function: given the cluster_params input, return whether a cluster should be
       merged or not.
    */
    virtual bool Merge(const cluster_params &cluster1,
		       const cluster_params &cluster2);

    /// Method to re-configure the instance
    void reconfigure();

  };
}

#endif
/** @} */ // end of doxygen group 

