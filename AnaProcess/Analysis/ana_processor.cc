#ifndef ANA_PROCESSOR_CC
#define ANA_PROCESSOR_CC

#include "ana_processor.hh"

ana_processor::ana_processor(){
  _name="ana_processor";
  _ofile_name="";
  //_mode=storage_manager::READ; // default I/O mode is to produce no output
  _storage=storage_manager::get();
  _fout=0;
  reset();
}

void ana_processor::set_verbosity(MSG::Level level){
  set_verbosity(level);
  if(_storage)
    _storage->set_verbosity(level);
  for(std::vector<ana_base*>::const_iterator iter(_analyzers.begin());
      iter!=_analyzers.end();
      ++iter){
    (*iter)->set_verbosity(level);
  }
}

void ana_processor::reset(){
  
  if(_verbosity[MSG::DEBUG])
    Message::send(MSG::DEBUG,__PRETTY_FUNCTION__,"called...");

  if(_fout){
    _fout->Close();
    _fout=0;
  }

  if(_storage)
    _storage->reset();

  _analyzers.clear();
  _ana_status.clear();
  _nevents=0;
  _index=0;

  _process=INIT;
}

bool ana_processor::initialize(){

  set_verbosity(_verbosity_level);

  if(_verbosity[MSG::DEBUG])
    Message::send(MSG::DEBUG,__PRETTY_FUNCTION__,"called...");

  if(_process!=INIT){
    Message::send(MSG::ERROR,__FUNCTION__,
		  "Logic error: the function should not be called.");
    return false;
  }

  //_storage->set_io_mode(storage_manager::READ);
  if(!_storage->open()) {

    Message::send(MSG::ERROR,__FUNCTION__,"File I/O failure...");

    return false;

  }

  if(_ofile_name.size()==0){

    Message::send(MSG::ERROR,__FUNCTION__,
		  "Must provide an output filename!");
    return false;
  }

  _fout=TFile::Open(_ofile_name.c_str(),"RECREATE");

  if(!_fout){

    sprintf(_buf,"Failed to open an output file: %s",_ofile_name.c_str());

    Message::send(MSG::ERROR,__FUNCTION__,_buf);

    return false;

  }

  bool status = true;

  for(std::vector<ana_base*>::iterator iter(_analyzers.begin());
      iter!=_analyzers.end();
      ++iter) {

    (*iter)->set_output_file(_fout);

    if(!((*iter)->initialize())){

      Message::send(MSG::ERROR,__PRETTY_FUNCTION__,
		    Form("Failed to initialize: %s",(*iter)->class_name().c_str()));

      status = false;
    }

  }    
  _process=READY;
  _index=0;
  _nevents=0;
  return status;
}

bool ana_processor::process_event(uint32_t index){

  if(_process==INIT) {
    
    if(!initialize()) {
      Message::send(MSG::ERROR,__FUNCTION__,"Aborting.");
      return false;
    }
    else
      _process=PROCESSING;
  }

  if(_process==READY)
    _process=PROCESSING;

  bool event_found = index ? _storage->go_to(index) : _storage->next_event();

  if(event_found){
    
    for(std::vector<ana_base*>::iterator iter(_analyzers.begin());
	iter!=_analyzers.end();
	++iter)
      _ana_status[(*iter)]=(*iter)->analyze(_storage);

    _index++;
    _nevents++;
    
    return true;

  }
  else
    return finalize();
}

bool ana_processor::run(uint32_t start_index, uint32_t nevents){

  if(_verbosity[MSG::DEBUG])
    Message::send(MSG::DEBUG,__PRETTY_FUNCTION__,"called...");

  bool status=true;

  if(_process==INIT) status = initialize();

  if(!status){

    Message::send(MSG::ERROR,__PRETTY_FUNCTION__,"Aborting.");

    return false;
  }

  _index=start_index;

  if(start_index)
    _storage->go_to(start_index);

  if(!nevents)
    nevents=_storage->get_entries();
  if(nevents > (_storage->get_entries() - start_index))
    nevents=_storage->get_entries() - start_index;

  sprintf(_buf,"Processing %d events...",nevents);
  Message::send(MSG::NORMAL,__FUNCTION__,_buf);

  int ten_percent_ctr=0;
  
  while(status){

    status=process_event();

    if(nevents && nevents < _nevents){
      Message::send(MSG::NORMAL,__FUNCTION__,Form("Processed %d/%d events! Aborting...",_nevents,nevents));
      finalize();
      break;
    }

    if(nevents > 10 && (_nevents >= ten_percent_ctr * nevents/10)) {
      
      sprintf(_buf," ... %3d%% done ...",ten_percent_ctr*10);
      Message::send(MSG::NORMAL,__FUNCTION__,_buf);
      ten_percent_ctr++;
    }

    if(_process!=PROCESSING) break;
    
  }
  
  return status;
  
}

bool ana_processor::finalize() {

  if(_verbosity[MSG::DEBUG])
    Message::send(MSG::DEBUG,__PRETTY_FUNCTION__,"called...");

  if(_process!=PROCESSING && _process!=READY) {
    Message::send(MSG::ERROR,__FUNCTION__,
		  "Logic error: the function should not be called.");
    return false;
  }

  bool status=true;
  for(std::vector<ana_base*>::iterator iter(_analyzers.begin());
      iter!=_analyzers.end();
      ++iter) {

    _ana_status[(*iter)]=_ana_status[(*iter)] && (*iter)->finalize();

    status = status && _ana_status[(*iter)];
  }

  _process=FINISHED;
  reset();
  return status;  
}

bool ana_processor::get_ana_status(ana_base* ptr) const{

  std::map<ana_base*,bool>::const_iterator iter(_ana_status.find(ptr));
  if(iter==_ana_status.end()) return false;

  else return (*iter).second;
  
}

#endif
