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
my_proc=ana_processor()

# Specify IO mode
my_proc.set_io_mode(storage_manager.READ)
#my_proc.set_io_mode(storage_manager.WRITE)
#my_proc.set_io_mode(storage_manager.BOTH)

# Set input root file: this is decoder output root file.
# This time, we use a sample file prepared.
my_proc.add_input_file("larlight.root")

# Set output root file: this is a separate root file in which your
# analysis module can store anything such as histograms, your own TTree, etc.
my_proc.set_ana_output_file("myout.root")

# Add analysis unit to process
my_proc.add_process(TrackViewer())

# Set TDirectory name in the ROOT file where TTrees can be found
# If no TDirectory, comment out.
storage_manager.get().set_in_rootdir("scanner")

while my_proc.process_event():
    
    sys.stdin.readline()

# done!
