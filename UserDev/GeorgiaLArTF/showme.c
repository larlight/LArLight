void showme(){

  stylize();
  
  TFile *f = new TFile("timing_output.root","READ");

  TCanvas *c = new TCanvas("c","Trigger Types",600,600);
  c->Divide(2,2);
  TCanvas *d = new TCanvas("d","Simple Hists",800,600);
  d->Divide(3,2);
  TCanvas *e = new TCanvas("e","Channel Read Info",600,600);
  e->Divide(2,2);
  TCanvas *j = new TCanvas("j","Simple Hists - Trig",800,600);
  j->Divide(3,2);
  TCanvas *k = new TCanvas("k","Key Timing Info",800,600);
  k->Divide(3,2);

  TGraph *gr1[100]; 
  TGraph *gr[100];
  TLegend *leg[100];


  k->cd(1);
  gr1[3] = (TGraph*) f->Get("key_fem_trig_trfrm_vs_trfrm");
  gr1[3]->GetXaxis()->SetTitle("TRIG Frame Number");
  gr1[3]->GetYaxis()->SetTitle("FEM Trig Frame Number");
  color(gr1[3],8); gr1[3]->Draw("AP");

  gr1[4] = (TGraph*) f->Get("key_fem_trig_evfrm_vs_trfrm");
  gr1[4]->GetXaxis()->SetTitle("TRIG Frame Number");
  gr1[4]->GetYaxis()->SetTitle("FEM Event Frame Number");
  color(gr1[4],22); gr1[4]->Draw("P same");
  leg[2] = new TLegend(0.2,0.7,0.6,0.85);
  leg[2]->SetFillColor(0);
  leg[2]->SetBorderSize(0);
  leg[2]->AddEntry(gr1[3],"FEM Trig Frame Number","p");
  leg[2]->AddEntry(gr1[4],"FEM Event Frame Number","p");
  leg[2]->Draw();

  k->cd(2);
  gr1[5] = (TGraph*) f->Get("key_fem_trig_trfrmdiff_vs_evtno");
  gr1[5]->GetXaxis()->SetTitle("Event number");
  gr1[5]->GetYaxis()->SetTitle("Trigger Frame Diff (FEM-Trig)");
  color(gr1[5],8); gr1[5]->Draw("AP");

  k->cd(3);
  gr1[6] = (TGraph*) f->Get("key_fem_trig_trfrmdiff_vs_trfrm");
  gr1[6]->GetXaxis()->SetTitle("Trig Frame number");
  gr1[6]->GetYaxis()->SetTitle("Trigger Frame Diff (FEM-Trig)");
  color(gr1[6],8); gr1[6]->Draw("AP");

  k->cd(4);
  gr1[7] = (TGraph*) f->Get("KEY_FEM_evtfrm_vs_trfrm");
  gr1[7]->GetXaxis()->SetTitle("Trig Frame number");
  gr1[7]->GetYaxis()->SetTitle("Event Frame");
  color(gr1[7],8); gr1[7]->Draw("AP");



  TH1D *h8 = (TH1D*) f->Get("pmt_nu_ch");
  e->cd(1); colorh(h8,9); h8->Draw();

  TH1D *h9 = (TH1D*) f->Get("pmt_nu_nrd");
  e->cd(2); colorh(h9,9); h9->Draw();

  TH2D *h10 = (TH2D*) f->Get("KEY_discID_vs_ch");
  e->cd(3); h10->Draw("colz");



  TH1D *h1 = (TH1D*) f->Get("pmt_nu_evno");
  d->cd(1); colorh(h1,9); h1->Draw();
  TH1D *ht1 = (TH1D*) f->Get("trig_trid");
  colorh(ht1,8); ht1->Draw("same p");

  TH1D *h2 = (TH1D*) f->Get("pmt_nu_evfrm");
  d->cd(2); colorh(h2,9); h2->Draw();
  TH1D *ht2 = (TH1D*) f->Get("trig_trfrm");
  colorh(ht2,8); ht2->Draw("same p");

  TH1D *h3 = (TH1D*) f->Get("pmt_nu_add");
  TH1D *h4 = (TH1D*) f->Get("pmt_nu_mod");
  d->cd(4); 
  colorh(h3,9); colorh(h4,9);
  h3->Draw(); h4->Draw("same");

  TH1D *h5 = (TH1D*) f->Get("pmt_nu_trfrm");
  d->cd(3); colorh(h5,9); h5->Draw();
  ht2->Draw("same p");

  TH1D *h7 = (TH1D*) f->Get("pmt_nu_trslc");
  d->cd(6); colorh(h7,9); h7->Draw();
  TH1D *ht7 = (TH1D*) f->Get("trig_trslc");
  colorh(ht7,8); ht7->Draw();

  TH1D *ht8 = (TH1D*) f->Get("trig_actv");
  j->cd(1); colorh(ht8,8); ht8->Draw();
  
  TH1D *ht9 = (TH1D*) f->Get("trig_veto");
  j->cd(2); colorh(ht9,8); ht9->Draw();

  TH1D *ht10 = (TH1D*) f->Get("trig_rem64");
  j->cd(3); colorh(ht10,8); ht10->Draw();

  TH1D *ht11 = (TH1D*) f->Get("trig_rem16");
  j->cd(3); colorh(ht11,8); ht11->Draw();



  /////
  //Trig types
  gr[0] = (TGraph*) f->Get("key_trig_type_vs_evtno");
  c->cd(1); 
  gr[0]->GetXaxis()->SetTitle("Event number (FEM=TRIG)");
  gr[0]->GetYaxis()->SetTitle("Trigger Type");
  color(gr[0],8); gr[0]->Draw("AP");
  leg[0] = new TLegend(0.4,0.5,0.8,0.8);
  leg[0]->SetFillColor(0);
  leg[0]->SetBorderSize(0);
  leg[0]->AddEntry("","1 = Gate 1 (NuMI)","");
  leg[0]->AddEntry("","2 = Gate 2 (BNB)","");
  leg[0]->AddEntry("","3 = PMT (any)","");
  leg[0]->AddEntry("","4 = PC Trig","");
  leg[0]->AddEntry("","5 = Calib","");
  leg[0]->AddEntry("","6 = EXT","");
  leg[0]->Draw();
  //----
  gr[1] = (TGraph*) f->Get("key_trig_type_vs_trfrm");
  c->cd(2);
  gr[1]->GetXaxis()->SetTitle("FEM Trig Frame number");
  gr[1]->GetYaxis()->SetTitle("Trigger Type");
  color(gr[1],8); gr[1]->Draw("AP");
  leg[1] = new TLegend(0.4,0.5,0.8,0.8);
  leg[1]->SetFillColor(0);
  leg[1]->SetBorderSize(0);
  leg[1]->AddEntry("","1 = Gate 1 (NuMI)","");
  leg[1]->AddEntry("","2 = Gate 2 (BNB)","");
  leg[1]->AddEntry("","3 = PMT (any)","");
  leg[1]->AddEntry("","4 = PC Trig","");
  leg[1]->AddEntry("","5 = Calib","");
  leg[1]->AddEntry("","6 = EXT","");
  leg[1]->Draw();
  //----
  gr[2] = (TGraph*) f->Get("key_ntrigs_vs_evtno");
  c->cd(3);
  gr[2]->GetXaxis()->SetTitle("Event number (FEM=TRIG)");
  gr[2]->GetYaxis()->SetTitle("Number of triggers");
  color(gr[2],8); gr[2]->Draw("AP");
  
}

