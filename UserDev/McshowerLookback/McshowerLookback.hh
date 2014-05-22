/**
 * \file McshowerLookback.hh
 *
 * \ingroup McshowerLookback
 * 
 * \brief Class def header for a class McshowerLookback
 *
 * @author littlejohn
 */

/** \addtogroup McshowerLookback

    @{*/
#ifndef MCSHOWERLOOKBACK_HH
#define MCSHOWERLOOKBACK_HH

#include <iostream>
#include <vector>
#include "ana_base.hh"

/**
   \class McshowerLookback
   User defined class McshowerLookback ... these comments are used to generate
   doxygen documentation!
 */

namespace larlight {
 
  class McshowerLookback{
    
  public:
    
    /// Default constructor (min energy is 0.02GeV)
    McshowerLookback(){ _cutoff_energy = 0.02; };
    
    /// Takes in event_mcshowers and fills a map of 
    /// (G4trackid of daughter particle => MCShower index in ev_mcshower)
    void FillShowerMap(larlight::event_mcshower* my_mcshow, 
		       std::map<UInt_t,UInt_t> &shower_idmap);
    
    /// Takes in event_simchannels and fills a map of
    /// (channel => larlight::simch)
    void FillSimchMap(larlight::event_simch* my_simch, 
		      std::map<UShort_t, larlight::simch> &simch_map);
    
    /// Takes in a single hit, two maps, and a list of MCShower indices and returns 
    /// a vector of charge fractions from the different MCShowers the hit belongs to.
    /// For example, if the list of MCShowers indices was (0, 1, 3)
    /// and the hit's charge is 20% in MCShower 0, 70% in MCShower1, 
    /// 0% in MCShower3 the returned vector is (0.2, 0.8, 0.0, 0.1)
    /// This last element is charge from either
    ///    * non-showers
    ///    * showers not included in the inpur list of MCShower indices
    ///    * showers below the cutoff threshold (20MeV default)
    std::vector<float> MatchHitsAll(const larlight::hit &this_hit, 
				    const std::map<UShort_t, larlight::simch> &simch_map, 
				    const std::map<UInt_t,UInt_t> &shower_idmap, 
				    const std::vector<UInt_t> &MCShower_indices);
    
    
    void SetCutoffEnergy(double energy) { _cutoff_energy = energy; }
    
    //std::vector<double> MatchHitsAll;
    //double MatchClusOne;
    //std::vector<double> MatchClusAll;
    
    
    /// Default destructor
    virtual ~McshowerLookback(){};
    
  protected:
    
    double _cutoff_energy;
  };
  
}
#endif
/** @} */ // end of doxygen group 

