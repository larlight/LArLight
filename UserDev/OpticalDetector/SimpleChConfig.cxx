#ifndef SIMPLECHCONFIG_CXX
#define SIMPLECHCONFIG_CXX

#include "SimpleChConfig.h"

namespace opdet {

  //--------------------------------------------------------------------------------
  SimpleChConfig::SimpleChConfig() : fParams(kChConfigTypeMax,std::vector<float>()),
				     fDefault(kChConfigTypeMax,0)
  //--------------------------------------------------------------------------------
  {
    for(size_t i=0; i<kChConfigTypeMax; ++i)
      fParams.at(i).resize(kNChannels,0);

    /// Some hard-coded parameters for now
    for(size_t i=0; i<kNChannels; ++i) {
      fParams.at(kPedestalMean).at(i)   = 2048;
      fParams.at(kPedestalSpread).at(i) = 0.3;
      fParams.at(kQE).at(i)             = 0.01;
      fParams.at(kHighGain).at(i)       = 20;
      fParams.at(kLowGain).at(i)        = 4;
      fParams.at(kGainSpread).at(i)     = 0.05;
      fParams.at(kT0).at(i)             = 0;
      fParams.at(kT0Spread).at(i)       = 0;
      fParams.at(kDarkRate).at(i)       = 1.e-5;
    }

    fDefault.at(kPedestalMean)   = 2048;
    fDefault.at(kPedestalSpread) = 0.3;
    fDefault.at(kQE)             = 0.01;
    fDefault.at(kHighGain)       = 20;
    fDefault.at(kLowGain)        = 4;
    fDefault.at(kGainSpread)     = 0.05;
    fDefault.at(kT0)             = 0;
    fDefault.at(kT0Spread)       = 0;
    fDefault.at(kDarkRate)       = 1.e-5;
  }

  //---------------------------------------------------------------
  float SimpleChConfig::GetParameter(const ChConfigType_t type, 
				     const unsigned short ch) const
  //---------------------------------------------------------------
  {
    if(ch == kINVALID_CHANNEL) return fDefault.at(type);
    if(ch >= kNChannels) throw OpticalDetectorException("Invalid channel number provided!");
    return fParams.at(type).at(ch);
  }

  //-------------------------------------------------------------------------------------
  const std::vector<float>& SimpleChConfig::GetParameter(const ChConfigType_t type) const
  //-------------------------------------------------------------------------------------
  {
    return fParams.at(type);
  }

}

#endif
