import sys
from ROOT import *
from ROOT import larlight as fmwk

processor = fmwk.ana_processor()

processor.set_io_mode(fmwk.storage_manager.READ)

processor.set_rootdir('scanner')

for x in xrange(len(sys.argv)):

    if x < 1: continue
    processor.add_input_file(sys.argv[x])

processor.set_ana_output_file("ana.root")

pi0Mass_module = fmwk.ComputePi0Mass.GetInstance()

pi0_study_module = fmwk.Pi0StudyModule()

processor.add_process(pi0Mass_module)
processor.add_process(pi0_study_module)

processor.run()
