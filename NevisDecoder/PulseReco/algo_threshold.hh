/**
 * \file algo_threshold.hh
 *
 * \ingroup PulseReco
 * 
 * \brief Class definition file of algo_threshold
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup PulseReco

    @{*/
#ifndef ALGO_THRESHOLD_HH
#define ALGO_THRESHOLD_HH

#include "preco_algo_base.hh"


/**
   \class algo_threshold
   This class implements threshold algorithm to preco_algo_base class.
   The algorithm defines a pulse start when it find an ADC count above a threshold.
   The threshold is defined from two values: value 1 = user defined ADC count, and
   value 2 = n * pedestal standard deviation where "n" is specified by a user.
   The algorithm takes a larger value among those two values, and use it as the 
   minimum threshold to declaire a pulse firing. Once it finds the pulse start with
   the ADC value above the threshold, it scands consecutive ADC samples until it 
   finds another ADC sample that goes below the threshold, which is considere to be
   the end of the pulse.
 */
class algo_threshold : public preco_algo_base {

public:

  /// Default constructor
  algo_threshold();

  /// Default destructor
  virtual ~algo_threshold(){};

  /**
     A method to return the storage pulse_collection type enum.
     This one returns DATA_STRUCT::THRES_WIN_PULSE_COLLECTION
  */
  virtual DATA_STRUCT::DATA_TYPE storage_type(){return DATA_STRUCT::THRES_WIN_PULSE_COLLECTION;};

  /// Implementation of preco_algo_base::reco method
  virtual bool reco(const std::vector<UShort_t> *wf);

  /// Implementation of preco_algo_base::reset method
  virtual void reset();

  /// A method to set user-defined ADC threshold value
  void set_adc_threshold(double v) {_adc_thres = v;};

  /** 
      A method to set a multiplication factor to the pedestal standard deviation
      which is used as one of two input values to define a threshold.
  */
  void set_nsigma(double v) {_nsigma = v;};
  
protected:

  /// A variable holder for a user-defined absolute ADC threshold value
  double _adc_thres;

  /// A variable holder for a multiplicative factor for the pedestal standard deviation to defin the threshold.
  double _nsigma;

};

#endif

/** @} */ // end of doxygen group
