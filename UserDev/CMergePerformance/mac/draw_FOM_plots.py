from ROOT import *
import sys, os

if len(sys.argv) != 2:
    print
    print " Improper usage. python draw_FOM_plots.py file_with_histos.root "
    print
    quit()

print "Making out_pngs folder if doesn't currently exist."
os.system("mkdir out_pngs")

filename = sys.argv[1]

f = TFile(filename)
c = TCanvas("c","c",600,800)
c.Divide(1,3)


hEff_after_merging_view0.SetLineColor(kRed)
hEff_after_merging_view1.SetLineColor(kRed)
hEff_after_merging_view2.SetLineColor(kRed)
hPurity_after_merging_view0.SetLineColor(kRed)
hPurity_after_merging_view1.SetLineColor(kRed)
hPurity_after_merging_view2.SetLineColor(kRed)
hClusQoverMCQ_after_merging_view0.SetLineColor(kRed)
hClusQoverMCQ_after_merging_view1.SetLineColor(kRed)
hClusQoverMCQ_after_merging_view2.SetLineColor(kRed)

gStyle.SetOptStat(111111)

img = TImage.Create()


c.cd(1)
hEff_before_merging_view0.Draw()
hEff_after_merging_view0.Draw("sames")
c.cd(2)
hEff_before_merging_view1.Draw()
hEff_after_merging_view1.Draw("sames")
c.cd(3)
hEff_before_merging_view2.Draw()
hEff_after_merging_view2.Draw("sames")
c.Update()
print "Make whatever canvas changes you want, then hit enter to save this as Efficiency_%s.png"%filename
sys.stdin.readline()
img.FromPad(c)
img.WriteImage("out_pngs/Efficiency_%s.png"%filename[:-5])


c.cd(1)
gPad.SetLogy()
hPurity_before_merging_view0.Draw()
hPurity_after_merging_view0.Draw("sames")
c.cd(2)
gPad.SetLogy()
hPurity_before_merging_view1.Draw()
hPurity_after_merging_view1.Draw("sames")
c.cd(3)
gPad.SetLogy()
hPurity_before_merging_view2.Draw()
hPurity_after_merging_view2.Draw("sames")
c.Update()
print "Make whatever canvas changes you want, then hit enter to save this as Purity_%s.png"%filename
sys.stdin.readline()
img.FromPad(c)
img.WriteImage("out_pngs/Purity_%s.png"%filename[:-5])



c.cd(1)
hClusQoverMCQ_before_merging_view0.Draw()
hClusQoverMCQ_after_merging_view0.Draw("sames")
c.cd(2)
hClusQoverMCQ_before_merging_view1.Draw()
hClusQoverMCQ_after_merging_view1.Draw("sames")
c.cd(3)
hClusQoverMCQ_before_merging_view2.Draw()
hClusQoverMCQ_after_merging_view2.Draw("sames")
c.Update()
print "Make whatever canvas changes you want, then hit enter to save this as OtherPurity_%s.png"%filename
sys.stdin.readline()
img = TImage.Create()
img.FromPad(c)
img.WriteImage("out_pngs/OtherPurity_%s.png"%filename[:-5])

print "Donezo."
