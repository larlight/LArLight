/**
 * \file CMAlgoStartInCone.hh
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class CMAlgoStartInCone
 *
 * @author david
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef CMALGOSTARTINCONE_HH
#define CMALGOSTARTINCONE_HH

#include <iostream>
#include "CBoolAlgoBase.hh"
#include "GeometryUtilities.hh"
#include <math.h>

namespace cluster {
  
  /**
     \class CMAlgoStartInCone
     If start point of Cluster B in Cone region of Cluster A then merge
  */
  class CMAlgoStartInCone : public CBoolAlgoBase{
    
  public:
    
    /// Default constructor
    CMAlgoStartInCone();
    
    /// Default destructor
    virtual ~CMAlgoStartInCone(){};
  
    /// Merging Algorithm is Here
    virtual bool Bool(const ClusterParamsAlgNew &cluster1,
		      const ClusterParamsAlgNew &cluster2);
    
    /// Method to re-configure the instance
    void reconfigure();

    /// Set Minimum number of hits for cone-cluster
    void SetMinHits(int n){ _NhitsMin = n; }

    /// Set Minimum number of hits for cone-cluster
    void SetMinLen(double l){ _lenMin = l; }

    /// Set Verbosity of messages
    void SetVerbose(bool verbosity){ _verbose = verbosity; }

    /// Set Debug for messages
    void SetDebug(bool debug){ _debug = debug; }

    /// Set Angle Compatibility betweeen the clusters
    void SetAngleCompat(double deg){ _angleCompat = deg; }

    /// Set Length Reach: How for out the cone extends as percent of cluster length
    void SetLengthReach(double frac){ _lengthReach = frac; }

  protected:

    double _wire_2_cm, _time_2_cm; /// Conversion factors ogtten from GeometryUtilities

    int _NhitsMin;     /// Larger cluster which determines cone must have this many hits
    double _lenMin;    /// Larger cluster which determines cone must be at least this long
    bool _verbose;
    bool _debug;
    double _angleCompat; /// Two clusters must have direction within this value of each other
    double _lengthReach; ///How four out - as percent of cluster length - cone will extend from start point
    
  };

}  
#endif
/** @} */ // end of doxygen group 

