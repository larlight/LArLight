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

  void FillShowerMap(larlight::event_mcshower* my_mcshow, std::map<unsigned short, unsigned long> &shower_idmap);
  void FillSimchMap(larlight::event_simch* mysimch, std::map<unsigned long, larlight::simch> &simch_map);
  double MatchHitsOne(larlight::hit* myhit, std::map<unsigned short, larlight::simch>* map_clusid, std::map<unsigned short, unsigned long>* map_showid);
  //std::vector<double> MatchHitsAll;
  //double MatchClusOne;
  //std::vector<double> MatchClusAll;
  
  
  /// Default destructor
  virtual ~McshowerLookback(){};

};

#endif
/** @} */ // end of doxygen group 

