import sys

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

from ROOT import gSystem
gSystem.Load("libKalekoAna")
from ROOT import larlight as fmwk
from ROOT import kaleko
from ROOT import *
# Create ana_processor instance
my_proc = fmwk.ana_processor()
my_proc.set_verbosity(fmwk.MSG.NORMAL)

# Set input root file
my_proc.add_input_file(sys.argv[1])

# Have it find the scanner directory
fmwk.storage_manager.get().set_in_rootdir("scanner")
fmwk.storage_manager.get().set_data_to_read(fmwk.DATA.MCTruth,False)
fmwk.storage_manager.get().set_data_to_read(fmwk.DATA.Calorimetry,False)
fmwk.storage_manager.get().set_data_to_read(fmwk.DATA.Shower,False)
#fmwk.storage_manager.get().set_data_to_read(fmwk.DATA.ShowerAngleCluster,False)


# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.READ)

# Specify input TDirectory name if given
if len(sys.argv) > 2:

    my_proc.set_input_rootdir(sys.argv[2])

# Specify output root file name
my_proc.set_ana_output_file("out.root");

# Attach a template process
my_ana= kaleko.QuickClusterAna()
my_viewer = fmwk.ClusterViewer()

my_proc.add_process(my_ana)
my_proc.add_process(my_viewer)

print
print  "Finished configuring ana_processor. Start event loop!"
print

my_viewer.SetClusterType(larlight.DATA.FuzzyCluster)


c_graph=TCanvas("csub","Individual cluster start/end points",500,300)

while my_proc.process_event():

    currentview = 0;

    #draw individual clusters
    c_graph.cd()
    for iview in xrange(0,3):
        for iclus in xrange(my_viewer.GetClusterGraph_Reco(int(iview),bool(true)).size()):

            if int(my_viewer.GetHisto_Reco(int(iview)).at(iclus).GetEntries()) < 15:
                continue

            gstart=my_viewer.GetClusterGraph_Reco(int(iview),bool(true)).at(iclus)
            gend  =my_viewer.GetClusterGraph_Reco(int(iview),bool(false)).at(iclus)
            print "this cluster has %d hits" % my_viewer.GetHisto_Reco(int(iview)).at(iclus).GetEntries()
            xmin=my_viewer.GetHisto_Hits(int(iview)).GetXaxis().GetXmin()
            xmax=my_viewer.GetHisto_Hits(int(iview)).GetXaxis().GetXmax()
            ymin=my_viewer.GetHisto_Hits(int(iview)).GetYaxis().GetXmin()
            ymax=my_viewer.GetHisto_Hits(int(iview)).GetYaxis().GetXmax()
            gstart.GetXaxis().SetLimits(xmin,xmax)
            gend.GetXaxis().SetLimits(xmin,xmax)       
            gstart.GetYaxis().SetRangeUser(ymin,ymax)
            gend.GetYaxis().SetRangeUser(ymin,ymax)
            gstart.SetTitle("View: %d, Cluster: %d"%(iview+1,iclus))
            gstart.SetMarkerSize(3)
            gstart.SetMarkerStyle(30)
            gend.SetMarkerSize(3)
            gend.SetMarkerStyle(29)
            gstart.Draw("ALP")
            gend.Draw("LP")
            my_viewer.GetHisto_Reco(int(iview)).at(iclus).Draw("same")
            leg = TLegend(0.6,0.65,0.88,0.85)
            leg.AddEntry(gstart,"Start Point","p")
            leg.AddEntry(gend,"End Point","p")
            leg.Draw()
            c_graph.Update()
            print "Drawing cluster %d out of %d for view %d. To look at the next cluster hit enter." % (iclus,my_viewer.GetClusterGraph_Reco(int(iview),bool(true)).size()-1,iview+1)
            sys.stdin.readline()

    print "Hit Enter to continue to next evt..."
    sys.stdin.readline()

# done!
