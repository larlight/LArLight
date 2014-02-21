#ifndef DAVIDANA_CC
#define DAVIDANA_CC

#include "DavidAna.hh"

namespace larlight {

  bool DavidAna::initialize() {

    _baseline      =   0;
    event_num      =   0;

    return true;
  }
  
  bool DavidAna::analyze(storage_manager* storage) {
  
    //Count events (since variable seems set to 0)
    event_num += 1;
    bool interesting = false;

    if (event_num == 1){

    const event_fifo *event_wf = (event_fifo*)(storage->get_data(DATA::TPCFIFO));
    
    //waveform counter
    int wfnum = 0;

    int multiplicity = 0;

    //Loop over all waveforms
    for (size_t i=0; i<event_wf->size(); i++){
      
      wfnum += 1;

      //Check for empty waveforms!
      const fifo* tpc_data = (&(event_wf->at(i)));
      if(tpc_data->size()<1){
	Message::send(MSG::ERROR,__FUNCTION__,
		      Form("Found 0-length waveform: Event %d ... Ch. %d",event_wf->event_number(),tpc_data->channel_number()));
	continue;
      }

      //determine collection plane
      //(for now by looking at value of first adc)
      if ( tpc_data->at(0) > 2000 )
	_baseline = 2048;
      else
	_baseline = 400;

      //count how many waveforms from same channel
      if ( i > 0 ){
	if ( (&event_wf->at(i))->channel_number() == (&event_wf->at(i-1))->channel_number() ) {multiplicity += 1; }
	else { multiplicity = 0; }
      }

      //Here allow for selection of 1 channel
      if ( (tpc_data->channel_number())%1==0 ){
	
	//create temporary histogram
	
	char c[25];
	//char d[25];
	sprintf(c,"fff_ev_%d_wf_%d_%d",event_num,tpc_data->channel_number(),multiplicity);
	TH1D* temphist = new TH1D(c,"pulse",tpc_data->size(),0,tpc_data->size());
	//sprintf(d,"ggg_ev_%d_wf_%d_%d",event_num,tpc_data->channel_number(),i);
	//TH1D* newpulse = new TH1D(d,"newpulse",tpc_data->size()-_NSamples/2,0,tpc_data->size()-_NSamples/2);
	
	//Get Waveform
	for (UShort_t adc_index=0; adc_index<tpc_data->size(); adc_index++){
	  
	  int adcs = tpc_data->at(adc_index);
	  
	  //determine if something interesting happens
	  if ( (adcs-_baseline > 4) || (_baseline-adcs > 4) )
	    interesting = true;
	  
	  temphist->SetBinContent(adc_index+1,adcs);

	}
	
	if (interesting)
	  temphist->Write();
	interesting = false;

      }//End IF correct channel #
      
    }//End for each waveform
    
    }

    return true;
    
  }//End Analyze Loop
  
  
  bool DavidAna::finalize() {
    
    return true;
  }
}
#endif
