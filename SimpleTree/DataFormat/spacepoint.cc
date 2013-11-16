#ifndef SPACEPOINT_CC
#define SPACEPOINT_CC

#include "spacepoint.hh"

sps::sps(DATA::DATA_TYPE type) : data_base(type)
{
  initialize();
}

void sps::clear_event()
{

  _nsps=-1;

  for(size_t i=0; i<kMaxSPS; ++i) {
    
    _spsId[i]      = -99999;
    _spsx[i]       = -99999;
    _spsy[i]       = -99999;
    _spsz[i]       = -99999;
    _spsx_sigma[i] = -99999;
    _spsy_sigma[i] = -99999;
    _spsz_sigma[i] = -99999;
    _chi2[i]       = -99999;

  }
  
}

void sps::set_address(TTree* t)
{

  if(t->GetBranch("nsps")) t->SetBranchAddress("nsps",&_nsps);
  else t->Branch("nsps",&_nsps,"nsps/I");

  if(t->GetBranch("spsId")) t->SetBranchAddress("spsId",_spsId);
  else t->Branch("spsId",_spsId,"spsId[nsps]/I");

  if(t->GetBranch("_spsx")) t->SetBranchAddress("spsx",_spsx);
  else t->Branch("spsx",_spsx,"spsx[nsps]/D");

  if(t->GetBranch("_spsy")) t->SetBranchAddress("spsy",_spsy);
  else t->Branch("spsy",_spsy,"spsy[nsps]/D");

  if(t->GetBranch("_spsz")) t->SetBranchAddress("spsz",_spsz);
  else t->Branch("spsz",_spsz,"spsz[nsps]/D");

  if(t->GetBranch("_spsx_sigma")) t->SetBranchAddress("spsx_sigma",_spsx_sigma);
  else t->Branch("spsx_sigma",_spsx_sigma,"spsx_sigma[nsps]/D");

  if(t->GetBranch("_spsy_sigma")) t->SetBranchAddress("spsy_sigma",_spsy_sigma);
  else t->Branch("spsy_sigma",_spsy_sigma,"spsy_sigma[nsps]/D");

  if(t->GetBranch("_spsz_sigma")) t->SetBranchAddress("spsz_sigma",_spsz_sigma);
  else t->Branch("spsz_sigma",_spsz_sigma,"spsz_sigma[nsps]/D");

  if(t->GetBranch("_chi2")) t->SetBranchAddress("chi2",_chi2);
  else t->Branch("chi2",_chi2,"chi2[nsps]/D");

}

#endif
