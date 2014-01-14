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

# We want to read & write the data file
my_proc.set_io_mode(storage_manager.BOTH)

# Set input root file: this is decoder output root file.
# This time, we use a sample file prepared.
my_proc.add_input_file(sys.argv[1])

# Set output root file: this is a separate root file in which your
# analysis module can store anything such as histograms, your own TTree, etc.
my_proc.set_ana_output_file("ana.root")

# Set output data file: this is data output root file
my_proc.set_output_file("out.root")

# Create pulse reconstruction instance to be attached to ana_processor
preco = pulse_reco()

# Add pulse reconstruction algorithms you wish to run
preco.add_reco_algo(algo_fixed_window())
preco.add_reco_algo(algo_threshold())

# Set parameters for pedestal estimation
preco.set_ped_algo(preco.kHEAD)
preco.set_ped_nsample_cosmic(1)

# Attach pulse reco unit to ana_processor
my_proc.add_process(preco)

# Uncomment if you wish to run in verbose mode
#my_proc.set_verbosity(MSG.DEBUG)

# Let's run it.
my_proc.run()

# done!
