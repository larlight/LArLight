#include "algo_sn_tpc_huffman.hh"

namespace larlight {

  algo_sn_tpc_huffman::algo_sn_tpc_huffman() : algo_tpc_huffman()
  {
    _name = "algo_sn_tpc_huffman";
  }  

  FEM::FEM_WORD algo_sn_tpc_huffman::get_word_class(const UInt_t word) const {
      // One of core functions to identify PMT binary word format
      if( word == 0x0 ) return FEM::UNDEFINED_WORD;
      else if( (word & 0xffffffff) == 0xffffffff ) // Unique marker
        return FEM::EVENT_HEADER;
      else if( (word & 0xffffffff) == 0xe0000000 ) // Unique marker
        return FEM::EVENT_LAST_WORD;
      if( (word & 0xffff) == 0xffff )              // Unique marker
        return FEM::FEM_HEADER;
      // else if( (word & 0xf000) == 0xf000 )         // Could be ADC word
      //   return FEM::FEM_HEADER;
      // Yun-Tse 2014/11/14: Cannot handle the ADC words with two more ffff
      else if ( (word & 0xf000) == 0xf000 && ( ( _last_word & 0xf000 ) == 0xf000 || ( _last_word & 0xf000 ) == 0x3000 ) )
        return FEM::FEM_HEADER;
      else if( !((word>>15) & 0x1) ) {

        if ( ( word & 0xf000 ) == 0x1000 )         // Channel time word
          return FEM::CHANNEL_TIME;
        else if( (word & 0xf000) == 0x2000 )       // Uncompressed ADC word
          return FEM::CHANNEL_WORD;
        else if ( ( word & 0xf000 ) == 0x3000 )    // Channel: last word of the packet
          return FEM::CHANNEL_PACKET_LAST_WORD;
        else if( (word & 0xf000) == 0x4000 )       // Channel first word
          return FEM::CHANNEL_HEADER;
        else
          return FEM::UNDEFINED_WORD;              // Undefined
      }
      else if( ((word>>15) & 0x1) ){
        return FEM::CHANNEL_WORD;                  // compressed ADC word
      }else
        return FEM::UNDEFINED_WORD;

    // FEM::FEM_WORD code = algo_tpc_huffman::get_word_class(word); 
  }

