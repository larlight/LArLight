

{
  gSystem->Load("lib/liblar1Sens.so");
  lar1::NueAppearanceFitter n;
  n.setFileSource("/uboone/app/users/cadams/reprocessing/LArLight/UserDev/lar1Sens/files/");
  n.setVerbose(true);
  n.setDebug(false);
  n.setSpecialNameText("");
  n.setSpecialNameTextOsc("");
  n.setSpecialNameText_far("");
  n.setSpecialNameTextOsc_far("");
  n.setFlatSystematicError(0.00);
  n.setMode("nu");
  n.setUse100m(true);
  n.setUse150m(false);
  n.setUse200m(false);
  n.setUse100mLong(false);
  n.setUse470m(false);
  n.setUse700m(false);
  n.setUseT600_onaxis(true);
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

  n.setInflateSystematics(false);
  n.setSystematicInflationAmount(0.00);

  n.setUseCovarianceMatrix(true);
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
  n.Loop();
  // n.MakePlots();
  // n.MakeSimplePlot();
  // n.MakeEventRatePlots();
  // n.MakeAltSensPlot();

/*
Total number of events in the background at L = 100m: 18782.8
Total number of events in the background at L = 150m: 8136.13
Total number of events in the background at L = 200m: 4472.6

 */


}
