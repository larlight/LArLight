////////////////////////////////////////////////////////////////////////
// \file OpDigiProperties.h
//
// \brief service containing information about optical digitization
//
// \author bjpjones@mit.edu
// \author kazuhiro@nevis.columbia.edu
//
////////////////////////////////////////////////////////////////////////

#ifndef OPDET_OPDIGIPROPERTIES_H
#define OPDET_OPDIGIPROPERTIES_H

// LArSoft includes
#include "OpticalDetectorData/OpticalTypes.h"
#include "Geometry/Geometry.h"

// ART includes
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art/Framework/Services/Registry/ServiceMacros.h"

// ROOT includes
#include "TF1.h"

namespace opdet
{
    class OpDigiProperties {
    public:

      OpDigiProperties(fhicl::ParameterSet const& pset, art::ActivityRegistry& reg);
      ~OpDigiProperties();

      void reconfigure(fhicl::ParameterSet const& p);

      /// Returns sample frequency in MHz
      double SampleFreq()   { return fSampleFreq; }
      /// Returns window start time in us ... with respect to MC photon T0
      double TimeBegin()    { return fTimeBegin;  }
      /// Returns window end time in us ... with respect to MC photon T0
      double TimeEnd()      { return fTimeEnd;    }

      /**
	 DEPRECATED as far as Kazu is concerned for UBooNE PMT simulation.
	 Any comment? --Kazu 08/05/2013
      */
      std::vector<double> WaveformInit(std::string WaveformFile);

      /** 
	  Convert the given time into time-slice number.
	  Input time should be in ns unit and measurd w.r.t. MC photon T0
      */
      optdata::TimeSlice_t GetTimeSlice(double time_ns);

      /// Returns quantum efficiency
      double QE()              const { return fQE;              }
      /// Returns rate of dark noise
      double DarkRate()        const { return fDarkRate;        }
      /// Returns rate of pedestal fluctuation
      double PedFlucRate()     const { return fPedFlucRate;     }
      /// Returns amplitude of pedestal fluctuation
      optdata::ADC_Count_t PedFlucAmp()      const { return fPedFlucAmp;      }
      /// Returns the saturation scale of the electronics
      optdata::ADC_Count_t SaturationScale() const { return fSaturationScale; }
      /// Returns the ADCBaseline set mean value
      optdata::ADC_Count_t ADCBaseline() const { return fADCBaseline; }
      /// Returns WF time constant used in analytical model
      double WFTimeConstant()  const { return fWFTimeConstant;  }
      /// Returns WF power factor used in analytical model
      double WFPowerFactor()   const { return fWFPowerFactor;   }
      /// Returns set mean gain value for LOW gain
      double LowGainMean  () const { return fLowGainMean;  }
      /// Returns set mean gain value for HIGH gain
      double HighGainMean () const { return fHighGainMean; }
      /// Returns generated LOW gain value for input channel (PMT-to-PMT spread applied)
      double LowGainMean  (optdata::Channel_t ch) const;
      /// Returns generated HIGH gain value for input channel (PMT-to-PMT spread applied)
      double HighGainMean (optdata::Channel_t ch) const;
      
      /// Returns set value for intrinsic gain spread (common to HIGH and LOW)
      double GainSpread        () const { return fGainSpread;         }
      /// Returns set value for PMT-to-PMT gain spread 
      double GainSpread_PMT2PMT() const { return fGainSpread_PMT2PMT; }
      /// Generate & return LOW gain value for an input channel using mean & spread for this channel
      double LowGain(optdata::Channel_t ch) const;
      /// Generate & return HIGH gain value for an input channel using mean & spread for this channel
      double HighGain(optdata::Channel_t ch) const;

      /// Returns a vector of double which represents a binned SPE waveform
      std::vector<double> SinglePEWaveform() const { return fWaveform;        } 
      /// Returns an array of HIGH gain
      std::vector<double> HighGainArray()    const { return fHighGainArray;   }
      /// Returns an array of LOW gain
      std::vector<double> LowGainArray()     const { return fLowGainArray;    }
      /// Returns an array of gain spread
      std::vector<double> GainSpreadArray()  const { return fGainSpreadArray; }
      /// Returns an array of generated pedestal mean value per channel
      std::vector<optdata::ADC_Count_t> PedMeanArray() const {return fPedMeanArray;}

      /// Utility function ... To be verified (Kazu 08/05/13)
      double GetSPEArea();
      /// Utility function ... To be verified (Kazu 08/05/13) 
      double GetSPECumulativeArea();
      /// Utility function ... To be verified (Kazu 08/05/13)
      double GetSPEAmplitude();
      /// Utility function ... To be verified (Kazu 08/05/13)
      double GetSPECumulativeAmplitude();
      
    private:

      double fSampleFreq;
      double fTimeBegin;
      double fTimeEnd;
      double fPERescale;

      std::vector<double> GenEmpiricalWF(std::string WaveformFile);
      std::vector<double> GenAnalyticalWF();
      void GenerateWaveform();
      void FillGainArray();
      void FillPedMeanArray();

      bool   fUseEmpiricalGain;
      bool   fUseEmpiricalShape;
      double fQE;
      double fPedFlucRate;
      optdata::ADC_Count_t fPedFlucAmp;
      double fDarkRate;
      optdata::ADC_Count_t fSaturationScale;
      optdata::ADC_Count_t fADCBaseline;
      double fADCBaseSpread;
      double fLowGainMean;
      double fHighGainMean;
      double fVoltageAmpForSPE;
      double fWFTimeConstant;
      double fWFPowerFactor;
      double fWFLength;
      double fGainSpread;
      double fGainSpread_PMT2PMT;
      TF1* fAnalyticalSPE;

      std::string fHighGainFile;
      std::string fLowGainFile;
      std::string fWaveformFile;
      std::string fGainSpreadFile;
      std::vector<double> fWaveform;
      std::vector<double> fLowGainArray;
      std::vector<double> fHighGainArray;
      std::vector<double> fGainSpreadArray;
      std::vector<optdata::ADC_Count_t> fPedMeanArray;
      art::ServiceHandle<geo::Geometry> fGeometry;
 
    }; // class OpDigiProperties

} //namespace opdet

DECLARE_ART_SERVICE(opdet::OpDigiProperties, LEGACY)

#endif
