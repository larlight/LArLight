////////////////////////////////////////////////////////////////////////
//
//  \file OpDigiProperties_service.cc
//
////////////////////////////////////////////////////////////////////////

// LArSoft includes
#include "OpticalDetector/OpDigiProperties.h"
#include "OpticalDetectorData/OpticalTypes.h"
#include "Geometry/Geometry.h"

// Framework includes
#include "messagefacility/MessageLogger/MessageLogger.h" 

// CLHEP includes
#include "CLHEP/Random/RandGaussQ.h"

// C++ includes
#include <fstream>

namespace opdet{

  //--------------------------------------------------------------------
  OpDigiProperties::OpDigiProperties(fhicl::ParameterSet const& pset, 
					 art::ActivityRegistry &/*reg*/) 
    : fAnalyticalSPE(0)
  {
    this->reconfigure(pset);
  }

  //--------------------------------------------------------------------
  OpDigiProperties::~OpDigiProperties() 
  {
    
  }


  //--------------------------------------------------------------------
  double OpDigiProperties::GetSPEArea()
  {
    double SPEArea=0;
    for(size_t i=0; i!=fWaveform.size(); ++i)
      SPEArea+=fWaveform.at(i);
    return SPEArea;
  }

  //--------------------------------------------------------------------
  double OpDigiProperties::GetSPEAmplitude()
  {
    double AmpSoFar=0;
    for(size_t i=0; i!=fWaveform.size(); ++i)
      if(fWaveform.at(i)>AmpSoFar) AmpSoFar=fWaveform.at(i);
    return AmpSoFar;
  }

  //--------------------------------------------------------------------
  double OpDigiProperties::GetSPECumulativeArea()
  {
    double Cumulative=0, SPEArea=0;
    for(size_t i=0; i!=fWaveform.size(); ++i)
      {
	Cumulative += fWaveform.at(i);
	SPEArea    += Cumulative;
      } 
    return SPEArea;
  }

  //--------------------------------------------------------------------
  double OpDigiProperties::GetSPECumulativeAmplitude()
  {
    double AmpSoFar=0, Cumulative=0;
    for(size_t i=0; i!=fWaveform.size(); ++i)
      {
	Cumulative += fWaveform.at(i);
	if(Cumulative>AmpSoFar) AmpSoFar=Cumulative;
      } 
    return AmpSoFar;
  }
  
  
  //--------------------------------------------------------------------

  double OpDigiProperties::LowGainMean(optdata::Channel_t ch) const
  {
    return fLowGainArray[ch];
  }

  //--------------------------------------------------------------------
  double OpDigiProperties::HighGainMean(optdata::Channel_t ch) const
  {
    return fHighGainArray[ch];
  }
  //--------------------------------------------------------------------
  double OpDigiProperties::LowGain(optdata::Channel_t ch) const
  {
    return CLHEP::RandGauss::shoot(fLowGainArray[ch],fGainSpreadArray[ch]*fLowGainArray[ch]);
  }
  //--------------------------------------------------------------------
  double OpDigiProperties::HighGain(optdata::Channel_t ch) const
  {
    return CLHEP::RandGauss::shoot(fHighGainArray[ch],fGainSpreadArray[ch]*fHighGainArray[ch]);
  }
  //--------------------------------------------------------------------
  optdata::TimeSlice_t OpDigiProperties::GetTimeSlice(double time_ns) 
  {
    if( time_ns/1.e3 > (fTimeEnd-fTimeBegin)) return std::numeric_limits<optdata::TimeSlice_t>::max();

    else return optdata::TimeSlice_t((time_ns/1.e3-fTimeBegin)*fSampleFreq);
  }


