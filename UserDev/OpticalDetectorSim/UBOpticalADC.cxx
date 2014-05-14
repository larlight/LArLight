#ifndef UBOPTICALADC_CXX
#define UBOPTICALADC_CXX

#include "UBOpticalADC.h"

namespace opdet {

  //------------------------
  UBOpticalADC::UBOpticalADC()
  //------------------------
  {
    fTimeInfo = ::util::TimeService::GetME().OpticalClock();
    fDuration = fTimeInfo.TickPeriod();
  }


  //--------------------------------------------------------------
  void UBOpticalADC::SetTimeInfo(const util::ElecClock &start_freq,
				double duration)
  //--------------------------------------------------------------
  {
    if(duration < start_freq.TickPeriod())

      throw OpticalDetectorException(Form("Waveform length (%g ns) smaller than the tick size (%g ns)",
					  duration,
					  start_freq.TickPeriod()
					  )
				     );

    fTimeInfo = start_freq;
    fDuration = duration;
  }

  //------------------------
  void UBOpticalADC::Reset()
  //------------------------
  {
    fSPE.Reset();
    fPED.Reset();
    fDuration = 0;
    fTimeInfo.SetTime(0);
    fInputPhotonTime.clear();
    fDarkPhotonTime.clear();
    fPhotonTime.clear();
  }

  //--------------------------------------------------------------
  void UBOpticalADC::SetPhotons(const std::vector<double>& g4time)
  //--------------------------------------------------------------
  {
    fInputPhotonTime.clear();
    fInputPhotonTime.reserve(g4time.size());
    for(auto const &v : g4time) fInputPhotonTime.push_back(v);
  }

  //--------------------------------------------------------------
  void UBOpticalADC::GenDarkNoise(double dark_rate, double period)
  //--------------------------------------------------------------
  {
    fDarkPhotonTime.clear();

    unsigned int dark_count = RandomServer::GetME().Poisson(dark_rate * fDuration);

    fDarkPhotonTime.resize(dark_count, fTimeInfo.Time());

    for(size_t i=0; i<dark_count; ++i)

      fDarkPhotonTime.at(i) += RandomServer::GetME().Uniform(period);
    
  }

  //-------------------------------------------------------------------
  void UBOpticalADC::GenWaveform(const unsigned int ch, 
				 std::vector<unsigned short>& high_wf,
				 std::vector<unsigned short>& low_wf  )
  //-------------------------------------------------------------------
  {
    //
    // Initialize
    //
    size_t nticks = fTimeInfo.Ticks(fDuration);
    std::vector<float> high_tmp_wf(nticks,0);
    std::vector<float> low_tmp_wf;
    low_tmp_wf.reserve(nticks);

    //
    // Generate Signal & DarkNoise
    //
    // Configure to generate high gain SPE
    fSPE.Reset();

    fSPE.SetT0(ChConfig::GetME().GetParameter(kT0,ch),
	       ChConfig::GetME().GetParameter(kT0Spread,ch));

    fSPE.SetGain(ChConfig::GetME().GetParameter(kHighGain,ch),
		 ChConfig::GetME().GetParameter(kHighGain,ch) *
		 ChConfig::GetME().GetParameter(kGainSpread,ch));
    
    GenDarkNoise(ChConfig::GetME().GetParameter(kDarkRate,ch),
		 fDuration);

    // Create combined photon time with QE applied on signal photons

    fPhotonTime.clear();
    fPhotonTime.reserve(fInputPhotonTime.size() * fDarkPhotonTime.size());
    const double qe = ChConfig::GetME().GetParameter(kQE,ch);
    for(auto const &v : fDarkPhotonTime) fPhotonTime.push_back(v);
    for(auto const &v : fInputPhotonTime)
      
      if(RandomServer::GetME().Uniform(1.) < qe) fPhotonTime.push_back(v);
    
    fSPE.SetPhotons(fPhotonTime);
    fSPE.Process(high_tmp_wf,fTimeInfo);

    // Copy waveform to low_tmp_wf with gain ratio
    double gain_ratio = (ChConfig::GetME().GetParameter(kLowGain,ch) / 
			 ChConfig::GetME().GetParameter(kHighGain,ch));
    for(auto const &v : high_tmp_wf) 

      low_tmp_wf.push_back(v * gain_ratio);

    //
    // Simulate pedestal
    //
    fPED.Reset();
    fPED.SetPedestal(ChConfig::GetME().GetParameter(kPedestalMean,ch),
		     ChConfig::GetME().GetParameter(kPedestalSpread,ch));
    fPED.Process(high_tmp_wf,fTimeInfo);
    fPED.Process(low_tmp_wf,fTimeInfo);

    // Make sure algorithms did not alter the waveform size

    if(high_tmp_wf.size()!=nticks || low_tmp_wf.size()!=nticks)

      throw OpticalDetectorException("Waveform length changed (prohibited)!");

    //
    // Digitize amplitude
    //
    high_wf.clear();
    low_wf.clear();
    high_wf.reserve(nticks);
    low_wf.reserve(nticks);

    for(size_t i=0; i<nticks; ++i) {

      float v = high_tmp_wf.at(i);

      if(v > ((float)kADC_MAX)) high_wf.push_back(kADC_MAX);

      else if(v < 0) high_wf.push_back(0);
      
      else high_wf.push_back( (unsigned short)(v));

      v = low_tmp_wf.at(i);

      if(v > ((float)kADC_MAX)) low_wf.push_back(kADC_MAX);

      else if(v < 0) low_wf.push_back(0);
      
      else low_wf.push_back( (unsigned short)(v));
      
    }
  }
  
}

#endif
