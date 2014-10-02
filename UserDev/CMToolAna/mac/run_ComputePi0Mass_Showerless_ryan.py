import sys
from ROOT import *
from ROOT import larlight as fmwk

processor = fmwk.ana_processor()

#need to use BOTH so output from first module is used as input to second
processor.set_io_mode(fmwk.storage_manager.BOTH)
#processor.set_io_mode(fmwk.storage_manager.WRITE)

processor.set_rootdir('scanner')

for x in xrange(len(sys.argv)):

    if x < 1: continue
    processor.add_input_file(sys.argv[x])

#need to save output file from shower module otherwise it won't work
#^^^ WHY??
processor.set_output_file("afterout.root")

processor.set_ana_output_file("afterana.root")



pi0Mass_module = fmwk.ComputePi0Mass_RG()
pi0Mass_module.SetDebug(True)
#pi0Mass_module.SetShowerType(fmwk.DATA.Shower)


processor.add_process(pi0Mass_module)

processor.run()
