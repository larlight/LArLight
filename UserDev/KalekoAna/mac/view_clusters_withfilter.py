# Load libraries
import ROOT, sys, os
from ROOT import *

# Now import ana_processor & your class. For this example, ana_base.
from ROOT import *

filename = sys.argv[1]
my_proc = larlight.ana_processor()
my_proc.set_verbosity(larlight.MSG.DEBUG)

my_proc.set_io_mode(larlight.storage_manager.READ)

# Set input root files... need MCShower and shower reco in 1 or many files
for x in xrange(len(sys.argv)):

    if x < 1: continue
    my_proc.add_input_file(sys.argv[x])


larlight.storage_manager.get().set_in_rootdir("scanner")

my_proc.set_ana_output_file("")

kaleko_filter   = kaleko.test_xdirreco_filter()
raw_viewer   = larlight.ClusterViewer()

#decide if to show hit charge OR MCShowers on RHS of TCanvas [default: false]
#raw_viewer.SetDrawShowers(True)

#if you're showing hit charge, whether to use log z scale [default: true]
#raw_viewer.SetHitsLogZ(False)

my_proc.enable_filter(True);
my_proc.add_process(kaleko_filter)
my_proc.add_process(raw_viewer)

raw_viewer.SetClusterType(larlight.DATA.Cluster)
raw_viewer.SetDrawStartEnd(True)

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

    sys.stdin.readline()


# done!
