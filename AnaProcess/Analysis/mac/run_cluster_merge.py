from ROOT import *
gSystem.Load("libAnalysis")
import sys

ana_proc = larlight.ana_processor()

#ana_proc.set_verbosity(larlight.MSG.DEBUG)

ana_proc.set_io_mode(larlight.storage_manager.BOTH)

ana_proc.add_input_file(sys.argv[1])

larlight.storage_manager.get().set_in_rootdir("scanner")

ana_proc.set_output_file("aho.root")

ana_proc.set_ana_output_file("")

#ana_proc.add_process(larlight.ClusterMergeAlgo.get())

ana_proc.add_process(larlight.ClusterMerge())

#ana_proc.process_event()
ana_proc.run()


