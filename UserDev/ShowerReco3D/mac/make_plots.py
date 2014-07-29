from ROOT import *
import sys

def make_plots():
    
    elecs=TFile.Open("combined_electrons_area.root","READ");
    gammas=TFile.Open("combined_gammas_area.root","READ");


    tels=elecs.Get("shower_tree");
    tels.SetName("elshower");

    tgams=gammas.Get("shower_tree");
    tgams.SetName("gamshower");

    #
    # MC Tree
    #

    mctels=elecs.Get("mctruth_tree");
    mctels.SetName("elmc");

    mctgams=gammas.Get("mctruth_tree");
    mctgams.SetName("gammc");

    tels.AddFriend("elmc");
    tgams.AddFriend("gammc");


    c1 =TCanvas(" dE/dx plot ","dE/dx plot");

    tels.SetLineColor(kBlue);
    tels.SetLineWidth(2);
    tels.Draw("fdEdx[2]>>(60,0,12)","fdEdx[2]>0");


    tgams.SetLineColor(kRed);
    tgams.SetLineWidth(2);
    tgams.Draw("fdEdx[2]","fdEdx[2]>0","same");


    
   
    hMCX = TH1D("hMCX","",100,-120,120);
    hMCY = TH1D("hMCY","",100,-120,120);
    hMCZ = TH1D("hMCZ","",100,-120,120);
    
    hdcosX = TH1D("hdcosX","",100,-2.1,2.1);
    hdcosY = TH1D("hdcosY","",100,-2.1,2.1);
    hdcosZ = TH1D("hdcosZ","",100,-2.1,2.1);
    
    hEner = TH1D("hEner","",100,-1000,1000);
    MChEner = TH1D("MChEner","",100,-10,1100);
    recohEner = TH1D("recohEner","",100,-10,1100);
    
    heldEdx = TH1D("heldEdx","",60,0,12);
    hgamdEdx = TH1D("hgamdEdx","",60,0,12);
    
    for x in xrange(tels.GetEntries()):

	tels.GetEntry(x)
        
        #MCTruth section
        
        mc_vtx = tels.mctruth_branch.at(0).GetParticles().at(0).Trajectory().at(0).Position()

        lep_dcosx_truth = tels.mctruth_branch.at(0).GetParticles().at(0).Trajectory().at(0).Px()/tels.mctruth_branch.at(0).GetParticles().at(0).Trajectory().at(0).E();
        lep_dcosy_truth = tels.mctruth_branch.at(0).GetParticles().at(0).Trajectory().at(0).Py()/tels.mctruth_branch.at(0).GetParticles().at(0).Trajectory().at(0).E();
	lep_dcosz_truth = tels.mctruth_branch.at(0).GetParticles().at(0).Trajectory().at(0).Pz()/tels.mctruth_branch.at(0).GetParticles().at(0).Trajectory().at(0).E();
        
        mctenergy = tels.mctruth_branch.at(0).GetParticles().at(0).Trajectory().at(0).E()
        
        fMCPhistart= 0.0 
        fMCThetastart=  0.0
        if (not(lep_dcosx_truth == 0.0 and lep_dcosz_truth == 0.0)) :
	  fMCPhistart=TMath.ATan2(lep_dcosx_truth,lep_dcosz_truth)
	if  (not(lep_dcosx_truth == 0.0 and lep_dcosy_truth == 0.0 and lep_dcosz_truth == 0.0)) :
	  fMCThetastart = TMath.Pi()*0.5-TMath.ATan2(TMath.Sqrt(lep_dcosx_truth*lep_dcosx_truth + lep_dcosz_truth*lep_dcosz_truth),lep_dcosy_truth)        
        
        
        if tels.shower_branch.size() > 0 :
	  recovtx = tels.shower_branch.at(0).ShowerStart()
	  recodcos = tels.shower_branch.at(0).Direction()
	  recoen = tels.shower_branch.at(0).Energy().at(2)
	  if tels.mctruth_branch.at(0).GetParticles().at(0).Trajectory().at(0).E() > 0.200 :
	    heldEdx.Fill(tels.shower_branch.at(0).dEdx().at(2))
	else :
	  continue
        #print mc_vtx;

        hMCX.Fill(mc_vtx[0]-recovtx[0])
        hMCY.Fill(mc_vtx[1]-recovtx[1])
        hMCZ.Fill(mc_vtx[2]-recovtx[2])

        hdcosX.Fill(lep_dcosx_truth-recodcos[0])
        hdcosY.Fill(lep_dcosy_truth-recodcos[1])
        hdcosZ.Fill(lep_dcosz_truth-recodcos[2])
        
        hEner.Fill(mctenergy - recoen)
        MChEner.Fill(mctenergy*1000)
        recohEner.Fill(recoen)
        #hPhi.Fill(FMCPhistart - )
    
    for x in xrange(tgams.GetEntries()):

	tgams.GetEntry(x)
	if tgams.shower_branch.size() > 0 :
	   if tgams.mctruth_branch.at(0).GetParticles().at(0).Trajectory().at(0).E() > 0.200 :
	      hgamdEdx.Fill(tgams.shower_branch.at(0).dEdx().at(2))
	else :
	  continue
    
    
    cx=TCanvas(" x_reco ","x_reco");   
    hMCX.SetLineWidth(2)
    hMCX.DrawCopy()    
    cy=TCanvas(" y_reco ","y_reco");    
    hMCY.SetLineWidth(2)
    hMCY.DrawCopy()    
    cz=TCanvas(" z_reco ","z_reco");    
    hMCZ.SetLineWidth(2)
    hMCZ.DrawCopy()    
    
    cdx=TCanvas(" dirx_reco ","dirx_reco");    
    hdcosX.SetLineWidth(2)
    hdcosX.DrawCopy()    
    cdy=TCanvas(" diry_reco ","diry_reco");  
    hdcosY.SetLineWidth(2)
    hdcosY.DrawCopy()    
    cdz=TCanvas(" dirz_reco ","dirz_reco");    
    hdcosZ.SetLineWidth(2)
    hdcosZ.DrawCopy()    
  
    cdener=TCanvas(" ener_reco ","ener_reco");    
    hEner.DrawCopy()    
  
    cdenernew=TCanvas(" ener_reco and MC","ener_reco and MC");    
    MChEner.DrawCopy()    
    recohEner.SetLineColor(kRed)
    recohEner.DrawCopy("same")    
    
    
    cdedx=TCanvas("new dEdx","new dEdx");    
    heldEdx.SetLineColor(kBlue)
    hgamdEdx.SetLineColor(kRed)
    heldEdx.SetLineWidth(2)
    heldEdx.DrawCopy()
    hgamdEdx.SetLineWidth(2)
    hgamdEdx.DrawCopy("same")
    
    sys.stdin.readline()
    
    
if __name__ == '__main__':
    make_plots()    
    

    
