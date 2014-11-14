/**
 * \file DistToBoxWall.hh
 *
 * \ingroup GeoAlgo
 * 
 * \brief Class def header for a class DistToBoxWall
 *
 * @author kazuhiro
 */

/** \addtogroup GeoAlgo
    
    @{*/
#ifndef DISTTOBOXWALL_HH
#define DISTTOBOXWALL_HH

#include "GeoAlgoException.hh"
#include <sstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <TVector3.h>

namespace geoalgo {
  
  /**
     \class DistToBoxWall
     User defined class DistToBoxWall ... these comments are used to generate
     doxygen documentation!
  */
  class DistToBoxWall{
    
  public:
    
    /// Default constructor
    DistToBoxWall();
    
    /// Alternative constructor
    DistToBoxWall(double const x_min, double const x_max,
		  double const y_min, double const y_max,
		  double const z_min, double const z_max);
    
    /// Default destructor
    virtual ~DistToBoxWall(){};
    
    /// Reset function to get default values
    void Reset();
    
    /// Setter for maximum coordinate value (if you want to change)
    void SetXYZMax(double const x, double const y, double const z) 
    { 
      _xyz_max[0] = x; 
      _xyz_max[1] = y; 
      _xyz_max[2] = z; 
    }
    
    /// Setter for minimum coordinate value (if you want to change)
    void SetXYZMin(double const x, double const y, double const z) 
    { 
      _xyz_min[0] = x; 
      _xyz_min[1] = y; 
      _xyz_min[2] = z; 
    }
    
    /// Getter for a distance from a point to the box boundary
    double DistanceToWall(std::vector<double> const& point) const;
    
    /// Getter for a distance from a point to the box boundary along a direction (or backwards)
    /// Specify 0 for backwards, 1 for forwards
    double DistanceToWall(std::vector<double> const& point,
			  std::vector<double> dir,
			  bool ForwardOrBack ) const; 

	/// Getter for distance from point to box (when point is outside box)
	double DistanceToWallFromOut(std::vector<double> const &point) const;

  protected:
    
    /// "rectangular box" xyz minimum coordinate values
    std::vector<double> _xyz_min;
    
    /// "rectangular box" xyz maximum coordinate values
    std::vector<double> _xyz_max;
    
  };
}

#endif
/** @} */ // end of doxygen group 

