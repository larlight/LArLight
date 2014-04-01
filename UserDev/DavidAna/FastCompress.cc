#ifndef FASTCOMPRESS_CC
#define FASTCOMPRESS_CC

#include "FastCompress.hh"

namespace larlight {

  bool FastCompress::initialize() {

    _event_num      =   0;
    _compressfctr   =   0;
    hCompress = new TH1D("Compression", "Compression Factor [1/%]", 30, 0, 30);

    std::ifstream myin;
    std::string line;
    std::string params;
    std::vector<std::string> paramarray;
    myin.open("compression_parameters.txt");
    std::getline(myin,line);
    std::stringstream linestream(line);
    while ( std::getline(linestream,params,'\t') )
      paramarray.push_back(params);
    double varask = atof(paramarray.at(0).c_str());
    _NVarSamples = atoi(paramarray.at(1).c_str());
    _NSamplesante = atoi(paramarray.at(2).c_str());
    _NSamplespost = atoi(paramarray.at(3).c_str());
    std::cout << "var/bin: " << varask << " _NVarSamples: " << _NVarSamples
	      << " _NSamplesante: " << _NSamplesante << " NSamplespost: " 
	      << _NSamplespost << std::endl;
    myin.close();
    _VarCut        =  _NVarSamples*varask;
    _baseline      =   0;
    std::cout << "The cut on variance is: " << _VarCut << std::endl;


    return true;
  }
  
  bool FastCompress::analyze(storage_manager* storage) {

    _event_num += 1;

    //get vector of waveforms
    larlight::event_tpcfifo *event_wf = (event_tpcfifo*)(storage->get_data(DATA::TPCFIFO));
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
      larlight::tpcfifo* tpc_data = (&(event_wf->at(i)));      
      wfnum += 1;
      //Check for empty waveforms!
      if(tpc_data->size()<1){
	Message::send(MSG::ERROR,__FUNCTION__,
		      Form("Found 0-length waveform: Event %d ... Ch. %d",event_wf->event_number(),tpc_data->channel_number()));
	continue;
      }
      
      _compressfctr = 0;
      //Apply Compression algorithm
      Compress( tpc_data, &new_event_wf);
      //original waveform: if counting header info: +2+10+1+1+2
      double wf_original_size = tpc_data->size();
      _compressfctr = _compressfctr/wf_original_size;
      hCompress->Fill(_compressfctr);

    }//loop over all waveofmrs

    if (new_event_wf.size() >= 1)
      _wfnumber = new_event_wf.size();

    //replace event_ef with new_event_wf data
    //this will be outputed to output script
    event_wf->clear();
    for (int i=0; i<new_event_wf.size(); i++){
      event_wf->push_back(new_event_wf.at(i));
    }

    
    return true;
  }

  //****************************************************************************************
  void FastCompress::Compress( larlight::tpcfifo* tpc_data, larlight::event_tpcfifo* new_event_wf) {
  //****************************************************************************************
      
      //***************************
      //Start Compression Algorithm
      //***************************

      //short vector containing last few samples
      std::deque<int> last_few;
      //set variable for baseline
      double baseline_tot = 0;
      double old_point    = 0;
      //initially just set baseline to first point
      double baseline     = tpc_data->at(1);
      double baseold      = 0;
      double delbase      = 0;
      //initially se variance to 0
      double variance     = 0;
      //set to last bin recorded by prev waveform
      int prev_recorded = 0;
      
      //Get Waveform
      for (UShort_t adc_index=0; adc_index<tpc_data->size(); adc_index++){
	
	int adcs = tpc_data->at(adc_index);
	
	//IF before _NSamples don't do anything
	if ( adc_index < _NVarSamples)
	  {
	    last_few.push_back(adcs);
	    baseline_tot += adcs;
	    baseline = baseline_tot/_NVarSamples;
	  }
	else
	  {
	    old_point = last_few.at(0);
	    baseline_tot -= old_point;
	    last_few.pop_front();
	    baseline_tot += adcs;
	    last_few.push_back(adcs);
	    baseold = baseline;
	    baseline = baseline_tot/_NVarSamples;
	    delbase = baseline - baseold;
	    //if variance = 0...then recalculate
	    if ( variance == 0){
	      for (UShort_t i=0; i<last_few.size(); i++)
		variance += pow((last_few.at(i)-baseline),2);
	    }
	    //else...
	    else{
	      variance -= (old_point-baseold)*(old_point-baseold);
	      variance += (adcs-baseold)*(adcs-baseold);
	      variance -= _NVarSamples*delbase*delbase;
	    }


	    if ( (variance > _VarCut) && (adc_index > prev_recorded) )
	      {
		variance = 0;
		make_new_wf_simple(tpc_data, adc_index, new_event_wf);
		prev_recorded = adc_index+_NSamplespost;
		//calculate compresson
		_compressfctr += (1+_NSamplesante+_NSamplespost);  //1 16-bit word per ADC sample 
	      }
	    
	  }//if not at beginning of wf
	  
      }//for all adc bins
      
  }

  void FastCompress::make_new_wf_simple(larlight::tpcfifo* wf, int start, larlight::event_tpcfifo* new_event_wf) {

    UInt_t wf_time = wf->readout_sample_number_2MHz();
    wf_time = start-_NSamplesante;

    //create vector where to store ADCs
    std::vector<UShort_t> new_wf_bins;
  
    //make sure not overflowing before or after original waveform
    int start_record = start-_NSamplesante;
    if (start_record < 0) { start_record = 0; wf_time = 0; }
    int end_record   = start + _NSamplespost;
    if (end_record > wf->size()) { end_record = wf->size(); }
  
    //get interesting samples +- _NSamples
    for (int i=start_record; i<end_record; i++)
      new_wf_bins.push_back(wf->at(i));

    //print compression
    //std::cout << "Compressing WF of size: " << wf->size() << std::endl;
    //std::cout << "Into WF of size:        " << new_wf_bins.size() << std::endl;
    
    //create new waveform
    UInt_t chan = wf->channel_number();
    larlight::tpcfifo new_wf( chan, wf->readout_frame_number(),
			      wf_time, wf->module_address(),
			      wf->module_id(), larutil::Geometry::GetME()->View(chan),
			      larutil::Geometry::GetME()->SignalType(chan), DATA::TPCFIFO, new_wf_bins);

    //std::cout << "NEW WF: " << (&new_wf)->channel_number() << std::endl;
    //write new waveform
    new_event_wf->push_back(new_wf);
    //std::cout << new_wf.readout_sample_number_RAW() << std::endl;
  }




  bool FastCompress::finalize() {
  
    float compression = 1.0/(hCompress->GetMean());
    std::cout << "Compression: " << compression << std::endl;
    hCompress->Write();
  
    return true;
  }
}
#endif