  //--------------------------------------------------------------------
  void OpDigiProperties::reconfigure(fhicl::ParameterSet const& p)
  {
    fSampleFreq        = p.get< double       >("SampleFreq"       );
    fTimeBegin         = p.get< double       >("TimeBegin"        );
    fTimeEnd           = p.get< double 	     >("TimeEnd"          );
    fWaveformFile      = p.get< std::string  >("WaveformFile"     );
    fPERescale         = p.get< double       >("PERescale"        );

    // PMT properties
    fQE              = p.get<double>("QE");
    fDarkRate        = p.get<double>("DarkRate");
    fSaturationScale = p.get<optdata::ADC_Count_t>("SaturationScale");

    // Shaper properties
    fUseEmpiricalGain= p.get<bool        >("UseEmpiricalGain");
    fHighGainFile    = p.get<std::string >("HighGainFile");
    fLowGainFile     = p.get<std::string >("LowGainFile");
    fGainSpreadFile  = p.get<std::string >("GainSpreadFile");    

    fHighGainMean       = p.get<double   >("HighGainMean");
    fLowGainMean        = p.get<double   >("LowGainMean");
    fGainSpread          = p.get<double   >("GainSpread");
    fGainSpread_PMT2PMT  = p.get<double   >("GainSpread_PMT2PMT");

    // Digitization ped fluc
    fPedFlucRate     = p.get<double>("PedFlucRate");
    fPedFlucAmp      = p.get<optdata::ADC_Count_t>("PedFlucAmp");
    fADCBaseline     = p.get<optdata::ADC_Count_t>("ADCBaseline");
    fADCBaseSpread   = p.get<double>("ADCBaseSpread");

    // WF related stuff
    fUseEmpiricalShape = p.get< bool         >("UseEmpiricalShape");
    fWFLength          = p.get< double       >("WFLength"         );

    fWaveformFile      = p.get< std::string  >("WaveformFile"     );
    // Option 2: WF from analytical function
    fWFPowerFactor     = p.get< double       >("WFPowerFactor"    );
    fWFTimeConstant    = p.get< double       >("WFTimeConstant"   );
    fVoltageAmpForSPE  = p.get< double       >("VoltageAmpForSPE" );

    // Generate the SPE waveform (i.e. fWaveform)
    GenerateWaveform();

    // Fill gain array
    FillGainArray();

    // Fill baseline mean
    FillPedMeanArray();

    // Report 
    std::string msg(Form("%-10s ... %-10s ... %-10s ... %-10s\n","Ch. Number","Pedestal","High Gain","Low Gain"));
    for(unsigned int i=0;i<fGeometry->NOpChannels();++i) {
      msg+=Form("%-10d ... %-10d ... %-10g ... %-10g\n",i,fPedMeanArray[i],fHighGainArray[i],fLowGainArray[i]);
    }
    mf::LogInfo(__FUNCTION__)<<msg.c_str();
      
    return;
  }

  //
  // As far as Kazu is concerned, this function is deprecated.
  // Any comment? --Kazu 08/05/2013
  //
  std::vector<double> OpDigiProperties::WaveformInit(std::string fWaveformFile)
  {
    // Read in waveform vector from text file
    ifstream WaveformFile (fWaveformFile.c_str());
    std::string line;

    mf::LogInfo("OpDigiProperties")<<"OpDigiProperties opening OpDet waveform at " << fWaveformFile.c_str();

    // Read in each line and place into waveform vector 
    std::vector<double> PEWaveform;
    if (WaveformFile.is_open())
      {
	while ( WaveformFile.good() )
	  {
	    getline (WaveformFile, line);
	    PEWaveform.push_back( fPERescale * strtod( line.c_str(), NULL ) );
	  }
      }
    else throw cet::exception("OpDigiProperties") << "No Waveform File: Unable to open file\n";

    WaveformFile.close();
    return(PEWaveform);
  }

  // Fill the array of pedestal mean
  void OpDigiProperties::FillPedMeanArray(){
    for(unsigned int i=0;i<fGeometry->NOpChannels();++i)
      fPedMeanArray.push_back((optdata::ADC_Count_t)(CLHEP::RandGauss::shoot((double)fADCBaseline,fADCBaseSpread)+0.5));
  }

