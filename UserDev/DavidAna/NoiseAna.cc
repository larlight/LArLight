#ifndef NOISEANA_CC
#define NOISEANA_CC

#include "NoiseAna.hh"

namespace larlight {

  bool NoiseAna::initialize() {

    _ChMin = 1;
    _ChMax = 11885;
    //Bin Time - 2MHz for TPC data
    _BinTime = 500*1.0e-9; //500 ns time-ticks
    //RMS histogra per channel
    for (int u=0; u<(_ChMax-_ChMin); u++)
      RMSperChan[u] = new TH1D(Form("RMS_Ch_%d",u),Form("Noise Ch %d",u), 1000, 0, 100);

    //NoiseSpec needs to be scaled so that x-axis is a frequency range (Hz)
    NoiseSpec  = new TH1D("NoiseSpec",   "Noise Freq. Spectrum; w [Hz]",     100,      0,   (2*3.14)/_BinTime);
    AllChan    = new TH1D("AllChan",     "All Channel Numbers",     20000,      0, 20000);
    AllRMS     = new TH1D("AllRMS",      "All RMS values; RMS [ADCs]",             30,      0,    10);
    ChannelRMS = new TH1D("Channel RMS", "RMS values; Channel; RMS", (_ChMax-_ChMin),  _ChMin, _ChMax);
    SineExmpl  = new TH1D("sineexample", "Sine Example",             3000,      0,  2*3.14);
    sine       = new TH1D("sine", "sine example", 3000, 0, 3000);
    //Vector holding RMS for each channel as they come...then average
    // RMSvec.reserve(1185);
    //for (int y=0; y<RMSvec.size() ; y++)
    //  RMSvec[y]=0;
    /*
    //Sine Example
    for (int y=0; y<3000; y++)
      sine->SetBinContent(y+1,TMath::Sin(y));
    TH1 *ffexmpl = sine->FFT(NULL,"MAG");
    for (int w=0; w<(3000/2+1); w++)
      SineExmpl->AddBinContent(w+1,ffexmpl->GetBinContent(w));
    delete ffexmpl;
    */
    event_num = 0;

    return true;
  }
  
  bool NoiseAna::analyze(storage_manager* storage) {

    //Count events (since variable seems set to 0)
    event_num += 1;
    std::cout << "event number: " << event_num << std::endl;

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
      AllChan->Fill(chan);

      //Make histogram with ADC values - use to find mean
      //max ADC value is 4096 -> afterwards saturation...
      TH1D *ADCs = new TH1D("ADCs", "ADC values", 4096, 0, 4096);
      //Baseline subtracted ADCs
      TH1D *ADC_subtracted = new TH1D("ADC_subtracted", "Baseline subtracted", tpc_data->size(), 0, tpc_data->size());

      //Make histogram for RMS noise
      //use mean to characterize channel noise
      TH1D *RMS  = new TH1D("RMS", "RMS noise", 100, 0, 100);

      //loop over ADCs and insert in histo
      for (UInt_t u=0; u<tpc_data->size(); u++)
	ADCs->Fill(tpc_data->at(u));
      double baseline = ADCs->GetMean();

      //Now subtract baseline and find noise RMS
      for (UInt_t u=0; u<tpc_data->size(); u++){
	RMS->Fill( sqrt( ((tpc_data->at(u))-baseline) * ((tpc_data->at(u))-baseline) ) );
	ADC_subtracted->SetBinContent( u+1 , (tpc_data->at(u)-baseline) );// TMath::Sin(u)+TMath::Sin(2*u)+TMath::Sin(3*u));//
      }
      double rmsnoise = RMS->GetMean();
      //save waveform if RMS is high
      if (event_num == 1){
	TH1D *waveform =new TH1D(Form("waveform_ev%d_ch%d",event_num,chan),Form("waveform_ev%d_ch%d",event_num,chan)
				 ,tpc_data->size(), 0, tpc_data->size());
	for (UInt_t k=0; k<tpc_data->size(); k++)
	  waveform->SetBinContent(k+1,tpc_data->at(k));
	waveform->Write();
	}
      /*
      //Foiurier Transform of noise
      TH1 *fft = ADC_subtracted->FFT(NULL,"MAG");
      for (UInt_t w=0; w<((tpc_data->size())/2+1); w++)
	NoiseSpec->AddBinContent(w+1,fft->GetBinContent(w));
      if (baseline<1000)//temporary...only collection plane noise
	AllRMS->Fill(rmsnoise);
      //Fill Histo of corresponding channel with RMS
      if ( (chan >= _ChMin) && (chan < _ChMax) ) 
	RMSperChan[chan-_ChMin]->Fill(rmsnoise);
      */
      delete ADCs;
      delete RMS;
      delete ADC_subtracted;
      //delete fft;
      //else
	//std::cout << "Ch Num not being considered: " << chan << std::endl;
    }//Loop over all waveforms - end

    return true;
  }

  bool NoiseAna::finalize() {

    //Fill ChannelRMS with avg value of RMS for that channel
    for (int u=0; u<(_ChMax-_ChMin); u++)
      ChannelRMS->SetBinContent(u+1,RMSperChan[u]->GetMean());

    AllRMS->Write();
    ChannelRMS->Write();
    AllChan->Write();
    NoiseSpec->Write();

    sine->Write();
    SineExmpl->Write();

    return true;
  }
}
#endif
