#ifndef ELECCLOCK_CC
#define ELECCLOCK_CC

#include "ElecClock.h"
#include "TimeServiceException.h"

namespace util {

  ElecClock::ElecClock(double time,
		       double frame_period,
		       double frequency)
  : fTime(time),
    fFramePeriod(frame_period),
    fFrequency(frequency)
  { 
    if( fFrequency <= 0 ) throw util::TimeServiceException("Negative frequency is prohibited!"); 
  }

}

#endif
