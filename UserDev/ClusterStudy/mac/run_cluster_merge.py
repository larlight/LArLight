from ROOT import *
gSystem.Load("libAnalysis")
import sys

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
parser.add_argument("-n", "--num-events",help="Number of events to process")
args = parser.parse_args()

if len(sys.argv) == 1:
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

if args.data_output == None:
    args.data_output = "default_event_output.root"
    print "No event output file selected.  If necessary, output will go to:"
    print "\t"+args.data_output

if args.ana_output == None:
    args.ana_output = "default_ana_output.root"
    print "No ana output file selected.  If necessary, output will go to:"
    print "\t"+args.ana_output


ana_proc = larlight.ana_processor()

if args.verbose:
    ana_proc.set_verbosity(larlight.MSG.DEBUG)

# Not sure what this does
ana_proc.set_io_mode(larlight.storage_manager.BOTH)

# Add the input file.  Not sure if the above takes multiple input files yet
ana_proc.add_input_file(args.source)

# ?
larlight.storage_manager.get().set_in_rootdir("scanner")

# set output file
ana_proc.set_output_file(args.data_output)

# Set destination for ana stuff
ana_proc.set_ana_output_file(args.ana_output)

my_merge_alg = larlight.ClusterMergeAlg()
my_merger = larlight.ClusterMerge()

my_merger.set_mergealg(my_merge_alg)

ana_proc.add_process(my_merge_alg)

ana_proc.add_process(my_merger)

while ana_proc.process_event() and ana_proc.get_process_status() == ana_proc.PROCESSING:
    
    print my_merge_alg.GetMergeTree()

    sys.stdin.readline()

#ana_proc.run()


