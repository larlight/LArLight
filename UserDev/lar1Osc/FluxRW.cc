#include "FluxRW.hh"
#include "TFile.h"
#include "TH1F.h"
#include "TH1D.h"

#include <iostream>
#include <string>
#include <cmath>

namespace lar1{
  FluxRW::FluxRW()
  {
    std::cout <<"Initializing FluxRW"<<std::endl;

    Int_t nbins=0;
    fBinSize=0.05; // using histograms with 50MeV bins
    //miniboone histogram file
   // Get the path to the files using the larlight env variable:
    std::string path = GetEnv("MAKE_TOP_DIR");

    path.append("/UserDev/lar1Osc/");
    std::string mbhistfile= path;
    mbhistfile.append("FluxRW/hist/april07_baseline_rgen610.6_flux.root");
    std::cout <<"Loading BNB flux histograms from "<<mbhistfile<<std::endl;
    TFile f1(mbhistfile.c_str());
    if ( !f1.IsOpen() ) {
      std::cout <<"Can't find BNB histogram file. Aborting!"<<std::endl;
      exit(0);
    }
    for (Int_t intype=1;intype<=4;intype++) {
      for (Int_t iptype=1;iptype<=4;iptype++) {
      TH1F *mbhist = dynamic_cast<TH1F*> (f1.Get(Form("h1%i%i",iptype,intype)));
        nbins=mbhist->GetNbinsX();
        //Need to scale to get to nu/m^2/1e7POT/50MeV (gshist is normalized 1e6POT*10 (factor of 10 because using 10 redecays))                           
        Double_t scale = 
        (1./(std::pow(610.6,2)*3.14159)) //events spread over A=610.6^2 cm2
      * (1.0e4)                  //cm2->m2
      * (1.0e7);                   //scale to 1e7 POT (GShist 1e6POT*10redecay)
    
        mbhist->Scale(scale);
        std::vector<Double_t> vec;
        for (Int_t ibin=1;ibin<nbins+1;ibin++) {
      vec.push_back(mbhist->GetBinContent(ibin));
        }
        fMB[intype-1][iptype-1]=vec;      
      }
    }
    f1.Close();

    std::string nu[4]={"nue","nuebar","numu","numubar"};

    for (Int_t intype=1;intype<=4;intype++) {
      std::string gshistfile = path;
      gshistfile.append(Form("/FluxRW/hist/%s_rawntp.root",nu[intype-1].c_str()));
      std::cout <<"Loading gsimple like flux histograms from "<<gshistfile<<std::endl;
      TFile f2(gshistfile.c_str());

      if ( !f2.IsOpen() ) {
        std::cout <<"Can't find gsimple histogram file. Aborting!"<<std::endl;
        exit(0);
      }

      TH1D* hmup = dynamic_cast<TH1D*> (f2.Get("hMuPlus_Flux"));
      TH1D* hmum = dynamic_cast<TH1D*> (f2.Get("hMuMinus_Flux"));
      TH1D* hpip = dynamic_cast<TH1D*> (f2.Get("hPiPlus_Flux"));
      TH1D* hpim = dynamic_cast<TH1D*> (f2.Get("hPiMinus_Flux"));
      TH1D* hk0  = dynamic_cast<TH1D*> (f2.Get("hKL_Flux"));
      TH1D* hkp  = dynamic_cast<TH1D*> (f2.Get("hKPlus_Flux"));
      TH1D* hkm  = dynamic_cast<TH1D*> (f2.Get("hKMinus_Flux"));

      TH1D* gshist[4];
      gshist[0]=dynamic_cast<TH1D*> (hmup->Clone());
      gshist[0]->Add(hmum);

      gshist[1]=dynamic_cast<TH1D*> (hpip->Clone());
      gshist[1]->Add(hpim);

      gshist[2]=dynamic_cast<TH1D*> (hk0->Clone());

      gshist[3]=dynamic_cast<TH1D*> (hkp->Clone());
      gshist[3]->Add(hkm);

      for (Int_t iptype=1;iptype<=4;iptype++) {
        std::vector<Double_t> vec(0);
        for (Int_t ibin=1;ibin<nbins;ibin++) {
      vec.push_back(gshist[iptype-1]->GetBinContent(ibin));
        }
        fGS[intype-1][iptype-1]=vec;
      }

      f2.Close();
    }
    std::cout <<std::endl;
    std::cout <<"***************************************************"<<std::endl;
    std::cout <<"* Conventions used when calling member functions:"  <<std::endl;
    std::cout <<"* ntype = 1, 2, 3 or 4 (nue, nuebar, numu, numubar)"<<std::endl;
    std::cout <<"* ptype = 1, 2, 3 or 4 (mu+-, pi+-, K0L, K+-)"      <<std::endl;
    std::cout <<"***************************************************"<<std::endl;
  }

  FluxRW::~FluxRW()
  {
  }

  Double_t FluxRW::GetWeight(Double_t energy, Int_t ntype, Int_t ptype)
  {
    Double_t wgh=1;

    if (ntype>4 || ntype<1) {
      std::cout <<"Invalid neutrino type ntype="<<ntype<<" ! Expected 1,2,3 or 4 (nue, nuebar, numu or numubar)"<<std::endl;
      std::cout <<"Returning weight=1"<<std::endl;
    } else if (ptype>4 || ptype<1) {
      std::cout <<"Invalid neutrino parent type ptype="<<ptype<<" ! Expected 1,2,3 or 4 (mu+-, pi+-, K0 or K+-)"<<std::endl;
      std::cout <<"Returning weight=1"<<std::endl;
    } else {

      Int_t bin=Int_t(energy/fBinSize);
      Double_t num   = fMB[ntype-1][ptype-1][bin];
      Double_t denom = fGS[ntype-1][ptype-1][bin];

      if ( denom != 0 && num != 0 ) 
        wgh=num/denom;
    }

    return wgh;
  }

  TH1D* FluxRW::GetWeightHist(Int_t ntype, Int_t ptype) 
  {
    if (ntype>4 || ntype<1) {
      std::cout <<"Invalid neutrino type ntype="<<ntype<<" ! Expected 1,2,3 or 4 (nue, nuebar, numu or numubar)"<<std::endl;
    } else if (ptype>4 || ptype<1) {
      std::cout <<"Invalid neutrino parent type ptype="<<ptype<<" ! Expected 1,2,3 or 4 (mu+-, pi+-, K0 or K+-)"<<std::endl;
    }

    Int_t nbins=fGS[ntype-1][ptype-1].size();
    Float_t low=0;
    Float_t high=fBinSize*Float_t(nbins);
    TH1D* h=new TH1D(Form("hist_%i_%i",ntype,ptype),"",nbins,low,high);

    for (Int_t i=0;i<nbins;i++) {

      Double_t num   = fMB[ntype-1][ptype-1][i];
      Double_t denom = fGS[ntype-1][ptype-1][i];
      if ( denom != 0 && num !=0 ) 
        h->SetBinContent(i+1,num/denom);
      else
        h->SetBinContent(i+1,1.);
    }

    return h;
  }

  std::string FluxRW::GetEnv( const std::string & var ){
    const char * val = std::getenv( var.c_str() );
    if ( val == 0 ) {
        return "";
    }
    else {
        return val;
    }
  }


}