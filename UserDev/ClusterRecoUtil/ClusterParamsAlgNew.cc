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


    // 
    finished_GetAverages       = false;
    finished_GetRoughAxis      = false;
    finished_GetProfileInfo    = false;
    finished_RefineStartPoints = false;
    finished_GetFinalSlope     = false;

      /*
      mean_charge			= -999.999 ;
      mean_x				= -999.999 ;
      mean_y				= -999.999 ;
      rms_x      			= -999.999 ;
      rms_y				= -999.999 ;
      charge_wgt_x			= -999.999 ;
      charge_wgt_y			= -999.999 ;
      eigenvalue_principal		= -999.999 ;
      eigenvalue_secondary		= -999.999 ;
      multi_hit_wires			= -999.999 ;
      N_Wires				= -999.999 ;
      verticalness			= -999.999 ;
      rough_2d_slope			= -999.999 ;
      rough_2d_intercept		= -999.999 ;
      opening_angle			= -999.999 ;
      opening_angle_highcharge		= -999.999 ;
      closing_angle			= -999.999 ;
      closing_angle_highcharge		= -999.999 ;
      offaxis_hits			= -999.999 ;
      modified_hit_density		= -999.999 ;	
      length				= -999.999 ;
      width				= -999.999 ;
      hit_density_1D			= -999.999 ;
      hit_density_2D			= -999.999 ;
      angle_2d				= -999.999 ;
      direction				= -999.999 ;
      showerness			= -999.999 ;
      trackness				= -999.999 ;
      */

      gser = (larutil::GeometryUtilities*)(larutil::GeometryUtilities::GetME());
  }
  


  void ClusterParamsAlgNew::FillParams(){

  }

    // Loop 1 calculates:
    //  mean_charge
    //  mean_x
    //  mean_y
    //  charge_wgt_x
    //  charge_wgt_y
	
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
		_this_params.angle_2d = atan(slopecw)*180/PI;
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
      
    for(auto & hit : hitVector)
      {
      
      
    
      }  // end second loop on hits. Now should have filled profile vectors.
      
    
  }
  
  
  
  
  
  
  
}

#endif



