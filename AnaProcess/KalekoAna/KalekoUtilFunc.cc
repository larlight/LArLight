#ifndef KALEKOUTILFUNC_CC
#define KALEKOUTILFUNC_CC

#include "KalekoUtilFunc.hh"

void UtilFunctions::set_style(){

  TGaxis::SetMaxDigits(10);
  //TGaxis::SetNoExponent();
  gROOT->SetStyle("Plain");
  gStyle->SetEndErrorSize(0);
  gStyle->SetStatX(0.88);
  gStyle->SetStatY(0.84);
  gStyle->SetStatW(0.20);
  gStyle->SetStatH(0.10);
  gStyle->SetOptFit(1);
  gStyle->SetOptDate(0);
  gStyle->SetOptStat(111110);

  // Canvas
  gStyle->SetCanvasDefW(600);
  gStyle->SetCanvasDefH(500);
  gStyle->SetCanvasColor(0);
  gStyle->SetStatColor(0);
  gStyle->SetStatBorderSize(1);
  gStyle->SetFrameFillColor(10);
  gStyle->SetPadColor(23);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetTitleColor(0, "");
  gStyle->SetPadTopMargin(0.11);
  gStyle->SetPadRightMargin(0.08);
  gStyle->SetPadLeftMargin(0.13);
  gStyle->SetPadBottomMargin(0.10);
  
  // Font ... use large Times-Roman fonts
  gStyle->SetTextFont(42);
  gStyle->SetTextSize(0.01);
  gStyle->SetLabelFont(22,"xyz");
  gStyle->SetLabelSize(0.04,"xy");
  gStyle->SetLabelOffset(0.01,"xy");
  gStyle->SetTitleFont(22,"xyz");
  gStyle->SetTitleSize(0.04,"xy");
  gStyle->SetTitleOffset(1.2,"x");
  gStyle->SetTitleOffset(1.5,"y");
  
  // Gradient Color
  const Int_t NRGBs = 5;
  const Int_t NCont = 99;
  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00};
  Double_t red[NRGBs] =   { 0.00, 0.00, 0.87, 1.00, 0.51};
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00};
  Double_t blue[NRGBs] =  { 0.51, 1.00, 0.12, 0.00, 0.00};
  TColor::CreateGradientColorTable(NRGBs,stops,red,green,blue,NCont);
  gStyle->SetNumberContours(NCont);
  
}


TGraphAsymmErrors* UtilFunctions::GenWindowEfficiencyGraph(TH1D* myRecoTrackAngleHist, double efficiency_denominator, int center_bin, int max_window_binradius, double errorbar_CL){

  TGraphAsymmErrors *WindowEfficiencyGraph = new TGraphAsymmErrors();
  char graph_name[500];
  sprintf(graph_name,"WindowEfficiencyGraph_centerbin%d",center_bin);
  WindowEfficiencyGraph->SetName(graph_name);

  //calculating error bars with frequentist feldman-cousins method
  TEfficiency *pEff = new TEfficiency();
  pEff->SetStatisticOption(TEfficiency::kFFC);

  double radians_per_bin = myRecoTrackAngleHist->GetBinWidth(center_bin);

  double window_integral, window_efficiency, upperlimit, lowerlimit = -1;
  
  int pt_ctr = 0;
  for (int window_width = max_window_binradius;
       window_width > 0;
       --window_width){

    window_integral = myRecoTrackAngleHist->
      Integral(center_bin-window_width,
	       center_bin+window_width);
    
    window_efficiency = (double)window_integral/(double)efficiency_denominator;
    
    lowerlimit = pEff->FeldmanCousins(efficiency_denominator,
				      window_integral,
				      errorbar_CL,
				      false);
    upperlimit = pEff->FeldmanCousins(efficiency_denominator,
				      window_integral,
				      errorbar_CL,
				      true);
    WindowEfficiencyGraph->
      SetPoint(pt_ctr, 
	       (double)window_width*radians_per_bin,
	       window_efficiency);   

    WindowEfficiencyGraph->
      SetPointError(pt_ctr,
		    0,
		    0,
		    lowerlimit,
		    upperlimit);

    std::cout<< Form("window_width = %f radians, efficiency = %f, with uncertainties (upper,lower)=(%f,%f)",(double)window_width*radians_per_bin,window_efficiency,upperlimit,lowerlimit)<<std::endl;

    pt_ctr++;
  }

  return WindowEfficiencyGraph;
 
}



#endif
