#ifndef SPACEPOINT_CC
#define SPACEPOINT_CC

#include "spacepoint.hh"

sps::sps(DATA::DATA_TYPE type) : data_base(type)
{
  initialize();
}

void sps::add_spacepoint(UShort_t spsID, 
			 Float_t x,  Float_t y,  Float_t z,
			 Float_t ex, Float_t ey, Float_t ez,
			 Float_t chi2)
{
  _spsId[_no_sps] = spsID;
  _vtxx[_no_sps]  = x;
  _vtxy[_no_sps]  = y;
  _vtxz[_no_sps]  = z;
  _vtxx_sigma[_no_sps] = ex;
  _vtxy_sigma[_no_sps] = ey;
  _vtxz_sigma[_no_sps] = ez;
  _chi2[_no_sps]  = chi2;
  _no_sps++;
}


void sps::clear_event(bool all)
{
  // Clear data_base variables
  data_base::clear_event(all);

  for(UShort_t index = 0;
      (index < _no_sps) || (all && index<DATA::kMaxHits);
      ++index) {

    _spsId[index]      = 0;
    _vtxx[index]       = 0;
    _vtxy[index]       = 0;
    _vtxz[index]       = 0;
    _vtxx_sigma[index] = 0;
    _vtxy_sigma[index] = 0;
    _vtxz_sigma[index] = 0;
    _chi2[index]       = 0;

  }
  _no_sps=0;

}

void sps::set_address(TTree* t)
{

  if(t->GetBranch("nsps")) t->SetBranchAddress("nsps",&_no_sps);
  else t->Branch("nsps",&_no_sps,"nsps/s");

  if(t->GetBranch("spsId")) t->SetBranchAddress("spsId",_spsId);
  else t->Branch("spsId",_spsId,"spsId[no_sps]/s");

  if(t->GetBranch("_vtxx")) t->SetBranchAddress("vtxx",_vtxx);
  else t->Branch("vtxx",_vtxx,"vtxx[no_sps]/F");

  if(t->GetBranch("_vtxy")) t->SetBranchAddress("vtxy",_vtxy);
  else t->Branch("vtxy",_vtxy,"vtxy[no_sps]/F");

  if(t->GetBranch("_vtxz")) t->SetBranchAddress("vtxz",_vtxz);
  else t->Branch("vtxz",_vtxz,"vtxz[no_sps]/F");

  if(t->GetBranch("_vtxx_sigma")) t->SetBranchAddress("vtxx_sigma",_vtxx_sigma);
  else t->Branch("vtxx_sigma",_vtxx_sigma,"vtxx_sigma[no_sps]/F");

  if(t->GetBranch("_vtxy_sigma")) t->SetBranchAddress("vtxy_sigma",_vtxy_sigma);
  else t->Branch("vtxy_sigma",_vtxy_sigma,"vtxy_sigma[no_sps]/F");

  if(t->GetBranch("_vtxz_sigma")) t->SetBranchAddress("vtxz_sigma",_vtxz_sigma);
  else t->Branch("vtxz_sigma",_vtxz_sigma,"vtxz_sigma[no_sps]/F");

  if(t->GetBranch("_chi2")) t->SetBranchAddress("chi2",_chi2);
  else t->Branch("chi2",_chi2,"chi2[no_sps]/F");

}

#endif
