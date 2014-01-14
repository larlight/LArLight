/**
 * \file pulse.hh
 *
 * \ingroup DataFormat
 * 
 * \brief Physics pulse data base class def header file.
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/
#ifndef PULSE_HH
#define PULSE_HH

#include "data_base.hh"

namespace larlight {
  /**
   \class pulse 
   Channel-wise data member class to hold reconstructed pulse information
  */
  class pulse : public data_base {

  public:

    /// Default constructor
    pulse(DATA::DATA_TYPE type=DATA::DATA_TYPE_MAX);

    /// Default copy constructor
    pulse(const pulse& original) : data_base(original),
					     _channel_number(original._channel_number),
					     _ped_mean(original._ped_mean),
					     _ped_rms(original._ped_rms),
					     _pulse_peak(original._pulse_peak),
					     _charge(original._charge),
					     _start_time(original._start_time),
					     _end_time(original._end_time),
					     _max_time(original._max_time),
					     _channel_frame_id(original._channel_frame_id),
					     _timeslice(original._timeslice),
                                             _disc_id(original._disc_id)
    {};

    /// Setter for the channel number
    void set_channel_number   (UShort_t ch)   {_channel_number=ch;};

    /// Setter for pedestal mean
    void set_ped_mean (double v) {_ped_mean=v;};

    /// Setter for pedestal rms
    void set_ped_rms  (double v) {_ped_rms=v;};

    /// Setter for pulse peak height
    void set_pulse_peak (double v) {_pulse_peak=v;};

    /// Setter for pulse charge (integral)
    void set_charge (double v) {_charge=v;};

    /// Setter for pulse start time
    void set_start_time(double v) {_start_time=v;};

    /// Setter for pulse end time
    void set_end_time(double v) {_end_time=v;};

    /// Setter for pulse max time (peak point)
    void set_max_time(double v) {_max_time=v;};

    /// Setter for discriminator id
    void set_disc_id(FEM::DISCRIMINATOR id) { _disc_id=id; }

    /// Setter for original waveform's frame number
    void set_channel_frame_id(UInt_t id) { _channel_frame_id=id; }

    /// Setter for original waveform's sample time-slice
    void set_timeslice(UInt_t t) { _timeslice=t; }

    /// Getter for discriminator id
    FEM::DISCRIMINATOR disc_id() const { return _disc_id;}

    /// Getter for original waveform's frame number
    UInt_t frame_id() const { return _channel_frame_id;}

    /// Getter for original waveform's sample time-slice
    UInt_t timeslice() const { return _timeslice;}

    /// Getter for the channel number
    UShort_t channel_number() const {return _channel_number;};

    /// Getter for pedestal mean
    double ped_mean () const { return _ped_mean;};

    /// Getter for pedestal rms
    double ped_rms  () const { return _ped_rms;};

    /// Getter for pulse peak height
    double pulse_peak () const { return _pulse_peak;};

    /// Getter for pulse charge (integral)
    double charge () const { return _charge;};

    /// Getter for pulse start time
    double start_time () const { return _start_time;};

    /// Getter for pulse end time
    double end_time () const { return _end_time;};

    /// Getter for pulse max time (peak point)
    double max_time () const { return _max_time;};

    /// Method to clear currently held data contents in the buffer
    virtual void clear_data();

    /// Default destructor
    ~pulse(){};


  protected:

    /// Internal function to reset variables.
    void init_vars();

    UShort_t _channel_number; ///< Channel number
    double   _ped_mean;          ///< pedestal mean
    double   _ped_rms;           ///< pedestal std. deviation
    double   _pulse_peak;        ///< peak height  
    double   _charge;            ///< charge sum (integral)
    double   _start_time;        ///< pulse start time
    double   _end_time;          ///< pulse end time
    double   _max_time;          ///< pulse max time (where highestpeak is)
    UInt_t   _channel_frame_id;  ///< original waveofrm's readout frame id
    UInt_t   _timeslice;         ///< original waveform's sample number
    FEM::DISCRIMINATOR _disc_id; ///< original waveform's discriminator id

    ////////////////////////
    ClassDef(pulse,4)
    ////////////////////////
  };

 /**
   \class event_pulse 
   PMT-wise data member class to hold a collection of ADC samples.
  */
  class event_pulse : public std::vector<pulse>,
		      public data_base {

  public:

    /// Default constructor
    event_pulse(DATA::DATA_TYPE type=DATA::DATA_TYPE_MAX) 
      : data_base(type) {clear_data();}

    /// Default copy constructor
    event_pulse(const event_pulse& original)
      : std::vector<pulse>(original),
	data_base(original),
	_sum_charge(original._sum_charge),
	_sum_peak(original._sum_peak),
	_npulse(original._npulse)
    {}

    /** Method to calculate summed variables
	This iterates over stored pulse and sum charge, peark height, and # pulses.
	Note that, if a user is responsible for storing all pulses, it is faster if a
	user calculate this sum as (s)he stores pulses, then use setter functions for
	each parameters to set a value. This method is useful in case a user looses a
	track OR a user wish to re-calculate the sum on stored pulse for some reasons.
     */
    void calculate_sum();

    /// Getter for charge sum
    double sum_charge() const {return _sum_charge;}

    /// Getter for summed peak
    double sum_peak() const   {return _sum_peak;}

    /// Getter for summed # of pulses
    double npulse() const     {return _npulse;}

    /// Setter for charge sum
    void set_sum_charge(double v) {_sum_charge=v;}

    /// Setter for summed peak
    void set_sum_peak(double v)   {_sum_peak=v;}

    /// Setter for number of pulses.
    void set_npulse(UInt_t v)   {_npulse=v;}

    /// Method to clear currently held data contents in the buffer
    virtual void clear_data();

    /// Default destructor
    ~event_pulse(){}


  private:

    /// Internal function to reset variables.
    void init_vars();

    double _sum_charge;  ///< summed charge from each pulse
    double _sum_peak;    ///< summed peak from each pulse 
    UInt_t _npulse;      ///< summed number of pulses stored

    ////////////////////////
    ClassDef(event_pulse,1)
    ////////////////////////
  };

 }

#endif
/** @} */ // end of doxygen group 
