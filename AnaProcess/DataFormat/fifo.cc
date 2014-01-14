#ifndef FIFO_CC
#define FIFO_CC

#include "fifo.hh"

namespace larlight {

  fifo::fifo(DATA::DATA_TYPE type) : std::vector<UShort_t>(),
			       data_base(type)
  {
    if(_type!=DATA::PMTFIFO && _type!=DATA::TPCFIFO) {
      
      Message::send(MSG::ERROR,__FUNCTION__,Form("Invalid data type specification (=%d)!",_type));
      
      Message::send(MSG::ERROR,__FUNCTION__,"Assuming PMT FIFOChannel...");
      
      _type=DATA::PMTFIFO;
      
    }
    
    clear_data();
  }

  void fifo::clear_data(){
    data_base::clear_data();
    clear();
    init_vars();
  }
  
  void fifo::init_vars(){
    _channel_number=FEM::INVALID_CH;
    _channel_frame_id=FEM::INVALID_WORD;
    _disc_id=FEM::DISC_MAX;
    _timeslice=FEM::INVALID_WORD;
  }
  
  void event_fifo::clear_data(){
    data_base::clear_data();
    clear();
    init_vars();
  }
  
  void event_fifo::init_vars(){
    _event_frame_id=FEM::INVALID_WORD;
    _module_address=FEM::INVALID_WORD;
    _module_id=FEM::INVALID_WORD;
    _checksum=FEM::INVALID_WORD;
    _nwords=FEM::INVALID_WORD;
    _trigger_frame_id=FEM::INVALID_WORD;
    _trigger_timeslice=FEM::INVALID_WORD;
  }

}
#endif
