// File CoreysSuperAwesomePlotter.C

#include "TGraph.h"
#include "TNtuple.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TString.h"
#include "TMath.h"
#include "TH2D.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TLatex.h"


#include <fstream>
#include <iostream>

bool fileExists(const char *filename)
{
  ifstream ifile(filename);
  return ifile.is_open();
}

std::vector<float> smoothVector(const std::vector<float> & data, int range = 3){
  
  std::vector<float> output(data.size(), 0.0);
  for (int i = 0; i < data.size(); ++i)
  {
    if (i < range){
      for (int val = 0; val <= 2*i; val ++){
        output[i] += data[val];
      }
      output[i] /= 2*i+1;      
    }
    if (i >= data.size() - range){
      double temp_range= data.size() - i - 1;
      for (int val = i - temp_range; val <= i + temp_range; val ++){
        output[i] += data[val];
      }
      output[i] /= 2*temp_range+1;     
    }
    else{
      for (int val = i - range; val <= i + range; val ++){
        output[i] += data[val];
      }
      output[i] /= 2*range+1;
    }
  }
  return output;
}

void add_plot_label( char* label, double x, double y, double size = 0.05, int color = 1, int font = 62, int align = 22 ){

  TLatex *latex = new TLatex( x, y, label );
  latex->SetNDC();
  latex->SetTextSize(size);
  latex->SetTextColor(color);
  latex->SetTextFont(font);
  latex->SetTextAlign(align);
  latex->Draw();
}

std::vector< std::vector< float> > getChi2MapFromFile(TString fileName, int npoints = 500){
  if (!fileExists(fileName)){
    std::cerr << "The file you asked for ... \n\t"
              << fileName << "\n doesn't exist, exiting before root seg faults.\n";
    exit(-1);
  }

  std::vector< std::vector< float> > thisChi2(npoints+1, std::vector<float>(npoints + 1, 0.0));
    
  TFile f(fileName, "READ");

  TNtuple * thisNtuple;
  f.GetObject("chi2", thisNtuple);

  // set up the variables to be read in by the ntuple:
  Float_t dm2_temp;
  Float_t sin22th_temp;
  Float_t chi2_temp;

  thisNtuple->SetBranchAddress("chisq", &chi2_temp);
  thisNtuple->SetBranchAddress("dm2", &dm2_temp);
  thisNtuple->SetBranchAddress("sin22th", &sin22th_temp);

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

  return thisChi2;

}



