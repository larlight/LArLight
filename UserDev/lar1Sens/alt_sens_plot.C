#include "TNtuple.h"
#include "TMath.h"
#include "TFile.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TLatex.h"
#include "TAxis.h"
#include "TH1F.h"
#include "TH2D.h"
#include "TLegend.h"
#include "TMarker.h"
#include "TROOT.h"
#include "TStyle.h"

#include "lsnd_plot.c"


#include <iostream>
#include <fstream>

std::vector<float> Bin_LSND_Data(int npoints, std::vector<float> dm2points, std::vector<float> sin22thpoints);

void add_plot_label( char* label, double x, double y, double size = 0.05, int color = 1, int font = 62, int align = 22 ){

  TLatex *latex = new TLatex( x, y, label );
  latex->SetNDC();
  latex->SetTextSize(size);
  latex->SetTextColor(color);
  latex->SetTextFont(font);
  latex->SetTextAlign(align);
  latex->Draw();
}

void alt_sens_plot(){


  // Need to know where the ntuples are:
  TString ntuplePath = "/Users/cja33/Desktop/lar1Repos/lar1/MC2_proposal_ntuples/";
  TString fileNameRoot = ntuplePath;
  // fileNameRoot += "nue_appearance_ecalo1_ND_uB_FD_nu_scale_";
  fileNameRoot += "nue_appearance_ecalo1_";
  bool use100m = true;
  bool use470m = true;
  bool use700m = true;

// nue_appearance_ecalo1_ND_uB_FD_nu_nearDetStats_chi2.root

  std::string mode = "nu";
  
  if (use100m) fileNameRoot += "ND_";
  if (use470m) fileNameRoot += "uB_";
  if (use700m) fileNameRoot += "FD_";

  if (mode == "nu") fileNameRoot += "nu_";
  if (mode == "nubar") fileNameRoot += "nubar_";


  fileNameRoot += "nearDetStats_";
  // fileNameRoot += "chi2.root";

  // ND_uB_FD_nubar_scale_;

  int npoints = 250;  // same number of points as the pictures produced.

  // contains the points at which chi2 is calc'd
  std::vector<float>  dm2Vec;
  std::vector<float>  sin22thVec;
  std::vector<float>  farDetMassVec;

  // configure the dm2, sin22th points:
  double dm2min(0.01), dm2max(100.0);
  double sin22thmin(0.0001), sin22thmax(1);

  for (int i = 0; i <= npoints; i ++)
  {
    double dm2 = pow(10.,(TMath::Log10(dm2min)+(i*1./npoints)*TMath::Log10(dm2max/dm2min)));
    double sin22th = pow(10.,(TMath::Log10(sin22thmin)+(i*1./npoints)*TMath::Log10(sin22thmax/sin22thmin)));
    dm2Vec.push_back(dm2);
    sin22thVec.push_back(sin22th);
  }


      // set up the variables to be read in by the ntuple:
    Float_t dm2_temp;
    Float_t sin22th_temp;
    Float_t chi2_temp;

    TString thisFileName = fileNameRoot;
    thisFileName += "chi2.root";

    TFile f(thisFileName, "READ");

    TNtuple * thisNtuple;
    f.GetObject("chi2", thisNtuple);

    thisNtuple->SetBranchAddress("chisq", &chi2_temp);
    thisNtuple->SetBranchAddress("dm2", &dm2_temp);
    thisNtuple->SetBranchAddress("sin22th", &sin22th_temp);

    // build a vector for this ntuple, then push it back to the main set
    std::vector< std::vector< float> > thisChi2(npoints+1, std::vector<float>(npoints + 1, 0.0));
    int i_entry = 0;
    int i_dm2 = 0;
    int i_sin22th = 0;
    while (i_entry < thisNtuple->GetEntries())
    {
      thisNtuple->GetEntry(i_entry);
      thisChi2[i_dm2][i_sin22th] = chi2_temp;
      i_sin22th ++;
      if (i_sin22th % (npoints+1) == 0){
        i_sin22th = 0;
        i_dm2 ++;
      }
      i_entry ++;
    }
    std::cout << "Finished reading chi2. " << std::endl;
  

  std::vector<float> lsnd_bottom_edge = Bin_LSND_Data(npoints, dm2Vec, sin22thVec);

  // Now make a plot of chi2 along that line lsnd_bins
  float chi2_at_bottom_edge[npoints+1];
  float dm2_points[npoints+1];

  for (int i = 0; i <= npoints; i++)
  {
    dm2_points[i] = dm2Vec.at(i);
    chi2_at_bottom_edge[i] = sqrt(thisChi2[i][lsnd_bottom_edge.at(i)]);
  }
  TGraph * lsnd_bottom_edge_graph = new TGraph(npoints+1, dm2_points, chi2_at_bottom_edge);

  lsnd_bottom_edge_graph -> GetHistogram()->SetMinimum(0.0);
  // lsnd_bottom_edge_graph -> GetHistogram()->SetMaximum(20);

  TCanvas * canv2 = new TCanvas("c2","Sensitivity along bottom of LSND region",1000,500);
  if (mode == "nu")
    lsnd_bottom_edge_graph -> SetTitle("Sensitivity to 3+1 #nu signal along the lsnd edge.");
  else 
    lsnd_bottom_edge_graph -> SetTitle("Sensitivity to 3+1 #bar{#nu} signal along the lsnd edge.");
  lsnd_bottom_edge_graph -> GetXaxis() -> SetTitle("#Delta m^{2} (eV^{2})");
  lsnd_bottom_edge_graph -> GetYaxis() -> SetTitle("Sensitivity (#sigma)");
  lsnd_bottom_edge_graph -> GetXaxis() -> SetTitleSize(.06);
  lsnd_bottom_edge_graph -> GetYaxis() -> SetTitleSize(.06);
  lsnd_bottom_edge_graph -> GetXaxis() -> CenterTitle();
  lsnd_bottom_edge_graph -> GetYaxis() -> CenterTitle();


  lsnd_bottom_edge_graph -> GetXaxis() -> SetTitleOffset(0.85);
  lsnd_bottom_edge_graph -> GetYaxis() -> SetTitleOffset(0.35);

  // Drawing attributes:
  lsnd_bottom_edge_graph -> SetLineWidth(2);
  lsnd_bottom_edge_graph -> SetLineColor(4);
  canv2 -> SetGrid();
  canv2 -> SetLogx();

    // sprintf(label,"Detector Mass (ton): %.0f", farDetMassVec[mass_i]);
  lsnd_bottom_edge_graph -> Draw("AC");
    // add_plot_label(label, 0.345, 0.84);

  char name[50];
  // if (farDetMassVec[mass_i] < 100)
  //   sprintf(name,"lsnd_edge_00%0.f", farDetMassVec[mass_i]);
  // else if (farDetMassVec[mass_i] < 1000)
  //   sprintf(name,"lsnd_edge_0%0.f", farDetMassVec[mass_i]);
  // else 
    sprintf(name,"lsnd_edge_%s", mode.c_str() );

    TString name_temp = name;

    // save the plot:
    canv2 -> Print(name_temp+".pdf", "pdf");
    canv2 -> Print(name_temp+".png", "png");

}


