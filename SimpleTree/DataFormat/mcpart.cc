#ifndef MCPART_CC
#define MCPART_CC

#include "mcpart.hh"

//####################################################
mcpart::mcpart() : data_base(DATA::MCParticle)
//####################################################
{
  _name = DATA::DATA_TREE_NAME[DATA::MCParticle];
  initialize();
}

//#########################################################################
void mcpart::add_particle(UShort_t trackID, Int_t pdgid, Int_t status_code)
//#########################################################################
{
  if(!_part_info_set) {

    print(MSG::ERROR,__FUNCTION__,
	  "Must call set_part_info() BEFORE add_particle to set particle information correctly!");

    return;
  }

  if(!_fv_part_info_set) {
    
    print(MSG::ERROR,__FUNCTION__,
	  "Must call set_fv_part_info() BEFORE add_particle to set particle information correctly!");

    return;
  }

  _trackID[_num_part]     = trackID;
  _pdgid[_num_part]       = pdgid;
  _status_code[_num_part] = status_code;
  _num_part++;

  _part_info_set=false;
  _fv_part_info_set=false;
}

//#########################################################################################
void mcpart::set_part_info(Double_t mom, Double_t momx,  Double_t momy,  Double_t momz,
			   Float_t startx, Float_t starty, Float_t startz, Double_t startt,
			   Float_t endx,   Float_t endy,   Float_t endz,   Double_t endt,
			   Float_t len)
//#########################################################################################
{

  _mom[_num_part]    = mom;
  _momx[_num_part]   = momx;
  _momy[_num_part]   = momy;
  _momz[_num_part]   = momz;
  _startx[_num_part] = startx;
  _starty[_num_part] = starty;
  _startz[_num_part] = startz;
  _startt[_num_part] = startt;
  _endx[_num_part]   = endx;
  _endy[_num_part]   = endy;
  _endz[_num_part]   = endz;
  _endt[_num_part]   = endt;
  _len[_num_part]    = len;

  _part_info_set=true;
}

//#########################################################################################
void mcpart::set_fv_part_info(Double_t mom, Double_t momx,  Double_t momy,  Double_t momz,
			      Float_t startx, Float_t starty, Float_t startz, Double_t startt,
			      Float_t endx,   Float_t endy,   Float_t endz,   Double_t endt,
			      Float_t len)
//#########################################################################################
{
  _fv_mom[_num_part]    = mom;
  _fv_momx[_num_part]   = momx;
  _fv_momy[_num_part]   = momy;
  _fv_momz[_num_part]   = momz;
  _fv_startx[_num_part] = startx;
  _fv_starty[_num_part] = starty;
  _fv_startz[_num_part] = startz;
  _fv_startt[_num_part] = startt;
  _fv_endx[_num_part]   = endx;
  _fv_endy[_num_part]   = endy;
  _fv_endz[_num_part]   = endz;
  _fv_endt[_num_part]   = endt;
  _fv_len[_num_part]    = len;

  _fv_part_info_set=true;
}

//####################################################
void mcpart::clear_event(bool all)
//####################################################
{
  /// clear base class variables
  data_base::clear_event(all);
  
  for(UShort_t index = 0;
      (index < _num_part) || (all && index<DATA::kMaxParticles);
      ++index) {
    _trackID[index]     = 0;    
    _pdgid[index]       = 0;      
    _status_code[index] = 0;
    
    _momx[index]   = 0;       
    _momy[index]   = 0;       
    _momz[index]   = 0;       
    _startx[index] = 0;     
    _starty[index] = 0;     
    _startz[index] = 0;     
    _startt[index] = 0;     
    _endx[index]   = 0;       
    _endy[index]   = 0;       
    _endz[index]   = 0;       
    _endt[index]   = 0;       
    _mom[index]    = 0;        
    _len[index]    = 0;        
    
    _fv_momx[index]   = 0;    
    _fv_momy[index]   = 0;    
    _fv_momz[index]   = 0;    
    _fv_startx[index] = 0;  
    _fv_starty[index] = 0;  
    _fv_startz[index] = 0;  
    _fv_startt[index] = 0;  
    _fv_endx[index]   = 0;    
    _fv_endy[index]   = 0;    
    _fv_endz[index]   = 0;    
    _fv_endt[index]   = 0;    
    _fv_mom[index]    = 0;     
    _fv_len[index]    = 0;     
  }

  _num_part=0;
  _part_info_set=false;
  _fv_part_info_set=false;

}

