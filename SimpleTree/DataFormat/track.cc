#ifndef TRACK_CC
#define TRACK_CC

#include "track.hh"

track::track(DATA::DATA_TYPE type) : data_base(type)
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


void track::add_track(UShort_t trackID, 
		      Float_t  startx, Float_t  starty,   Float_t  startz,   Float_t  startd,
		      Float_t  endx,   Float_t  endy,     Float_t  endz,     Float_t  endd,
		      Float_t  theta,  Float_t  theta_xz, Float_t  theta_yz, 
		      Double_t mom,    Double_t len)
{
  _trackID[_no_tracks]  = trackID;
  _startx[_no_tracks]   = startx;
  _starty[_no_tracks]   = starty;
  _startz[_no_tracks]   = startz;
  _startd[_no_tracks]   = startd;
  _endx[_no_tracks]     = endx;
  _endy[_no_tracks]     = endy;
  _endz[_no_tracks]     = endz;
  _endd[_no_tracks]     = endd;
  _theta[_no_tracks]    = theta;
  _theta_xz[_no_tracks] = theta_xz;
  _theta_yz[_no_tracks] = theta_yz;
  _mom[_no_tracks]      = mom;
  _len[_no_tracks]      = len;
  _no_tracks++;
}

void track::add_trajectory(UShort_t vtxID, 
			   Float_t x,  Float_t y,  Float_t z,
			   Double_t px, Double_t py, Double_t pz)
{
  _vtxID[_no_points] = vtxID;
  _vtxx[_no_points]  = x;
  _vtxy[_no_points]  = y;
  _vtxz[_no_points]  = z;
  _momx[_no_points]  = px;
  _momy[_no_points]  = py;
  _momz[_no_points]  = pz;
  _no_points++;
}

void track::clear_event(bool all)
{
  // Clear data_base data
  data_base::clear_event(all);

  // Clear track data
  for(UShort_t index = 0;
      (index < _no_tracks) || (all && index<DATA::kMaxHits);
      ++index) {

    _trackID[index]  = 0;
    _startx[index]   = 0;   
    _starty[index]   = 0;   
    _startz[index]   = 0;   
    _startd[index]   = 0;   
    _endx[index]     = 0;     
    _endy[index]     = 0;     
    _endz[index]     = 0;     
    _endd[index]     = 0;     
    _theta[index]    = 0;    
    _phi[index]      = 0;      
    _theta_xz[index] = 0; 
    _theta_yz[index] = 0; 
    _mom[index]      = 0;      
    _len[index]      = 0;      

  }
  _no_tracks = 0;

  for(UShort_t index = 0;
      (index < _no_points) || (all && index<DATA::kMaxHits);
      ++index) {
    
    _vtxID[index] = 0;
    _vtxx[index]  = 0;
    _vtxy[index]  = 0;
    _vtxz[index]  = 0;
    _momx[index]  = 0;
    _momy[index]  = 0;
    _momz[index]  = 0;
    
  } // clear loop ends
  _no_points = 0;

}

void track::set_address(TTree* t)
{
  // base class address set
  data_base::set_address(t);

  // track class address set
  if(t->GetBranch("no_tracks"))  t->SetBranchAddress("no_tracks",&_no_tracks);
  else t->Branch("no_tracks", &_no_tracks, "no_tracks/s");

  if(t->GetBranch("trackID"))  t->SetBranchAddress("trackID",_trackID);
  else t->Branch("trackID", _trackID, "trackID[no_tracks]/s");

  if(t->GetBranch("startx"))   t->SetBranchAddress("startx",_startx);
  else t->Branch("startx", _startx, "startx[no_tracks]/F");

  if(t->GetBranch("starty"))   t->SetBranchAddress("starty",_starty);
  else t->Branch("starty", _starty, "starty[no_tracks]/F");

  if(t->GetBranch("startz"))   t->SetBranchAddress("startz",_startz);
  else t->Branch("startz", _startz, "startz[no_tracks]/F");

  if(t->GetBranch("startd"))   t->SetBranchAddress("startd",_startd);
  else t->Branch("startd", _startd, "startd[no_tracks]/F");

  if(t->GetBranch("endx"))   t->SetBranchAddress("endx",_endx);
  else t->Branch("endx", _endx, "endx[no_tracks]/F");

  if(t->GetBranch("endy"))   t->SetBranchAddress("endy",_endy);
  else t->Branch("endy", _endy, "endy[no_tracks]/F");

  if(t->GetBranch("endz"))   t->SetBranchAddress("endz",_endz);
  else t->Branch("endz", _endz, "endz[no_tracks]/F");

  if(t->GetBranch("endd"))   t->SetBranchAddress("endd",_endd);
  else t->Branch("endd", _endd, "endd[no_tracks]/F");

  if(t->GetBranch("theta"))  t->SetBranchAddress("theta",_theta);
  else  t->Branch("theta", _theta, "theta[no_tracks]/F");

  if(t->GetBranch("phi"))    t->SetBranchAddress("phi",_phi);
  else t->Branch("phi", _phi, "phi[no_tracks]/F");

  if(t->GetBranch("theta_xz")) t->SetBranchAddress("theta_xz",_theta_xz);
  else t->Branch("theta_xz", _theta_xz, "theta_xz[no_tracks]/F");

  if(t->GetBranch("theta_yz")) t->SetBranchAddress("theta_yz",_theta_yz);
  else t->Branch("ttheta_yz", _theta_yz, "theta_yz[no_tracks]/F");

  if(t->GetBranch("mom"))    t->SetBranchAddress("mom",_mom);
  else t->Branch("mom", _mom, "mom[no_tracks]/D");

  if(t->GetBranch("len"))    t->SetBranchAddress("len",_len);
  else t->Branch("len", _len, "len[no_tracks]/D");      

  //--- addition ---//
  if(t->GetBranch("no_points")) t->SetBranchAddress("no_points",&_no_points);
  else t->Branch("no_points",&_no_points,"no_points/s");

  if(t->GetBranch("vtxID")) t->SetBranchAddress("vtxID",_vtxID);
  else t->Branch("vtxID",_vtxID,"vtxID/s");
  
  if(t->GetBranch("vtxx")) t->SetBranchAddress("vtxx",_vtxx);
  else t->Branch("vtxx",_vtxx,"vtxx[no_points]/F");

  if(t->GetBranch("vtxy")) t->SetBranchAddress("vtxy",_vtxy);
  else t->Branch("vtxy",_vtxy,"vtxy[no_points]/F");

  if(t->GetBranch("vtxz")) t->SetBranchAddress("vtxz",_vtxz);
  else t->Branch("vtxz",_vtxz,"vtxz[no_points]/F");

  if(t->GetBranch("momx")) t->SetBranchAddress("momx",_momx);
  else t->Branch("momx",_momx,"momx[no_points]/D");

  if(t->GetBranch("momy")) t->SetBranchAddress("momy",_momy);
  else t->Branch("momy",_momy,"momy[no_points]/D");

  if(t->GetBranch("momz")) t->SetBranchAddress("momz",_momz);
  else t->Branch("momz",_momz,"momz[no_points]/D");

}

#endif
