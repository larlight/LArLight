/**
 * \file mcpart.hh
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for mcpart data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat
    
@{*/

#ifndef MCPART_HH
#define MCPART_HH

#include "data_base.hh"

/**
   \class mcpart
   LArSoft Mcpart information container
*/
class mcpart : public data_base {

public:

  /// Default constructor
  mcpart(DATA::DATA_TYPE type=DATA::Track);

  /// Default destructor
  virtual ~mcpart(){};

  /**
     Implementation data_base::clear_event. 
     When the argument boolean "all" is set to true, clear ALL C-array data elements to the default value.
     When "all" is set to false (default), only filled entries are cleared.
     This way we save time to loop over array elements.
  */
  virtual void clear_event(bool all=false);

  /// Implementation of track data address setter
  virtual void set_address(TTree* t);

protected:

  UShort_t _no_part;                           ///< Number of particles stored
  UShort_t  _trackID[DATA::kMaxParticles];     ///< Track ID ... unique per particle from G4
  Int_t     _pdgid[DATA::kMaxParticles];       ///< PDGID of a particle
  Int_t     _status_code[DATA::kMaxParticles]; ///< Status code of a particle (what is this?)

  Double_t  _momx[DATA::kMaxParticles];        ///< X component of momentum @ particle 1st step
  Double_t  _momy[DATA::kMaxParticles];        ///< Y component of momentum @ particle 1st step
  Double_t  _momz[DATA::kMaxParticles];        ///< Z component of momentum @ particle 1st step
  Float_t   _startx[DATA::kMaxParticles];      ///< X coordinate position @ particle 1st step
  Float_t   _starty[DATA::kMaxParticles];      ///< Y coordinate position @ particle 1st step
  Float_t   _startz[DATA::kMaxParticles];      ///< Z coordinate position @ particle 1st step
  Double_t  _startt[DATA::kMaxParticles];      ///< Time (G4 definition) @ particle 1st step
  Float_t   _endx[DATA::kMaxParticles];        ///< X coordinate position @ particle last step  
  Float_t   _endy[DATA::kMaxParticles];        ///< Y coordinate position @ particle last step  
  Float_t   _endz[DATA::kMaxParticles];        ///< Z coordinate position @ particle last step  
  Float_t   _endt[DATA::kMaxParticles];        ///< Time (G4 definition) @ particle last step  
  Double_t  _mom[DATA::kMaxParticles];         ///< Momentum (scalar magnitude) @ particle 1st step
  Float_t   _len[DATA::kMaxParticles];         ///< Summed length of particle's trajectories

  Double_t  _fv_momx[DATA::kMaxParticles];     ///< X component of momentum @ particle 1st step in the Fiducial Volume
  Double_t  _fv_momy[DATA::kMaxParticles];     ///< Y component of momentum @ particle 1st step in the Fiducial Volume
  Double_t  _fv_momz[DATA::kMaxParticles];     ///< Z component of momentum @ particle 1st step in the Fiducial Volume
  Float_t   _fv_startx[DATA::kMaxParticles];   ///< X coordinate position @ particle 1st step in the Fiducial Volume
  Float_t   _fv_starty[DATA::kMaxParticles];   ///< Y coordinate position @ particle 1st step in the Fiducial Volume
  Float_t   _fv_startz[DATA::kMaxParticles];   ///< Z coordinate position @ particle 1st step in the Fiducial Volume
  Double_t  _fv_startt[DATA::kMaxParticles];   ///< Time (G4 definition) @ particle 1st step in the Fiducial Volume
  Float_t   _fv_endx[DATA::kMaxParticles];     ///< X coordinate position @ particle last step in the Fiducial Volume  
  Float_t   _fv_endy[DATA::kMaxParticles];     ///< Y coordinate position @ particle last step in the Fiducial Volume  
  Float_t   _fv_endz[DATA::kMaxParticles];     ///< Z coordinate position @ particle last step in the Fiducial Volume  
  Float_t   _fv_endt[DATA::kMaxParticles];     ///< Time (G4 definition) @ particle last step in the Fiducial Volume  
  Double_t  _fv_mom[DATA::kMaxParticles];      ///< Momentum (scalar magnitude) @ particle 1st step in the Fiducial Volume
  Float_t   _fv_len[DATA::kMaxParticles];      ///< Summed length of particle's trajectories in the Fiducial Volume

};

#endif

/** @} */ // end of doxygen group 
