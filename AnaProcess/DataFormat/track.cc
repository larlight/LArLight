#ifndef TRACK_CC
#define TRACK_CC

#include "track.hh"

//########################################
track::track() : data_base(),
		 fXYZ(),
		 fDir(),
		 fCov(),
		 fdQdx(),
		 fFitMomentum()
//########################################
{ clear_data(); }


//########################################
track::track(const track& original) :   data_base(original),
					fID(original.fID),
					fXYZ(original.fXYZ),
					fDir(original.fDir),
					fCov(original.fCov),
					fdQdx(original.fdQdx),
					fFitMomentum(original.fFitMomentum)
//########################################
{}

//########################################
void track::clear_data()
//########################################
{
  data_base::clear_data();  
  fID = -1; 
  fXYZ.clear(); 
  fDir.clear(); 
  fCov.clear(); 
  fdQdx.clear();
  fFitMomentum.clear(); 
}

//############################################################################
void event_track::get_axis_range(double &max, double &min, const int axis) const
//############################################################################
{

  if(axis < 0 || axis>2) {

    Message::get()->send(MSG::ERROR,__FUNCTION__,Form("Invalid axis index: %d",axis));
    
    return;

  }

  for(auto trk : fTrack_v) {

    for(size_t i=0; i<trk.n_point(); i++) {

      const TVector3 vtx = trk.vertex_at(i);

      if(vtx[axis] > max) max = vtx[axis];

      if(vtx[axis] < min) min = vtx[axis];
    }
  }

  return;
}


//############################################################################
void event_track::get_axis_range(double &xmax, double &xmin,
				 double &ymax, double &ymin,
				 double &zmax, double &zmin) const
//############################################################################
{

  for(auto trk : fTrack_v) {

    for(size_t i=0; i<trk.n_point(); i++) {

      const TVector3 vtx = trk.vertex_at(i);

      if(vtx[0] > xmax) xmax = vtx[0];
      if(vtx[0] < xmin) xmin = vtx[0];
      if(vtx[1] > ymax) ymax = vtx[1];
      if(vtx[1] < ymin) ymin = vtx[1];
      if(vtx[2] > zmax) zmax = vtx[2];
      if(vtx[2] < zmin) zmin = vtx[2];
      
    }
  }

  return;
}


#endif
