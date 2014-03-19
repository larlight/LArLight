#ifndef CLUSTERPARAMSALGNEW_HH
#define CLUSTERPARAMSALGNEW_HH

#include "ClusterParamsAlgNew.hh"

//-----Math-------
#include <math.h>       
#define PI 3.14159265


namespace cluster{

  ClusterParamsAlgNew::ClusterParamsAlgNew(std::vector<larutil::PxHit>){

    // Make default values
    // Is done by the struct    
  
    // Make sure TPrincipal is initialized:
    principal = new TPrincipal(2);

    finished_GetAverages       = false;
    finished_GetRoughAxis      = false;
    finished_GetProfileInfo    = false;
    finished_RefineStartPoints = false;
    finished_GetFinalSlope     = false;

    gser = (larutil::GeometryUtilities*)(larutil::GeometryUtilities::GetME());
  }
  


  void ClusterParamsAlgNew::FillParams(bool override_DoGetAverages      ,  
				       bool override_DoGetRoughAxis     ,  
				       bool override_DoGetProfileInfo   ,  
				       bool override_DoRefineStartPoints,  
				       bool override_DoGetFinalSlope     ){
    GetAverages      (override_DoGetAverages      );
    GetRoughAxis     (override_DoGetRoughAxis     );
    GetProfileInfo   (override_DoGetProfileInfo   );
    RefineStartPoints(override_DoRefineStartPoints);
    GetFinalSlope    (override_DoGetFinalSlope    );
  }

  void ClusterParamsAlgNew::GetParams(cluster::cluster_params&  inputstruct){
    inputstruct = _this_params;
    return;
  }

  /**
   * Calculates the following variables:
   * mean_charge
   * mean_x
   * mean_y
   * charge_wgt_x
   * charge_wgt_y
   * eigenvalue_principal
   * eigenvalue_secondary
   * multi_hit_wires
   * N_Wires
   * **/
  void ClusterParamsAlgNew::GetAverages(bool override){
    if (finished_GetAverages && !override) return;

    principal -> Clear();

    _this_params.N_Hits = hitVector.size();

    std::map<double, int> wireMap;

    double mean_charge = 0.0;



    for (auto & hit : hitVector){
      double data[2];
      data[0] = hit.w;
      data[1] = hit.t;
      principal -> AddRow(data);
      _this_params.charge_wgt_x += hit.w*hit.charge;
      _this_params.charge_wgt_y += hit.t*hit.charge;
      mean_charge += hit.charge;

      wireMap[hit.w] ++;

    }
    _this_params.N_Wires = wireMap.size();
    _this_params.multi_hit_wires = _this_params.N_Hits - _this_params.N_Wires;

    _this_params.charge_wgt_x /= mean_charge;
    _this_params.charge_wgt_y /= mean_charge;

    _this_params.mean_x = (* principal -> GetMeanValues())[0];
    _this_params.mean_y = (* principal -> GetMeanValues())[1];
    _this_params.mean_charge /= _this_params.N_Hits;

    principal -> MakePrincipals();

    _this_params.eigenvalue_principal = (* principal -> GetEigenValues() )[0];
    _this_params.eigenvalue_secondary = (* principal -> GetEigenValues() )[1];

  }

  // Also does the high hitlist
  void ClusterParamsAlgNew::GetRoughAxis(bool override){

    if (finished_GetAverages && !override) {

      finished_GetRoughAxis = false;

      return;}

    //using the charge weighted coordinates find the axis from slope
    double ncw=0;
    double sumtime=0;//from sum averages
    double sumwire=0;//from sum averages
    double sumwiretime=0;//sum over (wire*time)
    double sumwirewire=0;//sum over (wire*wire)
    //next loop over all hits again
    for (auto & hit : hitVector){
      //if charge is above avg_charge
      if(hit.charge > _this_params.mean_charge){
	ncw+=1;
	sumwire+=hit.w;
	sumtime+=hit.t;
	sumwiretime+=hit.w*hit.t;
	sumwirewire+=pow(hit.w,2);	
      }//for high charge
    }//For hh loop

    //Looking for the slope and intercept of the line above avg_charge hits
    double slopecw=0;
    //double ceptcw=0;
    slopecw= (ncw*sumwiretime- sumwire*sumtime)/(ncw*sumwirewire-sumwire*sumwire);//slope for cw
    //ceptcw= _this_params.charge_wgt_y  -slopecw*(_this_params.charge_wgt_x);//intercept for cw
    //Getthe 2D_angle
    _this_params.cluster_angle_2d = atan(slopecw)*180/PI;
    return;
  }


