#ifndef TRACK_CC
#define TRACK_CC

#include "track.hh"

track::track(DATA::DATA_TYPE type) : data_base()
{
  if(type!=DATA::Bezier && 
     type!=DATA::Kalman3DSPS &&
     type!=DATA::Kalman3DHit &&
     type!=DATA::Track) {
    
    print(MSG::ERROR,__FUNCTION__,
	  Form("Invalid data type for track: %d ... setting default DATA::Track type",type));
    _type = DATA::Track;
  }else
    _type = type;

  _name = DATA::DATA_TREE_NAME[type];
  initialize();
}

void track::clear_event()
{
  // Clear data_base data
  data_base::clear_event();

  _ntracks = -1;
  // Clear track data
  for(size_t i=0; i<kMaxTrack; ++i){

    _trackId[i] = -99999;
    _startx[i] = -99999;   
    _starty[i] = -99999;   
    _startz[i] = -99999;   
    _startd[i] = -99999;   
    _endx[i] = -99999;     
    _endy[i] = -99999;     
    _endz[i] = -99999;     
    _endd[i] = -99999;     
    _theta[i] = -99999;    
    _phi[i] = -99999;      
    _theta_xz[i] = -99999; 
    _theta_yz[i] = -99999; 
    _mom[i] = -99999;      
    _len[i] = -99999;      

  } // clear loop ends

}

void track::set_address(TTree* t)
{
  // base class address set
  data_base::set_address(t);

  // track class address set
  if(t->GetBranch("ntracks"))  t->SetBranchAddress("ntracks",&_ntracks);
  else t->Branch("ntracks", &_ntracks, "ntracks/I");

  if(t->GetBranch("trackId"))  t->SetBranchAddress("trackId",&_trackId);
  else t->Branch("trackId", &_trackId, "trackId[ntracks]/I");

  if(t->GetBranch("startx"))   t->SetBranchAddress("startx",&_startx);
  else t->Branch("startx", &_startx, "startx[ntracks]/D");

  if(t->GetBranch("starty"))   t->SetBranchAddress("starty",&_starty);
  else t->Branch("starty", &_starty, "starty[ntracks]/D");

  if(t->GetBranch("startz"))   t->SetBranchAddress("startz",&_startz);
  else t->Branch("startz", &_startz, "startz[ntracks]/D");

  if(t->GetBranch("startd"))   t->SetBranchAddress("startd",&_startd);
  else t->Branch("startd", &_startd, "startd[ntracks]/D");

  if(t->GetBranch("endx"))   t->SetBranchAddress("endx",&_endx);
  else t->Branch("endx", &_endx, "endx[ntracks]/D");

  if(t->GetBranch("endy"))   t->SetBranchAddress("endy",&_endy);
  else t->Branch("endy", &_endy, "endy[ntracks]/D");

  if(t->GetBranch("endz"))   t->SetBranchAddress("endz",&_endz);
  else t->Branch("endz", &_endz, "endz[ntracks]/D");

  if(t->GetBranch("endd"))   t->SetBranchAddress("endd",&_endd);
  else t->Branch("endd", &_endd, "endd[ntracks]/D");

  if(t->GetBranch("theta"))  t->SetBranchAddress("theta",&_theta);
  else  t->Branch("theta", &_theta, "theta[ntracks]/D");

  if(t->GetBranch("phi"))    t->SetBranchAddress("phi",&_phi);
  else t->Branch("phi", &_phi, "phi[ntracks]/D");

  if(t->GetBranch("theta_xz")) t->SetBranchAddress("theta_xz",&_theta_xz);
  else t->Branch("theta_xz", &_theta_xz, "theta_xz[ntracks]/D");

  if(t->GetBranch("theta_yz")) t->SetBranchAddress("theta_yz",&_theta_yz);
  else t->Branch("ttheta_yz", &_theta_yz, "theta_yz[ntracks]/D");

  if(t->GetBranch("mom"))    t->SetBranchAddress("mom",&_mom);
  else t->Branch("mom", &_mom, "mom[ntracks]/D");

  if(t->GetBranch("len"))    t->SetBranchAddress("len",&_len);
  else t->Branch("len", &_len, "len[ntracks]/D");      

}

#endif
