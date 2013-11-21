#ifndef EVENT_CC
#define EVENT_CC

#include "event.hh"

//####################################################
event::event() : data_base(DATA::Event)
//####################################################
{
  _name = DATA::DATA_TREE_NAME[DATA::Event];
  initialize();
}

//####################################################
void event::clear_event(bool all)
//####################################################
{
  /// Clear base class variables
  data_base::clear_event(all);

  _eventID  = 0;
  _runID    = 0;
  _subrunID = 0;
  _triggerT = 0;
  _beamT    = 0;
  _pot      = 0;
  _etau     = 0;
  _isdata   = 0;
}

//#################################################################
void event::set_event(UInt_t eventID, UInt_t runID, UInt_t subrunID,
		      Bool_t isdata,
		      Double_t triggerT, Double_t beamT,
		      Double_t pot, Double_t etau)
//#################################################################
{
  _eventID  = eventID;
  _runID    = runID;  
  _subrunID = subrunID;
  _triggerT = triggerT;
  _beamT    = beamT;
  _pot      = pot;
  _etau     = etau;
  _isdata   = isdata;
}

//#################################################################
Bool_t event::set_address(TTree* t,Bool_t create)
//#################################################################
{
  print(MSG::DEBUG,__PRETTY_FUNCTION__,Form("%s called...",_name.c_str()));
  /// Set base class address
  Bool_t exist = data_base::set_address(t,create);

  if(t->GetBranch(Form("%s_ID",_name.c_str()))) t->SetBranchAddress(Form("%s_ID",_name.c_str()),&_eventID);
  else {
    exist = false;
    if(create) t->Branch(Form("%s_ID",_name.c_str()),&_eventID,Form("%s_ID/i",_name.c_str()));
  }      

  if(t->GetBranch(Form("%s_runID",_name.c_str()))) t->SetBranchAddress(Form("%s_runID",_name.c_str()),&_runID);
  else if(create) t->Branch(Form("%s_runID",_name.c_str()),&_runID,Form("%s_runID/i",_name.c_str()));

  if(t->GetBranch(Form("%s_subrunID",_name.c_str()))) t->SetBranchAddress(Form("%s_subrunID",_name.c_str()),&_subrunID);
  else if(create) t->Branch(Form("%s_subrunID",_name.c_str()),&_subrunID,Form("%s_subrunID/i",_name.c_str()));

  if(t->GetBranch(Form("%s_triggerT",_name.c_str()))) t->SetBranchAddress(Form("%s_triggerT",_name.c_str()),&_triggerT);
  else if(create) t->Branch(Form("%s_triggerT",_name.c_str()),&_triggerT,Form("%s_triggerT/D",_name.c_str()));

  if(t->GetBranch(Form("%s_beamT",_name.c_str()))) t->SetBranchAddress(Form("%s_beamT",_name.c_str()),&_beamT);
  else if(create) t->Branch(Form("%s_beamT",_name.c_str()),&_beamT,Form("%s_beamT/D",_name.c_str()));

  if(t->GetBranch(Form("%s_pot",_name.c_str()))) t->SetBranchAddress(Form("%s_pot",_name.c_str()),&_pot);
  else if(create) t->Branch(Form("%s_pot",_name.c_str()),&_pot,Form("%s_pot/D",_name.c_str()));

  if(t->GetBranch(Form("%s_etau",_name.c_str()))) t->SetBranchAddress(Form("%s_etau",_name.c_str()),&_etau);
  else if(create) t->Branch(Form("%s_etau",_name.c_str()),&_etau,Form("%s_etau/D",_name.c_str()));

  if(t->GetBranch(Form("%s_isdata",_name.c_str()))) t->SetBranchAddress(Form("%s_isdata",_name.c_str()),&_isdata);
  else if(create) t->Branch(Form("%s_isdata",_name.c_str()),&_isdata,Form("%s_isdata/O",_name.c_str()));

  return exist;
}

#endif
