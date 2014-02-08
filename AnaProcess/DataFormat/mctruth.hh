/**
 * \file mctruth.hh
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for mctruth data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat
    
@{*/

#ifndef MCTRUTH_HH
#define MCTRUTH_HH

#include "mcpart.hh"
#include "mcnu.hh"

namespace larlight {
  /**
     \class mctruth
     Simplified version of MCParticle data container
  */
  class mctruth : public data_base {
    
  public:

    /// Default constructor
    mctruth(DATA::DATA_TYPE type=DATA::MCTruth);

    /// Default destructor
    virtual ~mctruth(){}

    /// Copy constructor
    mctruth(const mctruth& origin) : data_base(origin),
				     fPartList(origin.fPartList),
				     fMCNeutrino(origin.fMCNeutrino),
				     fOrigin(origin.fOrigin),
				     fNeutrinoSet(origin.fNeutrinoSet)
    {}

    /// Clear method
    virtual void clear_data();

    ///--- Setters/Adders ---///
    void Add(mcpart part)              { fPartList.push_back(part); }
    void SetOrigin( MC::Origin_t orig) { fOrigin = orig;            }
    void SetNeutrino(Int_t CCNC,
		     Int_t mode,
		     Int_t interactionType,
		     Int_t target,
		     Int_t nucleon,
		     Int_t quark,
		     Double_t w,
		     Double_t x,
		     Double_t y,
		     Double_t qsqr);

    ///--- Getters ---///

    const std::vector<larlight::mcpart>& GetParticles() const { return fPartList;    }
    const mcnu&                          GetNeutrino()  const { return fMCNeutrino;  }
    MC::Origin_t                         Origin()       const { return fOrigin;      }
    Bool_t                               NeutrinoSet()  const { return fNeutrinoSet; }

  private:

    std::vector<larlight::mcpart> fPartList;    ///< list of particles in this event
    larlight::mcnu                fMCNeutrino;  ///< reference to neutrino info - null if not a neutrino
    MC::Origin_t                  fOrigin;      ///< origin for this event
    Bool_t                        fNeutrinoSet; ///< flag for whether the neutrino information has been set
    
    ////////////////////////
    ClassDef(mctruth,6)
    ////////////////////////
      
  };
  
  /**
     \class event_mctruth
     LArSoft MCTruth class equivalent data container
  */
  class event_mctruth : public std::vector<larlight::mctruth>,
			public data_base {
    
  public:
    
    /// Default constructor
    event_mctruth(DATA::DATA_TYPE type=DATA::MCTruth);
    
    /// Copy constructor
    event_mctruth(const event_mctruth& original) : std::vector<larlight::mctruth>(original),
						   data_base   (original)
    {}
    
    /// Default destructor
    virtual ~event_mctruth(){}
    
    /// Clear method
    virtual void clear_data() {data_base::clear_data(); clear();}
    
  private:
    
    ////////////////////////
    ClassDef(event_mctruth,1)
    ////////////////////////
      
  };
}
#endif

/** @} */ // end of doxygen group 
