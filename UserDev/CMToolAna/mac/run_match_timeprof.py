import sys
from ROOT import *
from ROOT import larlight as fmwk

processor = fmwk.ana_processor()

processor.set_io_mode(fmwk.storage_manager.READ)

processor.set_verbosity(fmwk.MSG.DEBUG)

#Getting rid of segfaults likea boss
ana_module = fmwk.CMatchPerformance()

ana_module.SetClusterType(larlight.DATA.MCShowerCluster)

priority_algo = cmtool.CPAlgoNHits()
priority_algo.SetMinHits(24)

algo_array = cmtool.CFAlgoArray()
#algo_array.SetMode(cmtool.CFAlgoArray.kPositiveAddition)
algo_array.AddAlgo(cmtool.CFAlgo3DAngle())
#algo_array.AddAlgo(cmtool.CFAlgoStartPointMatch())
#algo_array.AddAlgo(cmtool.CFAlgoTimeOverlap())
#algo_array.AddAlgo(cmtool.CFAlgoWireOverlap())
#algo_array.AddAlgo(cmtool.CFAlgoVolumeOverlap())

#match_algo = cmtool.CFAlgoStartPointMatch()

ana_module.GetManager().AddMatchAlgo(algo_array)
ana_module.GetManager().AddPriorityAlgo(priority_algo)

ana_module.GetManager().DebugMode(cmtool.CMManagerBase.kPerMerging)
ana_module.GetManager().ReportTimings(True)
#processor.add_process(shower_reco)
processor.add_process(ana_module)


processor.add_input_file(sys.argv[1])

if len(sys.argv)>2:

    processor.set_rootdir(sys.argv[2])

processor.set_output_file("")

processor.set_ana_output_file("ana.root")

processor.run()
