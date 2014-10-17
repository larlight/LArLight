import sys,array
from ROOT import *

ch=TChain("ana_tree")
ch.AddFile(sys.argv[1])

# Cut value arrays
length_cuts = [ x*2   for x in xrange(50) ] # 5cm steps, spans from 0=>100 cm

# Efficiency arrays for first daughter
length_eff = [0 for x in length_cuts]
length_feff = [0 for x in length_cuts]

hTempEC = TH1D("hTempEC","truth - reco distance to wall",100,-70,70)
c=TCanvas("c","",600,500)
c.SetGrid()

for x in xrange(len(length_cuts)):
    hTempEC.Reset()
    length_eff[x] = float(ch.GetEntries("_dist_BackAlongTraj>=%g" % length_cuts[x])) / ch.GetEntries()
    length_feff[x] = float(ch.GetEntries("_dist_AlongTraj>=%g" % length_cuts[x])) / ch.GetEntries() 


#print ch.GetEntries() 

length_eff_xvalues = array.array('d', length_cuts)
length_eff_yvalues = array.array('d', length_eff)
length_eff_graph = TGraph(len(length_cuts), length_eff_xvalues, length_eff_yvalues)
length_eff_graph.SetMarkerStyle(20)
length_eff_graph.SetMarkerColor(kBlue)

length_feff_yvalues = array.array('d', length_feff)
length_feff_graph = TGraph(len(length_cuts), length_eff_xvalues, length_feff_yvalues)
length_feff_graph.SetMarkerStyle(20)
length_feff_graph.SetMarkerColor(kGreen+2)

length_eff_graph.SetMaximum(1)
length_eff_graph.SetMinimum(0.6)
c.SetGrid()
length_eff_graph.SetTitle("Efficiency vs Length Forward(green) and Backward(blue) Trajectory Cut")
length_eff_graph.GetXaxis().SetTitle("Length Along Shower")

length_eff_graph.Draw("AP")
length_feff_graph.Draw("P")


c.Update()
sys.stdin.readline()
