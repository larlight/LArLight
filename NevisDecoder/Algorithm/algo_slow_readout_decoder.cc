#ifndef ALGO_SLOW_READOUT_DECODER_CC
#define ALGO_SLOW_READOUT_DECODER_CC

#include "algo_slow_readout_decoder.hh"

namespace larlight {

  algo_slow_readout_decoder::algo_slow_readout_decoder() :
    algo_base(),
    _ch_data()
  {
    _name="algo_slow_readout_decoder";
    _last_word=FEM::INVALID_WORD;
    _print_adcval=false;
    _continue_to_next_event=false;
    _process=READ_HEADER;
    _event_data=0;
    set_verbosity(MSG::NORMAL);

    reset();

  }

  void algo_slow_readout_decoder::reset(){

    init_ch_info();
    init_event_info();
    _event_data=0;

  }

  void algo_slow_readout_decoder::init_ch_info(){

    _channel_header_count=0;
    _ch_data.clear_data();

  }

  void algo_slow_readout_decoder::init_event_info(){

    _event_header_count=0;
    _last_channel_number=FEM::INVALID_CH;
    _last_disc_id=FEM::DISC_MAX;
    _beam_event=false;
    _beam_ref_sample=FEM::INVALID_WORD;
    _beam_ref_nwords=FEM::INVALID_WORD;
    _beam_ref_frame=FEM::INVALID_WORD;
    _process=READ_HEADER;
    algo_base::init_checker_info();

  }

  void algo_slow_readout_decoder::print_adc_values()
  {
    sprintf(_buf,"Printing ADC values for Event=%d Channel=%d",
	    _event_data->event_number(),
	    _ch_data.channel_number());
    Message::send(MSG::INFO,_buf);
    size_t ctr=0;
    std::string line("");
    for(fifo::iterator iter(_ch_data.begin());
	iter!=_ch_data.end();
	++iter)
      {
	sprintf(_buf," %-6d",(*iter));
	line+=_buf;
	ctr++;
	if(ctr%8==0) {
	  Message::send(MSG::INFO,line.c_str());
	  line="";
	}
      }
  }

  bool algo_slow_readout_decoder::process_word(UInt_t word) {

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

    // IF data pointer is not set, set
    if(!_event_data) {
      _event_data=(event_pmtfifo*)(_storage->get_data(DATA::PMTFIFO));
      if(!_event_data) {
	Message::send(MSG::ERROR,__FUNCTION__,"Could not retrieve pmt_wf_collection poitner!");
	return false;
      }
    }

    bool status=true;
    switch(_process){
    case READ_HEADER:
      //
      // hand over 32-bit word as it is for processing event header
      //
      status=process_header(word);
      break;
    case READ_CHANNEL:
      //
      // Exceptional case handling: if word type is event header,
      // warn a user & process it as a header
      //
      UInt_t word_class=get_word_class(word);
      if(word_class==FEM::FEM_HEADER || 
	 word_class==FEM::EVENT_HEADER) {
	sprintf(_buf,"Found an event header (%x) while looping over channel data (last word: %x)!",
		word,_last_word);
	Message::send(MSG::ERROR,__FUNCTION__,_buf);

	if(_debug_mode) {
	  // Check data quality: return is not relevant as this is debug mode.
	  //if(_beam_event) apply_beamgate_correction();
	  status = check_event_quality();

	  init_event_info();
	  // Process this as a header 
	  if(!status){
	    sprintf(_buf,"DEBUG mode-> continue ... but Skip saving event: %d ",_event_data->event_number());
	    Message::send(MSG::WARNING,__FUNCTION__,_buf);
	  }else{
	    sprintf(_buf,"DEBUG mode-> continue ... Good checksum -> stored previous event: %d",_event_data->event_number());
	    Message::send(MSG::WARNING,__FUNCTION__,_buf);
	    _storage->next_event();
	  }
	  status=process_header(word);

	  // As this is debug mode, we let the run continiue
	  status=true;
	}
	else
	  status=false;
	break;
      }
      //
      // hand over 2 of 16-bit words for channel data
      //
      UInt_t first_word  = word & 0xffff;
      UInt_t second_word = word >> 16; 
      status = process_ch_word(first_word,_last_word);
      if(status && _process==READ_CHANNEL)
	status=process_ch_word(second_word,_last_word);
      else if(!status){
	// Something went wrong in processing channel word.
	// If _debug_mode is set true, we let it continue to
	// the next event.
	if(_debug_mode) {
	  status=true;
	  _continue_to_next_event=true;
	  _process=READ_HEADER;
	}
      }
      break;
    }
    return status;
  }

