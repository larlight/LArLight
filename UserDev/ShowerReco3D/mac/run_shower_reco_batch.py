import sys

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)


from ROOT import gSystem,TMath
gSystem.Load("libShowerReco3D")
gSystem.Load("libBase")
gSystem.Load("libLArUtil")
from ROOT import larlight as fmwk, cmtool, shower,larutil

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

#
# Kazu disables to read-in shower data product from input (only speeds up)
#
my_proc.set_data_to_read(fmwk.DATA.Shower,False)

# Create analysis unit
ana_unit = fmwk.ShowerReco3D()

# 
# Attach Matching algorithm
#

#Andrzej: The algorithms below are ranked by their effectiveness-- TimeOverlap is best, 
#then 3DAngle, then StartPoint . Right now, only TimeOverlap is called.
algo_array = cmtool.CFAlgoArray()
#algo_array.SetMode(cmtool.CFAlgoArray.kPositiveAddition)
algo_array.AddAlgo(cmtool.CFAlgoTimeOverlap())
#algo_array.AddAlgo(cmtool.CFAlgo3DAngle())
#algo_array.AddAlgo(cmtool.CFAlgoStartPointMatch())

ana_unit.GetManager().AddMatchAlgo(algo_array)

my_proc.add_process(ana_unit)

print
print  "Finished configuring ana_processor. Start event loop!"
print

my_proc.run(0)

sys.exit(0)

