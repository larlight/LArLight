# Load libraries
import ROOT, sys, os
from ROOT import *

# Now import ana_processor & your class. For this example, ana_base.
gSystem.Load("libClusterStudy")
from ROOT import *
if len(sys.argv) != 2:
    print
    print "*** Improper usage. Usage: python viewclusters.py /path/to/input/file.root ***"
    print


filename = sys.argv[1]
#filename = "/Users/davidkaleko/Data/ShowerStudy/PDGID_11/shower_larlight_11.root"
my_proc = larlight.ana_processor()
my_proc.set_verbosity(larlight.MSG.DEBUG)

my_proc.set_io_mode(larlight.storage_manager.READ)

my_proc.add_input_file(filename)

larlight.storage_manager.get().set_in_rootdir("scanner")
larlight.storage_manager.get().set_data_to_read(larlight.DATA.MCTruth,False)
larlight.storage_manager.get().set_data_to_read(larlight.DATA.Shower,False)
larlight.storage_manager.get().set_data_to_read(larlight.DATA.Calorimetry,False)
larlight.storage_manager.get().set_data_to_read(larlight.DATA.UserInfo,False)

my_proc.set_ana_output_file("")

raw_viewer   = larlight.ClusterViewer()
merge_viewer = larlight.MergeViewer()


merge_viewer.SetPrintClusterInfo(True)

########################################
# attach merge algos here
########################################
prohib_array = cluster.CMAlgoArray()

tracksep_prohibit = cluster.CMAlgoTrackSeparate()
tracksep_prohibit.SetMinNumHits(30)
tracksep_prohibit.SetMinAngleDiff(0.1)
tracksep_prohibit.SetMaxOpeningAngle(30)
tracksep_prohibit.SetMinLength(10.)
tracksep_prohibit.SetMaxWidth(25.)
tracksep_prohibit.SetDebug(False)
tracksep_prohibit.SetVerbose(False)
prohib_array.AddAlgo(tracksep_prohibit,False)

outofcone_prohibit = cluster.CMAlgoOutOfConeSeparate()
outofcone_prohibit.SetDebug(False)
outofcone_prohibit.SetVerbose(False)
prohib_array.AddAlgo(outofcone_prohibit,False)

merge_viewer.GetManager().AddSeparateAlgo(prohib_array)

#algos that require AND condition have to be added in array
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
algo.SetMinHits(10)
algo.SetSquaredDistanceCut(5.)

algo_array.AddAlgo(algo,False)

trackblob = cluster.CMAlgoStartTrack()

algo_array.AddAlgo(trackblob,False)

merge_viewer.GetManager().AddMergeAlgo(algo_array)
#----------------------------------------------------------

merge_viewer.GetManager().MergeTillConverge(True)

########################################
# done attaching merge algos
########################################


my_proc.add_process(raw_viewer)

my_proc.add_process(merge_viewer)

raw_viewer.SetClusterType(larlight.DATA.FuzzyCluster)
merge_viewer.SetClusterType(larlight.DATA.FuzzyCluster)

gStyle.SetOptStat(0)

#start on first event always
user_input_evt_no = -1;

while true:

    try:
        user_input_evt_no = input('Hit Enter to continue to next evt, or type in an event number to jump to that event:')
    except SyntaxError:
        user_input_evt_no = user_input_evt_no + 1

    my_proc.process_event(user_input_evt_no)

    raw_viewer.DrawAllClusters();

    merge_viewer.DrawAllClusters();


#    for plane in xrange(larutil.Geometry.GetME().Nplanes()):
#
#        print "    Plane:", plane
#        
#        for cindex in xrange(merge_viewer.ClusterCount(plane)):
#
#            print "        Cluster:",cindex
#            merge_viewer.DrawOneCluster(plane,cindex)
#            sys.stdin.readline()
#    

    print "    Hit enter to go next event..."
    sys.stdin.readline()


# done!
