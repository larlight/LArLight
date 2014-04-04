#ifndef NOISEANA_CC
#define NOISEANA_CC

#include "NoiseAna.hh"

namespace larlight {

  bool NoiseAna::initialize() {

    _ChMin = 0;
    _ChMax = 2000;
    //Bin Time - 2MHz for TPC data
    _BinTime = 5*(1.0e-7); //500 ns time-ticks
    //RMS histogram per channel
    //for (int u=0; u<(_ChMax-_ChMin); u++)
    //  RMSperChan[u] = new TH1D(Form("RMS_Ch_%d",u),Form("Noise Ch %d",u), 1000, 0, 100);
    for (int j=0; j<20000; j++){
      for (int k=0; k<100; k++){
	ChanRMS[j][k] = -1;
	ChanBaseline[j][k] = -1;
      }
    }
    //NoiseSpec: Frequency spectrum of Noise
    //NoiseSpec needs to be scaled so that x-axis is a frequency range (Hz)
    NoiseSpec  = new TH1D("NoiseSpec",   "Noise Freq. Spectrum; w [Hz]",     100,      0,   (2*3.14)/_BinTime);
    //Channel Numbers
    AllChan    = new TH1D("AllChan",     "All Channel Numbers",     (_ChMax-_ChMin), _ChMin, _ChMax);
    //RMS values for all waveforms for all events
    AllRMS     = new TH1D("AllRMS",      "All RMS values; RMS [ADCs]",             1000,      0,    5);
    //Avg over all events of RMS noise per channel
    ChannelRMS = new TH1D("ChannelRMS", "RMS values; Channel; RMS", (_ChMax-_ChMin),  _ChMin, _ChMax);
    //Mean of Baseline values -- channel by channel
    BaseMeanHisto     = new TH1D("BaselineMean", "Mean of Baseline values by Channel", 3000, 0, 3000);
    //RMS of Baseline values -- channel by channel
    BaseRMSHisto     = new TH1D("BaselineRMS", "RMS of Baseline values by Channel", 1000, 0, 5);
    //BaseTrendHisto: tendency of baseline for channel to increase (+1) or decrease (-1)
    BaseTrendHisto  = new TH1D("BaseTrend", "Baseline Trend", 101, -1, 1);
    //RMSTrendHisto: tendency of RMS noise for channel to increase (+1) or decrease (-1)
    RMSTrendHisto  = new TH1D("RMSTrend", "RMS Trend", 101, -1, 1);
    //Max ADCs vs. channel for 1 user-selected event
    MaxADCsHisto = new TH1D("MaxADCs", "Max ADCs; Chan Num; ADCs", 20000, 0, 20000);

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
      int chan = tpc_data->channel_number();
      int chan_index = (tpc_data->module_address())*100+chan;
      //      std::cout << "Channel Index: " << chan_index << std::endl;
      //AllChan->Fill(chan);

      //Baseline subtracted ADCs
      TH1D *ADC_subtracted = new TH1D("ADC_subtracted", "Baseline subtracted", tpc_data->size(), 0, tpc_data->size());

      //loop over ADCs and find baseline
      int ADCsum = 0;
      for (UInt_t u=0; u<tpc_data->size(); u++)
	ADCsum += tpc_data->at(u);
      double baseline = ADCsum/(tpc_data->size());

      ChanBaseline[chan_index][event_num] = baseline;
      /*
      if (event_num > 0){
	if (ChanBaseline[chan_index][event_num] > ChanBaseline[chan_index][event_num-1])
	  BaselineTrend[chan_index] += 1;
	else
	  BaselineTrend[chan_index] -= 1;
      }
      else   //preset value to 0
	BaselineTrend[chan_index] = 0;
      */
      //Now subtract baseline and find noise RMS
      double RMS = 0;
      for (UInt_t u=0; u<tpc_data->size(); u++){
	RMS += ( sqrt( ((tpc_data->at(u))-baseline) * ((tpc_data->at(u))-baseline) ) );
	ADC_subtracted->SetBinContent( u+1 , (tpc_data->at(u)-baseline) );
      }
      double rmsnoise = RMS/tpc_data->size();
      //      std::cout << "RMS noise: " << rmsnoise << std::endl;
      AllRMS->Fill(rmsnoise);

      //Fill Histo of corresponding channel with RMS
      if ( (chan_index >= _ChMin) && (chan_index < _ChMax) )
	ChanRMS[chan_index][event_num] = rmsnoise;
      else
	std::cout << "Channel out of bounds!   Chan Number: " << chan_index  << std::endl;
      /*
      //study variation over "time"
      if (event_num > 0){
	if (ChanRMS[chan_index][event_num] > ChanRMS[chan_index][event_num-1])
	  RMSTrend[chan_index] += 1;
	else
	  RMSTrend[chan_index] -= 1;
      }
      else   //preset value to 0
	RMSTrend[chan_index] = 0;
      */
      //save waveform if RMS is high
      if ( (rmsnoise>30) ){
	TH1D *waveform =new TH1D(Form("waveform_ev%d_ch%d",event_num,chan_index),Form("waveform_ev%d_ch%d",event_num,chan_index)
				 ,tpc_data->size(), 0, tpc_data->size());
	for (UInt_t k=0; k<tpc_data->size(); k++)
	  waveform->SetBinContent(k+1,tpc_data->at(k));
	waveform->Write();
	}
      
      //Foiurier Transform of noise
      /*
      TH1 *fft = ADC_subtracted->FFT(NULL,"MAG");
      for (UInt_t w=0; w<((tpc_data->size())/2+1); w++)
	NoiseSpec->AddBinContent(w+1,fft->GetBinContent(w));
      */


      delete ADC_subtracted;
      //delete fft;

    }//Loop over all waveforms - end

