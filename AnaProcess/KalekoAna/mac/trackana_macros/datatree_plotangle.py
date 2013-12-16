import ROOT
from ROOT import *


angles = [ 0, 10, 20, 30, 40, 50, 60, 70, 80, 90 ]

#no statistics box
gStyle.SetOptStat(0)

c = TCanvas()
c.SetLogy()
    
for x in xrange(len(angles)):
    
    f = TFile("DataTree_v4_ang%d.root" % angles[x],"READ")
    tree=f.Get("tree")
    
    myhist = TH1F("myhist","",100,0,180)
    myhist.SetLineColor(kBlack)
    myhist.SetFillColor(kGray)
    myhist.SetFillStyle(3002)
    mchist = TH1F("mchist",";ThetaYZ (ThetaXZ=0);Counts",100,0,180)
    mchist.SetFillColor(kRed)
    mchist.SetFillStyle(3001)
    tree.Draw("myMCTheta>>mchist")
    #tree.Draw("myRecoTrackAngle>>myhist","","same")
    tree.Draw("myRecoTrackAngle>>myhist")

    mchist.Draw()
    myhist.Draw("same")

    text=TText(100,2500,"ThetaYZ=%d"%angles[x])
    text.SetTextSize(0.1)
    text.Draw()


    c.Update(); c.Modified()
    c.SaveAs("angle%d.png" % angles[x])
    f.Close()
