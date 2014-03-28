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
  McshowerLookback();

  void FillShowerMap(larlight::event_mcshower* my_mcshow, std::map<unsigned long, unsigned short> &shower_idmap);
  void FillSimchMap(larlight::event_simch* my_simch, std::map<unsigned short, larlight::simch> &simch_map);
  void MatchHitsAll(larlight::hit this_hit, std::map<unsigned short, larlight::simch> simch_map, std::map<unsigned long, unsigned short> shower_idmap, unsigned int num_mcshow);
  //std::vector<double> MatchHitsAll;
  //double MatchClusOne;
  //std::vector<double> MatchClusAll;
  
  
  /// Default destructor
  virtual ~McshowerLookback(){};

};

#endif
/** @} */ // end of doxygen group 

