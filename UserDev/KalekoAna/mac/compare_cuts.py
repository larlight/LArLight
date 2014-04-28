import sys
import ROOT
from ROOT import *

f = TFile("qca_out_00.root")
t = ana_tree;

keep_going = true;

while keep_going:
    param = input("Enter parameter to investigate in quotes (ttree variables): ")

    t.Draw(param,"","")

    h_nocut=htemp.Clone("h_nocut")

    del htemp

    t.Draw(param,"dk_good_clus==1","")

    h_mccut=htemp.Clone("h_mccut")

    del htemp

    c = TCanvas("c","canvas",600,400)

    h_nocut.SetLineColor(kBlue)
    h_mccut.SetLineColor(kRed)
    c.cd()
    h_nocut.Draw()
    h_mccut.Draw("sames")

    reco_cut = input("enter in a reco cut and see how it affects the blue curve: ")

    c1.cd()
    t.Draw(param,reco_cut,"")

    h_recocut = htemp.Clone("h_recocut")
    #c1.Close()
    del htemp
    c.cd()
    h_recocut.SetLineColor(kGreen)
    h_recocut.Draw("sames")
    c.Update()

    keep_going = input("enter 1 to continue, 0 to stop :")
