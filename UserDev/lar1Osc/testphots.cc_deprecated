#ifndef TESTPHOTS_CC
#define TESTPHOTS_CC

#include "testphots.hh"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"
#include "TFile.h"
#include "TLegend.h"
#include "TStyle.h"


void testphots::run()
{

//TCanvas * discrim2=new TCanvas("distance from start2","distance from start2");  
  
         std::cout<<"before opening an output"<<std::endl;
       TFile * output=new TFile("histos_for_corey.root","RECREATE");
       std::cout<<"opened!"<<std::endl;
  
       TDirectory *savedir = gDirectory;
       
TFile          *infile1=new TFile("/uboone/data/users/andrzejs/MC_uboone.root","READ");
TTree          *fCris=(TTree *)infile1->Get("MCTree");    //Cristoph's file
TFile          *infile2=new TFile("/uboone/data/users/andrzejs/ComboNtuples_MC2_Proposal/combined_ntup_processed_nue.root","READ");
// TTree          *fChain=(TTree *)infile2->Get("EventsTot");   // Corey's file

    
    
   // TH1F * electrons =new TH1F("electrons","electrons",150,0,1100);

    TH1F * photons100=new TH1F("photons100","photons100",150,0,1100);
    TH1F * photons200=new TH1F("photons200","photons200",150,0,1100);
    
    TH1F * photons100top=new TH1F("photons100top","photons100top",150,0,1100);
    TH1F * photons200top=new TH1F("photons200top","photons200top",150,0,1100);
    TH1F * photons100bottom=new TH1F("photons100bottom","photons100bottom",150,0,1100);
    TH1F * photons200bottom=new TH1F("photons200bottom","photons200bottom",150,0,1100);
    
    TH1F * photonsYZ100=new TH1F("photons100YZ","photons100YZ",150,0,1100);
    TH1F * photonsYZ200=new TH1F("photons200YZ","photons200YZ",150,0,1100);
    
    TH1F * photonsYZ100top=new TH1F("photons100YZtop","photons100YZtop",150,0,1100);
    TH1F * photonsYZ200top=new TH1F("photons200YZtop","photons200YZtop",150,0,1100);
    TH1F * photonsYZ100bottom=new TH1F("photons100YZbottom","photons100YZbottom",150,0,1100);
    TH1F * photonsYZ200bottom=new TH1F("photons200YZbottom","photons200YZbottom",150,0,1100);

    // #### histograms for Corey
    
      // Default to 120 equal length bins fom 0.2 GeV to 3 GeV:
    std::vector < float > defaultBins;
    double emin = 0.0;
    double emax = 3.0;
    double nbins = 120.0;
    double binSize = (emax - emin)/nbins;
    defaultBins.resize(nbins+1);
    for (double i = 0; i <= nbins; i++) {
      defaultBins.at(i) = emin + i*binSize;
    }
    
    
   TH1F * photons200c=new TH1F("photons200_nocut","photons200_nocut",nbins,&(defaultBins[0]));
   TH1F * photons200cat50=new TH1F("photons200_at50","photons200_at50",nbins,&(defaultBins[0]));
   TH1F * photons200cat100=new TH1F("photons200_at100","photons200_at100",nbins,&(defaultBins[0]));
   
   
    // #### End histograms for Corey
    
    emin = 0.0, emax = 3.0;
    int ebins = 15;
    TH1F * photons100energy=new TH1F("photonsenergy","photonsenergy", 5*ebins,emin,emax);
  //  TH1F * electrons100energy=new TH1F("electronsenergy","electronssenergy",150,0,3000);
    
   
    
    TH2D  * PhotonsvsY =new TH2D("PhotDistToStartvsY","PhotDistToStart;Vertical Position",75,0,1100,24,-120,120);
     TH2D  * PhotonsYZvsY =new TH2D("PhotDistToStartYZvsY","PhotDistToStartYZ;Vertical Position",75,0,1100,24,-120,120);
    //TH2D  * ElectronsvsY =new TH2D("ElectDistToStartvsY","ElectDistToStart;Vertical Position",150,0,1100,48,-120,120);
    
    //Entries involving genie FSP:
    
    
    std::vector<double> ComptonEnergy;
    std::vector<double> *comptE = &ComptonEnergy;
    
    //std::vector<double>  *comptE;
    std::vector<double> ComptonPx;
    std::vector<double>  *comptPx=&ComptonPx;
    std::vector<double> ComptonPy;
    std::vector<double>  *comptPy=&ComptonPy;
    std::vector<double> ComptonPz;
    std::vector<double>  *comptPz=&ComptonPz;
    std::vector<double> Comptonx;
    std::vector<double>  *comptx=&Comptonx;
    std::vector<double> Comptony;
    std::vector<double>  *compty=&Comptony;
    std::vector<double> Comptonz;
    std::vector<double>  *comptz=&Comptonz; 

    
    
//   TBranch *b_comptE;
//     TBranch *b_comptPx;
//     TBranch *b_comptPy;
//     TBranch *b_comptPz; 
//     TBranch *b_comptx;
//     TBranch *b_compty;
//     TBranch *b_comptz; 

    
   
//    fCris->SetBranchAddress("ComptonEnergy",   &comptE, &b_comptE);
//    fCris->SetBranchAddress("ComptonMomentum_x", &comptPx, &b_comptPx);
//    fCris->SetBranchAddress("ComptonMomentum_y", &comptPy, &b_comptPy);
//    fCris->SetBranchAddress("ComptonMomentum_x", &comptPz, &b_comptPz);
//    fCris->SetBranchAddress("ComptonPosition_x", &comptx, &b_comptx);
//    fCris->SetBranchAddress("ComptonPosition_y", &compty, &b_compty);
//    fCris->SetBranchAddress("ComptonPosition_x", &comptz, &b_comptz);
//    
   fCris->SetBranchAddress("ComptonEnergy",   &comptE);
   fCris->SetBranchAddress("ComptonMomentum_x", &comptPx);
   fCris->SetBranchAddress("ComptonMomentum_y", &comptPy);
   fCris->SetBranchAddress("ComptonMomentum_z", &comptPz);
   fCris->SetBranchAddress("ComptonPosition_x", &comptx);
   fCris->SetBranchAddress("ComptonPosition_y", &compty);
   fCris->SetBranchAddress("ComptonPosition_z", &comptz);
   
   
   lar1::Utils utils;
   int iDet=1;
   
   long int NEntries=fCris->GetEntries();
   
   std::cout << "Get Entries: " << NEntries << std::endl;
   
   for(long int i =0;i<NEntries;i++)
   {
    fCris->GetEntry(i); 
   
    int xsize=(*comptE).size();
    
    std::cout << "i, xsize: " << i << " " << xsize << " " << (*comptPx).size() << " " << (*comptPy).size() <<" "<<(*comptPz).size() << " retest" << (*comptPx).size() << std::endl;
    
    std::cout << "i, xsize: " << i << " " << xsize << " " << (*comptx).size() << " " << (*compty).size() <<" "<<(*comptz).size() << std::endl;
    
    
    for(int ix=0;ix<xsize;ix++)
      {
      //(128.175 , 0 , 518.4)
      TVector3 vtx((*comptx)[ix]/10.+128.175, (*compty)[ix]/10.+0, (*comptz)[ix]/10.+518.4);
      // int isFid = utils.IsFiducial( iDet, vtx );
    
      TVector3 lepDir((*comptPx)[ix], (*comptPy)[ix]+0, (*comptPz)[ix]);
      double ElectDistToStart = utils.GetLengthToStart(vtx, lepDir, iDet);
      double ElectDistToStartYZ = utils.GetYZLengthToStart(vtx, lepDir, iDet);
     // if((*comptE)[ix]>200)
      // std::cout << " distances: " << ElectDistToStart << " "<< ElectDistToStartYZ  << std::endl;
    

      
      
      if((*comptE)[ix]>100)
      {
	
/// ### filling histograms for Corey	
       photons200c->Fill((*comptE)[ix]/1000.);
       if(ElectDistToStart>50)	
	  photons200cat50->Fill((*comptE)[ix]/1000.);
       if(ElectDistToStart>100)	
	  photons200cat100->Fill((*comptE)[ix]/1000.);
 /// ### end filling histograms for Corey	      
       
       photons100->Fill(ElectDistToStart);
        photonsYZ100->Fill(ElectDistToStartYZ);
       photons100energy->Fill((*comptE)[ix]/1000.);
       
       if(vtx[1]>=0)
       {
	  photons100top->Fill(ElectDistToStart);
	  photonsYZ100top->Fill(ElectDistToStartYZ);
       }
       else
	photons100bottom->Fill(ElectDistToStart);
        photonsYZ100bottom->Fill(ElectDistToStartYZ);
      }
	  
      if((*comptE)[ix]>200)
      {photons200->Fill(ElectDistToStart);
       
      
      photonsYZ200->Fill(ElectDistToStartYZ);
       if(vtx[1]>=0)
       {
	  photons200top->Fill(ElectDistToStart);
	  photonsYZ200top->Fill(ElectDistToStartYZ);
       }
       else
       {
	 photons200bottom->Fill(ElectDistToStart);
       photonsYZ200bottom->Fill(ElectDistToStartYZ);
       }
       PhotonsvsY->Fill(ElectDistToStart,vtx[1]); 
       PhotonsYZvsY->Fill(ElectDistToStartYZ,vtx[1]); 
      }
      }
    std::cout << " out of inner loop" << std::endl;
   }
     
    std::cout << "really out " << std::endl;
   // TCanvas * discrim=new TCanvas("distance from start","distance from start");
    
    std::cout << "after canvas " << std::endl;
    
    gStyle->SetOptStat(0);
    
    double scale_factor=210/192.;
    photons100->Scale(scale_factor);
    photons200->Scale(scale_factor);
    TH1D * electrons=(TH1D *)infile2->Get("ElectDistToStart");
    electrons->SetLineColor(kBlue);
    electrons->Scale( 0.118/0.23 );
    electrons->SetLineWidth(2);
    electrons->DrawCopy();
    //fChain->SetLineWidth(2);
    //fChain->Draw("ElectDistToStart>>(150,0,1100)","isCC && IsFiducial && inno==12 ");
    std::cout << "after fChain " << std::endl;
    //electrons->DrawCopy();
    photons100->SetLineWidth(2);
    photons200->SetLineWidth(2);
    photons100->SetLineColor(kRed);
    photons100->DrawCopy("same");
    photons200->SetLineColor(kBlack);
    photons200->DrawCopy("same");
    // # Setting the Legend #
// ######################
    TLegend *leg = new TLegend();
    leg = new TLegend(0.58,0.65,0.88,0.88);
    leg->SetTextSize(0.04);
    leg->SetTextAlign(12);
    leg->SetFillColor(kWhite);
    leg->SetLineColor(kWhite);
    leg->SetShadowColor(kWhite);
    leg->SetHeader("Projected Distance from Wall");
    leg->AddEntry(electrons, "#nu_e CC");
    leg->AddEntry(photons100, "compton el; E > 100MeV");
    leg->AddEntry(photons200, "compton el; E > 200MeV");
//leg->AddEntry(hmcEnergyTrue, "MC Truth");
leg->Draw();

    gPad->Print("distance_all.png");
    
    std::cout << "cross - check" << photons200->GetEntries()*scale_factor << std::endl;
    std::cout << " electrons total " << electrons->Integral() << " " << " | photons above 200MeV total" << photons200->Integral() <<  std::endl;
     std::cout << " cuts: @ 0 : electrons" << electrons->Integral(0,150) << " " << "| photons above 200MeV " << photons200->Integral(0,150) <<  std::endl;
    std::cout << " cuts: @ 50  : electrons" << electrons->Integral(7,150) << " " << "| photons above 200MeV " << photons200->Integral(7,150) <<  std::endl;
     std::cout << " cuts: @ 100  : electrons " << electrons->Integral(14,150) << " " << "| photons above 200MeV " << photons200->Integral(14,150) <<  std::endl;
      std::cout << " cuts: @ 150  : electrons " << electrons->Integral(21,150) << " " << "| photons above 200MeV " << photons200->Integral(21,150) <<  std::endl;
    //discrim->Print("test.eps");
   // return;
    /////////////###################################3
    std::cout << "after canvas " << std::endl;
    /////////////###################################3
    
    

    photons100top->Scale(scale_factor);
    photons200top->Scale(scale_factor);
    TH1D * electronstop=(TH1D *)infile2->Get("ElectDistToStartTop");
    electronstop->SetLineColor(kBlue);
    electronstop->Scale( 0.118/0.23 );
    electronstop->SetLineWidth(2);
    electronstop->DrawCopy();
    //fChain->SetLineWidth(2);
    //fChain->Draw("ElectDistToStart>>(150,0,1100)","isCC && IsFiducial && inno==12 ");
    std::cout << "after fChain " << std::endl;
    //electrons->DrawCopy();
    photons100top->SetLineWidth(2);
    photons200top->SetLineWidth(2);
    photons100top->SetLineColor(kRed);
    photons100top->DrawCopy("same");
    photons200top->SetLineColor(kBlack);
    photons200top->DrawCopy("same");
    // # Setting the Legend #
// ######################
    TLegend *legtop = new TLegend();
    legtop = new TLegend(0.58,0.65,0.88,0.88);
    legtop->SetTextSize(0.04);
    legtop->SetTextAlign(12);
    legtop->SetFillColor(kWhite);
    legtop->SetLineColor(kWhite);
    legtop->SetShadowColor(kWhite);
    legtop->SetHeader("Projected Distance from Wall");
    legtop->AddEntry(electronstop, "#nu_e CC in Top of Chamber");
    legtop->AddEntry(photons100top, "compton el; E > 100MeV in Top of Chamber");
    legtop->AddEntry(photons200top, "compton el; E > 200MeV in Top of Chamber");
//leg->AddEntry(hmcEnergyTrue, "MC Truth");
    legtop->Draw();
    
     gPad->Print("distance_all_top.png");
    
     std::cout << " electrons top  " << electronstop->Integral() << " " << "photons above 200MeV " << photons200top->GetEntries()*210/192.1 <<  std::endl;
    
      std::cout << "cross - check" << photons200top->GetEntries()*scale_factor << std::endl;
    std::cout << " electrons: top  " << electronstop->Integral() << " " << "| photons above 200MeV " << photons200top->Integral() <<  std::endl;
     std::cout << " cuts:  @ 0| electrons: " << electronstop->Integral(0,150) << " " << "| photons above 200MeV " << photons200top->Integral(0,150) <<  std::endl;
    std::cout << " cuts: @ 50| electrons: " << electronstop->Integral(7,150) << " " << "| photons above 200MeV " << photons200top->Integral(7,150) <<  std::endl;
     std::cout << " cuts: @ 100| electrons: " << electronstop->Integral(14,150) << " " << "| photons above 200MeV " << photons200top->Integral(14,150) <<  std::endl;
      std::cout << " cuts: @ 150| electrons: " << electronstop->Integral(21,150) << " " << "| photons above 200MeV " << photons200top->Integral(21,150) <<  std::endl;
     
     
     
     /////////////###################################3
    std::cout << "after canvas " << std::endl;
    /////////////###################################3
    
    

    photons100bottom->Scale(scale_factor);
    photons200bottom->Scale(scale_factor);
    TH1D * electronsbottom=(TH1D *)infile2->Get("ElectDistToStartBottom");
    electronsbottom->SetLineColor(kBlue);
    electronsbottom->Scale( 0.118/0.23 );
    electronsbottom->SetLineWidth(2);
    electronsbottom->DrawCopy();
    //fChain->SetLineWidth(2);
    //fChain->Draw("ElectDistToStart>>(150,0,1100)","isCC && IsFiducial && inno==12 ");
    std::cout << "after fChain " << std::endl;
    //electrons->DrawCopy();
    photons100bottom->SetLineWidth(2);
    photons200bottom->SetLineWidth(2);
    photons100bottom->SetLineColor(kRed);
    photons100bottom->DrawCopy("same");
    photons200bottom->SetLineColor(kBlack);
    photons200bottom->DrawCopy("same");
    // # Setting the Legend #
// ######################
    TLegend *legbottom = new TLegend();
    legbottom = new TLegend(0.58,0.65,0.88,0.88);
    legbottom->SetTextSize(0.04);
    legbottom->SetTextAlign(12);
    legbottom->SetFillColor(kWhite);
    legbottom->SetLineColor(kWhite);
    legbottom->SetShadowColor(kWhite);
    legbottom->SetHeader("Projected Distance from Wall");
    legbottom->AddEntry(electronsbottom, "#nu_e CC in Bottom of Chamber");
    legbottom->AddEntry(photons100bottom, "compton el; E > 100MeV in Bottom of Chamber");
    legbottom->AddEntry(photons200bottom, "compton el; E > 200MeV in Bottom of Chamber");
//leg->AddEntry(hmcEnergyTrue, "MC Truth");
    legbottom->Draw();
    
     std::cout << " electrons bottom  " << electronsbottom->Integral() << " " << "photons above 200MeV " << photons200bottom->GetEntries()*210/192.1 <<  std::endl;
     
      std::cout << "cross - check" << photons200bottom->GetEntries()*scale_factor << std::endl;
     std::cout << " electronsbottom  " << electronsbottom->Integral() << " " << "| photons above 200MeV " <<  photons200bottom->Integral() <<  std::endl;
     std::cout << " cuts: @ 0 | electrons :" << electronsbottom->Integral(0,150) << " " << "| photons above 200MeV " << photons200bottom->Integral(0,150) <<  std::endl;
    std::cout << " cuts: @ 50 | electrons :" << electronsbottom->Integral(7,150) << " " << "| photons above 200MeV " << photons200bottom->Integral(7,150) <<  std::endl;
     std::cout << " cuts: @ 100 | electrons :" << electronsbottom->Integral(14,150) << " " << "| photons above 200MeV " <<  photons200bottom->Integral(14,150) <<  std::endl;
      std::cout << " cuts: @ 150 | electrons :" << electronsbottom->Integral(21,150) << " " << "| photons above 200MeV " <<  photons200bottom->Integral(21,150) <<  std::endl;
     
       gPad->Print("distance_all_bottom.png");
      
     //############################## YZ distance
     //
     // ################################################################3 
      
      
        photonsYZ100->Scale(scale_factor);
    photonsYZ200->Scale(scale_factor);
    TH1D * electronsYZ=(TH1D *)infile2->Get("ElectDistToStartYZ");
    electronsYZ->SetLineColor(kBlue);
    electronsYZ->Scale( 0.118/0.23 );
    electronsYZ->SetLineWidth(2);
    electronsYZ->DrawCopy();
    //fChain->SetLineWidth(2);
    //fChain->Draw("ElectDistToStart>>(150,0,1100)","isCC && IsFiducial && inno==12 ");
    std::cout << "after fChain " << std::endl;
    //electronsYZ->DrawCopy();
    photonsYZ100->SetLineWidth(2);
    photonsYZ200->SetLineWidth(2);
    photonsYZ100->SetLineColor(kRed);
    photonsYZ100->DrawCopy("same");
    photonsYZ200->SetLineColor(kBlack);
    photonsYZ200->DrawCopy("same");
    // # Setting the Legend #
// ######################
    TLegend *legYZ = new TLegend();
    legYZ = new TLegend(0.58,0.65,0.88,0.88);
    legYZ->SetTextSize(0.04);
    legYZ->SetTextAlign(12);
    legYZ->SetFillColor(kWhite);
    legYZ->SetLineColor(kWhite);
    legYZ->SetShadowColor(kWhite);
    legYZ->SetHeader("Projected Distance from Wall");
    legYZ->AddEntry(electronsYZ, "#nu_e CC");
    legYZ->AddEntry(photonsYZ100, "compton el; E > 100MeV");
    legYZ->AddEntry(photonsYZ200, "compton el; E > 200MeV");
//legYZ->AddEntry(hmcEnergyTrue, "MC Truth");
legYZ->Draw();

 gPad->Print("distance_all_YZ.png");

    std::cout << "cross - check" << photonsYZ200->GetEntries()*scale_factor << std::endl;
    std::cout << " electronsYZ  " << electronsYZ->Integral() << " " << "| photons above 200MeV " << photonsYZ200->Integral() <<  std::endl;
     std::cout << " cuts: @ 0 | electrons:  " << electronsYZ->Integral(0,150) << " " << "| photons above 200MeV " << photonsYZ200->Integral(0,150) <<  std::endl;
    std::cout << " cuts: @ 50 | electrons:  " << electronsYZ->Integral(7,150) << " " << "| photons above 200MeV " << photonsYZ200->Integral(7,150) <<  std::endl;
     std::cout << " cuts: @ 100 | electrons:  " << electronsYZ->Integral(14,150) << " " << "| photons above 200MeV " << photonsYZ200->Integral(14,150) <<  std::endl;
      std::cout << " cuts: @ 150 | electrons:  " << electronsYZ->Integral(21,150) << " " << "| photons above 200MeV " << photonsYZ200->Integral(21,150) <<  std::endl;
    //discrim->Print("test.eps");
   // return;
    /////////////###################################3
    std::cout << "after canvas " << std::endl;
    /////////////###################################3
    
    

    photonsYZ100top->Scale(scale_factor);
    photonsYZ200top->Scale(scale_factor);
    TH1D * electronsYZtop=(TH1D *)infile2->Get("ElectDistToStartYZTop");
    electronsYZtop->SetLineColor(kBlue);
    electronsYZtop->Scale( 0.118/0.23 );
    electronsYZtop->SetLineWidth(2);
    electronsYZtop->DrawCopy();
    //fChain->SetLineWidth(2);
    //fChain->Draw("ElectDistToStart>>(150,0,1100)","isCC && IsFiducial && inno==12 ");
    std::cout << "after fChain " << std::endl;
    //electronsYZ->DrawCopy();
    photonsYZ100top->SetLineWidth(2);
    photonsYZ200top->SetLineWidth(2);
    photonsYZ100top->SetLineColor(kRed);
    photonsYZ100top->DrawCopy("same");
    photonsYZ200top->SetLineColor(kBlack);
    photonsYZ200top->DrawCopy("same");
    // # Setting the Legend #
// ######################
    TLegend *legYZtop = new TLegend();
    legYZtop = new TLegend(0.58,0.65,0.88,0.88);
    legYZtop->SetTextSize(0.04);
    legYZtop->SetTextAlign(12);
    legYZtop->SetFillColor(kWhite);
    legYZtop->SetLineColor(kWhite);
    legYZtop->SetShadowColor(kWhite);
    legYZtop->SetHeader("Projected Distance from Wall");
    legYZtop->AddEntry(electronsYZtop, "#nu_e CC in Top of Chamber");
    legYZtop->AddEntry(photonsYZ100top, "compton el; E > 100MeV in Top of Chamber");
    legYZtop->AddEntry(photonsYZ200top, "compton el; E > 200MeV in Top of Chamber");
//legYZ->AddEntry(hmcEnergyTrue, "MC Truth");
    legYZtop->Draw();
    
     gPad->Print("distance_all_YZ_top.png");
    
     std::cout << " electronsYZ top  " << electronsYZtop->Integral() << " " << "photons above 200MeV " << photonsYZ200top->GetEntries()*210/192.1 <<  std::endl;
    
      std::cout << "cross - check" << photonsYZ200top->GetEntries()*scale_factor << std::endl;
    std::cout << " electronsYZtop  " << electronsYZtop->Integral() << " " << " | photons above 200MeV " << photonsYZ200top->Integral() <<  std::endl;
     std::cout << " cuts: @ 0 | electrons:  " << electronsYZtop->Integral(0,150) << " " << " | photons above 200MeV " << photonsYZ200top->Integral(0,150) <<  std::endl;
    std::cout << " cuts: @ 50 | electrons:  " << electronsYZtop->Integral(7,150) << " " << " | photons above 200MeV " << photonsYZ200top->Integral(7,150) <<  std::endl;
     std::cout << " cuts: @ 100 | electrons:  " << electronsYZtop->Integral(14,150) << " " << " | photons above 200MeV " << photonsYZ200top->Integral(14,150) <<  std::endl;
      std::cout << " cuts: @ 150 | electrons:  " << electronsYZtop->Integral(21,150) << " " << " | photons above 200MeV " << photonsYZ200top->Integral(21,150) <<  std::endl;
     
     
     
     /////////////###################################3
    std::cout << "after canvas " << std::endl;
    /////////////###################################3
    
    

    photonsYZ100bottom->Scale(scale_factor);
    photonsYZ200bottom->Scale(scale_factor);
    TH1D * electronsYZbottom=(TH1D *)infile2->Get("ElectDistToStartYZBottom");
    electronsYZbottom->SetLineColor(kBlue);
    electronsYZbottom->Scale( 0.118/0.23 );
    electronsYZbottom->SetLineWidth(2);
    electronsYZbottom->DrawCopy();
    //fChain->SetLineWidth(2);
    //fChain->Draw("ElectDistToStart>>(150,0,1100)","isCC && IsFiducial && inno==12 ");
    std::cout << "after fChain " << std::endl;
    //electronsYZ->DrawCopy();
    photonsYZ100bottom->SetLineWidth(2);
    photonsYZ200bottom->SetLineWidth(2);
    photonsYZ100bottom->SetLineColor(kRed);
    photonsYZ100bottom->DrawCopy("same");
    photonsYZ200bottom->SetLineColor(kBlack);
    photonsYZ200bottom->DrawCopy("same");
    // # Setting the Legend #
// ######################
    TLegend *legYZbottom = new TLegend();
    legYZbottom = new TLegend(0.58,0.65,0.88,0.88);
    legYZbottom->SetTextSize(0.04);
    legYZbottom->SetTextAlign(12);
    legYZbottom->SetFillColor(kWhite);
    legYZbottom->SetLineColor(kWhite);
    legYZbottom->SetShadowColor(kWhite);
    legYZbottom->SetHeader("Projected Distance from Wall");
    legYZbottom->AddEntry(electronsYZbottom, "#nu_e CC in Bottom of Chamber");
    legYZbottom->AddEntry(photonsYZ100bottom, "compton el; E > 100MeV in Bottom of Chamber");
    legYZbottom->AddEntry(photonsYZ200bottom, "compton el; E > 200MeV in Bottom of Chamber");
//legYZ->AddEntry(hmcEnergyTrue, "MC Truth");
    legYZbottom->Draw();
    
     gPad->Print("distance_all_bottom_YZ.png");
    
     std::cout << " electronsYZ bottom  " << electronsYZbottom->Integral() << " " << "photons above 200MeV " << photonsYZ200bottom->GetEntries()*210/192.1 <<  std::endl;
     
      std::cout << "cross - check" << photonsYZ200bottom->GetEntries()*scale_factor << std::endl;
    std::cout << " electronsYZbottom  " << electronsYZbottom->Integral() << " " << " | photons above 200MeV " << photonsYZ200bottom->Integral() <<  std::endl;
     std::cout << " cuts: @ 0 | electrons:  " << electronsYZbottom->Integral(0,150) << " " << " | photons above 200MeV " << photonsYZ200bottom->Integral(0,150) <<  std::endl;
    std::cout << " cuts: @ 50 | electrons:  " << electronsYZbottom->Integral(7,150) << " " << " | photons above 200MeV " << photonsYZ200bottom->Integral(7,150) <<  std::endl;
     std::cout << " cuts: @ 100 | electrons:  " << electronsYZbottom->Integral(14,150) << " " << " | photons above 200MeV " << photonsYZ200bottom->Integral(14,150) <<  std::endl;
      std::cout << " cuts: @ 150 | electrons:  " << electronsYZbottom->Integral(21,150) << " " << " | photons above 200MeV " << photonsYZ200bottom->Integral(21,150) <<  std::endl;
     
// ########################### After distance plots
// ########################### After distance plots
      
     
   //   TCanvas * electrons2D=new TCanvas("electrons2d","electrons2d");
      
      TH2D * elecsvsY = (TH2D *)infile2->Get("ElectDistToStartvsY");
      elecsvsY->DrawCopy("COLZ");
      
       gPad->Print("electrons_vs_Y.png");
      
     // TCanvas * photons2D=new TCanvas("photons2d","photons2d");
      
      PhotonsvsY->DrawCopy("COLZ");
      
      gPad->Print("photons_vs_Y.png");
      
      
      TH2D * elecsYZvsY = (TH2D *)infile2->Get("ElectDistToStartYZvsY");
      elecsYZvsY->DrawCopy("COLZ");
      
       gPad->Print("electronsYZ_vs_Y.png");
      
     // TCanvas * photons2D=new TCanvas("photons2d","photons2d");
      
      PhotonsYZvsY->DrawCopy("COLZ");
      
      gPad->Print("photonsYZ_vs_Y.png");
      
      
      
      
     // TCanvas * Energies=new TCanvas("Energies","Energies");
      photons100energy->SetLineColor(kRed);
      photons100energy->SetLineWidth(2);
      photons100energy->DrawCopy();
      TH1D * electronsenergy=(TH1D *)infile2->Get("LepTrueEnergy");
      electronsenergy->SetLineWidth(2);
      electronsenergy->DrawCopy("same");
      
       gPad->Print("energies.png");
      
     //  infile1->Close();
     //  infile2->Close();

       savedir->cd();
       
       photons200c->Write();
       std::cout<<"wrote 1"<<std::endl;
       photons200cat50->Write();
       std::cout<<"wrote 2"<<std::endl;
       photons200cat100->Write();
       std::cout<<"wrote 3"<<std::endl;
       
       output->Close();
       std::cout<<"closed"<<std::endl;
       
  // infile1->Close();
  // infile2->Close();
   
}
    

#endif