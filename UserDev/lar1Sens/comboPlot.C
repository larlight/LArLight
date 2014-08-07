

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
  hist->GetXaxis()->SetLabelSize(0);
  hist->GetYaxis()->SetLabelSize(0);
  hist->GetXaxis()->SetTitleFont(62);
  hist->GetYaxis()->SetTitleFont(62);
  hist->GetXaxis()->SetTickLength(0);
  hist->GetYaxis()->SetTickLength(0);
  hist->Draw();
  // plotUtils.lsnd_plot(c);

  // TLegend * leg = plotUtils.getLSNDLegend();
  // leg -> Draw();
  double ypos = 0.85;
  plotUtils.add_plot_label((char*)"PRELIMINARY",0.93,ypos,0.025,kRed-3,62,32);
  plotUtils.add_plot_label((char*)"#nu mode, CC Events", 0.93, ypos-0.03, 0.025, 1, 62,32);
  plotUtils.add_plot_label((char*)"Reconstructed Energy", 0.93, ypos-0.06, 0.025, 1, 62,32);
  plotUtils.add_plot_label((char*)"80\% #nu_{e} Efficiency", 0.93, ypos-0.09, 0.025, 1, 62,32);
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

  TString path = "/uboone/app/users/cadams/reprocessing/LArLight/UserDev/lar1Sens/";

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

  files.push_back("equalStats_scaleNDonly_chi2/nue_appearance_ecalo2_nuND_100m_T600_onaxis_flatStats_shapeOnly_chi2.root");
  files.push_back("equalStats_scaleNDonly_chi2/nue_appearance_ecalo2_nuND_150m_T600_onaxis_flatStats_shapeOnly_chi2.root");
  files.push_back("equalStats_scaleNDonly_chi2/nue_appearance_ecalo2_nuND_200m_T600_onaxis_flatStats_shapeOnly_chi2.root");


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
  index.push_back(1);
  index.push_back(2);
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

  TGraph * graph_100 = graphsForRatio.front();
  TGraph * graph_150 = graphsForRatio[1];
  TGraph * graph_200 = graphsForRatio[2];
  TGraph *graph_100_R = new TGraph(npoints+1);
  TGraph *graph_150_R = new TGraph(npoints+1);
  TGraph *graph_200_R = new TGraph(npoints+1);
  
  graph_100_R->SetLineColor(kBlack);
  graph_100_R->SetLineStyle(1);
  graph_100_R->SetLineWidth(2);

  graph_150_R->SetLineColor(kRed-3);
  graph_150_R->SetLineStyle(1);
  graph_150_R->SetLineWidth(2);

  graph_200_R->SetLineColor(kBlue-3);
  graph_200_R->SetLineStyle(1);
  graph_200_R->SetLineWidth(2);

  // Also make the ratio plots
  // 
  double x_100 = 0, y_100 = 0;
  double x_150 = 0, y_150 = 0;
  double x_200 = 0, y_200 = 0;
  for(int i = 0; i <= npoints; i++){
  
      graph_100->GetPoint(i,x_100,y_100);
      graph_150->GetPoint(i,x_150,y_150);
      graph_200->GetPoint(i,x_200,y_200);

      graph_100_R->SetPoint(i, (y_100), (x_100-x_100)/x_100 );
      graph_150_R->SetPoint(i, (y_150), (x_100-x_150)/x_100 );
      graph_200_R->SetPoint(i, (y_200), (x_100-x_200)/x_100 );
      
  }

  TCanvas *c3 = new TCanvas("c3", "Sensitivity",616,22,700,700);
  gStyle->SetOptStat(0);
  c3->SetFillColor(0);
  c3->SetBorderMode(0);
  c3->SetBorderSize(2);
  c3->SetLogx();
  // c3->SetLogy();
  c3->SetLeftMargin(0.15);
  c3->SetRightMargin(0.05);
  c3->SetTopMargin(0.05);
  c3->SetBottomMargin(0.15);
  c3->SetFrameBorderMode(0);
  c3->SetFrameBorderMode(0);

    TH2D *hr1__1 = new TH2D("hr1__1","",500,0.01,100,500,-5.0,5);
    hr1__1->SetDirectory(0);
    hr1__1->SetStats(0);
    
    hr1__1->GetYaxis()->SetTitle("Ratio of 5#sigma C.L. Limits");
    // hr1__1->GetYaxis()->SetTitle("Ratio of 90% C.L. Limits");
    hr1__1->GetYaxis()->CenterTitle(true);
    hr1__1->GetYaxis()->SetLabelFont(62);
    hr1__1->GetYaxis()->SetLabelOffset(0.002);
    hr1__1->GetYaxis()->SetLabelSize(0.04);
    hr1__1->GetYaxis()->SetTitleSize(0.05);
    hr1__1->GetYaxis()->SetTitleFont(62);
    hr1__1->GetYaxis()->SetTitleOffset(1.2);


    hr1__1->GetXaxis()->SetTitle("#Deltam_{41}^{2} [eV^{2}]");
    hr1__1->GetXaxis()->CenterTitle(true);
    hr1__1->GetXaxis()->SetLabelFont(62);
    hr1__1->GetXaxis()->SetLabelSize(0.04);
    hr1__1->GetXaxis()->SetTitleSize(0.05);
    hr1__1->GetXaxis()->SetTitleFont(62);
    hr1__1->GetXaxis()->SetTitleOffset(1.2);
    
    hr1__1->Draw("");
    graph_100_R->Draw("same");
    graph_150_R->Draw("same");
    graph_200_R->Draw("same");

    TLatex *tex_pre = new TLatex(.2,.72,"PRELIMINARY");
    tex_pre->SetNDC();
    tex_pre->SetTextFont(62);
    tex_pre->SetTextColor(kRed-3);
    tex_pre->SetTextSize(0.03);
    tex_pre->Draw();

    TLatex *tex_numu = new TLatex(.2,.92,"LAr1-ND and T600 (600 m, on axis)  #nu#lower[0.4]{e} Appearance Sensitivity");
    tex_numu->SetNDC();
