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
#include "ClusterParams.hh"

#include <vector>

#include "DataFormat-TypeDef.hh"
#include "LArUtil-TypeDef.hh"
#include "HoughBaseAlg.hh"
#include "TPrincipal.h"

namespace cluster {
   
  class ClusterParamsAlgNew {

  public:

    ClusterParamsAlgNew(std::vector<larutil::PxHit>);

    /**                                                                             
     * Runs all the functions which calculate cluster params                        
     * and stashes the results in the private ClusterParams                         
     * struct.                                                                      
     *                                                                              
     * @param override_DoGetAverages       force re-execution of GetAverages()      
     * @param override_DoGetRoughAxis      force re-execution of GetRoughAxis()     
     * @param override_DoGetProfileInfo    force re-execution of GetProfileInfo()   
     * @param override_DoRefineStartPoints force re-execution of RefineStartPoints()
     * @param override_DoGetFinalSlope     force re-execution of GetFinalSlope()    
     */
    void FillParams(bool override_DoGetAverages      =false,
                    bool override_DoGetRoughAxis     =false,
                    bool override_DoGetProfileInfo   =false,
                    bool override_DoRefineStartPoints=false,
                    bool override_DoGetFinalSlope    =false );

    void GetParams(cluster::cluster_params &);

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
    //void GetRoughAxis(bool override=false);
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
    std::vector<larutil::PxHit> hitVector;         // This vector holds the wrapped up hit list
 
    larutil::GeometryUtilities *gser;
    TPrincipal *principal;
    
    //settable parameters:
     double fChargeCutoffThreshold[3]; 
     int fplane;
    
    std::vector< double > charge_profile;
    std::vector< double > coarse_charge_profile;
    int coarse_nbins;
    int profile_nbins;
    int profile_maximum_bin;
    double profile_integral_forward;
    double profile_integral_backward;
    double projectedlength;
    
    //extreme intercepts using the rough_2d_slope
    double inter_high;
    double inter_low;
    double inter_high_side;
    double inter_low_side;
    
    bool finished_GetAverages;
    bool finished_GetRoughAxis;
    bool finished_GetProfileInfo;
    bool finished_RefineStartPoints;
    bool finished_GetFinalSlope;


    double rough_2d_slope;    // slope 
    double rough_2d_intercept;    // slope 
    larutil::PxPoint rough_begin_point; 
    larutil::PxPoint rough_end_point;

    cluster::cluster_params _this_params;

  }; //class ClusterParamsAlgNew
  
} //namespace cluster








#endif
