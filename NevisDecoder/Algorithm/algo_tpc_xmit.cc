#ifndef ALGO_TPC_XMIT
#define ALGO_TPC_XMIT

#include "algo_tpc_xmit.hh"

namespace larlight {

  //#########################################
  algo_tpc_xmit::algo_tpc_xmit() 
    : algo_fem_decoder_base(),
      _ch_data(DATA::TPCFIFO)
  {
  //#########################################

    reset();

  }

  //#########################################
  void algo_tpc_xmit::reset() {
  //#########################################

    _event_data = 0;

    _ch_data.clear_data();

    algo_fem_decoder_base::reset();

  }

  //#########################################
  bool algo_tpc_xmit::process_ch_word(const UInt_t word, 
				      UInt_t &last_word) 
  {
  //#########################################

    bool status=true;

    if(_verbosity[MSG::DEBUG]){
      sprintf(_buf,"Processing ch-word: %x",word);
      Message::send(MSG::DEBUG,__FUNCTION__,_buf);
    }

    FEM::FEM_WORD word_class=get_word_class(word);
    FEM::FEM_WORD last_word_class=get_word_class(last_word);

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

    case FEM::CHANNEL_LAST_WORD:

      //
      // Check if the last word was channel word
      //
      if(last_word_class == FEM::CHANNEL_WORD) {

	// Check if the channel number in this ch-last-word agrees with that in the header
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

      }
      else{

	Message::send(MSG::ERROR,__FUNCTION__,
		      Form("Unexpected channel ending (%x)! Last word = %x",word,last_word));

	status = false;

      }

      break;

    case FEM::CHANNEL_WORD:

      //
      // Check if the last word was channel header or channel adc word
      //
      if(last_word_class == FEM::CHANNEL_HEADER ||
	 last_word_class == FEM::CHANNEL_WORD){

	status = decode_ch_word(word,last_word);

      }else{

	Message::send(MSG::ERROR,__FUNCTION__,
		      Form("Unexpected channel word (%x)! Last word = %x",word,last_word));

	status = false;

      }

      break;

    case FEM::EVENT_LAST_WORD:
    case FEM::FEM_LAST_WORD:
    case FEM::UNDEFINED_WORD:
    case FEM::EVENT_HEADER:
    case FEM::FEM_HEADER:
    case FEM::FEM_FIRST_WORD:

      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Unexpected word found while channel word processing: %x",word));

      status = false;

      break;
    }

    // If processing of this word is successful, add it to the checksum
    if(status) {

      _nwords++;
      _checksum += word;
      //std::cout<<_nwords<<" ... "<<_checksum<<std::endl;
    }else{

      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Failed processing channel word %x (previous=%x)",word,last_word));

      clear_event();
    }

    if(word_class != FEM::UNDEFINED_WORD)

      last_word = word;