  void ClusterParamsAlgNew::GetProfileInfo(bool override)
  {
    if(rough_2d_slope==-999.999 || rough_2d_intercept==-999.999 ) //these variables need to be initialized to othervalues? 
      GetRoughAxis(true);      

    coarse_nbins=2;
    profile_nbins=100;
    
    charge_profile.clear();
    coarse_charge_profile.clear();
    charge_profile.resize(profile_nbins);
    coarse_charge_profile.resize(coarse_nbins);
    
    //get slope of lines orthogonal to those found crossing the shower.
    double inv_2d_slope=0;
    if(rough_2d_slope){
      inv_2d_slope=-1./rough_2d_slope; //*fWireTimetoCmCm*fWireTimetoCmCm;
    }
    else
      inv_2d_slope=-999999.;

    double inter_high=-999999;
    double inter_low=999999;
    double inter_high_side=-999999;
    double inter_low_side=999999;
    //loop over all hits. Create coarse and fine profiles of the charge weight to help refine the start/end points and have a first guess of direction
    for(auto & hit : hitVector)
      {
      
	//calculate intercepts along   
	double intercept=hit.t-inv_2d_slope*(double)hit.w;
    
	double side_intercept=hit.t-rough_2d_slope*(double)hit.w;
    
	if(intercept > inter_high ){
	  inter_high=intercept;
    	}
    
	if(intercept < inter_low ){
	  inter_low=intercept;
        }  

	if(side_intercept > inter_high_side ){
	  inter_high_side=side_intercept;
    	}
    
	if(side_intercept < inter_low_side ){
	  inter_low_side=side_intercept;
        }
    

      }   // end of first HitIter loop, at this point we should have the extreme intercepts 
	
    /////////////////////////////////////////////
    // Second loop. Fill profiles. 
    /////////////////////////////////////////////
    
    // get projected points at the beginning and end of the axis.
     
     larutil::PxPoint HighOnlinePoint, LowOnlinePoint,BeginOnlinePoint;
     
     gser->GetPointOnLineWSlopes(rough_2d_slope,rough_2d_intercept,inter_high,HighOnlinePoint);
     gser->GetPointOnLineWSlopes(rough_2d_slope,rough_2d_intercept,inter_low,LowOnlinePoint);
  

     //define BeginOnlinePoint as the one with lower wire number (for now)
     
     BeginOnlinePoint = (HighOnlinePoint.w > LowOnlinePoint.w) ? LowOnlinePoint : HighOnlinePoint;
     
     
     double projectedlength=gser->Get2DDistance(HighOnlinePoint,LowOnlinePoint);
     
      
      
    for(auto & hit : hitVector)
      {
      
       larutil::PxPoint OnlinePoint;
       // get coordinates of point on axis.
       gser->GetPointOnLine(rough_2d_slope,LowOnlinePoint,hit,OnlinePoint);
    
       double linedist=gser->Get2DDistance(OnlinePoint,BeginOnlinePoint);
       double ortdist=gser->Get2DDistance(OnlinePoint,hit);
    
      ////////////////////////////////////////////////////////////////////// 
      //calculate the weight along the axis, this formula is based on rough guessology. 
      // there is no physics motivation behind the particular numbers, A.S.
      /////////////////////////////////////////////////////////////////////// 
      double weight= (ortdist<1.) ? 10*hit.charge : 5*hit.charge/ortdist;
    
      int fine_bin=(int)linedist/projectedlength*profile_nbins;
      int coarse_bin=(int)linedist/projectedlength*coarse_nbins; 
      
      if(fine_bin<profile_nbins)  //only fill if bin number is in range
	charge_profile[fine_bin]+=weight;
      
      if(coarse_bin<coarse_nbins) //only fill if bin number is in range
	coarse_charge_profile[coarse_bin]+=weight;
      
           
      }  // end second loop on hits. Now should have filled profile vectors.
      
    
  }
  
  
  
  
  
  
  
}

#endif
