#ifndef TRACK_CC
#define TRACK_CC

#include "track.hh"

//#####################################################################
track::track(DATA::DATA_TYPE type) : data_base(type)
//#####################################################################
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

//###########################################################################################
void track::add_track(UShort_t trackID, 
		      Float_t  startx, Float_t  starty,   Float_t  startz,   Float_t  startd,
		      Float_t  endx,   Float_t  endy,     Float_t  endz,     Float_t  endd,
		      Float_t  theta,  Double_t mom,    Double_t len)
//###########################################################################################
{
  _trackID[_num_tracks]  = trackID;
  _startx[_num_tracks]   = startx;
  _starty[_num_tracks]   = starty;
  _startz[_num_tracks]   = startz;
  _startd[_num_tracks]   = startd;
  _endx[_num_tracks]     = endx;
  _endy[_num_tracks]     = endy;
  _endz[_num_tracks]     = endz;
  _endd[_num_tracks]     = endd;
  _theta[_num_tracks]    = theta;
  _startmom[_num_tracks] = mom;
  _len[_num_tracks]      = len;
  _num_tracks++;
}

//#####################################################################
void track::add_trajectory(UShort_t vtxID, 
			   Float_t x,  Float_t y,  Float_t z,
			   Double_t momx,  Double_t momy,  Double_t momz)
//#####################################################################
{
  _vtxID[_num_points] = vtxID;
  _vtxx[_num_points]  = x;
  _vtxy[_num_points]  = y;
  _vtxz[_num_points]  = z;
  _momx[_num_points]  = momx;
  _momy[_num_points]  = momy;
  _momz[_num_points]  = momz;
  _num_points++;
}

//#####################################################################
void track::clear_event(bool all)
//#####################################################################
{
  // Clear data_base data
  data_base::clear_event(all);
  
  // Clear track data
  for(UShort_t index = 0;
      (index < _num_tracks) || (all && index<DATA::kMaxTracks);
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
    _startmom[index] = 0;      
    _len[index]      = 0;      

  }
  _num_tracks = 0;

  for(UShort_t index = 0;
      (index < _num_points) || (all && index<DATA::kMaxTrackPoints);
      ++index) {
    
    _vtxID[index] = 0;
    _vtxx[index]  = 0;
    _vtxy[index]  = 0;
    _vtxz[index]  = 0;
    _momx[index]  = 0;
    _momy[index]  = 0;
    _momz[index]  = 0;
    
  } // clear loop ends
  _num_points = 0;

}

