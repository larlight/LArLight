# Load libraries
import ROOT, sys, os
from ROOT import *

# Now import ana_processor & your class. For this example, ana_base.
gSystem.Load("libAnalysis")

#if len(sys.argv) != 2:
#    print
#    print "*** Improper usage. Usage: python kaleko_trackviewer.py /path/to/input/file.root ***"
#    print


#filename = "shower_dsout_sac.root"
filename = "/Users/kazuhiro/PlayGIT/LArLight/AnaProcess/Analysis/mac/DATA/PDGID_11/shower_larlight_11.root"



my_proc = larlight.ana_processor()

my_proc.set_io_mode(larlight.storage_manager.READ)

#my_proc.add_input_file(sys.argv[1])
my_proc.add_input_file(filename)

larlight.storage_manager.get().set_in_rootdir("scanner")

my_proc.set_ana_output_file("my_empty_clusterviewer_output.root")

my_ana = larlight.ClusterViewer()

my_proc.add_process(my_ana)





c=TCanvas("c","",600,800)
c.Divide(1,3)
gStyle.SetOptStat(0)

#while my_proc.process_event():

my_proc.process_event()



for pad in xrange(3):

    c.cd(pad+1)

    vReco = my_ana.GetHisto_Reco(int(pad))
        
    for x in xrange(vReco.size()):
        vReco.at(x).Draw("same")
        
c.Update()
print "continuing to next evt..."
sys.stdin.readline()

# done!
