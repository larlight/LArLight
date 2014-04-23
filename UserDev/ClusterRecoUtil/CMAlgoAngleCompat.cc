#ifndef CMALGOANGLECOMPAT_CC
#define CMALGOANGLECOMPAT_CC

#include "CMAlgoAngleCompat.hh"

namespace cluster {

  CMAlgoAngleCompat::CMAlgoAngleCompat() {

    //this just sets default values
    SetVerbose(true);

    SetAngleCut(30.); // in degrees
    
    SetAllow180Ambig(false);

  } //end constructor

  bool CMAlgoAngleCompat::Merge(const cluster_params &cluster1,
				const cluster_params &cluster2){
    
    //pretty sure we don't need conversion factors here. 
    //already in cm/cm units, degrees? need to check that
    double angle1 = cluster1.angle_2d;// * _time_2_cm / _wire_2_cm;
    double angle2 = cluster2.angle_2d;// * _time_2_cm / _wire_2_cm;
    
    bool compatible = false;
    
    if(_allow_180_ambig)
      compatible = ( abs(angle1-angle2)     < _max_allowed_2D_angle_diff ||
		     abs(angle1-angle2-180) < _max_allowed_2D_angle_diff ||
		     abs(angle1-angle2+180) < _max_allowed_2D_angle_diff   );
    
    else
      compatible = ( abs(angle1-angle2)     < _max_allowed_2D_angle_diff );
    
    if(_verbose) {
      if(compatible) std::cout<<"These two clusters are compatible in angle."<<std::endl;
      else std::cout<<"These two clusters are NOT compatible in angle."<<std::endl;
    }
    
    return compatible;

  } // end Merge function 



}//end namespace cluster
#endif
