#ifndef MCPART_CC
#define MCPART_CC

#include "mcpart.hh"

//####################################################
mcpart::mcpart(DATA::DATA_TYPE type) : data_base(type)
//####################################################
{
  _name = DATA::DATA_TREE_NAME[type];
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

  _trackID[_no_part]     = trackID;
  _pdgid[_no_part]       = pdgid;
  _status_code[_no_part] = status_code;
  _no_part++;

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

  _mom[_no_part]    = mom;
  _momx[_no_part]   = momx;
  _momy[_no_part]   = momy;
  _momz[_no_part]   = momz;
  _startx[_no_part] = startx;
  _starty[_no_part] = starty;
  _startz[_no_part] = startz;
  _startt[_no_part] = startt;
  _endx[_no_part]   = endx;
  _endy[_no_part]   = endy;
  _endz[_no_part]   = endz;
  _endt[_no_part]   = endt;
  _len[_no_part]    = len;

  _part_info_set=true;
}

//#########################################################################################
void mcpart::set_fv_part_info(Double_t mom, Double_t momx,  Double_t momy,  Double_t momz,
			      Float_t startx, Float_t starty, Float_t startz, Double_t startt,
			      Float_t endx,   Float_t endy,   Float_t endz,   Double_t endt,
			      Float_t len)
//#########################################################################################
{
  _fv_mom[_no_part]    = mom;
  _fv_momx[_no_part]   = momx;
  _fv_momy[_no_part]   = momy;
  _fv_momz[_no_part]   = momz;
  _fv_startx[_no_part] = startx;
  _fv_starty[_no_part] = starty;
  _fv_startz[_no_part] = startz;
  _fv_startt[_no_part] = startt;
  _fv_endx[_no_part]   = endx;
  _fv_endy[_no_part]   = endy;
  _fv_endz[_no_part]   = endz;
  _fv_endt[_no_part]   = endt;
  _fv_len[_no_part]    = len;

  _fv_part_info_set=true;
}

//####################################################
void mcpart::clear_event(bool all)
//####################################################
{
  /// clear base class variables
  data_base::clear_event(all);
  
  for(UShort_t index = 0;
      (index < _no_part) || (all && index<DATA::kMaxParticles);
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

  _no_part=0;
  _part_info_set=false;
  _fv_part_info_set=false;

}

//####################################################
void mcpart::set_address(TTree* t)
//####################################################
{
  // Set base class address
  data_base::set_address(t);

  if(t->GetBranch("no_part")) t->SetBranchAddress("no_part",&_no_part);
  else t->Branch("no_part",&_no_part,"no_part/s");
  
  if(t->GetBranch("pdgid")) t->SetBranchAddress("pdgid",_pdgid);
  else t->Branch("pdgid",_pdgid,"pdgid[no_part]/I");

  if(t->GetBranch("trackID")) t->SetBranchAddress("trackID",_trackID);
  else t->Branch("trackID",_trackID,"trackID[no_part]/b");

  if(t->GetBranch("status_code")) t->SetBranchAddress("status_code",_status_code);
  else t->Branch("status_code",_status_code,"status_code[no_part]/I");

  if(t->GetBranch("momx")) t->SetBranchAddress("momx",_momx);
  else t->Branch("momx",_momx,"momx[no_part]/D");

  if(t->GetBranch("momy")) t->SetBranchAddress("momy",_momy);
  else t->Branch("momy",_momy,"momy[no_part]/D");

  if(t->GetBranch("momz")) t->SetBranchAddress("momz",_momz);
  else t->Branch("momz",_momz,"momz[no_part]/D");

  if(t->GetBranch("startx")) t->SetBranchAddress("startx",_startx);
  else t->Branch("startx",_startx,"startx[no_part]/F");

  if(t->GetBranch("starty")) t->SetBranchAddress("starty",_starty);
  else t->Branch("starty",_starty,"starty[no_part]/F");

  if(t->GetBranch("startz")) t->SetBranchAddress("startz",_startz);
  else t->Branch("startz",_startz,"startz[no_part]/F");

  if(t->GetBranch("endx")) t->SetBranchAddress("endx",_endx);
  else t->Branch("endx",_endx,"endx[no_part]/F");

  if(t->GetBranch("endy")) t->SetBranchAddress("endy",_endy);
  else t->Branch("endy",_endy,"endy[no_part]/F");

  if(t->GetBranch("endz")) t->SetBranchAddress("endz",_endz);
  else t->Branch("endz",_endz,"endz[no_part]/F");

  if(t->GetBranch("mom")) t->SetBranchAddress("mom",_mom);
  else t->Branch("mom",_mom,"mom[no_part]/D");

  if(t->GetBranch("len")) t->SetBranchAddress("len",_len);
  else t->Branch("len",_len,"len[no_part]/F");

  if(t->GetBranch("fv_momx")) t->SetBranchAddress("fv_momx",_fv_momx);
  else t->Branch("fv_momx",_fv_momx,"fv_momx[no_part]/D");

  if(t->GetBranch("fv_momy")) t->SetBranchAddress("fv_momy",_fv_momy);
  else t->Branch("fv_momy",_fv_momy,"fv_momy[no_part]/D");

  if(t->GetBranch("fv_momz")) t->SetBranchAddress("fv_momz",_fv_momz);
  else t->Branch("fv_momz",_fv_momz,"fv_momz[no_part]/D");

  if(t->GetBranch("fv_startx")) t->SetBranchAddress("fv_startx",_fv_startx);
  else t->Branch("fv_startx",_fv_startx,"fv_startx[no_part]/F");

  if(t->GetBranch("fv_starty")) t->SetBranchAddress("fv_starty",_fv_starty);
  else t->Branch("fv_starty",_fv_starty,"fv_starty[no_part]/F");

  if(t->GetBranch("fv_startz")) t->SetBranchAddress("fv_startz",_fv_startz);
  else t->Branch("fv_startz",_fv_startz,"fv_startz[no_part]/F");

  if(t->GetBranch("fv_endx")) t->SetBranchAddress("fv_endx",_fv_endx);
  else t->Branch("fv_endx",_fv_endx,"fv_endx[no_part]/F");

  if(t->GetBranch("fv_endy")) t->SetBranchAddress("fv_endy",_fv_endy);
  else t->Branch("fv_endy",_fv_endy,"fv_endy[no_part]/F");

  if(t->GetBranch("fv_endz")) t->SetBranchAddress("fv_endz",_fv_endz);
  else t->Branch("fv_endz",_fv_endz,"fv_endz[no_part]/F");

  if(t->GetBranch("fv_mom")) t->SetBranchAddress("fv_mom",_fv_mom);
  else t->Branch("fv_mom",_fv_mom,"fv_mom[no_part]/D");

  if(t->GetBranch("fv_len")) t->SetBranchAddress("fv_len",_fv_len);
  else t->Branch("fv_len",_fv_len,"fv_len[no_part]/F");

}

#endif
