////////////////////////////////////////////////////////////////////////
// ClusterParamsAlg.h
//
// ClusterParamsAlg class
//
// Andrzej Szelc (andrzej.szelc@yale.edu)
//
////////////////////////////////////////////////////////////////////////
#ifndef CLUSTERPARAMSALG_H
#define CLUSTERPARAMSALG_H

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
   
  class ClusterParamsAlg {

  public:
    
    ClusterParamsAlg(std::string mother="");
    
    void reconfigure(); 
    
    int Find2DAxisRough(double &lineslope,double &lineintercept,double &goodness,
			const std::vector <larlight::hit*> &hitlist); /**Calculate 2D angle histograms, provided vertex is know */ 
    
    int Find2DAxisRoughHighCharge(double &lineslope,double &lineintercept,double &goodness,
				  const std::vector <larlight::hit*> &hitlist);  
    //  void CalculateAxisParameters(unsigned nClust, std::vector < larlight::hit >  hitlist,double wstart,double tstart,double wend,double tend);
    
    int  Find2DStartPointsBasic(const std::vector<larlight::hit*> &hitlist,
				double &wire_start,double &time_start,double &wire_end,double &time_end);
    
    int Find2DStartPointsBasic(double lineslope,double lineintercept,
			       const std::vector<larlight::hit*> &hitlist,
			       double &wire_start,double &time_start,double &wire_end,double &time_end);
    
    int Find2DStartPointsHighCharge(const std::vector<larlight::hit*> &hitlist,
				    double &wire_start,double &time_start,double &wire_end,double &time_end);
    
    int FindTrunk(const std::vector <larlight::hit*> &hitlist,
		  double &wstn,double &tstn,double &wendn,double &tendn,double lineslope,double lineintercept);
    
    int FindTrunk(const std::vector <larlight::hit*> &hitlist,
		  double &wstn,double &tstn,double &wendn,double &tendn); //overloaded interface, will calculate the lineslope and basic points using Find2DStartPointsBasic.
    
    void FindDirectionWeights(double lineslope,double wstn,double tstn,double wendn,double tendn, 
			      const std::vector <larlight::hit*> &hitlist,
			      double &HiBin,double &LowBin,double &invHiBin,double &invLowBin, double *altWeight=0); 
    
    void FindSideWeights(double lineslope,double lineintercept,double wstn,double tstn,int direction, 
			 const std::vector <larlight::hit*> &hitlist,
			 double &sideWeightCharge); 

    /*
    void RefineStartPointsHough(const std::vector<larlight::hit*> &hitlist,
				double & wire_start,
				double & time_start, 
				double & wire_end,
				double & time_end, 
				int &direction);
    */    
    int DecideClusterDirection(const std::vector <larlight::hit*> &hitlist,
			       double lineslope,double wstn,double tstn,double wendn,double tendn);
    
    int FindHitCountDirection(const std::vector<larlight::hit*> &hitlist, 
			      double  wire_start,double  time_start, double  wire_end,double  time_end, double lineslope);
    
    int FindMultiHitDirection(const std::vector<larlight::hit*> &hitlist, 
			      double  wire_start,double  time_start, double  wire_end,double  time_end, double lineslope);
    
    bool isShower(double lineslope,double wstn,double tstn,double wendn,double tendn, 
		  const std::vector <larlight::hit*> &hitlist);
    
    int MultiHitWires(const std::vector <larlight::hit*> &hitlist);
    
    //     
    //     void RefineStartPoints(unsigned int nClust, std::vector< larlight::hit >  hitlist, double  wire_start,double time_start);
    //     
    //     double Get2DAngleForHit( larlight::hit starthit,std::vector < larlight::hit > hitlist);
    //     
//     double Get2DAngleForHit( unsigned int wire, double time,std::vector < larlight::hit > hitlist);
    int FindPrincipalDirection(const std::vector<larlight::hit*> &hitlist, 
			       double  wire_start,double  time_start, double  wire_end,double  time_end,double lineslope);

    void GetPrincipal(const std::vector<larlight::hit*> &hitlist, TPrincipal * pc);

    int Find2DAxisPrincipal(double &lineslope,double &lineintercept,double &goodness,
			    const std::vector <larlight::hit*> &hitlist);

    int Find2DAxisPrincipalHighCharge(double &lineslope,double &lineintercept,double &goodness,
				      const std::vector <larlight::hit*> &hitlist);

    double Get2DAngleForHit( unsigned int swire,double stime,
			     const std::vector <larlight::hit*> &hitlist);
    
  private:
    
    const std::string extname; 
    std::string funcname; 
    //HoughBaseAlg fHBAlg;  
    double fWiretoCm,fTimetoCm,fWireTimetoCmCm;
    double fWirePitch ;   // wire pitch in cm
    double fTimeTick;
    TF1 * linefittest_cm;
    TH2F *  tgxtest;
    TH1F *  hithistinv, * hitinv2, * hitreinv2;
    TH1F *  fh_omega_single;
    int fMinHitListSize;
    double fOutlierRadius;
    larutil::GeometryUtilities *gser;
    std::vector<double> fChargeCutoffThreshold;
    double fSelectBoxSizePar;
    double fSelectBoxSizePerp;
    // double fSelectBoxDiff;
    bool fForceRightGoing;
    //helper functions

    void FindExtremeIntercepts(const std::vector <larlight::hit*> &hitlist,
			       double perpslope,
			       double &inter_high,
			       double &inter_low);  
    
    void SelectLocalHitlist(const std::vector<larlight::hit*> &hitlist, 
			    std::vector< larlight::hit* > &hitlistlocal, 
			    double  wire_start,double time_start, double radlimit);
    
    void CreateHighHitlist(double threshold,
			   const std::vector<larlight::hit*> &hitlist,
			   std::vector<larlight::hit*> &hitlist_high);
    
    void FindMinMaxWiresTimes(double &MinWire, double &MaxWire,double &MinTime,double &Maxtime,double &MeanCharge,
			      const std::vector<larlight::hit*> &hitlist);
    
    double fHitDensityCutoff;
    double fMultiHitWireCutoff;
    double fOffAxisHitsCutoff;
    double fPrincipalComponentCutoff;
    bool fShowerSelisORorAND;
    
    
    
  }; //class ClusterParamsAlg
  
} //namespace cluster








#endif
