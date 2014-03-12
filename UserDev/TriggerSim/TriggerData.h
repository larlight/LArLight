// TriggerData/TriggerData.h
#ifndef TRIGGER_H
#define TRIGGER_H

// C++ includes
#include <vector>
#include <limits>
#include <stdexcept>
#include <iostream>
namespace trigdata {

  class Trigger {

  public:

    /// Default ctor
    Trigger();

    /// Alternative constructor
    Trigger(unsigned int counter,
	    unsigned int trigger_sample,
	    unsigned int trigger_frame,
	    unsigned int beamgate_sample,
	    unsigned int beamgate_frame,
	    uint32_t bits)
      : fTriggerNumber(counter),
	fTriggerSample(trigger_sample),
	fTriggerFrame(trigger_frame),
	fBeamGateSample(beamgate_sample),
	fBeamGateFrame(beamgate_frame),
	fTriggerBits(bits)
    {}

    /// Default dtor
    ~Trigger() {};

    unsigned int   TriggerNumber()  const { return fTriggerNumber;  }
    unsigned int   TriggerSample()  const { return fTriggerSample;  }
    unsigned int   TriggerFrame()   const { return fTriggerFrame;   }  
    unsigned int   BeamGateSample() const { return fBeamGateSample; }
    unsigned int   BeamGateFrame()  const { return fBeamGateFrame;  }
    unsigned int   TriggerBits()    const { return fTriggerBits;    }
    bool           Triggered( const unsigned char bit) const;

  private:
    unsigned int fTriggerNumber;  ///< Trigger counter
    unsigned int fTriggerSample;  ///< Trigger timing: sample number
    unsigned int fTriggerFrame;   ///< Trigger timing: frame number
    unsigned int fBeamGateSample; ///< Beam-Gate start timing: sample number
    unsigned int fBeamGateFrame;  ///< Beam-Gate start timing: frame number
    unsigned int fTriggerBits;    ///< Trigger bits ... dedicated bit-by-bit function available

  };

} // namespace trigdata

#endif 
