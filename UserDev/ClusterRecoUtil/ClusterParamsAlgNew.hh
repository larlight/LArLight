////////////////////////////////////////////////////////////////////////
// ClusterParamsAlgNew.h
//
// ClusterParamsAlgNew class
//
// Andrzej Szelc (andrzej.szelc@yale.edu)
//
////////////////////////////////////////////////////////////////////////
#ifndef CLUSTERPARAMSALGNEW_H
#define CLUSTERPARAMSALGNEW_H

#include <TMath.h>
#include <TH2F.h>
#include <TF1.h>
#include <TH1F.h>
#include <TPrincipal.h>

#include "PxUtils.h"

#include <vector>

#include "DataFormat-TypeDef.hh"
#include "LArUtil-TypeDef.hh"
#include "HoughBaseAlg.hh"

namespace larreco {
   
  class ClusterParamsAlgNew {
  public:
    
    ClusterParamsAlgNew(std::vector<util::PxHit>){ 
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
   } 

    void FillParams();
    cluster_params * GetParams();

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
     * @param override force recalculation of variables
     */
    void GetAverages(bool override=false);


    /**
     * Calculates the following variables:
     * verticalness
     * rough_2d_slope
     * rough_2d_intercept
     * @param override [description]
     */
    void GetRoughAxis(bool override=false);


    /**
     * Calculates the following variables:
     * opening_angle
     * opening_angle_highcharge
     * closing_angle
     * closing_angle_highcharge
     * offaxis_hits
     * @param override [description]
     */
    void GetProfileInfo(bool override=false);

    /**
     * Calculates the following variables:
     * length
     * width
     * @param override [description]
     */
    void RefineStartPoints(bool override=false);

    /**
     * Calculates the following variables:
     * hit_density_1D
     * hit_density_2D
     * angle_2d
     * direction
     * @param override [description]
     */
    void GetFinalSlope(bool override=false);    


    
  private:
    std::vector<util::PxHit> hitVector;         // This vector holds the wrapped up hit list

    bool finished_GetAverages;
    bool finished_GetRoughAxis;
    bool finished_GetProfileInfo;
    bool finished_RefineStartPoints;
    bool finished_GetFinalSlope;


    double rough_2d_slope;    // slope 
    double rough_2d_intercept;    // slope 
    
    cluster_params _this_params;

  }; //class ClusterParamsAlgNew
  
} //namespace cluster








#endif
