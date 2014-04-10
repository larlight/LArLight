#ifndef FASTCOMPRESS_CC
#define FASTCOMPRESS_CC

#include "FastCompress.hh"

namespace larlight {

  bool FastCompress::initialize() {

    _event_num      =    0;
    _compressfctr   =    0;
    _PedCollection  =  400;
    _PedInduction   = 2048;
    _baseline       =    0;
    //First set all parameters for all compression schemes
    _Threshold      =    10;
    _NVarSamples    =    10;
    _NSamplesante   =    15;
    _NSamplespost   =    30;
    _VarCut         =   100;
    _Scheme         = "threshold";

    //Histogram that will store compression factor
    hCompress = new TH1D("Compression", "Compression Factor [1/%]", 1000, 0, 3);

    //read compression scheme and params from file
    std::ifstream myin;
    std::string line;
    std::string params;
    std::vector<std::string> paramarray;
    myin.open("compression_parameters.txt");
    std::getline(myin,line);
    std::stringstream linestream(line);
    while ( std::getline(linestream,params,'\t') )
      paramarray.push_back(params);
    _Scheme = paramarray.at(0).c_str();
    if ( _Scheme == "threshold" ){
      _Threshold = atoi(paramarray.at(1).c_str());
      std::cout << "-----------------------------------------" << std::endl;
      std::cout << "Scheme chosen: " << _Scheme << std::endl;
      std::cout << "threshold = " << _Threshold << std::endl;
      std::cout << "-----------------------------------------" << std::endl;
    }
    if ( _Scheme == "variance" ){
      double varask  = atof(paramarray.at(1).c_str());
      _NVarSamples   = atoi(paramarray.at(2).c_str());
      _NSamplesante  = atoi(paramarray.at(3).c_str());
      _NSamplespost  = atoi(paramarray.at(4).c_str());
      _VarCut        = _NVarSamples*varask;
      std::cout << "-----------------------------------------" << std::endl;
      std::cout << "Scheme chosen: " << _Scheme << std::endl;
      std::cout << "var/bin = " << varask << std::endl;
      std::cout << " NVarSamples = " << _NVarSamples << std::endl;
      std::cout << " NSamplesante = " << _NSamplesante << std::endl;
      std::cout << " NSamplespost = " << _NSamplespost << std::endl;
      std::cout << "-----------------------------------------" << std::endl;
    }
    myin.close();

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
      if ( _Scheme == "threshold" )
	ThresholdCompress( tpc_data, &new_event_wf);
      if ( _Scheme == "variance" )
	VarianceCompress( tpc_data, &new_event_wf);
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
    for (unsigned int i=0; i<new_event_wf.size(); i++){
      event_wf->push_back(new_event_wf.at(i));
    }

    
    return true;
  }

  //---------------------------------------------------------------------------------------------------
  void FastCompress::ThresholdCompress( larlight::tpcfifo* tpc_data, larlight::event_tpcfifo* new_event_wf) {

    //boolean to see if in interesting segment
    bool interesting = false;
    //holders for wf start and end
    UShort_t wf_start = 0;
    UShort_t wf_end = 0;
    //baseline for this wf
    UInt_t chan = tpc_data->channel_number();
    short baseline;
    if ( larutil::Geometry::GetME()->SignalType(chan) == larlight::GEO::kInduction)
      baseline = _PedInduction;
    else if ( larutil::Geometry::GetME()->SignalType(chan) == larlight::GEO::kCollection )
      baseline = _PedCollection;
    else {
      //Baseline not found! Save full waveform!
      //save waveform!
      make_new_wf(tpc_data, 0, tpc_data->size(), new_event_wf);
      _compressfctr += tpc_data->size();
      return;
    }

    //Get Waveform
    for (UShort_t adc_index=0; adc_index<tpc_data->size(); adc_index++){
	
      short adcs = tpc_data->at(adc_index);
	
      //does it make the cut?
      if ( ((adcs-baseline) > _Threshold) or ((adcs-baseline) < -_Threshold) ){
	if (!interesting)
	  wf_start = adc_index;
	interesting = true;
      }
      else {
	//if interesting is true means end of saving...
	if (interesting){
	  wf_end = adc_index;	
	  make_new_wf(tpc_data, wf_start, wf_end, new_event_wf);
	  _compressfctr += (wf_end-wf_start);
	  wf_start = 0;
	  wf_end = 0;
	}
	interesting = false;
      }
    }//for ADC Ticks    
    
    return;
      
  }



  //---------------------------------------------------------------------------------------------------
  void FastCompress::VarianceCompress( larlight::tpcfifo* tpc_data, larlight::event_tpcfifo* new_event_wf) {

      
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
		make_new_wf(tpc_data, adc_index-_NSamplesante, adc_index+_NSamplespost, new_event_wf);
		prev_recorded = adc_index+_NSamplespost;
		//calculate compresson
		_compressfctr += (1+_NSamplesante+_NSamplespost);  //1 16-bit word per ADC sample 
	      }
	    
	  }//if not at beginning of wf
	  
      }//for all adc bins
      
  }

  void FastCompress::make_new_wf(larlight::tpcfifo* wf, int ADC_start,
				 int ADC_end, larlight::event_tpcfifo* new_event_wf) {

    UInt_t wf_time = wf->readout_sample_number_2MHz();
    wf_time = ADC_start;

    //create vector where to store ADCs
    std::vector<UShort_t> new_wf_bins;
  
    //make sure not overflowing before or after original waveform
    int start_record = ADC_start;
    if (start_record < 0) { start_record = 0; wf_time = 0; }
    int end_record   = ADC_end;
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

    //write new waveform
    new_event_wf->push_back(new_wf);

  }




  bool FastCompress::finalize() {
  
    float compression = 1.0/(hCompress->GetMean());
    std::cout << "-----------------------------------------" << std::endl;    
    std::cout << "Compression: " << compression << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
    hCompress->Write();
  
    return true;
  }
}
#endif
