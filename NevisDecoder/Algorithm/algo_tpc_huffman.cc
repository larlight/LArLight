#ifndef ALGO_TPC_HUFFMAN_CC
#define ALGO_TPC_HUFFMAN_CC

#include "algo_tpc_huffman.hh"

namespace larlight {

  algo_tpc_huffman::algo_tpc_huffman() : algo_tpc_xmit()
  {
    _name = "algo_tpc_huffman";
    reset();
  }  

  void algo_tpc_huffman::reset() {

    algo_tpc_xmit::reset();

  }

  void algo_tpc_huffman::clear_event() {

    _search_header = false;

    algo_tpc_xmit::clear_event();

  }

  bool algo_tpc_huffman::process_word(const UInt_t word) {

    // If in back_trace mode, add this word in record
    if(_bt_mode){

      // Check if buffer is filled
      if(!_bt_nwords_filled)
	_bt_nwords_filled = (_bt_nwords == _bt_words.size());

      // If filled, remove the oldest element
      if(_bt_nwords_filled)
	_bt_words.pop_front();

      // Add new word
      _bt_words.push_back(word);

    }

    if(word==0x0)

      return true;

    bool status = true;
    UInt_t word_class = get_word_class(word);
    UInt_t last_word_class = get_word_class(_last_word);

    if( _search_for_next_event ) {

      if( !(word_class == FEM::EVENT_HEADER && 
	    last_word_class == FEM::EVENT_LAST_WORD ) ) {

	_last_word = word;
	return status;
      }

    }



    //
    // In case of huffman coded data stream, uniquely marked
    // words are only:
    // (*) event header
    // (*) event last word 
    // (*) channel header
    // (*) channel last word
    //
    switch(word_class){

    case FEM::EVENT_HEADER:

      if( (last_word_class == FEM::EVENT_LAST_WORD && !(_header_info.nwords)) || !(_event_data)) {

	_search_for_next_event = false;

	status = process_event_header(word,_last_word);
      }
      else{

	UInt_t first_word  = (word & 0xffff);
	UInt_t second_word = (word >> 16);

	status = process_ch_word(first_word,_last_word);

	if(status) status = process_ch_word(second_word,_last_word);

      }

      break;

    case FEM::FEM_HEADER:

      if(_nwords && _nwords == _header_info.nwords){

	_nwords++;
	status = store_event();

      }

      if(status){ 

	if(!(_header_info.nwords))

	  status = process_fem_header(word,_last_word);

	else{

	  UInt_t first_word  = (word & 0xffff);
	  UInt_t second_word = (word >> 16);

	  status = process_ch_word(first_word,_last_word);

	  if(status) status = process_ch_word(second_word,_last_word);

	}
      }

      break;

    case FEM::EVENT_LAST_WORD:

      //if(last_word_class == FEM::CHANNEL_LAST_WORD){

      status = process_event_last_word(word,_last_word);
      break;
      //}

    default:

      UInt_t first_word  = (word & 0xffff);
      UInt_t second_word = (word >> 16);

      status = process_ch_word(first_word,_last_word);

      if(status) status = process_ch_word(second_word,_last_word);

    }

    if(!status){

      if(_bt_mode)
	backtrace();

      clear_event();

      if(_debug_mode){

	Message::send(MSG::WARNING,__FUNCTION__,"DEBUG MODE => Continue to next event...");

	_search_for_next_event = true;

      }

    }

    return status;
  }

  bool algo_tpc_huffman::process_event_header(const UInt_t word,
					      UInt_t &last_word) 
  {
    bool status = true;

    if(!_event_data)

      _event_data = (event_fifo*)(_storage->get_data(DATA::TPCFIFO));

    else if(get_word_class(last_word)!=FEM::EVENT_LAST_WORD){

      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Unexpected event header (%x) with the previous word %x!",word,last_word));

      status = false;

    }

    last_word = word;

    return status;
  }

  bool algo_tpc_huffman::process_fem_header(const UInt_t word,
					    UInt_t &last_word) 
  {
    bool status = true;
    //
    // This function must be called for true FEM headers.
    // Hence we implement a strict check to make sure this is FEM header
    //
    UInt_t last_word_class = get_word_class(last_word);

    if(last_word_class != FEM::EVENT_HEADER &&
       last_word_class != FEM::FEM_HEADER &&
       last_word_class != FEM::CHANNEL_LAST_WORD) {

      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Unexpected FEM header (%x) with the previous word %x!",word,last_word));

      status = false;
    }else if(get_word_class(word>>16)!=FEM::FEM_HEADER) {

      // Event header should come as a 32-bit word which is a pair of two 16-bit header words.
      // The first 16-bit is already checked by this point. Check the second part.

      Message::send(MSG::ERROR,__FUNCTION__,Form("Found an odd event header word: %x",word));

      status = false;    
    }else{

      _event_header_words[_event_header_count] = word;

      _event_header_count++;

      if(_event_header_count == FEM_HEADER_COUNT)

	status = decode_fem_header(_event_header_words);

    }

    last_word = word;

    return status;
  }

