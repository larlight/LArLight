import sys
from ROOT import *
gSystem.Load("libCore")
gSystem.Load("libOpticalDetectorSim")

proc=opdet.Tester()

mgr = larlight.storage_manager()
mgr.set_io_mode(larlight.storage_manager.WRITE)
mgr.set_out_filename("out.root")
mgr.open()

proc.initialize()

for x in xrange(10):
    print "Event",x
    proc.analyze(mgr)
    mgr.next_event()

proc.finalize()
mgr.close()


