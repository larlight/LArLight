#ifndef STORAGE_MANAGER_CC
#define STORAGE_MANAGER_CC
#include "storage_manager.hh"

storage_manager* storage_manager::me=0;

//###################################################################################
storage_manager::storage_manager(storage_manager::MODE mode)
  : FrameworkBase()
//###################################################################################
{
  _name="storage_manager";
  _fout=0;
  _out_fname="";
  _name_tdirectory="";
  _status=INIT;

  reset();
  _mode=mode;

};

//###################################################################################
data_base* storage_manager::get_data(DATA::DATA_TYPE type)
//###################################################################################
{

  // If data class object does not exist, and if it's either WRITE or BOTH mode, create it.
  if(!_ptr_data_array[type] && _mode != READ){

    _fout->cd();

    _out_ch[(size_t)type]=new TTree(Form("%s",DATA::DATA_TREE_NAME[type].c_str()),
				    Form("%s Tree",DATA::DATA_TREE_NAME[type].c_str()));
    _out_ch[(size_t)type]->SetMaxTreeSize    (1024*1024*1024);
    _out_ch[(size_t)type]->SetMaxVirtualSize (1024*1024*1024);
    
    create_data_ptr(type);

    _ptr_data_array[(size_t)type]->set_address(_out_ch[(size_t)type]);
    
    Message::send(MSG::WARNING,__FUNCTION__,
		  Form("Requested tree %s which does not exists yet. Created a new one.", 
		       _out_ch[(size_t)type]->GetName())
		  );

    _write_data_array[(size_t)type]=true;

  }

  return _ptr_data_array[type];

}

//###################################################################################
void storage_manager::reset()
//###################################################################################
{
  if(_verbosity[MSG::DEBUG])
    Message::send(MSG::DEBUG,__PRETTY_FUNCTION__,"called ...");
  
  switch(_status){
  case READY_IO:
  case OPENED:
  case OPERATING:
    close();
    break;
  case INIT:
  case CLOSED:
    break;
  }

  _index=0;
  _nevents=0;
  _nevents_written=0;
  _nevents_read=0;
  _mode=UNDEFINED;
  _status=INIT;
  _in_fnames.clear();

  for(size_t i=0; i<DATA::DATA_TYPE_MAX; ++i) {
    _read_data_array[i]=true;   // Attempt to read in all data by default
    _write_data_array[i]=false; // Attemp to write no data by default 
    _in_ch[i]=0;
    _out_ch[i]=0;
    _ptr_data_array[i]=0;
  }

  if(_verbosity[MSG::DEBUG])
    Message::send(MSG::DEBUG,__PRETTY_FUNCTION__,"ends ...");  
};

//###################################################################################
bool storage_manager::is_open()
//###################################################################################
{
  bool status=true;
  switch(_status){
  case OPENED:
  case READY_IO:
  case OPERATING:
    break;
  case INIT:
  case CLOSED:
    status=false;
  }

  return status;
}

//###################################################################################
bool storage_manager::is_ready_io()
//###################################################################################
{
  bool status=true;
  switch(_status){
  case READY_IO:
  case OPERATING:
    break;
  case INIT:
  case OPENED:
  case CLOSED:
    status=false;
  }
  return status;
}