  bool algo_tpc_huffman::process_event_last_word(const UInt_t word,
						 UInt_t &last_word) 
  {
    bool status = true;

    //
    // Make an explicit check.
    // Previous word should be the channel last word
    // By this point data should be saved and _header_info should be cleared.
    //
    UInt_t last_word_class = get_word_class(last_word);

    if(last_word_class != FEM::CHANNEL_LAST_WORD) {

      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Unexpected event last word (%x) with the previous word %x!",word,last_word));

      status = false;

    }else if(_header_info.event_id!=FEM::INVALID_WORD){

      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("End of event (%x) ... but buffer holds data for event %d!",word,_header_info.event_id));

      status = false;

    }

    last_word = word;

    return status;
  }

  bool algo_tpc_huffman::process_fem_last_word(const UInt_t word,
					       UInt_t &last_word) 
  {
    //
    // This function should not be used as of now
    //
    Message::send(MSG::ERROR,__FUNCTION__,
		  "This function should not be called...");

    last_word = word;
    return false;
  }

  bool algo_tpc_huffman::process_ch_word(const UInt_t word,
					 UInt_t &last_word) 
  {

    //
    // This function expects either of three kinds:
    // ch header ... marked
    // ch data (adc) ... unmarked
    // ch last word ... marked
    //
    bool status = true;
    UInt_t word_class      = get_word_class(word);
    UInt_t last_word_class = get_word_class(last_word);

    // Check for zero-padding that is allowed after channel last word
    if(word == 0x0){

      if(get_word_class(last_word)!=FEM::CHANNEL_LAST_WORD){

	Message::send(MSG::ERROR,__FUNCTION__,
		      Form("Unexpected Zero-padding found after %x",last_word));
	status = false;
      }else if(_verbosity[MSG::INFO])

	Message::send(MSG::INFO,__FUNCTION__,
		      Form("Zero-padding found after %x",last_word));

      return status;
    }

    switch(word_class){

    case FEM::CHANNEL_HEADER:

      //
      // Check if the last word was event header or channel last word
      //
      if(last_word_class == FEM::FEM_HEADER ||
	 last_word_class == FEM::CHANNEL_LAST_WORD ) {

	// New data starts here. 
	// Clear data content & assign channel number.
	_ch_data.clear_data();

	_ch_data.set_channel_number( (word & 0xfff) );

	if(_verbosity[MSG::DEBUG])

	  Message::send(MSG::DEBUG,__FUNCTION__,
			Form("New channel header: %d",_ch_data.channel_number()));

      }else{

	Message::send(MSG::ERROR,__FUNCTION__,
		      Form("Unexpected channel header (%x)! Last word = %x",word,last_word));

	status = false;

      }

      break;

    case FEM::CHANNEL_LAST_WORD:{

      // Check channel number is same as the header
      UShort_t ch( (word & 0xfff) );
      if(ch == _ch_data.channel_number()) {

	if(_verbosity[MSG::DEBUG])

	  Message::send(MSG::DEBUG,__FUNCTION__,
			Form("Finished reading %zu samples for Ch %d",
			     _ch_data.size(),_ch_data.channel_number()));

	// Store
	_event_data->push_back(_ch_data);

	_ch_data.clear_data();

      }else{

	  Message::send(MSG::ERROR,__FUNCTION__,
			Form("Ch. number disagreement! Header: %u ... Last: %u!",
			     _ch_data.channel_number(),ch) );

	  status = false;
      }

      // Attempt to store data if nwords matches with the expected number
      if(status && _nwords == _header_info.nwords){
	_nwords++;
	_checksum += word;
	status = store_event();
      }

      break;
    }
    case FEM::FEM_LAST_WORD:

      if(last_word_class == FEM::CHANNEL_LAST_WORD)

	status = process_fem_last_word(word,last_word);

      else status = decode_ch_word(word,last_word);

      break;

    default:

      status = decode_ch_word(word,last_word);
    }

    if(status && _header_info.nwords){

      _nwords++;
      _checksum += word;

    }    

    last_word = word;

    return status;
  }
  
}

#endif
