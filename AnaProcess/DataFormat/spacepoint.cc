#ifndef SPACEPOINT_CC
#define SPACEPOINT_CC

#include "spacepoint.hh"

namespace larlight {
  
  //############################################################################
  void event_sps::get_axis_range(double &max, double &min, const int axis) const
  //############################################################################
  {
    
    if(axis < 0 || axis>2) {
      
      Message::get()->send(MSG::ERROR,__FUNCTION__,Form("Invalid axis index: %d",axis));
      
      return;
      
    }
    
    for(auto sps : fSpacePoint_v){
      
      if(sps.XYZ()[axis] > max) max = sps.XYZ()[axis];
      if(sps.XYZ()[axis] > min) min = sps.XYZ()[axis];
      
    }
    
    return;
  }
  
  
  //############################################################################
  void event_sps::get_axis_range(double &xmax, double &xmin,
				 double &ymax, double &ymin,
				 double &zmax, double &zmin) const
  //############################################################################
  {
    
    for(auto sps : fSpacePoint_v){
      
      if(sps.XYZ()[0] > xmax) xmax = sps.XYZ()[0];
      if(sps.XYZ()[0] > xmin) xmin = sps.XYZ()[0];
      if(sps.XYZ()[1] > ymax) ymax = sps.XYZ()[1];
      if(sps.XYZ()[1] > ymin) ymin = sps.XYZ()[1];
      if(sps.XYZ()[2] > zmax) zmax = sps.XYZ()[2];
      if(sps.XYZ()[2] > zmin) zmin = sps.XYZ()[2];
      
    }
    
    return;
  }
}
#endif
  
