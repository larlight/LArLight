import sys

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

from ROOT import gSystem
from ROOT import larlight as fmwk

# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Set input root file
for i in xrange(len(sys.argv)):
    if i == 0: continue;
    my_proc.add_input_file(sys.argv[i])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.READ)

# Specify input TDirectory name if given (DEFAULT SCANNER)
my_proc.set_input_rootdir("scanner")

# Specify output root file name
my_proc.set_ana_output_file("tmpfile.root")

# Enable filter mode for ana_processor
my_proc.enable_filter(True)

# Attach filters to do event selection
lee_filter = fmwk.LEEEventSelection()
#lee_filter.SetFiducialCutDistance(17.)
my_proc.add_process(lee_filter)

# Attach my process that converts larlight data into ubsens LEE data holder
lee_ds = fmwk.LEEDataScanner()
lee_ds.SetIncludeRecoShowers(False)
my_proc.add_process(lee_ds)

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

