import sys

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

from ROOT import gSystem
from ROOT import TCanvas, TGraph 
from ROOT import *
gSystem.Load("libBase")
gSystem.Load("libLArUtil")
gSystem.Load("libClusterRecoUtil")
gSystem.Load("libTestEff")
from ROOT import larlight as fmwk
from ROOT import cluster
from ROOT import larutil

import argparse

parser = argparse.ArgumentParser(description="Python script to process and merge showers.")
group = parser.add_mutually_exclusive_group()
group.add_argument("-v", "--verbose", help="Turn on verbose output",
                    action="store_true")
group.add_argument("-q", "--quiet", help="Turn off most output",
                    action="store_true")
parser.add_argument("-s","--source",help="Name of input file")
parser.add_argument("-o","--data-output",help="Output data file, if event is changed")
parser.add_argument("-a","--ana-output",help="Analysis output file")
parser.add_argument("-n","--num-events",help="Number of events to process")
parser.add_argument("-d","--display",help="Turn on the display to see each view before and after." )
args = parser.parse_args()

if len(sys.argv) == 1:
    print "\n-------You forgot to include a source file!-------\n"
    parser.print_help()

if args.verbose:
    print "Verbose mode turned on."
    if args.source != None:
        print "\tSource file is " + args.source
    if args.data_output != None:
        print "\tData output file is " + args.data_output
    if args.ana_output != None:
        print "\tAna output file is " + args.ana_output

if args.source == None:
    print "Error: please specificy an input file with -s or --source."
    quit()

if args.num_events == None:
    args.num_events = -1

if args.data_output == None:
    args.data_output = "default_event_output.root"
    print "No event output file selected.  If necessary, output will go to:"
    print "\t"+args.data_output

if args.ana_output == None:
    args.ana_output = "default_ana_output.root"
    print "No ana output file selected.  If necessary, output will go to:"
    print "\t"+args.ana_output

mgr = fmwk.storage_manager()

mgr.set_io_mode(mgr.READ)

mgr.add_in_filename(args.source)
if len(sys.argv) > 2:
    mgr.set_in_rootdir("scanner")

mgr.open()


# Create ana_processor instance
#my_proc = fmwk.ana_processor()

# Set input root file
#my_proc.add_input_file(sys.argv[1])

# Specify IO mode
#my_proc.set_io_mode(fmwk.storage_manager.READ)

# Specify input TDirectory name if given
#if len(sys.argv) > 2:

 #   my_proc.set_input_rootdir(sys.argv[2])

# Specify output root file name
#my_proc.set_ana_output_file("from_test_ana_you_can_remove_me.root");


# Attach a template process
#my_proc.add_process(fmwk.TestEff());

print
print  "Finished configuring ana_processor. Start event loop!"
print


chit=TCanvas("chit","chit",600,500)
#chit.SetGridx(1)
#chit.SetGridy(1)
algo = cluster.ClusterParamsExecutor() 

# Let's run it.
#my_proc.run();

fGSer = larutil.GeometryUtilities.GetME()


while mgr.next_event():

 for plane in xrange(larutil.Geometry.GetME().Nplanes()):
	algo.LoadAllHits(mgr.get_data(fmwk.DATA.GausHit), plane)
	algo.GetAverages(True)
        algo.GetRoughAxis(True)
        algo.GetProfileInfo(True)
        algo.RefineDirection(True)
        algo.RefineStartPoints(True)
#        algo.FillPolygon()
        algo.GetFinalSlope(True)
        algo.Report()
        algo.PrintFANNVector()
        result = algo.GetParams()

# done!
print
print "Finished running ana_processor event loop!"
print

sys.exit(0)

