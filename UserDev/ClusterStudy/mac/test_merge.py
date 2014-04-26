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

proc.GetManager().AddMergeAlgo(cluster.CMAlgoFake())

proc.GetManager().DebugMode(cluster.CMergeManager.kPerIteration)
#proc.GetManager().DebugMode(cluster.CMergeManager.kPerAlgoSet)
#proc.GetManager().DebugMode(cluster.CMergeManager.kPerMerging)

mgr.add_process(proc)

while mgr.process_event():

    print 
    bk=proc.GetManager().GetBookKeeper()

    res = bk.GetResult()

    for x in xrange(res.size()):

        for y in xrange(res.at(x).size()):

            print res.at(x).at(y),
        print

    sys.stdin.readline()