    //Count events (since variable seems set to 0)
    event_num += 1;
    std::cout << "event number: " << event_num << std::endl;

    return true;
  }

  bool NoiseAna::finalize() {

    /*
    //Choose Channel to plot Baselines for
    int chplot = -1;
    std::cout << "Plot Baseline value for one channel for all events..." << std::endl;
    while (chplot == -1){
      std::cout << "Insert Channel to plot" << std::endl;;
      std::cin >> chplot;
      if ( (chplot<_ChMax) and (chplot>_ChMin) ) {
	if  (ChanBaseline[chplot][99] != -1) 
	  break;
	else { std::cout << "Channel not present in this data set..." << std::endl; chplot=-1; }
      }
      else { std::cout << "Channel out of bounds..." << std::endl; chplot=-1; }
    }
    
    TH1D *chbaseline =new TH1D(Form("BaselineChan_%d",chplot),Form("BaselineChan_%d",chplot)
			     ,100, ChanBaseline[chplot][0]-10, ChanBaseline[chplot][0]+10);
    for (UInt_t k=0; k<100; k++)
      chbaseline->Fill(ChanBaseline[chplot][k]);
    chbaseline->Write();
    */

    //Calculate Baseline Mean and RMS channel-by-channel
    for (int ch=0; ch<(_ChMax-_ChMin); ch++){
      if (ChanBaseline[ch][0]!=-1 and ChanBaseline[ch][99]!=-1){
	double baseavg = 0;
	double baserms = 0;
	for (int evt=0; evt<100; evt++)
	  baseavg += ChanBaseline[ch][evt];
	baseavg = baseavg/100.0;
	for (int evt=0; evt<100; evt++)
	  baserms += (ChanBaseline[ch][evt]-baseavg)*(ChanBaseline[ch][evt]-baseavg);
	baserms = TMath::Sqrt(baserms/100.0);
	BaseMeanHisto->Fill(baseavg);
	BaseRMSHisto->Fill(baserms);
      }
      else
	std::cout << "Baseline Histos Not Written..." << std::endl;
    }

    //Fill ChannelRMS with avg value of RMS for that channel
    for (int u=0; u<(_ChMax-_ChMin); u++){
      //if channel not empty
      if (ChanRMS[u][0] != -1){
	//if all 100 events full
	if (ChanRMS[u][99] != -1){
	  double thisrms = 0;
	  for (int i=0; i<100; i++)
	    thisrms += ChanRMS[u][i];
	  ChannelRMS->SetBinContent(u+1,thisrms/100);
	  BaseTrendHisto->Fill(BaselineTrend[u]/99.0);
	  RMSTrendHisto->Fill(RMSTrend[u]/99.0);
	}//if channel completely full (100 events)
	else {std::cout << "This Chan had < 100 events: " << u << std::endl;}
      }//if channel has at least 1 element
    }//loop over all channel numbers
    
    AllRMS->Write();
    ChannelRMS->Write();
    AllChan->Write();
    NoiseSpec->Write();
    BaseTrendHisto->Write();
    RMSTrendHisto->Write();
    BaseMeanHisto->Write();
    BaseRMSHisto->Write();
    MaxADCsHisto->Write();

    sine->Write();
    SineExmpl->Write();
    rand->Write();
    RandExmpl->Write();

    return true;
  }
}
#endif
