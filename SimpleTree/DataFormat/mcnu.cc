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
			Double_t en,  Double_t momx,  Double_t momy,  Double_t momz,
			Double_t en_lep, Double_t momx_lep, Double_t momy_lep, Double_t momz_lep,
			Int_t pdgid_parent,
			Double_t momx_parent, Double_t momy_parent, Double_t momz_parent)
//#############################################################################################
{

  _pdgid[_no_nu]    = pdgid;
  _ccnc[_no_nu]     = ccnc;
  _mode[_no_nu]     = mode;
  _q2[_no_nu]       = q2;
  _hmass[_no_nu]    = hmass;
  _hitnuc[_no_nu]   = hitnuc;
  _vtxx[_no_nu]     = vtxx;
  _vtxy[_no_nu]     = vtxy;
  _vtxz[_no_nu]     = vtxz;
  _en[_no_nu]       = en;
  _momx[_no_nu]     = momx;
  _momy[_no_nu]     = momy;
  _momz[_no_nu]     = momz;
  _en_lep[_no_nu]   = en_lep;
  _momx_lep[_no_nu] = momx_lep;
  _momy_lep[_no_nu] = momy_lep;
  _momz_lep[_no_nu] = momz_lep;

  _momx_parent[_no_nu]  = momx_parent;
  _momy_parent[_no_nu]  = momy_parent;
  _momz_parent[_no_nu]  = momz_parent;
  _pdgid_parent[_no_nu] = pdgid_parent;

  _no_nu++;

}

//#####################################################################
void mcnu::clear_event(bool all)
//#####################################################################
{

  /// Clear data_base variables
  data_base::clear_event(all);

  for(UShort_t index = 0;
      (index < _no_nu) || (all && index<DATA::kMaxNeutrinos);
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
    _en[index]       = 0;
    _momx[index]     = 0;
    _momy[index]     = 0;
    _momz[index]     = 0;
    _en_lep[index]   = 0;
    _momx_lep[index] = 0;
    _momy_lep[index] = 0;
    _momz_lep[index] = 0;

    _momx_parent[index]  = 0;
    _momy_parent[index]  = 0;
    _momz_parent[index]  = 0;
    _pdgid_parent[index] = 0;
    
  }
  _no_nu=0;
}

//#####################################################################
void mcnu::set_address(TTree* t)
//#####################################################################
{

  /// Set base class memory address
  data_base::set_address(t);

  if(t->GetBranch("no_nu")) t->SetBranchAddress("no_nu",&_no_nu);
  else t->Branch("no_nu",&_no_nu,"no_nu/b");
  
  if(t->GetBranch("pdgid")) t->SetBranchAddress("pdgid",_pdgid);
  else t->Branch("pdgid",_pdgid,"pdgid[no_nu]/I");

  if(t->GetBranch("ccnc")) t->SetBranchAddress("ccnc",_ccnc);
  else t->Branch("ccnc",_ccnc,"ccnc[no_nu]/O");

  if(t->GetBranch("mode")) t->SetBranchAddress("mode",_mode);
  else t->Branch("mode",_mode,"mode[no_nu]/b");

  if(t->GetBranch("q2")) t->SetBranchAddress("q2",_q2);
  else t->Branch("q2",_q2,"q2[no_nu]/D");

  if(t->GetBranch("hmass")) t->SetBranchAddress("hmass",_hmass);
  else t->Branch("hmass",_hmass,"hmass[no_nu]/D");

  if(t->GetBranch("hitnuc")) t->SetBranchAddress("hitnuc",_hitnuc);
  else t->Branch("hitnuc",_hitnuc,"hitnuc[no_nu]/I");

  if(t->GetBranch("vtxx")) t->SetBranchAddress("vtxx",_vtxx);
  else t->Branch("vtxx",_vtxx,"vtxx[no_nu]/F");

  if(t->GetBranch("vtxy")) t->SetBranchAddress("vtxy",_vtxy);
  else t->Branch("vtxy",_vtxy,"vtxy[no_nu]/F");

  if(t->GetBranch("vtxz")) t->SetBranchAddress("vtxz",_vtxz);
  else t->Branch("vtxz",_vtxz,"vtxz[no_nu]/F");

  if(t->GetBranch("en")) t->SetBranchAddress("en",_en);
  else t->Branch("en",_en,"en[no_nu]/D");

  if(t->GetBranch("momx")) t->SetBranchAddress("momx",_momx);
  else t->Branch("momx",_momx,"momx[no_nu]/D");

  if(t->GetBranch("momy")) t->SetBranchAddress("momy",_momy);
  else t->Branch("momy",_momy,"momy[no_nu]/D");

  if(t->GetBranch("momz")) t->SetBranchAddress("momz",_momz);
  else t->Branch("momz",_momz,"momz[no_nu]/D");

  if(t->GetBranch("en_lep")) t->SetBranchAddress("en_lep",_en_lep);
  else t->Branch("en_lep",_en_lep,"en_lep[no_nu]/D");

  if(t->GetBranch("momx_lep")) t->SetBranchAddress("momx_lep",_momx_lep);
  else t->Branch("momx_lep",_momx_lep,"momx_lep[no_nu]/D");

  if(t->GetBranch("momy_lep")) t->SetBranchAddress("momy_lep",_momy_lep);
  else t->Branch("momy_lep",_momy_lep,"momy_lep[no_nu]/D");

  if(t->GetBranch("momz_lep")) t->SetBranchAddress("momz_lep",_momz_lep);
  else t->Branch("momz_lep",_momz_lep,"momz_lep[no_nu]/D");

  if(t->GetBranch("momx_parent")) t->SetBranchAddress("momx_parent",_momx_parent);
  else t->Branch("momx_parent",_momx_parent,"momx_parent[no_nu]/D");

  if(t->GetBranch("momy_parent")) t->SetBranchAddress("momy_parent",_momy_parent);
  else t->Branch("momy_parent",_momy_parent,"momy_parent[no_nu]/D");

  if(t->GetBranch("momz_parent")) t->SetBranchAddress("momz_parent",_momz_parent);
  else t->Branch("momz_parent",_momz_parent,"momz_parent[no_nu]/D");

  if(t->GetBranch("pdgid_parent")) t->SetBranchAddress("pdgid_parent",_pdgid_parent);
  else t->Branch("pdgid_parent",_pdgid_parent,"pdgid_parent[no_nu]/I");

}

#endif

