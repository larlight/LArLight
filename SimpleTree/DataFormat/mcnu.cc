#ifndef MCNU_CC
#define MCNU_CC

#include "mcnu.hh"

//#####################################################################
mcnu::mcnu(DATA::DATA_TYPE type) : data_base(type)
//#####################################################################
{
  if(type!=DATA::MCNeutrino && 
     type!=DATA::GENIE_MCNeutrino) {
    print(MSG::ERROR,__FUNCTION__,
	  Form("Invalid data type for mcnu: %d ... setting default DATA::MCNeutrino type",type));
    _type = DATA::MCNeutrino;
  }else
    _type = type;
  _name = DATA::DATA_TREE_NAME[type];
  initialize();
}

//#############################################################################################
void mcnu::add_neutrino(Int_t pdgid,  Bool_t ccnc,    UChar_t mode,
			Double_t q2,  Double_t hmass,
			Int_t hitnuc,
			Float_t vtxx, Float_t vtxy,   Float_t vtxz,
			Double_t E,  Double_t momx,  Double_t momy,  Double_t momz,
			Double_t E_lep, Double_t momx_lep, Double_t momy_lep, Double_t momz_lep,
			Int_t pdgid_parent,
			Double_t momx_parent, Double_t momy_parent, Double_t momz_parent)
//#############################################################################################
{
  // Check C-array size capability
  if(_num_nu>=DATA::kMaxNeutrinos) {

    if(_num_nu==DATA::kMaxNeutrinos)

      print(MSG::ERROR,__FUNCTION__,
	    Form("Exceeding the maximum number of MCNeutrino that can be stored (%d)",DATA::kMaxNeutrinos));
    
    _num_nu++;

    return;
  }
  _pdgid[_num_nu]    = pdgid;
  _ccnc[_num_nu]     = ccnc;
  _mode[_num_nu]     = mode;
  _q2[_num_nu]       = q2;
  _hmass[_num_nu]    = hmass;
  _hitnuc[_num_nu]   = hitnuc;
  _vtxx[_num_nu]     = vtxx;
  _vtxy[_num_nu]     = vtxy;
  _vtxz[_num_nu]     = vtxz;
  _E[_num_nu]       = E;
  _momx[_num_nu]     = momx;
  _momy[_num_nu]     = momy;
  _momz[_num_nu]     = momz;
  _E_lep[_num_nu]   = E_lep;
  _momx_lep[_num_nu] = momx_lep;
  _momy_lep[_num_nu] = momy_lep;
  _momz_lep[_num_nu] = momz_lep;

  _momx_parent[_num_nu]  = momx_parent;
  _momy_parent[_num_nu]  = momy_parent;
  _momz_parent[_num_nu]  = momz_parent;
  _pdgid_parent[_num_nu] = pdgid_parent;

  _num_nu++;

}

//#####################################################################
void mcnu::clear_event(bool all)
//#####################################################################
{

  if(_num_nu >= DATA::kMaxNeutrinos) {

    print(MSG::WARNING,__FUNCTION__,
	  Form("Excess MCNeutrino %d (saved %d)",_num_nu,DATA::kMaxNeutrinos));

    _num_nu = DATA::kMaxNeutrinos;

  }

  /// Clear data_base variables
  data_base::clear_event(all);

  for(UShort_t index = 0;
      (index < _num_nu) || (all && index<DATA::kMaxNeutrinos);
      ++index) {

    _pdgid[index]    = 0;
    _ccnc[index]     = 0;
    _mode[index]     = 0;
    _q2[index]       = 0;
    _hmass[index]    = 0;
    _hitnuc[index]   = 0;
    _vtxx[index]     = 0;
    _vtxy[index]     = 0;
    _vtxz[index]     = 0;
    _E[index]       = 0;
    _momx[index]     = 0;
    _momy[index]     = 0;
    _momz[index]     = 0;
    _E_lep[index]   = 0;
    _momx_lep[index] = 0;
    _momy_lep[index] = 0;
    _momz_lep[index] = 0;

    _momx_parent[index]  = 0;
    _momy_parent[index]  = 0;
    _momz_parent[index]  = 0;
    _pdgid_parent[index] = 0;
    
  }
  _num_nu=0;
}

