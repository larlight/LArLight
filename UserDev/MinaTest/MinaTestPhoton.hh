/**
 * \file MinaTestPhoton.hh
 *
 * \ingroup MinaTest
 * 
 * \brief Class def header for a class MinaTestPhoton
 *
 * @author ehimwich
 */

/** \addtogroup MinaTest

    @{*/
#ifndef MINATESTPHOTON_HH
#define MINATESTPHOTON_HH

#include <iostream>
#include <TObject.h>
#include <vector>
#include <climits>
#include <limits>

/**
   \class MinaTestPhoton
   User defined class MinaTestPhoton ... these comments are used to generate
   doxygen documentation!
 */
class MinaTestPhoton : public TObject{

public:

  /// A constant to indicate invalid integer value in the storage
  static const int kINVALID_INT = std::numeric_limits<int>::max();

  /// Default constructor
  MinaTestPhoton(){ clear();};

  /// Default destructor
  virtual ~MinaTestPhoton(){};


  //int Event;
  //int Run;
  //int PDG;
  //int TrackID;

  void clear() { 
    MotherTID = kINVALID_INT;
    Energy = 0.;
    StartXYZT.clear();
    EndXYZT.clear();
  };

  void SetPhoton(double const E, std::vector<double> const sXYZT, std::vector<double> const eXYZT, int const MTID) {
    Energy = E;
    MotherTID = MTID;
    //MotherPDG = MPDG;
    StartXYZT = sXYZT;
    EndXYZT = eXYZT;
  };

  int GetEnergy() const { return Energy; }
  int GetMotherTID() const { return MotherTID; }
  //void GetMotherPDG() const { return MotherPDG; }
  std::vector<double> GetStartXYZT() const { return StartXYZT; }
  std::vector<double> GetEndXYZT() const { return EndXYZT; }

protected:

  std::vector<double> StartXYZT;
  std::vector<double> EndXYZT;
  double Energy;
  int MotherTID;
  //int MotherPDG;

  ClassDef(MinaTestPhoton,1);

};


#endif
/** @} */ // end of doxygen group 

