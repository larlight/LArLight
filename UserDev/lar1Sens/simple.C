// #include <vector>+

{
  gSystem->Load("lib/liblar1Sens.so");
  lar1::NueAppearanceFitter n;
  n.setFileSource("/Users/cja33/lar1_data/");
  n.setVerbose(true);
  n.setDebug(false);
  n.setSpecialNameText("");
  n.setSpecialNameTextOsc("");
  n.setSpecialNameText_far("");
  n.setSpecialNameTextOsc_far("");
  n.setFlatSystematicError(0.00);
  n.setMode("nu");
  n.setUse100m(false);
  n.setUse150m(false);
  n.setUse200m(false);
  n.setUse100mLong(false);
  n.setUse470m(true);
  n.setUse700m(false);
  n.setUseT600_onaxis(false);
  n.setUseT600_offaxis(false);
  n.setUbooneScale(1.0);
  n.setLAr1NDScale(1.0/(100*1.65*1.65));
  // n.setLAr1NDScale(1.0/3.0);
  n.setLAr1FDScale(1.0);
  n.setEnergyType("ecalo2");
  n.setUseNearDetStats(false);
  n.setShapeOnlyFit(true);
  n.setNearDetSystematicError(0.0);
  n.setForceRemake(false);
  n.setUseInfiniteStatistics(false);
  // n.setElectContainedDist(150);

  std::vector<float> miniboone_bins;
  miniboone_bins.push_back(0.100);
  miniboone_bins.push_back(0.200);
  miniboone_bins.push_back(0.300);
  miniboone_bins.push_back(0.375);
  miniboone_bins.push_back(0.475);
  miniboone_bins.push_back(0.550);
  miniboone_bins.push_back(0.675);
  miniboone_bins.push_back(0.800);
  miniboone_bins.push_back(0.950);
  miniboone_bins.push_back(1.100);
  miniboone_bins.push_back(1.300);
  miniboone_bins.push_back(1.500);
  miniboone_bins.push_back(3.000);
  // miniboone_bins.push_back(0.200);
  // miniboone_bins.push_back(0.480);
  // miniboone_bins.push_back(0.760);
  // miniboone_bins.push_back(1.040);
  // miniboone_bins.push_back(1.320);
  // miniboone_bins.push_back(1.600);
  // miniboone_bins.push_back(1.880);
  // miniboone_bins.push_back(2.160);
  // miniboone_bins.push_back(2.440);
  // miniboone_bins.push_back(2.720);
  // miniboone_bins.push_back(3.000);
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
  std::cout << "Nue bins:\n";
  for (int i = 0; i < miniboone_bins.size(); i++) std::cout << miniboone_bins[i] << "\n";
  std::cout << "Numu bins:\n";
  for (int i = 0; i < numu_bins.size(); i++) std::cout << numu_bins[i] << "\n";
  std::cout << std::endl;

  n.setNueBins(miniboone_bins);
  n.setNumuBins(numu_bins);

  n.setInflateSystematics(false);
  n.setSystematicInflationAmount(0.00);

  n.setUseCovarianceMatrix(false);
  n.setUseSignalCovarianceMatrix(false);
  // Gotta define the backgrounds:
  int kUnisim(0), kPiMinus(1), kPiPlus(2), kKaon0(3), kKMinus(4), kKPlus(5), kTotal(6);
  // n.setMultiWeightSource(kUnisim);
  // n.setMultiWeightSource(kPiMinus);
  // n.setMultiWeightSource(kPiPlus);
  // n.setMultiWeightSource(kKaon0);
  // n.setMultiWeightSource(kKMinus);
  // n.setMultiWeightSource(kKPlus);
  n.setMultiWeightSource(kTotal);

  n.setAbsolute_MWSource(false);

  n.setNpoints(50);
  n.setNWeights(1000);

  n.setSavePlots(true);

  n.Prepare();
  n.ReadData();
  // n.BuildCovarianceMatrix();
  // n.MakeRatioPlots();
  // n.Loop();
  // n.MakePlots();
  // n.MakeSimplePlot();
  n.MakeEventRatePlots();
  // n.MakeAltSensPlot();

/*
Total number of events in the background at L = 100m: 18782.8
Total number of events in the background at L = 150m: 8136.13
Total number of events in the background at L = 200m: 4472.6

 */


}
