#ifndef MCTRUTH_CC
#define MCTRUTH_CC

#include "mctruth.hh"

mctruth::mctruth(DATA::DATA_TYPE type) : data_base(type){

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

/// Implementation of event-wise track data clear method
void mctruth::clear_event(bool all)
{
  // Clear data_base variables
  data_base::clear_event(all);

  for(UShort_t index = 0;
      (index < _no_part) || (all && index<DATA::kMaxPrimaries);
      ++index) {

    _pdgid[index] = 0;
    _vtxx[index]  = 0;
    _vtxy[index]  = 0;
    _vtxz[index]  = 0;
    _en[index]    = 0;
    _mom[index]   = 0;
    _momx[index]  = 0;
    _momy[index]  = 0;
    _momz[index]  = 0;
    _mass[index]  = 0;
    _status_code[index] = 0;
    _gen_trackID[index] = 0;
    _no_daughter[index] = 0;
    _mother[index]      = 0;
    
  }
  _no_part = 0;
}

/// Method to add a primary particle
void mctruth::add_primary(Int_t pdgid, Int_t trackID, Int_t status_code, 
			  Int_t ndaughter, Int_t mother,
			  Float_t x, Float_t y, Float_t z,
			  Double_t mass, Double_t en,
			  Double_t mom,  Double_t momx, Double_t momy, Double_t momz)
{
  _pdgid[_no_part]       = pdgid;
  _gen_trackID[_no_part] = trackID;
  _status_code[_no_part] = status_code;
  _no_daughter[_no_part] = ndaughter;
  _mother[_no_part]      = mother;
  _vtxx[_no_part]        = x;
  _vtxy[_no_part]        = y;
  _vtxz[_no_part]        = z;
  _mass[_no_part]        = mass;
  _en[_no_part]          = en;
  _mom[_no_part]         = mom;
  _momx[_no_part]        = momx;
  _momy[_no_part]        = momy;
  _momz[_no_part]        = momz;
  _no_part++;
}

/// Implementation of track data address setter
void mctruth::set_address(TTree* t)
{

  //Set address of the data_base variables
  data_base::set_address(t);

  if(t->GetBranch("no_part")) t->SetBranchAddress("no_part",&_no_part);
  else t->Branch("no_part",&_no_part,"no_part/s");

  if(t->GetBranch("pdgid")) t->SetBranchAddress("pdgid",_pdgid);
  else t->Branch("pdgid",&_pdgid,"pdgid[no_part]/I");

  if(t->GetBranch("en"))  t->SetBranchAddress("en",_en);
  else t->Branch("en",_en,"en[no_part]/D");

  if(t->GetBranch("vtxx")) t->SetBranchAddress("vtxx",_vtxx);
  else t->Branch("vtxx",_vtxx,"vtxx[no_part]/F");

  if(t->GetBranch("vtxy")) t->SetBranchAddress("vtxy",_vtxy);
  else t->Branch("vtxy",_vtxy,"vtxy[no_part]/F");

  if(t->GetBranch("vtxz")) t->SetBranchAddress("vtxz",_vtxz);
  else t->Branch("vtxz",_vtxz,"vtxz[no_part]/F");

  if(t->GetBranch("momx")) t->SetBranchAddress("momx",_momx);
  else t->Branch("momx",_momx,"momx[no_part]/D");

  if(t->GetBranch("momy")) t->SetBranchAddress("momy",_momy);
  else t->Branch("momy",_momy,"momy[no_part]/D");

  if(t->GetBranch("momz")) t->SetBranchAddress("momz",_momz);
  else t->Branch("momz",_momz,"momz[no_part]/D");

  if(t->GetBranch("mom")) t->SetBranchAddress("mom",_mom);
  else t->Branch("mom",_mom,"mom[no_part]/D");

  if(t->GetBranch("status_code")) t->SetBranchAddress("status_code",_status_code);
  else t->Branch("status_code",_status_code,"status_code[no_part]/I");
 
  if(t->GetBranch("mass")) t->SetBranchAddress("mass",_mass);
  else t->Branch("mass",_mass,"mass[no_part]/D");

  if(t->GetBranch("gen_trackID")) t->SetBranchAddress("gen_trackID",_gen_trackID);
  else t->Branch("gen_trackID",_gen_trackID,"gen_trackID[no_part]/I");

  if(t->GetBranch("no_daughter")) t->SetBranchAddress("no_daughter",_no_daughter);
  else t->Branch("no_daughter",_no_daughter,"no_daughter[no_part]/I");

  if(t->GetBranch("mother")) t->SetBranchAddress("motuer",_mother);
  else t->Branch("mother",_mother,"mother[no_part]/I");

}

#endif
