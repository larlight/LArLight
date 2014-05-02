  
from ROOT import *
gSystem.Load("liblar1Sens.so");
from ROOT import lar1
# from ROOT import larlight

n = lar1.NueAppearanceFitter()
# n.setFileSource("/Users/cja33/Desktop/LAr1ND/lar1Repos/lar1/ntuples_2/");
n.setVerbose(False);
n.setDebug(False);
n.setSpecialNameText("");
n.setSpecialNameTextOsc("");
n.setSpecialNameText_far("600");
n.setSpecialNameTextOsc_far("600");
n.setFlatSystematicError(0.2);
n.setMode("nu");
n.setUse100m(True);
n.setUse100mLong(False);
n.setUse470m(True);
n.setUse700m(False);
n.setUseT600(True);
n.setUbooneScale(1.0);
n.setLAr1NDScale(1.0);
n.setLAr1FDScale(1.0);
n.setEnergyType("ecalo1");
n.setNpoints(10);
n.setUseNearDetStats(true);
n.setShapeOnlyFit(false);
n.setNearDetSystematicError(0.2);



n.setNpoints(250);

n.Prepare();
n.ReadData();
n.Loop();
# n.MakePlots();
n.MakeEventRatePlots();

