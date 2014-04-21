#ifndef NOISEANA_CC
#define NOISEANA_CC

#include "NoiseAna.hh"

#include <complex>
#include <iostream>
#include <valarray>
#include <math.h>
#include <vector>
 
const double Pi = 3.141592653589793238460;
 
typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;
 
// Cooley–Tukey FFT (in-place)
void fft(CArray& x)
{
  const size_t N = x.size();
  if (N <= 1) return;
 
  // divide
  CArray even = x[std::slice(0, N/2, 2)];
  CArray  odd = x[std::slice(1, N/2, 2)];
 
  // conquer
  fft(even);
  fft(odd);
 
  // combine
  for (size_t k = 0; k < N/2; ++k)
    {
      Complex t = std::polar(1.0, -2 * Pi * k / N) * odd[k];
      x[k    ] = even[k] + t;
      x[k+N/2] = even[k] - t;
    }
}
 
// inverse fft (in-place)
void ifft(CArray& x)
{
  // conjugate the complex numbers
  x = x.apply(std::conj);
 
  // forward fft
  fft( x );
 
  // conjugate the complex numbers again
  x = x.apply(std::conj);
 
  // scale the numbers
  x /= x.size();
}

void transform( std::vector<int>& input, std::vector<double>& output ){

  int ticks = input.size();

  Complex time[ticks];
  for (int i=0; i < ticks; i++)
    time[i] = input.at(i);

  CArray data(time, ticks);
  fft(data);

  //now create new array of Freq. spectrum
  for (int j=0; j < ticks; j++)
    output.at(j) = sqrt( data[j].imag()*data[j].imag() + data[j].real()*data[j].real() );

}


namespace larlight {

  bool NoiseAna::initialize() {

    _ChMin = 0;
    _ChMax = 2000;
    //Bin Time - 2MHz for TPC data
    _BinTime = 5*(1.0e-7); //500 ns time-ticks
    //RMS histogram per channel
    //for (int u=0; u<(_ChMax-_ChMin); u++)
    //  RMSperChan[u] = new TH1D(Form("RMS_Ch_%d",u),Form("Noise Ch %d",u), 1000, 0, 100);

    //Crates Being Used:
    _NCrates = 1;
    _NBoards = 20;
    _NChans  = 64;
    _TotChans = 2000*_NCrates;


    //Clear all maps
    for (short i=0; i<_NCrates; i++){
      for (short j=0; j<_NBoards; j++){
	for (short k=0; k<_NChans; k++){
	  for (short e=0; e<100; e++){
	    RMSNoiseMap[i][j][k][e] = 0;
	    BaselineMap[i][j][k][e] = 0;
	  }
	  ChanEntries[i][j][k] = 0;
	}
      }
    }

    //NoiseSpec needs to be scaled so that x-axis is a frequency range (Hz)
    NoiseSpec  = new TH1D("NoiseSpec",   "Noise Freq. Spectrum; w [Hz]",     10000,      0,   (2*3.14)/_BinTime);    

    //Sine example -- to get proper scaling of x-axis
    SineExmpl  = new TH1D("sineexample", "Sine Example",             3000,      0,  2*3.14);
    sine       = new TH1D("sine", "sine example", 3000, 0, 3000);
    //Random example -- to get proper scaling of x-axis
    RandExmpl  = new TH1D("randexample", "Rand Example",             3000,      0,  2*3.14);
    rand       = new TH1D("rand", "rand example", 3000, 0, 3000);
    //Vector holding RMS for each channel as they come...then average
    // RMSvec.reserve(1185);
    //for (int y=0; y<RMSvec.size() ; y++)
    //  RMSvec[y]=0;


    //Rand Example
    TRandom *R = new TRandom(time(0));
    for (int y=0; y<3000; y++)
      rand->SetBinContent(y+1,R->Uniform(-10,10));
    TH1 *ranexmpl = rand->FFT(NULL,"MAG");
    for (int w=0; w<(3000/2+1); w++)
      RandExmpl->AddBinContent(w+1,ranexmpl->GetBinContent(w));
    delete ranexmpl;
    //Sine Example
    for (int y=0; y<3000; y++)
      sine->SetBinContent(y+1,TMath::Sin(2*TMath::Pi()*y)+TMath::Sin(2*TMath::Pi()*2*y));
    TH1 *ffexmpl = sine->FFT(NULL,"MAG");
    for (int w=0; w<(3000/2+1); w++)
      SineExmpl->AddBinContent(w+1,ffexmpl->GetBinContent(w));
    delete ffexmpl;

    event_num = 0;

    return true;
  }
  
