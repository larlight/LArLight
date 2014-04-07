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
#include <TCanvas.h>
#include <TLegend.h>

#include "PxUtils.h"
#include "ClusterParams.hh"
#include "RecoUtilException.hh"

#include <vector>

#include "DataFormat-TypeDef.hh"
#include "LArUtil-TypeDef.hh"
//#include "HoughBaseAlg.hh"
#include "TPrincipal.h"

namespace cluster {
   
  class ClusterParamsAlgNew {

  public:

    ClusterParamsAlgNew();
    ~ClusterParamsAlgNew();

    ClusterParamsAlgNew(const std::vector<const larlight::hit*>&);

    ClusterParamsAlgNew(const std::vector<larutil::PxHit>&);

    void Initialize();

    //void SetHits(const std::vector<larutil::PxHit*>&);
    
    void SetHits(const std::vector<const larlight::hit*> &);

    void SetHits(const std::vector<larutil::PxHit> &);

    void SetRefineDirectionQMin(double qmin){ fQMinRefDir = qmin; }

    void SetVerbose(){ verbose = true;}

    void Report();

    /**
     * This function returns a feature vector suitable for a neural net
     * This function uses the data from cluster_params but packages it
     * up in a different way, and so is inappropriate to include in 
     * clusterParams.hh.   That's why it's here.
     * @param  data  takes a reference to a vector, templated to allow double or float
     */
    void  GetFANNVector(std::vector<float> & data);


    /**
      Runs all the functions which calculate cluster params                        
      and stashes the results in the private ClusterParams                         
      struct.                                                                      
                                                                                   
      @param override_DoGetAverages       force re-execution of GetAverages()      
      @param override_DoGetRoughAxis      force re-execution of GetRoughAxis()     
      @param override_DoGetProfileInfo    force re-execution of GetProfileInfo()   
      @param override_DoRefineStartPoints force re-execution of RefineStartPoints()
      @param override_DoGetFinalSlope     force re-execution of GetFinalSlope()    
    */
    void FillParams(bool override_DoGetAverages      =false,
                    bool override_DoGetRoughAxis     =false,
                    bool override_DoGetProfileInfo   =false,
                    bool override_DoRefineStartPoints=false,
		    bool override_DoRefineDirection  =false,
                    bool override_DoGetFinalSlope    =false );

    const cluster_params& GetParams()
    { return fParams;}

    /**
       Calculates the following variables:
       mean_charge
       mean_x
       mean_y
       charge_wgt_x
       charge_wgt_y
       eigenvalue_principal
       eigenvalue_secondary
       multi_hit_wires
       N_Wires
       @param override force recalculation of variables
    */
    void GetAverages(bool override=false);


    /**
      Calculates the following variables:
      verticalness
      fRough2DSlope
      fRough2DIntercept
      @param override [description]
    */
    //void GetRoughAxis(bool override=false);
    void GetRoughAxis(bool override=false);


    /**
       Calculates the following variables:
       opening_angle
       opening_angle_highcharge
       closing_angle
       closing_angle_highcharge
       offaxis_hits
       @param override [description]
    */
    void GetProfileInfo(bool override=false);


    /**
       Calculates the following variables:
       length
       width
       @param override [description]
    */
    void RefineStartPoints(bool override=false);

    /**
       Calculates the following variables:
       hit_density_1D
       hit_density_2D
       angle_2d
       direction
       @param override [description]
    */
    void GetFinalSlope(bool override=false);

    void RefineDirection(bool override=false);

    void FillPolygon();

    void GetOpeningAngle();

    const larutil::PxPoint& RoughStartPoint() {return fRoughBeginPoint;}
    const larutil::PxPoint& RoughEndPoint() {return fRoughEndPoint;}

  protected:
    
    larutil::GeometryUtilities  *fGSer;
    TPrincipal *fPrincipal;

    /**
       This vector holds the pointer to hits. 
       This should be used for computation for speed.
    */
    std::vector<larutil::PxHit> fHitVector;

    // bool to control debug/verbose mode
    // defaults to off.
    bool verbose;

    //settable parameters:
    double fChargeCutoffThreshold[3]; 
    int fPlane;

    //this is required in RefineDirection
    double fQMinRefDir;
    
    std::vector< double > fChargeProfile;
    std::vector< double > fCoarseChargeProfile;
    int fCoarseNbins;
    int fProfileNbins;
    int fProfileMaximumBin;
    double fProfileIntegralForward;
    double fProfileIntegralBackward;
    double fProjectedLength;
    
    //extreme intercepts using the rough_2d_slope
   // double fInterHigh;
   // double fInterLow;
    double fBeginIntercept;
    double fEndIntercept;
    double fInterHigh_side;
    double fInterLow_side;
    
    // book keeping variables to validate completion of methods:
    bool fFinishedGetAverages;       
    bool fFinishedGetRoughAxis;      
    bool fFinishedGetProfileInfo;    
    bool fFinishedRefineStartPoints; 
    bool fFinishedRefineDirection;   
    bool fFinishedGetFinalSlope;     


    double fRough2DSlope;        // slope 
    double fRough2DIntercept;    // slope 
    larutil::PxPoint fRoughBeginPoint;
    larutil::PxPoint fRoughEndPoint;

    public:

    cluster::cluster_params fParams;

  }; //class ClusterParamsAlgNew
  
} //namespace cluster








#endif