  bool algo_slow_readout_decoder::process_header(const UInt_t word) {

    bool status=true;
    //
    // Handle the case this is not a header word
    //
    FEM::FEM_WORD word_class=get_word_class(word);
    if(_verbosity[MSG::DEBUG]){
      sprintf(_buf,"Processing Header: %x",word);
      Message::send(MSG::DEBUG,__FUNCTION__,_buf);
    }
    switch(word_class){

      // Ommit undefined word
    case FEM::UNDEFINED_WORD:
      sprintf(_buf,"Skipping undefined word while searching an event header: %x",word);
      Message::send(MSG::WARNING,__FUNCTION__,_buf);
      break;

      // Raise error for non-header words
    case FEM::FEM_FIRST_WORD:
    case FEM::CHANNEL_HEADER:
    case FEM::CHANNEL_WORD:
    case FEM::CHANNEL_LAST_WORD:
    case FEM::FEM_LAST_WORD:
    case FEM::EVENT_LAST_WORD:
      if(!_continue_to_next_event){
	sprintf(_buf,"Encountered unexpected word while an event header search: %x (word type=%d)",
		word,word_class);
	Message::send(MSG::ERROR,__FUNCTION__,_buf);
	if(!_debug_mode)
	  status=false;
      }else if(_verbosity[MSG::WARNING]){
	sprintf(_buf,"Skipping non-header till next event (debug mode): %x",word);
	Message::send(MSG::WARNING,__FUNCTION__,_buf);
      }    
      break;

      // Header word -> store & process
    case FEM::EVENT_HEADER:
      // This is just a marker. Nothing really to be done.
      if(_continue_to_next_event) _continue_to_next_event=false;
      break;
    case FEM::FEM_HEADER:
      if(_continue_to_next_event) _continue_to_next_event=false;

      // Event header should come as a 32-bit word which is a pair of two 16-bit header words.
      // The first 16-bit is already checked by this point. Check the second part.
      if(get_word_class(word>>16)!=FEM::FEM_HEADER) {
	sprintf(_buf,"Found an odd event header word: %x",word);
	Message::send(MSG::ERROR,__FUNCTION__,_buf);
	status=false;
	break;
      }

      if (_event_header_count<FEM_HEADER_COUNT) {

	// Store header words
	_event_header_words[_event_header_count]=word;
	_event_header_count++;

	if(_event_header_count==FEM_HEADER_COUNT) {

	  // Process header words
	  if(decode_event_header(_event_header_words)) {
	    _event_header_count=0;
	    _process=READ_CHANNEL;
	    status=true;
	  }
	  else
	    status=false;
	}
      }
      else {
	// Raise error if a header word count > set constant (should not happen)
	Message::send(MSG::ERROR,__FUNCTION__,
		      "Logic error: event header word counter not working!");
	status=false;
      }
      break;
    }
    _last_word=word;

    return status;  
  }

