import sys
from ROOT import *

Toy_tree_ch=TChain("myana/Toy_tree","")
POT_tree_ch=TChain("myana/Toy_POT_tree","")

for i in range(0,2):
    Toy_tree_ch.AddFile("MCC5_BNB_Cosmic_Hist__%s.root" %i)
    POT_tree_ch.AddFile("MCC5_BNB_Cosmic_Hist__%s.root" %i)
    print "File number %s opened" %i

my_ana = DarkPhotonAnaC()

my_ana.RunEventLoop(Toy_tree_ch);

Total_POT=my_ana.RunPOTLoop(POT_tree_ch);

print Total_POT