//####################################################
Bool_t mcpart::set_address(TTree* t,Bool_t create)
//####################################################
{
  print(MSG::DEBUG,__PRETTY_FUNCTION__,Form("%s called...",_name.c_str()));
  // Set base class address
  Bool_t exist = data_base::set_address(t,create);

  if(t->GetBranch(Form("num_%s",_name.c_str()))) t->SetBranchAddress(Form("num_%s",_name.c_str()),&_num_part);
  else {
    exist = false;
    if(create) t->Branch(Form("num_%s",_name.c_str()),&_num_part,Form("num_%s/s",_name.c_str()));
  }
  
  if(t->GetBranch(Form("%s_pdgid",_name.c_str()))) t->SetBranchAddress(Form("%s_pdgid",_name.c_str()),_pdgid);
  else if(create) t->Branch(Form("%s_pdgid",_name.c_str()),_pdgid,Form("%s_pdgid[num_%s]/I",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_trackID",_name.c_str()))) t->SetBranchAddress(Form("%s_trackID",_name.c_str()),_trackID);
  else if(create) t->Branch(Form("%s_trackID",_name.c_str()),_trackID,Form("%s_trackID[num_%s]/s",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_status_code",_name.c_str()))) t->SetBranchAddress(Form("%s_status_code",_name.c_str()),_status_code);
  else if(create) t->Branch(Form("%s_status_code",_name.c_str()),_status_code,Form("%s_status_code[num_%s]/I",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_momx",_name.c_str()))) t->SetBranchAddress(Form("%s_momx",_name.c_str()),_momx);
  else if(create) t->Branch(Form("%s_momx",_name.c_str()),_momx,Form("%s_momx[num_%s]/D",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_momy",_name.c_str()))) t->SetBranchAddress(Form("%s_momy",_name.c_str()),_momy);
  else if(create) t->Branch(Form("%s_momy",_name.c_str()),_momy,Form("%s_momy[num_%s]/D",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_momz",_name.c_str()))) t->SetBranchAddress(Form("%s_momz",_name.c_str()),_momz);
  else if(create) t->Branch(Form("%s_momz",_name.c_str()),_momz,Form("%s_momz[num_%s]/D",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_startx",_name.c_str()))) t->SetBranchAddress(Form("%s_startx",_name.c_str()),_startx);
  else if(create) t->Branch(Form("%s_startx",_name.c_str()),_startx,Form("%s_startx[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_starty",_name.c_str()))) t->SetBranchAddress(Form("%s_starty",_name.c_str()),_starty);
  else if(create) t->Branch(Form("%s_starty",_name.c_str()),_starty,Form("%s_starty[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_startz",_name.c_str()))) t->SetBranchAddress(Form("%s_startz",_name.c_str()),_startz);
  else if(create) t->Branch(Form("%s_startz",_name.c_str()),_startz,Form("%s_startz[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_endx",_name.c_str()))) t->SetBranchAddress(Form("%s_endx",_name.c_str()),_endx);
  else if(create) t->Branch(Form("%s_endx",_name.c_str()),_endx,Form("%s_endx[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_endy",_name.c_str()))) t->SetBranchAddress(Form("%s_endy",_name.c_str()),_endy);
  else if(create) t->Branch(Form("%s_endy",_name.c_str()),_endy,Form("%s_endy[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_endz",_name.c_str()))) t->SetBranchAddress(Form("%s_endz",_name.c_str()),_endz);
  else if(create) t->Branch(Form("%s_endz",_name.c_str()),_endz,Form("%s_endz[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_mom",_name.c_str()))) t->SetBranchAddress(Form("%s_mom",_name.c_str()),_mom);
  else if(create) t->Branch(Form("%s_mom",_name.c_str()),_mom,Form("%s_mom[num_%s]/D",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_len",_name.c_str()))) t->SetBranchAddress(Form("%s_len",_name.c_str()),_len);
  else if(create) t->Branch(Form("%s_len",_name.c_str()),_len,Form("%s_len[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_fv_momx",_name.c_str()))) t->SetBranchAddress(Form("%s_fv_momx",_name.c_str()),_fv_momx);
  else if(create) t->Branch(Form("%s_fv_momx",_name.c_str()),_fv_momx,Form("%s_fv_momx[num_%s]/D",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_fv_momy",_name.c_str()))) t->SetBranchAddress(Form("%s_fv_momy",_name.c_str()),_fv_momy);
  else if(create) t->Branch(Form("%s_fv_momy",_name.c_str()),_fv_momy,Form("%s_fv_momy[num_%s]/D",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_fv_momz",_name.c_str()))) t->SetBranchAddress(Form("%s_fv_momz",_name.c_str()),_fv_momz);
  else if(create) t->Branch(Form("%s_fv_momz",_name.c_str()),_fv_momz,Form("%s_fv_momz[num_%s]/D",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_fv_startx",_name.c_str()))) t->SetBranchAddress(Form("%s_fv_startx",_name.c_str()),_fv_startx);
  else if(create) t->Branch(Form("%s_fv_startx",_name.c_str()),_fv_startx,Form("%s_fv_startx[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_fv_starty",_name.c_str()))) t->SetBranchAddress(Form("%s_fv_starty",_name.c_str()),_fv_starty);
  else if(create) t->Branch(Form("%s_fv_starty",_name.c_str()),_fv_starty,Form("%s_fv_starty[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_fv_startz",_name.c_str()))) t->SetBranchAddress(Form("%s_fv_startz",_name.c_str()),_fv_startz);
  else if(create) t->Branch(Form("%s_fv_startz",_name.c_str()),_fv_startz,Form("%s_fv_startz[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_fv_endx",_name.c_str()))) t->SetBranchAddress(Form("%s_fv_endx",_name.c_str()),_fv_endx);
  else if(create) t->Branch(Form("%s_fv_endx",_name.c_str()),_fv_endx,Form("%s_fv_endx[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_fv_endy",_name.c_str()))) t->SetBranchAddress(Form("%s_fv_endy",_name.c_str()),_fv_endy);
  else if(create) t->Branch(Form("%s_fv_endy",_name.c_str()),_fv_endy,Form("%s_fv_endy[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_fv_endz",_name.c_str()))) t->SetBranchAddress(Form("%s_fv_endz",_name.c_str()),_fv_endz);
  else if(create) t->Branch(Form("%s_fv_endz",_name.c_str()),_fv_endz,Form("%s_fv_endz[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_fv_mom",_name.c_str()))) t->SetBranchAddress(Form("%s_fv_mom",_name.c_str()),_fv_mom);
  else if(create) t->Branch(Form("%s_fv_mom",_name.c_str()),_fv_mom,Form("%s_fv_mom[num_%s]/D",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_fv_len",_name.c_str()))) t->SetBranchAddress(Form("%s_fv_len",_name.c_str()),_fv_len);
  else if(create) t->Branch(Form("%s_fv_len",_name.c_str()),_fv_len,Form("%s_fv_len[num_%s]/F",_name.c_str(),_name.c_str()));

  return exist;
}

#endif
