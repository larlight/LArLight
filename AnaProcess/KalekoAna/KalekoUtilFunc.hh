/**
 * \file KalekoUtilFunc.hh
 *
 * \ingroup KalekoAna
 * 
 * \brief Class def header for a class UtilFunctions
 *
 * @author davidkaleko
 */

/** \addtogroup KalekoAna

    @{*/



#ifndef KALEKOUTILFUNC_HH
#define KALEKOUTILFUNC_HH

#include <iostream>

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


class UtilFunctions {

public:

  /// The usual constructor
  UtilFunctions(){};
  
  /// The usual destructor
  ~UtilFunctions(){};

  /// Function to set style parameters to Kaleko's liking
  static void set_style();

  /// Function to generate window efficiency histograms
  TGraphAsymmErrors* GenWindowEfficiencyGraph(TH1D* input_histo, double efficiency_denominator, int center_bin, double errorbar_CL);


  /// Some utility constants
  double DegreesPerRadian = 57.2957795;
  double RadiansPerDegree = 0.01745329;

};

#endif
/** @} */ // end of doxygen group 
