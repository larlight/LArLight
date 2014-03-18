from ROOT import *
gSystem.Load("libAnalysis")
import sys

#filename = sys.argv[1]
filename = "~/Data/YaleStudy/electron/combined_00.root"


# Create processor 
my_proc = larlight.ana_processor()
my_proc.set_verbosity(larlight.MSG.DEBUG)
my_proc.set_io_mode(larlight.storage_manager.READ)
my_proc.add_input_file(filename)
my_proc.set_input_rootdir("scanner")
my_proc.set_ana_output_file("")

# Create analyzer
my_ana = larreco.ClusterMergeAlg()

# Run processor in a loop. Each time ana_processor::run() is called, 
# analyzers that were run are detached from ana_processor instance.
# So you have to attach it @ the beginning of the loop. Also I/O config
# should be explicitly reset.

# Attach analyzer
my_proc.add_process(my_ana)


# Run
while my_proc.process_event():
    print "processing event!"






