/**
 * \file ToyAna.hh
 *
 * \ingroup MinaTest
 * 
 * \brief Class def header for a class DarkPhotonAnaC
 *
 * @author kazuhiro
 */

/** \addtogroup MinaTest

    @{*/
#ifndef DARKPHOTONANAC_HH
#define DARKPHOTONANAC_HH

#include <iostream>
#include <TTree.h>
#include <TFile.h>
#include <TH1D.h>
#include "MinaTestEvent.hh"
#include "MinaTestPhoton.hh"
#include "MinaTestParticle.hh"

/**
   \class DarkPhotonAnaC
   User defined class DarkPhotonAnaC ... these comments are used to generate
   doxygen documentation!
*/
class DarkPhotonAnaC{

public:
  
  /// Default constructor
  DarkPhotonAnaC(){};
  
  /// Default destructor
  virtual ~DarkPhotonAnaC(){};

  void RunEventLoop(TTree* tree);
  //double lineDist(const MinaTestParticle& part1, const MinaTestParticle& part2);

};

#endif
/** @} */ // end of doxygen group 
