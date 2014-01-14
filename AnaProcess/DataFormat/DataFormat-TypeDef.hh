//
// type-def file for ROOT
//

#ifndef DATAFORMAT_TYPEDEF_HH
#define DATAFORMAT_TYPEDEF_HH

// Header files included here
#include "wire.hh"
#include "pmtfifo.hh"
#include "tpcfifo.hh"
#include "trigger.hh"
#include "hit.hh"
#include "user_info.hh"
#include "track.hh"
#include "cluster.hh"
#include "mctruth.hh"
#include "spacepoint.hh"
#include "shower.hh"
#include "calorimetry.hh"
#include "storage_manager.hh"
// Class forward declaration here
namespace larlight{

  class data_base;
  
  class wire;
  class event_wire;
  
  class hit;
  class event_hit;

  class cluster;
  class event_cluster;

  class pmtfifo;
  class event_pmtfifo;

  class tpcfifo;
  class event_tpcfifo;

  class trigger;
  
  class part_mc;
  class event_mc;
  
  class spacepoint;
  class event_sps;
  
  class user_info;
  class event_user;
  
  class track;
  class event_track;

  class shower;
  class event_shower;

  class calorimetry;
  class event_calorimetry;
  
  class storage_manager;
};
#endif



