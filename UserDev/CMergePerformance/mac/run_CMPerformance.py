import sys

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

from ROOT import gSystem
gSystem.Load("libCMergePerformance")
from ROOT import larlight, cluster, cmtool

# Create ana_processor instance
my_proc = larlight.ana_processor()

# Set input root file
my_proc.add_input_file(sys.argv[1])

# Specify IO mode
my_proc.set_io_mode(larlight.storage_manager.READ)

# Specify input TDirectory name
my_proc.set_input_rootdir("scanner")

# Specify output root file name
my_proc.set_ana_output_file("CMP_ana_out.root");

#attach the CMergePerformance process
my_CMP = larlight.CMergePerformance()
my_proc.add_process(my_CMP)

#whether you want to analyze performance of raw input data 
my_CMP.SetRunBeforeMerging(True)
#whether you want to run merging and analyze performance of output
my_CMP.SetRunMerging(False)

#if you aren't running merging, all of this is irrelevant:
my_CMP.GetManager().MergeTillConverge(False)
my_CMP.GetManager().DebugMode(cmtool.CMergeManager.kNone)
my_CMP.SetClusterType(larlight.DATA.Cluster)
my_CMP.SetDebug(False)

########################################
# attach merge algos here
########################################

## PROHIBIT ALGOS ##
tracksep_prohibit = cmtool.CBAlgoTrackSeparate()
tracksep_prohibit.SetMinNumHits(30)
tracksep_prohibit.SetMinAngleDiff(0.1)
tracksep_prohibit.SetMaxOpeningAngle(30)
tracksep_prohibit.SetMinLength(10.)
tracksep_prohibit.SetMaxWidth(25.)
tracksep_prohibit.SetDebug(False)
tracksep_prohibit.SetVerbose(False)
my_CMP.GetManager().AddSeparateAlgo(tracksep_prohibit)

outofcone_prohibit = cmtool.CBAlgoOutOfConeSeparate()
outofcone_prohibit.SetDebug(False)
outofcone_prohibit.SetVerbose(False)
my_CMP.GetManager().AddSeparateAlgo(outofcone_prohibit)


#merge algos that require AND condition have to be added in array
#----------------------------------------------------------
algo_array = cmtool.CBAlgoArray()

angalg = cmtool.CBAlgoAngleCompat()
angalg.SetVerbose(False)
angalg.SetDebug(False)
angalg.SetAllow180Ambig(False)
angalg.SetUseOpeningAngle(False)
angalg.SetAngleCut(3.)
#False here means use "OR" condition
algo_array.AddAlgo(angalg,False)


algo = cmtool.CBAlgoShortestDist()
algo.SetVerbose(False)
algo.SetDebug(False)
algo.SetMinHits(0)
algo.SetSquaredDistanceCut(5.)

algo_array.AddAlgo(algo,False)

my_CMP.GetManager().AddMergeAlgo(algo_array)
#----------------------------------------------------------

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

