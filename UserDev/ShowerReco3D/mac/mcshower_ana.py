import sys
from ROOT import larlight as fmwk

mgr = fmwk.ana_processor()

mgr.set_io_mode(fmwk.storage_manager.READ)

mgr.set_verbosity(fmwk.MSG.DEBUG)

for x in xrange(len(sys.argv)-1):

    mgr.add_input_file(sys.argv[x+1])

mgr.set_rootdir("scanner")

mgr.set_ana_output_file("out_%s" % sys.argv[1])

mgr.add_process(fmwk.MCShowerAna())

mgr.run()




