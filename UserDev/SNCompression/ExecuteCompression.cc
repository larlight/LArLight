#ifndef EXECUTECOMPRESSION_CC
#define EXECUTECOMPRESSION_CC

#include "ExecuteCompression.hh"

namespace larlight {

  bool ExecuteCompression::initialize() {
    /// currently nothing is done in the initialize function
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
      int pl = larutil::Geometry::GetME()->ChannelToPlane(tpc_data->channel_number());
	  
      // finally, apply compression:
      // *-------------------------*
      // 1) Convert tpc_data object to just the vector of shorts which make up the ADC ticks
      std::vector<unsigned short> ADCwaveform = getADCs(tpc_data);
      // 2) Now apply the compression algorithm. _compress_algo is an instance of CompressionAlgoBase
      _compress_algo->ApplyCompression(ADCwaveform,pl);
      // 3) Retrieve the output waveforms (vectors of vectors of shorts) produced during the compression
      std::vector<std::vector<unsigned short> > compressOutput = _compress_algo->GetOutputWFs();
      // 4) Retrieve the time-ticks at which each output waveform saved starts
      std::vector<int> outTimes = _compress_algo->GetOutputWFTimes();
      // 5) Calculate compression factor [ for now Ticks After / Ticks Before ]
      CalculateCompression(ADCwaveform, compressOutput);
      // 6) clear _InWF and _OutWF from compression algo object -> resetting algorithm for next time it is called
      _compress_algo->Reset();
      // 7) Replace .root data file *event_wf* with new waveforms
      SwapData(tpc_data, compressOutput, outTimes);
	
    }//for all waveforms

    //now take new WFs and place in event_wf vector
    event_wf->clear();
    for (size_t m=0; m < _out_event_wf.size(); m++)
      event_wf->push_back(_out_event_wf.at(m));

    return true;
  }

  bool ExecuteCompression::finalize() {

    // Print out compression factor
    std::cout << "Compression Factor is: " << _inTicks/_outTicks << std::endl;
  
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


  void ExecuteCompression::CalculateCompression(std::vector<unsigned short> beforeADCs, std::vector<std::vector<unsigned short> > afterADCs){
    
    _inTicks += beforeADCs.size();

    for (size_t n=0; n < afterADCs.size(); n++)
      _outTicks += afterADCs.at(n).size();

    return;
  }

}
#endif
