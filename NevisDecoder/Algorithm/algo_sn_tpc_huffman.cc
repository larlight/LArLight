#include "algo_sn_tpc_huffman.hh"

namespace larlight {

  algo_sn_tpc_huffman::algo_sn_tpc_huffman() : algo_tpc_huffman()
  {
    _name = "algo_sn_tpc_huffman";
    reset();
  }  

  FEM::FEM_WORD algo_sn_tpc_huffman::get_word_class(const UInt_t word) const {
      // One of core functions to identify PMT binary word format
      if( word == 0x0 ) return FEM::UNDEFINED_WORD;
      else if( (word & 0xffffffff) == 0xffffffff ) // Unique marker, but with the huffman
                                                   // coding, not unique anymore
        return FEM::EVENT_HEADER;
      else if( (word & 0xffffffff) == 0xe0000000 ) // Unique marker, but with the huffman
                                                   // coding, not unique anymore
        return FEM::EVENT_LAST_WORD;
      if( (word & 0xffff) == 0xffff )              // Unique marker, but with the huffman
                                                   // coding, not unique anymore
        return FEM::FEM_HEADER;
      else if( (word & 0xf000) == 0xf000 )         // Could be ADC word
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

  //#################################################
  bool algo_sn_tpc_huffman::decode_fem_header(const UInt_t *event_header){
  //#################################################

    bool status=true;
    //
    // Get event information
    //

    // (1) check if the very first 16-bit word is as expected
    if(!( event_header[0] & 0xffff))
      Message::send(MSG::ERROR,"Unexpected first word in event headers!");

    if(!status) return status;

    // Initialize data holder
    _header_info.clear_event();

    // (2) get module address ... lowest 5 of 12 bits
    _header_info.module_address = ( (event_header[0]>>16 & 0xfff)    & 0x1f);

    // (3) get module ID number ... 8:5 bit of 12 bits
    _header_info.module_id      = ( (event_header[0]>>16 & 0xfff)>>5 & 0xf);

    // (4) get number of 16-bit words to be read in this event.
    // Lower 12 bits of two 16-bit words.
    // The very last "last word for channel info" is not included in this.
    // For later checking purpose, increment by 1.
    _header_info.nwords         = ( (((event_header[1]>>16) & 0xfff) + ((event_header[1] & 0xfff)<<12)));

    // (5) get event ID
    // Lower 12 bits of two 16-bit words.
    _header_info.event_number       = ( (((event_header[2]>>16) & 0xfff) + ((event_header[2] & 0xfff)<<12)));

    // (6) get frame ID
    // Lower 12 bits of two 16-bit words.
    _header_info.event_frame_number = ( (((event_header[3]>>16) & 0xfff) + ((event_header[3] & 0xfff)<<12)));

    // (7) get checksum
    _header_info.checksum       = ( (((event_header[4]>>16) & 0xfff) + ((event_header[4] & 0xfff)<<12)));


    // Set the trigger frame to 0 to avoid confusion (any smart guess will only hurt)
    _header_info.fem_trig_frame_number = 0;

    _header_info.fem_trig_sample_number = 0;

    // Report if verbosity is set.
    // if(_verbosity[MSG::INFO])
    //   {
	std::string msg;
	for(size_t i=0; i<FEM_HEADER_COUNT; ++i)
	  msg += Form("%x ", event_header[i]);
	Message::send(MSG::INFO, __FUNCTION__, Form("Decoded Header: %s",msg.c_str()));
	Message::send(MSG::INFO, __FUNCTION__, Form("Module %d (ID=%d)", _header_info.module_address, _header_info.module_id));
	Message::send(MSG::INFO, __FUNCTION__, Form("Event ID %d",_header_info.event_number));
	Message::send(MSG::INFO, __FUNCTION__, Form("Frame ID %d",_header_info.event_frame_number));
	Message::send(MSG::INFO, __FUNCTION__, Form("Number of Words = %d",_header_info.nwords));
	Message::send(MSG::INFO, __FUNCTION__, Form("Checksum = %x", _header_info.checksum));

    //  }

    _checksum=0;

    _nwords=0;

    return status;
  }

  bool algo_sn_tpc_huffman::process_word(const UInt_t word) {

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

    if ( word == 0x0 ) {
      Message::send( MSG::WARNING, __FUNCTION__, Form("Unexpected 32-bit zero-padding, last word 0x%x.", _last_word) );
      return true;
    }

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
    // In case of huffman coded SN data stream, uniquely marked
    // words are only:
    // (*) channel header
    // (*) channel time word
    // (*) channel last word of packet
    // (*) channel ADC word (non-Huffman compression)
    //
    switch(word_class){

    case FEM::EVENT_HEADER:
      // Yun-Tse 2015/1/26: Need a condition since it is possible to be an ADC word
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
      // Yun-Tse 2015/1/26: This is for the data without event header and event last word...
      /* if(_nwords && _nwords == _header_info.nwords){
        Message::send(MSG::INFO,__FUNCTION__, Form("word = FEM header, nword = %d, header info nword = %d, we have _nwords++; and store_event()", _nwords, _header_info.nwords) );

	_nwords++;
	status = store_event();

      } */

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
      if ( last_word_class == FEM::CHANNEL_PACKET_LAST_WORD ) {
        // This is the normal end of event, with some packets in the last channel
        status = process_event_last_word(word,_last_word);

      } else if ( ( last_word_class == FEM::CHANNEL_HEADER ) || ( _last_word == 0x0 ) ) {
        // If the last channel contains no packet after zero-suppression,
        // we have to store this channel when the end of FEM or the end of event is met.
        // In this decoder, an "event" contains a single FEM;
        // if we have multiple FEMs in an event, we will have multiple events
        // in the frame of the decoder.
        // Therefore we only need to deal with the end of event.
        // There could be two possibilities for the previous word in this case
        // 1) Channel header
        // 2) a 16-bit zero-padding
        store_ch_data();
        status = process_event_last_word(word,_last_word);

      } else {

        UInt_t first_word  = (word & 0xffff);
        UInt_t second_word = (word >> 16);

        status = process_ch_word(first_word,_last_word);

        if(status) status = process_ch_word(second_word,_last_word);
      }

      break;
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

  bool algo_sn_tpc_huffman::process_event_last_word(const UInt_t word,
                                                 UInt_t &last_word)
  {
    bool status = true;

    //
    // Make an explicit check.
    // Previous word should be the channel last word of packet
    //
    UInt_t last_word_class = get_word_class(last_word);

    // Yun-Tse 2015/1/20: Add the possibility to have a zero-padding word before
    if ( ( last_word_class != FEM::CHANNEL_PACKET_LAST_WORD ) && ( last_word != 0x0 ) ) {

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
    // This function expects either of four kinds:
    // ch header ... marked
    // ch packet time
    // ch data (adc) ... unmarked, either Huffman compressed or not
    // ch packet last word ... marked
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

      // If this channel is NOT the first channel in a FEM
      if ( last_word_class == FEM::CHANNEL_PACKET_LAST_WORD ) {

        _channel_number_holder = (word & 0x3f);
        _readout_frame_number_holder = ( ( word >> 6) & 0x3f );

        if(_verbosity[MSG::DEBUG])
          Message::send( MSG::DEBUG,__FUNCTION__, 
                         Form("New channel number: %d, New frame number: %d", _channel_number_holder, _readout_frame_number_holder ) ); 
        // Yun-Tse 2015/1/26: Here is for the data format without event header and event last word
        // Attempt to store data if nwords matches with the expected number
        /* if(status && _nwords == _header_info.nwords){
          Message::send( MSG::INFO,__FUNCTION__, Form("I don't know why I'm here: if(status && _nwords == _header_info.nwords), word = channel header, last word = channel packet last word, nwords = %d, header info nwords = %d, we have _nwords++, _checksum, and store_event() " , _nwords, _header_info.nwords) );
          _nwords++;
          _checksum += word;
          status = store_event();
        } */

      }       
      // Yun-Tse 2015/1/20: Add the option "last_word_class == FEM::CHANNEL_HEADER"
      // for the case that the last channel has no data after 0-suppression
      else if ( last_word_class == FEM::CHANNEL_HEADER ) {

        // Store and clear
        store_ch_data();

        // Set the new channel info
        _channel_number_holder = (word & 0x3f);
        _readout_frame_number_holder = ( ( word >> 6) & 0x3f );

        if(_verbosity[MSG::DEBUG])
          Message::send( MSG::DEBUG,__FUNCTION__, 
                         Form("New channel number: %d, New frame number: %d", _channel_number_holder, _readout_frame_number_holder ) ); 

      }
      // Check if the last word was an FEM header, i.e. this is the first channel
      // in a FEM
      else if ( last_word_class == FEM::FEM_HEADER ) {
        if(_verbosity[MSG::DEBUG])
          Message::send(MSG::INFO,__FUNCTION__, "word = channel header, last word = FEM header");

        // New data starts here.
        // Clear data content & assign channel number.
        _ch_data.clear_data();

        _channel_number_holder = (word & 0x3f);
        _readout_frame_number_holder = ( ( word >> 6) & 0x3f );

        if(_verbosity[MSG::DEBUG])
          Message::send(MSG::DEBUG,__FUNCTION__,
                        Form("New channel header: %d, New frame number: %d", _channel_number_holder, _readout_frame_number_holder ) );

      } else {

	Message::send(MSG::ERROR,__FUNCTION__,
		      Form("Unexpected channel header (%x)! Last word = %x",word,last_word));

	status = false;

      }

      break;

    // Yun-Tse 2015/1/26: I am not sure if this will happen in the current format...
    /* case FEM::EVENT_LAST_WORD: {
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
    } */

    case FEM::CHANNEL_TIME: {
      if ( ( last_word_class != FEM::CHANNEL_HEADER ) && ( last_word_class != FEM::CHANNEL_PACKET_LAST_WORD ) ) {

        status = false;
        Message::send(MSG::ERROR,__FUNCTION__,
                    Form("Unexpected channel time word (%x) with the previous word %x (word type %d)!", word, last_word, last_word_class ) );
      } else {

        _ch_data.set_readout_sample_number( (word & 0xfff) );
      }
      break;
    }

    case FEM::CHANNEL_PACKET_LAST_WORD: {

      status = decode_ch_word( ( word & 0xfff ), last_word );

      // Store the channel data
      store_ch_data();

      break;
    }

    case FEM::UNDEFINED_WORD:
      Message::send( MSG::WARNING, __FUNCTION__, Form("Expected 16-bit zero-padding, last word 0x%x.", last_word) );
      break;

    default:

      status = decode_ch_word(word,last_word);
      if ( !status ) Message::send( MSG::ERROR, __FUNCTION__, Form("Error in Event 0x%x, Channel 0x%x, Readout sample number 0x%x", _header_info.event_number-1, _channel_number_holder, _ch_data.readout_sample_number_RAW() ) );
    }

    if(status && _header_info.nwords){

      _nwords++;
      _checksum += word;

    }    

    last_word = word;

    return status;
  }
  

  void algo_sn_tpc_huffman::store_ch_data() {
    // Save
    _ch_data.set_module_id( _header_info.module_id );
    _ch_data.set_module_address( _header_info.module_address );
    _ch_data.set_channel_number( _channel_number_holder );
    _ch_data.set_readout_frame_number( _readout_frame_number_holder );
    _event_data->push_back( _ch_data );
    // Message::send( MSG::INFO, __FUNCTION__, Form("Ch 0x%x, stored %zu channels", _channel_number_holder, _event_data->size() ) );

    // Clear
    _ch_data.clear_data();
    return;

  }
}
