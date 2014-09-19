

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
return;

  double ypos = 0.85;
  plotUtils.add_plot_label((char*)"PRELIMINARY",0.93,ypos,0.025,kRed-3,62,32);
  plotUtils.add_plot_label((char*)"#nu mode, CC Events", 0.93, ypos-0.03, 0.025, 1, 62,32);
  plotUtils.add_plot_label((char*)"Reconstructed Energy", 0.93, ypos-0.06, 0.025, 1, 62,32);
  plotUtils.add_plot_label((char*)"80\% #nu#lower[.6]{#scale[.6]{e}} Efficiency", 0.93, ypos-0.09, 0.025, 1, 62,32);
  plotUtils.add_plot_label((char*)"Statistical and Flux Uncert. Only", 0.93,ypos-0.12, 0.025, 1, 62,32);
  plotUtils.add_plot_label((char*)"5#sigma Confidence Limits", 0.93, ypos-0.15, 0.025, 1, 62,32);

  /*
  nue_appearance_ecalo1_2ND_uB_FD_cont150_nu_nearDetStats_chi2.root
  nue_appearance_ecalo1_2ND_uB_IC_cont150_nu_nearDetStats_chi2.root
  nue_appearance_ecalo1_2ND_uB_IC_nu_nearDetStats_chi2.root
  nue_appearance_ecalo1_ND_uB_FD_cont150_nu_nearDetStats_chi2.root
  nue_appearance_ecalo1_ND_uB_FD_nu_nearDetStats_chi2.root
  nue_appearance_ecalo1_ND_uB_IC_nu_nearDetStats_chi2.root
   */

  TString path = "/Users/cja33/Desktop/booster-oscillation-analyses/nue_figures/chi2Files/";
  // TString path = "/uboone/app/users/cadams/reprocessing/LArLight/UserDev/lar1Sens/";

  std::vector<std::string> files;
  // files.push_back("files/nue_appearance_ecalo2_nuND_100m_T600_onaxis_covMat_shapeOnly_chi2.root");
  // files.push_back("files/nue_appearance_ecalo2_nuND_150m_T600_onaxis_covMat_shapeOnly_chi2.root");
  // files.push_back("files/nue_appearance_ecalo2_nuND_200m_T600_onaxis_covMat_shapeOnly_chi2.root");
  // files.push_back("files/nue_appearance_ecalo2_covMat_ND_100mT600_offaxisnu_flatStats_shapeOnly_chi2.root");
  // files.push_back("files/nue_appearance_ecalo2_covMat_ND_150mT600_offaxisnu_flatStats_shapeOnly_chi2.root");
  // files.push_back("files/nue_appearance_ecalo2_covMat_ND_200mT600_offaxisnu_flatStats_shapeOnly_chi2.root");

  // files.push_back("equalStats_chi2/nue_appearance_ecalo2_nuND_100m_T600_onaxis_flatStats_shapeOnly_chi2.root");
  // files.push_back("equalStats_chi2/nue_appearance_ecalo2_nuND_150m_T600_onaxis_flatStats_shapeOnly_chi2.root");
  // files.push_back("equalStats_chi2/nue_appearance_ecalo2_nuND_200m_T600_onaxis_flatStats_shapeOnly_chi2.root");

  files.push_back("nue_appearance_ecalo2_dist0_nuND_100m_T600_onaxis_covMat_shapeOnly_chi2.root");
  // files.push_back("equalStats_scaleNDonly_chi2/nue_appearance_ecalo2_nuND_150m_T600_onaxis_flatStats_shapeOnly_chi2.root");
  // files.push_back("equalStats_scaleNDonly_chi2/nue_appearance_ecalo2_nuND_200m_T600_onaxis_flatStats_shapeOnly_chi2.root");


  // files.push_back("files/nue_appearance_ecalo2_nuND_100m_T600_onaxis_flatStats_shapeOnly_chi2.root");
  // files.push_back("files/nue_appearance_ecalo2_nuND_150m_T600_onaxis_flatStats_shapeOnly_chi2.root");
  // files.push_back("files/nue_appearance_ecalo2_nuND_200m_T600_onaxis_flatStats_shapeOnly_chi2.root");

  std::vector<std::string> names90;
  names90.push_back(" ND @ 100m, T600 @ 600m");
  names90.push_back(" ND @ 150m, T600 @ 600m");
  names90.push_back(" ND @ 200m, T600 @ 600m");
  names90.push_back(" ND @ 100m, T600 @ 600m");
  names90.push_back(" ND @ 150m, T600 @ 600m");
  names90.push_back(" ND @ 200m, T600 @ 600m");

  std::vector<std::string> names3s;
  names3s.push_back("3#sigma limit, ND @ 100m, T600 @ 600m, on axis");
  names3s.push_back("3#sigma limit, ND @ 150m, T600 @ 600m, on axis");
  names3s.push_back("3#sigma limit, ND @ 200m, T600 @ 600m, on axis");
  names3s.push_back("3#sigma limit, ND @ 100m, T600 @ 600m, off axis");
  names3s.push_back("3#sigma limit, ND @ 150m, T600 @ 600m, off axis");
  names3s.push_back("3#sigma limit, ND @ 200m, T600 @ 600m, off axis");
  

  std::vector<std::string> names5s;
  names5s.push_back(" ND @ 100m, T600 @ 600m");
  names5s.push_back(" ND @ 150m, T600 @ 600m");
  names5s.push_back(" ND @ 200m, T600 @ 600m");
  names5s.push_back(" ND @ 100m, T600 @ 600m");
  names5s.push_back(" ND @ 150m, T600 @ 600m");
  names5s.push_back(" ND @ 200m, T600 @ 600m");


  std::vector<int> index;
  index.push_back(0);
  // index.push_back(1);
  // index.push_back(2);
  // index.push_back(3);
  // index.push_back(4);
  // index.push_back(5);
  // index.push_back(6);
  // index.push_back(7);
  // index.push_back(8);
  // index.push_back(9);
  // index.push_back(10);
  // index.push_back(11);
  // index.push_back(12);




  std::vector<int> colors;
  colors.push_back(1);
  colors.push_back(2);
  colors.push_back(4);
  colors.push_back(1);
  colors.push_back(2);
  colors.push_back(4);
  colors.push_back(1);
  colors.push_back(2);
  colors.push_back(4);
  colors.push_back(8);
  colors.push_back(11);
  colors.push_back(28);
  colors.push_back(1);
  colors.push_back(8);
  colors.push_back(4);
  colors.push_back(28);

  TLegend * graphLeg = new TLegend(.54,.55,.93,.65);
  graphLeg->SetFillStyle(0);
  graphLeg->SetFillColor(0);
  graphLeg->SetBorderSize(0);
  graphLeg->SetTextSize(0.025);
  graphLeg->SetTextAlign(32);

  std::vector<TGraph *> graphsForRatio;


  for (auto j = index.begin(); j != index.end(); j++){
    int i = *j;
    TString this_File = path;
    this_File += files[i];
    TFile f(this_File);
    TGraph * sens90 = (TGraph*) f.Get("Graph;1");
    graphsForRatio.push_back(sens90);
    sens90 -> SetLineColor(colors[i]);
    sens90 -> SetLineStyle(1);
    sens90 -> SetLineWidth(2);
    sens90 -> SetMarkerStyle(29);
    sens90 -> Draw("l same");
    graphLeg->AddEntry(sens90,names5s[i].c_str(),"l");
    // graphLeg->AddEntry(sens90,"Stupid");
  }

  graphLeg -> Draw();

}






