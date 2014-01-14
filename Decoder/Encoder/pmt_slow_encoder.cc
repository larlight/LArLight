#ifndef PMT_SLOW_ENCODER_CC
#define PMT_SLOW_ENCODER_CC

#include "pmt_slow_encoder.hh"

void reset(){

}

bool pmt_slow_encoder::initialize() {

  _bin_file.open();
  return _bin_file.is_open();
}

bool pmt_slow_encoder::analyze(storage_manager *storage){

  _index=0;

  pmt_wf_collection *data = (pmt_wf_collection*)(storage->get_data(DATA_STRUCT::PMT_WF_COLLECTION));

  encode_event_header(data);
  encode_ch_word(data);

  _bin_file.write_multi_word(_words,_index+1);
  _index=0;
  return true;
}

void pmt_slow_encoder::encode_event_header(const pmt_wf_collection* data) {

  // (1) First 32-bit word include module address and id
  _words[_index]  = (format(PMT::FEM_HEADER,
			    ((data->module_id())<<5) + data->module_address()))<<16;
  _words[_index] += format(PMT::FEM_HEADER,0xfff);
  _index++;

  // (2) Second 32-bit word include word counts
  _words[_index]  = format(PMT::FEM_HEADER,(data->nwords())>>12);
  _words[_index] += (format(PMT::FEM_HEADER,(data->nwords()) & 0xfff))<<16;
  _index++;
  
  // (3) Third 32-bit word include event number
  _words[_index]  = format(PMT::FEM_HEADER,(data->event_id())>>12);
  _words[_index] += (format(PMT::FEM_HEADER,(data->event_id()) & 0xfff))<<16;
  _index++;

  // (4) Forth 32-bit word include frame number
  _words[_index]  = format(PMT::FEM_HEADER,(data->event_frame_id())>>12);
  _words[_index] += (format(PMT::FEM_HEADER,(data->event_frame_id()) & 0xfff))<<16;
  _index++;

  // (5) Fifth 32-bit word include checksum
  _words[_index]  = format(PMT::FEM_HEADER,
			   (data->checksum())>>12);
  _words[_index] += (format(PMT::FEM_HEADER,
			    (data->checksum()) & 0xfff))<<16;
  _index++;

  // (6) Six-th 32-bit word include trigger info
  _words[_index]  = (format(PMT::FEM_HEADER,
			    (data->trigger_timeslice()) & 0xff))<<16;
  _words[_index] += format(PMT::FEM_HEADER,
			   (((data->trigger_frame_id()) & 0xf)<<4) + ((data->trigger_timeslice() & 0xffff)>>8));
  _index++;
  
}

void pmt_slow_encoder::encode_ch_word(const pmt_wf_collection* data) {


  for(pmt_wf_collection::const_iterator iter(data->begin());
      iter!=data->end();
      ++iter){

    

  }

}

bool pmt_slow_encoder::finalize() {

  _bin_file.close();
  return true;
}

#endif
