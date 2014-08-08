from ROOT import larlight as fmwk
import sys

mgr = fmwk.ana_processor()

mgr.set_io_mode(fmwk.storage_manager.BOTH)

mgr.set_rootdir('scanner')

for x in xrange(len(sys.argv)-2):

    mgr.add_input_file(sys.argv[x+2])

mgr.set_output_file(sys.argv[1])

mgr.set_data_to_read(fmwk.DATA.Shower,False)

mgr.add_process(fmwk.ana_base())

mgr.run()




