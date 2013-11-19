#ifndef SPACEPOINT_CC
#define SPACEPOINT_CC

#include "spacepoint.hh"

//#####################################################################
sps::sps() : data_base(DATA::SpacePoint)
//#####################################################################
{
  _name=DATA::DATA_TREE_NAME[DATA::SpacePoint];
  initialize();
}

//#####################################################################
void sps::add_spacepoint(UShort_t spsID, 
			 Float_t x,  Float_t y,  Float_t z,
			 Float_t ex, Float_t ey, Float_t ez,
			 Float_t chi2)
//#####################################################################
{
  _spsID[_num_sps] = spsID;
  _vtxx[_num_sps]  = x;
  _vtxy[_num_sps]  = y;
  _vtxz[_num_sps]  = z;
  _vtxx_sigma[_num_sps] = ex;
  _vtxy_sigma[_num_sps] = ey;
  _vtxz_sigma[_num_sps] = ez;
  _chi2[_num_sps]  = chi2;
  _num_sps++;
}

//#####################################################################
void sps::clear_event(bool all)
//#####################################################################
{
  // Clear data_base variables
  data_base::clear_event(all);

  for(UShort_t index = 0;
      (index < _num_sps) || (all && index<DATA::kMaxSpacePoints);
      ++index) {

    _spsID[index]      = 0;
    _vtxx[index]       = 0;
    _vtxy[index]       = 0;
    _vtxz[index]       = 0;
    _vtxx_sigma[index] = 0;
    _vtxy_sigma[index] = 0;
    _vtxz_sigma[index] = 0;
    _chi2[index]       = 0;

  }
  _num_sps=0;

}

//#####################################################################
void sps::set_address(TTree* t)
//#####################################################################
{

  //Set address of the data_base variables
  data_base::set_address(t);

  if(t->GetBranch(Form("num_%s",_name.c_str()))) t->SetBranchAddress(Form("num_%s",_name.c_str()),&_num_sps);
  else t->Branch(Form("num_%s",_name.c_str()),&_num_sps,Form("num_%s/s",_name.c_str()));

  if(t->GetBranch(Form("%s_spsID",_name.c_str()))) t->SetBranchAddress(Form("%s_spsID",_name.c_str()),_spsID);
  else t->Branch(Form("%s_spsID",_name.c_str()),_spsID,Form("%s_spsID[num_sps]/s",_name.c_str()));

  if(t->GetBranch(Form("%s_vtxx",_name.c_str()))) t->SetBranchAddress(Form("%s_vtxx",_name.c_str()),_vtxx);
  else t->Branch(Form("%s_vtxx",_name.c_str()),_vtxx,Form("%s_vtxx[num_sps]/F",_name.c_str()));

  if(t->GetBranch(Form("%s_vtxy",_name.c_str()))) t->SetBranchAddress(Form("%s_vtxy",_name.c_str()),_vtxy);
  else t->Branch(Form("%s_vtxy",_name.c_str()),_vtxy,Form("%s_vtxy[num_sps]/F",_name.c_str()));

  if(t->GetBranch(Form("%s_vtxz",_name.c_str()))) t->SetBranchAddress(Form("%s_vtxz",_name.c_str()),_vtxz);
  else t->Branch(Form("%s_vtxz",_name.c_str()),_vtxz,Form("%s_vtxz[num_sps]/F",_name.c_str()));

  if(t->GetBranch(Form("%s_vtxx_sigma",_name.c_str()))) t->SetBranchAddress(Form("%s_vtxx_sigma",_name.c_str()),_vtxx_sigma);
  else t->Branch(Form("%s_vtxx_sigma",_name.c_str()),_vtxx_sigma,Form("%s_vtxx_sigma[num_sps]/F",_name.c_str()));

  if(t->GetBranch(Form("%s_vtxy_sigma",_name.c_str()))) t->SetBranchAddress(Form("%s_vtxy_sigma",_name.c_str()),_vtxy_sigma);
  else t->Branch(Form("%s_vtxy_sigma",_name.c_str()),_vtxy_sigma,Form("%s_vtxy_sigma[num_sps]/F",_name.c_str()));

  if(t->GetBranch(Form("%s_vtxz_sigma",_name.c_str()))) t->SetBranchAddress(Form("%s_vtxz_sigma",_name.c_str()),_vtxz_sigma);
  else t->Branch(Form("%s_vtxz_sigma",_name.c_str()),_vtxz_sigma,Form("%s_vtxz_sigma[num_sps]/F",_name.c_str()));

  if(t->GetBranch(Form("%s_chi2",_name.c_str()))) t->SetBranchAddress(Form("%s_chi2",_name.c_str()),_chi2);
  else t->Branch(Form("%s_chi2",_name.c_str()),_chi2,Form("%s_chi2[num_sps]/F",_name.c_str()));

}

#endif
