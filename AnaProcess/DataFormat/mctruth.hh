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

/**
   \class mctruth
   LArSoft MCTruth class equivalent data container
*/
class mctruth : public data_base {

public:
  
  /// Default constructor
  mctruth(){clear_data();};

  /// Copy constructor
  mctruth(const mctruth& original) : data_base(original){};

  /// Default destructor
  virtual ~mctruth(){};

  /// Clear method
  void clear_data(){data_base::clear_data();};
  
private:

  ////////////////////////
  ClassDef(mctruth,1)
  ////////////////////////

};

/**
 \class event_mc
 A collection storage class of multiple mctruth.
*/
class event_mc : public data_base {
  
public:

  /// Default constructor
  event_mc() : data_base() {clear_data();};
  
  /// Default copy constructor
  event_mc(const event_mc& original) : data_base(original),
				       fMCTruth_v(original.fMCTruth_v)
  {};

  /// Default destructor
  ~event_mc(){};
  
  /// Method to clear currently held data contents in the buffer
  virtual void clear_data(){data_base::clear_data(); fMCTruth_v.clear();};

  /// Method to add a track
  inline void  add_mctruth(const mctruth mc) { fMCTruth_v.push_back(mc);  };

  /// Method to get an array of tracks
  inline const std::vector<mctruth>& GetMCCollection() { return fMCTruth_v;};

private:

  std::vector<mctruth> fMCTruth_v;

  ////////////////////////
  ClassDef(event_mc,1)
  ////////////////////////
};

#endif

/** @} */ // end of doxygen group 
