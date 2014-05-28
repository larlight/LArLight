import sys

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE merge_algo_cut_value\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

from ROOT import gSystem
gSystem.Load("libCMergePerformance")
from ROOT import larlight, cluster

filename = sys.argv[1]
angle_cutval = float(sys.argv[2])
dist_cutval = float(sys.argv[3])

# Create ana_processor instance
my_proc = larlight.ana_processor()

# Set input root file
my_proc.add_input_file(filename)

# Specify IO mode
my_proc.set_io_mode(larlight.storage_manager.READ)

# Specify input TDirectory name
my_proc.set_input_rootdir("scanner")

# Specify output root file name
#this split bs is to just isolate the filename (cut out path, cut out .root)
output_filename = "CMP_ana_out_%s_cut%s_%s.root"%(filename.split('/')[-1][:-5],angle_cutval,dist_cutval)
my_proc.set_ana_output_file(output_filename)
print "output_filename is %s" % output_filename

#attach the CMergePerformance process
my_CMP = larlight.CMergePerformance()
my_proc.add_process(my_CMP)

#can toggle off running on raw fuzzycluster data and just look @ merged stuff
my_CMP.SetRunBeforeMerging(False)
#can toggle off running merging to just make FOM plots for raw fuzzycluster data, runs faster
my_CMP.SetRunMerging(True)
my_CMP.GetManager().MergeTillConverge(False)
my_CMP.GetManager().DebugMode(cluster.CMergeManager.kNone)
my_CMP.SetClusterType(larlight.DATA.FuzzyCluster)
my_CMP.SetDebug(False)


########################################
# attach merge algos here
########################################

## PROHIBIT ALGOS ##
tracksep_prohibit = cluster.CMAlgoTrackSeparate()
tracksep_prohibit.SetMinNumHits(30)
tracksep_prohibit.SetMinAngleDiff(0.1)
tracksep_prohibit.SetMaxOpeningAngle(30)
tracksep_prohibit.SetMinLength(10.)
tracksep_prohibit.SetMaxWidth(25.)
tracksep_prohibit.SetDebug(False)
tracksep_prohibit.SetVerbose(False)
my_CMP.GetManager().AddSeparateAlgo(tracksep_prohibit)

outofcone_prohibit = cluster.CMAlgoOutOfConeSeparate()
outofcone_prohibit.SetDebug(False)
outofcone_prohibit.SetVerbose(False)
my_CMP.GetManager().AddSeparateAlgo(outofcone_prohibit)


#merge algos that require AND condition have to be added in array
#----------------------------------------------------------
algo_array = cluster.CMAlgoArray()

angalg = cluster.CMAlgoAngleCompat()
angalg.SetVerbose(False)
angalg.SetDebug(False)
angalg.SetAllow180Ambig(False)
angalg.SetUseOpeningAngle(False)
angalg.SetAngleCut(3.)
#False here means use "OR" condition
algo_array.AddAlgo(angalg,False)


algo = cluster.CMAlgoShortestDist()
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

