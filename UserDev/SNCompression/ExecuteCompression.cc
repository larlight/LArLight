#ifndef EXECUTECOMPRESSION_CC
#define EXECUTECOMPRESSION_CC

#include "ExecuteCompression.hh"

namespace larlight {

  bool ExecuteCompression::initialize() {
    
    // Initalize Histogram that tracks compression factor
    if (!_compress_tree) { _compress_tree = new TTree("_compress_tree","Compression Info Tree"); }
    _compress_tree->Branch("_evt",&_evt,"evt/I");
    _compress_tree->Branch("_compression",&_compression,"compression/D");
    _compress_tree->Branch("_compressionU",&_compressionU,"compressionU/D");
    _compress_tree->Branch("_compressionV",&_compressionV,"compressionV/D");
    _compress_tree->Branch("_compressionY",&_compressionY,"compressionY/D");

    _compressionU = 0;
    _compressionV = 0;
    _compressionY = 0;
    _compression  = 0;
    _NplU = _NplV = _NplY = 0;

    _evt = 0;

    return true;
  }
  
  bool ExecuteCompression::analyze(storage_manager* storage) {

    // If no compression algorithm has been defined, skip
    if ( _compress_algo == 0 ){
      print(MSG::ERROR,__FUNCTION__,"Compression Algorithm Not Set! Exiting");
      return false;
    }

    // Otherwise Get RawDigits and execute compression
    larlight::event_tpcfifo *event_wf = (event_tpcfifo*)(storage->get_data(DATA::TPCFIFO));
    // If raw_digits object is empty -> exit
    if(!event_wf) {
      print(MSG::ERROR,__FUNCTION__,"Data storage did not find associated waveforms!");
      return false;
    }

    // clear place-holder for new, compressed, waveforms
    _out_event_wf.clear();

    // reset variables that hold compression factor
    _inTicks  = 0;
    _outTicks = 0;
    
    // Loop over all waveforms
    for (size_t i=0; i<event_wf->size(); i++){
      
      //get tpc_data
      larlight::tpcfifo* tpc_data = (&(event_wf->at(i)));      
      //Check for empty waveforms!
      if(tpc_data->size()<1){
	Message::send(MSG::ERROR,__FUNCTION__,
		      Form("Found 0-length waveform: Event %d ... Ch. %d",event_wf->event_number(),tpc_data->channel_number()));
	continue;
      }//if wf size < 1

      // Figure out channel's plane:
      // used because different planes will have different "buffers"
      UInt_t ch = tpc_data->channel_number();
      int pl = larutil::Geometry::GetME()->ChannelToPlane(ch);

      // finally, apply compression:
      // *-------------------------*
      // 1) Convert tpc_data object to just the vector of shorts which make up the ADC ticks
      std::vector<unsigned short> ADCwaveform = getADCs(tpc_data);
      // 2) Now apply the compression algorithm. _compress_algo is an instance of CompressionAlgoBase
      _compress_algo->ApplyCompression(ADCwaveform,pl,ch);
      // 3) Retrieve the output waveforms (vectors of vectors of shorts) produced during the compression
      std::vector<std::vector<unsigned short> > compressOutput = _compress_algo->GetOutputWFs();
      // 4) Retrieve the time-ticks at which each output waveform saved starts
      std::vector<int> outTimes = _compress_algo->GetOutputWFTimes();

      //MergeOverlaps(compressOutput,outTimes);
      // 6) Study the Compression results for this channel
      _compress_study->StudyCompression(ADCwaveform, compressOutput, outTimes,pl);
      // 7) Calculate compression factor [ for now Ticks After / Ticks Before ]
      CalculateCompression(ADCwaveform, compressOutput, pl);
      // 8) clear _InWF and _OutWF from compression algo object -> resetting algorithm for next time it is called
      _compress_algo->Reset();
      if (_saveOutput){
	// 9) Replace .root data file *event_wf* with new waveforms
	SwapData(tpc_data, compressOutput, outTimes);
      }
	
    }//for all waveforms

    _compressionU /= _NplU;
    _compressionV /= _NplV;
    _compressionY /= _NplY;
    _compression  /= (_NplU+_NplV+_NplY);
    _compress_tree->Fill();
    _NplU = _NplV = _NplY = 0;
    _compressionU = _compressionV = _compressionY = 0;
    _evt += 1;
    
    //now take new WFs and place in event_wf vector
    if (_saveOutput){
      event_wf->clear();
      for (size_t m=0; m < _out_event_wf.size(); m++)
	event_wf->push_back(_out_event_wf.at(m));
    }
    return true;
  }

  bool ExecuteCompression::finalize() {

    _compress_algo->EndProcess(_fout);
    _compress_study->EndProcess(_fout);
    _compress_tree->Write();

    return true;
  }

  std::vector<unsigned short> ExecuteCompression::getADCs(larlight::tpcfifo* tpc_data){

    std::vector<unsigned short> ADCs;
    ADCs.clear();
    for (size_t i=0; i < tpc_data->size(); i++)
      ADCs.push_back((unsigned short)(tpc_data->at(i)));

    return ADCs;

  }

  void ExecuteCompression::SwapData(larlight::tpcfifo* tpc_data, std::vector<std::vector<unsigned short> > outputWFs,
				    std::vector<int> outTimes){

    // In this function we are taking the old larlight::tpcfifo object and replacing it with
    // new larlight::tpcfifo objects, one per output waveform coming from compression
    // Most of the information for this object (channel number, wire plane, etc) stays the same
    // What changes is the actual list of ADC counts, and the start time of the vector.

    UInt_t wf_time = tpc_data->readout_sample_number_RAW();
    UInt_t chan = tpc_data->channel_number();


    //loop over new waveforms created
    for (size_t n=0; n < outputWFs.size(); n++){
      larlight::tpcfifo new_tpc_data( chan, tpc_data->readout_frame_number(),
				      wf_time+outTimes.at(n), tpc_data->module_address(),
				      tpc_data->module_id(), larutil::Geometry::GetME()->View(chan),
				      larutil::Geometry::GetME()->SignalType(chan),
				      DATA::TPCFIFO, outputWFs.at(n));

      _out_event_wf.push_back(new_tpc_data);
    }

    return;
  }


  void ExecuteCompression::CalculateCompression(const std::vector<unsigned short> &beforeADCs,
						const std::vector<std::vector<unsigned short> > &afterADCs,
						int pl){
    
    double inTicks = beforeADCs.size();
    double outTicks = 0;
    
    for (size_t n=0; n < afterADCs.size(); n++)
      outTicks += afterADCs.at(n).size();
    
    if (pl==0){
      _compressionU += outTicks/inTicks;
      _NplU += 1;
    }
    else if (pl==1){
      _compressionV += outTicks/inTicks;
      _NplV += 1;
    }
    else if (pl==2){
      _compressionY += outTicks/inTicks;
      _NplY += 1;
    }
    else
      std::cout << "What plane? Error?" << std::endl;
    
    _compression += outTicks/inTicks;

    return;
  }

  


}
#endif
