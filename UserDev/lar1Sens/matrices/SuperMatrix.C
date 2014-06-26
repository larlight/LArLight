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


  double NULLVec[3][2][55];
  double OscVec[3][2][1001][7][55];

  int nbinsE = 0;

  if (use100m){
    std::string temp_name = "/Users/jzennamo/Desktop/LArLite/Files/ErrorMatrix/combined_ntuple_100m_nu_processed_nue.root";
    TFile temp_file(temp_name.c_str());
    TH1D *NULL_100;
    NULL_100 = (TH1D*)(temp_file.Get("NumuCC"));
    NULL_100->Rebin(5);
    nbinsE = NULL_100->GetNbinsX();
    for(int i = 0; i <= nbinsE; i++){
      NULLVec[1][0][i] = (NULL_100->GetBinContent(i));
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
	  OscVec[1][0][u][s][i] = (OSC_100->GetBinContent(i));
	}
	delete OSC_100;
      }
    }    
    delete NULL_100;

    temp_file.Close();
    std::string temp_name = "/Users/jzennamo/Desktop/LArLite/Files/ErrorMatrix/combined_ntuple_100m_nu_processed_numu.root";
    TFile temp_file(temp_name.c_str());
    TH1D *NULL_100;
    NULL_100 = (TH1D*)(temp_file.Get("NumuCC"));
    NULL_100->Rebin(5);
    nbinsE = NULL_100->GetNbinsX();
    for(int i = 0; i <= nbinsE; i++){
      NULLVec[2][0][i] = (NULL_100->GetBinContent(i));
      NULLVec[0][0][i] = (NULL_100->GetBinContent(i));
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
	  OscVec[2][0][u][s][i] = (OSC_100->GetBinContent(i));
	  OscVec[0][0][u][s][i] = (OSC_100->GetBinContent(i));
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
      NULLVec[1][1][i] = (NULL_470->GetBinContent(i));
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
	  OscVec[1][1][u][s][i] = (OSC_470->GetBinContent(i));
	}
	delete OSC_470;
      }
    }
    
    delete NULL_470;
    temp_file.Close();

    std::string temp_name = "/Users/jzennamo/Desktop/LArLite/Files/ErrorMatrix/combined_ntuple_600m_onaxis_nu_processed_numu.root";

    TFile temp_file(temp_name.c_str());
    TH1D *NULL_470;
    NULL_470 = (TH1D*)(temp_file.Get("NumuCC"));
    NULL_470->Rebin(5);
    for(int i = 0; i <= nbinsE; i++){
      NULLVec[2][1][i] = (NULL_470->GetBinContent(i));
      NULLVec[0][1][i] = (NULL_470->GetBinContent(i));
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
	  OscVec[2][1][u][s][i] = (OSC_470->GetBinContent(i));
          OscVec[0][1][u][s][i] = (OSC_470->GetBinContent(i));
	}
	delete OSC_470;
      }
    }
    
    delete NULL_470;
    temp_file.Close();

  }

  int nL = 2;
  int signals = 3;
 
  TMatrix M6 (nbinsE*nL*signals,nbinsE*nL*signals);
  TMatrix C6 (nbinsE*nL*signals,nbinsE*nL*signals);
  TMatrix M5 (nbinsE*nL*signals,nbinsE*nL*signals);
  TMatrix M4 (nbinsE*nL*signals,nbinsE*nL*signals);
  TMatrix M3 (nbinsE*nL*signals,nbinsE*nL*signals);
  TMatrix M2 (nbinsE*nL*signals,nbinsE*nL*signals);
  TMatrix M1 (nbinsE*nL*signals,nbinsE*nL*signals);
  TMatrix M0 (nbinsE*nL*signals,nbinsE*nL*signals);
  TMatrix Null0 (nbinsE*nL*signals,nbinsE*nL*signals);
  
  Null0.Zero();

  int N = 0;

  TH1D *Fig5 = new TH1D("Fig5",";;",nbinsE,emin,emax);

  int Erri = 0, Errj = 0;

  //  std::cout << "Filling Error Matrix..." << std::endl;
  for(int Lrow = 0; Lrow < 2; Lrow++){
    for(int Srow = 0; Srow < signals; Srow++){
      for(int Erow = 0; Erow < nbinsE; Erow++){

      Errj = 0;
      for(int Lcol = 0; Lcol < 2; Lcol++){
	for(int Scol = 0; Scol < signals; Scol++){
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

	    M6 (Erri,Errj) += (NULLVec[Srow][Lrow][Erow]-OscVec[Srow][Lrow][u][6][Erow])*(NULLVec[Scol][Lcol][Ecol]-OscVec[Scol][Lcol][u][6][Ecol]);
            M5 (Erri,Errj) += (NULLVec[Srow][Lrow][Erow]-OscVec[Srow][Lrow][u][5][Erow])*(NULLVec[Scol][Lcol][Ecol]-OscVec[Scol][Lcol][u][5][Ecol]);
            M4 (Erri,Errj) += (NULLVec[Srow][Lrow][Erow]-OscVec[Srow][Lrow][u][4][Erow])*(NULLVec[Scol][Lcol][Ecol]-OscVec[Scol][Lcol][u][4][Ecol]);
            M3 (Erri,Errj) += (NULLVec[Srow][Lrow][Erow]-OscVec[Srow][Lrow][u][3][Erow])*(NULLVec[Scol][Lcol][Ecol]-OscVec[Scol][Lcol][u][3][Ecol]);
            M2 (Erri,Errj) += (NULLVec[Srow][Lrow][Erow]-OscVec[Srow][Lrow][u][2][Erow])*(NULLVec[Scol][Lcol][Ecol]-OscVec[Scol][Lcol][u][2][Ecol]);
            M1 (Erri,Errj) += (NULLVec[Srow][Lrow][Erow]-OscVec[Srow][Lrow][u][1][Erow])*(NULLVec[Scol][Lcol][Ecol]-OscVec[Scol][Lcol][u][1][Ecol]);
            M0 (Erri,Errj) += (NULLVec[Srow][Lrow][Erow]-OscVec[Srow][Lrow][u][0][Erow])*(NULLVec[Scol][Lcol][Ecol]-OscVec[Scol][Lcol][u][0][Ecol]);

	    N++;
	    
	  }

	  M6 (Erri,Errj) /= N;
          M5 (Erri,Errj) /= N;
          M4 (Erri,Errj) /= N;
          M3 (Erri,Errj) /= N;
          M2 (Erri,Errj) /= N;
          M1 (Erri,Errj) /= N;
	  M0 (Erri,Errj) /= N;

	  M6 (Erri,Errj) /= NULLVec[Srow][Lrow][Erow]*NULLVec[Scol][Lcol][Ecol];

	  //if(Srow == 0 || Scol == 0) M6 (Erri,Errj) = 0;
	  
          Errj++;

	  }}}

      Erri++;

      }}}

  
  //  std::cout << "...Error Matrix Filled" << std::endl;


  TCanvas* c6 = new TCanvas("c6","",700,700);
  c6->SetLeftMargin(.2);
  c6->SetBottomMargin(.2);
  c6->SetTopMargin(.05);
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


  TLatex *nue1 = new TLatex(.2,.15,"#nu_{e}^{osc} ");
  nue1->SetNDC();
  nue1->SetTextFont(62);
  nue1->SetTextSize(0.03);
  nue1->Draw();

  TLatex *nufosc1 = new TLatex(.3,.15,"#nu_{e}");
  nufosc1->SetNDC();
  nufosc1->SetTextFont(62);
  nufosc1->SetTextSize(0.03);
  nufosc1->Draw();

  TLatex *numu1 = new TLatex(.4,.15,"#nu_{#mu} ");
  numu1->SetNDC();
  numu1->SetTextFont(62);
  numu1->SetTextSize(0.03);
  numu1->Draw();

  TLatex *nue2 = new TLatex(.5,.15,"#nu_{e}^{osc} ");
  nue2->SetNDC();
  nue2->SetTextFont(62);
  nue2->SetTextSize(0.03);
  nue2->Draw();

  TLatex *nufosc2 = new TLatex(.6,.15,"#nu_{e}");
  nufosc2->SetNDC();
  nufosc2->SetTextFont(62);
  nufosc2->SetTextSize(0.03);
  nufosc2->Draw();

  TLatex *numu2 = new TLatex(.7,.15,"#nu_{#mu} ");
  numu2->SetNDC();
  numu2->SetTextFont(62);
  numu2->SetTextSize(0.03);
  numu2->Draw();

  TLatex *ND = new TLatex(.2,.05,"LAr1-ND (100m) ");
  ND->SetNDC();
  ND->SetTextFont(62);
  ND->SetTextSize(0.04);
  ND->Draw();

  TLatex *MD = new TLatex(.62,.05,"T600");
  MD->SetNDC();
  MD->SetTextFont(62);
  MD->SetTextSize(0.04);
  MD->Draw();

  TLatex *ND45 = new TLatex(.075,.2,"LAr1-ND (100m) ");
  ND45->SetNDC();
  ND45->SetTextAngle(90);
  ND45->SetTextFont(62);
  ND45->SetTextSize(0.04);
  ND45->Draw();

  TLatex *MD45 = new TLatex(.075,.72,"T600");
  MD45->SetNDC();
  MD45->SetTextAngle(90);
  MD45->SetTextFont(62);
  MD45->SetTextSize(0.04);
  MD45->Draw();


  TLatex *nue145 = new TLatex(.15,.22,"#nu_{e}^{osc} ");
  nue145->SetNDC();
  nue145->SetTextFont(62);
  nue145->SetTextSize(0.03);
  nue145->Draw();

  TLatex *nufosc145 = new TLatex(.15,.35,"#nu_{e}");
  nufosc145->SetNDC();
  nufosc145->SetTextFont(62);
  nufosc145->SetTextSize(0.03);
  nufosc145->Draw();

  TLatex *numu145 = new TLatex(.15,.47,"#nu_{#mu} ");
  numu145->SetNDC();
  numu145->SetTextFont(62);
  numu145->SetTextSize(0.03);
  numu145->Draw();

  TLatex *nue245 = new TLatex(.15,.6,"#nu_{e}^{osc} ");
  nue245->SetNDC();
  nue245->SetTextFont(62);
  nue245->SetTextSize(0.03);
  nue245->Draw();

  TLatex *nufosc245 = new TLatex(.15,.72,"#nu_{e}");
  nufosc245->SetNDC();
  nufosc245->SetTextFont(62);
  nufosc245->SetTextSize(0.03);
  nufosc245->Draw();

  TLatex *numu245 = new TLatex(.15,.85,"#nu_{#mu} ");
  numu245->SetNDC();
  numu245->SetTextFont(62);
  numu245->SetTextSize(0.03);
  numu245->Draw();


  /*  TLatex *Total = new TLatex(.2,.95,"Total Covariance Matrix");
  Total->SetNDC();
  Total->SetTextFont(62);
  Total->SetTextSize(0.05);
  Total->Draw();
  */
  // cout<<"\nEnd of routine.\n"

  /*  for(int i = 0; i < nbinsE*nL*signals; i++){
    for(int j = 0; j < nbinsE*nL*signals; j++){

      std::cout << M6 (i,j) <<"\t";
      
      }
      }*/

  for(int i = 0; i < Erri; i++){
    for(int j = 0; j < Errj; j++){

      C6 (i,j) = M6(i,j) / sqrt(M6 (i,i) * M6 (j,j));

    }
  }


  TCanvas* c7 = new TCanvas("c7","",700,700);
  c7->SetLeftMargin(.2);
  c7->SetBottomMargin(.2);
  c7->SetTopMargin(.05);
  c7->SetRightMargin(.2);
  c7->cd();

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


  nue1->Draw();
  nufosc1->Draw();
  numu1->Draw();
  nue2->Draw();
  nufosc2->Draw();
  numu2->Draw();
  ND->Draw();
  MD->Draw();
  ND45->Draw();
  MD45->Draw();
  nue145->Draw();
  nufosc145->Draw();
  numu145->Draw();
  nue245->Draw();
  nufosc245->Draw();
  numu245->Draw();



  return 0;

}


#ifndef __CINT__
int main()
{
  multiple_detector_fit();
  return 0;
}
# endif

void SuperMatrix(){
  multiple_detector_fit();
  return;
}
