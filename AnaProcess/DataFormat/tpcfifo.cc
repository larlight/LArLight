#ifndef TPCFIFO_CC
#define TPCFIFO_CC

#include "tpcfifo.hh"

namespace larlight {

  void tpcfifo::clear_data(){
    data_base::clear_data();
    init_vars();
  }
  
  void tpcfifo::init_vars(){
    clear();
    _channel_number=FEM::INVALID_CH;
  }
  
  void event_tpcfifo::clear_data(){
    data_base::clear_data();
    init_vars();
  }
  
  void event_tpcfifo::init_vars(){
    clear();
    _event_frame_id=FEM::INVALID_WORD;
    _module_address=FEM::INVALID_WORD;
    _module_id=FEM::INVALID_WORD;
    _channel_header_count=FEM::INVALID_WORD;
    _checksum=FEM::INVALID_WORD;
    _nwords=FEM::INVALID_WORD;
    _trigger_frame_id=FEM::INVALID_WORD;
    _trigger_timeslice=FEM::INVALID_WORD;
  }
}
#endif
