#ifndef EVB_ALGO_BASE_CC
#define EVB_ALGO_BASE_CC

#include "evb_algo_base.hh"

namespace larlight {

  evb_algo_base::evb_algo_base(DATA::DATA_TYPE type) : larlight_base()
  {
    _name = "evb_algo_base";
    
    _fout = 0;

    if( type != DATA::PMTFIFO &&
	type != DATA::TPCFIFO &&
	type != DATA::Trigger ) {

	print(MSG::ERROR,Form("Invalid algorithm type: %d",type));

	_type = DATA::DATA_TYPE_MAX;
    }else{

      print(MSG::DEBUG,__FUNCTION__,Form("%s constructor",class_name().c_str()));

      _type = type;

    }

    _current_event_number = 0;

    _eof  = false;

    _storage = new storage_manager();

    _storage->set_io_mode(_storage->READ);
  }

  bool evb_algo_base::initialize()
  {    
    _storage->set_verbosity(get_verbosity());

    if(_type==DATA::DATA_TYPE_MAX) return false;

    bool status = true;

    for(size_t i=0; i<DATA::DATA_TYPE_MAX; ++i)
      
      if( ((DATA::DATA_TYPE)i) == _type ) _storage->set_data_to_read( (DATA::DATA_TYPE)i, true);
      else _storage->set_data_to_read( (DATA::DATA_TYPE)i, false);

    status = _storage->open();

    if(status) status = _storage->next_event();

    if(!status) {

      print(MSG::ERROR,__FUNCTION__,
	    Form("Empty data : \"%s\" ... aborting!",DATA::DATA_TREE_NAME[_type].c_str()));

      _eof = true;

    }else{

      _current_event_number = get_event_number();

      _eof = false;
    }

    return status;
  }

  bool evb_algo_base::process(storage_manager *out_storage, UInt_t id)
  {
    
    if(_type==DATA::DATA_TYPE_MAX) return false;
    if(_eof) return false;
    
    // process 
    bool status = true;

    if(_verbosity[MSG::INFO]) {

      print(MSG::INFO,__FUNCTION__,
	    Form("data stream for \"%s\" ... event %d",DATA::DATA_TREE_NAME[_type].c_str(),_current_event_number));

    }

    if(_current_event_number==id || id==DATA::INVALID_UINT) 

      status = build(out_storage,id);

    else if(_current_event_number > id)
      
      throw evb_exception(Form("data stream for \"%s\" did not find input event with event number %d!", 
			       DATA::DATA_TREE_NAME[_type].c_str(),id)
			  );

    // while find_next is false, process 
    while(status && !find_next(id) && !_eof)
      
      status = build(out_storage,id);

    return status;
  }

  UInt_t evb_algo_base::get_event_number() const
  {

    if(_type==DATA::PMTFIFO || _type==DATA::TPCFIFO)
      
      return ((const event_fifo*)(_storage->get_data(_type)))->event_number();
    
    else
      
      return ((const trigger*)(_storage->get_data(_type)))->trig_number();
    
  }

  bool evb_algo_base::find_next(UInt_t id) {

    if(_current_event_number > id) return true;

    if(!_storage->next_event()) {
      
      print(MSG::NORMAL,__FUNCTION__,
	    Form(" Stream: \"%s\" reached the EOF... aborting!",DATA::DATA_TREE_NAME[data_type()].c_str()));

      _eof   = true;
      
      return false;
    }
    
    bool status = true;
    
    if(id==DATA::INVALID_UINT)

      status = (_current_event_number != get_event_number());
    
    else {

      if(_current_event_number == id) 

	status = (_current_event_number != get_event_number());

      else if (_current_event_number < id) {

	_current_event_number = get_event_number();

	while(_current_event_number < id && _storage->next_event())

	  _current_event_number = get_event_number();

	status = (_current_event_number > id);

	_eof = (_storage->get_index() == _storage->get_entries());
      }

      else print(MSG::ERROR,__FUNCTION__,Form("LOGIC ERROR: should not ever happen..."));

    }

    _current_event_number = get_event_number();

    return status;
  }

  bool evb_algo_base::finalize()
  {
    if(_type==DATA::DATA_TYPE_MAX) return false;
    
    if(_storage->is_open())

      _storage->close();

    _storage->reset();

    return true;
  }

}
#endif
