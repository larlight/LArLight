/**
 * \file PointToLineDist.hh
 *
 * \ingroup GeoAlgo
 * 
 * \brief Class def header for a class PointToLineDist
 *
 * @author david
 */

/** \addtogroup GeoAlgo

    @{*/
#ifndef POINTTOLINEDIST_HH
#define POINTTOLINEDIST_HH

#include <iostream>
#include <vector>
#include <math.h>

/**
   \class PointToLineDist
   User defined class PointToLineDist ... these comments are used to generate
   doxygen documentation!
 */
class PointToLineDist{

public:

  /// Default constructor
  PointToLineDist(){ _point.clear(); _segmentStart.clear(); _segmentEnd.clear(); };

  /// Default destructor
  virtual ~PointToLineDist(){};

  double Distance();

  double Distance(std::vector<float> point,
		  std::vector<float> segmentStart,
		  std::vector<float> segmentEnd);

  double DotProduct(std::vector<float> A, std::vector<float> B);

private:

  std::vector<float>  _point;
  std::vector<float>  _segmentStart;
  std::vector<float> _segmentEnd;

};

#endif
/** @} */ // end of doxygen group 

