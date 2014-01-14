/**
 * \file algo_fixed_window.hh
 *
 * \ingroup PulseReco
 * 
 * \brief Class definition file of algo_fixed_window
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup PulseReco

    @{*/

#ifndef ALGO_FIXED_WINDOW_HH
#define ALGO_FIXED_WINDOW_HH

#include "preco_algo_base.hh"

/**
   \class algo_fixed_window
   This class implements fixed_window algorithm to preco_algo_base class.
   The algorithm defines a pulse in user-specified time window.
   A typical usage is to set the beginning of the window to be 0 (= start of the waveform)
   and integrate over the time of interest. By default, the ending is set to index=0, in 
   which case it uses the ending index of the input waveform (i.e. full integration).
 */
class algo_fixed_window : public preco_algo_base {

public:

  /// Default constructor
  algo_fixed_window();
  
  /// Default destructor
  virtual ~algo_fixed_window(){};

  /**
     A method to return the storage pulse_collection type enum.
     This one returns DATA_STRUCT::FIXED_WIN_PULSE_COLLECTION
  */
  virtual DATA_STRUCT::DATA_TYPE storage_type(){return DATA_STRUCT::FIXED_WIN_PULSE_COLLECTION;};

  /// Implementation of preco_algo_base::reco() method
  virtual bool reco(const std::vector<UShort_t> *wf);

  /// Implementation of preco_algo_base::reset() method
  virtual void reset();

  /// Specify the beginning of fixed pulse time window
  void set_start_index(size_t index) {_index_start = index;};

  /// Specify the end of fixed pulse time window
  void set_end_index  (size_t index) {_index_end   = index;};
  
protected:

  size_t _index_start; ///< index marker for the beginning of the pulse time window
  size_t _index_end;   ///< index marker for the end of pulse time window

};

#endif

/** @} */ // end of doxygen group
