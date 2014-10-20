#
# Example PyROOT script to run analysis module, ana_base.
# The usage is same for inherited analysis class instance.
#

# Load libraries
import os, ROOT, sys
from ROOT import gSystem
import time
gSystem.Load("libAnalysis")
gSystem.Load("libLArUtil")
gSystem.Load("libDavidAna")


# Now import ana_processor & your class. For this example, ana_base.
from ROOT import larlight as fmwk

# Create ana_processor instance
my_proc=fmwk.ana_processor()

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.READ)
#my_proc.set_io_mode(storage_manager.WRITE)
#my_proc.set_io_mode(storage_manager.BOTH)

# Specify what data to read
my_proc.set_data_to_read(fmwk.DATA.MCTruth)
my_proc.set_data_to_read(fmwk.DATA.MCParticle)

# Set input root file: this is decoder output root file.
# This time, we use a sample file prepared.
#my_proc.add_input_file("./../../../NevisDecoder/Decoder/mac/xmit_subrun_2014_01_13_1_dma_no_1.root")
#my_proc.add_input_file("./../../../NevisDecoder/Decoder/mac/xmit_subrun_2014_01_13_1_trigger.root")
my_proc.add_input_file(sys.argv[1])

# Specify ROOT TDirectory in the file if such structure is present (which is the case for DataScanner output)
my_proc.set_input_rootdir("scanner")

# Set output root file: this is a separate root file in which your
# analysis module can store anything such as histograms, your own TTree, etc.
my_proc.set_ana_output_file("tpcout.root")

# Create analysis class instance. For this example, ana_base.
# To show how one can run multiple analysis modules at once,
# we make multiple ana_base instance.

makeTree=fmwk.MakeMCTree()
makeTree.SetVerbose(False)

# Add analysis modules to the processor

my_proc.add_process(makeTree)
#my_proc.add_process(my_ana_2)
#my_proc.add_process(my_ana_3)

# Let's run it.
t0 = int(round(time.time()*1000))
numEvts = 100
my_proc.run(0,numEvts)
t1 = int(round(time.time()*1000))
dt = (t1-t0)/1000. #seconds
print "time diff is {0} sec.".format(dt)
print "time per event is: {0} milli-sec.".format(1000*dt/numEvts)
'''
while my_proc.process_event():
    usrinput = raw_input("Hit Enter: next evt  ||  q: exit viewer\n")
    if ( usrinput == "q" ):
        break
'''
# done!
