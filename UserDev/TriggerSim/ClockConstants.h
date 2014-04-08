
#ifndef CLOCKCONSTANTS_H
#define CLOCKCONSTANTS_H

#include <limits>
#include <climits>

namespace util {

  /// Maximum time in nano-second
  const double kTIME_MAX = std::numeric_limits<double>::max();

  /// Default Frequency in GHz
  const double kDEFAULT_FREQUENCY = 1.e-9;

  /// Default Frame period in nano-second
  const double kDEFAULT_FRAME_PERIOD = 1.6e6;

  /// Default Optical clock speed in GHz
  const double kDEFAULT_FREQUENCY_OPTICAL = 0.064;

  /// Default TPC clock speed in GHz
  const double kDEFAULT_FREQUENCY_TPC = 0.002;

  /// Default Trigger clock speed in GHz
  const double kDEFAULT_FREQUENCY_TRIGGER = 0.016;

  /// Default G4 reference time in nano-second
  const double kDEFAULT_MC_CLOCK_T0 = 0;
}

#endif
