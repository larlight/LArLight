

{
  gSystem->Load("lib/liblar1Sens.so");
  lar1::NueAppearanceFitter n;
  n.setFileSource("/Users/jzennamo/Desktop/LArLite/Files/");
  n.setVerbose(true);
  n.setDebug(false);
  n.setSpecialNameText("");
  n.setSpecialNameTextOsc("");
  n.setSpecialNameText_far("600");
  n.setSpecialNameTextOsc_far("600");
  n.setFlatSystematicError(0.00);
  n.setMode("nu");
  n.setUse100m(true);
  n.setUse100mLong(false);
  n.setUse470m(false);
  n.setUse700m(false);
  n.setUseT600(true);
  n.setUbooneScale(1.0);
  n.setLAr1NDScale(1.0);
  // n.setLAr1NDScale(1.0/3.0);
  //  n.setLAr1FDScale(1.0);
  n.setEnergyType("ecalo2");
  n.setUseNearDetStats(false);
  n.setShapeOnlyFit(false);
  n.setNearDetSystematicError(0.0);
  n.setForceRemake(false);
  n.setUseInfiniteStatistics(false);
  // n.setElectContainedDist(150);

  n.setInflateSystematics(false);
  n.setSystematicInflationAmount(0.04);

  n.setNpoints(250);

  n.Prepare();
  n.ReadData();
  n.Loop();
  n.MakePlots();
  //  n.MakeEventRatePlots();
  n.MakeAltSensPlot();


}
