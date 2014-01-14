#ifndef SPACEPOINT_CC
#define SPACEPOINT_CC

#include "spacepoint.hh"

namespace larlight {
  
  //############################################################################
  void event_sps::get_axis_range(Double_t &max, Double_t &min, const int axis) const
  //############################################################################
  {
    
    if(axis < 0 || axis>2) {
      
      Message::get()->send(MSG::ERROR,__FUNCTION__,Form("Invalid axis index: %d",axis));
      
      return;
      
    }
    
    for(size_t i=0; i<this->size(); ++i) {
      
      if((this->at(i)).XYZ()[axis] > max) max = (this->at(i)).XYZ()[axis];
      if((this->at(i)).XYZ()[axis] > min) min = (this->at(i)).XYZ()[axis];
      
    }
    
    return;
  }
  
  
  //############################################################################
  void event_sps::get_axis_range(Double_t &xmax, Double_t &xmin,
				 Double_t &ymax, Double_t &ymin,
				 Double_t &zmax, Double_t &zmin) const
  //############################################################################
  {
    
    for(size_t i=0; i<this->size(); ++i) {
      
      if((this->at(i)).XYZ()[0] > xmax) xmax = (this->at(i)).XYZ()[0];
      if((this->at(i)).XYZ()[0] > xmin) xmin = (this->at(i)).XYZ()[0];
      if((this->at(i)).XYZ()[1] > ymax) ymax = (this->at(i)).XYZ()[1];
      if((this->at(i)).XYZ()[1] > ymin) ymin = (this->at(i)).XYZ()[1];
      if((this->at(i)).XYZ()[2] > zmax) zmax = (this->at(i)).XYZ()[2];
      if((this->at(i)).XYZ()[2] > zmin) zmin = (this->at(i)).XYZ()[2];
      
    }
    
    return;
  }
}
#endif
  
