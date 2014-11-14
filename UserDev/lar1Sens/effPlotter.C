#include "TH1F.h"
#include "TCanvas.h"

void effPlotter(){
  lar1::EffCalc a;

  lar1::PlotUtils utils;

  a.setPath("/Users/cja33/nominal_ntuples/text/");
  a.setDetector("470m");
  a.getNominalData();


  utils.set_plot_style();


  TCanvas * events;



  TCanvas * c1 = new TCanvas("stupidNue","stupidNue",600,800);
  // c1->Divide(2);

  double minVertexEnergySignal = 0.015;
  int background = 6;
  while (minVertexEnergySignal < 0.505){
    c1 -> cd();
    // TH1F * temp = a.makeEfficiency(background,3,minVertexEnergySignal,minVertexEnergySignal);
    TH1F * tempNue  = a.makeNueEfficiency(3,minVertexEnergySignal,minVertexEnergySignal);
    // TH1F * tempPhot = a.makePhotonEfficiency(3,minVertexEnergySignal,minVertexEnergySignal);
    tempNue -> Draw();
    utils.add_plot_label(Form("VE: %g MeV",1000*minVertexEnergySignal),.6,.2,0.04,1,62,12);
    // utils.add_plot_label(Form("VE: %g MeV",1000*minVertexEnergySignal),.6,.2,0.04,1,62,12);
    c1 -> Print(Form("eff_%g.png",minVertexEnergySignal));
    events = a.createBackground(3,minVertexEnergySignal,minVertexEnergySignal);

    events -> Print(Form("events_norm_%g.png",minVertexEnergySignal));

    // events = a.createBackground(3,minVertexEnergySignal,minVertexEnergySignal,false);
    // events -> Print(Form("events_unnorm_%g.png",minVertexEnergySignal));
    minVertexEnergySignal += 0.005;
  }
}