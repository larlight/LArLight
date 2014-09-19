void add_plot_label( char* label, double x, double y, double size = 0.05, int color = 1, int font = 62, int align = 22 ){

  TLatex *latex = new TLatex( x, y, label );
  latex->SetNDC();
  latex->SetTextSize(size);
  latex->SetTextColor(color);
  latex->SetTextFont(font);
  latex->SetTextAlign(align);
  latex->Draw();
}

void plot_matrices(){

  gStyle->SetOptStat(0);
  gStyle->SetPalette(56,0);

  // TFile *f = new TFile("matrixForDave.root");
  TFile *f = new TFile("/Users/cja33/Desktop/booster-oscillation-analyses/nue_figures/matrixFiles/nue_appearance_ecalo2_dist0_matrixFile_6_ND_100m_uB_.root");

  TH1D *nue_near = (TH1D*)f->Get("nueEventRates_100m");
  TH1D *numu_near = (TH1D*)f->Get("numuEventRates_100m");
  TH1D *nue_far = (TH1D*)f->Get("nueEventRates_470m");
  TH1D *numu_far = (TH1D*)f->Get("numuEventRates_470m");

  TH1D *nue_near_error = nue_near->Clone();
  TH1D *numu_near_error = numu_near->Clone();
  TH1D *nue_far_error = nue_far->Clone();
  TH1D *numu_far_error = numu_far->Clone();

  const Int_t Nbins = nue_near->GetNbinsX() + numu_near->GetNbinsX() 
    + nue_far->GetNbinsX() + numu_far->GetNbinsX();

  TH1D *events = new TH1D("events","events",Nbins,0,Nbins);

  for(int i = 1; i <= nue_near->GetNbinsX(); i++)
    events->SetBinContent(i,nue_near->GetBinContent(i));
  for(int i = 1; i <= numu_near->GetNbinsX(); i++)
    events->SetBinContent(i+nue_near->GetNbinsX(),numu_near->GetBinContent(i));
  for(int i = 1; i <= nue_far->GetNbinsX(); i++)
    events->SetBinContent(i+nue_near->GetNbinsX()+numu_near->GetNbinsX(),nue_far->GetBinContent(i));
  for(int i = 1; i <= numu_far->GetNbinsX(); i++)
    events->SetBinContent(i+nue_near->GetNbinsX()+numu_near->GetNbinsX()+nue_near->GetNbinsX(),
			  numu_far->GetBinContent(i));


  TH2D *E = (TH2D*)f->Get("collapsed_covMatHist");
  E->SetTitle("");
  E->GetXaxis()->CenterTitle();
  E->GetXaxis()->SetTitle("LAr1-ND (100m)          T600 (on-axis)");
  E->GetXaxis()->SetTitleSize(0.05);
  E->GetYaxis()->SetTitle("LAr1-ND (100m)     T600 (on-axis)");
  E->GetYaxis()->SetTitleSize(0.05);
  TCanvas *c1 = new TCanvas("c1","c1",650,650);
  c1->SetLogz();
  E->Draw("colz");
  add_plot_label("#nu_{e}",0.24,0.24,0.06);
  add_plot_label("#nu_{#mu}",0.42,0.43,0.06,0);
  add_plot_label("#nu_{e}",0.59,0.62,0.06);
  add_plot_label("#nu_{#mu}",0.77,0.8,0.06,0);
  add_plot_label("Total Covariance Matrix",0.5,0.95,0.04);
 


  TH2D *F = (TH2D*)f->Get("collapsed_fracMatHist");
  F->SetTitle("");
  F->GetXaxis()->CenterTitle();
  F->GetXaxis()->SetTitle("LAr1-ND (100m)          T600 (on-axis)");
  F->GetXaxis()->SetTitleSize(0.05);
  F->GetYaxis()->SetTitle("LAr1-ND (100m)     T600 (on-axis)");
  F->GetYaxis()->SetTitleSize(0.05);
  TCanvas *c2 = new TCanvas("c2","c2",650,650);
  F->SetMinimum(-0.005);
  F->Draw("colz");
  add_plot_label("#nu_{e}",0.24,0.24,0.07);
  add_plot_label("#nu_{#mu}",0.42,0.43,0.07);
  add_plot_label("#nu_{e}",0.59,0.62,0.07);
  add_plot_label("#nu_{#mu}",0.77,0.8,0.07);
  add_plot_label("Fractional Error Matrix",0.5,0.95,0.04);
  add_plot_label("ND",0.2,0.47,0.05,4);
  add_plot_label("FD",0.55,0.85,0.05,4);
  add_plot_label("ND-FD",0.59,0.47,0.05,4);
  TLine *split = new TLine();
  split->SetLineStyle(2);
  split->SetLineWidth(5);
  split->SetLineColor(5);
  split->DrawLineNDC(0.15,0.53,0.85,0.53);
  split->DrawLineNDC(0.51,0.15,0.51,0.91);



  TH2D *R = (TH2D*)f->Get("collapsed_corrMatHist");
  R->SetTitle("");
  R->GetXaxis()->CenterTitle();
  //  R->GetXaxis()->SetTitle("LAr1-ND (100m)       T600 (on-axis)");
  R->GetXaxis()->SetTitleSize(0.045);
  //R->GetYaxis()->SetTitle("LAr1-ND (100m)       T600 (on-axis)  ");
  R->GetYaxis()->SetTitleOffset(0.8);
  R->GetYaxis()->SetTitleSize(0.045);
  R->GetXaxis()->SetLabelSize(0);
  R->GetYaxis()->SetLabelSize(0);
  TCanvas *c3 = new TCanvas("c3","c3",650,650);
  //R->SetMinimum(-0.4);
  R->SetMaximum(1);
  R->DrawCopy("colz");
  add_plot_label("#nu_{e}",0.24,0.24,0.07,0);
  add_plot_label("#nu_{e}-#nu_{#mu}",0.42,0.24,0.07,0);
  add_plot_label("#nu_{e}-#nu_{e}",0.59,0.24,0.07,0);
  add_plot_label("#nu_{#mu}",0.42,0.43,0.07,0);
  add_plot_label("#nu_{#mu}-#nu_{#mu}",0.77,0.43,0.07,0);
  add_plot_label("#nu_{e}",0.59,0.62,0.07,0);
  add_plot_label("#nu_{e}-#nu_{#mu}",0.76,0.62,0.07,0);
  add_plot_label("#nu_{#mu}",0.77,0.8,0.07,0);
  add_plot_label("Flux Correlation Matrix",0.5,0.95,0.04);
  add_plot_label("ND",0.2,0.47,0.05,5);
  add_plot_label("FD",0.55,0.85,0.05,5);
  add_plot_label("ND-FD",0.59,0.47,0.05,5);
  add_plot_label("ND-FD",0.24,0.85,0.05,5);
  add_plot_label(TString::Format("|     %0.1f - %1.0f GeV    |     %0.1f - %1.0f GeV     |     %0.1f - %1.0f GeV    |     %0.1f - %1.0f GeV     |",
				 nue_near->GetBinLowEdge(1),
				 nue_near->GetXaxis()->GetBinUpEdge(nue_near->GetNbinsX()),
				 numu_near->GetBinLowEdge(1),
				 numu_near->GetXaxis()->GetBinUpEdge(numu_near->GetNbinsX()),
				 nue_far->GetBinLowEdge(1),
				 nue_far->GetXaxis()->GetBinUpEdge(nue_near->GetNbinsX()),
				 numu_far->GetBinLowEdge(1),
				 numu_far->GetXaxis()->GetBinUpEdge(numu_near->GetNbinsX())),
		 0.5,0.12,0.025);
 
  split->DrawLineNDC(0.15,0.53,0.85,0.53);
  split->DrawLineNDC(0.5,0.15,0.5,0.91);
 
  TCanvas *c4 = new TCanvas("c4","c4",1300,650);
  c4->Divide(2,1);
  c4->cd(1);
  //R->SetMinimum(-0.4);
  R->SetMaximum(1);
  R->GetXaxis()->SetRange(21,30);
  R->GetYaxis()->SetRange(1,10);
  gStyle->SetPaintTextFormat("1.2f");
  R->SetMarkerSize(1);
  R->Draw("colz");
  add_plot_label("#nu_{e} ND - #nu_{e} FD Correlations",0.5,0.87,0.04,0);
  add_plot_label(TString::Format("|                    0.2         -         3 GeV                   |",
				 nue_near->GetBinLowEdge(1),
				 nue_near->GetXaxis()->GetBinUpEdge(nue_near->GetNbinsX())),
				 0.5,0.06,0.04);
  c4->cd(2);
  R->Draw("text");
  add_plot_label(TString::Format("|                    0.2         -         3 GeV                   |",
				 nue_near->GetBinLowEdge(1),
				 nue_near->GetXaxis()->GetBinUpEdge(nue_near->GetNbinsX())),
				 0.5,0.06,0.04);

  TH1D *diagErrors = events->Clone();
  for( int i = 1; i <= events->GetNbinsX(); i++ ){
    //diagErrors->SetBinContent(i,sqrt(E->GetBinContent(i,i)/(events->GetBinContent(i)*events->GetBinContent(i))));
    cout << i << ": " <<  sqrt(F->GetBinContent(i,i)) << endl;
    diagErrors->SetBinContent(i,sqrt(F->GetBinContent(i,i)));
    if( i <= nue_near->GetNbinsX() ) 
      nue_near_error->SetBinContent(i,sqrt(F->GetBinContent(i,i)));
    if( i > nue_near->GetNbinsX() + numu_near->GetNbinsX() && i <= nue_near->GetNbinsX() + numu_near->GetNbinsX() + nue_far->GetNbinsX() ){
      cout << "i = " << i << endl;
      nue_far_error->SetBinContent(i-(nue_near->GetNbinsX() + numu_near->GetNbinsX()),sqrt(F->GetBinContent(i,i)));
    }
  }
  diagErrors->SetMinimum(0);
  TCanvas *cDiag = new TCanvas("cDiag","cDiag",800,800);
  cDiag->Divide(1,2);
  cDiag->cd(1);
  gPad->SetLogy();
  events->GetYaxis()->SetTitle("Events");
  events->GetYaxis()->SetTitleOffset(0.8);
  events->Draw("H");
  //nue_near->Draw();
  //nue_near_error->Draw();
  cDiag->cd(2);
  diagErrors->SetMinimum(0);
  diagErrors->SetMaximum(0.21);
  diagErrors->SetMarkerStyle(8);
  diagErrors->GetYaxis()->SetTitle("Fractional Error");
  diagErrors->GetYaxis()->SetTitleOffset(0.8);
  diagErrors->Draw("P");
  split->SetLineColor(1);
  split->SetLineWidth(2);
  split->DrawLine(10,0,10,0.205);
  split->DrawLine(20,0,20,0.205);
  split->DrawLine(30,0,30,0.205);
  add_plot_label("#nu_{e} ND",0.24,0.24,0.07,1);
  add_plot_label("#nu_{#mu} ND",0.42,0.24,0.07,1);
  add_plot_label("#nu_{e} FD",0.59,0.24,0.07,1);
  add_plot_label("#nu_{#mu} FD",0.77,0.24,0.07,1);

  TCanvas *cDiag2 = new TCanvas("cDiag2","cDiag2",800,600);
  cDiag2->cd();
  diagErrors->SetMinimum(0);
  diagErrors->SetMaximum(0.21);
  // diagErrors->SetMarkerStyle(15);
  diagErrors->GetYaxis()->SetTitle("Fractional Error");
  diagErrors->GetYaxis()->SetTitleOffset(0.8);
  //diagErrors->Draw("P");
  nue_near_error->SetMinimum(0);
  nue_near_error->SetMaximum(0.18);
  nue_near_error->GetYaxis()->SetTitle("Fractional Uncertainty");
  nue_near_error->GetXaxis()->CenterTitle();
  nue_near_error->GetXaxis()->SetTitle("Neutrino Energy (GeV)");
  nue_near_error->Draw();
  nue_far_error->Draw("same p");
  add_plot_label("Absolute Flux Errors on #nu_{e} Background Predictions",0.5,0.85,0.035,1);
  add_plot_label("ND: points",0.75,0.45,0.04,1);
  add_plot_label("FD: line",0.75,0.37,0.04,1);
  //split->SetLineColor(1);
  //split->SetLineWidth(2);
  //split->DrawLine(10,0,10,0.205);
  //split->DrawLine(20,0,20,0.205);
  //split->DrawLine(30,0,30,0.205);
  //add_plot_label("#nu_{e} ND",0.24,0.24,0.07,1);
  //add_plot_label("#nu_{#mu} ND",0.42,0.24,0.07,1);
  //add_plot_label("#nu_{e} FD",0.59,0.24,0.07,1);
  //add_plot_label("#nu_{#mu} FD",0.77,0.24,0.07,1);


  //  c1->Print("nueErrorMatrix.pdf");
  //c2->Print("nueFracMatrix.pdf");
  // c3->Print("nueCorrMatrix.pdf");
  //c4->Print("nueCorrMatrix_zoom.pdf");
  //cDiag2->Print("fractionalErrors.pdf");
  
  //  c1->Print("nueErrorMatrix.eps");
  //c2->Print("nueFracMatrix.eps");
  //c3->Print("nueCorrMatrix.eps");
  //c4->Print("nueCorrMatrix_zoom.eps");
  //  cDiag2->Print("fractionalErrors.eps");

}