    return status;
  }

  //#########################################################
  bool algo_tpc_xmit::check_event_quality(){
  //#########################################################

    bool status = true;

    // Check if _checksum and _nwords agrees with that of event header.
    //if(_nwords != _header_info.nwords) {
    _nwords-=1;
    if(_nwords!=_header_info.nwords){

      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Disagreement on nwords: counted=%u, expected=%u",_nwords,_header_info.nwords));

      status = false;

    }

    //if(_checksum != _header_info.checksum) {
    if((_checksum & 0xffffff) !=_header_info.checksum){

      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Disagreement on checksum: summed=%x, expected=%x",_checksum,_header_info.checksum));

      status = false;

    }

    return status;

  }



  //#################################################
  bool algo_tpc_xmit::process_event_header(const UInt_t word, 
					   UInt_t &last_word) 
  {
  //#################################################

    bool status = true;

    if(!_event_data)

      _event_data = (event_fifo*)(_storage->get_data(DATA::TPCFIFO));

    if(get_word_class(last_word)==FEM::EVENT_LAST_WORD) {

      // Attempt to store data

      status = store_event();
    }else if(last_word != FEM::INVALID_WORD){

      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Unexpected word (%x, previous=%x) while processing event header!",word,last_word));

      status = false;

    }

    last_word = word;

    return status;

  }

  //#########################################################
  bool algo_tpc_xmit::process_fem_last_word(const UInt_t word,
					    UInt_t &last_word)
  {
  //#########################################################

    // This should not exist in TPC-XMIT
    Message::send(MSG::ERROR,__FUNCTION__,
		  Form("FEM-LAST-WORD (%x, previous=%x) ... NOT expected for TPC!",word,last_word));

    last_word = word;

    return false;
  }

  //#########################################################
  bool algo_tpc_xmit::process_event_last_word(const UInt_t word,
					      UInt_t &last_word)
  {  
  //#########################################################

    if(_verbosity[MSG::INFO]){

      Message::send(MSG::INFO,__FUNCTION__,
		    Form("End of event word: %x...",word));

    }

    last_word = word;

    return true;
  }


  //#########################################################
  void algo_tpc_xmit::clear_event(){
  //#########################################################

    algo_fem_decoder_base::clear_event();

    _event_data->clear_data();

    _ch_data.clear();

  }

  //#########################################################
  bool algo_tpc_xmit::store_event(){
  //#########################################################

    bool status = check_event_quality();

    // Store if condition is satisfied
    if(status) {

      if(_verbosity[MSG::INFO]){

	Message::send(MSG::INFO,__FUNCTION__,
		      Form("Storing event %u with %zu channel entries...",
			   _event_data->event_id(), _event_data->size()));

      }

      _event_data->set_module_address    ( _header_info.module_address    );
      _event_data->set_module_id         ( _header_info.module_id         );
      _event_data->set_event_id          ( _header_info.event_id          );
      _event_data->set_event_frame_id    ( _header_info.event_frame_id    );
      _event_data->set_trigger_frame_id  ( _header_info.trigger_frame_id  );
      _event_data->set_trigger_timeslice ( _header_info.trigger_timeslice );
      _event_data->set_nwords            ( _header_info.nwords            );
      _event_data->set_checksum          ( _header_info.checksum          );

      status = _storage->next_event();

    }else{

      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Skipping to store event %d...",_header_info.event_id));

      status = false;

    }

    clear_event();

    return status;
  }

  //#########################################################
  bool algo_tpc_xmit::decode_ch_word(const UInt_t word, 
				     UInt_t &last_word)
  {
  //#########################################################

    bool status = true;
    // Simply append if it is not compressed
    if( !(is_compressed(word)) ) _ch_data.push_back( (word & 0xfff) );

    else if(!(_ch_data.size())){

      // This is a problem: if huffman coded, then we must have a previous ADC sample
      // as a reference. Raise an error.

      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Huffman coded word %x found while the previous was non-ADC word (%x)!",
			 word,last_word));

      status = false;

    }
    else{

      // Compresed data is in last 15 bit of this word.
      UInt_t data = (word & 0x7fff);

      // The compression is based on an extremely simple Huffman encoding.
      // The Huffman tree used here assigns the following values:
      //
      // Value   Code
      //  +3     0000001
      //  +2     00001
      //  +1     001
      //  +0     1
      //  -1     01
      //  -2     0001
      //  -3     000001

      /*
      size_t zero_count     = 0;
      for(size_t index=0; index<15 && status; ++index){

	if( !((data >> index) & 0x1) ) zero_count++;
	else{

	  switch(zero_count){

	  case 0:
	    _ch_data.push_back( (*(_ch_data.rbegin())) ); break;	  

	  case 1:
	    _ch_data.push_back( (*(_ch_data.rbegin())) -1 ); break;

	  case 2:
	    _ch_data.push_back( (*(_ch_data.rbegin())) +1 ); break;

	  case 3:
	    _ch_data.push_back( (*(_ch_data.rbegin())) -2 ); break;

	  case 4:
	    _ch_data.push_back( (*(_ch_data.rbegin())) +2 ); break;

	  case 5:
	    _ch_data.push_back( (*(_ch_data.rbegin())) -3 ); break;

	  case 6:
	    _ch_data.push_back( (*(_ch_data.rbegin())) +3 ); break;

	  default:
	    Message::send(MSG::ERROR,__FUNCTION__,
			  Form("Encountered unexpected number of zeros (=%zu) in the compressed word %x!",
			       zero_count,word));
	    status = false;
	  };

	  zero_count=0;
	}
      }
      */

      size_t zero_count = 0;
      bool   non_zero_found = false;
      for(size_t index=0; index<15 && status; ++index){

	if( !((data >> index) & 0x1) )

	  zero_count += ( non_zero_found ? 1 : 0 );

	else{

	  if(!non_zero_found) non_zero_found=true;

	  else status = add_huffman_adc(_ch_data,zero_count);

	  if(status) zero_count=0;
	}
      }

      if(zero_count && status){

	status = add_huffman_adc(_ch_data,zero_count);

      }    

      if(!status)

	Message::send(MSG::ERROR,__FUNCTION__,
		      Form("Encountered unexpected number of zeros (=%zu) in the compressed word %x!",
			   zero_count,word));

    }

    return status;

  }
}

#endif