  bool algo_sn_tpc_huffman::process_word(const UInt_t word) {
    Message::send(MSG::INFO,__FUNCTION__, Form("word %x, last word %x", word, _last_word) );

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

    Message::send(MSG::INFO,__FUNCTION__, Form("word type=%d",word_class));

    if( _search_for_next_event ) {

      if( !(word_class == FEM::EVENT_HEADER && 
	    last_word_class == FEM::EVENT_LAST_WORD ) ) {

	_last_word = word;
	return status;
      }

    }



    //
    // In case of huffman coded SN data stream, uniquely marked
    // words are only:
    // (*) event header
    // (*) event last word 
    // (*) channel header
    // (*) channel time word
    // (*) channel last word of packet
    // (*) channel ADC word
    //
    switch(word_class){

    case FEM::EVENT_HEADER:

      Message::send(MSG::INFO,__FUNCTION__, "word = event header");
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

      Message::send(MSG::INFO,__FUNCTION__, Form("word = FEM header, nword = %d, header info nword = %d", _nwords, _header_info.nwords) );
      // Yun-Tse 2014/11/19: I don't understand why this if statement is here...
      if(_nwords && _nwords == _header_info.nwords){
        Message::send(MSG::INFO,__FUNCTION__, Form("word = FEM header, nword = %d, header info nword = %d, we have _nwords++; and store_event()", _nwords, _header_info.nwords) );

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

      Message::send(MSG::INFO,__FUNCTION__, "word = event last word");
      status = process_event_last_word(word,_last_word);
      break;
      //}

    default:

      Message::send(MSG::INFO,__FUNCTION__, "word = the rest");
      UInt_t first_word  = (word & 0xffff);
      UInt_t second_word = (word >> 16);

      Message::send(MSG::INFO,__FUNCTION__, Form("1st word type=%d ... 2nd word type=%d",
						 get_word_class(first_word),
						 get_word_class(second_word)));

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

  bool algo_sn_tpc_huffman::process_event_last_word(const UInt_t word,
                                                 UInt_t &last_word)
  {
    bool status = true;

    //
    // Make an explicit check.
    // Previous word should be the channel last word of packet
    //
    UInt_t last_word_class = get_word_class(last_word);

    if(last_word_class != FEM::CHANNEL_PACKET_LAST_WORD) {

      Message::send(MSG::ERROR,__FUNCTION__,
                    Form("Unexpected event last word (%x) with the previous word %x!",word,last_word));

      status = false;

    }

    last_word = word;
    status = store_event();

    return status;
  }

  bool algo_sn_tpc_huffman::process_ch_word(const UInt_t word,
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

      /*
      if(get_word_class(last_word)!=FEM::CHANNEL_LAST_WORD){

	Message::send(MSG::ERROR,__FUNCTION__,
		      Form("Unexpected Zero-padding found after %x",last_word));
	status = false;
      }else if(_verbosity[MSG::INFO])
      */
      if ( get_word_class(last_word) == FEM::CHANNEL_PACKET_LAST_WORD ) {

	Message::send(MSG::INFO,__FUNCTION__,
		      Form("Zero-padding found after %x",last_word));

	return status;
      }
    }

    switch(word_class){

    case FEM::CHANNEL_HEADER:
      Message::send(MSG::INFO,__FUNCTION__, "word = channel header");

      if ( last_word_class == FEM::CHANNEL_PACKET_LAST_WORD ) {

        if(_verbosity[MSG::DEBUG])
          Message::send(MSG::DEBUG,__FUNCTION__,
                        Form("Finished reading %zu samples for Ch %d",
                             _ch_data.size(),_ch_data.channel_number()));

        // Store
        _ch_data.set_module_id(_header_info.module_id);
        _ch_data.set_module_address(_header_info.module_address);
        _event_data->push_back(_ch_data);

        _ch_data.clear_data();
        _ch_data.set_channel_number( (word & 0x3f) );


        Message::send( MSG::INFO,__FUNCTION__, 
                       Form("word = channel header, last word = channel packet last word, nwords = %d, header info nwords = %d" , _nwords, _header_info.nwords) );
        // Attempt to store data if nwords matches with the expected number
        if(status && _nwords == _header_info.nwords){
          Message::send( MSG::INFO,__FUNCTION__, Form("I don't know why I'm here: if(status && _nwords == _header_info.nwords), word = channel header, last word = channel packet last word, nwords = %d, header info nwords = %d, we have _nwords++, _checksum, and store_event() " , _nwords, _header_info.nwords) );
          _nwords++;
          _checksum += word;
          status = store_event();
        }

      }       
      //
      // Check if the last word was event header
      //
      else if (last_word_class == FEM::FEM_HEADER ) {
        Message::send(MSG::INFO,__FUNCTION__, "word = channel header, last word = FEM header");

        // New data starts here.
        // Clear data content & assign channel number.
        _ch_data.clear_data();

        _ch_data.set_channel_number( (word & 0x3f) );

        if(_verbosity[MSG::DEBUG])

          Message::send(MSG::DEBUG,__FUNCTION__,
                        Form("New channel header: %d",_ch_data.channel_number()));

      } else {

	Message::send(MSG::ERROR,__FUNCTION__,
		      Form("Unexpected channel header (%x)! Last word = %x",word,last_word));

	status = false;

      }

      break;

    case FEM::EVENT_LAST_WORD: {
      Message::send( MSG::INFO,__FUNCTION__, Form("word = event last word, nwords = %d, header info nwords = %d" , _nwords, _header_info.nwords) );
      // Store
      _ch_data.set_module_id(_header_info.module_id);
      _ch_data.set_module_address(_header_info.module_address);
      _event_data->push_back(_ch_data);

      _ch_data.clear_data();

      
      // Attempt to store data if nwords matches with the expected number
      if(status && _nwords == _header_info.nwords){
      Message::send( MSG::INFO,__FUNCTION__, Form("if(status && _nwords == _header_info.nwords), word = event last word, nwords = %d, header info nwords = %d, we have _nwords++, checksum, and store_event()" , _nwords, _header_info.nwords) );
        _nwords++;
        _checksum += word;
        status = store_event();
      }
      break;
    }

    default:

      status = decode_ch_word(word,last_word);
    }

    if(status && _header_info.nwords){

      Message::send( MSG::INFO,__FUNCTION__, Form("if(status && _header_info.nwords), nwords = %d, header info nwords = %d, we have _nwords++ and checksum" , _nwords, _header_info.nwords) );
      _nwords++;
      _checksum += word;

    }    

    Message::send(MSG::INFO,__FUNCTION__, Form("end of function, nword = %d, header info nwords = %d", _nwords, _header_info.nwords) );
    last_word = word;

    return status;
  }
  
}
