// #include <vector>
// #include <iostream>

{
  gSystem->Load("lib/liblar1Sens.so");

  lar1::NueAppearanceFitter n;
  n.setFileSource("/media/cadams/data_linux/nominal_ntuples/");
  // n.setFileSource("/media/cadams/data_linux/eff_70/");
  // n.setFileSource("/media/cadams/data_linux/eff_60/");
  // n.setFileSource("/media/cadams/data_linux/eff_50/");

  n.setVerbose(true);
  n.setDebug(false);
  n.setSpecialNameText("");
  n.setSpecialNameTextOsc("");
  n.setSpecialNameText_far("");
  n.setSpecialNameTextOsc_far("");
  n.setMode("nu");


  n.setUse100m(       true);
  n.setUse470m(       true);
  n.setUseT600_onaxis(true);

  n.setLAr1NDPOT((1+1.0)*6.6e20);
  n.setUboonePOT((2+1.0)*6.6e20);
  n.setIcarusPOT((1+1.0)*6.6e20);

  // n.extendFileName("fullCosmics");
  
  n.setLAr1NDScale(1.0*(1.0-0.015));
  n.setUbooneScale(1.0*(1.0-0.02 ));
  n.setIcarusScale(1.0*(1.0-0.03 ));
  n.setLAr1NDCosmicScale(1-0.95);
  n.setUbooneCosmicScale(1-0.95);
  n.setIcarusCosmicScale((1-0.95));
  n.extendFileName("lessCosmics");

  // n.setEnergyType("elep");
  // n.setEnergyType("eccqe");
  n.setEnergyType("ecalo2");
  n.setShapeOnlyFit(false);
  n.setForceRemake(false);
  n.setUseInfiniteStatistics(false);
  // n.setElectContainedDist(100);

  n.setTopologyCut(0.05,3.0);
  // n.setMinVertexEnergySignal(0.05); 

  // n.setCosmicsFile("output/no_cut/histos_for_corey.root");
  // n.setMinDistanceToStart(0);

  std::vector<float> nue_bins;
  // nue_bins.push_back(0.100);
  nue_bins.push_back(0.200);
  nue_bins.push_back(0.350);
  nue_bins.push_back(0.500);
  nue_bins.push_back(0.650);
  nue_bins.push_back(0.800);
  nue_bins.push_back(0.950);
  nue_bins.push_back(1.100);
  nue_bins.push_back(1.300);
  nue_bins.push_back(1.500);
  nue_bins.push_back(1.750);
  nue_bins.push_back(2.000);
  nue_bins.push_back(3.000);
  // double emin = 0.200;
  // double emax = 3.0;
  // double nbins = 120.0 - 8.0;
  // double binSize = (emax - emin)/nbins;
  // nue_bins.resize(nbins+1);
  // for (double i = 0; i <= nbins; i++) {
  //   nue_bins.at(i) = emin + i*binSize;
  // }

  // 
  std::vector<float> numu_bins;
  numu_bins.push_back(.200);
  numu_bins.push_back(.300);
  numu_bins.push_back(.400);
  numu_bins.push_back(.450);
  numu_bins.push_back(.500);
  numu_bins.push_back(.550);
  numu_bins.push_back(.600);
  numu_bins.push_back(.650);
  numu_bins.push_back(.700);
  numu_bins.push_back(.750);
  numu_bins.push_back(.800);
  numu_bins.push_back(.850);
  numu_bins.push_back(.900);
  numu_bins.push_back(.950);
  numu_bins.push_back(1.000);
  numu_bins.push_back(1.250);
  numu_bins.push_back(1.500);
  numu_bins.push_back(2.000);
  numu_bins.push_back(2.500);
  numu_bins.push_back(3.000);
  // std::cout << "Nue bins:\n";
  // for (int i = 0; i < nue_bins.size(); i++) std::cout << nue_bins[i] << "\n";
  // std::cout << "Numu bins:\n";
  // for (int i = 0; i < numu_bins.size(); i++) std::cout << numu_bins[i] << "\n";
  // std::cout << std::endl;

  n.setNueBins(nue_bins);
  n.setNumuBins(numu_bins);

  n.setIncludeFosc(true);
  n.setIncludeNumus(false);
  n.setIncludeCosmics(true);
  n.setIncludeDirt(true);


  n.setInflateSystematics(false);
  n.setSystematicInflationAmount(0.00);


  // Gotta define the backgrounds:
  int kUnisim(0), kPiMinus(1), kPiPlus(2), kKaon0(3), kKMinus(4), kKPlus(5), kTotal_flux(6);
  int kTotal_xsec(0);
  // n.setMultiWeightSource(kUnisim);
  // n.setMultiWeightSource(kPiMinus);
  // n.setMultiWeightSource(kPiPlus);
  // n.setMultiWeightSource(kKaon0);
  // n.setMultiWeightSource(kKMinus);
  // n.setMultiWeightSource(kKPlus);  
  n.setMultiWeightSource(kTotal_xsec);
  n.setAbsolute_MWSource(false);

  n.setUseMultiWeights(false);
  n.setUseXSecWeights(false);
  n.setUseFluxWeights(false);

  n.setNpoints(500);
  n.setNWeights(250);

  n.setSavePlots(true);

  std::vector<std::string> covMatList;
  std::vector<int> covMatListSource;
  covMatList.push_back("xsec");
  covMatListSource.push_back(kTotal_xsec);
  covMatList.push_back("flux");
  covMatListSource.push_back(kTotal_flux);

  n.setIncludeDetSys(false);

  n.setCovMatList(covMatList);
  n.setCovMatListSource(covMatListSource);


  n.setBuildCovarianceMatrix(false);
  n.setMakeRatioPlots(false);
  n.setLoop(true);
  n.setMakeSimplePlot(true);
  n.setMakeEventRatePlots(true);
  n.setMakeAltSensPlot(false);

  n.Run();



  return;
}







