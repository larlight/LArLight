import sys,array
from ROOT import *

ch=TChain("reco_tree")
ch.AddFile(sys.argv[1])

#
# We find numbers as a function of cut values.
# We define a set of cut values + container for numbers (produced per cut value)
#

# Cut value arrays
fid_cuts    = [ x*2   for x in xrange(30) ] # 5cm steps, spans from 0=>60 cm
length_cuts = [ x*2   for x in xrange(50) ] # 5cm steps, spans from 0=>100 cm

# Efficiency arrays for first daughter
fid_eff    = [0 for x in fid_cuts]
length_eff = [0 for x in length_cuts]

fid_eff_mc    = [0 for x in fid_cuts]
length_eff_mc = [0 for x in length_cuts]

hTempEC = TH1D("hTempEC","truth - reco distance to wall",100,-70,70)
c=TCanvas("c","",600,500)
c.SetGrid()

#
#ch.Draw("_dist1_AlongTraj - _reco_dist1_AlongTraj>>hTempEC") 
#

for x in xrange(len(fid_cuts)):
    hTempEC.Reset()
    fid_eff[x] = float(ch.GetEntries("_dist_to_wall>=%g" % fid_cuts[x])) / ch.GetEntries()

for x in xrange(len(length_cuts)):
    hTempEC.Reset()
    length_eff[x] = float(ch.GetEntries("_dist_along_traj>=%g" % length_cuts[x])) / ch.GetEntries()


#print ch.GetEntries() 

fid_eff_xvalues = array.array('d', fid_cuts)
fid_eff_yvalues = array.array('d', fid_eff)
fid_eff_graph = TGraph(len(fid_cuts), fid_eff_xvalues, fid_eff_yvalues)
fid_eff_graph.SetMarkerStyle(20)
fid_eff_graph.SetMarkerColor(kBlue)

fid_eff_graph.SetMaximum(1)
fid_eff_graph.SetMinimum(0)
fid_eff_graph.SetTitle("Efficiency vs Fiducial Cut")
fid_eff_graph.GetXaxis().SetTitle("Fiducial Volume Cut")


fid_eff_graph.Draw("AP")
c.Update()
sys.stdin.readline()


length_eff_xvalues = array.array('d', length_cuts)
length_eff_yvalues = array.array('d', length_eff)
length_eff_graph = TGraph(len(length_cuts), length_eff_xvalues, length_eff_yvalues)
length_eff_graph.SetMarkerStyle(20)
length_eff_graph.SetMarkerColor(kBlue)

length_eff_graph.SetMaximum(1)
length_eff_graph.SetMinimum(0)
c.SetGrid()
length_eff_graph.SetTitle("Efficiency vs Length Cut")
length_eff_graph.GetXaxis().SetTitle("Length along shower")

length_eff_graph.Draw("AP")
c.Update()
sys.stdin.readline()