  // Fill arrays (std::vector<double>) for PMT gain mean & spread information.
  void OpDigiProperties::FillGainArray(){
    if(fUseEmpiricalGain)
      {
	// Fill fron user's text files.
	mf::LogWarning("OpDigiProperties")<<"Using empirical table of gain for each PMT...";
	std::string FullPath;
	cet::search_path sp("FW_SEARCH_PATH");

	if( !sp.find_file(fHighGainFile, FullPath) )
	  throw cet::exception("OpDigiProperties") << "Unable to find high gain spread file in " << sp.to_string() << "\n";

	mf::LogWarning("OpDigiProperties")<<"OpDigiProperties opening high gain spread file at " << FullPath.c_str();
	ifstream HighGainFile(FullPath.c_str());
	if(HighGainFile.is_open()) {
	  std::string line;
	  while ( HighGainFile.good() ){
	    getline(HighGainFile, line);
	    fHighGainArray.push_back(strtod(line.c_str(),NULL));
	  }
	}else throw cet::exception("OpDigiProperties")<<"Unable to open file!\n";

	FullPath="";
	if( !sp.find_file(fLowGainFile, FullPath) )
	  throw cet::exception("OpDigiProperties") << "Unable to find low gain spread file in " << sp.to_string() << "\n";

	mf::LogWarning("OpDigiProperties")<<"OpDigiProperties opening low gain spread file at " << FullPath.c_str();	
	ifstream LowGainFile(FullPath.c_str());
	if(LowGainFile.is_open()) {
	  std::string line;
	  while ( LowGainFile.good() ){
	    getline(LowGainFile, line);
	    fLowGainArray.push_back(strtod(line.c_str(),NULL));
	  }
	}else throw cet::exception("OpDigiProperties")<<"Unable to open file!\n";

	FullPath="";
	if( !sp.find_file(fGainSpreadFile, FullPath) )
	  throw cet::exception("OpDigiProperties") << "Unable to find low gain spread file in " << sp.to_string() << "\n";

	mf::LogWarning("OpDigiProperties")<<"OpDigiProperties opening low gain spread file at " << FullPath.c_str();	
	ifstream GainSpreadFile(FullPath.c_str());
	if(GainSpreadFile.is_open()) {
	  std::string line;
	  while ( GainSpreadFile.good() ){
	    getline(GainSpreadFile, line);
	    fGainSpreadArray.push_back(strtod(line.c_str(),NULL));
	  }
	}else throw cet::exception("OpDigiProperties")<<"Unable to open file!\n";
	
      }
    else{
      // Generate a set of gake gain mean & spread.
      std::string txt;
      txt+=     "    Generating gain for each pmt.\n";
      txt+=Form("        High gain mean: %g ADC/p.e.\n", fHighGainMean);
      txt+=Form("        Low  gain mean: %g ADC/p.e.\n", fLowGainMean);
      txt+=Form("        PMT-to-PMT gain spread : %g \n", fGainSpread_PMT2PMT);
      txt+=Form("        Intrinsic gain spread  : %g \n", fGainSpread);
      mf::LogWarning("OpDigiProperties")<<txt.c_str();
      for(unsigned int i=0;i<fGeometry->NOpChannels();++i) {
	fLowGainArray.push_back(CLHEP::RandGauss::shoot(fLowGainMean,fLowGainMean*fGainSpread_PMT2PMT));
	fHighGainArray.push_back(CLHEP::RandGauss::shoot(fHighGainMean,fHighGainMean*fGainSpread_PMT2PMT));
	fGainSpreadArray.push_back(fGainSpread);
      }
    }

    //
    // Note:
    // Check for # entries. These exceptions ensures we have enough # of elements.
    // When a user access the elements by a channel number using LowGainMean(Channel_t ch) etc., 
    // those functions do not check if the given channel number is valid or not to keep a high
    // performance of the code. If there's an invalid memory access error in run-time, then
    // it must mean the user provided an invalid channel number and not due to insufficient
    // vector elements filled in this function.
    //
    if(fLowGainArray.size()<fGeometry->NOpChannels()) 
      throw cet::exception("OpDigiProperties")<<"Low gain missing for some channels!\n";
    if(fHighGainArray.size()<fGeometry->NOpChannels()) 
      throw cet::exception("OpDigiProperties")<<"High gain missing for some channels!\n";
    if(fGainSpreadArray.size()<fGeometry->NOpChannels()) 
      throw cet::exception("OpDigiProperties")<<"Gain spread missing for some channels!\n";
  }

