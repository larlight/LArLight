#ifndef DAVIDANA_CC
#define DAVIDANA_CC

#include "DavidAna.hh"

namespace larlight {

  bool DavidAna::initialize() {

    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you need, you have an output root file pointer "_fout".

    failure =       new TH1D("failure",     "channel number",    9000, 0, 9000);
    event_num = 0;
    //Number of samples to use for baseline+variance calculation
    _NSamples = 10;
    _VarCut = 10;
    return true;
  }
  
  bool DavidAna::analyze(storage_manager* storage) {
  
    //
    // Do your event-by-event analysis here. This function is called for 
    // each event in the loop. You have "storage" pointer which contains 
    // event-wise data. For a reference of pmt_wf_collection class instance, 
    // see the class index in the documentation.

    //Count events (since variable seems set to 0)
    event_num += 1;
    
    const event_fifo *event_wf = (event_fifo*)(storage->get_data(DATA::TPCFIFO));
    //For some reason all event numbers = 0?
    //    tpc_evno->Fill(event_wf->event_number());
    
    //waveform counter
    int wfnum = 0;
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
      
      //***************************
      //Start Compression Algorithm
      //***************************
      
      //Here allow for selection of 1 channel
      if ( (tpc_data->channel_number())%10==0 ){
	
	//variable for interesting events
	bool interesting=false;

	//create temporary histogram
	char c[25];
	char d[25];
	sprintf(c,"fff_ev_%d_wf_%d",event_num,tpc_data->channel_number());
	TH1D* temphist = new TH1D(c,"pulse",tpc_data->size(),0,tpc_data->size());
	sprintf(d,"ggg_ev_%d_wf_%d",event_num,tpc_data->channel_number());
	TH1D* newpulse = new TH1D(d,"newpulse",tpc_data->size()-_NSamples/2,0,tpc_data->size()-_NSamples/2);
	
	//short vector containing last few samples
	std::deque<int> last_few;
	//set variable for baseline
	double baseline_tot = 0;
	//initially just set baseline to first point
	double baseline = tpc_data->at(0);
	//initially se variance to 0
	double variance = 0;

	//Get Waveform
	for (Int_t adc_index=0; adc_index<tpc_data->size(); adc_index++){
	  
	  int adcs = tpc_data->at(adc_index);
	  
	  //interesting?
	  if ( (adcs-baseline) > 10)
	    interesting = true;

	  //IF before _NSamples don't do anything
	  if ( adc_index < _NSamples)
	    {
	      last_few.push_back(adcs);
	      baseline_tot += adcs;
	      newpulse->SetBinContent(adc_index+1,adcs);
	    }
	  else
	    {
	      baseline_tot -= last_few.at(0);
	      last_few.pop_front();
	      baseline_tot += adcs;
	      last_few.push_back(adcs);
	      baseline = baseline_tot/_NSamples;
	      
	      //reset variance
	      variance = 0;
	      //calculate variance:
	      for (int i=0; i<last_few.size(); i++)
		variance += pow((last_few.at(i)-baseline),2);
	      //Apply filter
	      //currently setting bins that don't pass cut to previous value
	      if (variance>_VarCut)
		newpulse->SetBinContent(adc_index+1-_NSamples/2.0,tpc_data->at(adc_index-_NSamples/2));
	      else{
		//		std::cout << "Index:     " << adc_index+1 << std::endl;
		//		std::cout << "Writing @: " << adc_index+1-_NSamples/2 << std::endl;
		//		std::cout << "Value:     " << newpulse->GetBinContent(adc_index-_NSamples/2) << std::endl << std::endl;
		newpulse->SetBinContent(adc_index+1-_NSamples/2.0,newpulse->GetBinContent(adc_index-_NSamples/2));
	      }
	      
	    }

	  temphist->SetBinContent(adc_index+1,adcs);

	}
	
	if (interesting){
	  temphist->Write();
	  newpulse->Write();
	}
	
      }//End IF correct channel #
      
    }//End for each waveform
    
    return true;
    
  }//End Analyze Loop
  
  
  bool DavidAna::finalize() {
    
    // This function is called at the end of event loop.
    // Do all variable finalization you wish to do here.
    // If you need, you can store your ROOT class instance in the output
    // file. You have an access to the output file through "_fout" pointer.

    failure->Write();
  
    return true;
  }
}
#endif
