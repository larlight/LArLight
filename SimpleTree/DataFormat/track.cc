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
		      Float_t  theta,  Double_t startmom,    Double_t len)
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
  _startmom[_num_tracks] = startmom;
  _len[_num_tracks]      = len;
  _num_tracks++;
}

//#####################################################################
void track::add_trajectory(UShort_t vtxID, 
			   Float_t x,  Float_t y,  Float_t z,
			   Float_t dirx,  Float_t diry,  Float_t dirz,
			   Double_t mom)
//#####################################################################
{
  _vtxID[_num_points] = vtxID;
  _vtxx[_num_points]  = x;
  _vtxy[_num_points]  = y;
  _vtxz[_num_points]  = z;
  _dirx[_num_points]  = dirx;
  _diry[_num_points]  = diry;
  _dirz[_num_points]  = dirz;
  _mom[_num_points]   = mom;
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
    _dirx[index]  = 0;
    _diry[index]  = 0;
    _dirz[index]  = 0;
    
  } // clear loop ends
  _num_points = 0;

}

//#####################################################################
Bool_t track::set_address(TTree* t,Bool_t create)
//#####################################################################
{
  print(MSG::DEBUG,__PRETTY_FUNCTION__,Form("%s called...",_name.c_str()));
  // base class address set
  Bool_t exist = data_base::set_address(t,create);

  // track class address set
  if(t->GetBranch(Form("num_%s",_name.c_str())))
    t->SetBranchAddress(Form("num_%s",_name.c_str()),&_num_tracks);
  else {
    exist = false;
    if(create) t->Branch(Form("num_%s",_name.c_str()), &_num_tracks,Form("num_%s/s",_name.c_str()));
  }

  if(t->GetBranch(Form("%s_trackID",_name.c_str())))  t->SetBranchAddress(Form("%s_trackID",_name.c_str()),_trackID);
  else if(create) t->Branch(Form("%s_trackID",_name.c_str()), _trackID, Form("%s_trackID[num_%s]/s",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_startx",_name.c_str())))   t->SetBranchAddress(Form("%s_startx",_name.c_str()),_startx);
  else if(create) t->Branch(Form("%s_startx",_name.c_str()), _startx, Form("%s_startx[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_starty",_name.c_str())))   t->SetBranchAddress(Form("%s_starty",_name.c_str()),_starty);
  else if(create) t->Branch(Form("%s_starty",_name.c_str()), _starty, Form("%s_starty[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_startz",_name.c_str())))   t->SetBranchAddress(Form("%s_startz",_name.c_str()),_startz);
  else if(create) t->Branch(Form("%s_startz",_name.c_str()), _startz, Form("%s_startz[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_startd",_name.c_str())))   t->SetBranchAddress(Form("%s_startd",_name.c_str()),_startd);
  else if(create) t->Branch(Form("%s_startd",_name.c_str()), _startd, Form("%s_startd[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_endx",_name.c_str())))   t->SetBranchAddress(Form("%s_endx",_name.c_str()),_endx);
  else if(create) t->Branch(Form("%s_endx",_name.c_str()), _endx, Form("%s_endx[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_endy",_name.c_str())))   t->SetBranchAddress(Form("%s_endy",_name.c_str()),_endy);
  else if(create) t->Branch(Form("%s_endy",_name.c_str()), _endy, Form("%s_endy[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_endz",_name.c_str())))   t->SetBranchAddress(Form("%s_endz",_name.c_str()),_endz);
  else if(create) t->Branch(Form("%s_endz",_name.c_str()), _endz, Form("%s_endz[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_endd",_name.c_str())))   t->SetBranchAddress(Form("%s_endd",_name.c_str()),_endd);
  else if(create) t->Branch(Form("%s_endd",_name.c_str()), _endd, Form("%s_endd[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_theta",_name.c_str())))  t->SetBranchAddress(Form("%s_theta",_name.c_str()),_theta);
  else if(create)  t->Branch(Form("%s_theta",_name.c_str()), _theta, Form("%s_theta[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_phi",_name.c_str())))    t->SetBranchAddress(Form("%s_phi",_name.c_str()),_phi);
  else if(create) t->Branch(Form("%s_phi",_name.c_str()), _phi, Form("%s_phi[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_startmom",_name.c_str())))    t->SetBranchAddress(Form("%s_startmom",_name.c_str()),_startmom);
  else if(create) t->Branch(Form("%s_startmom",_name.c_str()), _startmom, Form("%s_startmom[num_%s]/D",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_len",_name.c_str())))    t->SetBranchAddress(Form("%s_len",_name.c_str()),_len);
  else if(create) t->Branch(Form("%s_len",_name.c_str()), _len, Form("%s_len[num_%s]/D",_name.c_str(),_name.c_str()));      

  //--- addition ---//
  if(t->GetBranch(Form("%s_points",_name.c_str()))) t->SetBranchAddress(Form("%s_points",_name.c_str()),&_num_points);
  else if(create) t->Branch(Form("%s_points",_name.c_str()),&_num_points,Form("%s_points/s",_name.c_str()));

  if(t->GetBranch(Form("%s_vtxID",_name.c_str()))) t->SetBranchAddress(Form("%s_vtxID",_name.c_str()),_vtxID);
  else if(create) t->Branch(Form("%s_vtxID",_name.c_str()),_vtxID,Form("%s_vtxID[%s_points]/s",_name.c_str(),_name.c_str()));
  
  if(t->GetBranch(Form("%s_vtxx",_name.c_str()))) t->SetBranchAddress(Form("%s_vtxx",_name.c_str()),_vtxx);
  else if(create) t->Branch(Form("%s_vtxx",_name.c_str()),_vtxx,Form("%s_vtxx[%s_points]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_vtxy",_name.c_str()))) t->SetBranchAddress(Form("%s_vtxy",_name.c_str()),_vtxy);
  else if(create) t->Branch(Form("%s_vtxy",_name.c_str()),_vtxy,Form("%s_vtxy[%s_points]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_vtxz",_name.c_str()))) t->SetBranchAddress(Form("%s_vtxz",_name.c_str()),_vtxz);
  else if(create) t->Branch(Form("%s_vtxz",_name.c_str()),_vtxz,Form("%s_vtxz[%s_points]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_dirx",_name.c_str()))) t->SetBranchAddress(Form("%s_dirx",_name.c_str()),_dirx);
  else if(create) t->Branch(Form("%s_dirx",_name.c_str()),_dirx,Form("%s_dirx[%s_points]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_diry",_name.c_str()))) t->SetBranchAddress(Form("%s_diry",_name.c_str()),_diry);
  else if(create) t->Branch(Form("%s_diry",_name.c_str()),_diry,Form("%s_diry[%s_points]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_dirz",_name.c_str()))) t->SetBranchAddress(Form("%s_dirz",_name.c_str()),_dirz);
  else if(create) t->Branch(Form("%s_dirz",_name.c_str()),_dirz,Form("%s_dirz[%s_points]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_mom",_name.c_str()))) t->SetBranchAddress(Form("%s_mom",_name.c_str()),_mom);
  else if(create) t->Branch(Form("%s_mom",_name.c_str()),_mom,Form("%s_mom[%s_points]/D",_name.c_str(),_name.c_str()));

  return exist;
}

#endif
