/**
 * \file CMalgoPolyStartInPoly.hh
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class CMAlgoStartInPoly
 *
 * @author David Caratelli
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef CMALGOSTARTINPOLY_HH
#define CMALGOSTARTINPOLY_HH

#include <iostream>
#include "CMergeAlgoBase.hh"
#include "GeometryUtilities.hh"

namespace cluster {
  /**
     \class CMalgoStartInPoly
     If start point of one cluster inside other's polygon -> merge
  */
  class CMAlgoStartInPoly : public CMergeAlgoBase{
    
  public:
    
    /// Default constructor
    CMAlgoStartInPoly();
    
    /// Default destructor
    virtual ~CMAlgoStartInPoly(){};
 
    /**
       Core function: given the cluster_params input, return whether a cluster should be
       merged or not.
    */

    /// Method to set cut value on minimum number of hits considered
    void SetMinHitsCut(int n) { _MinHits = n; }


    /// Merging Algorithm is Here
    virtual bool Merge(const cluster_params &cluster1,
		       const cluster_params &cluster2);

    /// Method to re-configure the instance
    void reconfigure();

  protected:

    double _wire_2_cm, _time_2_cm; /// Conversion factors ogtten from GeometryUtilities
    int _MinHits; /// Minimum number of hits for cluster whose start point is being considered. We want it to be a good start point...

  };
}

#endif
/** @} */ // end of doxygen group 

