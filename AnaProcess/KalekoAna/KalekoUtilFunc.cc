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
  //no name of hist, yes entries, mean, rms (right to left)
  //add 111110 also includes overflow and underflow
  gStyle->SetOptStat(1110);

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
  gStyle->SetTitleSize(0.05,"xy");
  //center the title
  gStyle->SetTitleX(0.5);
  gStyle->SetTitleAlign(23);

  //note, to center axis titles you need to do hist.GetXaxis().CenterTitle()
  //  gStyle->SetTitleOffset(1.2,"x");
  //  gStyle->SetTitleOffset(1.5,"y");
  
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


TGraphAsymmErrors* UtilFunctions::GenWindowEfficiencyGraph(TH1D* myRecoTrackAngleHist, double efficiency_denominator, int center_bin, double errorbar_CL){

  TGraphAsymmErrors *WindowEfficiencyGraph = new TGraphAsymmErrors();
  char graph_name[500];
  sprintf(graph_name,"WindowEfficiencyGraph_centerbin%d",center_bin);
  WindowEfficiencyGraph->SetName(graph_name);

  //calculating error bars with frequentist feldman-cousins method
  TEfficiency *pEff = new TEfficiency();
  pEff->SetStatisticOption(TEfficiency::kFFC);

  double degrees_per_bin = myRecoTrackAngleHist->GetBinWidth(center_bin);

  double window_integral, window_efficiency, upperlimit, lowerlimit = -1;
  
  int pt_ctr = 0;
  bool ReachedMaxWindowSize = false;
  int window_width = 1;
  while(!ReachedMaxWindowSize){

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
	       (double)window_width*degrees_per_bin,
	       window_efficiency);   

    WindowEfficiencyGraph->
      SetPointError(pt_ctr,
		    0,
		    0,
		    window_efficiency-lowerlimit,
		    upperlimit-window_efficiency);

    //    std::cout<< Form("window_width = %f radians, efficiency = %f, with uncertainties (upper,lower)=(%f,%f)",(double)window_width*degrees_per_bin,window_efficiency,upperlimit,lowerlimit)<<std::endl;


    //make the window one bin wider
    window_width += 1;
    pt_ctr++;
    //    std::cout<<"GenWindow: efficiency = "<<window_efficiency<<", ptr counter now = "<<pt_ctr<<"and myRecoTrackAngleHist.GetEntries() = "<<myRecoTrackAngleHist->GetEntries()<<std::endl;

    if(window_efficiency > 0.99 || pt_ctr > myRecoTrackAngleHist->GetNbinsX() ) ReachedMaxWindowSize = true;
  }

  return WindowEfficiencyGraph;
 
}

bool UtilFunctions::IsInDetectorVolume(TVector3 xyzpoint){
  //tpc is x = 0 to 260, y = 0 to 256, z = 0 to 1200
  double det_xmin = 0;  double det_xmax = 260;
  double det_ymin = -128;  double det_ymax = 128;
  double det_zmin = 0;  double det_zmax = 1200;

  if ( xyzpoint.X() > det_xmin && xyzpoint.X() < det_xmax &&
       xyzpoint.Y() > det_ymin && xyzpoint.Y() < det_ymax &&
       xyzpoint.Z() > det_zmin && xyzpoint.Z() < det_zmax   )
    return true;
  else
    return false;

}

TGraph* UtilFunctions::MakeGraphFromVectorOfPairs(std::vector<std::pair<double,double>> &myvecofpairs, TGraph* mygraph){

  int counter = 0;
  for(std::vector<std::pair<double,double>>::iterator iter = myvecofpairs.begin();
      iter != myvecofpairs.end();
      ++iter){
    mygraph->SetPoint(counter,iter->first,iter->second);
    counter++;
  }

  return mygraph;
}


double UtilFunctions::CalculateWeightedAvgTheta(const track t){
  //function to calculate weighted average angle through a reconstructed track
  //the "weight" is the length between the two points. weight longer stepsizes more heavily

  //initialize our angle
  double angle = 0;

  double sum_of_weights = 0;
  double weight = 0;

  //loop over points in the track
  for(int point = 0; point < (int)t.n_point()-1; ++point){
    weight = ( t.vertex_at(point+1) - t.vertex_at(point) ).Mag();
    angle += ( t.vertex_at(point+1) - t.vertex_at(point) ).Theta() * weight;
    sum_of_weights += weight;
  }//end loop over points in the track
  
  angle /= sum_of_weights;

  return angle;
}



#endif

