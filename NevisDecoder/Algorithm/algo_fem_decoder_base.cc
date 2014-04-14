#ifndef ALGO_FEM_DECODER_BASE
#define ALGO_FEM_DECODER_BASE

#include "algo_fem_decoder_base.hh"

namespace larlight {

  //#################################################
  algo_fem_decoder_base::algo_fem_decoder_base() 
    : algo_base() 
  //#################################################
  {
    _name = "algo_fem_decoder_base";
    reset();
  }


  //#################################################
  void algo_fem_decoder_base::reset() {
  //#################################################

    algo_base::reset();

    _last_word             = FEM::INVALID_WORD;

    _event_header_count    = 0;

    _search_for_next_event = false;

    for(size_t i=0; i< FEM_HEADER_COUNT; i++)

      _event_header_words[i] = 0;

  }


  //#################################################
  bool algo_fem_decoder_base::process_word(UInt_t word)
  //#################################################
  {
    //std::cout<<Form("%x",word)<<std::endl;
    // 
    // A simple function to call other members depending on the status.
    // Let other members to deal with expected/unexpected case
    //

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

    bool status=true;
    UInt_t word_class=get_word_class(word);
    UInt_t last_word_class=get_word_class(_last_word);
    //
    // Skip this word if a boolean is set to skip to the next event header
    //
    if( _search_for_next_event &&  
	word_class == FEM::FEM_HEADER &&
	last_word_class != FEM::FEM_HEADER ) 

      _search_for_next_event = false;


    if( _search_for_next_event ) {

      if(_verbosity[MSG::INFO])

	Message::send(MSG::INFO,__FUNCTION__,
		      Form("Skipping a word (%x, previous=%x) to the next event..",word,_last_word));

      _last_word = FEM::INVALID_WORD;

      return true;
    }

    switch(word_class){

    case FEM::EVENT_HEADER:

      // (1) Call process_event_header()
      status = process_event_header(word,_last_word);
      break;

    case FEM::FEM_HEADER:

      // (1) Check if the last word was EVENT_HEADER, FEM_HEADER, or EVENT_LAST_WORD
      //     - If EVENT_HEADER, continue to (2)
      //     - If FEM_HEADER,   continue to (2)
      //     - If EVENT_LAST_WORD, attempt to store & continue to (2)
      // (2) Call process_fem_header()
      status = process_fem_header(word,_last_word);
      break;

    case FEM::FEM_FIRST_WORD:
    case FEM::CHANNEL_HEADER:
    case FEM::CHANNEL_WORD:
    case FEM::CHANNEL_LAST_WORD:
    case FEM::FEM_LAST_WORD:
      {
	// (1) Call process_ch_word()
	//     - Children class should implement what should be checked in correlation to the last word.

	// Split two 16 bit words
	UInt_t first_word  = (word & 0xffff);
	UInt_t second_word = (word >> 16);

	if(status) status = process_ch_word(first_word,_last_word);

	// Check if the left 16-bit word is also the relevant type or not
	if(status){

	  switch(get_word_class(second_word)){
	  case FEM::FEM_FIRST_WORD:
	  case FEM::CHANNEL_HEADER:
	  case FEM::CHANNEL_WORD:
	  case FEM::CHANNEL_LAST_WORD:
	    status = process_ch_word(second_word,_last_word);
	    break;
	  case FEM::FEM_LAST_WORD:
	    status = process_fem_last_word(second_word,_last_word);
	    break;
	  default:
	    status = false;
	    Message::send(MSG::ERROR,__FUNCTION__,
			  Form("Unexpected word (%x) while processing channel data (previous=%x)",second_word,first_word));
	  }

	}
	break;
      }
    case FEM::EVENT_LAST_WORD:
      // (1) Call process_event_last_word()
      status = process_event_last_word(word,_last_word);
      break;

    case FEM::UNDEFINED_WORD: 


      if(word != 0x0 || (last_word_class != FEM::EVENT_LAST_WORD && last_word_class != FEM::FEM_LAST_WORD) ) {

	Message::send(MSG::ERROR,__FUNCTION__,
		      Form("Undefined word: %x (previous = %x)",word,_last_word));

	status = false;
      }else if(_verbosity[MSG::INFO]){

	// This happens sometimes according to Chi 10/01/13
	Message::send(MSG::INFO,__FUNCTION__,
		      Form("Padding of undefined word (tolerated): %x (previous=%x)",word,_last_word));
      }
      break;
    }

    if(!status){

      backtrace();

      if(_debug_mode) {

	if(_header_info.event_number>0)

	  Message::send(MSG::WARNING,__FUNCTION__,Form("Failed decoding event %d ...",_header_info.event_number));

	Message::send(MSG::WARNING,__FUNCTION__,"DEBUG MODE => Continue to the next event...\n");

	_search_for_next_event = true;
	clear_event();
      }
    }

    return status;
  }

