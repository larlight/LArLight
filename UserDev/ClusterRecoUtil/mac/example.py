import sys
from ROOT import *
gSystem.Load("libClusterRecoUtil")
from ROOT import larlight as fmwk
from ROOT import cluster

mgr = fmwk.storage_manager()

mgr.set_io_mode(mgr.READ)

mgr.add_in_filename(sys.argv[1])
if len(sys.argv) > 2:
    mgr.set_in_rootdir(sys.argv[2])

mgr.open()

chit=TCanvas("chit","chit",600,500)
algo = cluster.ClusterParamsExecutor()

while mgr.next_event():

    clusters = mgr.get_data(fmwk.DATA.ShowerAngleCluster)

    if not clusters:
        continue

    print "Event:",clusters.event_id()
    for x in xrange(clusters.size()):
        
        print "  Cluster ID:",clusters.at(x).ID()

        algo.LoadCluster(clusters.at(x),
                         mgr.get_data(clusters.get_hit_type()))
        algo.FillParams(True,True,True,True,True)
        print "(%g,%g) => (%g,%g)" % (algo.StartPoint().w,
                                      algo.StartPoint().t,
                                      algo.EndPoint().w,
                                      algo.EndPoint().t)
        chit.cd()
        algo.GetHitView().Draw("COLZ")
        chit.Update()
        sys.stdin.readline()