//###################################################################################
bool storage_manager::open()
//###################################################################################
{
  bool status=true;

  if(_verbosity[MSG::DEBUG])
    Message::send(MSG::DEBUG,__PRETTY_FUNCTION__,"called ...");

  if((_mode==READ || _mode==BOTH) && _in_fnames.size()==0){
    Message::send(MSG::ERROR,
		  __FUNCTION__,
		  "Open attempt w/o specifing a file name!");
    return false;
  }
  
  if((_mode==WRITE || _mode==BOTH) && _out_fname.size()==0){
    Message::send(MSG::ERROR,
		  __FUNCTION__,
		  "Open attempt w/o specifing a file name!");
    return false;
  }

  switch(_mode){

  case BOTH:
  case READ:

    for(std::vector<std::string>::const_iterator iter(_in_fnames.begin());
	iter!=_in_fnames.end();
	++iter) {

      TFile *fin = TFile::Open((*iter).c_str(),"READ");
      if(!fin) {
	sprintf(_buf,"Open attempt failed for a file: %s", (*iter).c_str());
	Message::send(MSG::ERROR,__FUNCTION__,_buf);
	status=false;
      }else{
	sprintf(_buf,"Opening a file in READ mode: %s", (*iter).c_str());
	Message::send(MSG::NORMAL,__FUNCTION__,_buf);
	fin->Close();
      }
      
    }

    if(_mode==READ) break;
  case WRITE:
    sprintf(_buf,"Opening a file in WRITE mode: %s",_out_fname.c_str());
    Message::send(MSG::NORMAL,__FUNCTION__,_buf);
    _fout=TFile::Open(_out_fname.c_str(),"RECREATE");
    if(!_fout) {
      sprintf(_buf,"Open attempt failed for a file: %s", _out_fname.c_str());
      Message::send(MSG::ERROR,__FUNCTION__,_buf);
      status=false;
    }
    break;

  case UNDEFINED:
    Message::send(MSG::ERROR,
		  __FUNCTION__,
		  "Open attempt w/o specifing I/O mode!");
    status=false;
    break;
  }
  
  if(!status) return status;

  if(_verbosity[MSG::DEBUG])
    Message::send(MSG::DEBUG,__PRETTY_FUNCTION__,"ends ...");

  _status=OPENED;
  return prepare_tree();
};

//###################################################################################
bool storage_manager::prepare_tree()
//###################################################################################
{
  
  bool status=true;

  std::vector<uint64_t> nevents_array(DATA::DATA_TYPE_MAX,0);
  
  if(_verbosity[MSG::DEBUG])
    Message::send(MSG::DEBUG,__PRETTY_FUNCTION__,"called ...");

  if(!_status==OPENED) {
    sprintf(_buf,"Unexpected function call @ _status=%d!",_status);
    Message::send(MSG::ERROR,__FUNCTION__,_buf);
    status=false;
  }

  for(size_t i=0; i<DATA::DATA_TYPE_MAX; ++i){

    if(!_status) break;

    if(_mode!=WRITE && _read_data_array[i]) {

      std::string tree_name(Form("%s",DATA::DATA_TREE_NAME[(DATA::DATA_TYPE)i].c_str()));

      if(_name_tdirectory.size()>0)
	_in_ch[i]=new TChain(Form("%s/%s",_name_tdirectory.c_str(),tree_name.c_str()),
			     Form("%s Tree",DATA::DATA_TREE_NAME[(DATA::DATA_TYPE)i].c_str()));
      else
	_in_ch[i]=new TChain(tree_name.c_str(),
			     Form("%s Tree",DATA::DATA_TREE_NAME[(DATA::DATA_TYPE)i].c_str()));
      
      for(size_t j=0; j<_in_fnames.size(); ++j)

	_in_ch[i]->AddFile(_in_fnames[j].c_str());

      // Ignore ROOT error message due to not finding a TTree 
      gErrorIgnoreLevel = kBreak;
      nevents_array[i]=_in_ch[i]->GetEntries();
      gErrorIgnoreLevel = kWarning;
      
      if(nevents_array[i]) { 

	create_data_ptr((DATA::DATA_TYPE)i);

	_ptr_data_array[i]->set_address((TTree*)_in_ch[i]);

	if(!_nevents) _nevents = nevents_array[i];

	if(_mode==BOTH) _write_data_array[i]=true;

      }else{
	delete _in_ch[i];
	_in_ch[i]=0;
	if(_mode==BOTH) _write_data_array[i]=false;
      }
    }

    if(_mode!=READ && _write_data_array[i] ) {

      _fout->cd();

      _out_ch[i]=new TTree(Form("%s",DATA::DATA_TREE_NAME[(DATA::DATA_TYPE)i].c_str()),
			   Form("%s Tree",DATA::DATA_TREE_NAME[(DATA::DATA_TYPE)i].c_str()));
      _out_ch[i]->SetMaxTreeSize    (1024*1024*1024);
      _out_ch[i]->SetMaxVirtualSize (1024*1024*1024);
      
      create_data_ptr((DATA::DATA_TYPE)i);
      
      _ptr_data_array[i]->set_address(_out_ch[i]);
    }

    _nevents_written=0;
    _nevents_read=0;
    _index=0;

  }

  if( _mode!=WRITE && _nevents==0) {
    Message::send(MSG::ERROR,__FUNCTION__,"Did not find any relevant data tree!");
    status=false;
  }

  for(size_t i=0; i<DATA::DATA_TYPE_MAX; ++i) {

    if(nevents_array[i] && _nevents!=nevents_array[i]) {
      sprintf(_buf,"Different number of entries found on tree: %s",DATA::DATA_TREE_NAME[(DATA::DATA_TYPE)i].c_str());
      Message::send(MSG::ERROR,__FUNCTION__,_buf);
      status=false;
    }

  }

  if(status) _status=READY_IO;

  else close();

  if(_verbosity[MSG::DEBUG])
    Message::send(MSG::DEBUG,__PRETTY_FUNCTION__,"ends ...");

  return status;
}

