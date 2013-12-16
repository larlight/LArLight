# Load libraries
import ROOT, sys, os
from ROOT import *

# Now import ana_processor & your class. For this example, ana_base.
gSystem.Load("libAnalysis")

#if len(sys.argv) != 2:
#    print
#    print "*** Improper usage. Usage: python viewclusters.py /path/to/input/file.root ***"
#    print


filename = sys.argv[1]
#filename = "/Users/davidkaleko/Data/ShowerStudy/PDGID_11/shower_larlight_11.root"
my_proc = larlight.ana_processor()
my_proc.set_verbosity(larlight.MSG.DEBUG)

my_proc.set_io_mode(larlight.storage_manager.READ)

my_proc.add_input_file(filename)

larlight.storage_manager.get().set_in_rootdir("scanner")
larlight.storage_manager.get().set_data_to_read(larlight.DATA.MCTruth,False)
larlight.storage_manager.get().set_data_to_read(larlight.DATA.Shower,False)
larlight.storage_manager.get().set_data_to_read(larlight.DATA.Calorimetry,False)
larlight.storage_manager.get().set_data_to_read(larlight.DATA.UserInfo,False)

#is there a way to disable ana_proc from creating an output file at all?
my_proc.set_ana_output_file("")

my_ana = larlight.ClusterViewer()

my_proc.add_process(my_ana)


gStyle.SetOptStat(0)



c=TCanvas("c","Wire v. Time Cluster Viewer",900,600)
c.Divide(2,3)

c_graph=TCanvas("csub","Individual cluster start/end points",500,300)

while my_proc.process_event():
    currentview = 0;
    for pad in xrange(1,7,2):
        
        c.cd(pad)
        
        vReco = my_ana.GetHisto_Reco(int(currentview))
        vReco.at(0).Draw()
        for x in xrange(1,vReco.size()):
            vReco.at(x).Draw("same")
    
        c.cd(pad+1)
        my_ana.GetHisto_Hits(int(currentview)).Draw("COLZ")
    
        currentview = currentview + 1
        c.Update()
    
    #now draw individual clusters on the separate canvase
    c_graph.cd()
    for iview in xrange(0,3):
        for iclus in xrange(my_ana.GetClusterGraph_Reco(int(iview)).size()):
            g=my_ana.GetClusterGraph_Reco(int(iview)).at(iclus)
            xmin=my_ana.GetHisto_Hits(int(iview)).GetXaxis().GetXmin()
            xmax=my_ana.GetHisto_Hits(int(iview)).GetXaxis().GetXmax()
            ymin=my_ana.GetHisto_Hits(int(iview)).GetYaxis().GetXmin()
            ymax=my_ana.GetHisto_Hits(int(iview)).GetYaxis().GetXmax()
            g.GetXaxis().SetLimits(xmin,xmax)       
            g.GetYaxis().SetRangeUser(ymin,ymax)
            g.SetTitle("View: %d, Cluster: %d"%(iview+1,iclus))
            g.SetMarkerSize(5)
            g.SetMarkerStyle(34)
            g.Draw("ALP")
            c_graph.Update()
            print "Drawing cluster %d out of %d for view %d. To look at the next cluster hit enter." % (iclus+1,my_ana.GetClusterGraph_Reco(int(iview)).size(),iview+1)
            sys.stdin.readline()

    print "Hit Enter to continue to next evt..."
    sys.stdin.readline()

# done!
