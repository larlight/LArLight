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

my_merge_alg = larlight.ClusterMergeAlg()
my_merger = larlight.ClusterMerge()

my_merger.set_mergealg(my_merge_alg)

ana_proc.add_process(my_merge_alg)

ana_proc.add_process(my_merger)

while ana_proc.process_event():
    
    print my_merge_alg.GetMergeTree()

    sys.stdin.readline()

#ana_proc.run()


