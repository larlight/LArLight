#ifndef CMALGOCENTEROFMASS_CC
#define CMALGOCENTEROFMASS_CC

#include "CMAlgoCenterOfMass.hh"

namespace cluster {

  //----------------------------------------
  CMAlgoCenterOfMass::CMAlgoCenterOfMass() : CBoolAlgoBase()
  //----------------------------------------
  {

    SetVerbose(false);
    SetDebug(false);
    SetMaxHitsSmallClus(10);
    SetMinHitsBigClus(40);

  }

  //--------------------------------------------------------
  bool CMAlgoCenterOfMass::Bool(const ClusterParamsAlgNew &cluster1,
			    const ClusterParamsAlgNew &cluster2)
  //--------------------------------------------------------
  {

    int small = 0;
    int hitssmall = 0;

    //determine if clusters ok and if so which is big and which is small
    if ( (cluster1.GetParams().N_Hits > _minHits) and (cluster2.GetParams().N_Hits < _maxHits) )
      small = 2;
    else if ( (cluster2.GetParams().N_Hits > _minHits) and (cluster1.GetParams().N_Hits < _maxHits) )
      small = 1;
    else
      return false;
    
    //Define COM values on w & t
    double COM_t_s = 0;
    double COM_w_s = 0;
    double Q_s     = 0;
    //Define cone parameters
    double opening_angle;
    double direc_angle;
    double length;
    double start_w;
    double start_t;

    //Get Hit vector for small cluster
    std::vector<larutil::PxHit> hitss;
    if ( small == 1 ){
      hitss = cluster1.GetHitVector();
      hitssmall = hitss.size();
      direc_angle   = cluster2.GetParams().angle_2d;
      opening_angle = cluster2.GetParams().opening_angle * (180./3.14);
      length  = cluster2.GetParams().length;
      start_w = cluster2.GetParams().start_point.w;
      start_t = cluster2.GetParams().start_point.t;
    }      
    if ( small == 2 ){
      hitss = cluster2.GetHitVector();
      hitssmall = hitss.size();
      direc_angle   = cluster1.GetParams().angle_2d;
      opening_angle = cluster1.GetParams().opening_angle * (180./3.14);
      length  = cluster1.GetParams().length;
      start_w = cluster1.GetParams().start_point.w;
      start_t = cluster1.GetParams().start_point.t;
    }

    for (auto& hit: hitss){
      COM_t_s += hit.t * hit.charge;
      COM_w_s += hit.w * hit.charge;
      Q_s     += hit.charge;
    }
    COM_t_s /= Q_s;
    COM_w_s /= Q_s;

    if (_debug) {
      std::cout << "Small Cluster: " << std::endl;
      std::cout << "N Hits: " << hitssmall << std::endl;
      std::cout << "COM: (w,t) -> (" << COM_w_s << ", " << COM_t_s << ")" << std::endl;
      std::cout << std::endl;
    }

    //Get COM
    std::pair<float,float> COM_s;
    COM_s = std::make_pair( COM_w_s, COM_t_s );

    //Get rotate COM to see if in cone
    double COM_w_rot = (COM_w_s - start_w)*cos(direc_angle*3.14/180.) + (COM_t_s - start_t)*sin(direc_angle*3.14/180.);
    double COM_t_rot = - (COM_w_s - start_w)*sin(direc_angle*3.14/180.) + (COM_t_s - start_t)*cos(direc_angle*3.14/180.);

    //check if COM is in polygon:
    if ( small == 1 ){
      //look for polygon overlap
      if ( cluster2.GetParams().PolyObject.PointInside( COM_s ) ){
	if (_verbose) { std::cout << "Polygon Overlap -> Merge!" << std::endl << std::endl;}
	return true;
      }
      //look for COM in cone
      if ( (COM_w_rot < length) and ( COM_w_rot > 0 ) and
	   ( abs(COM_t_rot) < abs(COM_w_rot*sin(opening_angle*3.14/180.)) ) ){
	if (_verbose) { std::cout << "COM in Cone -> Merge! " << std::endl; }
	return true;
      }
    }
    if ( small == 2 ){
      // COM in Polygon
      if ( cluster1.GetParams().PolyObject.PointInside( COM_s ) ){
	if (_verbose) { std::cout << "Polygon Overlap -> Merge!" << std::endl << std::endl;}
	return true;
      }
      //COM in cone
      if ( (COM_w_rot < length) and ( COM_w_rot > 0 ) and
	   ( abs(COM_t_rot) < abs(COM_w_rot*sin(opening_angle*3.14/180.)) ) ){
	if (_verbose) { std::cout << "COM in Cone -> Merge! " << std::endl; }
	return true;
      }

    }
   
    return false;
  }

  //-----------------------
  void CMAlgoCenterOfMass::Report()
  //-----------------------
  {

  }

}

#endif
