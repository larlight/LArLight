/**
 * \file TrajectoryInVolume.hh
 *
 * \ingroup GeoAlgo
 * 
 * \brief Class def header for a class TrajectoryInVolume
 *
 * @author David Caratelli
 */

/** \addtogroup GeoAlgo

    @{*/
#ifndef TRAJECTORYINVOLUME_HH
#define TRAJECTORYINVOLUME_HH

#include <iostream>
#include <vector>
#include <math.h>

/**
   \class TrajectoryInVolume
   Takes in a vector of 3D points and Returns if the trajectory
   delimited by these points crosses a given TPC-like volume
   doxygen documentation!
 */
class TrajectoryInVolume{

public:

  /// Default constructor
  TrajectoryInVolume(){ _epsilon = 0.0001; };

  /// Default destructor
  virtual ~TrajectoryInVolume(){};

  /// Set Volume
  void SetVolume(double xMin, double xMax, double yMin, double yMax, double zMin, double zMax)
  { _xMin = xMin; _xMax = xMax; _yMin = yMin; _yMax = yMax; _zMin = zMin; _zMax = zMax; }

  /// Set Trajectory Points
  void SetTrajectory(std::vector<std::vector<double> > traj) { _traj = traj; }

  /// Add Trajectory point
  void AddTrajectoryPoint(std::vector<double> point);

  /// See if a list of trajectory points intersects the volume
  bool IsInVolume();

  /// See if a list of trajectory points intersects the volume
  bool IsInVolume(std::vector<std::vector<double> > traj);

  /// See if Line intersects volume
  bool LineInVolume(std::vector<double> point1, std::vector<double> point2);

  /// See if point in volume
  bool PointInVolume(std::vector<double> point);

  /// Measure distance between two points
  double Distance(std::vector<double> point1, std::vector<double> point2);
  
private:

  /// epsilon value to determine if line parallel to one of x,y, or z axis.
  /// equal to cosine of angle
  double _epsilon;
  
  /// Volume boundaries:
  double _xMin, _xMax, _yMin, _yMax, _zMin, _zMax;

  /// Trajectory vector of 3-entry vectors (in x,y,z order)
  std::vector<std::vector<double> > _traj;

};


#endif
/** @} */ // end of doxygen group 

