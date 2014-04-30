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

class McshowerLookback{

public:

  /// Default constructor
  McshowerLookback(){};

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
  /// For example, if the list of MCShowers indicies was (0, 1, 3)
  /// and the hit's charge is 20% in MCShower 0, 70% in MCShower1, 
  /// 0% in MCShower3, and 10% in an unknown MCShower, 
  /// the returned vector is (0.2, 0.8, 0.0, 0.10)
  /// (the unknown MCShower is always the last entry in the return vector)
  std::vector<float> MatchHitsAll(const larlight::hit &this_hit, 
				  std::map<UShort_t, larlight::simch> &simch_map, 
				  std::map<UInt_t,UInt_t> &shower_idmap, 
				  std::vector<UInt_t> MCShower_indices);


  //std::vector<double> MatchHitsAll;
  //double MatchClusOne;
  //std::vector<double> MatchClusAll;
  
  
  /// Default destructor
  virtual ~McshowerLookback(){};

};

#endif
/** @} */ // end of doxygen group 

