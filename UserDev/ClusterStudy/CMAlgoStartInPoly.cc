#ifndef CMALGOSTARTINPOLY_CC
#define CMALGOSTARTINPOLY_CC

#include "CMAlgoStartInPoly.hh"

namespace cluster {

  CMAlgoStartInPoly::CMAlgoStartInPoly()
  {
    // Nothing to be done in the base class
    this->reconfigure();

    _wire_2_cm = larutil::GeometryUtilities::GetME()->WireToCm();
    _time_2_cm = larutil::GeometryUtilities::GetME()->TimeToCm();

    SetMinHitsCut(40);

  }


  void CMAlgoStartInPoly::reconfigure(){

    //not sure what needs to be reset/reconfigured for this algo
    
  }//end reconfigure function

  
  bool CMAlgoStartInPoly::Bool(const ClusterParamsAlgNew &cluster1,
			       const ClusterParamsAlgNew &cluster2)
  {

    //If start point of one cluster is inside the other's polygon
    //boundary --> then merge!
    
    float w_start1 = cluster1.GetParams().start_point.w * _wire_2_cm;
    float t_start1 = cluster1.GetParams().start_point.t * _time_2_cm;

    float w_start2 = cluster2.GetParams().start_point.w * _wire_2_cm;
    float t_start2 = cluster2.GetParams().start_point.t * _time_2_cm;

    //make Point() for the two start points
    std::pair<float,float> start1;
    std::pair<float,float> start2;
    start1 = std::make_pair( w_start1, t_start1);
    start2 = std::make_pair( w_start2, t_start2);

    //check if start point for cluster1 is in Polygon of cluster2
    if ( ( cluster2.GetParams().PolyObject.PointInside( start1) ) and (cluster1.GetParams().N_Hits > _MinHits) ){
      std::cout << "Start point of Cluster 1 in Polygon of Cluster 2!" << std::endl;
      return true;
    }
    //check if start point for cluster1 is in Polygon of cluster2
    if ( ( cluster1.GetParams().PolyObject.PointInside( start2) ) and (cluster1.GetParams().N_Hits > _MinHits) ){
      std::cout << "Start point of Cluster 2 in Polygon of Cluster 1!" << std::endl;
      return true;
    }

    return false;
  }


}

#endif
