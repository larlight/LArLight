

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
  n.setFlatSystematicError(0.20);
  n.setMode("nu");
  n.setUse100m(false);
  n.setUse150m(true);
  n.setUse200m(false);
  n.setUse100mLong(false);
  n.setUse470m(false);
  n.setUse700m(false);
  n.setUseT600_onaxis(true);
  n.setUseT600_offaxis(false);
  n.setUbooneScale(1.0/(1.65*1.65));
  n.setLAr1NDScale(1.0/(1.65*1.65));
  // n.setLAr1NDScale(1.0/3.0);
  n.setLAr1FDScale(1.0);
  n.setEnergyType("ecalo2");
  n.setUseNearDetStats(false);
  n.setShapeOnlyFit(true);
  n.setNearDetSystematicError(0.2);
  n.setForceRemake(false);
  n.setUseInfiniteStatistics(false);
  // n.setElectContainedDist(150);

  n.setInflateSystematics(false);
  n.setSystematicInflationAmount(0.04);

  n.setUseCovarianceMatrix(true);
  n.setUseSignalCovarianceMatrix(false);

  n.setNpoints(150);
  n.setNWeights(1000);

  n.setSavePlots(true);

  n.Prepare();
  n.ReadData();
  // n.BuildCovarianceMatrix();
  // n.MakeRatioPlots();
  n.Loop();
  // n.MakePlots();
  n.MakeSimplePlot();
  // n.MakeEventRatePlots();
  // n.MakeAltSensPlot();


}