  bool algo_slow_readout_decoder::decode_event_header(const UInt_t *event_header){

    bool status=true;
    //
    // Get event information
    //

    // (1) check if the very first 16-bit word is as expected
    if(!( event_header[0] & 0xffff))
      Message::send(MSG::ERROR,"Unexpected first word in event headers!");

    if(!status) return status;

    // (2) get module address ... lowest 5 of 12 bits
    //  _event_data->set_module_address(( event_header[0]>>16 & 0xfff ) & 0x1f);
    _event_data->set_module_address(( event_header[0]>>16 & 0xfff ) & 0x1f);

    // (3) get module ID number ... upper 7 bit of 12 bits
    _event_data->set_module_id((event_header[0]>>16 & 0xfff)>>5 & 0x7f);

    // (4) get number of 16-bit words to be read in this event.
    // Lower 12 bits of two 16-bit words.
    // The very last "last word for channel info" is not included in this.
    // For later checking purpose, increment by 1.
    _event_data->set_nwords( (( (event_header[1]>>16) & 0xfff ) + ((event_header[1] & 0xfff)<<12)) );

    // (5) get event ID
    // Lower 12 bits of two 16-bit words.
    _event_data->set_event_number( (( (event_header[2]>>16) & 0xfff ) + ((event_header[2] & 0xfff)<<12)));

    // (6) get frame ID
    // Lower 12 bits of two 16-bit words.
    _event_data->set_event_frame_number((( (event_header[3]>>16) & 0xfff ) + ((event_header[3] & 0xfff)<<12)));

    // (7) get checksum
    _event_data->set_checksum( (( (event_header[4]>>16) & 0xfff ) + ((event_header[4] & 0xfff)<<12)) );


  #ifdef INCLUDE_EXTRA_HEADER
    _event_data->set_fem_trig_frame_number( (((event_header[5]>>16) & 0xfff)>>4 & 0xf) +
				       (((_event_data->event_frame_number())>>4)<<4) ); 
    // Correct for a roll over
    _event_data->set_fem_trig_frame_number( round_diff(_event_data->event_frame_number(), 
						  _event_data->fem_trig_frame_number(),
						  0x7));
    _event_data->set_fem_trig_sample_number( (((event_header[5]>>16) & 0xf)<<8) + (event_header[5] & 0xff) );
  #endif

    // Report if verbosity is set.
    if(_verbosity[MSG::INFO])
      {
	sprintf(_buf,"Module %d (ID=%d)", _event_data->module_address(), _event_data->module_id());
	Message::send(MSG::INFO,_buf);
	sprintf(_buf,"Event ID %d",_event_data->event_number());
	Message::send(MSG::INFO,_buf);
	sprintf(_buf,"Frame ID %d",_event_data->event_frame_number());
	Message::send(MSG::INFO,_buf);
	sprintf(_buf,"Number of Words = %d",_event_data->nwords());
	Message::send(MSG::INFO,_buf);
	sprintf(_buf,"Checksum = %x", _event_data->checksum());
	Message::send(MSG::INFO,_buf);
	sprintf(_buf,"Trigger Frame %d",_event_data->fem_trig_frame_number());
	Message::send(MSG::INFO,_buf);
	sprintf(_buf,"Trigger Sample %d",_event_data->fem_trig_sample_number_16MHz());
	Message::send(MSG::INFO,_buf);
      }

    _checksum=_event_data->checksum();
    _nwords=_event_data->nwords();

    return status;
  }

  bool algo_slow_readout_decoder::process_ch_word(const UInt_t word,
						  UInt_t &last_word){
    // extract information
    FEM::FEM_WORD last_word_class=get_word_class(last_word);
    FEM::FEM_WORD word_class=get_word_class(word);

    // Print out DEBUG information
    if(_verbosity[MSG::DEBUG]){
      sprintf(_buf,"Processing: %x ... Previous: %x ... Remaining: %d words, checksum=%d",
	      word,last_word,_nwords,_checksum);
      Message::send(MSG::DEBUG,__FUNCTION__,_buf);
    }

    if(word_class==FEM::UNDEFINED_WORD){
      sprintf(_buf,"Found an undefined word: %x ",word);
      if(last_word_class!=FEM::CHANNEL_LAST_WORD && 
	 last_word_class!=FEM::FEM_LAST_WORD &&
	 last_word_class!=FEM::EVENT_LAST_WORD)
	Message::send(MSG::WARNING,__FUNCTION__,_buf);
      else{ 
	if(_verbosity[MSG::INFO])
	  Message::send(MSG::INFO,__FUNCTION__,_buf);
      }
      return true;
    }

    bool status = decode_ch_word(word,last_word_class);

    if(!status){
      // Something went wrong in decode_ch_word function
      sprintf(_buf,"Status failure while analyzing: %x ... (previous word: %x)",word,last_word);
      Message::send(MSG::ERROR,__FUNCTION__,_buf);
    }else{
      // subtract # words & check-sum
      if(word_class==FEM::EVENT_LAST_WORD){
	// This is the last sample for this event.
	if(_verbosity[MSG::INFO]){
	  sprintf(_buf,"Found the last event word: %x",word);
	  Message::send(MSG::INFO,__FUNCTION__,_buf);
	}
	//if(_beam_event) apply_beamgate_correction();
	status = check_event_quality();
	if(status)
	  _storage->next_event();
	else if(_debug_mode){
	  sprintf(_buf,"DEBUG mode-> continue ... but Skip saving event: %d ",_event_data->event_number());
	  Message::send(MSG::WARNING,__FUNCTION__,_buf);
	  status=true;
	}
	init_event_info();
      }else if(word_class!=FEM::FEM_LAST_WORD){
	_nwords-=1;
	_checksum-=word;
      }

    }

    last_word=word;
    return status;
  }


