#ifndef MCTRUTH_CC
#define MCTRUTH_CC

#include "mctruth.hh"

//#####################################################################
mctruth::mctruth(DATA::DATA_TYPE type) : data_base(type)
//#####################################################################
{

  if(type!=DATA::CRY_MCTruth &&
     type!=DATA::GENIE_MCTruth &&
     type!=DATA::MCTruth) {
    
    print(MSG::ERROR,__FUNCTION__,
	  Form("Invalid data type for mctruth: %d ... setting default DATA::MCTruth type",type));
    _type = DATA::MCTruth;
  }else
    _type = type;
  _name = DATA::DATA_TREE_NAME[type];
  initialize();
}

//#####################################################################
void mctruth::clear_event(bool all)
//#####################################################################
{
  // Clear data_base variables
  data_base::clear_event(all);

  for(UShort_t index = 0;
      (index < _num_part) || (all && index<DATA::kMaxPrimaries);
      ++index) {

    _pdgid[index] = 0;
    _vtxx[index]  = 0;
    _vtxy[index]  = 0;
    _vtxz[index]  = 0;
    _E[index]    = 0;
    _mom[index]   = 0;
    _momx[index]  = 0;
    _momy[index]  = 0;
    _momz[index]  = 0;
    _mass[index]  = 0;
    _status_code[index] = 0;
    _gen_trackID[index] = 0;
    _num_daughter[index] = 0;
    _mother[index]      = 0;
    
  }
  _num_part = 0;
}

//###################################################################################
void mctruth::add_primary(Int_t pdgid, Int_t trackID, Int_t status_code, 
			  Int_t ndaughter, Int_t mother,
			  Float_t x, Float_t y, Float_t z,
			  Double_t mass, Double_t E,
			  Double_t mom,  Double_t momx, Double_t momy, Double_t momz)
//###################################################################################
{
  _pdgid[_num_part]       = pdgid;
  _gen_trackID[_num_part] = trackID;
  _status_code[_num_part] = status_code;
  _num_daughter[_num_part] = ndaughter;
  _mother[_num_part]      = mother;
  _vtxx[_num_part]        = x;
  _vtxy[_num_part]        = y;
  _vtxz[_num_part]        = z;
  _mass[_num_part]        = mass;
  _E[_num_part]          = E;
  _mom[_num_part]         = mom;
  _momx[_num_part]        = momx;
  _momy[_num_part]        = momy;
  _momz[_num_part]        = momz;
  _num_part++;
}

//###################################################################################
Bool_t mctruth::set_address(TTree* t,Bool_t create)
//###################################################################################
{
  print(MSG::DEBUG,__PRETTY_FUNCTION__,Form("%s called...",_name.c_str()));
  //Set address of the data_base variables
  Bool_t exist = data_base::set_address(t,create);

  if(t->GetBranch(Form("num_%s",_name.c_str()))) t->SetBranchAddress(Form("num_%s",_name.c_str()),&_num_part);
  else {
    exist = false;
    if(create) t->Branch(Form("num_%s",_name.c_str()),&_num_part,Form("num_%s/s",_name.c_str()));
  }

  if(t->GetBranch(Form("%s_pdgid",_name.c_str()))) t->SetBranchAddress(Form("%s_pdgid",_name.c_str()),_pdgid);
  else if(create) t->Branch(Form("%s_pdgid",_name.c_str()),&_pdgid,Form("%s_pdgid[num_%s]/I",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_E",_name.c_str())))  t->SetBranchAddress(Form("%s_E",_name.c_str()),_E);
  else if(create) t->Branch(Form("%s_E",_name.c_str()),_E,Form("%s_E[num_%s]/D",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_vtxx",_name.c_str()))) t->SetBranchAddress(Form("%s_vtxx",_name.c_str()),_vtxx);
  else if(create) t->Branch(Form("%s_vtxx",_name.c_str()),_vtxx,Form("%s_vtxx[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_vtxy",_name.c_str()))) t->SetBranchAddress(Form("%s_vtxy",_name.c_str()),_vtxy);
  else if(create) t->Branch(Form("%s_vtxy",_name.c_str()),_vtxy,Form("%s_vtxy[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_vtxz",_name.c_str()))) t->SetBranchAddress(Form("%s_vtxz",_name.c_str()),_vtxz);
  else if(create) t->Branch(Form("%s_vtxz",_name.c_str()),_vtxz,Form("%s_vtxz[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_momx",_name.c_str()))) t->SetBranchAddress(Form("%s_momx",_name.c_str()),_momx);
  else if(create) t->Branch(Form("%s_momx",_name.c_str()),_momx,Form("%s_momx[num_%s]/D",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_momy",_name.c_str()))) t->SetBranchAddress(Form("%s_momy",_name.c_str()),_momy);
  else if(create) t->Branch(Form("%s_momy",_name.c_str()),_momy,Form("%s_momy[num_%s]/D",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_momz",_name.c_str()))) t->SetBranchAddress(Form("%s_momz",_name.c_str()),_momz);
  else if(create) t->Branch(Form("%s_momz",_name.c_str()),_momz,Form("%s_momz[num_%s]/D",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_mom",_name.c_str()))) t->SetBranchAddress(Form("%s_mom",_name.c_str()),_mom);
  else if(create) t->Branch(Form("%s_mom",_name.c_str()),_mom,Form("%s_mom[num_%s]/D",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_status_code",_name.c_str()))) t->SetBranchAddress(Form("%s_status_code",_name.c_str()),_status_code);
  else if(create) t->Branch(Form("%s_status_code",_name.c_str()),_status_code,Form("%s_status_code[num_%s]/I",_name.c_str(),_name.c_str()));
 
  if(t->GetBranch(Form("%s_mass",_name.c_str()))) t->SetBranchAddress(Form("%s_mass",_name.c_str()),_mass);
  else if(create) t->Branch(Form("%s_mass",_name.c_str()),_mass,Form("%s_mass[num_%s]/D",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_gen_trackID",_name.c_str()))) t->SetBranchAddress(Form("%s_gen_trackID",_name.c_str()),_gen_trackID);
  else if(create) t->Branch(Form("%s_gen_trackID",_name.c_str()),_gen_trackID,Form("%s_gen_trackID[num_%s]/I",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_num_daughter",_name.c_str()))) t->SetBranchAddress(Form("%s_num_daughter",_name.c_str()),_num_daughter);
  else if(create) t->Branch(Form("%s_num_daughter",_name.c_str()),_num_daughter,Form("%s_num_daughter[num_%s]/I",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_mother",_name.c_str()))) t->SetBranchAddress(Form("%s_mother",_name.c_str()),_mother);
  else if(create) t->Branch(Form("%s_mother",_name.c_str()),_mother,Form("%s_mother[num_%s]/I",_name.c_str(),_name.c_str()));

  return exist;
}

#endif