  //#################################################
  bool algo_fem_decoder_base::process_fem_header(const UInt_t word, UInt_t &last_word) {
  //#################################################

    bool status=true;
    //
    // Check if this is an event header word or not
    //
    FEM::FEM_WORD word_class=get_word_class(word);
    if(_verbosity[MSG::DEBUG]){
      sprintf(_buf,"Processing Header: %x",word);
      Message::send(MSG::DEBUG,__FUNCTION__,_buf);
    }


    if( word_class != FEM::FEM_HEADER) {

      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Encountered unexpected word while an event header search: %x (word type=%d)",
			 word,word_class) );
      status = false;    

    }else if(get_word_class(word>>16)!=FEM::FEM_HEADER) {

      // Event header should come as a 32-bit word which is a pair of two 16-bit header words.
      // The first 16-bit is already checked by this point. Check the second part.

      Message::send(MSG::ERROR,__FUNCTION__,Form("Found an odd event header word: %x",word));

      status = false;

    }

    if(status) {

      //
      // Check the last word type
      //
      switch(get_word_class(last_word)){
      case FEM::EVENT_HEADER:      
      case FEM::FEM_HEADER:
      case FEM::FEM_LAST_WORD:
	// Expected. Nothing to do
	break;
      case FEM::FEM_FIRST_WORD:
      case FEM::CHANNEL_HEADER:
      case FEM::CHANNEL_WORD:
      case FEM::UNDEFINED_WORD:
      case FEM::EVENT_LAST_WORD:
	// ERROR
	Message::send(MSG::ERROR,__FUNCTION__,
		      Form("Unexpected word while FEM_HEADER processing: %x (previous word=%x)",word,last_word));
	status = false;
	break;
      case FEM::CHANNEL_LAST_WORD:
	// Store data
	status = store_event();
	if(_debug_mode) {
	  // We have to handle this case separately from a uniifed handling @ process_word
	  // Because "this" header word is still good. Make a warning to a user.

	  Message::send(MSG::WARNING,__FUNCTION__,"DEBUG MODE => Continue to the next event...\n");

	  clear_event();

	  status = true; 
	}
	break;
      }

      if(status){

	// Process the subject word as an event header
	if (_event_header_count<FEM_HEADER_COUNT) {

	  // Store header words
	  _event_header_words[_event_header_count]=word;
	  _event_header_count++;

	  // If stored number of header words reached to the expected number, decode.
	  if(_event_header_count==FEM_HEADER_COUNT) {

	    // Decode header words
	    status = decode_fem_header(_event_header_words);

	  }
	}else{

	  // Raise error if a header word count > set constant (should not happen)
	  Message::send(MSG::ERROR,__FUNCTION__,
			Form("Logic error: event header word counter not working! (%zu/%zu)",
			     _event_header_count,FEM_HEADER_COUNT));
	  status=false;
	}
      }
    }

    if(!status){

      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Failed processing the word %x (last word %x) as an event header!",word,last_word));

    }

    last_word = word;
    return status;  

  }


  //#################################################
  bool algo_fem_decoder_base::decode_fem_header(const UInt_t *event_header){
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

    // (3) get module ID number ... upper 7 bit of 12 bits
    _header_info.module_id      = ( (event_header[0]>>16 & 0xfff)>>5 & 0x7f);

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


  #ifdef INCLUDE_EXTRA_HEADER

    _header_info.fem_trig_frame_number  = ( ((event_header[5] & 0xfff)>>3 & 0xf) +
					    (((_header_info.event_frame_number)>>3)<<3));

    // Correct for a roll over
    _header_info.fem_trig_frame_number  = round_diff(_header_info.event_frame_number, _header_info.fem_trig_frame_number, 0x7);

    _header_info.fem_trig_sample_number = ((((event_header[5]>>16) & 0xf)<<8) + (event_header[5] & 0xff));

  #endif

    // Report if verbosity is set.
    if(_verbosity[MSG::INFO])
      {
	std::string msg;
	for(size_t i=0; i<FEM_HEADER_COUNT; ++i)
	  msg += Form("%x ", event_header[i]);
	Message::send(MSG::INFO,Form("Decoded Header: %s",msg.c_str()));
	Message::send(MSG::INFO,Form("Module %d (ID=%d)", _header_info.module_address, _header_info.module_id));
	Message::send(MSG::INFO,Form("Event ID %d",_header_info.event_number));
	Message::send(MSG::INFO,Form("Frame ID %d",_header_info.event_frame_number));
	Message::send(MSG::INFO,Form("Number of Words = %d",_header_info.nwords));
	Message::send(MSG::INFO,Form("Checksum = %x", _header_info.checksum));
	Message::send(MSG::INFO,Form("Trigger Frame %d",_header_info.fem_trig_frame_number));
	Message::send(MSG::INFO,Form("Trigger Sample %d",_header_info.fem_trig_sample_number));
      }

    _checksum=0;

    _nwords=0;

    return status;
  }


  //#################################################
  UInt_t algo_fem_decoder_base::round_diff(UInt_t ref_id, 
					   UInt_t subject_id, 
					   UInt_t diff) const
  //#################################################
  {
    // Used to recover pmt/trigger frame id from roll over effect.
    // One can test this by simply calling this function.
    // For instance, to test the behavior for a roll-over of 0x7 ...
    //
    // > root
    // root[0] gSystem->Load("libDecoder")
    // root[1] algo_slow_readout_decoder k
    // root [6] k.get_pmt_frame(583,584,0x7)
    // (const unsigned int)584
    // root [7] k.get_pmt_frame(584,583,0x7)
    // (const unsigned int)583
    //
    // I think this implementation works. ... Aug. 12 2013
    // No... Kazu Apr 14 2014
    if( (subject_id > ref_id) && ((subject_id-ref_id) >= diff) )
      return subject_id - (diff+1);
    else if( (ref_id > subject_id) && ((ref_id-subject_id) >= diff) )	   
      return subject_id + (diff+1);
    else
      return subject_id;

  }

  //#################################################
  bool algo_fem_decoder_base::add_huffman_adc(std::vector<UShort_t> &wf, size_t zero_count) 
  {
  //#################################################

    bool status = true;

    switch(zero_count){

    case 0:
      wf.push_back( (*(wf.rbegin())) ); break;	  

    case 1:
      wf.push_back( (*(wf.rbegin())) -1 ); break;

    case 2:
      wf.push_back( (*(wf.rbegin())) +1 ); break;

    case 3:
      wf.push_back( (*(wf.rbegin())) -2 ); break;

    case 4:
      wf.push_back( (*(wf.rbegin())) +2 ); break;

    case 5:
      wf.push_back( (*(wf.rbegin())) -3 ); break;

    case 6:
      wf.push_back( (*(wf.rbegin())) +3 ); break;

    default:

      std::cout<<zero_count<<std::endl;

      status = false;
    }

    return status;

  }
}

#endif
