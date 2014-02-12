//
// type-def file for ROOT
//

#ifndef DATAFORMAT_TYPEDEF_HH
#define DATAFORMAT_TYPEDEF_HH

// Header files included here
#include "data_base.hh"
#include "mctrack.hh"
#include "mcpart.hh"
#include "mcnu.hh"
#include "mctruth.hh"
#include "wire.hh"
#include "fifo.hh"
#include "pulse.hh"
#include "trigger.hh"
#include "hit.hh"
#include "user_info.hh"
#include "track.hh"
#include "cluster.hh"
#include "spacepoint.hh"
#include "shower.hh"
#include "calorimetry.hh"
#include "storage_manager.hh"
// Class forward declaration here

namespace larlight{

  class data_base;
  class event_base;

  class mcstep;
  class mctrack;

  class mcpart;
  class event_mcpart;

  class mcnu;

  class mctruth;
  class event_mctruth;
  
  class wire;
  class event_wire;
  
  class hit;
  class event_hit;

  class cluster;
  class event_cluster;

  class fifo;
  class event_fifo;

  class pulse;  
  class event_pulse;

  class trigger;
  
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
}
#endif



