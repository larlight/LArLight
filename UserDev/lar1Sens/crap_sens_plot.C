
#include "TString.h"
#include "TBox.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TFile.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TStyle.h"
#include <vector>

void alt_sens_plot(){

  TString fileSource = "/Users/cja33/nominal_ntuples/sens_plots/";

  std::vector<TString>names;
  names.push_back(fileSource+"altSensPlot_ND_100m_uB_T600_onaxis_nue_appearance_ecalo2_nu_vePhot0.05_gap3_lessCosmics_xsec_0_flux_6_.root");
  names.push_back(fileSource+"altSensPlot_ND_100m_T600_onaxis_nue_appearance_ecalo2_nu_vePhot0.05_gap3_lessCosmics_xsec_0_flux_6_.root");
  names.push_back(fileSource+"altSensPlot_ND_100m_uB_nue_appearance_ecalo2_nu_vePhot0.05_gap3_lessCosmics_xsec_0_flux_6_.root");

  std::vector<int> colors;
  colors.push_back(1);
  colors.push_back(2);
  colors.push_back(4);

  std::vector<TString> legEntry;
  legEntry.push_back("NecroBooNE, MicroBooNE, T600");
  legEntry.push_back("NecroBooNE, T600");
  legEntry.push_back("NecroBooNE, MicroBooNE");

  TFile * base = new TFile(names.front(), "OPEN");

  TGraph * fiveSigmaLine = (TGraph*) base -> Get("fiveSigmaLine");
  TGraph * threeSigmaLine = (TGraph*) base -> Get("threeSigmaLine");
  TGraph * ninetyCLLine = (TGraph*) base -> Get("ninetyCLLine");
  // fiveSigmaLine -> SetLineStyle(2);
  // fiveSigmaLine -> SetLineWidth(2);

  fiveSigmaLine->SetMinimum(0.0);
  fiveSigmaLine->SetMaximum(20);
  gStyle->SetPadBottomMargin(0.16);

  TCanvas * canv2 = new TCanvas("c2","Sensitivity along bottom of LSND region",1000,500);
  fiveSigmaLine -> SetTitle("Sensitivity to 3+1  #nu signal along the LSND 90% CL");
  fiveSigmaLine -> GetXaxis() -> SetTitle("#Delta m^{2} (eV^{2})");
  fiveSigmaLine -> GetYaxis() -> SetTitle("Sensitivity #left( #sqrt{ #Delta #chi^{2}} #right)");
  fiveSigmaLine -> GetXaxis() -> SetTitleSize(.06);
  fiveSigmaLine -> GetYaxis() -> SetTitleSize(.06);
  fiveSigmaLine -> GetXaxis() -> CenterTitle();
  fiveSigmaLine -> GetYaxis() -> CenterTitle();

  fiveSigmaLine -> GetXaxis() -> SetTitleOffset(0.85);
  fiveSigmaLine -> GetYaxis() -> SetTitleOffset(0.55);

  canv2 -> SetGrid();
  canv2 -> SetLogx();
  canv2 -> SetFillColor(41);

  fiveSigmaLine -> Draw();

  TBox * slab1 = new TBox(3.37287,0.0,5.44503,20);
  TBox * slab2 = new TBox(7.72681,0.0,9.63829,20);

  slab1->SetFillStyle(3001);
  slab1->SetFillColor(kYellow+1);
  slab1->Draw();
  slab2->SetFillStyle(3001);
  slab2->SetFillColor(kYellow+1);
  slab2->Draw();

  fiveSigmaLine -> SetLineColor(5);
  threeSigmaLine -> SetLineColor(5);
  ninetyCLLine -> SetLineColor(5);

  fiveSigmaLine -> Draw("same");
  threeSigmaLine -> Draw("same");
  ninetyCLLine -> Draw("same");

  TLegend * legend = new TLegend(0.15,0.6,0.45,0.89);
  // legend -> SetFillStyle(0);
  legend -> SetFillColor(kGray);
  // legend -> SetBorderSize(0);
  legend -> SetMargin(0.1);
  legend -> SetTextSize(0.045);

  for (unsigned int i = 0; i < names.size(); i++){
    TFile * temp = new TFile(names[i],"OPEN");
    TGraph * graph1 = (TGraph*) temp -> Get("graph_0");
    TGraph * graph2 = (TGraph*) temp -> Get("graph_1");
    TGraph * graph3 = (TGraph*) temp -> Get("graph_2");

    graph1 -> SetLineColor(kYellow - i);
    graph2 -> SetLineColor(kYellow - i);
    graph3 -> SetLineColor(kYellow - i);
    graph1 -> Draw("same");
    graph2 -> Draw("same");
    graph3 -> Draw("same");

    legend -> AddEntry(graph1,legEntry[i],"l");

  }

  legend -> AddEntry(fiveSigmaLine,"5 #sigma Sensitivity","l");
  legend -> AddEntry(threeSigmaLine,"3 #sigma Sensitivity","l");
  legend -> AddEntry(ninetyCLLine,"90% CL Sensitivity","l");
  legend -> Draw();

}