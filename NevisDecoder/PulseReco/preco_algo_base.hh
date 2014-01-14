/**
 * \file preco_algo_base.hh
 *
 * \ingroup PulseReco
 * 
 * \brief Class definition file of preco_algo_base
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup PulseReco

    @{*/

#ifndef PRECO_ALGO_BASE_HH
#define PRECO_ALGO_BASE_HH

#include "Base-TypeDef.hh"
#include <TString.h>
#include <cmath>
#include <functional>
#include <numeric>

struct pulse_param{
  
  double peak, area;
  double t_start, t_max, t_end;
  
  pulse_param(){
    reset_param();
  };
  
  void reset_param(){      
    peak = area = -1;
    t_start = t_max = t_end = -1;
  };
  
};

/**
   \class preco_algo_base
   The base class of pulse reconstruction algorithms. All algorithms should inherit from this calss
   to be executed by a manager class, pulse_reco. Note that this class does not depend on the rest
   of the framework except for the use of constants. In order to reconstruct a pulse, all it requires
   is a std::vector<UShort_t> type object (i.e. raw waveform), waveform pedestal, and its standard
   deviation. All of these are to be provided by an executer. Reconstructed pulse parameters are
   stored in the pulse_param struct object.

   All methods specified as "virtual" should be implemented by the inherit children class.

   This class provides some basic std::vector calculation algorithms such as integral, derivative,
   max and min algorithms. Inherit children classes are encouraged to use these provided methods
   when possible.
 */
class preco_algo_base : public decoder_base {

public:

  /// Default constructor
  preco_algo_base();

  /// Default destructor
  virtual ~preco_algo_base(){};

  /**
     A method to return the storage pulse_collection type enum.
     A children reco algo class should implement the unique,  relevant type to be returned.
     This base class use the basic one PULSE_COLLECTION.
  */
  virtual DATA_STRUCT::DATA_TYPE storage_type(){return DATA_STRUCT::PULSE_COLLECTION;};

  /// A method to be called event-wise to reset parameters
  virtual void reset();

  /** A core method: this executes the algorithm and stores reconstructed parameters
      in the pulse_param struct object.
   */
  virtual bool reco(const std::vector<UShort_t> *wf)=0;

  /** A getter for the pulse_param struct object. 
      Reconstruction algorithm may have more than one pulse reconstructed from an input waveform.
      Note you must, accordingly, provide an index key to specify which pulse_param object to be retrieved.
   */
  const pulse_param* get_pulse(size_t index=0) const;

  /// A getter for the number of reconstructed pulses from the input waveform
  size_t get_npulse() const   {return _pulse_v.size();};

  /// A setter for the pedestal mean value
  void set_ped_mean(double v) { _ped_mean = v; };

  /// A setter for the pedestal standard deviation
  void set_ped_rms(double v)  { _ped_rms = v; };

  /// A getter for the set pedestal mean value
  double ped_mean() const     {return _ped_mean; };

  /// A getter for the set pedestal standard deviation
  double ped_rms() const      {return _ped_rms;  };

protected:

  /// A container array of pulse_param struct objects to store (possibly multiple) reconstructed pulse(s).
  std::vector<pulse_param> _pulse_v;

  /// A subject pulse_param object to be filled with the last reconstructed pulse parameters
  pulse_param _pulse;

  /// Pedestal mean value
  double _ped_mean;

  /// Pedestal standard deviation
  double _ped_rms;

protected:

  /**
     A method to integrate an waveform from index "begin" to the "end". The result is filled in "result" reference.
     If the "end" is default (=0), then "end" is set to the last index of the waveform.
  */
  bool integral   (const std::vector<UShort_t> *wf, double &result, size_t begin=0, size_t end=0) const;

  /**
     A method to compute derivative, which is a simple subtraction of previous ADC sample from each sample.
     The result is stored in the input "diff" reference vector which is Int_t type as a derivative could be negative.
  */
  bool derivative (const std::vector<UShort_t> *wf, std::vector<Int_t> &diff, size_t begin=0, size_t end=0) const;

  /**
     A method to return the maximum value of ADC sample within the index from "begin" to "end".
     If the "end" is default (=0), then "end" is set to the last index of the waveform.
  */
  size_t max(const std::vector<UShort_t> *wf, double &result, size_t begin=0, size_t end=0) const;

  /**
     A method to return the minimum value of ADC sample within the index from "begin" to "end".
     If the "end" is default (=0), then "end" is set to the last index of the waveform.
  */
  size_t min(const std::vector<UShort_t> *wf, double &result, size_t begin=0, size_t end=0) const;

};


#endif

/** @} */ // end of doxygen group
