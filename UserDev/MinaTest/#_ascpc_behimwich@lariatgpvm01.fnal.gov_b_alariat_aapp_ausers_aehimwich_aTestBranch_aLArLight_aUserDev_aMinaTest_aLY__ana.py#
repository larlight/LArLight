import sys
from ROOT import *

ch=TChain("myana/Toy_tree","")

for i in range(1,76):
    ch.AddFile("DarkPhotonCosmicHist__%s.root" %i)
    print "File number %s opened" %i

my_ana = DarkPhotonAnaC()

my_ana.RunEventLoop(ch);
