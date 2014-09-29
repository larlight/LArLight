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

    /// Default destructor
    virtual ~ShowerContainmentCut(){};

    /// Reset function to get default values
    void Reset();

    /// Setter for maximum coordinate value (if you want to change)
    void SetMax(double const x, double const y, double const z) 
    { 
      _coord_max[0] = x; 
      _coord_max[1] = y; 
      _coord_max[2] = z; 
    }

    /// Setter for the origin coordinate (default = 0,0,0)
    void SetOrigin(double const x, double const y, double const z)
    {
      _origin_vtx[0] = x; 
      _origin_vtx[1] = y; 
      _origin_vtx[2] = z; 
    }

    /// Getter for a distance from a point to the box boundary
    double DistanceToWall(std::vector<double> const& point,
			  std::vector<double> const& dir) const ;

  protected:

    /// "rectangular box" origin vtx in 3D
    std::vector<double> _origin_vtx;

    /// "rectangular box" dimension (x, y, z)
    std::vector<double> _coord_max;

  };
}

#endif
/** @} */ // end of doxygen group 

