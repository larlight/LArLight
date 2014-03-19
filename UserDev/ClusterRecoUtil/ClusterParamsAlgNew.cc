#ifndef CLUSTERPARAMSALGNEW_HH
#define CLUSTERPARAMSALGNEW_HH

#include "ClusterParamsAlgNew.hh"

//-----Math-------
#include <math.h>       
#define PI 3.14159265


namespace cluster{

  ClusterParamsAlgNew::ClusterParamsAlgNew(std::vector<util::PxHit>){

    // Make default values
    // Is done by the struct    


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

  }
  


  void ClusterParamsAlgNew::FillParams(){

  }

  cluster::cluster_params* ClusterParamsAlgNew::GetParams(){



    // Loop 1 calculates:
    //  mean_charge
    //  mean_x
    //  mean_y
    //  charge_wgt_x
    //  charge_wgt_y
    //  
     

     // SOMEONE SHOULD FIX THIS!!!!!!!!!!!!!!
    return 0;
  }


  void ClusterParamsAlgNew::GetAverages(bool override){

	//get the slope from 

    if (finished_GetAverages) return;
    else return;
  }

  // Also does the high hitlist
  void ClusterParamsAlgNew::GetRoughAxis(bool override){

 if (finished_GetAverages && !override) {

      finished_GetRoughAxis = false;

      return;}

		// grab things from averages
		//&&comes from averages!
		    double mean_charge;//from averages
		    double charge_wgt_y;//from averages
		    double charge_wgt_x;//from averages

	//using the charge weighted coordinates find the axis from slope
		double ncw=0;
		double time;//from sum averages
		double wire;//from sum averages
		double wiretime=0;//sum over (wire*time)
		double wirewire=0;//sum over (wire*wire)
	//next loop over all hits
	    for(unsigned int a=0; a<hitVector.size();a++) {
		//Calculate the slope for all hits
			//if charge is above avg
		if(hitVector[a].charge >mean_charge){
		ncw+=1;
		wire+=hitVector[a].w;
		time+=hitVector[a].t;
		wiretime+=hitVector[a].w*hitVector[a].t;
		wirewire+=pow(hitVector[a].w,2);	
					  }//for high charge
					}//For hh loop
	//currently have weighted each hit evenly will need charge weight them to fit nicely

	double slopecw=0;
		slopecw= (ncw*wiretime- wire*time)/(ncw*wirewire-wire*wire);//slope for cw
	double ceptcw=0;
		ceptcw= charge_wgt_y  -slopecw*(charge_wgt_x);//intercept for cw

		//the 2D_angle
		double angle_2d = atan(slopecw)*180/PI;

		

	//stuff will go here
	return;
    //if (finished_GetRoughAxis && !override) return;
    
  }


}

#endif



