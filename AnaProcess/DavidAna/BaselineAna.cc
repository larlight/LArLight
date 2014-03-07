#ifndef BASELINEANA_CC
#define BASELINEANA_CC

#include "BaselineAna.hh"

namespace larlight {

  bool BaselineAna::initialize() {

    //Baseline Histo
    _BaseHisto = new TH1D("BaseHisto", "Baseline Histo; ADCs",    4096,       0,    4096);
    //Histo of Channl Nums used
    _ChNumHist = new TH1D("ChanNum",   "Channel Number",        100000,       0,  100000);

    event_num = 0;

    return true;
  }
  
  bool BaselineAna::analyze(storage_manager* storage) {
    
    //Count events (since variable seems set to 0)
    event_num += 1;

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

      //Get Channel number
      _ChNumHist->Fill(tpc_data->channel_number());

      //Make histogram with ADC values - use to find mean
      //max ADC value is 4096 -> afterwards saturation...
      TH1D *ADCs = new TH1D("ADCs", "ADC values", 4096, 0, 4096);
      
      //loop over ADCs and insert in histo
      for (UInt_t u=0; u<tpc_data->size(); u++)
	ADCs->Fill(tpc_data->at(u));

      _BaseHisto->Fill(ADCs->GetMean());
      //std::cout << "Baseline for waveform is: " << ADCs->GetMean() << std::endl;

      //clear hsitogram
      delete ADCs;

    }//Loop over all waveforms - end
  
    return true;
  }

  bool BaselineAna::finalize() {
  
    _BaseHisto->Write();
    _ChNumHist->Write();
  
    return true;
  }
}
#endif