void color(TGraph *gr,Int_t color){

  gr->SetMarkerStyle(33);
  gr->SetMarkerSize(0.8);
  gr->SetMarkerColor(color);

}

void colorh(TH1D *h,Int_t color){
  
  h->SetLineWidth(1);
  h->SetLineColor(color);
  h->SetFillColor(color);
  h->SetFillStyle(3004);
		  
}

void setup(TLegend leg){

  leg = new TLegend(0.5,0.5,0.7,0.7);
  leg.SetFillColor(0);
  leg.SetBorderSize(0);

}

void stylize(){

  gStyle->SetOptStat(0000);
  gStyle->SetOptFit(0000);
  gStyle->SetOptTitle(1);
  gStyle->SetPadColor(0);
  gStyle->SetCanvasColor(0);
  gStyle->SetStatColor(0);
  gStyle->SetTitleColor(1);
  gStyle->SetPadBorderMode(0);
  gStyle->SetFrameBorderMode(0);
  gStyle->SetCanvasBorderMode(0);

  gStyle->SetPadBottomMargin(0.16);
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetPadLeftMargin(0.18);
  gStyle->SetPadRightMargin(0.15);
  gStyle->SetTextSize(0.15);
  gStyle->SetTitleSize(0.08);
  gStyle->SetTitleSize(0.06,"X");
  gStyle->SetTitleSize(0.06,"Y");
  gStyle->SetTitleOffset(1.3,"Y");
  gStyle->SetLabelSize(0.04,"X");
  gStyle->SetLabelSize(0.04,"Y");

  gStyle->SetPalette(51);

}
