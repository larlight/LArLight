# Load libraries
import ROOT, sys, os
from ROOT import *

# Now import ana_processor & your class. For this example, ana_base.
gSystem.Load("libAnalysis")
gSystem.Load("libLArUtil")
gSystem.Load("libDavidAna")

filename = sys.argv[1]

my_proc = larlight.ana_processor()

my_proc.set_verbosity(larlight.MSG.DEBUG)

my_proc.set_io_mode(larlight.storage_manager.READ)

my_proc.add_input_file(filename)

#larlight.storage_manager.get().set_in_rootdir("scanner")
larlight.storage_manager.get().set_data_to_read(larlight.DATA.FFTHit)
larlight.storage_manager.get().set_data_to_read(larlight.DATA.UserInfo,False)

#is there a way to disable ana_proc from creating an output file at all?
my_proc.set_ana_output_file("")

my_ana = larlight.HitViewer()

my_proc.add_process(my_ana)


gStyle.SetOptStat(0)

c=TCanvas("c","Wire v. Time Hit Viewer",900,600)
#c.Divide(1,3)

while my_proc.process_event():

    currentview = 0;
    #First fill the 6 pads on the main canvas with stuff
    for pad in xrange(1,2,1):
        
        c.cd(pad)
        my_ana.GetHisto_Hits(int(currentview)).Draw("COLZ")
    
        currentview = currentview + 1
        c.Update()
    

    usrinput = raw_input("Hit Enter: next evt  ||  q: exit viewer\n")
    if ( usrinput == "q" ):
        break
    

# done!