//###################################################################################
void storage_manager::create_data_ptr(DATA::DATA_TYPE type)
//###################################################################################
{

  if(_ptr_data_array[type]) return;

  switch(type){
  case DATA::Event:
    _ptr_data_array[type]=(data_base*)(new event(type));
    break;
  case DATA::Track:
  case DATA::Bezier:
  case DATA::Kalman3DHit:
  case DATA::Kalman3DSPS:
    _ptr_data_array[type]=(data_base*)(new track(type));
    break;
  case DATA::SpacePoint:
    _ptr_data_array[type]=(data_base*)(new sps(type));
    break;
  case DATA::Hit:
  case DATA::CrawlerHit:
  case DATA::GausHit:
  case DATA::APAHit:
  case DATA::FFTHit:
  case DATA::RFFHit:
    _ptr_data_array[type]=(data_base*)(new hit(type));
    break;
  case DATA::MCTruth:
  case DATA::GENIE_MCTruth:
  case DATA::CRY_MCTruth:
    _ptr_data_array[type]=(data_base*)(new mctruth(type));
    break;
  case DATA::MCNeutrino:
  case DATA::GENIE_MCNeutrino:
    _ptr_data_array[type]=(data_base*)(new mcnu(type));
    break;
  case DATA::Cluster:
  case DATA::CrawlerCluster:
  case DATA::DBCluster:
  case DATA::FuzzyCluster:
  case DATA::HoughCluster:
    _ptr_data_array[type]=(data_base*)(new cluster(type));
    break;
  case DATA::UserInfo:
  case DATA::FIFOChannel:
  case DATA::Wire:
  case DATA::Seed:
  case DATA::Shower:
  case DATA::Calorimetry:
  case DATA::DATA_TYPE_MAX:
    print(MSG::ERROR,__FUNCTION__,Form("Data identifier not supported: %d",(int)type));
    break;
  }

  return;
}

//###################################################################################
void storage_manager::delete_data_ptr(DATA::DATA_TYPE type)
//###################################################################################
{

  if(!_ptr_data_array[type]) return;

  delete _ptr_data_array[type];

  _ptr_data_array[type]=0;

  return;
}

//###################################################################################
bool storage_manager::close()
//###################################################################################
{

  if(_verbosity[MSG::DEBUG])
    Message::send(MSG::DEBUG,__PRETTY_FUNCTION__,"called ...");

  bool status=true;
  switch(_status){
  case INIT:
  case CLOSED:
    Message::send(MSG::ERROR,__FUNCTION__,"Attempt to close file while not operating I/O!");
    status=false;
    break;
  case OPENED:
  case READY_IO:
    Message::send(MSG::WARNING,__FUNCTION__,"Closing a file without any I/O operation done!");
    break;
  case OPERATING:
    if(_mode!=READ){

      //_fout->cd();

      for(size_t i=0; i<DATA::DATA_TYPE_MAX; i++) {

	if(!_out_ch[i]) {
	  
	  if(_verbosity[MSG::DEBUG])
	    
	    Message::send(MSG::DEBUG,__FUNCTION__,
			  Form("Skipping to write a Tree %s...", 
			       DATA::DATA_TREE_NAME[(DATA::DATA_TYPE)i].c_str()));

	  continue;
	}

	if(_verbosity[MSG::INFO])

	  Message::send(MSG::INFO,__FUNCTION__,Form("Writing TTree: %s",_out_ch[i]->GetName()));

	_fout = _out_ch[i]->GetCurrentFile();
	_out_ch[i]->Write();
	
	Message::send(MSG::NORMAL,__FUNCTION__,
		      Form("TTree \"%s\" written with %lld events...",
			   _out_ch[i]->GetName(),
			   _out_ch[i]->GetEntries()));
      }
    }
    break;
  }

  if(status) {

    sprintf(_buf,"Closing the output: %s",_out_fname.c_str());
    Message::send(MSG::NORMAL,__FUNCTION__,_buf);
    if(_fout) _fout->Close();
    _fout=0;

    for(size_t i=0; i<DATA::DATA_TYPE_MAX; ++i) {
      
      if(_in_ch[i])  { delete _in_ch[i];  _in_ch[i]=0;  }
      if(_out_ch[i]) { _out_ch[i]=0; }
      if(_ptr_data_array[i]) delete_data_ptr((DATA::DATA_TYPE)i);
      
    }

  }

  if(_verbosity[MSG::DEBUG])
    Message::send(MSG::DEBUG,__PRETTY_FUNCTION__,"ends ...");

  _status=CLOSED;
  return status;
}

