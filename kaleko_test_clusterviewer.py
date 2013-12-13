# Load libraries
import ROOT, sys, os
from ROOT import *

# Now import ana_processor & your class. For this example, ana_base.
gSystem.Load("libAnalysis")

#if len(sys.argv) != 2:
#    print
#    print "*** Improper usage. Usage: python kaleko_trackviewer.py /path/to/input/file.root ***"
#    print


filename = "shower_dsout_sac.root"



my_proc = larlight.ana_processor()

my_proc.set_io_mode(larlight.storage_manager.READ)

#my_proc.add_input_file(sys.argv[1])
my_proc.add_input_file(filename)

larlight.storage_manager.get().set_in_rootdir("scanner")

my_proc.set_ana_output_file("my_empty_clusterviewer_output.root")

my_ana = larlight.ClusterViewer()

my_proc.add_process(my_ana)





c=TCanvas("c","",600,500)
gStyle.SetOptStat(0)

#while my_proc.process_event():

my_proc.process_event()

vReco = my_ana.GetHisto_Reco_0()

#vReco.at(0).Draw()
#    for x in xrange(vReco.size()-1):
#        vReco.at(x+1).Draw("same");

print "continuing to next evt..."
sys.stdin.readline()

# done!
