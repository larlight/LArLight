#ifndef MCPART_HH
#define MCPART_HH
/**
 * \file mcpart.hh
 *
 * \ingroup SimpleTreeData
 * 
 * @author Kazu - Nevis 2013
 */

/** \addtogroup SimpleTreeData
    
@{*/

#include "data_base.hh"

/**
   \class mcpart
   LArSoft MCParticle information container dedicated for those particles going through the fiducial volume. Some information overlapps with MCTruth as this module also stores only the starting & ending point information. For detailed track, refer to a future implementation of mctrack class which is meant to hold the whole MC trajectory information.
*/
class mcpart : public data_base {

public:

  /// Default constructor
  mcpart(DATA::DATA_TYPE type=DATA::MCParticle);

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

  /**
     This method adds a particle information into C-array data structure based on
     the information provided by mcpart::set_part_info and mcpart::set_fv_part_info
     methods. Call those two methods first to set particle information before calling
     this function.
   */
  void add_particle(UShort_t trackID, Int_t pdgid, Int_t status_code);

  /**
     Needs to be called BEFORE mcpart::add_particle function.
     This sets particle information about the start and end of particle tracking
     disregard of the fiducial volume.
  */
  void set_part_info(Double_t mom, Double_t momx,  Double_t momy,  Double_t momz,
		     Float_t startx, Float_t starty, Float_t startz, Double_t startt,
		     Float_t endx,   Float_t endy,   Float_t endz,   Double_t endt,
		     Float_t len);

  /**
     Needs to be called BEFORE mcpart::add_particle function.
     This sets particle information about the start and end of particle tracking
     inside the fiducial volume.
  */
  void set_fv_part_info(Double_t mom, Double_t momx,  Double_t momy,  Double_t momz,
			Float_t startx, Float_t starty, Float_t startz, Double_t startt,
			Float_t endx,   Float_t endy,   Float_t endz,   Double_t endt,
			Float_t len);

protected:

  bool _part_info_set;    ///< Checker variable set to true by mcpart::set_part_info and false by mcpart::add_particle
  bool _fv_part_info_set; ///< Checker variable set to true by mcpart::set_fv_part_info and false by mcpart::add_particle

  UShort_t  _no_part;                           ///< Number of particles stored
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
