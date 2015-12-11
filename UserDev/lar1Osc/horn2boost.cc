#include "horn2boost.hh"
#include "TFile.h"
#include "TH1F.h"
#include "TH1D.h"

#include <iostream>
#include <string>


lar1::horn2boost::horn2boost(std::string horn2conf)
{
  std::cout <<"Initializing horn2boost";
  if (horn2conf!="") {
    std::cout <<" with "<<horn2conf<<" configuration";
  } else {
    std::cout <<" with default configuration";
  }
  std::cout <<std::endl;
  fBinSize=0.05;
  kMaxWeight=30;
  kDet.push_back("lar1nd");   
  kDet.push_back("uboone");
  kDet.push_back("T600");

  kNu.push_back("nue");
  kNu.push_back("nuebar");
  kNu.push_back("numu");
  kNu.push_back("numubar");

  kConf.push_back("bnb1horn_3.5m_210kA");
  kConf.push_back("bnb2horn");


  // Get the path to the files using the larlight env variable:
  std::string path = GetEnv("MAKE_TOP_DIR");
  path.append("/UserDev/lar1Osc/");

  for (uint iConf=0;iConf<kConf.size();iConf++) {
    for (uint iDet=0;iDet<kDet.size();iDet++) {
      for (uint iNu=0;iNu<kNu.size();iNu++) {
        std::string histfile;
        if (horn2conf=="" || iConf==0)
          histfile=Form("%s/twoHornHist/%s_rw_%s_%s.root",
                        path.c_str(),
                        kNu[iNu].c_str(),
                        kConf[iConf].c_str(),
                        kDet[iDet].c_str());
        else
          histfile=Form("%s/twoHornHist/%s_rw_%s_%s_%s.root",
                        path.c_str(),
                        kNu[iNu].c_str(),
                        kConf[iConf].c_str(),
                        horn2conf.c_str(),
                        kDet[iDet].c_str());
        // std::cout <<"Loading flux histograms from "<<histfile<<std::endl;
        
        TFile fin(histfile.c_str());
        
        if ( !fin.IsOpen() ) {
          std::cout <<"Can't find histogram file. Aborting!"<<std::endl;
          exit(0);
        }

        TH1D* hmup = dynamic_cast<TH1D*> (fin.Get("hMuPlus_Flux"));
        TH1D* hmum = dynamic_cast<TH1D*> (fin.Get("hMuMinus_Flux"));
        TH1D* hpip = dynamic_cast<TH1D*> (fin.Get("hPiPlus_Flux"));
        TH1D* hpim = dynamic_cast<TH1D*> (fin.Get("hPiMinus_Flux"));
        TH1D* hk0  = dynamic_cast<TH1D*> (fin.Get("hKL_Flux"));
        TH1D* hkp  = dynamic_cast<TH1D*> (fin.Get("hKPlus_Flux"));
        TH1D* hkm  = dynamic_cast<TH1D*> (fin.Get("hKMinus_Flux"));
  
        TH1D* gshist[4];
        gshist[0]=dynamic_cast<TH1D*> (hmup->Clone());
        gshist[0]->Add(hmum);
        
        gshist[1]=dynamic_cast<TH1D*> (hpip->Clone());
        gshist[1]->Add(hpim);
        
        gshist[2]=dynamic_cast<TH1D*> (hk0->Clone());
        
        gshist[3]=dynamic_cast<TH1D*> (hkp->Clone());
        gshist[3]->Add(hkm);

        Int_t nbins=0;
        nbins=gshist[0]->GetNbinsX();
        for (Int_t iptype=1;iptype<=4;iptype++) {
          std::vector<Double_t> vec(0);
          for (Int_t ibin=1;ibin<nbins;ibin++) {
            vec.push_back(gshist[iptype-1]->GetBinContent(ibin));
          }
          fGS[iConf][iDet][iNu][iptype-1]=vec;
        }

        fin.Close();
        hmum=0;
        hmup=0;
        hpim=0;
        hpip=0;
        hk0=0;
        hkp=0;
        hkm=0;

      }
    }
  }

  // std::cout <<std::endl;
  // std::cout <<"***************************************************"<<std::endl;
  // std::cout <<"* Conventions used when calling member functions:"  <<std::endl;
  // std::cout <<"* idet  = 1, 2, or 3 (LAr1ND, uboone, T600)"<<std::endl;
  // std::cout <<"* ntype = 1, 2, 3 or 4 (nue, nuebar, numu, numubar)"<<std::endl;
  // std::cout <<"* ptype = 1, 2, 3 or 4 (mu+-, pi+-, K0L, K+-)"      <<std::endl;
  // std::cout <<"***************************************************"<<std::endl;


}

lar1::horn2boost::~horn2boost()
{
}

Double_t lar1::horn2boost::GetWeight(Int_t idet, Int_t ntype, Int_t ptype, Double_t energy)
{
  Double_t wgh=1;

  if (ntype>4 || ntype<1) {
    std::cout <<"Invalid neutrino type ntype="<<ntype<<" ! Expected 1,2,3 or 4 (nue, nuebar, numu or numubar)"<<std::endl;
    std::cout <<"Returning weight=1"<<std::endl;
  } else if (ptype>4 || ptype<1) {
    std::cout <<"Invalid neutrino parent type ptype="<<ptype<<" ! Expected 1,2,3 or 4 (mu+-, pi+-, K0 or K+-)"<<std::endl;
    std::cout <<"Returning weight=1"<<std::endl;
  } else if (idet>3 || idet<0) {
    std::cout <<"Invalid detector type idet="<<idet<<std::endl;
    std::cout <<"Returning weight=1"<<std::endl;
  } else {

    Int_t bin=Int_t(energy/fBinSize);
    Double_t denom   = fGS[0][idet-1][ntype-1][ptype-1][bin];
    Double_t num = fGS[1][idet-1][ntype-1][ptype-1][bin];
    
    if ( denom != 0 && num != 0 ) 
      wgh=std::min(num/denom,kMaxWeight);
  }

  return wgh;
}

TH1D* lar1::horn2boost::GetWeightHist(Int_t idet, Int_t ntype, Int_t ptype) 
{
  Int_t nbins=fGS[0][idet-1][ntype-1][ptype-1].size();
  Float_t low=0;
  Float_t high=fBinSize*Float_t(nbins);
  TH1D* h=new TH1D(Form("hist_%i_%i_%i",idet,ntype,ptype),"",nbins,low,high);

  for (Int_t i=0;i<nbins;i++) {
    h->SetBinContent(i+1,GetWeight(idet,ntype,ptype,h->GetBinCenter(i+1)));
  }
  
  return h;
}

std::string lar1::horn2boost::GetEnv( const std::string & var ){
  const char * val = std::getenv( var.c_str() );
  if ( val == 0 ) {
      return "";
  }
  else {
      return val;
  }
}