  bool NoiseAna::analyze(storage_manager* storage) {

    const event_tpcfifo *event_wf = (event_tpcfifo*)(storage->get_data(DATA::TPCFIFO));
    

    //Loop over all waveforms - begin
    for (size_t i=0; i<event_wf->size(); i++){
      
      //Check for empty waveforms!
      const tpcfifo* tpc_data = (&(event_wf->at(i)));
      if(tpc_data->size()<1){
	Message::send(MSG::ERROR,__FUNCTION__,
		      Form("Found 0-length waveform: Event %d ... Ch. %d",event_wf->event_number(),tpc_data->channel_number()));
	continue;
      }

      //get channel number
      int chan        = tpc_data->channel_number();
      int mod_address = tpc_data->module_address();
      int mod_id      = tpc_data->module_id();
      int chan_index  = (tpc_data->module_address())*100+chan;
      UInt_t wfsize   = tpc_data->size();

      //std::cout << std::endl << "Chan: " << chan << "\tAddrs: " << mod_address << "\tID: " << mod_id << std::endl;
      ChanEntries[mod_id][mod_address][chan] += 1;

      //Histo for baseline subtracted ADCs
      TH1D *ADC_subtracted = new TH1D("ADC_subtracted", "Baseline subtracted", wfsize, 0, wfsize);
      std::vector<int> input;
      input.reserve(wfsize);
      std::vector<double> output;
      output.reserve(wfsize);

      //loop over ADCs and find baseline
      int ADCsum = 0;
      for (UInt_t u=0; u < wfsize; u++)
	ADCsum += tpc_data->at(u);
      double baseline = ((float)(ADCsum))/wfsize;
      BaselineMap[mod_id][mod_address][chan][event_num] = baseline;
      

      //Now subtract baseline and find noise RMS
      double RMS = 0;
      for (UInt_t u=0; u < wfsize; u++){
	RMS += ((tpc_data->at(u))-baseline) * ((tpc_data->at(u))-baseline) ;
	input.push_back((int)tpc_data->at(u));
	output.push_back(0.0);
	ADC_subtracted->SetBinContent( u+1 , (tpc_data->at(u)-baseline) );
      }
      double rmsnoise = sqrt( RMS/ ( wfsize - 1) );
      RMSNoiseMap[mod_id][mod_address][chan][event_num] = rmsnoise;

      //save waveform if RMS is high
      /*
      if ( (rmsnoise>30) ){
	TH1D *waveform =new TH1D(Form("waveform_ev%d_ch%d",event_num,chan_index),Form("waveform_ev%d_ch%d",event_num,chan_index)
				 ,tpc_data->size(), 0, tpc_data->size());
	for (UInt_t k=0; k<tpc_data->size(); k++)
	  waveform->SetBinContent(k+1,tpc_data->at(k));
	waveform->Write();
	}
      */

      //Foiurier Transform of noise
      /*
      transform(input,output);
      for (UInt_t w=0; w < output.size()/2+1; w++)
	NoiseSpec->AddBinContent(w+1,output.at(w));
      */
      TH1 *fft = ADC_subtracted->FFT(NULL,"MAG");
      for (UInt_t w=0; w<((tpc_data->size())/2+1); w++)
	NoiseSpec->AddBinContent(w+1,fft->GetBinContent(w));
      delete fft;

      delete ADC_subtracted;


    }//Loop over all waveforms - end

    //Count events (since variable seems set to 0)
    event_num += 1;
    std::cout << "event number: " << event_num << std::endl;

    return true;
  }

