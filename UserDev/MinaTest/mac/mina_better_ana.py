import sys
from ROOT import *

ch=TChain("myana/toy_tree","")

ch.AddFile(sys.argv[1])

my_ana = ToyAna()

my_ana.RunEventLoop(ch);

