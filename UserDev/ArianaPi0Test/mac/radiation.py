import sys

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

from ROOT import gSystem
gSystem.Load("libArianaPi0Test")
from ROOT import larlight as fmwk

# Create ana_processor instance
my_proc = fmwk.ana_processor()

#my_proc.add_input_file(sys.argv[1])

# Set input root file
for x in xrange(len(sys.argv)-1):
   my_proc.add_input_file(sys.argv[x+1])
#my_proc.add_input_file("~/WorkArea/data/NewFiles/LArLight/pdg_111/combined_cluster_00.root")

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.READ)

my_proc.set_rootdir("scanner")

# Specify output root file name
my_proc.set_ana_output_file("ana.root");

# Attach a template process
my_proc.add_process(fmwk.radiation_test());

print
print  "Finished configuring ana_processor. Start event loop!"
print

# Let's run it.
my_proc.run();

# done!
print
print "Finished running ana_processor event loop!"
print

sys.exit(0)

