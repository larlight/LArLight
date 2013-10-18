/**
 * \file data_base.hh
 *
 * \ingroup DataFormat
 * 
 * \brief Base class def for data class
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/
#ifndef DATA_BASE_HH
#define DATA_BASE_HH

#include "constants.hh"
#include <vector>
#include <limits>
#include <TObject.h>

/**
   \class data_base
   A base coass for PMT-wise & event-wise data holder class
 */
class data_base : public TObject {

public:

  /// Default constructor
  data_base() : TObject(){};

  /// Default copy constructor to avoid memory leak in ROOT streamer
  data_base(const data_base &original) : TObject(original)
  {};

  /// Default destructor
  virtual ~data_base(){};

  /// Clear method
  virtual void clear_data(){fRunNumber = fSubRunNumber = fEventID = -1;};

  /// run number setter
  void set_run      (int run) { fRunNumber    = run; };
  /// sub-run number setter
  void set_subrun   (int run) { fSubRunNumber = run; };
  /// event-id setter
  void set_event_id (int id ) { fEventID      = id;  };

  /// run number getter
  int run      () const { return fRunNumber;    };
  /// sub-run number getter
  int subrun   () const { return fSubRunNumber; };
  /// event-id getter
  int event_id () const { return fEventID;      };
  
private:

  /// Run number
  int fRunNumber;

  /// Sub-Run number
  int fSubRunNumber;

  /// Event ID
  int fEventID;

  ////////////////////////
  ClassDef(data_base,1)
  ////////////////////////

};

#endif
/** @} */ // end of doxygen group 
