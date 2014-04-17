/**
 * \file TimeService.h
 *
 * \ingroup TimeService
 * 
 * \brief Class def header for a class TimeService
 *
 * @author kterao
 */

/** \addtogroup TimeService

    @{*/
#ifndef TIMESERVICE_H
#define TIMESERVICE_H

#include <iostream>
#include "ElecClock.h"

namespace util {
  /**
     \class TimeService
     This class provides
  */
  class TimeService{
    
  private:
    
    /// Default constructor
    TimeService() 
      : fG4RefTime         (kDEFAULT_MC_CLOCK_T0),
	fFramePeriod       (kDEFAULT_FRAME_PERIOD),
	fClockSpeedTPC     (kDEFAULT_FREQUENCY_TPC),
	fClockSpeedOptical (kDEFAULT_FREQUENCY_OPTICAL),
	fClockSpeedTrigger (kDEFAULT_FREQUENCY_TRIGGER)
    {}
      
    /// Default destructor
    virtual ~TimeService(){};

    /// Electronics clock counting start time in G4 time frame [ns]
    double fG4RefTime;

    /// Frame period
    double fFramePeriod;

    /// TPC clock frequency in GHz
    double fClockSpeedTPC;

    /// Optical clock frequency in GHz
    double fClockSpeedOptical;

    /// Trigger clock frequency in GHz
    double fClockSpeedTrigger;

    /// singleton self pointer
    static TimeService* _me;

  public:

    static const TimeService& GetME() {
      
      if(!_me) _me = new TimeService;
      return *_me;

    }

    /// Given Geant4 time [ns], returns relative time [ns] w.r.t. electronics time T0 
    double G4ToElecTime(double g4_time) {return g4_time - fG4RefTime; }

    //
    // Getters of ElecClock with input time [ns] w.r.t. clock counting start 
    //
    /// Create a TPC clock for a given time [ns] from clock counting start
    ElecClock TPCClock(double time=0) const { return ElecClock(time,fFramePeriod,fClockSpeedTPC); }

    /// Create a Optical clock for a given time [ns] from clock counting start
    ElecClock OpticalClock(double time=0) const { return ElecClock(time,fFramePeriod,fClockSpeedOptical); }

    /// Create a Trigger clock for a given time [ns] from clock counting start    
    ElecClock TriggerClock(double time=0) const { return ElecClock(time,fFramePeriod,fClockSpeedTrigger); }

    //
    // Getters of ElecClock with input sample/frame number of the subject clock 
    //
    /// Create a TPC clock for a given sample/frame number in TPC clock frequency
    ElecClock TPCClock(unsigned int sample,
		       unsigned int frame) const
    { ElecClock clock = TPCClock(); clock.SetTime(sample,frame); return clock; }

    /// Create a Optical clock for a given sample/frame number in Optical clock frequency
    ElecClock OpticalClock(unsigned int sample,
			   unsigned int frame) const
    { ElecClock clock = OpticalClock(); clock.SetTime(sample,frame); return clock; }

    /// Create a Trigger clock for a given sample/frame number in Trigger clock frequency
    ElecClock TriggerClock(unsigned int sample,
			   unsigned int frame) const
    { ElecClock clock = TriggerClock(); clock.SetTime(sample,frame); return clock; }

    //
    // Getters of ElecClock for TPC/Optical readout window start time
    //
    // Create TPC clock with time set to the beginning of TPC readout window
    //ElecClock ReadOutBeginTPC(trigdata::Trigger trig) const
    //{ return OpticalClock(trig.ReadOutStartTPC()); }

    // Create Optical clock with time set to the beginning of Optical readout window
    //ElecClock ReadOutBeginOptical(trigdata::Trigger trig) const
    //{ return OpticalClock(trig.ReadOutStartOptical()); }

  };
  
}
#endif
/** @} */ // end of doxygen group 