//    tex_numu->SetTextColor(kRed-3);
    tex_numu->SetTextFont(62);
    tex_numu->SetTextSize(0.025);
    tex_numu->Draw();
    
    TLatex *tex_mode = new TLatex(.2,.87,"#nu mode, CC Events");
    tex_mode->SetNDC();
    tex_mode->SetTextFont(62);
    tex_mode->SetTextSize(0.025);
    tex_mode->Draw();
    
    TLatex *tex_un = new TLatex(.2,.835,"Statistical and Flux Uncert. Only");
    tex_un->SetNDC();
    tex_un->SetTextFont(62);
    tex_un->SetTextSize(0.025);
    tex_un->Draw();
    
    TLatex *tex_E = new TLatex(.2,.8025,"Reconstructed Energy");
    tex_E->SetNDC();
    tex_E->SetTextFont(62);
    tex_E->SetTextSize(0.025);
    tex_E->Draw();
    
    TLatex *tex_eff = new TLatex(.2,.77,"80% #nu#lower[0.4]{e} Efficiency");
    tex_eff->SetNDC();
    tex_eff->SetTextFont(62);
    tex_eff->SetTextSize(0.025);
    tex_eff->Draw();

    
    TLegend *leg = new TLegend(0.65,0.75,0.8,0.9,NULL,"brNDC");
    leg->SetBorderSize(0);
    leg->SetTextSize(0.025);
    leg->SetTextFont(62);
    leg->SetTextAlign(12);
    leg->SetLineColor(1);
    leg->SetLineStyle(1);
    leg->SetLineWidth(1);
    leg->SetFillColor(0);
    leg->SetFillStyle(0);
    TLine *gdummy3 = new TLine();
    gdummy3->SetLineColor(kBlack);
    gdummy3->SetLineStyle(1);
    gdummy3->SetLineWidth(2);
    TLegendEntry *entry=leg->AddEntry(gdummy3,"Ratio 100m to 100m","l");
    TLine *gdummy3 = new TLine();
    gdummy3->SetLineColor(kRed-3);
    gdummy3->SetLineStyle(1);
    gdummy3->SetLineWidth(2);
    TLegendEntry *entry=leg->AddEntry(gdummy3,"Ratio 150m to 100m","l");
    TLine *gdummy3 = new TLine();
    gdummy3->SetLineColor(kBlue-3);
    gdummy3->SetLineStyle(1);
    gdummy3->SetLineWidth(2);
    TLegendEntry *entry=leg->AddEntry(gdummy3,"Ratio 200m to 100m","l");
    
    leg->Draw();
    c3->Modified();
    c3->cd();
    c3->SetSelected(c3);
}






