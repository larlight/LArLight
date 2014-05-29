/**
 * \file CMAlgoCenterOfMass.hh
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class CMAlgoMergeAll
 *
 * @author david caratelli
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef CMALGOCENTEROFMASS_HH
#define CMALGOCENTEROFMASS_HH

#include <iostream>
#include "CBoolAlgoBase.hh"
#include <math.h>

namespace cluster {
  /**
     \class CMAlgoCenterOfMass
     Designed mainly for small clusters (< 10 hits):
     Find Cluster's center of mass weighing by Q
     Then see if COM is in polygon/cone of a big cluster
     If so -> merge
  */
  class CMAlgoCenterOfMass: public CBoolAlgoBase {
    
  public:
    
    /// Default constructor
    CMAlgoCenterOfMass();
    
    /// Default destructor
    virtual ~CMAlgoCenterOfMass(){};
 
    /**
       Core function: given the ClusterParamsAlgNew input, return whether a cluster should be
       merged or not.
    */
    virtual bool Bool(const ClusterParamsAlgNew &cluster1,
		      const ClusterParamsAlgNew &cluster2);

    /// Function to reset the algorithm instance ... maybe implemented via child class
    virtual void Reset(){}

    /// Function to report what's going on per merging
    virtual void Report();

    /// Function to set Verbosity of output
    void SetVerbose(bool on) { _verbose = on; }

    /// Function to set Debug mode of output
    void SetDebug(bool on) { _debug = on; }

    /// Function to set Max hits for small clsuters
    void SetMaxHitsSmallClus(int n) { _maxHits = n; }

    /// Function to se Min hits for big clusters
    void SetMinHitsBigClus(int n) { _minHits = n; }

    /// Function to set Max Distance for COM to be from start-end
    void SetMaxDistance(double d) { _MaxDist = d; }

    double ShortestDistanceSquared(double point_x, double point_y, 
				   double start_x, double start_y,
				   double end_x,   double end_y  ) const;
    
    
  protected:

    bool _debug;
    bool _verbose;
    int _minHits;
    int _maxHits;
    double _MaxDist;

  };
}

#endif
/** @} */ // end of doxygen group 

