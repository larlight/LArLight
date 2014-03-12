/**
 * \file pulse_selector_simple.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class pulse_selector_simple
 *
 * @author kazuhiro
 */

/** \addtogroup Analysis

    @{*/

#ifndef PULSE_SELECTOR_SIMPLE_HH
#define PULSE_SELECTOR_SIMPLE_HH

#include "pulse_selector_base.hh"

namespace larlight {

  /**
     \class pulse_selector_simple
     This is an implementatin of simple pulse selection algorithm on top of pulse_selector_base abstract class.
   */
  class pulse_selector_simple : public pulse_selector_base {

  public:

    /// Default constructor
    pulse_selector_simple();

    /// Default destructor
    virtual ~pulse_selector_simple(){};

    /** IMPLEMENT in pulse_selector_simple.cc!
	Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize(){return true;};

    /** IMPLEMENT in pulse_selector_simple.cc! 
	Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage){return true;};

    /** IMPLEMENT in pulse_selector_simple.cc! 
	Finalize method to be called after all events processed.
    */
    virtual bool finalize(){return true;};

    void reset_cuts();

    virtual bool select_pulse(const pulse* data);

    //
    // Setters
    //

    /// Setter for the range of pusle start time to select pulses of interest
    void set_range_pulse_start_time(double v1, double v2)   {_cut_tstart.first=v1;   _cut_tstart.second=v2;   };

    /// Setter for the range of pusle end time to select pulses of interest..
    void set_range_pulse_end_time(double v1, double v2)     {_cut_tend.first=v1;     _cut_tend.second=v2;     };

    /// Setter for the range of pusle amplitude to select pulses of interest..
    void set_range_pulse_amp(double v1, double v2)          {_cut_amp.first=v1;      _cut_amp.second=v2;      };

    /// Setter for the range of integrated charge to select pulses of interest..
    void set_range_pulse_charge(double v1,double v2)        {_cut_charge.first=v1;   _cut_charge.second=v2;   };

    /// Setter for the range of pedestal mean value to select pulses of interest..
    void set_range_ped_mean(double v1, double v2)           {_cut_pedbase.first=v1;  _cut_pedbase.second=v2;  };

    /// Setter for the range of pedestal rms to select pulses of interest..
    void set_range_ped_rms(double v1, double v2)            {_cut_pedrms.first=v1;   _cut_pedrms.second=v2;   };

    /// Setter for the range of frame id to select pulses of interest.
    void set_range_frame_number(UInt_t v1, UInt_t v2) {_cut_frame_number.first=v1; _cut_frame_number.second=v2; };

    /// Setter for the range of sample_number to select pulses of interest.
    void set_range_sample_number(UInt_t v1, UInt_t v2) {_cut_sample_number.first=v1; _cut_sample_number.second=v2; };

    /// Setter for the range of channel numbers to select pulses of interest.
    void set_range_channel(UShort_t v1, UShort_t v2) {_cut_channels.first=v1; _cut_channels.second=v2;};

    /// Setter for the range of event charge sum to select pulses of interest.
    void set_range_sum_charge(double v1, double v2)         {_cut_sum_charge.first=v1; _cut_sum_charge.second=v2;};

    /// Setter for the range of event peak sum to select pulses of interest.
    void set_range_sum_peak(double v1, double v2)           {_cut_sum_peak.first=v1; _cut_sum_peak.second=v2;};

    /// Setter for the range of event-wise number of pulses to select pulses of interest.
    void set_range_npulse(double v1, double v2)             {_cut_npulse.first=v1; _cut_npulse.second=v2;};


    //
    // Getters
    //

    /// Getter for the range of pusle start time to select pulses of interest
    const std::pair<double,double> range_pulse_start_time() const   {return _cut_tstart;};

    /// Getter for the range of pusle end time to select pulses of interest..
    const std::pair<double,double> range_pulse_end_time() const     {return _cut_tend;};

    /// Getter for the range of pusle amplitude to select pulses of interest..
    const std::pair<double,double> range_pulse_amp() const          {return _cut_amp;};

    /// Getter for the range of integrated charge to select pulses of interest..
    const std::pair<double,double> range_pulse_charge() const       {return _cut_charge;};

    /// Getter for the range of pedestal mean value to select pulses of interest..
    const std::pair<double,double> range_ped_mean() const           {return _cut_pedbase;};

    /// Getter for the range of pedestal rms to select pulses of interest..
    const std::pair<double,double> range_ped_rms() const            {return _cut_pedrms;};

    /// Getter for the range of frame number to select pulses of interest.
    const std::pair<UInt_t,UInt_t> range_frame_number() const {return _cut_frame_number;};

    /// Getter for the range of sample_number to select pulses of interest.
    const std::pair<UInt_t,UInt_t> range_sample_number() const {return _cut_sample_number;};

    /// Getter for the range of channel numbers to select pulses of interest.
    const std::pair<UInt_t,UInt_t> range_channel() const  {return _cut_channels;};

    /// Getter for the range of event charge sum to select pulses of interest.
    const std::pair<double,double> range_sum_charge() const         {return _cut_sum_charge;};

    /// Getter for the range of event peak sum to select pulses of interest.
    const std::pair<double,double> range_sum_peak() const           {return _cut_sum_peak;};

  protected:

    /// pairs to define a range of pulse parameters for displaying pulses.
    std::pair<double,double> _cut_tstart, _cut_tend, _cut_amp, _cut_charge, _cut_pedbase, _cut_pedrms;

    /// pairs to define a range of event parameters for displaying pulses.
    std::pair<double,double> _cut_sum_charge, _cut_sum_peak;

    /// pairs to define a range of event parameters for displaying pulses.
    std::pair<UInt_t,UInt_t> _cut_npulse;  

    /// a pair to define a range of frame number and sample_number number for displaying pulses.
    std::pair<UInt_t,UInt_t>  _cut_frame_number, _cut_sample_number;

    /// a pair to define a range of channel numbers for displaying pulses.
    std::pair<UShort_t,UShort_t> _cut_channels;
  };
}

#endif

/** @} */ // end of doxygen group 
