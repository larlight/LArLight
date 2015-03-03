#ifndef DECODER_MANAGER_CC
#define DECODER_MANAGER_CC

#include "decoder_manager.hh"

namespace larlight {

  decoder_manager::decoder_manager()
    : larlight_base(),
      _fin(FORMAT::ASCII)
  {
    _input_file="";
    _debug_mode=false;
    _fin=bin_io_handler();

    _read_block_size = 0;
    _read_by_block   = false;

    _storage=storage_manager::get();
    _storage->set_io_mode(storage_manager::WRITE);

    std::cout << "storage manage: set_io_mode: " << storage_manager::WRITE << std::endl;

    //if((_storage->input_filename()).size()==0)
    //  _storage->set_in_filename("out.root");

    _decoder=0;

    set_verbosity(MSG::NORMAL);
  }


  void decoder_manager::reset() {
    if(_storage->is_open()) {
      _storage->close();
      _storage->reset();
    }
    _fin.reset();
    _bin_files.clear();
  }

  bool decoder_manager::open_file()
  {
    return _fin.open();
  }


  bool decoder_manager::initialize()
  {
    if(_verbosity[MSG::DEBUG])
      Message::send(MSG::DEBUG,__FUNCTION__," begins...");
    if(!_decoder){
      Message::send(MSG::ERROR,__FUNCTION__,"Algorithm not attached. Aborting.");
      return false;
    }
    if(!_storage){
      Message::send(MSG::ERROR,__FUNCTION__,"Stoarge I/O pointer is empty.");
      return false;
    }

    _decoder->set_debug_mode(_debug_mode);
    _decoder->set_verbosity(_verbosity_level);
    _storage->set_verbosity(_verbosity_level);
    _fin.set_verbosity(_verbosity_level);

    if(_bin_files.size())
      _bin_file_iter = _bin_files.begin();

    _fin.set_filename((*_bin_file_iter).c_str());

    bool status=true;
    // Check if a file can be opened
    if(_fin.is_open())
      Message::send(MSG::WARNING,"Reading alrady-opened file contents!");
    else if(!_fin.open()) {
      Message::send(MSG::ERROR,__FUNCTION__,"Failed file I/O...");
      status=false;
    }

    if(!_storage->is_open())
      _storage->open();

    if(!_storage->is_ready_io()) {
      Message::send(MSG::ERROR,__FUNCTION__,
		    "Error in data output stream preparation.");
      status=false;
    }

   _decoder->set_storage_ptr(_storage);

    return status;
  }

  bool decoder_manager::decode(UInt_t limit) {

    if(_verbosity[MSG::DEBUG])
      Message::send(MSG::DEBUG,__FUNCTION__," begins...");

    bool status=true;
    UInt_t word = (_read_by_block) ? _fin.read_multi_word(_read_block_size) : _fin.read_word();
    UInt_t ctr=0;
    UInt_t index=0;
    time_t watch;
    while(status) {

      if(_fin.eof()){

	_bin_file_iter++;
	if(_bin_file_iter == _bin_files.end()) break;
	_fin.close();
	_fin.set_filename((*_bin_file_iter).c_str());

	if(!_fin.open()) {
	  Message::send(MSG::ERROR,__FUNCTION__,"Failed file I/O...");
	  status = false;
	  break;
	}
	word = (_read_by_block) ? _fin.read_multi_word(_read_block_size) : _fin.read_word();
      }

      try {
	status=_decoder->process_word(word);
      }catch(::larlight::decode_algo_exception e){

	std::cerr<<e.what()<<std::endl;
	status=false;
	if(_decoder->backtrace_mode())
	  _decoder->backtrace();
      }

      if(!status){
	//if(_decoder->backtrace_mode())
	//_decoder->backtrace();
	if(_debug_mode){
	  Message::send(MSG::ERROR,__FUNCTION__,"Process status failure ... but continue since DEBUG mode!");
	  status=true;
	}
      }

      index = _storage->get_index();

      if(limit && index>limit) break;
      
      if(index==(ctr*2000)){
	time(&watch);
	sprintf(_buf,"  ... processed %-6d events : %s",ctr*2000,ctime(&watch));
	Message::send(MSG::NORMAL,__FUNCTION__,_buf);
	ctr+=1;
      }

      word = (_read_by_block) ? _fin.read_multi_word(_read_block_size) : _fin.read_word();
    }
    
    if(!status && !_debug_mode){

      Message::send(MSG::ERROR,__FUNCTION__,Form("Event loop terminated. Stored: %d events",_storage->get_entries()));

    }else if(!(_decoder->is_event_empty())){

      Message::send(MSG::WARNING,__FUNCTION__,"Last event not stored by algorithm. Missing end-of-event word??");

      if(_decoder->check_event_quality()){

	Message::send(MSG::WARNING,__FUNCTION__,"Last event checksum agreed. Saving on file...");

	_storage->next_event();

      }else if(_fin.eof()){

	Message::send(MSG::WARNING,__FUNCTION__,"Skip saving the last event...");

	// Torelate the last event failure in case file is not closed properly
	//status=false;
	status = true;

      }else{

	Message::send(MSG::ERROR,__FUNCTION__,"Encountered a bad event before eof! Terminating...");
	status = false;

      }
    }

    return status;

  }

  bool decoder_manager::finalize() {

    if(_verbosity[MSG::DEBUG])
      Message::send(MSG::DEBUG,__FUNCTION__," begins...");  

    _storage->close();
    _fin.close();

    _decoder->reset();

    return true;
  }

  bool decoder_manager::run(UInt_t limit) {

    if(_verbosity[MSG::DEBUG])
      Message::send(MSG::DEBUG,__FUNCTION__," begins...");

    bool status=true;
    if(initialize())
      status=decode(limit);
    return (finalize() && status);

  }

}
#endif