//###################################################################################
bool storage_manager::go_to(uint32_t index)
//###################################################################################
{

  bool status=true;
  if(_mode==WRITE){
    Message::send(MSG::ERROR,__FUNCTION__,
		  "Cannot move the data pointer back/forth in WRITE mode.");
    status=false;
  }else if(_nevents && _nevents<index){
    sprintf(_buf,"Requested index, %d, exceeds the total entries, %d!",
	    index,_nevents);
    Message::send(MSG::ERROR,__FUNCTION__,_buf);
    status=false;
  }else
    _index=index;

  if(status) status=next_event();

  return status;
}

//###################################################################################
bool storage_manager::next_event()
//###################################################################################
{

  bool status=true;

  switch(_status){
  case READY_IO:
    // first event
    _status=OPERATING;
  case OPERATING:
    switch(_mode) {
    case READ:
      status=read_event();
      break;
    case WRITE:
      status=write_event();
      break;
    case BOTH:
      if(_nevents_read)
	status = write_event();
      if(status) status = read_event();
      break;
    case UNDEFINED:
      break;
    }
    break;
  case INIT:
  case OPENED:
  case CLOSED:
    Message::send(MSG::ERROR,__FUNCTION__,"Cannot perform I/O on a closed file!");
    status=false;
    break;
  }
  return status;
}

//###################################################################################
bool storage_manager::read_event()
//###################################################################################
{

  if(_index>=_nevents)
    return false;

  for(size_t i=0; i<DATA::DATA_TYPE_MAX; ++i) { 

    if(_in_ch[i])
      _in_ch[i]->GetEntry(_index);

  }

  _index++;
  _nevents_read++;
  return true;
}

//###################################################################################
bool storage_manager::write_event()
//###################################################################################
{
  
  for(size_t i=0; i<DATA::DATA_TYPE_MAX; ++i) {

    if(!_out_ch[i]) continue;

    _ptr_data_array[i]->entry(_index);
    _out_ch[i]->Fill();
    _ptr_data_array[i]->clear_event();

  }

  if(_mode==WRITE)
    _index++;
  _nevents_written++;
  //_event_wf->clear_data();
  return true;
}

//###################################################################################
const TChain* storage_manager::get_chain()
//###################################################################################
{
  
  TChain* ch=0;

  if( _status==INIT )

    if(!open()) return ch;

  if( _status!=READY_IO && _status!=OPERATING ) {

    print(MSG::ERROR,__FUNCTION__,"Call this function while file is open!");

    return ch;

  }

  if( _mode==WRITE ) {

    print(MSG::ERROR,__FUNCTION__,"Call this function in READ or BOTH I/O mode!");

    return ch;

  }

  for(size_t i=0; i<DATA::DATA_TYPE_MAX; ++i) {

    if(!_in_ch[i]) continue;

    print(MSG::NORMAL,__FUNCTION__,
	  Form("Found %s tree ... adding to a chain!",DATA::DATA_TREE_NAME[(DATA::DATA_TYPE)i].c_str()));
    if(ch) ch->AddFriend(_in_ch[i]);
    else ch=_in_ch[i];

  }

  return ch;

}

#endif
