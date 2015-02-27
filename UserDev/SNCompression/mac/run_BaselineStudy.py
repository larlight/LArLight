#
# Example PyROOT script to run analysis module, ana_base.
# The usage is same for inherited analysis class instance.
#

# Load libraries
import os, ROOT, sys
from ROOT import gSystem
from ROOT import compress

gSystem.Load("libAnalysis")
gSystem.Load("libLArUtil")
gSystem.Load("libSNCompression")

# Now import ana_processor & your class. For this example, ana_base.
from ROOT import larlight as fmwk

infilename = sys.argv[1]

# Create ana_processor instance
my_proc=fmwk.ana_processor()

# Specify IO mode
#my_proc.set_io_mode(fmwk.storage_manager.BOTH)
#my_proc.set_io_mode(storage_manager.WRITE)
my_proc.set_io_mode(fmwk.storage_manager.READ)

# Specify what data to read
my_proc.set_data_to_read(fmwk.DATA.TPCFIFO)
#my_proc.set_data_to_read(fmwk.DATA.MCTruth)

# Set input root file: this is decoder output root file.
# This time, we use a sample file prepared.
#my_proc.add_input_file("./../../../NevisDecoder/Decoder/mac/xmit_subrun_2014_01_13_1_trigger.root")
my_proc.add_input_file(infilename)
#set output data file
#my_proc.set_output_file(outfilename)

# Specify ROOT TDirectory in the file if such structure is present (which is the case for DataScanner output)
my_proc.set_input_rootdir("scanner")
my_proc.set_output_rootdir("scanner")

# Set output root file: this is a separate root file in which your
# analysis module can store anything such as histograms, your own TTree, etc.
my_proc.set_ana_output_file(sys.argv[2])
my_proc.set_output_file("compressedWFs.root")

#my_proc.set_output_rootdir("scanner")
# Create analysis class instance. For this example, ana_base.
# To show how one can run multiple analysis modules at once,
# we make multiple ana_base instance.

compAna=fmwk.StudyCompressionBufferSize()

#add Compression Algorithm
compAlgo = compress.CompressionAlgosncompress()
compAlgo.SetDebug(False)
compAlgo.SetBlockSize(64)
compAlgo.SetBaselineThresh(0.75)
compAlgo.SetVarianceThresh(1.00)
compAlgo.SetCompressThresh(3)
compAlgo.SetMaxADC(4095)
ticks = int(sys.argv[3])
compAlgo.SetUVYplaneBuffer(ticks,ticks,ticks,ticks,ticks,ticks);
compAna.SetCompressAlgo(compAlgo)


compAna.SetCompressAlgo(compAlgo)

# Add analysis modules to the processor

my_proc.add_process(compAna)

# Let's run it.

my_proc.run()

# done!
