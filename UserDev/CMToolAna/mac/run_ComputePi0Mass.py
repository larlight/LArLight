import sys
from ROOT import *
from ROOT import larlight as fmwk

processor = fmwk.ana_processor()

#need to use BOTH so output from first module is used as input to second
processor.set_io_mode(fmwk.storage_manager.READ)

processor.set_rootdir('scanner')

for x in xrange(len(sys.argv)):

    if x < 1: continue
    processor.add_input_file(sys.argv[x])

processor.set_ana_output_file("ana.root")

shower_module = fmwk.ShowerReco3D()

pi0Mass_module = fmwk.ComputePi0Mass()
pi0Mass_module.SetDebug(True)
pi0Mass_module.SetClusterType(fmwk.DATA.RyanCluster)

processor.add_process(pi0Mass_module)

processor.run()
