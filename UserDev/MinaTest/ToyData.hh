/**
 * \file ToyData.hh
 *
 * \ingroup MinaTest
 * 
 * \brief Class def header for a class ToyData
 *
 * @author kterao
 */

/** \addtogroup MinaTest

    @{*/
#ifndef TOYDATA_HH
#define TOYDATA_HH

#include "TObject.h"
#include <iostream>
#include <climits>
#include <limits>

/// A constant to indicate invalid integer value in the storage
static const int kINVALID_INT = std::numeric_limits<int>::max();

/**
   \class ToyData
   This is an example data product, ToyData.
   It is made per particle, and stores track ID and PDGID.
 */
class ToyData : public TObject {

public:

  /// Default constructor
  ToyData() { clear(); }

  /// Default destructor
  virtual ~ToyData(){}

  /// Method to clear data members
  void clear() { _track_id = _pdg_id = kINVALID_INT; }

  /// Method to set IDs
  void set_id(int const& track, int const& pdg) { _track_id = track; _pdg_id = pdg; }

  /// Getter for track id
  int track_id() const { return _track_id; }

  /// Getter for pdg id
  int pdg_id() const { return _pdg_id; }

protected:
  
  int _track_id; ///< Track ID
  int _pdg_id;   ///< PDG ID

  ClassDef(ToyData,1)

};

/**
   \class ToyEvent
   A toy data product created per event, it's basically a collection of ToyData.
   It holds an event id.
 */
class ToyEvent : public std::vector<ToyData> {

public:

  /// Default constructor
  ToyEvent() : std::vector<ToyData>() { clear(); }

  /// Default destructor
  virtual ~ToyEvent(){}

  /// Getter for event id
  int event_id() const { return _event_id; }

  /// Setter for event id
  void set_event_id(int id) { _event_id = id; }

  /// Method to clear data
  void clear() { _event_id = 0; std::vector<ToyData>::clear(); }

protected:
  
  int _event_id; ///< Event ID

  ClassDef(ToyEvent,1)

};

#endif
/** @} */ // end of doxygen group 

