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
#include "CBoolAlgoBase.hh"
#include "GeometryUtilities.hh"

namespace cluster {
  /**
     \class CMalgoStartInPoly
     If start point of one cluster inside other's polygon -> merge
  */
  class CMAlgoStartInPoly : public CBoolAlgoBase{
    
  public:
    
    /// Default constructor
    CMAlgoStartInPoly();
    
    /// Default destructor
    virtual ~CMAlgoStartInPoly(){};
 
    /**
       Core function: given the ClusterParamsAlgNew input, return whether a cluster should be
       merged or not.
    */

    /// Method to set cut value on minimum number of hits considered
    void SetMinHitsCut(int n) { _MinHits = n; }

    void SetDebug(bool debug) { _debug = debug; }

    /// Merging Algorithm is Here
    virtual bool Bool(const ClusterParamsAlgNew &cluster1,
		      const ClusterParamsAlgNew &cluster2);

    /// Method to re-configure the instance
    void reconfigure();

  protected:

    double _wire_2_cm, _time_2_cm; /// Conversion factors ogtten from GeometryUtilities
    int _MinHits; /// Minimum number of hits for cluster whose start point is being considered. We want it to be a good start point...
    bool _debug;
  };
}

#endif
/** @} */ // end of doxygen group 

