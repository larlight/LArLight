#include <sys/stat.h>
#include <unistd.h>
#include "TFile.h"
#include "TH1F.h"
#include "TString.h"
#include "TGraph.h"
#include "TGaxis.h"
#include "TNtuple.h"
#include "TChain.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TMatrix.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TH2D.h"
#include "TLegend.h"
#include "THStack.h"
#include "TImage.h"
#include "TMarker.h"
#include "TLatex.h"
#include <vector>
#include <iostream>
#include <ostream>
#include <sstream>
#include <fstream>



int multiple_detector_fit()
{

  //  std::cout << "Beginning : ... " << std::endl;

  Int_t npoints = 1000;
  Double_t emin = 0.2; 
  Double_t emax = 3.0;

  bool use100m = true;
  bool use470m = true;
  bool use600m = true;


  std::vector<int> baselines;
  std::vector<double> scales;
  std::vector<std::string> names;
  std::vector<double> volume;

  if (use100m) baselines.push_back(100);
  if (use470m) baselines.push_back(470);
  if (use600m) baselines.push_back(600);


  double NULLVec[2][55];
  double OscVec[2][1001][7][55];

  int nbinsE = 0;

  if (use100m){

    std::string temp_name = "/Users/jzennamo/Desktop/LArLite/Files/ErrorMatrix/combined_ntuple_100m_nu_processed_nue.root";

    TFile temp_file(temp_name.c_str());
    TH1D *NULL_100;
    NULL_100 = (TH1D*)(temp_file.Get("NumuCC"));
    NULL_100->Rebin(5);
    nbinsE = NULL_100->GetNbinsX();
    for(int i = 0; i <= nbinsE; i++){
      NULLVec[0][i] = (NULL_100->GetBinContent(i));
    }

    for(int u = 0; u < npoints; u++){
      for(int s = 0; s < 7; s++){
	TH1D *OSC_100;
	TString upoint = Form("%d",u);//std::to_string(u);
	TString name = "Universe_";
	TString name2 = "_MultiSim_";
	TString mul = Form("%d",s);// = std::to_string(s);
	
	name += upoint;
	name += name2;
	name += mul;	
       
	OSC_100 = (TH1D*)(temp_file.Get(name));
	OSC_100->Rebin(5);
	for(int i = 0; i <= nbinsE; i++){
	  OscVec[0][u][s][i] = (OSC_100->GetBinContent(i));
	}
	delete OSC_100;
      }
    }
    
    delete NULL_100;
    temp_file.Close();
  }

  if (use470m){
    std::string temp_name = "/Users/jzennamo/Desktop/LArLite/Files/ErrorMatrix/combined_ntuple_600m_onaxis_nu_processed_nue.root";

    TFile temp_file(temp_name.c_str());
    TH1D *NULL_470;
    NULL_470 = (TH1D*)(temp_file.Get("NumuCC"));
    NULL_470->Rebin(5);
    for(int i = 0; i <= nbinsE; i++){
      NULLVec[1][i] = (NULL_470->GetBinContent(i));
    }

    for(int u = 0; u < npoints; u++){
      for(int s = 0; s < 7; s++){
	TH1D *OSC_470;
	TString upoint = Form("%d",u);// = std::to_string(u);
	TString name = "Universe_";
	TString name2 = "_MultiSim_";
	TString mul = Form("%d",s);// = std::to_string(s);
	
	name += upoint;
	name += name2;
	name += mul;	
       
	OSC_470 = (TH1D*)(temp_file.Get(name));
	OSC_470->Rebin(5);
	for(int i = 0; i <= nbinsE; i++){
	  OscVec[1][u][s][i] = (OSC_470->GetBinContent(i));
	}
	delete OSC_470;
      }
    }
    
    delete NULL_470;
    temp_file.Close();
  }

  int nL = 2;

 
  TMatrix M6 (nbinsE*nL,nbinsE*nL);
  TMatrix C6 (nbinsE*nL,nbinsE*nL);
  TMatrix M5 (nbinsE*nL,nbinsE*nL);
  TMatrix M4 (nbinsE*nL,nbinsE*nL);
  TMatrix M3 (nbinsE*nL,nbinsE*nL);
  TMatrix M2 (nbinsE*nL,nbinsE*nL);
  TMatrix M1 (nbinsE*nL,nbinsE*nL);
  TMatrix M0 (nbinsE*nL,nbinsE*nL);
  TMatrix Null0 (nbinsE*nL,nbinsE*nL);
  
  Null0.Zero();

  int N = 0;

  TH1D *Fig5 = new TH1D("Fig5",";;",nbinsE,emin,emax);

  int Erri = 0, Errj = 0;

  //  std::cout << "Filling Error Matrix..." << std::endl;

  for(int Lrow = 0; Lrow < 2; Lrow++){
    for(int Erow = 0; Erow < nbinsE; Erow++){

      Errj = 0;

      for(int Lcol = 0; Lcol < 2; Lcol++){
        for(int Ecol = 0; Ecol < nbinsE; Ecol++){

          M6 (Erri,Errj) = 0;
          M5 (Erri,Errj) = 0;
          M4 (Erri,Errj) = 0;
          M3 (Erri,Errj) = 0;
          M2 (Erri,Errj) = 0;
          M1 (Erri,Errj) = 0;
          M0 (Erri,Errj) = 0;

	  N = 0;

	  for(int u = 0; u < npoints; u++){

	    M6 (Erri,Errj) += (NULLVec[Lrow][Erow]-OscVec[Lrow][u][6][Erow])*(NULLVec[Lcol][Ecol]-OscVec[Lcol][u][6][Ecol]);
            M5 (Erri,Errj) += (NULLVec[Lrow][Erow]-OscVec[Lrow][u][5][Erow])*(NULLVec[Lcol][Ecol]-OscVec[Lcol][u][5][Ecol]);
            M4 (Erri,Errj) += (NULLVec[Lrow][Erow]-OscVec[Lrow][u][4][Erow])*(NULLVec[Lcol][Ecol]-OscVec[Lcol][u][4][Ecol]);
            M3 (Erri,Errj) += (NULLVec[Lrow][Erow]-OscVec[Lrow][u][3][Erow])*(NULLVec[Lcol][Ecol]-OscVec[Lcol][u][3][Ecol]);
            M2 (Erri,Errj) += (NULLVec[Lrow][Erow]-OscVec[Lrow][u][2][Erow])*(NULLVec[Lcol][Ecol]-OscVec[Lcol][u][2][Ecol]);
            M1 (Erri,Errj) += (NULLVec[Lrow][Erow]-OscVec[Lrow][u][1][Erow])*(NULLVec[Lcol][Ecol]-OscVec[Lcol][u][1][Ecol]);
            M0 (Erri,Errj) += (NULLVec[Lrow][Erow]-OscVec[Lrow][u][0][Erow])*(NULLVec[Lcol][Ecol]-OscVec[Lcol][u][0][Ecol]);

	    N++;
	    
	  }

	  M6 (Erri,Errj) /= N;
          M5 (Erri,Errj) /= N;
          M4 (Erri,Errj) /= N;
          M3 (Erri,Errj) /= N;
          M2 (Erri,Errj) /= N;
          M1 (Erri,Errj) /= N;
	  M0 (Erri,Errj) /= N;
	  
	  M6 (Erri,Errj) /= NULLVec[Lrow][Erow]*NULLVec[Lcol][Ecol];
          M5 (Erri,Errj) /= NULLVec[Lrow][Erow]*NULLVec[Lcol][Ecol];
          M4 (Erri,Errj) /= NULLVec[Lrow][Erow]*NULLVec[Lcol][Ecol];
          M3 (Erri,Errj) /= NULLVec[Lrow][Erow]*NULLVec[Lcol][Ecol];
          M2 (Erri,Errj) /= NULLVec[Lrow][Erow]*NULLVec[Lcol][Ecol];
          M1 (Erri,Errj) /= NULLVec[Lrow][Erow]*NULLVec[Lcol][Ecol];
          M0 (Erri,Errj) /= NULLVec[Lrow][Erow]*NULLVec[Lcol][Ecol];

	  if(Lcol == 0 && Lrow == 0 && (Erri == Errj)) Fig5->SetBinContent(Ecol, M6 (Erri,Errj));

          Errj++;

	}}

      Erri++;

    }}

  for(int i = 0; i < Erri; i++){
    for(int j = 0; j < Errj; j++){

      C6 (i,j) = M6(i,j) / sqrt(M6 (i,i) * M6 (j,j));

    }
  }
  
  //  std::cout << "...Error Matrix Filled" << std::endl;


  TCanvas* c6 = new TCanvas("c6","",700,700);
  c6->SetLeftMargin(.1);
  c6->SetBottomMargin(.1);
  c6->SetTopMargin(.1);
  c6->SetRightMargin(.2);
  c6->cd();

  M6.Draw("COLZ");
  gStyle->SetPalette(56,0);
  TMatrixFBase->SetContour(999);
  TMatrixFBase->GetZaxis()->SetTitleFont(62);
  TMatrixFBase->GetZaxis()->SetLabelFont(62);
  TMatrixFBase->GetZaxis()->SetTitleSize(0.045);
  TMatrixFBase->GetZaxis()->SetTitle("Fractional Error Matrix");
  TMatrixFBase->GetZaxis()->SetTitleOffset(1.5);
  TMatrixFBase->GetXaxis()->SetTitle("");
  TMatrixFBase->GetXaxis()->SetLabelSize(0);
  TMatrixFBase->GetXaxis()->SetTitleOffset(1.5);
  TMatrixFBase->GetYaxis()->SetTitle("");
  TMatrixFBase->GetYaxis()->SetTitleOffset(1.5);
  TMatrixFBase->GetYaxis()->SetLabelSize(0);
  TMatrixFBase->SetStats(0);


  TLatex *ND = new TLatex(.2,.05,"LAr1-ND (100m) ");
  ND->SetNDC();
  ND->SetTextFont(62);
  ND->SetTextSize(0.05);
  ND->Draw();

  TLatex *MD = new TLatex(.57,.05,"T600");
  MD->SetNDC();
  MD->SetTextFont(62);
  MD->SetTextSize(0.05);
  MD->Draw();

  TLatex *ND45 = new TLatex(.075,.2,"LAr1-ND (100m) ");
  ND45->SetNDC();
  ND45->SetTextAngle(90);
  ND45->SetTextFont(62);
  ND45->SetTextSize(0.05);
  ND45->Draw();

  TLatex *MD45 = new TLatex(.075,.67,"T600");
  MD45->SetNDC();
  MD45->SetTextAngle(90);
  MD45->SetTextFont(62);
  MD45->SetTextSize(0.05);
  MD45->Draw();

  TLatex *Total = new TLatex(.2,.95,"Total Covariance Matrix");
  Total->SetNDC();
  Total->SetTextFont(62);
  Total->SetTextSize(0.05);
  Total->Draw();


  TCanvas* c51 = new TCanvas("c51","",700,700);
  c51->SetLeftMargin(.1);
  c51->SetBottomMargin(.1);
  c51->SetTopMargin(.1);
  c51->SetRightMargin(.2);
  c51->cd();

  C6.Draw("COLZ");
  gStyle->SetPalette(56,0);
  TMatrixFBase->SetContour(999);
  TMatrixFBase->GetZaxis()->SetTitleFont(62);
  TMatrixFBase->GetZaxis()->SetLabelFont(62);
  TMatrixFBase->GetZaxis()->SetTitleSize(0.045);
  TMatrixFBase->GetZaxis()->SetTitle("Correlation Matrix");
  TMatrixFBase->GetZaxis()->SetTitleOffset(1.5);
  TMatrixFBase->GetXaxis()->SetTitle("");
  TMatrixFBase->GetXaxis()->SetLabelSize(0);
  TMatrixFBase->GetXaxis()->SetTitleOffset(1.5);
  TMatrixFBase->GetYaxis()->SetTitle("");
  TMatrixFBase->GetYaxis()->SetTitleOffset(1.5);
  TMatrixFBase->GetYaxis()->SetLabelSize(0);
  TMatrixFBase->SetStats(0);

  ND->Draw();
  MD->Draw();
  ND45->Draw();
  MD45->Draw();
  Total->Draw();




  TCanvas* c5 = new TCanvas("c5","",700,700);
  c5->SetLeftMargin(.1);
  c5->SetBottomMargin(.1);
  c5->SetTopMargin(.1);
  c5->SetRightMargin(.2);
  c5->cd();

  M5.Draw("COLZ");
  gStyle->SetPalette(56,0);
  TMatrixFBase->SetContour(999);
  TMatrixFBase->GetZaxis()->SetTitleFont(62);
  TMatrixFBase->GetZaxis()->SetLabelFont(62);
  TMatrixFBase->GetZaxis()->SetTitleSize(0.045);
  TMatrixFBase->GetZaxis()->SetTitle("MultiSim 5 Error Matrix [%]");
  TMatrixFBase->GetZaxis()->SetTitleOffset(1.5);
  TMatrixFBase->GetXaxis()->SetTitle("");
  TMatrixFBase->GetXaxis()->SetLabelSize(0);
  TMatrixFBase->GetXaxis()->SetTitleOffset(1.5);
  TMatrixFBase->GetYaxis()->SetTitle("");
  TMatrixFBase->GetYaxis()->SetTitleOffset(1.5);
  TMatrixFBase->GetYaxis()->SetLabelSize(0);
  TMatrixFBase->SetStats(0);

  ND->Draw();
  MD->Draw();
  ND45->Draw();
  MD45->Draw();
  Total->Draw();




  TCanvas* c4 = new TCanvas("c4","",700,700);
  c4->SetLeftMargin(.1);
  c4->SetBottomMargin(.1);
  c4->SetTopMargin(.1);
  c4->SetRightMargin(.2);
  c4->cd();

  M4.Draw("COLZ");
  gStyle->SetPalette(56,0);
  TMatrixFBase->SetContour(999);
  TMatrixFBase->GetZaxis()->SetTitleFont(62);
  TMatrixFBase->GetZaxis()->SetLabelFont(62);
  TMatrixFBase->GetZaxis()->SetTitleSize(0.045);
  TMatrixFBase->GetZaxis()->SetTitle("MultiSim 4 Error Matrix [%]");
  TMatrixFBase->GetZaxis()->SetTitleOffset(1.5);
  TMatrixFBase->GetXaxis()->SetTitle("");
  TMatrixFBase->GetXaxis()->SetLabelSize(0);
  TMatrixFBase->GetXaxis()->SetTitleOffset(1.5);
  TMatrixFBase->GetYaxis()->SetTitle("");
  TMatrixFBase->GetYaxis()->SetTitleOffset(1.5);
  TMatrixFBase->GetYaxis()->SetLabelSize(0);
  TMatrixFBase->SetStats(0);

  ND->Draw();
  MD->Draw();
  ND45->Draw();
  MD45->Draw();
  Total->Draw();




  TCanvas* c3 = new TCanvas("c3","",700,700);
  c3->SetLeftMargin(.1);
  c3->SetBottomMargin(.1);
  c3->SetTopMargin(.1);
  c3->SetRightMargin(.2);
  c3->cd();

  M3.Draw("COLZ");
  gStyle->SetPalette(56,0);
  TMatrixFBase->SetContour(999);
  TMatrixFBase->GetZaxis()->SetTitleFont(62);
  TMatrixFBase->GetZaxis()->SetLabelFont(62);
  TMatrixFBase->GetZaxis()->SetTitleSize(0.045);
  TMatrixFBase->GetZaxis()->SetTitle("MultiSim 3 Error Matrix [%]");
  TMatrixFBase->GetZaxis()->SetTitleOffset(1.5);
  TMatrixFBase->GetXaxis()->SetTitle("");
  TMatrixFBase->GetXaxis()->SetLabelSize(0);
  TMatrixFBase->GetXaxis()->SetTitleOffset(1.5);
  TMatrixFBase->GetYaxis()->SetTitle("");
  TMatrixFBase->GetYaxis()->SetTitleOffset(1.5);
  TMatrixFBase->GetYaxis()->SetLabelSize(0);
  TMatrixFBase->SetStats(0);

  ND->Draw();
  MD->Draw();
  ND45->Draw();
  MD45->Draw();
  Total->Draw();




  TCanvas* c2 = new TCanvas("c2","",700,700);
  c2->SetLeftMargin(.1);
  c2->SetBottomMargin(.1);
  c2->SetTopMargin(.1);
  c2->SetRightMargin(.2);
  c2->cd();

  M2.Draw("COLZ");
  gStyle->SetPalette(56,0);
  TMatrixFBase->SetContour(999);
  TMatrixFBase->GetZaxis()->SetTitleFont(62);
  TMatrixFBase->GetZaxis()->SetLabelFont(62);
  TMatrixFBase->GetZaxis()->SetTitleSize(0.045);
  TMatrixFBase->GetZaxis()->SetTitle("MultiSim 2 Error Matrix [%]");
  TMatrixFBase->GetZaxis()->SetTitleOffset(1.5);
  TMatrixFBase->GetXaxis()->SetTitle("");
  TMatrixFBase->GetXaxis()->SetLabelSize(0);
  TMatrixFBase->GetXaxis()->SetTitleOffset(1.5);
  TMatrixFBase->GetYaxis()->SetTitle("");
  TMatrixFBase->GetYaxis()->SetTitleOffset(1.5);
  TMatrixFBase->GetYaxis()->SetLabelSize(0);
  TMatrixFBase->SetStats(0);

  ND->Draw();
  MD->Draw();
  ND45->Draw();
  MD45->Draw();
  Total->Draw();




  TCanvas* c1 = new TCanvas("c1","",700,700);
  c1->SetLeftMargin(.1);
  c1->SetBottomMargin(.1);
  c1->SetTopMargin(.1);
  c1->SetRightMargin(.2);
  c1->cd();

  M1.Draw("COLZ");
  gStyle->SetPalette(56,0);
  TMatrixFBase->SetContour(999);
  TMatrixFBase->GetZaxis()->SetTitleFont(62);
  TMatrixFBase->GetZaxis()->SetLabelFont(62);
  TMatrixFBase->GetZaxis()->SetTitleSize(0.045);
  TMatrixFBase->GetZaxis()->SetTitle("MultiSim 1 Error Matrix [%]");
  TMatrixFBase->GetZaxis()->SetTitleOffset(1.5);
  TMatrixFBase->GetXaxis()->SetTitle("");
  TMatrixFBase->GetXaxis()->SetLabelSize(0);
  TMatrixFBase->GetXaxis()->SetTitleOffset(1.5);
  TMatrixFBase->GetYaxis()->SetTitle("");
  TMatrixFBase->GetYaxis()->SetTitleOffset(1.5);
  TMatrixFBase->GetYaxis()->SetLabelSize(0);
  TMatrixFBase->SetStats(0);

  ND->Draw();
  MD->Draw();
  ND45->Draw();
  MD45->Draw();
  Total->Draw();




  TCanvas* c0 = new TCanvas("c0","",700,700);
  c0->SetLeftMargin(.1);
  c0->SetBottomMargin(.1);
  c0->SetTopMargin(.1);
  c0->SetRightMargin(.2);
  c0->cd();

  M0.Draw("COLZ");
  gStyle->SetPalette(56,0);
  TMatrixFBase->SetContour(999);
  TMatrixFBase->GetZaxis()->SetTitleFont(62);
  TMatrixFBase->GetZaxis()->SetLabelFont(62);
  TMatrixFBase->GetZaxis()->SetTitleSize(0.045);
  TMatrixFBase->GetZaxis()->SetTitle("MultiSim 0 Error Matrix [%]");
  TMatrixFBase->GetZaxis()->SetTitleOffset(1.5);
  TMatrixFBase->GetXaxis()->SetTitle("");
  TMatrixFBase->GetXaxis()->SetLabelSize(0);
  TMatrixFBase->GetXaxis()->SetTitleOffset(1.5);
  TMatrixFBase->GetYaxis()->SetTitle("");
  TMatrixFBase->GetYaxis()->SetTitleOffset(1.5);
  TMatrixFBase->GetYaxis()->SetLabelSize(0);
  TMatrixFBase->SetStats(0);

  ND->Draw();
  MD->Draw();
  ND45->Draw();
  MD45->Draw();
  Total->Draw();

 
  // cout<<"\nEnd of routine.\n"

  for(int i = 0; i < nbinsE*nL; i++){
    for(int j = 0; j < nbinsE*nL; j++){

      std::cout << M6(i,j) <<"\t";
      
      }
  }

  return 0;

}


#ifndef __CINT__
int main()
{
  multiple_detector_fit();
  return 0;
}
# endif

void Matrix(){
  multiple_detector_fit();
  return;
}
