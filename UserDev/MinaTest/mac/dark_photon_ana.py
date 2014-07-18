import sys
from ROOT import *

ch=TChain("myana/Toy_tree","")

ch.AddFile(sys.argv[1])

print "File opened"

my_ana = DarkPhotonAnaC()

my_ana.RunEventLoop(ch);
