#ifndef MCSTEP_CC
#define MCSTEP_CC

#include "mcstep.hh"

//####################################################
mcstep::mcstep(DATA::DATA_TYPE type) : data_base(type)
//####################################################
{
  _name=DATA::DATA_TREE_NAME[type];
  initialize();
}

//################################
void mcstep::clear_event(bool all)
//################################
{
  /// clear base data structure
  data_base::clear_event(all);

  for(UShort_t index = 0;
      (index < _no_steps) || (all && index<DATA::kMaxTrajectories);
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

  _no_steps=0;
}

//######################################################################
void mcstep::add_trajectory(UShort_t trackID, Int_t pdgid,
			    Double_t momx, Double_t momy, Double_t momz,
			    Float_t vtxx, Float_t vtxy, Float_t vtxz,
			    Double_t de, Double_t dx)
//######################################################################
{
  _trackID[_no_steps] = trackID;
  _pdgid[_no_steps]   = pdgid;
  _momx[_no_steps]    = momx;
  _momy[_no_steps]    = momy;
  _momz[_no_steps]    = momz;
  _vtxx[_no_steps]    = vtxx;
  _vtxy[_no_steps]    = vtxy;
  _vtxz[_no_steps]    = vtxz;
  _de[_no_steps]      = de;
  _dx[_no_steps]      = dx;
  _no_steps++;
}

//################################
void mcstep::set_address(TTree* t)
//################################
{
  /// set base class memory address
  data_base::set_address(t);

  if(t->GetBranch("no_steps")) t->SetBranchAddress("no_steps",&_no_steps);
  else t->Branch("no_steps",&_no_steps,"no_steps/i");

  if(t->GetBranch("trackID")) t->SetBranchAddress("trackID",_trackID);
  else t->Branch("trackID",_trackID,"trackID[no_steps]/i");

  if(t->GetBranch("pdgid")) t->SetBranchAddress("pdgid",_pdgid);
  else t->Branch("pdgid",_pdgid,"pdgid[no_steps]/I");

  if(t->GetBranch("momx")) t->SetBranchAddress("momx",_momx);
  else t->Branch("momx",_momx,"momx[no_steps]/D");

  if(t->GetBranch("momy")) t->SetBranchAddress("momy",_momy);
  else t->Branch("momy",_momy,"momy[no_steps]/D");

  if(t->GetBranch("momz")) t->SetBranchAddress("momz",_momz);
  else t->Branch("momz",_momz,"momz[no_steps]/D");

  if(t->GetBranch("vtxx")) t->SetBranchAddress("vtxx",_vtxx);
  else t->Branch("vtxx",_vtxx,"vtxx[no_steps]/F");

  if(t->GetBranch("vtxy")) t->SetBranchAddress("vtxy",_vtxy);
  else t->Branch("vtxy",_vtxy,"vtxy[no_steps]/F");

  if(t->GetBranch("vtxz")) t->SetBranchAddress("vtxz",_vtxz);
  else t->Branch("vtxz",_vtxz,"vtxz[no_steps]/F");

  if(t->GetBranch("de")) t->SetBranchAddress("de",_de);
  else t->Branch("de",_de,"de[no_steps]/D");

  if(t->GetBranch("dx")) t->SetBranchAddress("dx",_dx);
  else t->Branch("dx",_dx,"dx[no_steps]/F");


  
}


#endif
