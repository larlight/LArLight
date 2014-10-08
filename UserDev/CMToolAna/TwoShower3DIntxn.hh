/**
 * \file TwoShower3DIntxn.hh
 *
 * \ingroup CMToolAna
 * 
 * \brief Class def header for a class TwoShower3DIntxn
 * \brief This class takes in two (point + direction) vectors and finds the
 * intersection in 3D space (or, the closest thing to an intersection
 * since two 3D lines frequently don't intersect).
 * see: http://paulbourke.net/geometry/pointlineplane/
 *
 * @author davidkaleko
 */

/** \addtogroup CMToolAna

    @{*/
#ifndef TWOSHOWER3DINTXN_HH
#define TWOSHOWER3DINTXN_HH

#include <iostream>
#include <vector>
#include <limits>
#include <climits>
#include <math.h> //for pow

/**
   \class TwoShower3DIntxn
   User defined class TwoShower3DIntxn ... these comments are used to generate
   doxygen documentation!
 */

class TwoShower3DIntxn{
  
public:
  
  /// Default constructor
  TwoShower3DIntxn(){ };
  
  /// Default destructor
  virtual ~TwoShower3DIntxn(){};

  //Format of returned vector is (x,y,z,error)
  //where (x,y,z) are the coordinates of the best-intersection
  //(midpoint of shortest line connecting the two 3D input lines)
  //and error is the length^2 of the shortest line connecting the two
  //3D input lines (0 error means the 3D lines actually intersect)  
  std::vector<double> FindIntxn(std::vector<double> start1, 
				std::vector<double> dir1, 
				std::vector<double> start2, 
				std::vector<double> dir2);
  

};

#endif
/** @} */ // end of doxygen group 