  UInt_t algo_slow_readout_decoder::round_diff(UInt_t ref_id, 
					       UInt_t subject_id, 
					       UInt_t diff) const
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
    if( (subject_id > ref_id) && ((subject_id-ref_id) >= diff) )
      return subject_id - (diff+1);
    else if( (ref_id > subject_id) && ((ref_id-subject_id) >= diff) )	   
      return subject_id + (diff+1);
    else
      return subject_id;

    // Following part is provided by David & Georgia for a specific implementation for PMT... Aug. 12 2013
    // But I don't think this works right. 
    // -Kazu Aug. 12 2013
    /*
    UInt_t correct_frame= ( ( event_frame_number & (~0x7) ) | ( readout_frame_number & 0x7 ) );
    if( (correct_frame-event_frame_number)<-1 )
      return correct_frame+8;
    else if( (correct_frame-event_frame_number)>2 )
      return correct_frame-8;
    else 
      return correct_frame;
    */

  }

  void algo_slow_readout_decoder::apply_beamgate_correction() {

    for(event_pmtfifo::iterator iter(_event_data->begin());
	iter!=_event_data->end();
	++iter){

      if( (*iter).size()>BEAM_NWORDS ) {

	//
	// Case 1: pulse comes before the beamgate but in the same frame
	//
	if( (*iter).readout_sample_number_64MHz() < _beam_ref_sample && (*iter).readout_frame_number() == _beam_ref_frame )

	  // shift the sample number by diff of the sample size
	  (*iter).set_readout_sample_number( _beam_ref_sample + _beam_ref_nwords - (*iter).size() );

	//
	// Case 2: pulse comes before the beamgate and in the previous frame
	//
	if( (*iter).readout_frame_number() < _beam_ref_frame ) {

	  // shift the sample number by diff of the sample size first
	  (*iter).set_readout_sample_number( _beam_ref_sample + _beam_ref_nwords - (*iter).size() );

	  // if this sample number is exceeding the frame edge, change the frame number
	  if((*iter).readout_sample_number_64MHz() >= FRAME_WIDTH) {
	    (*iter).set_readout_sample_number((*iter).readout_sample_number_64MHz() - FRAME_WIDTH);
	    (*iter).set_readout_frame_number((*iter).readout_frame_number() + 1);
	  }
	}

	//
	// Case 3: pulse comes after the beamgate ... data should be readout correctly.
	//         Nothing to be done.
      }	    
    }

  }

  bool algo_slow_readout_decoder::check_event_quality() {

    if(_verbosity[MSG::INFO]){
      sprintf(_buf,"Exiting ch-loop in event: %d",_event_data->event_number());
      Message::send(MSG::INFO,__FUNCTION__,_buf);
      sprintf(_buf,"Number of channels read : %zd",_event_data->size());
      Message::send(MSG::INFO,__FUNCTION__,_buf);
    }

    bool status=true;

    if(_nwords>1){
      sprintf(_buf,"Number words expected - counted = %d",_nwords);
      Message::send(MSG::ERROR,__FUNCTION__,_buf);
      status=false;
    }
    if(_checksum & 0xffffff)
      _checksum -= 32768;

    if(_checksum & 0xffffff){
      sprintf(_buf,"Checksum expected - recieved = %d",_checksum);
      Message::send(MSG::ERROR,__FUNCTION__,_buf);
      status=false;
    }

    return status;
  }


  bool algo_slow_readout_decoder::decode_ch_word(const UInt_t word, 
						 const FEM::FEM_WORD last_word_class)  {
    
    FEM::FEM_WORD word_class = get_word_class(word);
    bool status=true;

    if(_verbosity[MSG::DEBUG]){
      sprintf(_buf,"Decoding word: %x ... last word class: %d",word,last_word_class);
      Message::send(MSG::DEBUG,__FUNCTION__,_buf);
    }  

    switch(word_class){

    case FEM::EVENT_HEADER:
    case FEM::FEM_HEADER:
      //
      // Found an event header: -> unexpected. This should be handled in process_word().
      // Return "false" if executed even in _debug_mode (as it's this program's logic error)
      sprintf(_buf,"Found an event header (%x) while processing channel data word!",word);
      Message::send(MSG::ERROR,__FUNCTION__,_buf);
      Message::send(MSG::ERROR,__FUNCTION__,"Terminate channel data readout loop.");
      status=false;
      break;

    case FEM::UNDEFINED_WORD:
      //
      // Found an undefined word: unexpected. This should be handled in process_ch_word().
      // Return "false" if executed even in _debug_mode=true... it's this program's logic error.
      sprintf(_buf,"Logic error: should not find an undefined word (%x) here.",word);
      Message::send(MSG::ERROR,__FUNCTION__,_buf);
      Message::send(MSG::ERROR,__FUNCTION__,"Terminate channel data readout loop.");
      status=false;
      break;

    case FEM::FEM_FIRST_WORD:
      //
      // First channel word in this event ... nothing special to do
      if(last_word_class!=FEM::FEM_HEADER){
	Message::send(MSG::ERROR,__FUNCTION__,
		      "Found the first word while the previous one was not event header!");
	status=false;
      }
      break;

    case FEM::CHANNEL_HEADER:
      //
      // Channel header ... read in channel info from this word
      if(last_word_class!=FEM::FEM_FIRST_WORD && last_word_class!=FEM::CHANNEL_LAST_WORD &&
	 !( (((*_event_data->rbegin()).size()) & 0x1) && _last_word==0x8000)) {
	Message::send(MSG::ERROR,__FUNCTION__,
		      "Found the channel header in wrong place: previous word is missing!");
	status=false;
      }else{
	// Initialize channel info, then fill with channel number & disc. id.
	init_ch_info();
	_ch_data.set_channel_number( word & 0x3f ); // Lower-most 6 bits of 12-bit word is channel number
	_ch_data.set_disc_id((FEM::DISCRIMINATOR)((word & 0xfff)>>9));  // Upper-most 2 bits of 12-bit word is discriminator ID
      }
      break;

    case FEM::CHANNEL_WORD:
    case FEM::CHANNEL_LAST_WORD:
      //
      // This is channel ADC count or possibly remaining channel header info
      // The first two sample are treated as channel header info
      if(last_word_class!=FEM::CHANNEL_HEADER && 
	 last_word_class!=FEM::CHANNEL_WORD &&
	 last_word_class!=FEM::CHANNEL_LAST_WORD ) {
	// Sanity check: the last word should be either channel header or data word.
	sprintf(_buf,"Missing a channel header word! Current word: %x ... Last word class %d",word,last_word_class);
	Message::send(MSG::ERROR,__FUNCTION__,_buf);

	status=false;
      }else{

	// Treat a case of missing very first channel header word
	if(last_word_class==FEM::CHANNEL_LAST_WORD){
	  
	  // Two possible cases: 
	  //    (1) previous waveform had odd # of ADC samples ... then this should be 0x8000
	  //    (2) two readout windows are combined
	  
	  //case (1)
	  if( word == 0x8000 ) {

	    if( ((*_event_data->rbegin()).size())%2 ) return true;

	    else { 
	      Message::send(MSG::ERROR,__FUNCTION__,
			    "Found a possible padding 0x8000 in an unexpected place (previous readout was even (%zu)!)...");
	      status = false;
	    }
	  }else{
	    // Do an operation that is done for the case of CHANNEL_HEADER, 
	    // but use a stored value of channel number & disc. id.
	    init_ch_info();
	    _ch_data.set_channel_number(_last_channel_number);
	    _ch_data.set_disc_id(_last_disc_id);
	    if(_verbosity[MSG::NORMAL])
	      Message::send(MSG::NORMAL,__FUNCTION__,
			    Form("Found consecutively readout data arrays @ event %d (missing channel very first header)!",
				 _event_data->event_number())
			    );
	  }
	}

	//
	// Ordinary operation for channel header / adc words
	//
	if(_channel_header_count==CHANNEL_HEADER_COUNT ) {
	  // Finished reading in header words. This should be a mere ADC count.
	  if(word_class!=FEM::CHANNEL_LAST_WORD)
	    _ch_data.push_back(word & 0xfff);
	}else if(last_word_class==FEM::CHANNEL_HEADER   ) {
	  // First of 2 channel header words. Record the values.

	  // This gives upper 8 bits of 20-bit sample_number number
	  _ch_data.set_readout_sample_number( (word & 0x1f)<<12 );       

	  // Lower 3 of 8 bits is the channel frame ID
	  _ch_data.set_readout_frame_number( ((word & 0xff)>>5) +
					 (((_event_data->event_frame_number())>>3)<<3) ); 

	  // Correct channel frame id roll-over w.r.t. event frame id
	  _ch_data.set_readout_frame_number(round_diff(_event_data->event_frame_number(),
						   _ch_data.readout_frame_number(),
						   0x7));
	  _channel_header_count++;
	}else if(last_word_class==FEM::CHANNEL_WORD     ) {
	  // Second of 2 channel header words. Record the values & inspect them.
	  _ch_data.set_readout_sample_number(_ch_data.readout_sample_number_64MHz() + (word & 0xfff)); // This gives lower 12 bits of 20-bit sample_number number
	  _channel_header_count++;

	  if(_verbosity[MSG::INFO]){
	    sprintf(_buf,"Read-in headers for Ch. %-3d!",_ch_data.channel_number());
	    Message::send(MSG::INFO,_buf);
	  }
	}

	// If this is channel's last word, store & clear channel info
	if(word_class==FEM::CHANNEL_LAST_WORD){
	  if(_verbosity[MSG::INFO]){
	    sprintf(_buf,"Encountered the last word (%x) for channel %d",word,_ch_data.channel_number());
	    Message::send(MSG::INFO,_buf);
	    sprintf(_buf,"Event frame  : %d",_event_data->event_frame_number());
	    Message::send(MSG::INFO,_buf);
	    sprintf(_buf,"PMT frame    : %d",_ch_data.readout_frame_number());
	    Message::send(MSG::INFO,_buf);
	    sprintf(_buf,"Disc. ID     : %d",_ch_data.disc_id());
	    Message::send(MSG::INFO,_buf);
	    sprintf(_buf,"Start Time   : %d",_ch_data.readout_sample_number_64MHz());
	    Message::send(MSG::INFO,_buf);
	    sprintf(_buf,"# ADC sample : %zd",_ch_data.size());
	    Message::send(MSG::INFO,_buf);
	  }
	  if( _print_adcval )
	    print_adc_values();
	  store_ch_data();
	  init_ch_info();
	}
      }
      break;
    case FEM::FEM_LAST_WORD:
    case FEM::EVENT_LAST_WORD:
      break;
    }
    return status;

  }

  void algo_slow_readout_decoder::store_ch_data(){

    if(_ch_data.channel_number() == BEAM_REF_CHANNEL) {
      _beam_event = (_beam_event || _ch_data.size() > BEAM_NWORDS);
      if(_beam_event){
	_beam_ref_nwords=(UInt_t)(_ch_data.size());
	_beam_ref_sample=_ch_data.readout_sample_number_64MHz();
	_beam_ref_frame=_ch_data.readout_frame_number();
      }
    }

    _ch_data.set_module_id(_event_data->module_id());
    _ch_data.set_module_address(_event_data->module_address());
    _event_data->push_back(_ch_data);

    // Store this waveform's channel number and discriminator id.
    // This may be used in the next pulse in case the next pulse is coming with no time space in between.
    // In such case, the next pulse is missing the channel very first header word because it is supposed
    // to be combined.
    _last_channel_number=_ch_data.channel_number();
    _last_disc_id=_ch_data.disc_id();

  }

}

#endif
