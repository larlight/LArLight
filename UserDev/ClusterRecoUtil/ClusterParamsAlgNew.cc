#ifndef CLUSTERPARAMSALGNEW_HH
#define CLUSTERPARAMSALGNEW_HH

#include "ClusterParamsAlgNew.hh"



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
    if (finished_GetAverages) return;
    else return;
  }

  // Also does the high hitlist
  void ClusterParamsAlgNew::GetRoughAxis(bool override){
    if (finished_GetRoughAxis && !override) return;
    
  }


}

#endif



