# Load libraries
import ROOT, sys, os
from ROOT import *
from ROOT import gSystem
from ROOT import compress

# Now import ana_processor & your class. For this example, ana_base.
gSystem.Load("libAnalysis")
gSystem.Load("libLArUtil")
gSystem.Load("libSNCompression")

from ROOT import larlight as fmwk

filename = sys.argv[1]

my_proc = fmwk.ana_processor()

my_proc.set_io_mode(fmwk.storage_manager.READ)

my_proc.set_data_to_read(fmwk.DATA.TPCFIFO)

my_proc.add_input_file(filename)

my_proc.set_input_rootdir("scanner")

#is there a way to disable ana_proc from creating an output file at all?
my_proc.set_ana_output_file("")

compAna=fmwk.ViewCompression()

#add Compression Algorithm
compAlgo = compress.CompressionAlgoThresh()
compAlgo.SetVerbose(False)
compAlgo.SetDebug(False)
compAna.SetCompressAlgo(compAlgo)

my_proc.add_process(compAna)

c1 = TCanvas("c1","c1",900,900)
c1.Divide(1,2)

while my_proc.process_event():

    for x in range(compAna.GetNumWFs()):
        compAna.processWF()
        if (compAna.GetNumOutWFs() > 0):
            for pad in xrange(1,3,1):
                c1.cd(pad)
                compAna.GetHistos(pad).Draw()
            c1.Update()

            usrinput = raw_input("Hit Enter: next evt  ||  q: exit viewer\n")
            if ( usrinput == "q" ):
                break


    

# done!