//#####################################################################
void track::set_address(TTree* t)
//#####################################################################
{
  // base class address set
  data_base::set_address(t);

  // track class address set
  if(t->GetBranch(Form("num_%s",_name.c_str())))  t->SetBranchAddress(Form("num_%s",_name.c_str()),&_num_tracks);
  else t->Branch(Form("num_%s",_name.c_str()), &_num_tracks, Form("num_%s/s",_name.c_str()));

  if(t->GetBranch(Form("%s_trackID",_name.c_str())))  t->SetBranchAddress(Form("%s_trackID",_name.c_str()),_trackID);
  else t->Branch(Form("%s_trackID",_name.c_str()), _trackID, Form("%s_trackID[num_tracks]/s",_name.c_str()));

  if(t->GetBranch(Form("%s_startx",_name.c_str())))   t->SetBranchAddress(Form("%s_startx",_name.c_str()),_startx);
  else t->Branch(Form("%s_startx",_name.c_str()), _startx, Form("%s_startx[num_tracks]/F",_name.c_str()));

  if(t->GetBranch(Form("%s_starty",_name.c_str())))   t->SetBranchAddress(Form("%s_starty",_name.c_str()),_starty);
  else t->Branch(Form("%s_starty",_name.c_str()), _starty, Form("%s_starty[num_tracks]/F",_name.c_str()));

  if(t->GetBranch(Form("%s_startz",_name.c_str())))   t->SetBranchAddress(Form("%s_startz",_name.c_str()),_startz);
  else t->Branch(Form("%s_startz",_name.c_str()), _startz, Form("%s_startz[num_tracks]/F",_name.c_str()));

  if(t->GetBranch(Form("%s_startd",_name.c_str())))   t->SetBranchAddress(Form("%s_startd",_name.c_str()),_startd);
  else t->Branch(Form("%s_startd",_name.c_str()), _startd, Form("%s_startd[num_tracks]/F",_name.c_str()));

  if(t->GetBranch(Form("%s_endx",_name.c_str())))   t->SetBranchAddress(Form("%s_endx",_name.c_str()),_endx);
  else t->Branch(Form("%s_endx",_name.c_str()), _endx, Form("%s_endx[num_tracks]/F",_name.c_str()));

  if(t->GetBranch(Form("%s_endy",_name.c_str())))   t->SetBranchAddress(Form("%s_endy",_name.c_str()),_endy);
  else t->Branch(Form("%s_endy",_name.c_str()), _endy, Form("%s_endy[num_tracks]/F",_name.c_str()));

  if(t->GetBranch(Form("%s_endz",_name.c_str())))   t->SetBranchAddress(Form("%s_endz",_name.c_str()),_endz);
  else t->Branch(Form("%s_endz",_name.c_str()), _endz, Form("%s_endz[num_tracks]/F",_name.c_str()));

  if(t->GetBranch(Form("%s_endd",_name.c_str())))   t->SetBranchAddress(Form("%s_endd",_name.c_str()),_endd);
  else t->Branch(Form("%s_endd",_name.c_str()), _endd, Form("%s_endd[num_tracks]/F",_name.c_str()));

  if(t->GetBranch(Form("%s_theta",_name.c_str())))  t->SetBranchAddress(Form("%s_theta",_name.c_str()),_theta);
  else  t->Branch(Form("%s_theta",_name.c_str()), _theta, Form("%s_theta[num_tracks]/F",_name.c_str()));

  if(t->GetBranch(Form("%s_phi",_name.c_str())))    t->SetBranchAddress(Form("%s_phi",_name.c_str()),_phi);
  else t->Branch(Form("%s_phi",_name.c_str()), _phi, Form("%s_phi[num_tracks]/F",_name.c_str()));

  if(t->GetBranch(Form("%s_startmom",_name.c_str())))    t->SetBranchAddress(Form("%s_startmom",_name.c_str()),_startmom);
  else t->Branch(Form("%s_startmom",_name.c_str()), _startmom, Form("%s_startmom[num_tracks]/D",_name.c_str()));

  if(t->GetBranch(Form("%s_len",_name.c_str())))    t->SetBranchAddress(Form("%s_len",_name.c_str()),_len);
  else t->Branch(Form("%s_len",_name.c_str()), _len, Form("%s_len[num_tracks]/D",_name.c_str()));      

  //--- addition ---//
  if(t->GetBranch(Form("%s_points",_name.c_str()))) t->SetBranchAddress(Form("%s_points",_name.c_str()),&_num_points);
  else t->Branch(Form("%s_points",_name.c_str()),&_num_points,Form("%s_points/s",_name.c_str()));

  if(t->GetBranch(Form("%s_vtxID",_name.c_str()))) t->SetBranchAddress(Form("%s_vtxID",_name.c_str()),_vtxID);
  else t->Branch(Form("%s_vtxID",_name.c_str()),_vtxID,Form("%s_vtxID/s",_name.c_str()));
  
  if(t->GetBranch(Form("%s_vtxx",_name.c_str()))) t->SetBranchAddress(Form("%s_vtxx",_name.c_str()),_vtxx);
  else t->Branch(Form("%s_vtxx",_name.c_str()),_vtxx,Form("%s_vtxx[num_points]/F",_name.c_str()));

  if(t->GetBranch(Form("%s_vtxy",_name.c_str()))) t->SetBranchAddress(Form("%s_vtxy",_name.c_str()),_vtxy);
  else t->Branch(Form("%s_vtxy",_name.c_str()),_vtxy,Form("%s_vtxy[num_points]/F",_name.c_str()));

  if(t->GetBranch(Form("%s_vtxz",_name.c_str()))) t->SetBranchAddress(Form("%s_vtxz",_name.c_str()),_vtxz);
  else t->Branch(Form("%s_vtxz",_name.c_str()),_vtxz,Form("%s_vtxz[num_points]/F",_name.c_str()));

  if(t->GetBranch(Form("%s_momx",_name.c_str()))) t->SetBranchAddress(Form("%s_momx",_name.c_str()),_momx);
  else t->Branch(Form("%s_momx",_name.c_str()),_momx,Form("%s_momx[num_points]/D",_name.c_str()));

  if(t->GetBranch(Form("%s_momy",_name.c_str()))) t->SetBranchAddress(Form("%s_momy",_name.c_str()),_momy);
  else t->Branch(Form("%s_momy",_name.c_str()),_momy,Form("%s_momy[num_points]/D",_name.c_str()));

  if(t->GetBranch(Form("%s_momz",_name.c_str()))) t->SetBranchAddress(Form("%s_momz",_name.c_str()),_momz);
  else t->Branch(Form("%s_momz",_name.c_str()),_momz,Form("%s_momz[num_points]/D",_name.c_str()));

}

#endif
