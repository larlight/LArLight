#include "ClusterParamsAlgNew.hh"


namespace cluster{

  ClusterParamsAlgNew::ClusterParamsAlgNew(std::vector<util::PxHit>){

    // Make default values
    // Is done by the struct    
  
    // Make sure TPrincipal is initialized:
    TPrincipal = new TPrincipal(2);


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
    //  
     

     // SOMEONE SHOULD FIX THIS!!!!!!!!!!!!!!
    return 0;
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

    N_Hits = hitVector.size();

    std::map<double, int> wireMap;

    mean_x = 0.0;
    mean_y = 0.0;
    mean_charge = 0.0;

    for (auto & hit : hitVector){
      double data[2];
      data[0] = hit.w;
      data[1] = hit.t;
      principal -> AddRow(data);
      charge_wgt_x += hit.w*hit.charge;
      charge_wgt_y += hit.t*hit.charge;
      mean_charge += hit.charge;

      wireMap[hit.w] ++;

    }
    N_Wires = wireMap.size();
    multi_hit_wires = N_Hits - N_Wires;

    charge_wgt_x / mean_charge;
    charge_wgt_y / mean_charge;

    mean_x = principal -> GetMeanValues()[0];
    mean_y = principal -> GetMeanValues()[1];
    mean_charge /= N_Hits;

    principal -> MakePrincipals();

    eigenvalue_principal = principal -> GetEigenValues()[0];
    eigenvalue_secondary = principal -> GetEigenValues()[1];

  }

  // Also does the high hitlist
  void ClusterParamsAlgNew::GetRoughAxis(bool override){

  }



}





