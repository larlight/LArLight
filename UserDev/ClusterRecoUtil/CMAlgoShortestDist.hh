/**
 * \file CMAlgoShortestDist.hh
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class CMAlgoShortestDist
 *
 * @author davidkaleko
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef CMALGOSHORTESTDIST_HH
#define CMALGOSHORTESTDIST_HH

#include <iostream>
#include "CMergeAlgoBase.hh"
#include "GeometryUtilities.hh"


namespace cluster {
  /**
     \class CMAlgoShortestDist
     User defined class CMAlgoShortestDist ... these comments are used to generate
     doxygen documentation!
  */
  class CMAlgoShortestDist : public CMergeAlgoBase{
    
  public:
    
    /// Default constructor
    CMAlgoShortestDist();
    
    /// Default destructor
    virtual ~CMAlgoShortestDist(){};
    
    /// Overloaded (from CMergeAlgoBase) Merge function
    virtual bool Merge(const cluster_params &cluster1,
		       const cluster_params &cluster2);
    
    /// Method to re-configure the instance
    void reconfigure();
    
    /// Method to set cut value in cm^2 for distance compatibility test
    void SetSquaredDistanceCut(double d) { _max_2D_dist2 = d; }

    /// Method to set verbose mode
    void SetVerbose(bool on) { _verbose = on; }
    
    /// Method to set a conversion factor from wire to cm scale
    void SetWire2Cm(double f) { _wire_2_cm = f; }

    /// Method to set a conversion factor from time to cm scale
    void SetTime2Cm(double f) { _time_2_cm = f; }

    /// Method to set minimum distance unit (maybe unnecessary)
    void SetMinDistUnit(double f) { _min_distance_unit = f; }

    /**
       Function to compute a distance between a 2D point (point_x, point_y) to a 2D finite line segment
       (start_x, start_y) => (end_x, end_y).
    */
    double ShortestDistanceSquared(double point_x, double point_y, 
				   double start_x, double start_y,
				   double end_x,   double end_y  ) const;



  protected:
    
    bool _verbose;         /// bool to suppress lots of output if you want
  
    double _wire_2_cm, _time_2_cm;


    double _max_2D_dist2;      /// max distance b/t clusters for comability, in cm^2
    double _min_distance_unit; /// minimum distance b/t start and end point of cluster to use it

  };
  

} //end namespace cluster

#endif
/** @} */ // end of doxygen group 
  
