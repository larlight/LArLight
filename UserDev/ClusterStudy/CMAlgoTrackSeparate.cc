#ifndef CMALGOTRACKSEPARATE_CC
#define CMALGOTRACKSEPARATE_CC

#include "CMAlgoTrackSeparate.hh"

namespace cluster {

  //----------------------------------------
  CMAlgoTrackSeparate::CMAlgoTrackSeparate() : CBoolAlgoBase()
  //----------------------------------------
  {

    //this just sets default values    
    SetVerbose(true);
    
    //1e9 is huge; everything will be merged
    SetMinNumHits(100);
    SetMinAngleDiff(10.); //in degrees
    SetMaxOpeningAngle(20.);

    _wire_2_cm = larutil::GeometryUtilities::GetME()->WireToCm();
    _time_2_cm = larutil::GeometryUtilities::GetME()->TimeToCm();

  }

  //--------------------------------------------------------
  bool CMAlgoTrackSeparate::Bool(const ClusterParamsAlgNew &cluster1,
			const ClusterParamsAlgNew &cluster2)
  //--------------------------------------------------------
  {

    //two clusters are considered un-mergable if:
    //1) both have more than _MinNumHits
    //2) opening angle for both < _MAxOpeningAngle
    //3) diff. in direction of both < _MinAngleDiff
    if ( (cluster1.GetParams().N_Hits > _MinNumHits) and
	 (cluster2.GetParams().N_Hits > _MinNumHits) and
	 ( (cluster1.GetParams().angle_2d - cluster2.GetParams().angle_2d) > _MinAngleDiff ) and
	 (cluster1.GetParams().opening_angle < _MaxOpeningAngle) and
	 (cluster2.GetParams().opening_angle < _MaxOpeningAngle) ){
      if (_verbose) { std::cout << "Separate with TrackSeparate!" << std::endl; }
      return true;
    }
    
    return false;

  }

  //-----------------------
  void CMAlgoTrackSeparate::Report()
  //-----------------------
  {
  }

}

#endif
