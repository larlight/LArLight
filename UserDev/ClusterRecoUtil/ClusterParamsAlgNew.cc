#include "ClusterParamsAlgNewNew.hh"


namespace cluster{

  ClusterParamsAlgNew(std::vector<util::PxHit>){

    // Make default values
    // Is done by the struct    


    // 
    finished_GetAverages       = false;
    finished_GetRoughAxis      = false;
    finished_GetProfileInfo    = false;
    finished_RefineStartPoints = false;
    finished_GetFinalSlope     = false;

  }



  void ClusterParamsAlgNew::FillParams(){

  }

  cluster_params * ClusterParamsAlgNew::GetParams(){



    // Loop 1 calculates:
    //  mean_charge
    //  mean_x
    //  mean_y
    //  charge_wgt_x
    //  charge_wgt_y

  }


  void ClusterParamsAlgNew::GetAverages(){
    if (finished_GetAverages)
  }

  // Also does the high hitlist
  void ClusterParamsAlgNew::GetRoughAxis(){

  }



}