std::vector<float> Bin_LSND_Data( int npoints,
                    std::vector<float> dm2points,
                    std::vector<float> sin22thpoints){

  // Get at the underlying array from the vectors, to set up the bins:
  float * xbins = & sin22thpoints[0];
  float * ybins = & dm2points[0];

  // for (auto & x : sin22thpoints) std::cout << "x: " << x << std::endl;

  TH2D * LSND_Data = new TH2D("data","data", npoints,xbins, npoints, ybins);

  const char* data_dir = "lsnd_data/";
  const Int_t NDATAFILES = 11;
  const char * file_list[NDATAFILES] = {"llreg_608_1.vec",
               "llreg_608_2.vec",
               "llreg_608_3.vec",
               "llreg_607_1.vec",
               "llreg_607_2.vec",
               "llreg_607_3.vec",
               "llreg_607_4.vec",
               "llreg_607_5.vec",
               "llreg_607_6.vec",
               "llreg_607_7.vec",
               "llreg_607_8.vec"};

  Int_t    nlines;
  Double_t dummy, dummy_old;
  Double_t x(0), y(0);
  for (Int_t ifile = 0; ifile<NDATAFILES; ifile++) {
    nlines = 0;
    char  filename[100];
    strcpy(filename, data_dir);
    //printf("%s\n",filename);
    strcat(filename, file_list[ifile]);
    //printf("%s\n",filename);
    ifstream datafile;
    datafile.open(filename, ios_base::in);
    //check if the file is open: 
    if (!datafile.is_open() ) {std::cerr << "lsnd_plot.C: file not opened" <<std::endl;}
    //else {std::cout << "Successfully opened " << filename << std::endl;}
    while (!datafile.eof()) {
      datafile >> dummy; 
      datafile >> dummy; 
      datafile >> x; 
      datafile >> y;
      LSND_Data -> Fill(x,y);
      nlines++;
      if (dummy == dummy_old) nlines--; //if last row was empty
      dummy_old = dummy;
    }
    datafile.close();
  }

  // TStyle * style = new TStyle(*gStyle);

  gStyle->SetOptStat(0000);
  gStyle->SetOptFit(0000);
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadRightMargin(0.05);
  gStyle->SetFrameBorderMode(0);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetPalette(0);
  gStyle->SetCanvasColor(0);
  gStyle->SetPadColor(0);


  TCanvas * d = new TCanvas("LSND Region", "LSND Region", 700, 700);
  d->SetLogx();
  d->SetLogy();  

  TH2D* hr1=new TH2D("hr1","hr1",500,0.0001,1.0,500,0.01,100.0);
  hr1->Reset();
  hr1->SetFillColor(0);
  hr1->SetTitle(";sin^{2}2#theta_{#mue};#Deltam^{2} (eV^{2})");
  hr1->GetXaxis()->SetTitleOffset(1.1);
  hr1->GetYaxis()->SetTitleOffset(1.2);
  hr1->GetXaxis()->SetTitleSize(0.05);
  hr1->GetYaxis()->SetTitleSize(0.05);
  hr1->SetStats(kFALSE);
  hr1->Draw();
  lsnd_plot(d);
  // gROOT->ProcessLine(".x ./lsnd_plot.c+(d)");
  // LSND_Data ->Draw("LF");

  TLegend* leg3=new TLegend(0.2,0.2,0.4,0.35);
  leg3->SetFillStyle(0);
  leg3->SetFillColor(0);
  leg3->SetBorderSize(0);
  leg3->SetTextSize(0.03);
  TGraph *gdummy1 = new TGraph();
  gdummy1->SetFillColor(29);
  TGraph *gdummy2 = new TGraph();
  gdummy2->SetFillColor(38);
  TMarker *gdummy3 = new TMarker();
  gdummy3 -> SetMarkerStyle(3);
  gdummy3 -> SetMarkerColor(1);
  TGraph *gdummy0 = new TGraph();
  gdummy0 -> SetFillColor(4);
  gdummy0 -> SetFillStyle(3004);
  leg3->AddEntry(gdummy2,"LSND 90% CL","F");
  leg3->AddEntry(gdummy1,"LSND 99% CL","F");
  leg3->AddEntry(gdummy3,"LSND Best Fit","P*");
  leg3->AddEntry(gdummy0,"Global Fit 90% CL (J. Kopp et al. arXiv:1303.3011)");   
  leg3->Draw();

  gStyle -> cd();

  std::vector<float> sin22thresult(npoints+1, 0.0);
  std::cout << "Result size: " << sin22thresult.size() << std::endl;
  // Now loop over the hist in y points and find the first point where the bins are filled:
  for (int i_dm2 = 1; i_dm2 <= npoints+1; i_dm2++)
  {
    for (int i_sin22th = npoints+1; i_sin22th > 0; i_sin22th--)
    {
      if (LSND_Data -> GetBinContent(i_sin22th,i_dm2) > 0) sin22thresult.at(i_dm2-1) = i_sin22th;
    }
    if (sin22thresult.at(i_dm2 -1) == 0) {
      if (i_dm2 != 1 ) sin22thresult.at(i_dm2 - 1) = sin22thresult.at(i_dm2-2);
      else sin22thresult.at(i_dm2 -1) = npoints;
    }
  }

  // Refine the line to extrapolate between the points that have no fit
  // refine(sin22thresult);



  // Need to validate that this works:
  float xpoints[npoints+1];
  float ypoints[npoints+1];
  
  // Pass it to a function to interpolate between constant sections.
  // refine(sin22thresult);

  for (int i = 0; i <= npoints; i++)
  {
    xpoints[i] = sin22thpoints[sin22thresult[i]];
    ypoints[i] = dm2points[i];
  }



  for (int i = 0; i <= npoints; i++)
    std::cout << "Line along dm2 = " << ypoints[i] << ",\tsin22th = " << xpoints[i] << std::endl;



  TGraph * line = new TGraph(npoints+1, xpoints, ypoints);
  line -> SetLineColor(4);
  line -> SetLineWidth(2);
  line -> Draw("same C");

  // Returns the index of what points to use in sin22th space
  return sin22thresult;

}
