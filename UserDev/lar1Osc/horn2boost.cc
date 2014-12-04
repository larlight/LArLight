#include "horn2boost.hh"
#include "TFile.h"
#include "TH1F.h"
#include "TH1D.h"

#include <iostream>
#include <string>

using namespace std;

horn2boost::horn2boost()
{
  // cout <<"Initializing horn2boost"<<endl;
  fBinSize=0.05;
  kMaxWeight=30;
  kDet.push_back("lar1nd");   
  kDet.push_back("uboone");
  kDet.push_back("T600");

  kNu.push_back("nue");
  kNu.push_back("nuebar");
  kNu.push_back("numu");
  kNu.push_back("numubar");

  kConf.push_back("bnb1horn");
  kConf.push_back("bnb2horn");


  // Get the path to the files using the larlight env variable:
  std::string path = GetEnv("MAKE_TOP_DIR");
  path.append("/UserDev/lar1Osc/");

  for (uint iConf=0;iConf<kConf.size();iConf++) {
    for (uint iDet=0;iDet<kDet.size();iDet++) {
      for (uint iNu=0;iNu<kNu.size();iNu++) {
        std::string histfile(Form("%s/twoHornHist/%s_rw_%s_%s.root",
                                  path.c_str(),
                                  kNu[iNu].c_str(),
                                  kConf[iConf].c_str(),
                                  kDet[iDet].c_str()));
        // cout <<"Loading flux histograms from "<<histfile<<endl;
        TFile fin(histfile.c_str());
        
        if ( !fin.IsOpen() ) {
          cout <<"Can't find histogram file. Aborting!"<<endl;
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

  // cout <<endl;
  // cout <<"***************************************************"<<endl;
  // cout <<"* Conventions used when calling member functions:"  <<endl;
  // cout <<"* idet  = 1, 2, or 3 (LAr1ND, uboone, T600)"<<endl;
  // cout <<"* ntype = 1, 2, 3 or 4 (nue, nuebar, numu, numubar)"<<endl;
  // cout <<"* ptype = 1, 2, 3 or 4 (mu+-, pi+-, K0L, K+-)"      <<endl;
  // cout <<"***************************************************"<<endl;


}

horn2boost::~horn2boost()
{
}

Double_t horn2boost::GetWeight(Int_t idet, Int_t ntype, Int_t ptype, Double_t energy)
{
  Double_t wgh=1;

  if (ntype>4 || ntype<1) {
    cout <<"Invalid neutrino type ntype="<<ntype<<" ! Expected 1,2,3 or 4 (nue, nuebar, numu or numubar)"<<endl;
    cout <<"Returning weight=1"<<endl;
  } else if (ptype>4 || ptype<1) {
    cout <<"Invalid neutrino parent type ptype="<<ptype<<" ! Expected 1,2,3 or 4 (mu+-, pi+-, K0 or K+-)"<<endl;
    cout <<"Returning weight=1"<<endl;
  } else if (idet>3 || idet<0) {
    cout <<"Invalid detector type idet="<<idet<<endl;
    cout <<"Returning weight=1"<<endl;
  } else {

    Int_t bin=Int_t(energy/fBinSize);
    Double_t denom   = fGS[0][idet-1][ntype-1][ptype-1][bin];
    Double_t num = fGS[1][idet-1][ntype-1][ptype-1][bin];
    
    if ( denom != 0 && num != 0 ) 
      wgh=min(num/denom,kMaxWeight);
  }

  return wgh;
}

TH1D* horn2boost::GetWeightHist(Int_t idet, Int_t ntype, Int_t ptype) 
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

std::string horn2boost::GetEnv( const std::string & var ){
  const char * val = std::getenv( var.c_str() );
  if ( val == 0 ) {
      return "";
  }
  else {
      return val;
  }
}

