#ifndef WFCOMPRESS_CC
#define WFCOMPRESS_CC

#include "WFCompress.hh"

namespace larlight {

  bool WFCompress::initialize() {

    //larlight::event_fifo new_event_wf;  //can I just create like this? How does it know all its properties?

    //_merge_tree     =   0;
    _merge_tree     = new TTree("tpcfifo_tree", "");
    _merge_tree->Branch("tpcfifo_branch", new_event_wf.GetName(), (&new_event_wf));
    _merge_tree->Branch("event_num", &_event_num, "_event_num/I");    
    _event_num      =   0;

    //ask for variance cut:
    double varask;
    std::cout << "What variance/bin value to use?" << std::endl;
    std::cin >> varask;
    //Number of samples to use for baseline+variance calculation
    std::cout << "Number of samples to calculate variance from?" << std::endl;
    std::cin >> _NVarSamples;
    std::cout << "Number of samples to write?" << std::endl;
    std::cin >> _NSamples;
    _VarCut        =  _NVarSamples*varask;
    _baseline      =   0;
    std::cout << "The cut on variance is: " << _VarCut << std::endl;

    return true;
  }
  
  bool WFCompress::analyze(storage_manager* storage) {

    _event_num += 1;
    std::cout << "New Event!   ...event number: " << _event_num <<  std::endl;

    //get vector of waveforms
    larlight::event_fifo *event_wf = (event_fifo*)(storage->get_data(DATA::TPCFIFO));
    //create new vector of waveforms to write
    new_event_wf.clear();

    if(!event_wf) {
      print(MSG::ERROR,__FUNCTION__,"Data storage did not find associated waveforms!");
      return false;
    }

    //waveform counter
    int wfnum = 0;
    //Loop over all waveforms
    for (size_t i=0; i<event_wf->size(); i++){

      //get tpc_data
      larlight::fifo* tpc_data = (&(event_wf->at(i)));      

      wfnum += 1;
      //Check for empty waveforms!
      if(tpc_data->size()<1){
	Message::send(MSG::ERROR,__FUNCTION__,
		      Form("Found 0-length waveform: Event %d ... Ch. %d",event_wf->event_number(),tpc_data->channel_number()));
	continue;
      }

      //Apply Compression algorithm
      Compress( tpc_data, &new_event_wf);

    }//loop over all waveofmrs

    if (new_event_wf.size() >= 1)
      {
	_wfnumber = new_event_wf.size();
	//std::cout << "Created New Waveform of size: " << _wfnumber << std::endl;
	_merge_tree->Fill();
      }
    
  
    return true;
  }

  //****************************************************************************************
  void WFCompress::Compress( larlight::fifo* tpc_data, larlight::event_fifo* new_event_wf) {
  //****************************************************************************************
      
      //***************************
      //Start Compression Algorithm
      //***************************

      //determine collection plane
      //(for now by looking at value of first adc)
      if ( tpc_data->at(0) > 2000 )
	_baseline = 2048;
      else
	_baseline = 400;
      
      //Here allow for selection of 1 channel
      if ( (tpc_data->channel_number())%30000!=0 ){
	
	//short vector containing last few samples
	std::deque<int> last_few;
	//set variable for baseline
	double baseline_tot = 0;
	//initially just set baseline to first point
	double baseline     = tpc_data->at(0);
	//initially se variance to 0
	double variance     = 0;
	//interesting?
	bool begin          = false;
	int begin_sample, end_sample;

	//std::cout << "  New Waveform!" << std::endl;

	//Get Waveform
	for (Int_t adc_index=0; adc_index<tpc_data->size(); adc_index++){
	  
	  int adcs = tpc_data->at(adc_index);
	  
	  //IF before _NSamples don't do anything
	  if ( adc_index < _NVarSamples)
	    {
	      last_few.push_back(adcs);
	      baseline_tot += adcs;
	    }
	  else
	    {
	      baseline_tot -= last_few.at(0);
	      last_few.pop_front();
	      baseline_tot += adcs;
	      last_few.push_back(adcs);
	      baseline = baseline_tot/_NVarSamples;
	      //reset variance
	      variance = 0;
	      //calculate variance:
	      for (int i=0; i<last_few.size(); i++)
		variance += pow((last_few.at(i)-baseline),2);

	      //Apply filter
	      //currently setting bins that don't pass cut to previous value

	      //if we pass the cut
	      if ( variance > _VarCut )
		{
		  //and it's the first time
		  if (!begin) { begin_sample = adc_index; }
		  //std::cout << "YES @ BIN:   " << adc_index << std::endl;
		  //set begin to true -- found something interesting
		  begin = true;
		}
	      else
		{
		  //if begin is true then we found the end of the waveform
		  if ( begin )
		    {
		      //std::cout << "NO @ BIN:  " << adc_index << std::endl;
		      //std::cout << "     Compressing!" << std::endl;
		      begin = false;
		      end_sample = adc_index;
		      //can now write waveform and surroundings
		      //DO THAT HERE!
        	      make_new_wf(tpc_data, begin_sample, end_sample, new_event_wf);
		    }
		}
	      
	    }//if not at beginning of wf
	  
	}//for all adc bins
	
      }//possibility of selecting channels



  }
      
  void WFCompress::make_new_wf(larlight::fifo* wf, int start, int stop, larlight::event_fifo* new_event_wf) {

    //create vector where to store ADCs
    std::vector<UShort_t> new_wf_bins;
  
    //make sure not overflowing before or after original waveform
    int start_record = start-_NSamples;
    if (start_record < 0) { start_record = 0; }
    int end_record   = stop + _NSamples;
    if (end_record > wf->size()) { end_record = wf->size(); }
  
    //get interesting samples +- _NSamples
    for (int i=start_record; i<end_record; i++)
      new_wf_bins.push_back(wf->at(i));

    //print compression
    //std::cout << "Compressing WF of size: " << wf->size() << std::endl;
    //std::cout << "Into WF of size:        " << new_wf_bins.size() << std::endl;
    
    //create new waveform
    larlight::fifo new_wf( wf->channel_number(), wf->readout_frame_number(),
			  wf->readout_sample_number_RAW(), wf->module_address(),
			   wf->module_id(), wf->disc_id(), DATA::TPCFIFO, new_wf_bins);

    //std::cout << "NEW WF: " << (&new_wf)->channel_number() << std::endl;
    //write new waveform
    new_event_wf->push_back(new_wf);

  }

  bool WFCompress::finalize() {

    _merge_tree->Write();
  
    return true;
  }
}
#endif
