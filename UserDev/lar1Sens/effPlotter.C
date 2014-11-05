#include "TH1F.h"
#include "TCanvas.h"

void effPlotter(){
  lar1::EffCalc a;

  lar1::PlotUtils utils;

  a.setPath("/Users/cja33/nominal_ntuples/text/");
  a.getNominalData();

  TCanvas * c1 = new TCanvas("stupid","stupid",600,800);

  double minVertexEnergySignal = 0.5;
  int background = 6;
  while (minVertexEnergySignal > 0.001){
    TH1F * temp = a.makeEfficiency(background,3,minVertexEnergySignal,minVertexEnergySignal);
    temp -> Draw();
    utils.add_plot_label(Form("VE: %g",minVertexEnergySignal),.6,.4);
    c1 -> Print(Form("canvas_%d_%g.png",background,minVertexEnergySignal));
    minVertexEnergySignal -= 0.025;
  }
}