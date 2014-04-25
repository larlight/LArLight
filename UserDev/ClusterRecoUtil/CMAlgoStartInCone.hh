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
#include "CMergeAlgoBase.hh"
#include "GeometryUtilities.hh"
#include <math.h>

namespace cluster {
  
  /**
     \class CMAlgoStartInCone
     If start point of Cluster B in Cone region of Cluster A then merge
  */
  class CMAlgoStartInCone : public CMergeAlgoBase{
    
  public:
    
    /// Default constructor
    CMAlgoStartInCone();
    
    /// Default destructor
    virtual ~CMAlgoStartInCone(){};
  
    /// Merging Algorithm is Here
    virtual bool Merge(const cluster_params &cluster1,
		       const cluster_params &cluster2);
    
    /// Method to re-configure the instance
    void reconfigure();

    /// Set Minimum number of hits for cone-cluster
    void SetMinHits(int n){ _NhitsMin = n; }

    /// Set Minimum number of hits for cone-cluster
    void SetMinLen(double l){ _lenMin = l; }

  protected:

    double _wire_2_cm, _time_2_cm; /// Conversion factors ogtten from GeometryUtilities

    int _NhitsMin;
    double _lenMin;
    
  };

}  
#endif
/** @} */ // end of doxygen group 

