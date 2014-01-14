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

#include "data_base.hh"
#include <map>
#include <set>
#include <TVector3.h>

namespace larlight {
  /**
     \class part_mc
     Simplified version of MCParticle data container
  */
  class part_mc : public data_base {
    
  public:
    
    /// Default constructor w/o arguments set to private (not-to-be-used)
    part_mc(){clear_data();};
    
    /// Default constructor requires pdgid, track & parent id
    part_mc(Int_t pdgid, Int_t track, Int_t parent, std::string process);
    
    /// Copy constructor
    part_mc(const part_mc& original) : data_base     (original),
				       fStepTime     (original.fStepTime),
				       fStepVertex   (original.fStepVertex),
				       fStepMomentum (original.fStepMomentum),
				       //fStepProcess  (original.fStepProcess),
				       fProcess      (original.fProcess),
				       fDaughters    (original.fDaughters),
				       fParentID     (original.fParentID),
				       fTrackID      (original.fTrackID),
				       fPDGID        (original.fPDGID)
    {};
    
    /// Default destructor
    virtual ~part_mc(){};
    
    /// Clear method
    void clear_data();
    
    /// Appender of a particle track
    void add_track(Double_t x,  Double_t y,  Double_t z,  Double_t t,
		   Double_t px, Double_t py, Double_t pz);
    
    /// Appender of a particle's daughter
    //void add_daughter(Int_t id, Int_t track_id=-1);
    void add_daughter(Int_t track_id){fDaughters.insert(track_id);};
    
    /// Getter for PDGID
    inline Int_t pdg_id()    const { return fPDGID;    };

    /// Getter for Track ID
    inline Int_t track_id()  const { return fTrackID;  };
    
    /// Getter for Parent ID
    inline Int_t parent_id() const { return fParentID; };
    
    /// Getter for step vertex
    inline const std::vector<TVector3>& step_vertex()   const { return fStepVertex;   };
    
    /// Getter for step time
    inline const std::vector<Double_t>&   step_time()     const { return fStepTime;     };
    
    /// Getter for step momentum
    inline const std::vector<TVector3>& step_momentum() const { return fStepMomentum; };
    
  private:
    
    std::vector<Double_t>    fStepTime;     ///< time at each step
    std::vector<TVector3>    fStepVertex;   ///< vertex at each step
    std::vector<TVector3>    fStepMomentum; ///< momentum at each step
    //std::vector<std::string> fStepProcess;  ///< process name at each step
    std::string              fProcess;      ///< process name for creation
    /// A list of daughters (daughter's particle id, which is unique, & step number)
    std::set<Int_t>          fDaughters;
    //std::map<Int_t, Int_t>       fDaughters;
    
    Int_t fParentID; ///< Parent's track id
    Int_t fTrackID;  ///< Unique tracking ID per particle
    Int_t fPDGID;    ///< Particle ID code
    
    ////////////////////////
    ClassDef(part_mc,5)
    ////////////////////////
      
  };
  
  /**
     \class event_mc
     LArSoft MCTruth class equivalent data container
  */
  class event_mc : public std::vector<larlight::part_mc>,
		   public data_base {
    
  public:
    
    /// Default constructor
    event_mc() : std::vector<larlight::part_mc>(), data_base() {clear_data();};
    
    /// Copy constructor
    event_mc(const event_mc& original) : std::vector<larlight::part_mc>(original),
					 data_base   (original),
					 //fMCNeutrino (original.fMCNeutrino),
					 fOrigin     (original.fOrigin)
	    //fNeutrinoSet(original.fNeutrinoSet
    {};
    
    /// Default destructor
    virtual ~event_mc(){};
    
    /// Clear method
    void clear_data();
    
    /// Setter for neutrino info
    //void set_nu_mc(nu_mc info) {fMCNeutrino = info;};
    
    /// Setter for generation code
    void set_gen_code(MC::Origin_t info) {fOrigin=info;};
    
    /// Getter for neutrino info
    //inline const nu_mc& nu_mc() {return fMCNeutrino;};
    
    /// Getter for generator id
    inline MC::Origin_t gen_code() const {return fOrigin;};
    
    /// Getter for a boolean to suggest if neutrinos were generated or not
    //inline const bool nu_info_set() {return fNeutrinoSet;};
    
    /**
       A utility function to obtain maximum & minimum of spacepoints' vertex along the specified axis.
       NOTE: the initial max/min values matter. It only modifies max/min if it finds values that is 
       above/below those initial values.
    */
    void get_axis_range (Double_t &max, Double_t &min, const Int_t axis) const;
    
    /**
       A utility function to obtain maximum & minimum of spacepoints' vertex along all axis.
       NOTE: the initial max/min values matter. It only modifies max/min if it finds values that is
       above/below those initial values.
    */
    void get_axis_range (Double_t &xmax, Double_t &xmin,
			 Double_t &ymax, Double_t &ymin,
			 Double_t &zmax, Double_t &zmin) const;  
    
  private:
    
    //nu_mc                fMCNeutrino;  ///< reference to neutrino info - null if not a neutrino
    MC::Origin_t         fOrigin;      ///< origin for this event
    //bool                 fNeutrinoSet; ///< flag for whether the neutrino information has been set
    
    ////////////////////////
    ClassDef(event_mc,4)
    ////////////////////////
      
  };
};
#endif

/** @} */ // end of doxygen group 
