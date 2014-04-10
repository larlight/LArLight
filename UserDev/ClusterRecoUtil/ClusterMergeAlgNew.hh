/**
 * \file ClusterMergeAlgNew.hh
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class ClusterMergeAlgNew
 *
 * @author kazuhiro
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef CLUSTERMERGEALGNEW_HH
#define CLUSTERMERGEALGNEW_HH

#include <iostream>
#include "larlight_base.hh"
#include "LArUtil-TypeDef.hh"
#include "ClusterParams.hh"
//#include "mac/Polygon.hh"
//#include "mac/PolyOverlap.cc"
#include "Polygon.hh"


namespace cluster {
  
  /**
     \class ClusterMergeAlgNew
     User defined class ClusterMergeAlgNew ... these comments are used to generate
     doxygen documentation!
  */
  class ClusterMergeAlgNew : public larlight::larlight_base{

    enum MergeAlg_t {
      kStartPoint=0,
      kPolygonCollision,
      kAngleCompat,
      kAlgoMax
    };
    
  public:
    
    /// Default constructor
    ClusterMergeAlgNew();
    
    /// Default destructor
    virtual ~ClusterMergeAlgNew(){};
    
    bool Merge(const cluster::cluster_params &param_a, 
	       const cluster::cluster_params &param_b);


    /// Method to set cut value in cm^2 for distance compatibility (TestStartPoint) test
    void SetSquaredDistanceCut(double d) { _max_2D_dist2 = d; }

    /// Method to set cut value in degrees for angle compatibility test
    void SetAngleCut(double angle) { _max_allowed_2D_angle_diff = angle; }

    /// Set verbosity
    void VerboseMode(bool on) { _verbose = on; }

  protected:

    /// Function that determines shortest distance b/t two line segments describing clusters
    bool TestStartPoint(const cluster::cluster_params &param_a, 
			const cluster::cluster_params &param_b);

    /// Function that determines if the two clusters' 2D axis angles agree w/in a set value
    bool Angle2DCompatibility(const cluster::cluster_params &param_a, 
			      const cluster::cluster_params &param_b);


    bool TestPolygonCollision(const cluster::cluster_params &param_a, 
			      const cluster::cluster_params &param_b);

    std::vector<bool> fAlgoSwitch;



    /// TestStartPoint utility function
    double ShortestDistanceSquared(double point_x, double point_y, 
				   double start_x, double start_y,
				   double end_x,   double end_y  ) const;


    bool _verbose; ///< Verbose mode boolean


    /// TestStartPoint (cluster merge) parameters
    double _max_2D_dist2;           //in cm^2
    double _min_distance_unit;      //in cm^2
    double _max_allowed_2D_angle_diff; //in degrees

  };
}

#endif
/** @} */ // end of doxygen group 

