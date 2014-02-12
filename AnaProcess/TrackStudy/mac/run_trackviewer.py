#
# Example PyROOT script to run analysis module, ana_base.
# The usage is same for inherited analysis class instance.
#

# Load libraries
import os, ROOT, sys
from ROOT import gSystem
gSystem.Load("libAnalysis")

# Now import ana_processor & your class. For this example, ana_base.
from ROOT import *

# Create ana_processor instance
my_proc=larlight.ana_processor()

# Specify IO mode
my_proc.set_io_mode(larlight.storage_manager.READ)
#my_proc.set_io_mode(storage_manager.WRITE)
#my_proc.set_io_mode(storage_manager.BOTH)

# Set input root file: this is decoder output root file.
# This time, we use a sample file prepared.
my_proc.add_input_file(sys.argv[1])

larlight.storage_manager.get().set_in_rootdir("scanner")

# Set output root file: this is a separate root file in which your
# analysis module can store anything such as histograms, your own TTree, etc.
my_proc.set_ana_output_file("myout.root")

# Add analysis unit to process
my_ana = larlight.TrackViewer()
my_proc.add_process(my_ana)

# Set TDirectory name in the ROOT file where TTrees can be found
# If no TDirectory, comment out.

ctr=0
c=TCanvas("c","",600,500)

while my_proc.process_event() and my_proc.get_process_status() == my_proc.PROCESSING:

    print "Process status:",my_proc.get_process_status()

    hMC   = my_ana.GetHisto_MC()
    hSPS  = my_ana.GetHisto_SPS()
    vReco = my_ana.GetHisto_Reco()
    vData = my_ana.GetData_Reco()

    first_draw=True

    if hSPS:
        hSPS.Draw("BOX")
        first_draw=False

    if hMC:
        if first_draw: 
            hMC.Draw("BOX")
        else:
            hMC.Draw("BOX sames")

    print
    print "Entry %d ... found %d spacepoints & %d reco-ed track" % (ctr,hSPS.GetEntries(),vReco.size())

    for x in xrange(vReco.size()):
        print "Plotting track %d " % vData[x].track_id()
        print "Number of points: %d" % vData[x].n_point()
        vReco[x].Draw("BOX sames")
        c.Update()

        sys.stdin.readline()

    ctr+=1

# done!
