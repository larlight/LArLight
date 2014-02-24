////////////////////////////////////////////////////////////////////////
// \file SimpleClock.h
//
// \brief SimpleClock class header file
//
// \author kazuhiro@nevis.columbia.edu
//
////////////////////////////////////////////////////////////////////////

#ifndef SIMPLECLOCK_H
#define SIMPLECLOCK_H

// ART includes

#include "TriggerData.h"
#include "ClockConstants.h"

// STL
#include <iostream>
#include <sstream>
// ROOT
#include <TString.h>

namespace trigger
{
  class SimpleClock {

  private:

    /// Private constructor w/o argument
    SimpleClock(){}

  public:

    /// Default constructor with fhicl parameters
    //SimpleClock(fhicl::ParameterSet const& pset);

    // Constructor w/o using fchil parameter
    SimpleClock(unsigned int frame_size,
		double clock_speed,
		double mc_start);

    /// Default destructor
    virtual ~SimpleClock(){}

    /// Method to report the current configuration
    void ReportConfig() const;

    /// Method to set trigger data
    void SetTriggerData(const trigdata::Trigger &td);

    /// Method to get a single tick period
    double TickPeriod() const { return 1./_clock_speed; }

    /// Method to get a frame tick count
    unsigned int FrameTick() const { return _frame_size; }
    
    /// Method to get a frame period
    double FramePeriod() const { return _frame_size/_clock_speed;}

    /// Method to get sample and frame count from period
    void Period2SampleFrame(const double period, unsigned int &sample, unsigned int &frame) const;

    /// Method to get sample and frame count from ticks
    void Tick2SampleFrame(const unsigned int tick, unsigned int &sample, unsigned int &frame) const;

    // Method to make sure trigger & beam data are set
    void CheckDataSet() const;
    
    //
    // Methods to get various timings w.r.t. Trigger time stamp
    //
    /// Returns TPC waveform start time (sec.) w.r.t. Trigger time stamp
    double TimeFromTrigger(const unsigned int sample, const unsigned int frame) const;

    /// Returns TPC waveform start time tick w.r.t. Trigger time stamp
    int   TickFromTrigger(const unsigned int sample, const unsigned int frame) const;

    //
    // Methods to get various timings w.r.t. Beam-gate-open time stamp
    //
    /// Returns TPC waveform start time (sec.) w.r.t. Trigger time stamp
    double TimeFromBeamGate(const unsigned int sample, const unsigned int frame) const;

    /// Returns TPC waveform start time tick w.r.t. Trigger time stamp
    int   TickFromBeamGate(const unsigned int sample, const unsigned int frame) const;

    /// Sample number converter
    unsigned int Sample(const SimpleClock& clock, const unsigned int sample) const;

    //
    // Methods to convert MC time (sec. in double) to various time ticks
    //
    /// Method to get frame number
    unsigned int MCFrame(const double time_sec) const;

    /// Method to get sample number
    unsigned int MCSample(const double time_sec) const;

  protected:
    
    /// Function to set the frame size
    void SetFrameSize(unsigned int n) {_frame_size = n;}

    /// Function to set the clock speed in Hz
    void SetClockSpeed(double speed)  {_clock_speed = speed;}

    /// Function to set the MC start time in G4 clock [s]
    void SetMCStart(double t) { _mc_start = t;}

  protected:

    bool   _data_set;    ///< Boolean to check if data is set or not

    int    _frame_size;  ///< frame size in seconds

    double _mc_start;    ///< MC event generation window start time

    unsigned int _beam_sample; ///< Beam gate start sample number in 64MHz clock

    unsigned int _beam_frame;  ///< Beam gate start frame number 

    unsigned int _trigger_sample; ///< Trigger sample number in 64MHz clock

    unsigned int _trigger_frame;  ///< Trigger frame number

    double _clock_speed; ///< Clock speed in Hz
    
  }; // class ClusterMatchAlg
  
} //namespace cluster
#endif
