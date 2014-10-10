#ifndef EXECUTECOMPRESSION_CC
#define EXECUTECOMPRESSION_CC

#include "ExecuteCompression.hh"

namespace larlight {

  bool ExecuteCompression::initialize() {

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

    if(!event_wf) {
      print(MSG::ERROR,__FUNCTION__,"Data storage did not find associated waveforms!");
      return false;
    }

    // clear holder for new, compressed, waveforms
    _out_event_wf.clear();
    
    //Loop over all waveforms
    for (size_t i=0; i<event_wf->size(); i++){
      
      //get tpc_data
      larlight::tpcfifo* tpc_data = (&(event_wf->at(i)));      
      //Check for empty waveforms!
      if(tpc_data->size()<1){
	Message::send(MSG::ERROR,__FUNCTION__,
		      Form("Found 0-length waveform: Event %d ... Ch. %d",event_wf->event_number(),tpc_data->channel_number()));
	continue;
      }//if wf size < 1
	  
      //finally, apply compression..
      std::vector<unsigned short> ADCwaveform = getADCs(tpc_data);
      _compress_algo->ApplyCompression(ADCwaveform);
      std::vector<std::vector<unsigned short> > compressOutput = _compress_algo->GetOutputWFs();
      //Calculate compression factor [ for now Ticks After / Ticks Before ]
      CalculateCompression(ADCwaveform, compressOutput);
      //clear _InWF and _OutWF from compression algo object
      _compress_algo->Reset();
      // Replace .root data file *event_wf* with new waveforms
      SwapData(tpc_data, compressOutput);
	
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

  void ExecuteCompression::SwapData(larlight::tpcfifo* tpc_data, std::vector<std::vector<unsigned short> > outputWFs){

    UInt_t wf_time = tpc_data->readout_sample_number_2MHz();
    UInt_t chan = tpc_data->channel_number();


    //loop over new waveforms created
    for (size_t n=0; n < outputWFs.size(); n++){
      larlight::tpcfifo new_tpc_data( chan, tpc_data->readout_frame_number(),
				      wf_time, tpc_data->module_address(),
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
