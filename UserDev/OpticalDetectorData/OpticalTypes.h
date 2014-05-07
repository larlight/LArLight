// OpticalDetectorData/OpticalTypes.h
// William Seligman <seligman@nevis.columbia.edu>

// Define some common types using by the OpticalDetector classes.

#ifndef OpticalDetectorData_OpticalTypes_h
#define OpticalDetectorData_OpticalTypes_h

#include <TROOT.h>
//#include <stdint.h>
#include <vector>

namespace optdata {

  // Assuming that we're using 12-bit ADCs, define a type that's
  // guaranteed to hold that full range of bits.
  typedef UShort_t ADC_Count_t;

  // Other types, in case something changes later on:
  typedef unsigned int Channel_t;
  typedef unsigned int TimeSlice_t;
  typedef unsigned int Frame_t;

  // Use an enum for categories of ADC channels, triggers, etc.
  typedef enum _optical_category_t {
    kUndefined,
    kHighGain,
    kLowGain,
    kFEMCosmicHighGain,
    kFEMCosmicLowGain,
    kFEMBeamHighGain,
    kFEMBeamLowGain,
    kBeamPMTTrigger,
    kCosmicPMTTrigger,
  } Optical_Category_t;

  // Use an enum for categories of injection pulse
  typedef enum _optical_pulse_t {
    kUnknownPulse,
    kG4Photon,
    kDarkNoise,
    kPulseTypeMax,
  } Optical_Pulse_t;

} // namespace optdata

#endif // OpticalDetectorData_OpticalTypes_h
