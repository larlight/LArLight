/**
 * \file mctruth.hh
 *
 * \ingroup SimpleTreeData
 * 
 * \brief Class def header for mctruth data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup SimpleTreeData
    
@{*/

#ifndef MCTRUTH_HH
#define MCTRUTH_HH

#include "data_base.hh"

/**
   \class mctruth
   LArSoft MCTruth information container
*/
class mctruth : public data_base {

public:

  /// Default constructor
  mctruth(DATA::DATA_TYPE type=DATA::Track);

  /// Default destructor
  virtual ~mctruth(){};

  /**
     Implementation data_base::clear_event. 
     When the argument boolean "all" is set to true, clear ALL C-array data elements to the default value.
     When "all" is set to false (default), only filled entries are cleared.
     This way we save time to loop over array elements.
  */
  virtual void clear_event(bool all=false);

  /// Implementation of track data address setter
  virtual void set_address(TTree* t);

  //--- Member getters ---//
  UChar_t         no_part()     const { return _no_part; };
  const Int_t*    pdgid()       const { return _pdgid;   };
  const Float_t*  vtxx()        const { return _vtxx;    };
  const Float_t*  vtxy()        const { return _vtxy;    };
  const Float_t*  vtxz()        const { return _vtxz;    };
  const Double_t* mom()         const { return _mom;     };
  const Double_t* momx()        const { return _momx;    };
  const Double_t* momy()        const { return _momy;    };
  const Double_t* momz()        const { return _momz;    };
  const Double_t* en()          const { return _en;      };
  const Double_t* mass()        const { return _mass;    };
  const Int_t*    status_code() const { return _status_code; };
  const Int_t*    gen_trackID() const { return _gen_trackID; };
  const Int_t*    no_daughter() const { return _no_daughter; };
  const Int_t*    mother()      const { return _mother;      };

  /// Method to add a primary particle
  void add_primary(Int_t pdgid,     Int_t trackID,  Int_t status_code, 
		   Int_t ndaughter, Int_t mother,
		   Float_t x,       Float_t y,      Float_t z,
		   Double_t mass,   Double_t en,
		   Double_t mom,    Double_t px,    Double_t py,    Double_t pz);

protected:

  UShort_t _no_part;                          ///< Number of primary particles
  Int_t    _pdgid[DATA::kMaxPrimaries];       ///< PDGID of a primary
  Int_t    _status_code[DATA::kMaxPrimaries]; ///< Status code of a primary... WHAT IS STATUS CODE??
  Int_t    _gen_trackID[DATA::kMaxPrimaries]; ///< Generation track ID of a primary ... (NOT G4 TRACK ID)
  Int_t    _no_daughter[DATA::kMaxPrimaries]; ///< Number of daughters created by a primary
  Int_t    _mother[DATA::kMaxPrimaries];      ///< The parent's track ID of a primary (if a parent exists)
  Float_t  _vtxx[DATA::kMaxPrimaries];        ///< X coordinate position of a primary @ generation
  Float_t  _vtxy[DATA::kMaxPrimaries];        ///< Y coordinate position of a primary @ generation
  Float_t  _vtxz[DATA::kMaxPrimaries];        ///< Z coordinate position of a primary @ generation
  Double_t _mass[DATA::kMaxPrimaries];        ///< Mass of a primary
  Double_t _en[DATA::kMaxPrimaries];          ///< Energy of a primary
  Double_t _mom[DATA::kMaxPrimaries];         ///< Momentum of a primary
  Double_t _momx[DATA::kMaxPrimaries];        ///< X component momentum of a primary @ generation
  Double_t _momy[DATA::kMaxPrimaries];        ///< Y component momentum of a primary @ generation
  Double_t _momz[DATA::kMaxPrimaries];        ///< Z component momentum of a primary @ generation

};

#endif

/** @} */ // end of doxygen group 
