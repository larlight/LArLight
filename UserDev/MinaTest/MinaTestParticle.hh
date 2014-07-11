/**
 * \file MinaTestParticle.hh
 *
 * \ingroup MinaTest
 * 
 * \brief Class def header for a class MinaTestParticle
 *
 * @author ehimwich
 */

/** \addtogroup MinaTest

    @{*/
#ifndef MINATESTPARTICLE_HH
#define MINATESTPARTICLE_HH

#include <iostream>
#include <TObject.h>
#include <vector>
#include <climits>
#include <limits>
#include "MinaTestPhoton.hh"
/**
   \class MinaTestParticle
   User defined class MinaTestParticle ... these comments are used to generate
   doxygen documentation!
 */

class MinaTestParticle : public TObject{

public:

  /// A constant to indicate invalid integer value in the storage
  static const int kINVALID_INT = std::numeric_limits<int>::max();
 
  //Constructor
  MinaTestParticle() { clear(); }

  //Destructor
  virtual ~MinaTestParticle(){}

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
  void SetParticle(int const TID, int const pdg, MinaTestPhoton Pho_) {
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

  MinaTestPhoton Photon_;
  int TrackID;
  int PDG;

  ClassDef(MinaTestParticle, 1);
  
};

#endif
/** @} */ // end of doxygen group 

