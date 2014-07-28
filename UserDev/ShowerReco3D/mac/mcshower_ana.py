import sys
from ROOT import larlight as fmwk

mgr = fmwk.ana_processor()

mgr.set_io_mode(fmwk.storage_manager.READ)

mgr.add_input_file(sys.argv[1])

if len(sys.argv) > 2:

    mgr.set_rootdir(sys.argv[2])

mgr.set_ana_output_file("out_%s" % sys.argv[1])

mgr.add_process(fmwk.MCShowerAna())

mgr.run(0)




