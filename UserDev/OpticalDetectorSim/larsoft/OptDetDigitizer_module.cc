// OptDetDigitizer_module.cc
// Kazuhiro Terao <kazuhiro@nevis.columbia.edu>, Jul 2013
// based on code by Ben Jones and Christie Chiu, MIT, Sept 2012
//   bjpjones@mit.edu, cschiu@mit.edu
//
// This module starts from MC truth sim::OnePhoton objects
// and produces a digitized waveform.

// LArSoft includes
#include "Simulation/SimListUtils.h"
#include "Simulation/SimPhotons.h"
#include "Simulation/sim.h"
#include "OpticalDetectorData/OpticalTypes.h"
#include "OpticalDetectorData/ChannelData.h"
#include "OpticalDetectorData/ChannelDataGroup.h"
#include "OpticalDetector/OpDigiProperties.h"
#include "Geometry/Geometry.h"

// ART includes
#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Persistency/Common/Ptr.h"
#include "art/Persistency/Common/PtrVector.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

// CLHEP includes
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandGaussQ.h"
#include "CLHEP/Random/RandPoisson.h"

// C++ language includes
#include <iostream>
#include <sstream>
#include <cstring>
#include <vector>

namespace opdet {

  class OptDetDigitizer : public art::EDProducer{
  public:
    
    OptDetDigitizer(const fhicl::ParameterSet&);
    virtual ~OptDetDigitizer();
    
    void produce(art::Event&);
    
    void beginJob();
    
  private:
    
    // The parameters we'll read from the .fcl file.
    std::string fInputModule;              // Input tag for OpDet collection
    float fSampleFreq;                     // in MHz
    float fTimeBegin;                      // in us
    float fTimeEnd;                        // in us
    float fQE;                             // quantum efficiency of opdet
    optdata::ADC_Count_t fSaturationScale;           // adc count w/ saturation occurs
    std::vector<optdata::ADC_Count_t> fPedMeanArray; // Array of pedestal baseline (per ch)
    float fDarkRate;                       // Noise rate in Hz
    optdata::ADC_Count_t fPedFlucAmp;                     // Pedestal fluctuation amplitude
    float fPedFlucRate;                    // Pedestal fluctuation rate
    float fWFRandTimeOffsetLow;            // The lower bound of WF's T=0 offset from Trigger
    float fWFRandTimeOffsetHigh;           // The upper bound of WF's T=0 offset from Trigger
    std::vector<double> fSinglePEWaveform;

    bool fSimGainSpread;

    CLHEP::RandFlat    * fFlatRandom;
    CLHEP::RandPoisson * fPoissonRandom;
    void AddDarkNoise (std::vector<double> &RawWF,double gain);
    void AddWaveform(optdata::TimeSlice_t time, 
		     std::vector<double>& OldPulse, 
		     std::vector<double>& NewPulse, 
		     double factor, 
		     bool extend=false);
    optdata::ChannelData ApplyDigitization (std::vector<double> const RawWF,
					    optdata::Channel_t const ch) const;
    art::ServiceHandle<OpDigiProperties> fOpDigiProperties;
    art::ServiceHandle<geo::Geometry> fGeom;

  };
} // namespace opdet

namespace opdet{

  DEFINE_ART_MODULE(OptDetDigitizer)

} //end namespace opdet

namespace opdet {

  OptDetDigitizer::OptDetDigitizer(fhicl::ParameterSet const& pset)
  {
    // Infrastructure piece
    produces<std::vector< optdata::ChannelDataGroup> >();

    optdata::ChannelDataGroup dg;
    // Input Module and histogram parameters come from .fcl
    fInputModule   = pset.get<std::string>("InputModule");
    fSimGainSpread = pset.get<bool       >("SimGainSpread");
    fTimeBegin  = fOpDigiProperties->TimeBegin();
    fTimeEnd    = fOpDigiProperties->TimeEnd();
    fSampleFreq = fOpDigiProperties->SampleFreq();
    fQE         = fOpDigiProperties->QE();
    fDarkRate   = fOpDigiProperties->DarkRate();
    fPedFlucAmp = fOpDigiProperties->PedFlucAmp();
    fPedFlucRate= fOpDigiProperties->PedFlucRate();
    fSaturationScale = fOpDigiProperties->SaturationScale();
    fPedMeanArray = fOpDigiProperties->PedMeanArray();

    // Initialize toy waveform vector fSinglePEWaveform
    fSinglePEWaveform = fOpDigiProperties->SinglePEWaveform();
    
    unsigned int seed = pset.get< unsigned int >("Seed", sim::GetRandomNumberSeed());
    createEngine(seed);

    // Sample a random fraction of detected photons 
    art::ServiceHandle<art::RandomNumberGenerator> rng;
    CLHEP::HepRandomEngine &engine = rng->getEngine();
    fFlatRandom       = new CLHEP::RandFlat(engine);
    fPoissonRandom    = new CLHEP::RandPoisson(rng->getEngine());
    
  }
  
