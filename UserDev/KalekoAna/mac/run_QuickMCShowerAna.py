import sys

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

from ROOT import gSystem
gSystem.Load("libKalekoAna")
from ROOT import larlight as fmwk
from ROOT import kaleko
# Create ana_processor instance
my_proc = fmwk.ana_processor()
my_proc.set_verbosity(fmwk.MSG.NORMAL)

# Set input root file
my_proc.add_input_file(sys.argv[1])

# Have it find the scanner directory
fmwk.storage_manager.get().set_in_rootdir("scanner")

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.READ)

# Specify output root file name
my_proc.set_ana_output_file("QuickMCShowerAna_ana_out.root");

# Attach a template process
my_ana= kaleko.QuickMCShowerAna()
my_proc.add_process(my_ana)

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