  void OpDigiProperties::GenerateWaveform()
  {
    std::string FullPath;

    if(fUseEmpiricalShape){
      cet::search_path sp("FW_SEARCH_PATH");    
      if( !sp.find_file(fWaveformFile, FullPath) )
	
	throw cet::exception("OpDigiProperties") << "Unable to find PMT waveform file in " << sp.to_string() << "\n";
      
      fWaveform   = GenEmpiricalWF(FullPath); 
      
    }else fWaveform = GenAnalyticalWF();
  }

  std::vector<double> OpDigiProperties::GenEmpiricalWF(std::string fWaveformFile)
  {
    // Read in waveform vector from text file
    ifstream WaveformFile (fWaveformFile.c_str());
    std::string line;

    mf::LogWarning("OpDigiProperties")<<"OpDigiProperties opening OpDet waveform at " << fWaveformFile.c_str();

    // Read in each line and place into waveform vector 
    std::vector<double> PEWaveform;
    if (WaveformFile.is_open())
      {
	double MaxAmp=0;
	int    NSample=0;
	while ( WaveformFile.good() && NSample<int(fWFLength*fSampleFreq))
	  {
	    getline (WaveformFile, line);
	    double Amp=strtod(line.c_str(),NULL);
	    PEWaveform.push_back(Amp);
	    if(Amp>MaxAmp) MaxAmp=Amp;
	    NSample++;
	  }
	// rescale
	if(MaxAmp<=0) throw cet::exception("OpDigiProperties_module")<<"Waveform amplitude <=0!\n";
	for(unsigned short i=0; i<PEWaveform.size(); i++){ PEWaveform[i]=PEWaveform[i]/MaxAmp; }
      }
    else throw cet::exception("No Waveform File") << "Unable to open file\n";

    WaveformFile.close();
    return(PEWaveform);
  }

  std::vector<double> OpDigiProperties::GenAnalyticalWF(){
    mf::LogWarning("OpDigiProperties")<<"    OpDigiProperties using analytical function for WF generation.";
    //
    // Generate waveform from analytical form
    //
    if(!fAnalyticalSPE) {// Create analytical function if not yet created
      fAnalyticalSPE = new TF1("_analyticalSPE",
			       "10^(22)*x^[1]*[0]*exp(-x/[2])/TMath::Factorial([1])",
			       0,fWFLength*2);             // x is time in micro-seconds
      fAnalyticalSPE->SetParameter(0,fVoltageAmpForSPE);             // voltage amplitude for s.p.e.
      fAnalyticalSPE->SetParameter(1,fWFPowerFactor);                // power factor (no unit)
      fAnalyticalSPE->SetParameter(2,fWFTimeConstant);               // time constant in us
    }
    //
    // Define a waveform vector
    //
    // Size of WF = time width [us] * frequency [MHz] 
    std::vector<double> PEWaveform(int( fWFLength * fSampleFreq), 0.0);
    double SamplingDuration = 1./fSampleFreq; // in micro seconds
    double MaxAmp=0;
    for(unsigned short i = 0; i<PEWaveform.size(); ++i){
      double Value=fAnalyticalSPE->Integral(   i   * SamplingDuration,
					      (i+1) * SamplingDuration) / SamplingDuration;
      PEWaveform[i]=Value;
      if(PEWaveform[i]>MaxAmp) MaxAmp=PEWaveform[i];
    }
    
    // rescale
    if(MaxAmp<=0) throw cet::exception("OpDigiProperties_module")<<"Waveform amplitude <=0!\n";
    for(unsigned short i=0; i<PEWaveform.size(); i++) {
      PEWaveform[i]=PEWaveform[i]/MaxAmp;
      if(PEWaveform[i]<1.e-4) PEWaveform[i]=0;
    }

    return PEWaveform;
  }

} // namespace

namespace opdet{
 
  DEFINE_ART_SERVICE(OpDigiProperties)

} // namespace util
