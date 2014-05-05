

{
  gSystem->Load("lib/liblar1Sens.so");
  lar1::NueAppearanceFitter n;
  // n.setFileSource("/Users/cja33/Desktop/LAr1ND/lar1Repos/lar1/ntuples_2/");
  n.setVerbose(true);
  n.setDebug(false);
  n.setSpecialNameText("");
  n.setSpecialNameTextOsc("");
  n.setSpecialNameText_far("600");
  n.setSpecialNameTextOsc_far("600");
  n.setFlatSystematicError(0.04);
  n.setMode("nu");
  n.setUse100m(true);
  n.setUse100mLong(false);
  n.setUse470m(true);
  n.setUse700m(false);
  n.setUseT600(true);
  n.setUbooneScale(1.0);
  n.setLAr1NDScale(1.0);
  n.setLAr1FDScale(1.0);
  n.setEnergyType("ecalo1");
  n.setUseNearDetStats(true);
  n.setShapeOnlyFit(true);
  n.setNearDetSystematicError(0.2);
  n.setForceRemake(false);
  n.setUseInfiniteStatistics(false);
  // n.setElectContainedDist(150);

  n.setInflateSystematics(false);
  n.setSystematicInflationAmount(0.02);

  n.setNpoints(250);

  n.Prepare();
  n.ReadData();
  n.Loop();
  n.MakePlots();
  // n.MakeEventRatePlots();


}