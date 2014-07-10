/**
 * \file MinaTest.hh
 *
 * \ingroup MinaTest
 * 
 * \brief Class def header for a class MinaTest
 *
 * @author mina
 */

/** \addtogroup MinaTest

    @{*/
#ifndef MINATEST_HH
#define MINATEST_HH

#include <iostream>
#include <TObject.h>
#include <vector>
/**
   \class MinaTest
   User defined class MinaTest ... these comments are used to generate
   doxygen documentation!
 */
class MinaTest : public TObject{

public:

  /// Default constructor
  MinaTest(){};

  /// Default destructor
  virtual ~MinaTest(){};

  struct Photon{

    int Event;
    int Run;
    int PDG;
    int TrackID;
    std::vector<double> StartXYZT;
    std::vector<double> EndXYZT;
    double Energy;
    int MotherTID;
    int MotherPDG;
 
   Photon(){};
  };

  struct Particle{

    int TrackID;
    int PDG;

    Particle(){};

  };

  std::vector<double> Gamma_;
  std::vector<double> All_;

  ClassDef(MinaTest,1);

};

#endif
/** @} */ // end of doxygen group 

