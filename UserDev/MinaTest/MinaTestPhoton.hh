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
#include "MinaTestConstants.hh"

/**
   \class MinaTestPhoton
   User defined class MinaTestPhoton ... these comments are used to generate
   doxygen documentation!
 */
class MinaTestPhoton : public TObject{

public:

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
    MotherPDG = kINVALID_INT;
    Energy = 0.;
    StartXYZT.clear();
    EndXYZT.clear();
    TrackLength = 0.;
  };

  void SetPhoton(double const E, std::vector<double> const sXYZT, std::vector<double> const eXYZT, int const MTID, int const MPDG, double const track_length) {
    Energy = E;
    MotherTID = MTID;
    MotherPDG = MPDG;
    StartXYZT = sXYZT;
    EndXYZT = eXYZT;
    TrackLength = track_length;
  };

  int GetEnergy() const { return Energy; }
  int GetMotherTID() const { return MotherTID; }
  int GetMotherPDG() const { return MotherPDG; }
  std::vector<double> GetStartXYZT() const { return StartXYZT; }
  std::vector<double> GetEndXYZT() const { return EndXYZT; }
  double GetTrackLength() const {return TrackLength; }

protected:

  std::vector<double> StartXYZT;
  std::vector<double> EndXYZT;
  double Energy;
  int MotherTID;
  double TrackLength;
  int MotherPDG;

  ClassDef(MinaTestPhoton,2);

};


#endif
/** @} */ // end of doxygen group 

