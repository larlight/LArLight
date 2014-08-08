import sys
from ROOT import *
gSystem.Load("libCoreyShowerStudy")
from ROOT import larlight as fmwk
from ROOT import shower


import argparse

parser = argparse.ArgumentParser(description="Python script to process and merge showers.")
group = parser.add_mutually_exclusive_group()
group.add_argument("-v", "--verbose", help="Turn on verbose output",
                    action="store_true")
group.add_argument("-q", "--quiet", help="Turn off most output",
                    action="store_true")
parser.add_argument("-s","--source",nargs='*',help="Name of input file")
parser.add_argument("-o","--data-output",help="Output data file, if event is changed")
parser.add_argument("-a","--ana-output",help="Analysis output file")
parser.add_argument("-n", "--num-events",help="Number of events to process")
parser.add_argument("-d","--display",help="Turn on the display to see each view before and after." )
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


# ana_proc = larlight.ana_processor()

# if args.verbose:
    # ana_proc.set_verbosity(larlight.MSG.DEBUG)

# Not sure what this does
# ana_proc.set_io_mode(larlight.storage_manager.BOTH)

# Add the input file.  Not sure if the above takes multiple input files yet
# ana_proc.add_input_file(args.source)

mgr = fmwk.storage_manager()

mgr.set_io_mode(mgr.READ)

for source in args.source:
    mgr.add_in_filename(source)
    
if len(sys.argv) > 2:
    mgr.set_in_rootdir("scanner")

mgr.open()

chit=TCanvas("chit","chit",600,500)
chit.SetGridx(1)
chit.SetGridy(1)

algo = shower.CoreyShowerStudy()

i = 0

while mgr.next_event():

    # if i > :
        # break
    # i = i+1
    showers = mgr.get_data(fmwk.DATA.MCShower)
    algo.LoadAllShowers(showers)
    hist = algo.GetChargeProfile()
    hist.Draw()
    chit.Update()
    # sys.stdin.readline()

canv50 = TCanvas("canv50","canv50", 600,500)
canv75 = TCanvas("canv75","canv75", 600,500)
canv90 = TCanvas("canv90","canv90", 600,500)

canv50.cd()
hist50 = algo.GetContain50VsEnergy()
hist50.GetXaxis().SetTitle("Shower Energy (GeV)")
hist50.GetYaxis().SetTitle("Distance (cm)")
hist50.SetTitle("Shower Energy vs. 50 Containment Length")
hist50.Draw("COLZ")
canv50.Update()
canv50.Print("containment50VsEnergy.pdf");
canv75.cd()
hist75 = algo.GetContain75VsEnergy()
hist75.GetXaxis().SetTitle("Shower Energy (GeV)")
hist75.GetYaxis().SetTitle("Distance (cm)")
hist75.SetTitle("Shower Energy vs. 75 Containment Length")
hist75.Draw("COLZ")
canv75.Update()
canv75.Print("containment75VsEnergy.pdf");
canv90.cd()
hist90 = algo.GetContain90VsEnergy()
hist90.GetXaxis().SetTitle("Shower Energy (GeV)")
hist90.GetYaxis().SetTitle("Distance (cm)")
hist90.SetTitle("Shower Energy vs. 90 Containment Length")
hist90.Draw("COLZ")
canv90.Update()
canv90.Print("containment90VsEnergy.pdf");

sys.stdin.readline()
