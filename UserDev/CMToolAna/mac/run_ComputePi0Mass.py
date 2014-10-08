import sys
from ROOT import *
from ROOT import larlight as fmwk

processor = fmwk.ana_processor()

#need to use BOTH so output from first module is used as input to second
processor.set_io_mode(fmwk.storage_manager.BOTH)

#processor.set_rootdir('scanner')

for x in xrange(len(sys.argv)):

    if x < 1: continue
    processor.add_input_file(sys.argv[x])

#need to save output file from shower module otherwise it won't work
processor.set_output_file("out.root")

processor.set_ana_output_file("ana.root")

shower_module = fmwk.ShowerReco3D()

pi0Mass_module = fmwk.ComputePi0Mass()
pi0Mass_module.SetDebug(True)
pi0Mass_module.SetApplyEnergyCorrection(False)

#
# Attach Matching algorithm
#

priority_algo = cmtool.CPAlgoNHits()
priority_algo.SetMinHits(20)
shower_module.GetManager().AddPriorityAlgo(priority_algo)

algo_array = cmtool.CFAlgoArray()
algo_array.SetMode(cmtool.CFAlgoArray.kSimpleAddition)
algo_array.AddAlgo(cmtool.CFAlgoTimeOverlap())
shower_module.GetManager().AddMatchAlgo(algo_array)



processor.add_process(shower_module)
processor.add_process(pi0Mass_module)

processor.run()
