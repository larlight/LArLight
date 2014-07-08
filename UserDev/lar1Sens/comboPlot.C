

{

  gSystem->Load("liblar1Sens.so");
  lar1::PlotUtils plotUtils;

  TH2D * hist = plotUtils.getEmptySensPlot();
  TCanvas * c = new TCanvas("canv","canv",800,800);
  c->SetLogy();
  c->SetLogx();
  hist->Draw();
  plotUtils.lsnd_plot(c);

  TLegend * leg = plotUtils.getLSNDLegend();
  leg -> Draw();

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
  // files.push_back("nue_appearance_ecalo1_2ND_uB_FD_cont150_nu_nearDetStats_chi2.root");
  // files.push_back("nue_appearance_ecalo1_2ND_uB_IC_cont150_nu_nearDetStats_chi2.root");
  // files.push_back("nue_appearance_ecalo1_2ND_uB_IC_nu_nearDetStats_chi2.root");
  // files.push_back("nue_appearance_ecalo1_ND_uB_FD_cont150_nu_nearDetStats_chi2.root");
  // files.push_back("nue_appearance_ecalo1_ND_uB_FD_nu_nearDetStats_chi2.root");
  // files.push_back("nue_appearance_ecalo1_ND_uB_IC_nu_nearDetStats_chi2.root");
  // files.push_back("nue_appearance_ecalo1_2ND_uB_nu_nearDetStats_chi2.root");
  // files.push_back("nue_appearance_ecalo1_2ND_uB_cont150_nu_nearDetStats_chi2.root");
  // files.push_back("nue_appearance_ecalo1_ND_uB_nu_nearDetStats_chi2.root");
  // files.push_back("nue_appearance_ecalo1_ND_uB_cont150_nu_nearDetStats_chi2.root");
  // files.push_back("nue_appearance_ecalo1_ND_IC_nu_nearDetStats_chi2.root");
  // files.push_back("nue_appearance_ecalo1_uB_IC_nu_nearDetStats_chi2.root");
  // files.push_back("nue_appearance_ecalo1_uB_nu_flatStats_chi2.root");
  files.push_back("files/nue_appearance_ecalo2_covMat_ND_100mT600_onaxisnu_flatStats_chi2.root");
  files.push_back("files/nue_appearance_ecalo2_covMat_ND_150mT600_onaxisnu_flatStats_chi2.root");
  files.push_back("files/nue_appearance_ecalo2_covMat_ND_200mT600_onaxisnu_flatStats_chi2.root");

  std::vector<std::string> names90;
  names90.push_back("90\% limit, ND @ 100m, T600 @ 600m onaxis");
  names90.push_back("90\% limit, ND @ 150m, T600 @ 600m onaxis");
  names90.push_back("90\% limit, ND @ 200m, T600 @ 600m onaxis");
  // names90.push_back("2*ND, uB, FD contained 90\% limit");  // index.push_back(0);
  // names90.push_back("2*ND, uB, IC contained 90\% limit");  // index.push_back(1);
  // names90.push_back("2*ND, uB, IC 90\% limit");            // index.push_back(2);
  // names90.push_back("ND, uB, FD contained 90\% limit");    // index.push_back(3);
  // names90.push_back("ND, uB, FD 90\% limit");              // index.push_back(4);
  // names90.push_back("ND, uB, IC 90\% limit");              // index.push_back(5);
  // names90.push_back("2*ND, uB 90\% limit");                // index.push_back(6);
  // names90.push_back("2*ND, uB contained 90\% limit");      // index.push_back(7);
  // names90.push_back("ND, uB 90\% limit");                  // index.push_back(8);
  // names90.push_back("ND, uB contained 90\% limit");        // index.push_back(9);
  // names90.push_back("ND, IC 90\% limit");                  // index.push_back(10);
  // names90.push_back("uB, IC 90\% limit");                  // index.push_back(11);
  // names90.push_back("uB 90\% limit");                      // index.push_back(12);

  std::vector<std::string> names3s;
  names3s.push_back("3#sigma limit, ND @ 100m, T600 @ 600m onaxis");
  names3s.push_back("3#sigma limit, ND @ 150m, T600 @ 600m onaxis");
  names3s.push_back("3#sigma limit, ND @ 200m, T600 @ 600m onaxis");
  names3s.push_back("2*ND, uB, FD contained 3#sigma limit");  // index.push_back(0);
  names3s.push_back("2*ND, uB, IC contained 3#sigma limit");  // index.push_back(1);
  names3s.push_back("2*ND, uB, IC 3#sigma limit");            // index.push_back(2);
  names3s.push_back("ND, uB, FD contained 3#sigma limit");    // index.push_back(3);
  names3s.push_back("ND, uB, FD 3#sigma limit");              // index.push_back(4);
  names3s.push_back("ND, uB, IC 3#sigma limit");              // index.push_back(5);
  names3s.push_back("2*ND, uB 3#sigma limit");                // index.push_back(6);
  names3s.push_back("2*ND, uB contained 3#sigma limit");      // index.push_back(7);
  names3s.push_back("ND, uB 3#sigma limit");                  // index.push_back(8);
  names3s.push_back("ND, uB contained 3#sigma limit");        // index.push_back(9);
  names3s.push_back("ND, IC 3#sigma limit");                  // index.push_back(10);
  names3s.push_back("uB, IC 3#sigma limit");                  // index.push_back(11);
  names3s.push_back("uB 3#sigma limit");                      // index.push_back(12);

  std::vector<std::string> names5s;
  names5s.push_back("5#sigma limit, ND @ 100m, T600 @ 600m onaxis");
  names5s.push_back("5#sigma limit, ND @ 150m, T600 @ 600m onaxis");
  names5s.push_back("5#sigma limit, ND @ 200m, T600 @ 600m onaxis");
  names5s.push_back("2*ND, uB, FD contained 5#sigma limit");  // index.push_back(0);
  names5s.push_back("2*ND, uB, IC contained 5#sigma limit");  // index.push_back(1);
  names5s.push_back("2*ND, uB, IC 5#sigma limit");            // index.push_back(2);
  names5s.push_back("ND, uB, FD contained 5#sigma limit");    // index.push_back(3);
  names5s.push_back("ND, uB, FD 5#sigma limit");              // index.push_back(4);
  names5s.push_back("ND, uB, IC 5#sigma limit");              // index.push_back(5);
  names5s.push_back("2*ND, uB 5#sigma limit");                // index.push_back(6);
  names5s.push_back("2*ND, uB contained 5#sigma limit");      // index.push_back(7);
  names5s.push_back("ND, uB 5#sigma limit");                  // index.push_back(8);
  names5s.push_back("ND, uB contained 5#sigma limit");        // index.push_back(9);
  names5s.push_back("ND, IC 5#sigma limit");                  // index.push_back(10);
  names5s.push_back("uB, IC 5#sigma limit");                  // index.push_back(11);
  names5s.push_back("uB 5#sigma limit");                      // index.push_back(12);


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
  colors.push_back(8);
  colors.push_back(11);
  colors.push_back(28);
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

  TLegend * graphLeg = new TLegend(.55,.75,.7,.85);
  graphLeg->SetFillStyle(0);
  graphLeg->SetFillColor(0);
  graphLeg->SetBorderSize(0);
  graphLeg->SetTextSize(0.02);

  for (auto j = index.begin(); j != index.end(); j++){
    int i = *j;
    TString this_File = path;
    this_File += files[i];
    TFile f(this_File);
    TGraph * sens90 = (TGraph*) f.Get("Graph;3");
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