  //-------------------------------------------------


  void OptDetDigitizer::beginJob()
  {
  }


  //-------------------------------------------------
  
  OptDetDigitizer::~OptDetDigitizer() 
  {
  }
  

  //-------------------------------------------------


  void OptDetDigitizer::AddWaveform (optdata::TimeSlice_t time, 
				     std::vector<double> &OldPulse,
				     std::vector<double> &NewPulse,
				     double factor,
				     bool extend)
  {
    if( (time+NewPulse.size()) > OldPulse.size() && extend )
      OldPulse.resize(time + NewPulse.size());
    for(size_t i = 0; i<NewPulse.size() && (time+i)<OldPulse.size(); ++i)
      OldPulse[time+i] += NewPulse[i] * factor;
  }

  //-------------------------------------------------

  void OptDetDigitizer::AddDarkNoise(std::vector<double> &RawWF, double gain){
    // Add dark noise
    double MeanDarkPulses = fDarkRate * (fTimeEnd-fTimeBegin) / 1000000;
    
    unsigned int NumberOfPulses = fPoissonRandom->fire(MeanDarkPulses);
    for(size_t i=0; i!=NumberOfPulses; ++i)
      {
	double PulseTime_ns = fTimeBegin*1000 + (fTimeEnd-fTimeBegin)*1000*(fFlatRandom->fire(1.0)); // Should be in ns
	optdata::TimeSlice_t PulseTime_ts = fOpDigiProperties->GetTimeSlice(PulseTime_ns);
	AddWaveform( PulseTime_ts,
		     RawWF, 
		     fSinglePEWaveform, 
		     gain);
      }

  }

  optdata::ChannelData OptDetDigitizer::ApplyDigitization(std::vector<double> const rawWF,
							  optdata::Channel_t const ch ) const
  {
    //
    // Digitization includes...
    //     (a) amplitude digitization
    //     (b) saturation
    //     (c) pedestal fluctuation
    //

    // prepare return data container
    optdata::ChannelData chData(ch);
    chData.reserve(rawWF.size());
    optdata::ADC_Count_t baseMean(fPedMeanArray.at(ch));
    for(optdata::TimeSlice_t time=0; time<rawWF.size(); ++time)
      {
	double thisSample = rawWF[time];

	optdata::ADC_Count_t thisCount = (optdata::ADC_Count_t)(thisSample)+baseMean;
	
	// (a) amplitude digitization
	if(CLHEP::RandFlat::shoot(1.0) < (thisSample - int(thisSample)))
	  thisCount+=1;

	// (b) saturation
	if(thisCount > fSaturationScale) thisCount = fSaturationScale;

	chData.push_back(thisCount);
      }

    // (c) pedestal fluctuation
    double timeSpan = chData.size() * 1.e-6/(fOpDigiProperties->SampleFreq());
    unsigned int nFluc = CLHEP::RandPoisson::shoot(fPedFlucRate * timeSpan);
    for(size_t i=0; i<nFluc; ++i)
      {
	optdata::TimeSlice_t pulseTime(CLHEP::RandFlat::shoot(0.0,(double)(chData.size())));
	optdata::ADC_Count_t amp = chData[pulseTime];
	if( CLHEP::RandFlat::shoot(0.,1.) > 0.5)
	  {
	    amp += fPedFlucAmp;
	    if(amp > fSaturationScale) amp=fSaturationScale;
	  }
	else
	  amp -= fPedFlucAmp;
	chData[pulseTime] = amp;
      }

    return chData;
  }

  //-------------------------------------------------