  bool NoiseAna::finalize() {

    
    //Entries for each Channel
    AllChan         = new TH1D("AllChan",      "Entries in Each Channel; Channel Number; Events",    _TotChans, 0, _TotChans);
    //RMS values for all waveforms for all events
    AllRMS          = new TH1D("AllRMS",       "All RMS values; RMS [ADCs]",                         1000,      0,    1);
    //Avg over all events of Baseline per channel
    ChannelBaseline = new TH1D("ChanBaseline", "Baseline by Channel; Channel; Baseline [ADCs]",      _TotChans, 0, _TotChans);
    //Avg over all events of RMS noise per channel
    ChannelRMS      = new TH1D("ChannelRMS",   "RMS values; Channel; RMS",                           _TotChans, 0, _TotChans);
    //Mean of Baseline values -- channel by channel
    BaseMeanHisto   = new TH1D("BaselineMean", "Mean of Baselines; [ADCs]",                          3000, 0, 3000);
    //RMS of Baseline values -- channel by channel
    BaseRMSHisto    = new TH1D("BaselineRMS",  "RMS of Baseline values by Channel [ADCs]",           1000, 0, 1);
    //Max ADCs vs. channel for 1 user-selected event
    MaxADCsHisto    = new TH1D("MaxADCs",      "Max ADCs; Chan Num; ADCs",                           20000, 0, 20000);
    //2D Histograms: Maps and Correlation between Baseline Variation and Noise RMS Level
    BoardVsChanRMS  = new TH2D("BoardVsChanRMSMap",          "Board vs. Chan RMS Noise Map; FEM #; Chan #",
			       _NBoards, 0, _NBoards, _NChans, 0, _NChans);
    BoardVsChanBase = new TH2D("BoardVsChanBaselineMap",     "Board vs. Chan Baseline Map; FEM #; Chan #", 
			       _NBoards, 0, _NBoards, _NChans, 0, _NChans);
    BaseRMSvsNoise  = new TH2D("BoardRMSvsNoise",            "Baseline RMS vs Noise per Chan; Noise [ADC]; Baseline RMS",
			       30, 0, 1, 30, 0, 1);



    TStyle *gStyle = new TStyle("Default","Default Style"); 
    gStyle->SetOptStat(0);

    //loop over maps and fill histograms
    for (short crate=0; crate < _NCrates; crate++){
      for (short board=0; board < _NBoards; board++){
	for (short channel=0; channel < _NChans; channel++){
	  int nevents = ChanEntries[crate][board][channel];
	  double avgBaseline = 0;
	  double avgNoise    = 0;
	  double varBaseline = 0;
	  double varNoise    = 0;
	  if ( nevents > 0 ){
	    AllChan->SetBinContent( 1000*crate+100*board+channel+1, nevents);
	    for (int n=0; n < nevents; n++){
	      avgBaseline += BaselineMap[crate][board][channel][n];
	      avgNoise    += RMSNoiseMap[crate][board][channel][n];
	      AllRMS->Fill( RMSNoiseMap[crate][board][channel][n] );
	    }
	    avgBaseline = avgBaseline/(double)nevents;
	    avgNoise    = avgNoise/(double)nevents;
	    ChannelBaseline->SetBinContent( 1000*crate+100*board+channel+1, avgBaseline);
	    ChannelRMS->SetBinContent( 1000*crate+100*board+channel+1, avgNoise);
	    BaseMeanHisto->Fill( avgBaseline );
	    BaseRMSHisto->Fill( avgNoise );
	    BoardVsChanRMS->SetBinContent( board , channel , avgNoise);
	    BoardVsChanBase->SetBinContent( board, channel, avgBaseline);
	    for (int n=0; n < nevents; n++){
	      varBaseline += (BaselineMap[crate][board][channel][n]-avgBaseline)*
		(BaselineMap[crate][board][channel][n]-avgBaseline);
	      varNoise    += (RMSNoiseMap[crate][board][channel][n]-avgNoise)*
		(RMSNoiseMap[crate][board][channel][n]-avgNoise);
	    }
	    varBaseline = sqrt( varBaseline );
	    varNoise    = sqrt( varNoise );
	    ChannelRMS->SetBinError( 1000*crate+100*board+channel+1, varNoise);
	    ChannelBaseline->SetBinError( 1000*crate+100*board+channel+1, varBaseline);
	    BaseRMSvsNoise->Fill(avgNoise, varBaseline);
	  }
	}
      }
    }

    AllChan->Write();
    ChannelBaseline->Write();
    ChannelRMS->Write();
    BaseRMSvsNoise->Write();
    BoardVsChanRMS->Write();
    BoardVsChanBase->Write();
    AllRMS->Write();
    ChannelRMS->Write();
    AllChan->Write();
    //NoiseSpec->Write();
    BaseMeanHisto->Write();
    BaseRMSHisto->Write();
    //sine->Write();
    //SineExmpl->Write();
    //rand->Write();
    //RandExmpl->Write();

    return true;
  }
}
#endif
