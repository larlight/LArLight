#ifndef ALGO_XMIT_DECODER_CC
#define ALGO_XMIT_DECODER_CC
#include "algo_xmit_decoder.hh"

namespace larlight {

  bool algo_xmit_decoder::process_header(UInt_t word){
    
    bool status=true;
    if(get_word_class(word)==FEM::EVENT_HEADER)
      {
	if(_verbosity[MSG::INFO]) {
	  sprintf(_buf,"Found event header word: %x",word);
	  Message::send(MSG::INFO,__FUNCTION__,_buf);
	}
      }else
      status = algo_slow_readout_decoder::process_header(word);
    return status;

  }

}

#endif
