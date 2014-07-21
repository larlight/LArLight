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
#include "MinaTestConstants.hh"

/**
   \class MinaTest
   User defined class MinaTest ... these comments are used to generate
   doxygen documentation!
 */
class photon : public TObject{

public:

  /// Default constructor
  photon(){ clear();};

  /// Default destructor
  virtual ~photon(){};


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

  ClassDef(photon,1);

};

class Particle : public TObject{

public:

  //Constructor
  Particle() { clear(); }

  //Destructor
  virtual ~Particle(){}

  void clear() {
    TrackID = PDG = kINVALID_INT;
    Photon_.clear();
  };

//  void SetParticle(int const TID, int const pdg) {
//    TrackID = TID;
//    PDG = pdg;
//    Photon_.clear();
//  };

  //The Photon must already be set as a photon in the analysis.
  void SetParticle(int const TID, int const pdg, photon Pho_) {
    TrackID = TID;
    PDG = pdg;
    Photon_ = Pho_;
  };
    

  int fTrackId() const {return TrackID;}
  int fPDG() const {return PDG;}
  double fEnergy() const {return Photon_.GetEnergy(); }
  int fMotherTID() const {return Photon_.GetMotherTID(); }
  //int fMotherPDG() const {return Photon_.GetMotherPDG(); }
  std::vector<double> fStartXYZT() const {return Photon_.GetStartXYZT(); }
  std::vector<double> fEndXYZT() const {return Photon_.GetEndXYZT(); }

protected:

  photon Photon_;
  int TrackID;
  int PDG;

  ClassDef(Particle, 1);
  
};

class MinaTest : public std::vector<Particle> {

public:
  
  //Constructor
  MinaTest() : std::vector<Particle>() { clear(); };

  //Destructor
  virtual ~MinaTest(){};
  
  //Setter for event 
  void SetEvent(int EventID, int run) {Event = EventID; Run = run;}
  int GetEvent() const {return Event;}
  int GetRun() const {return Run;}

  void clear() { Event = 0; Run = 0; std::vector<Particle>::clear(); }

protected:
  
  int Event;
  int Run;
  
  ClassDef(MinaTest,1);

};


#endif
/** @} */ // end of doxygen group 