void plot(std::string sens = "5s"){

  std::cout << "Default sens is: " << sens << std::endl;

  // First, want the names of all the chi2 files to read in:
  std::vector<TString> files;

  TString directory = "/Users/cja33/Desktop/booster-oscillation-analyses/nue_figures/chi2Files/";

  files.push_back(directory+"nue_appearance_ecalo2_dist0_nuND_100m_T600_onaxis_covMat_shapeOnly_chi2.root");
  files.push_back(directory+"nue_appearance_ecalo2_dist0_nuND_150m_T600_onaxis_covMat_shapeOnly_chi2.root");
  files.push_back(directory+"nue_appearance_ecalo2_dist0_nuND_200m_T600_onaxis_covMat_shapeOnly_chi2.root");


  // files.push_back(directory+"nue_appearance_ecalo2_dist0_nuND_100m_uB__covMat_shapeOnly_chi2.root");
  // files.push_back(directory+"nue_appearance_ecalo2_dist0_nuND_150m_uB__covMat_shapeOnly_chi2.root");
  // files.push_back(directory+"nue_appearance_ecalo2_dist0_nuND_200m_uB__covMat_shapeOnly_chi2.root");

  // files.push_back(directory+"nue_appearance_ecalo2_dist0_nuND_100m_uB_T600_onaxis_covMat_shapeOnly_chi2.root");
  // files.push_back(directory+"nue_appearance_ecalo2_dist0_nuND_150m_uB_T600_onaxis_covMat_shapeOnly_chi2.root");
  // files.push_back(directory+"nue_appearance_ecalo2_dist0_nuND_200m_uB_T600_onaxis_covMat_shapeOnly_chi2.root");

  TString referenceFile = files[0];

  std::vector<int> colors;
  colors.push_back(kBlack);
  colors.push_back(kRed-3);
  colors.push_back(kBlue-3);
  colors.push_back(kGreen);

  std::vector<std::string> names;
  names.push_back(" LAr1-ND (100m), T600 (600m)");
  names.push_back(" LAr1-ND (150m), T600 (600m)");
  names.push_back(" LAr1-ND (200m), T600 (600m)");

  // names.push_back(" LAr1-ND (100m), #muBooNE (470m)");
  // names.push_back(" LAr1-ND (150m), #muBooNE (470m)");
  // names.push_back(" LAr1-ND (200m), #muBooNE (470m)");
  // names.push_back(" LAr1-ND (100m), #muBooNE (470m), T600 (600m)");
  // names.push_back(" LAr1-ND (150m), #muBooNE (470m), T600 (600m)");
  // names.push_back(" LAr1-ND (200m), #muBooNE (470m), T600 (600m)");
  // names.push_back(" LAr1-ND (100m), T600 (600m)");
  // names.push_back(" MicroBooNE, nominal");
  // names.push_back(" MicroBooNE, no cosmics cut");
  // names.push_back(" MicroBooNE, 50cm cosmics cut");
  // names.push_back(" MicroBooNE, 100cm cosmics cut");


  // using "sens" to determine which curve to plot against
  // options are "90", "3s", "5s" ONLY
  if (sens != "90" && sens != "3s" && sens != "5s" ) {
    std::cerr << "Sorry, this macro just isn't smart enough to handle your request to plot "
              << sens << ". The options are \"90\", \"3s\", or \"5s\"." << std::endl;
    exit(-1);
  }

  // gROOT->ProcessLine(".L header.C+");

  int npoints = 500;
  // configure the dm2, sin22th points:
  double dm2min(0.01), dm2max(100.0);
  double sin22thmin(0.0001), sin22thmax(1.0);

// dm2 = pow(10.,(TMath::Log10(dm2min)+(dm2point*1./npoints)*TMath::Log10(dm2max/dm2min)));
// sin22th = pow(10.,(TMath::Log10(sin22thmin)+(sin22thpoint*1./npoints)*TMath::Log10(sin22thmax/sin22thmin)));
  

  // Need to have some lookup stuff:
  std::vector<float> dm2_IndexToPoint;
  std::vector<float> sin22th_IndexToPoint;
  // std::map<float, int> dm2_PointToIndex;
  // std::map<float, int> sin22th_PointToIndex;
  
  // dm2_IndexToPoint.reserve(npoints+1);
  // sin22th_IndexToPoint.reserve(npoints+1);
  // dm2_PointToIndex.reserve(npoints+1);
  // sin22th_PointToIndex.reserve(npoints+1);

  std::cout << "Setting up the array for dm2, sin22th." << std::endl;

  for (int i = 0; i <= npoints; i ++)
  {
    double dm2 = pow(10.,(TMath::Log10(dm2min)+(i*1./npoints)*TMath::Log10(dm2max/dm2min)));
    double sin22th = pow(10.,(TMath::Log10(sin22thmin)+(i*1./npoints)*TMath::Log10(sin22thmax/sin22thmin)));
    dm2_IndexToPoint.push_back(dm2);
    sin22th_IndexToPoint.push_back(sin22th);
    // dm2_PointToIndex[dm2] = i;
    // sin22th_PointToIndex[sin22th] = i;
  }

  // Actually read in the chi2 from file:
  std::vector < std::vector< std::vector< float> > > chi2_by_file;
  for(auto file : files){
    std::cout << "Reading in chi2 from "<< file << std::endl;
    chi2_by_file.push_back(getChi2MapFromFile(file, npoints));
  }

  // Get the reference point here!!!!
  //
  //
  if (!fileExists(referenceFile)){
    std::cerr << "The file you asked for ... \n\t"
              << referenceFile << "\n doesn't exist, exiting before root seg faults.\n";
    exit(-1);
  }
  TFile f(referenceFile);
  TGraph * refGraph;
  if (sens == "90") refGraph = (TGraph*) f.Get("Graph;1");
  else if (sens == "3s") refGraph = (TGraph*) f.Get("Graph;2");
  else if (sens == "5s") refGraph = (TGraph*) f.Get("Graph;3");


  // This is the important variable:
  std::vector<float> chi2Reference;
  std::vector<int> reference_dm2_point;
  std::vector<int> reference_sin22th_point;

  // Get the arrays of points out of the graph:
  Double_t * x_points = refGraph->GetX();
  Double_t * y_points = refGraph->GetY();

  // std::cout << "number of points in graph: " << refGraph->GetN() << std::endl;

  for (int point = 0; point <= npoints; ++point){
    int temp_dm2point = npoints * TMath::Log10(y_points[point]/dm2min)/ TMath::Log10(dm2max/dm2min);
    int temp_sin22thpoint = npoints * TMath::Log10(x_points[point]/sin22thmin)/ TMath::Log10(sin22thmax/sin22thmin);
    reference_dm2_point.push_back(temp_dm2point);
    reference_sin22th_point.push_back(temp_sin22thpoint);
    // std::cout <<"Matched ("<<x_points[point]<<","<<y_points[point]<<") to ("
              // <<reference_dm2_point[point]<<","<<reference_sin22th_point[point]<<").\n";
  }

  // I can't think of a situation where reference_dm2_point isn't equal to
  // dm2_IndexToPoint, because I expect it to just project onto the dm2 axis.
  // But, in case it's something pathological (why?!) I am allowing the
  // reference tgraph to just wander...
  //
  
  reference_dm2_point.resize(npoints+1);
  reference_sin22th_point.resize(npoints+1);
  chi2Reference.resize(npoints+1);


  /////////

  std::vector<std::vector<float> > chi2_alongLine_byFile;
  chi2_alongLine_byFile.resize(files.size());


  // Now, loop over the files and get the chi2 for each of the files
  // storing the ratios in vectors and then plot them onto TGraphs
  std::vector< std::vector<float> > ratioByFile;
  ratioByFile.resize(files.size());

  for(unsigned int file = 0; file < files.size(); file ++ ){
    chi2_alongLine_byFile[file].resize(npoints+1);
    ratioByFile[file].resize(npoints+1);
    std::cout << "sigma for file: " << files[file] << std::endl;
    for (int point = 0; point <= npoints; ++point)
    {
      // fill the stuff here:
      // the reference points are set up to be the index to go to in the chi2map
      chi2_alongLine_byFile[file][point] = sqrt(chi2_by_file[file][reference_dm2_point[point]][reference_sin22th_point[point]]);
      if (file == 0) chi2Reference[point] = chi2_alongLine_byFile[file][point];
      std::cout << "dm2: " << dm2_IndexToPoint[point]
                << ", sigma: " << sqrt(chi2_alongLine_byFile[file][point]) << std::endl;
    }
    chi2_alongLine_byFile[file] = smoothVector(chi2_alongLine_byFile[file]);
  }

  // for(unsigned int file = 0; file < files.size(); file ++ ){
  // for (int point = 0; point <= npoints; ++point)
  // {
  // ratioByFile[file][point] = chi2_alongLine_byFile[file][point] / chi2Reference[point];

  // }
  // }

  TLegend * leg = new TLegend(0.18,0.72,0.28,0.61,NULL,"brNDC");
  leg->SetBorderSize(0);
  leg->SetTextSize(0.025);
  leg->SetTextFont(62);
  leg->SetTextAlign(12);
  leg->SetLineColor(1);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);

  // Now do the actual plotting:
  std::vector<TGraph *> graphsForPlotting;
  graphsForPlotting.resize(files.size());
  for(unsigned int file = 0; file < files.size(); file ++ ){
    graphsForPlotting[file] = new TGraph(npoints+1,&(dm2_IndexToPoint[0]), &(chi2_alongLine_byFile[file][0]));
    graphsForPlotting[file] -> SetLineWidth(3);
    graphsForPlotting[file] -> SetLineColor(colors[file]);
    leg->AddEntry(graphsForPlotting[file],names[file].c_str(),"l");
  }


  TCanvas * canv = new TCanvas("canv","Ratio of blah blah blah",650,650);
  gStyle->SetOptStat(0);
  canv->SetFillColor(0);
  canv->SetBorderMode(0);
  canv->SetBorderSize(2);
  canv->SetLogx();
  // canv->SetLogy();
  canv->SetLeftMargin(0.15);
  canv->SetRightMargin(0.05);
  canv->SetTopMargin(0.05);
  canv->SetBottomMargin(0.15);
  canv->SetFrameBorderMode(0);
  canv->SetFrameBorderMode(0);
  // fill in plot settings here with:
  //
  // graphsForPlotting.front() -> SetMinimum()
  // etc.
  //
  TH2D *hr1__1 = new TH2D("hr1__1","",500,0.1,dm2max,500,1.1,15);
  hr1__1->SetDirectory(0);
  hr1__1->SetStats(0);
  hr1__1->GetYaxis()->SetTitle("Coverage of nominal uB 5#sigma C.L. [#sigma]");
  // hr1__1->GetYaxis()->SetTitle("Ratio of 90% C.L. Limits");
  hr1__1->GetYaxis()->CenterTitle(true);
  hr1__1->GetYaxis()->SetLabelFont(62);
  hr1__1->GetYaxis()->SetLabelOffset(0.003);
  hr1__1->GetYaxis()->SetLabelSize(0.04);
  hr1__1->GetYaxis()->SetTitleSize(0.05);
  hr1__1->GetYaxis()->SetTitleFont(62);
  hr1__1->GetYaxis()->SetTitleOffset(1.2);


  hr1__1->GetXaxis()->SetTitle("#Deltam#lower[0.4]{#scale[.6]{41}}#lower[-.6]{#scale[.6]{2}} [eV#lower[-.6]{#scale[.6]{2}}]");
  hr1__1->GetXaxis()->CenterTitle(true);
  hr1__1->GetXaxis()->SetLabelFont(62);
  hr1__1->GetXaxis()->SetLabelSize(0.04);
  hr1__1->GetXaxis()->SetTitleSize(0.05);
  hr1__1->GetXaxis()->SetTitleFont(62);
  hr1__1->GetXaxis()->SetTitleOffset(1.2);
  
  hr1__1->Draw("");
  
  // graphsForPlotting.front()->Draw("l");
  for(unsigned int file = 0; file < files.size(); file ++ ){
    graphsForPlotting[file] ->Draw("l same");
  }

  leg->Draw("same");

  TLatex *tex_pre = new TLatex(.18,.74,"PRELIMINARY");
  tex_pre->SetNDC();
  tex_pre->SetTextFont(62);
  tex_pre->SetTextColor(kRed-3);
  tex_pre->SetTextSize(0.03);
  tex_pre->Draw();

  TLatex *tex_mode = new TLatex(.18,.915,"#nu mode, CC Events");
  tex_mode->SetNDC();
  tex_mode->SetTextFont(62);
  tex_mode->SetTextSize(0.025);
  tex_mode->Draw();

  TLatex *tex_un = new TLatex(.18,.87,"Statistical and 20\% Systematic Uncert. Only");
  // TLatex *tex_un = new TLatex(.18,.87,"Statistical Uncert. Only");
  tex_un->SetNDC();
  tex_un->SetTextFont(62);
  tex_un->SetTextSize(0.025);
  tex_un->Draw();

  TLatex *tex_E = new TLatex(.18,.83,"Reconstructed Energy");
  tex_E->SetNDC();
  tex_E->SetTextFont(62);
  tex_E->SetTextSize(0.025);
  tex_E->Draw();

  TLatex *tex_eff = new TLatex(.18,.79,"80% #nu#lower[0.4]{e} Efficiency");
  tex_eff->SetNDC();
  tex_eff->SetTextFont(62);
  tex_eff->SetTextSize(0.025);
  tex_eff->Draw();

  TLatex *tex_ana = new TLatex(.18,.7,"Shape Only Analysis");
  tex_ana->SetNDC();
  tex_ana->SetTextFont(62);
  tex_ana->SetTextSize(0.03);
  // tex_ana->Draw();

  TLatex *tex_sca = new TLatex(.18,.66,"ND Stats. Scaled to 100m");
  tex_sca->SetNDC();
  tex_sca->SetTextFont(62);
  tex_sca->SetTextColor(kRed-3);
  tex_sca->SetTextSize(0.025);
  // tex_sca->Draw();


  std::cout << "Finished plotting!" << std::endl;

  return;

}


void CL_fitter(){
  plot();
  return;
}