  void OptDetDigitizer::produce(art::Event& evt)
  {

    //
    // Event-wise initialization
    //

    // Infrastructure piece
    std::unique_ptr< std::vector<optdata::ChannelDataGroup > > StoragePtr (new std::vector<optdata::ChannelDataGroup>);
    
    // Read in the Sim Photons
    sim::SimPhotonsCollection ThePhotCollection = sim::SimListUtils::GetSimPhotonsCollection(evt,fInputModule);

    // Convert units into ns from us/MHz
    double timeBegin_ns  = fTimeBegin  *  1000;
    double timeEnd_ns    = fTimeEnd    *  1000;
    double sampleFreq_ns = fSampleFreq /  1000;
    
    // Compute # of timeslices to be stored in the output. This is defined by a user input (fcl file)
    optdata::TimeSlice_t timeSliceWindow(fOpDigiProperties->GetTimeSlice(timeEnd_ns));
    
    /*
      Create output data product, optdata::ChannelDataGroup for each gain channel.
      Note : Although the frame + sample number in DATA should have a reference of T=0 @ DAQ start time, this is
             not handled in MC. Hence we do not set them here (use constructor default)
    */
    optdata::ChannelDataGroup rawWFGroup_HighGain(optdata::kHighGain);
    optdata::ChannelDataGroup rawWFGroup_LowGain(optdata::kLowGain);
    // Reserve entries equal to # of channels
    rawWFGroup_HighGain.reserve(fGeom->NOpChannels());
    rawWFGroup_LowGain.reserve(fGeom->NOpChannels());

    /*
      Define "raw" waveform container which will be filled based on G4 photon timing + SPE waveform information.
      Note this is not completely an analog waveform because it is digitized in terms of time (as it is using std::vector).
    */
    std::vector<std::vector<double> > rawWF_HighGain(fGeom->NOpChannels(),std::vector<double>(timeSliceWindow,0.0));
    std::vector<std::vector<double> > rawWF_LowGain(fGeom->NOpChannels(),std::vector<double>(timeSliceWindow,0.0));

    /*
      Start data processing ... see following steps
      (1) Loop over input array of optical photons & fill "raw" waveform container w/ corresponding SPE waveform
      (2) Loop over filled "raw" waveform and process (digitization, adding noise, baseline spread, etc)
    */

    //
    // Step (1) ... loop over G4 optical photons
    //

    // For every OpDet, convert PE into waveform and combine all together
    for(sim::SimPhotonsCollection::const_iterator itOpDet=ThePhotCollection.begin(); itOpDet!=ThePhotCollection.end(); itOpDet++)
      {
	const sim::SimPhotons& ThePhot=itOpDet->second;

	int ch = ThePhot.OpChannel();
	// For every photon in the hit:
	for(const sim::OnePhoton& Phot: ThePhot)
	  {
	    // Sample a random subset according to QE
	    if(fFlatRandom->fire(1.0)<=fQE)
	      {
		optdata::TimeSlice_t PhotonTime(fOpDigiProperties->GetTimeSlice(Phot.Time));
		if( Phot.Time > timeBegin_ns  &&  Phot.Time < timeEnd_ns )
		  {
		    if(fSimGainSpread)
		      {
			AddWaveform( PhotonTime, rawWF_HighGain[ch], fSinglePEWaveform, fOpDigiProperties->HighGain(ch));
			AddWaveform( PhotonTime, rawWF_LowGain[ch], fSinglePEWaveform, fOpDigiProperties->LowGain(ch));
		      }
		    else
		      {
			AddWaveform( PhotonTime, rawWF_HighGain[ch], fSinglePEWaveform, fOpDigiProperties->HighGainMean(ch));
			AddWaveform( PhotonTime, rawWF_LowGain[ch], fSinglePEWaveform, fOpDigiProperties->LowGainMean(ch));
		      }
		  }
	      } // random QE cut	    
	  } // for each Photon in SimPhotons
      }

    //
    // Loop over "raw" waveform (channel-wise)
    //
    for(unsigned short iCh = 0; iCh < rawWF_LowGain.size(); ++iCh){
      rawWF_LowGain[iCh].resize((timeEnd_ns - timeBegin_ns) * sampleFreq_ns);
      rawWF_HighGain[iCh].resize((timeEnd_ns - timeBegin_ns) * sampleFreq_ns);

      // Add dark noise
      if(fSimGainSpread){
	AddDarkNoise(rawWF_LowGain[iCh],fOpDigiProperties->LowGain(iCh));
	AddDarkNoise(rawWF_HighGain[iCh],fOpDigiProperties->HighGain(iCh));
      }else{
	AddDarkNoise(rawWF_LowGain[iCh],fOpDigiProperties->LowGainMean(iCh));
	AddDarkNoise(rawWF_HighGain[iCh],fOpDigiProperties->HighGainMean(iCh));
      }
      
      // Apply digitization and make channel data
      optdata::ChannelData chData_HighGain(ApplyDigitization(rawWF_HighGain[iCh],iCh));      
      optdata::ChannelData chData_LowGain(ApplyDigitization(rawWF_LowGain[iCh],iCh));

      rawWFGroup_HighGain.push_back(chData_HighGain);
      rawWFGroup_LowGain.push_back(chData_LowGain);
    } // for each OpDet in SimPhotonsCollection

    StoragePtr->push_back(rawWFGroup_HighGain);
    StoragePtr->push_back(rawWFGroup_LowGain);

    evt.put(std::move(StoragePtr));
  }  
}

