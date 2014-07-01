import sys
from ROOT import *
from ROOT import larlight as fmwk

processor = fmwk.ana_processor()

processor.set_io_mode(fmwk.storage_manager.READ)

ana_module = fmwk.CMatchPerformance()

priority_algo = cmtool.CPAlgoNHits()
priority_algo.SetMinHits(20)

match_algo = cmtool.CFAlgoStartPointMatch()

ana_module.GetManager().AddMatchAlgo(match_algo)
ana_module.GetManager().AddPriorityAlgo(priority_algo)

processor.add_process(ana_module)

processor.add_input_file(sys.argv[1])

if len(sys.argv)>2:

    processor.set_rootdir(sys.argv[2])

processor.set_output_file("")

processor.set_ana_output_file("ana.root")

processor.run()
