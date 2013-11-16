/**
 * \file hit.hh
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for hit data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/

#ifndef HIT_HH
#define HIT_HH

#include "data_base.hh"
/**
   \class hit
   LArSoft Hit class equivalent data container
*/
class hit : public data_base {

public:

  static const size_t kMaxHits = 20000;

public:

  /// Default constructor
  hit(DATA::DATA_TYPE type=DATA::Hit);

  /// Default destructor
  virtual ~hit(){};

  /// Implementation of event-wise track data clear method
  virtual void clear_event();

  /// Implementation of track data address setter
  virtual void set_address(TTree* t);

protected:
  
  int    _no_hits;                 ///< Number of hits
  int    _hit_plane[kMaxHits];     ///< Wire plane ID
  int    _hit_wire[kMaxHits];      ///< Wire ID number
  int    _hit_channel[kMaxHits];   ///< Channel number
  double _hit_peakT[kMaxHits];     ///< Hit peak time
  double _hit_charge[kMaxHits];    ///< Hit charge
  double _hit_ph[kMaxHits];        ///< ???
  int    _hit_trkid[kMaxHits];     ///< Associated track ID

  /**
     - What is _hit_ph?
     - Associated track id?
     - Hit time width (sigma)?
  */

};

#endif

/** @} */ // end of doxygen group 
