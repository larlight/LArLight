import ROOT,sys
ROOT.gSystem.Load("libClusterRecoUtil")
from ROOT import *
from ROOT import larlight as fmwk

mgr = fmwk.ana_processor()

mgr.add_input_file(sys.argv[1])

mgr.set_input_rootdir("scanner")

mgr.set_output_file("")

mgr.set_io_mode(fmwk.storage_manager.READ)

mgr.set_ana_output_file("")

proc = fmwk.ClusterMerger()

proc.GetManager().AddAlgo(cluster.CMAlgoFake())

proc.GetManager().DebugMode(cluster.CMergeManager.kPerIteration)
#proc.GetManager().DebugMode(cluster.CMergeManager.kPerMerging)

mgr.add_process(proc)

mgr.run(0,1)



