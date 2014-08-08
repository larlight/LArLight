import sys
from ROOT import *

def main() :
       
    #Energy in Pi0 Decay Histogram
    fin     = TFile.Open("DarkPhotonAnalyzed_Beam.root","READ")

    Pi0EnergyHist = fin.Get("BenergyPi0Photons")
        
    OtherEnergyHist = fin.Get("BenergyOtherPhotons")

#    Pi0EnergyHist.Scale(1/(Pi0EnergyHist.Integral()))
#    OtherEnergyHist.Scale(1/(OtherEnergyHist.Integral()))
#    
    Pi0EnergyHist.SetLineColor(kBlue)
    OtherEnergyHist.SetTitle("Photon Energy Distribution in #pi^{0} Decay")
#    
    OtherEnergyHist.SetLineColor(kBlack)
#
    sys.stdin.readline()
    Canvas = TCanvas("PhotonDistance","Photon Distance")
    Canvas.cd()
#
    OtherEnergyHist.GetXaxis().SetTitle("Energy (MeV)")
    
    sys.stdin.readline()
    gStyle.SetOptStat(0)
    OtherEnergyHist.Draw()
    Canvas.Update();

    sys.stdin.readline()
    gStyle.SetOptStat(0)
    Pi0EnergyHist.Draw("sames")
    Canvas.Update();
#
    Line = TLine(20,0,20,100000)
    Line.SetLineColor(kRed)
#
    Line.Draw("sames")


#    N_EnergyHist = fin.Get("Nenergy")
#    N_EnergyHist.SetLineColor(kBlack)
#    N_EnergyHist.SetTitle("Photon Energy Distribution in Nuclear Scattering")
#    N_EnergyHist.GetXaxis().SetTitle("Energy (MeV)")
#    gStyle.SetOptStat(0)
#
#    Canvas = TCanvas("PhotonEnergy","Photon Energy")
#    Canvas.cd() 
#
#    N_EnergyHist.Draw()
##
#    GroupHist = fin.Get("fphotongroup")
#    GroupHist.SetLineColor(kBlack)
#    GroupHist.SetTitle("Number of Photons in 1 cm Vertex Groups")
#    GroupHist.GetXaxis().SetTitle("Photons per Group")
#    gStyle.SetOptStat(0)
#
#    Canvas = TCanvas("PhotonEnergy","Photon Energy")
#    Canvas.cd() 
#
#    GroupHist.Draw()
#

    sys.stdin.readline()
    Canvas.SetLogy(1);
    sys.stdin.readline()
    Canvas.Update();
    sys.stdin.readline()    
    Canvas.Print("Beam_Pi0Energy.gif")

    fin.Close()
    

print __name__
if __name__ == "__main__":
    main()
