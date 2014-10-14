import sys

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

from ROOT import gSystem
from ROOT import larlight, cluster, cmtool

# Create ana_processor instance
my_proc = larlight.ana_processor()

# Set input root file
for x in xrange(len(sys.argv)-1):

    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(larlight.storage_manager.READ)

# Specify input TDirectory name
my_proc.set_input_rootdir("scanner")

# Specify output root file name
my_proc.set_ana_output_file("MCShowerEff.root");

#attach the CMergePerformance process
my_unit = larlight.MCShowerEff()

my_unit.SetClusterType(larlight.DATA.RyanCluster)
#my_unit.SetClusterType(larlight.DATA.FuzzyCluster)

my_proc.add_process(my_unit)

my_proc.set_verbosity(larlight.MSG.DEBUG)

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

