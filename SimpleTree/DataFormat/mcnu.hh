/**
 * \file mcnu.hh
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for mcnu data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat
    
@{*/

#ifndef MCNU_HH
#define MCNU_HH

#include "data_base.hh"
/**
   \class mcnu
   Simplified version of MCNeutrino information container
*/
class mcnu : public data_base {

public:

  /// Default constructor 
  mcnu(DATA::DATA_TYPE type=DATA::MCNeutrino);
  
  /// Default destructor
  virtual ~mcnu(){};

  /**
     Implementation data_base::clear_event. 
     When the argument boolean "all" is set to true, clear ALL C-array data elements to the default value.
     When "all" is set to false (default), only filled entries are cleared.
     This way we save time to loop over array elements.
  */
  virtual void clear_event(bool all=false);

  /// Implementation of track data address setter
  virtual void set_address(TTree* t);


  /// Setter method to add neutrino information
  void add_neutrino(Int_t pdgid,  Bool_t ccnc,    UChar_t mode,
		    Double_t q2,  Double_t hmass,
		    Int_t hitnuc,
		    Float_t vtxx, Float_t vtxy,   Float_t vtxz,
		    Double_t en,  Double_t momx,  Double_t momy,  Double_t momz,
		    Double_t en_lep, Double_t momx_lep, Double_t momy_lep, Double_t momz_lep,
		    Int_t pdgid_parent,
		    Double_t momx_parent, Double_t momy_parent, Double_t momz_parent);
  
  //--- Getter methods ---//
  UChar_t         no_nu()    const { return _no_nu;    };
  const Int_t*    pdgid()    const { return _pdgid;    };
  const Bool_t*   ccnc()     const { return _ccnc;     };
  const UChar_t*  mode()     const { return _mode;     };
  const Double_t* q2()       const { return _q2;       };
  const Double_t* hmass()    const { return _hmass;    };
  const Int_t*    hitnuc()   const { return _hitnuc;   };
  const Float_t*  vtxx()     const { return _vtxx;     };
  const Float_t*  vtxy()     const { return _vtxy;     };
  const Float_t*  vtxz()     const { return _vtxz;     };
  const Double_t* en()       const { return _en;       };
  const Double_t* momx()     const { return _momx;     };
  const Double_t* momy()     const { return _momy;     };
  const Double_t* momz()     const { return _momz;     };
  const Double_t* en_lep()   const { return _en_lep;   };
  const Double_t* momx_lep() const { return _momx_lep; };
  const Double_t* momy_lep() const { return _momy_lep; };
  const Double_t* momz_lep() const { return _momz_lep; };

  const Double_t* momx_parent()  const { return _momx_parent;  };
  const Double_t* momy_parent()  const { return _momy_parent;  };
  const Double_t* momz_parent()  const { return _momz_parent;  };
  const Int_t*    pdgid_parent() const { return _pdgid_parent; };
protected:

  UChar_t  _no_nu;                             ///< Number of neutrinos
  Int_t    _pdgid[DATA::kMaxNeutrinos];       ///< PDGID of a neutrino
  Bool_t   _ccnc[DATA::kMaxNeutrinos];        ///< 0=CC, 1=NC
  UChar_t  _mode[DATA::kMaxNeutrinos];        ///< 0=QE/E1, 1=RES, 2=DIS, 3=Coherent interaction mode 
  Double_t _q2[DATA::kMaxNeutrinos];          ///< Squared momentum transfer
  Double_t _hmass[DATA::kMaxNeutrinos];       ///< Hadronic invariant mass
  Int_t    _hitnuc[DATA::kMaxNeutrinos];      ///< PDGID of nucleus on which neutrino has interacted
  Float_t  _vtxx[DATA::kMaxNeutrinos];        ///< X coordinate position of the interaction vtx
  Float_t  _vtxy[DATA::kMaxNeutrinos];        ///< Y coordinate position of the interaction vtx
  Float_t  _vtxz[DATA::kMaxNeutrinos];        ///< Z coordinate position of the interaction vtx
  Double_t _en[DATA::kMaxNeutrinos];          ///< Energy
  Double_t _momx[DATA::kMaxNeutrinos];        ///< X component of neutrino momentum
  Double_t _momy[DATA::kMaxNeutrinos];        ///< Y component of neutrino momentum
  Double_t _momz[DATA::kMaxNeutrinos];        ///< Z component of neutrino momentum
  Double_t _en_lep[DATA::kMaxNeutrinos];      ///< Energy of the outgoing lepton
  Double_t _momx_lep[DATA::kMaxNeutrinos];    ///< X component of outgoing lepton momentum
  Double_t _momy_lep[DATA::kMaxNeutrinos];    ///< Y component of outgoing lepton momentum
  Double_t _momz_lep[DATA::kMaxNeutrinos];    ///< Z component of outgoing lepton momentum
  Double_t _momx_parent[DATA::kMaxNeutrinos]; ///< X component of parent particle momentum @ leaving BNB target
  Double_t _momy_parent[DATA::kMaxNeutrinos]; ///< Y component of parent particle momentum @ leaving BNB target
  Double_t _momz_parent[DATA::kMaxNeutrinos]; ///< Z component of parent particle momentum @ leaving BNB target
  Int_t    _pdgid_parent[DATA::kMaxNeutrinos];///< PDGID of a parent particle @ leaving BNB target
};

#endif

/** @} */ // end of doxygen group 
