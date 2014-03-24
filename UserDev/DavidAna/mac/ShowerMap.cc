void ShowerMap() {

  TFile *f = TFile::Open("./../mcshower.root");

  TH2D *hXYE  = (TH2D*)f->Get("XYEnergy");
  TH2D *hXZE  = (TH2D*)f->Get("XZEnergy");
  TH2D *hYZE  = (TH2D*)f->Get("YZEnergy");
  TH3D *hSh   = (TH3D*)f->Get("ShowerMap");

  TCanvas *c3 = new TCanvas("c3","Shower Energy Deposited", 1000, 1000);
  c3->Divide(2,2,0,0);
  c3->cd(1);
  hXYE->Draw("colz");
  c3->cd(2);
  hXZE->Draw("colz");
  c3->cd(3);
  hYZE->Draw("colz");
  c3->cd(4);
  hSh->Draw("box");
    
}
