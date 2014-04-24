import sys
from ROOT import *
gSystem.Load("libBase")
gSystem.Load("libLArUtil")
gSystem.Load("libClusterRecoUtil")
from ROOT import larlight as fmwk
from ROOT import cluster
from ROOT import clusteralg


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

# chit=TCanvas("chit","chit",600,500)
# chit.SetGridx(1)
# chit.SetGridy(1)
# algo = cluster.ClusterParamsExecutor()

# algo.SetUseHitBlurring(false);

canvas1 = TCanvas("canvas1", "canvas1", 700,400)
canvas1.SetGridx(1)
canvas1.SetGridy(1)

canvas2 = TCanvas("canvas2", "canvas2", 700,400)
canvas2.SetGridx(1)
canvas2.SetGridy(1)

trackSubtractor1 = clusteralg.TrackSubtractorAlg()
trackSubtractor1.Initialize()

trackSubtractor2 = clusteralg.TrackSubtractorAlg()
trackSubtractor2.Initialize()

alg = [trackSubtractor1, trackSubtractor2]
canv = [canvas1, canvas2]

processed_events=0

fGSer = larutil.GeometryUtilities.GetME()

while mgr.next_event():

    # Get event_cluster ... std::vector<larlight::cluster>
    cluster_v = mgr.get_data(fmwk.DATA.CrawlerCluster)

    # Get event_mctruth ... std::vector<larlight::mctruth>
    # mctruth_v = mgr.get_data(fmwk.DATA.MCTruth)

   #  # Get the primary particl generator vtx position
   #  mct_vtx=None
   #  if mctruth_v and mctruth_v.size():
   #      if mctruth_v.size()>1:
   #          print "Found more than 1 MCTruth. Only use the 1st one... \n \n"
   #      if mctruth_v.at(0).GetParticles().at(0).PdgCode() == 11:      ## electron    
   #          mct_vtx = mctruth_v.at(0).GetParticles().at(0).Trajectory().at(0).Position()
   #          print "\n electron \n"
   #      elif mctruth_v.at(0).GetParticles().at(0).PdgCode() == 22:    
   #          trajsize= mctruth_v.at(0).GetParticles().at(0).Trajectory().size()
   #          mct_vtx = mctruth_v.at(0).GetParticles().at(0).Trajectory().at(trajsize-1).Position()
   #          print "\n gamma \n"
   # #PdgCode

    if args.num_events == processed_events:
        exit()
        
    if not cluster_v:
        continue

    print "Event:",cluster_v.event_id()

    allhits = mgr.get_data(larlight.DATA.CrawlerHit)

    passed = list()
    # hitHISTS = list()
    # clustHISTS = list()
    # trackHISTS = list()


    for plane in xrange(larutil.Geometry.GetME().Nplanes()):

        passed.append(False)

        alg[plane].LoadAllHits(allhits, plane)
        alg[plane].LoopClusters(plane, cluster_v, allhits)


        # for x in xrange(cluster_v.size()):

        #     # print "  Cluster ID:",cluster_v.at(x).ID()

        #     alg[plane].AddCluster(plane, cluster_v.at(x), allhits)


        # print "Remaining Hits: %f" % alg[plane].GetRemainingHitsNumber()
        # print "Percent Tracks: %f" % alg[plane].GetPercentTracks()


        # if alg[plane].GetRemainingHitsNumber() > 50:
        #     if alg[plane].GetPercentTracks() < 0.5:
        if alg[plane].FailsCuts() == False:
          passed[-1] = True


    if passed[0] == True and passed[1] == True : 
        for plane in xrange(larutil.Geometry.GetME().Nplanes()):
            alg[plane].FillHists()

            canv[plane].cd()

            # Draw Hit 2D View & points
            allHitsHIST = alg[plane].GetAllHitsHist()
            allHitsHIST.SetMarkerColor(kGray)
            allHitsHIST.SetTitle("Event %d" % cluster_v.event_id())
            allHitsHIST.Draw("COLZ")
            canv[plane].Update()

            sys.stdin.readline()


            clustHitsHIST = alg[plane].GetClustHitsHist()
            clustHitsHIST.SetMarkerColor(kGreen)
            clustHitsHIST.Draw("same")
            canv[plane].Update()

            sys.stdin.readline()

            trackHitsHIST = alg[plane].GetTrackHitsHist()
            trackHitsHIST.SetMarkerColor(kRed)
            trackHitsHIST.Draw("same")



            # Update canvas
            canv[plane].Update()
            sys.stdin.readline()
    else:
        print "This event failed the cuts."
        
        
