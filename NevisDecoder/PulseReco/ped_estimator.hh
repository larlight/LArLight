/**
 * \file ped_estimator.hh
 *
 * \ingroup PulseReco
 * 
 * \brief Class definition file of ped_estimator
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup PulseReco

    @{*/


#ifndef PED_ESTIMATOR_HH
#define PED_ESTIMATOR_HH

#include "Base-TypeDef.hh"
#include "DataFormat-TypeDef.hh"
#include <cmath>

/**
   \class ped_estimator
   A class that calculates pedestal mean & standard deviation (here and elsewhere called as "RMS").   
 */
class ped_estimator : public decoder_base {

public:

  /// Default constructor
  ped_estimator();

  /// Default destructor
  virtual ~ped_estimator(){};

  /// Method to compute a pedestal of the input waveform using "nsample" ADC samples from "start" index.
  void compute_pedestal(const std::vector<UShort_t>* wf, size_t start, size_t nsample);

  /// Getter of the pedestal mean value
  double mean(){return _mean;};

  /// Getter of the pedestal standard deviation
  double sigma() {return _sigma;};

protected:

  /// A variable holder for pedestal mean value
  double _mean;

  /// A variable holder for pedestal standard deviation
  double _sigma;

};

#endif

/** @} */ // end of doxygen group
