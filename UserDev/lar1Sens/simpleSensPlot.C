{

  int npoints = 500;

  gSystem->Load("liblar1Sens.so");
  lar1::PlotUtils plotUtils;
    
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadBottomMargin(0.08);
  gStyle->SetPadLeftMargin(0.08);
  gStyle->SetPadRightMargin(0.05);

  TH2D * hist = plotUtils.getEmptySensPlot();
  TCanvas * c = new TCanvas("canv","canv",650,650);
  c->SetLogy();
  c->SetLogx();
  // hist->GetXaxis()->SetLabelSize(0);
  // hist->GetYaxis()->SetLabelSize(0);
  // hist->GetXaxis()->SetTitleFont(62);
  // hist->GetYaxis()->SetTitleFont(62);
  // hist->GetXaxis()->SetTickLength(0);
  // hist->GetYaxis()->SetTickLength(0);
  hist->Draw();
  plotUtils.lsnd_plot(c);

  TLegend * leg = plotUtils.getLSNDLegend();
  leg -> Draw();
  double ypos = 0.85;
  plotUtils.add_plot_label((char*)"PRELIMINARY",0.93,ypos,0.025,kRed-3,62,32);
  plotUtils.add_plot_label((char*)"#nu mode, CC Events", 0.93, ypos-0.03, 0.025, 1, 62,32);
  plotUtils.add_plot_label((char*)"Reconstructed Energy", 0.93, ypos-0.06, 0.025, 1, 62,32);
  plotUtils.add_plot_label((char*)"80\% #nu#lower[.6]{#scale[.6]{e}} Efficiency", 0.93, ypos-0.09, 0.025, 1, 62,32);
  plotUtils.add_plot_label((char*)"Statistical and Flux Uncert. Only", 0.93,ypos-0.12, 0.025, 1, 62,32);
  plotUtils.add_plot_label((char*)"5#sigma Confidence Limits", 0.93, ypos-0.15, 0.025, 1, 62,32);



  TString path = "/Users/cja33/Desktop/booster-oscillation-analyses/nue_figures/chi2Files/";

  // std::string fileName = "nue_appearance_ecalo2_dist0_nuND_100m_T600_onaxis_covMat_shapeOnly_chi2.root";
  // std::string fileName = "nue_appearance_ecalo2_dist0_nuND_100m_uB_T600_onaxis_covMat_shapeOnly_chi2.root";
  // std::string fileName = "nue_appearance_ecalo2_dist0_nuND_100m_uB__covMat_shapeOnly_chi2.root";
  // std::string fileName = "nue_appearance_ecalo2_dist0_nuND_150m_T600_onaxis_covMat_shapeOnly_chi2.root";
  // std::string fileName = "nue_appearance_ecalo2_dist0_nuND_150m_uB_T600_onaxis_covMat_shapeOnly_chi2.root";
  // std::string fileName = "nue_appearance_ecalo2_dist0_nuND_150m_uB__covMat_shapeOnly_chi2.root";
  // std::string fileName = "nue_appearance_ecalo2_dist0_nuND_200m_T600_onaxis_covMat_shapeOnly_chi2.root";
  // std::string fileName = "nue_appearance_ecalo2_dist0_nuND_200m_uB_T600_onaxis_covMat_shapeOnly_chi2.root";
  std::string fileName = "nue_appearance_ecalo2_dist0_nuND_200m_uB__covMat_shapeOnly_chi2.root";

  TString pdfName = "sensitivityPlot";


  TLegend * graphLeg = new TLegend(0.8,0.45,0.93,0.57);
  graphLeg->SetFillStyle(0);
  graphLeg->SetFillColor(0);
  graphLeg->SetBorderSize(0);
  graphLeg->SetTextSize(0.025);
  graphLeg->SetTextAlign(32);


  std::vector<std::string> detNames;
  if (fileName.find("ND_100m") != fileName.npos){
    detNames.push_back("LAr1-ND @ 100m");
    pdfName += "_ND_100m";
  }
  if (fileName.find("ND_150m") != fileName.npos){
    detNames.push_back("LAr1-ND @ 150m");
    pdfName += "_ND_150m";
  }
  if (fileName.find("ND_200m") != fileName.npos){
    detNames.push_back("LAr1-ND @ 200m");
    pdfName += "_ND_200m";
  }
  if (fileName.find("uB") != fileName.npos){
    detNames.push_back("MicroBooNE @ 470m");
    pdfName += "_uB";
  }
  if (fileName.find("T600_onaxis") != fileName.npos){
    detNames.push_back("T600 @ 600m");
    pdfName += "_T600_onaxis";
  }

  if (fileName.find("covMat") != fileName.npos){
    pdfName += "_covMat";
  }  
  if (fileName.find("shapeOnly") != fileName.npos){
    pdfName += "_shapeOnly";
  }  


  pdfName+= ".pdf";

  for (int i = 0; i < detNames.size() ;i++){
      plotUtils.add_plot_label(detNames[i].c_str(),  0.93, ypos - (0.20+i*0.03), 0.025, 1, 62,32);
  }


  TString this_File = path;
  this_File += fileName;
  TFile f(this_File);
  TGraph * sens90 = (TGraph*) f.Get("Graph;1");
  TGraph * sens3s = (TGraph*) f.Get("Graph;2");
  TGraph * sens5s = (TGraph*) f.Get("Graph;3");
  sens90 -> SetLineColor(1);
  sens90 -> SetLineStyle(1);
  sens90 -> SetLineWidth(2);
  sens90 -> SetMarkerStyle(29);
  sens90 -> Draw("l same");
  sens3s -> SetLineColor(4);
  sens3s -> SetLineStyle(1);
  sens3s -> SetLineWidth(2);
  sens3s -> SetMarkerStyle(29);
  sens3s -> Draw("l same");
  sens5s -> SetLineColor(4);
  sens5s -> SetLineStyle(2);
  sens5s -> SetLineWidth(2);
  sens5s -> SetMarkerStyle(29);
  sens5s -> Draw("l same");


  graphLeg->AddEntry(sens90,"90\% CL","l");
  graphLeg->AddEntry(sens3s,"3#sigma CL","l");
  graphLeg->AddEntry(sens5s,"5#sigma CL","l");

  graphLeg -> Draw();


  c -> Print(path + pdfName, "pdf");

}






