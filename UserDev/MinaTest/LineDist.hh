/**
 * \file LineDist.hh
 *
 * \ingroup MinaTest
 * 
 * \brief Class def header for a class LineDist
 *
 * @author mina
 */

/** \addtogroup MinaTest

    @{*/
#ifndef LINEDIST_HH
#define LINEDIST_HH

#include <iostream>
#include "MinaTestParticle.hh"
#include "MinaTestPhoton.hh"
#include <cmath>

/**
   \class LineDist
   User defined class LineDist ... these comments are used to generate
   doxygen documentation!
 */
class LineDist{

public:

  /// Default constructor
  LineDist(){};

  /// Default destructor
  virtual ~LineDist(){};
  
  double Calc(const MinaTestParticle& part1, const MinaTestParticle& part2);

};

#endif
/** @} */ // end of doxygen group 