//#####################################################################
Bool_t mcnu::set_address(TTree* t,Bool_t create)
//#####################################################################
{
  print(MSG::DEBUG,__PRETTY_FUNCTION__,Form("%s created...",_name.c_str()));
  /// Set base class memory address
  Bool_t exist = data_base::set_address(t,create);
  
  if(t->GetBranch(Form("num_%s",_name.c_str()))) t->SetBranchAddress(Form("num_%s",_name.c_str()),&_num_nu);
  else {
    exist = false;
    if(create) t->Branch(Form("num_%s",_name.c_str()),&_num_nu,Form("num_%s/b",_name.c_str()));
  }

  if(t->GetBranch(Form("%s_pdgid",_name.c_str()))) t->SetBranchAddress(Form("%s_pdgid",_name.c_str()),_pdgid);
  else if(create) t->Branch(Form("%s_pdgid",_name.c_str()),_pdgid,Form("%s_pdgid[num_%s]/I",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_ccnc",_name.c_str()))) t->SetBranchAddress(Form("%s_ccnc",_name.c_str()),_ccnc);
  else if(create) t->Branch(Form("%s_ccnc",_name.c_str()),_ccnc,Form("%s_ccnc[num_%s]/O",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_mode",_name.c_str()))) t->SetBranchAddress(Form("%s_mode",_name.c_str()),_mode);
  else if(create) t->Branch(Form("%s_mode",_name.c_str()),_mode,Form("%s_mode[num_%s]/b",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_q2",_name.c_str()))) t->SetBranchAddress(Form("%s_q2",_name.c_str()),_q2);
  else if(create) t->Branch(Form("%s_q2",_name.c_str()),_q2,Form("%s_q2[num_%s]/D",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_hmass",_name.c_str()))) t->SetBranchAddress(Form("%s_hmass",_name.c_str()),_hmass);
  else if(create) t->Branch(Form("%s_hmass",_name.c_str()),_hmass,Form("%s_hmass[num_%s]/D",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_hitnuc",_name.c_str()))) t->SetBranchAddress(Form("%s_hitnuc",_name.c_str()),_hitnuc);
  else if(create) t->Branch(Form("%s_hitnuc",_name.c_str()),_hitnuc,Form("%s_hitnuc[num_%s]/I",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_vtxx",_name.c_str()))) t->SetBranchAddress(Form("%s_vtxx",_name.c_str()),_vtxx);
  else if(create) t->Branch(Form("%s_vtxx",_name.c_str()),_vtxx,Form("%s_vtxx[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_vtxy",_name.c_str()))) t->SetBranchAddress(Form("%s_vtxy",_name.c_str()),_vtxy);
  else if(create) t->Branch(Form("%s_vtxy",_name.c_str()),_vtxy,Form("%s_vtxy[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_vtxz",_name.c_str()))) t->SetBranchAddress(Form("%s_vtxz",_name.c_str()),_vtxz);
  else if(create) t->Branch(Form("%s_vtxz",_name.c_str()),_vtxz,Form("%s_vtxz[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_E",_name.c_str()))) t->SetBranchAddress(Form("%s_E",_name.c_str()),_E);
  else if(create) t->Branch(Form("%s_E",_name.c_str()),_E,Form("%s_E[num_%s]/D",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_momx",_name.c_str()))) t->SetBranchAddress(Form("%s_momx",_name.c_str()),_momx);
  else if(create) t->Branch(Form("%s_momx",_name.c_str()),_momx,Form("%s_momx[num_%s]/D",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_momy",_name.c_str()))) t->SetBranchAddress(Form("%s_momy",_name.c_str()),_momy);
  else if(create) t->Branch(Form("%s_momy",_name.c_str()),_momy,Form("%s_momy[num_%s]/D",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_momz",_name.c_str()))) t->SetBranchAddress(Form("%s_momz",_name.c_str()),_momz);
  else if(create) t->Branch(Form("%s_momz",_name.c_str()),_momz,Form("%s_momz[num_%s]/D",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_E_lep",_name.c_str()))) t->SetBranchAddress(Form("%s_E_lep",_name.c_str()),_E_lep);
  else if(create) t->Branch(Form("%s_E_lep",_name.c_str()),_E_lep,Form("%s_E_lep[num_%s]/D",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_momx_lep",_name.c_str()))) t->SetBranchAddress(Form("%s_momx_lep",_name.c_str()),_momx_lep);
  else if(create) t->Branch(Form("%s_momx_lep",_name.c_str()),_momx_lep,Form("%s_momx_lep[num_%s]/D",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_momy_lep",_name.c_str()))) t->SetBranchAddress(Form("%s_momy_lep",_name.c_str()),_momy_lep);
  else if(create) t->Branch(Form("%s_momy_lep",_name.c_str()),_momy_lep,Form("%s_momy_lep[num_%s]/D",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_momz_lep",_name.c_str()))) t->SetBranchAddress(Form("%s_momz_lep",_name.c_str()),_momz_lep);
  else if(create) t->Branch(Form("%s_momz_lep",_name.c_str()),_momz_lep,Form("%s_momz_lep[num_%s]/D",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_momx_parent",_name.c_str()))) t->SetBranchAddress(Form("%s_momx_parent",_name.c_str()),_momx_parent);
  else if(create) t->Branch(Form("%s_momx_parent",_name.c_str()),_momx_parent,Form("%s_momx_parent[num_%s]/D",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_momy_parent",_name.c_str()))) t->SetBranchAddress(Form("%s_momy_parent",_name.c_str()),_momy_parent);
  else if(create) t->Branch(Form("%s_momy_parent",_name.c_str()),_momy_parent,Form("%s_momy_parent[num_%s]/D",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_momz_parent",_name.c_str()))) t->SetBranchAddress(Form("%s_momz_parent",_name.c_str()),_momz_parent);
  else if(create) t->Branch(Form("%s_momz_parent",_name.c_str()),_momz_parent,Form("%s_momz_parent[num_%s]/D",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_pdgid_parent",_name.c_str()))) t->SetBranchAddress(Form("%s_pdgid_parent",_name.c_str()),_pdgid_parent);
  else if(create) t->Branch(Form("%s_pdgid_parent",_name.c_str()),_pdgid_parent,Form("%s_pdgid_parent[num_%s]/I",_name.c_str(),_name.c_str()));
  
  return exist;
}

#endif

