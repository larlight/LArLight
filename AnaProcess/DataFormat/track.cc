#ifndef TRACK_CC
#define TRACK_CC

#include "track.hh"

namespace larlight {
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
  void track::get_axis_range(Double_t &max, Double_t &min, const Int_t axis) const
  //############################################################################
  {
    
    if(axis < 0 || axis>2) {
      
      Message::get()->send(MSG::ERROR,__FUNCTION__,Form("Invalid axis index: %d",axis));
      
      return;
      
    }
    
    for(size_t i=0; i<fXYZ.size(); ++i) {
      
      if(fXYZ[i][axis] > max) max = fXYZ[i][axis];
      
      if(fXYZ[i][axis] < min) min = fXYZ[i][axis];
      
    }
    
    return;
  }
  
  //############################################################################
  void track::get_axis_range(Double_t &xmax, Double_t &xmin,
			     Double_t &ymax, Double_t &ymin,
			     Double_t &zmax, Double_t &zmin) const
  //############################################################################
  {
    
    for(size_t i=0; i<fXYZ.size(); ++i) {
      
      if(fXYZ[i][0] > xmax) xmax = fXYZ[i][0];
      if(fXYZ[i][0] < xmin) xmin = fXYZ[i][0];
      if(fXYZ[i][1] > ymax) ymax = fXYZ[i][1];
      if(fXYZ[i][1] < ymin) ymin = fXYZ[i][1];
      if(fXYZ[i][2] > zmax) zmax = fXYZ[i][2];
      if(fXYZ[i][2] < zmin) zmin = fXYZ[i][2];
      
    }
    
    return;
  }
  
  
  //############################################################################
  void event_track::get_axis_range(Double_t &max, Double_t &min, const Int_t axis) const
  //############################################################################
  {
    
    if(axis < 0 || axis>2) {
      
      Message::get()->send(MSG::ERROR,__FUNCTION__,Form("Invalid axis index: %d",axis));
      
      return;
      
    }
    
    for(size_t i=0; i<this->size(); ++i)
      
      this->at(i).get_axis_range(max,min,axis);
    
    return;
  }
  
  //############################################################################
  void event_track::get_axis_range(Double_t &xmax, Double_t &xmin,
				   Double_t &ymax, Double_t &ymin,
				   Double_t &zmax, Double_t &zmin) const
  //############################################################################
  {
    
    for(size_t i=0; i<this->size(); ++i)
      
      this->at(i).get_axis_range(xmax,xmin,ymax,ymin,zmax,zmin);
    
    return;
  }
  
}
#endif
