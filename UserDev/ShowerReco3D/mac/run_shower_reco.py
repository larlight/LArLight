import sys

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

from ROOT import gSystem
gSystem.Load("libShowerReco3D")
from ROOT import larlight as fmwk, cmtool, shower

# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Set input root file
my_proc.add_input_file(sys.argv[1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.BOTH)

# Specify input TDirectory name if given
if len(sys.argv) > 2:

    my_proc.set_input_rootdir(sys.argv[2])

# Specify analysis output root file name
my_proc.set_ana_output_file("from_test_ana_you_can_remove_me.root");

# Specify data output root file name
my_proc.set_output_file("out.root")

# Create analysis unit
ana_unit = fmwk.ShowerReco3D()

# 
# Attach Matching algorithm
#
algo = cmtool.CFAlgoQRatio()
ana_unit.GetManager().AddMatchAlgo(algo)

my_proc.add_process(ana_unit)

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

