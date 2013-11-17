#ifndef MCSTEP_HH
#define MCSTEP_HH

/**
 * \file mcstep.hh
 *
 * \ingroup SimpleTreeData
 * 
 * @author Kazu - Nevis 2013
 */

/** \addtogroup SimpleTreeData
    
@{*/

#include "data_base.hh"

/**
   \class mcstep
   LArSoft MCTrajectory information container to hold particle's all trajectory points (or "track" in G4 terminology) when available. 
*/
class mcstep : public data_base {

public:

  /// Default constructor
  mcstep(DATA::DATA_TYPE type=DATA::MCTrajectory);

  /// Default destructor
  virtual ~mcstep(){};

  /**
     Implementation data_base::clear_event. 
     When the argument boolean "all" is set to true, clear ALL C-array data elements to the default value.
     When "all" is set to false (default), only filled entries are cleared.
     This way we save time to loop over array elements.
  */
  virtual void clear_event(bool all=false);

  /// A method to add a trajectory point
  void add_trajectory(UShort_t trackID, Int_t pdgid,
		      Double_t momx, Double_t momy, Double_t momz,
		      Float_t vtxx, Float_t vtxy, Float_t vtxz,
		      Double_t de, Double_t dx);

  /// Implementation of track data address setter
  virtual void set_address(TTree* t);

protected:

  UInt_t    _no_steps;                        ///< Number of particle trajectory points (steps) stored
  UShort_t  _trackID[DATA::kMaxTrajectories]; ///< Track ID ... unique per particle from G4
  Int_t     _pdgid[DATA::kMaxTrajectories];   ///< PDGID of a particle
  Double_t  _momx[DATA::kMaxTrajectories];    ///< X component of momentum 
  Double_t  _momy[DATA::kMaxTrajectories];    ///< Y component of momentum 
  Double_t  _momz[DATA::kMaxTrajectories];    ///< Z component of momentum 
  Float_t   _vtxx[DATA::kMaxTrajectories];    ///< X coordinate position 
  Float_t   _vtxy[DATA::kMaxTrajectories];    ///< Y coordinate position 
  Float_t   _vtxz[DATA::kMaxTrajectories];    ///< Z coordinate position 
  Double_t  _vtxt[DATA::kMaxTrajectories];    ///< Time (G4 definition) 
  Double_t  _de[DATA::kMaxTrajectories];      ///< Change in energy since last step
  Double_t  _dx[DATA::kMaxTrajectories];      ///< Length traveled since last step

};

#endif

/** @} */ // end of doxygen group 
