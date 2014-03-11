#ifndef MCSTEP_CC
#define MCSTEP_CC

#include "mcstep.hh"

//####################################################
mcstep::mcstep() : data_base(DATA::MCTrajectory)
//####################################################
{
  _name=DATA::DATA_TREE_NAME[DATA::MCTrajectory];
  initialize();
}

//################################
void mcstep::clear_event(bool all)
//################################
{
  /// clear base data structure
  data_base::clear_event(all);

  if(_num_steps >= DATA::kMaxTrajectories) {

    print(MSG::WARNING,__FUNCTION__,
	  Form("Excess MCTrajectory %d (saved %d)",_num_steps,DATA::kMaxTrajectories));

    _num_steps = DATA::kMaxTrajectories;

  }
  
  for(UShort_t index = 0;
      (index < _num_steps) || (all && index<DATA::kMaxTrajectories);
      ++index) {

    _trackID[index] = 0;
    _pdgid[index]   = 0;
    _momx[index]    = 0;
    _momy[index]    = 0;
    _momz[index]    = 0;
    _vtxx[index]    = 0;
    _vtxy[index]    = 0;
    _vtxz[index]    = 0;
    _de[index]      = 0;
    _dx[index]      = 0;
  }
  _num_steps=0;
}

//######################################################################
void mcstep::add_trajectory(UShort_t trackID, Int_t pdgid,
			    Double_t momx, Double_t momy, Double_t momz,
			    Float_t vtxx, Float_t vtxy, Float_t vtxz,
			    Double_t de, Float_t dx)
//######################################################################
{
  // Check C-array size capability
  if(_num_steps>=DATA::kMaxTrajectories) {

    if(_num_steps==DATA::kMaxTrajectories)

      print(MSG::ERROR,__FUNCTION__,
	    Form("Exceeding the maximum number of MCTrajectory that can be stored (%d)",DATA::kMaxTrajectories));

    _num_steps++;

    return;
  }

  _trackID[_num_steps] = trackID;
  _pdgid[_num_steps]   = pdgid;
  _momx[_num_steps]    = momx;
  _momy[_num_steps]    = momy;
  _momz[_num_steps]    = momz;
  _vtxx[_num_steps]    = vtxx;
  _vtxy[_num_steps]    = vtxy;
  _vtxz[_num_steps]    = vtxz;
  _de[_num_steps]      = de;
  _dx[_num_steps]      = dx;
  _num_steps++;
}

//################################
Bool_t mcstep::set_address(TTree* t,Bool_t create)
//################################
{
  print(MSG::DEBUG,__PRETTY_FUNCTION__,Form("%s called...",_name.c_str()));
  /// set base class memory address
  Bool_t exist = data_base::set_address(t,create);

  if(t->GetBranch(Form("num_%s",_name.c_str()))) t->SetBranchAddress(Form("num_%s",_name.c_str()),&_num_steps);
  else {
    exist = false;
    if(create) t->Branch(Form("num_%s",_name.c_str()),&_num_steps,Form("num_%s/i",_name.c_str()));
  }

  if(t->GetBranch(Form("%s_trackID",_name.c_str()))) t->SetBranchAddress(Form("%s_trackID",_name.c_str()),_trackID);
  else if(create) t->Branch(Form("%s_trackID",_name.c_str()),_trackID,Form("%s_trackID[num_%s]/s",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_pdgid",_name.c_str()))) t->SetBranchAddress(Form("%s_pdgid",_name.c_str()),_pdgid);
  else if(create) t->Branch(Form("%s_pdgid",_name.c_str()),_pdgid,Form("%s_pdgid[num_%s]/I",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_momx",_name.c_str()))) t->SetBranchAddress(Form("%s_momx",_name.c_str()),_momx);
  else if(create) t->Branch(Form("%s_momx",_name.c_str()),_momx,Form("%s_momx[num_%s]/D",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_momy",_name.c_str()))) t->SetBranchAddress(Form("%s_momy",_name.c_str()),_momy);
  else if(create) t->Branch(Form("%s_momy",_name.c_str()),_momy,Form("%s_momy[num_%s]/D",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_momz",_name.c_str()))) t->SetBranchAddress(Form("%s_momz",_name.c_str()),_momz);
  else if(create) t->Branch(Form("%s_momz",_name.c_str()),_momz,Form("%s_momz[num_%s]/D",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_vtxx",_name.c_str()))) t->SetBranchAddress(Form("%s_vtxx",_name.c_str()),_vtxx);
  else if(create) t->Branch(Form("%s_vtxx",_name.c_str()),_vtxx,Form("%s_vtxx[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_vtxy",_name.c_str()))) t->SetBranchAddress(Form("%s_vtxy",_name.c_str()),_vtxy);
  else if(create) t->Branch(Form("%s_vtxy",_name.c_str()),_vtxy,Form("%s_vtxy[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_vtxz",_name.c_str()))) t->SetBranchAddress(Form("%s_vtxz",_name.c_str()),_vtxz);
  else if(create) t->Branch(Form("%s_vtxz",_name.c_str()),_vtxz,Form("%s_vtxz[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_de",_name.c_str()))) t->SetBranchAddress(Form("%s_de",_name.c_str()),_de);
  else if(create) t->Branch(Form("%s_de",_name.c_str()),_de,Form("%s_de[num_%s]/D",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_dx",_name.c_str()))) t->SetBranchAddress(Form("%s_dx",_name.c_str()),_dx);
  else if(create) t->Branch(Form("%s_dx",_name.c_str()),_dx,Form("%s_dx[num_%s]/F",_name.c_str(),_name.c_str()));

  return exist;
  
}


#endif
