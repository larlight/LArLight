#ifndef CMALGOSTARTINCONE_CC
#define CMALGOSTARTINCONE_CC

#include "CMAlgoStartInCone.hh"

namespace cluster {

  CMAlgoStartInCone::CMAlgoStartInCone()
  {
    // Nothing to be done in the base class
    this->reconfigure();

    _wire_2_cm = larutil::GeometryUtilities::GetME()->WireToCm();
    _time_2_cm = larutil::GeometryUtilities::GetME()->TimeToCm();

    SetMinHits(40);
    SetMinLen(15);

  }


  void CMAlgoStartInCone::reconfigure(){

    //not sure what needs to be reset/reconfigured for this algo
    
  }//end reconfigure function

  
  bool CMAlgoStartInCone::Merge(const ClusterParamsAlgNew &cluster1,
				const ClusterParamsAlgNew &cluster2)
  {

    //apply filter immediately
    if ( (cluster1.GetParams().length < _lenMin) and (cluster2.GetParams().length < _lenMin)
	 and (cluster1.GetParams().N_Hits < _NhitsMin) and (cluster2.GetParams().N_Hits < _NhitsMin) )
      return false;

    //A cluster has an associated cone defined as the cone originating
    //at the start point of that cluster opening out in the direction
    //of the end point with an angle equal to the opening angle
    //If cluster A (B) has a start point within the volume defined
    //by the cone of cluster B (A), and if cluster B (A) is good enough
    // (enough hits, charge, length...) then the two are merged.

    double w_start1 = cluster1.GetParams().start_point.w * _wire_2_cm;
    double t_start1 = cluster1.GetParams().start_point.t * _time_2_cm;

    double w_start2 = cluster2.GetParams().start_point.w * _wire_2_cm;
    double t_start2 = cluster2.GetParams().start_point.t * _time_2_cm;

    double direction_1 = cluster1.GetParams().angle_2d;
    double direction_2 = cluster2.GetParams().angle_2d;

    double opening_angle_1 = cluster1.GetParams().opening_angle;
    double opening_angle_2 = cluster2.GetParams().opening_angle;

    //find if start point of A (B) in cone of B (A)
    //do this by translating point A (B) such that
    //start point of B (A) is at origin and +  axis
    //towards end point of B (A)
    double w_start1_rot = w_start1 - w_start2;
    double t_start1_rot = t_start1 - t_start2;
    double w_start2_rot = w_start2 - w_start1;
    double t_start2_rot = t_start2 - t_start1;

    w_start1_rot =   w_start1*cos(direction_2) + t_start1*sin(direction_2);
    t_start1_rot = - w_start1*sin(direction_2) + t_start1*cos(direction_2);
    w_start2_rot =   w_start2*cos(direction_1) + t_start2*sin(direction_1);
    t_start2_rot = - w_start2*sin(direction_1) + t_start2*cos(direction_1);

    if ( (w_start1_rot < cluster2.GetParams().length) and 
	 (t_start1_rot < (w_start1_rot*sin(opening_angle_2))) and
	 (cluster2.GetParams().length < _lenMin) and 
	 (cluster2.GetParams().N_Hits < _NhitsMin) ){
      std::cout << "Start point of Cluster 1 in cone of Cluster 2!" << std::endl;
      return true;
    }
    if ( (w_start2_rot < cluster1.GetParams().length ) and
	 (t_start2_rot < (w_start2_rot*sin(opening_angle_1))) and
	 (cluster1.GetParams().length < _lenMin) and 
	 (cluster1.GetParams().N_Hits < _NhitsMin) ){
      std::cout << "Start point of Cluster 2 in cone of Cluster 1!" << std::endl;
      return true;
    }

    return false;
  }


}


#endif
