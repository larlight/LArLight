/**
 * \file KalekoUtilFunc.hh
 *
 * \ingroup KalekoTrackAna
 * 
 * \brief Class def header for a class UtilFunctions
 *
 * @author davidkaleko
 */

/** \addtogroup KalekoTrackAna

    @{*/



#ifndef KALEKOUTILFUNC_HH
#define KALEKOUTILFUNC_HH

#include <iostream>

#include <track.hh>

#include <TCanvas.h>
#include <TLine.h>
#include <TPad.h>
#include <TPaveText.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TGaxis.h>
#include <TColor.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <TH1.h>
#include <TEfficiency.h>
#include <TVector3.h>

namespace kaleko {
  class UtilFunctions {
    
  public:
    
    /// The usual constructor
    UtilFunctions(){
      /// Some utility constants
      DegreesPerRadian=57.2957795;
      RadiansPerDegree=0.01745329;
    };
    
    /// The usual destructor
    ~UtilFunctions(){};
    
    /// Function to set style parameters to Kaleko's liking
    static void set_style();
    
    /// Function to generate window efficiency histograms
    TGraphAsymmErrors* GenWindowEfficiencyGraph(TH1D* input_histo, double efficiency_denominator, int center_bin, double errorbar_CL);
    
    /// Function to check whether a point is inside of the detector volume
    bool IsInDetectorVolume(TVector3 xyzpoint);
    
    /// Function to turn a vector of pairs into a TGraph, each pair is (x,y)
    TGraph* MakeGraphFromVectorOfPairs(std::vector< std::pair<double,double> > &myvecofpairs, TGraph* mygraph);
    
    double CalculateWeightedAvgTheta(const larlight::track t);
    
    /// Some utility constants, initialized in constructor
    double DegreesPerRadian;
    double RadiansPerDegree;
    
    
  };
}

#endif
/** @} */ // end of doxygen group 
