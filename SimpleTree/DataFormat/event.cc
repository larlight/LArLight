#ifndef EVENT_CC
#define EVENT_CC

#include "event.hh"

//####################################################
event::event(DATA::DATA_TYPE type) : data_base(type)
//####################################################
{
  _name = DATA::DATA_TREE_NAME[type];
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
void event::set_address(TTree* t)
//#################################################################
{
  /// Set base class address
  data_base::set_address(t);
  
  if(t->GetBranch("eventID")) t->SetBranchAddress("eventID",&_eventID);
  else t->Branch("eventID",&_eventID,"eventID/i");

  if(t->GetBranch("runID")) t->SetBranchAddress("runID",&_runID);
  else t->Branch("runID",&_runID,"runID/i");

  if(t->GetBranch("subrunID")) t->SetBranchAddress("subrunID",&_subrunID);
  else t->Branch("subrunID",&_subrunID,"subrunID/i");

  if(t->GetBranch("triggerT")) t->SetBranchAddress("triggerT",&_triggerT);
  else t->Branch("triggerT",&_triggerT,"triggerT/D");

  if(t->GetBranch("beamT")) t->SetBranchAddress("beamT",&_beamT);
  else t->Branch("beamT",&_beamT,"beamT/D");

  if(t->GetBranch("pot")) t->SetBranchAddress("pot",&_pot);
  else t->Branch("pot",&_pot,"pot/D");

  if(t->GetBranch("etau")) t->SetBranchAddress("etau",&_etau);
  else t->Branch("etau",&_etau,"etau/D");

  if(t->GetBranch("isdata")) t->SetBranchAddress("isdata",&_isdata);
  else t->Branch("isdata",&_isdata,"isdata/O");

}

#endif
