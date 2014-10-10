/**
 * \file ShowerContainmentCut.hh
 *
 * \ingroup ShowerStudy
 * 
 * \brief Class def header for a class ShowerContainmentCut
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerStudy

    @{*/
#ifndef SHOWERCONTAINMENTCUT_HH
#define SHOWERCONTAINMENTCUT_HH

#include "ShowerAnaException.hh"
#include <sstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <TVector3.h>

namespace showerana {

  /**
     \class ShowerContainmentCut
     User defined class ShowerContainmentCut ... these comments are used to generate
     doxygen documentation!
  */
  class ShowerContainmentCut{
    
  public:
    
    /// Default constructor
    ShowerContainmentCut();

    /// Alternative constructor
    ShowerContainmentCut(double const x_min, double const x_max,
			 double const y_min, double const y_max,
			 double const z_min, double const z_max);

    /// Default destructor
    virtual ~ShowerContainmentCut(){};

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

    /// Getter for a distance from a point to the box boundary along a direction
    double DistanceToWall(std::vector<double> const& point,
			  std::vector<double> dir) const ;

	/// Getter for a distance backwards to box boundaries along trajectory
	double DistanceBackwardsToWall(std::vector<double> const& point,
			  std::vector<double> dir) const ;

  protected:

    /// "rectangular box" xyz minimum coordinate values
    std::vector<double> _xyz_min;

    /// "rectangular box" xyz maximum coordinate values
    std::vector<double> _xyz_max;

  };
}

#endif
/** @} */ // end of doxygen group 

