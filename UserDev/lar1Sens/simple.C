// #include <vector>
// #include <iostream>

{
  gSystem->Load("lib/liblar1Sens.so");
  lar1::NueAppearanceFitter n;
  n.setFileSource("/Users/cja33/genie_RW/");
  // n.setFileSource("/Users/cja33/nominal_ntuples/");
  n.setVerbose(true);
  n.setDebug(false);
  n.setSpecialNameText("");
  n.setSpecialNameTextOsc("");
  n.setSpecialNameText_far("");
  n.setSpecialNameTextOsc_far("");
  n.setFlatSystematicError(0.20);
  n.setMode("nu");
  n.setUse100m(true);
  n.setUse470m(true);
  n.setUseT600_onaxis(false);
  n.setUbooneScale(1.0);
  // n.setLAr1NDScale(1.0);
  n.setLAr1NDScale(1.0);
  n.setLAr1FDScale(1.0);
  n.setEnergyType("ecalo2");
  n.setUseNearDetStats(false);
  n.setShapeOnlyFit(true);
  n.setNearDetSystematicError(0.2);
  n.setForceRemake(false);
  n.setUseInfiniteStatistics(false);
  // n.setElectContainedDist(100);

  n.setTopologyCut(0.15,1.5);

  // n.setIncludeCosmics(true);
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

  n.setIncludeFosc(false);
  n.setIncludeNumus(false);

  n.setInflateSystematics(false);
  n.setSystematicInflationAmount(0.00);

  n.setUseCovarianceMatrix(true);
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

  n.setUseXSecWeights(true);
  n.setUseFluxWeights(false);

  n.setNpoints(500);
  n.setNWeights(250);

  n.setSavePlots(true);

  n.setBuildCovarianceMatrix(true);
  n.setMakeRatioPlots(true);
  n.setLoop(false);
  n.setMakePlots(false);
  n.setMakeSimplePlot(false);
  n.setMakeEventRatePlots(true);
  n.setMakeAltSensPlot(false);

  n.Run();

  return;
}





