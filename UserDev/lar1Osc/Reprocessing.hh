//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat Sep 20 11:54:23 2014 by ROOT version 5.34/18
// from TTree EventsTot/Event info for ALL types
// found on file: ../../../genie_RW/ntuple_100m_nu_0_hist.root
//////////////////////////////////////////////////////////

#ifndef Reprocessing_h
#define Reprocessing_h

#include "TString.h"
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include "TStyle.h"
#include "TCanvas.h"
#include "THStack.h"
#include <TH2.h>
#include <TH3.h>
#include "Utils.hh"

#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <iomanip>

// Fixed size dimensions of array or collections stored in the TTree if any.
namespace lar1{

class Reprocessing {
  public :

   TFile          *infile;    
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           iflux;
   Int_t           nuchan;
   Int_t           inno;
   Double_t        enugen;
   Double_t        nuleng;
   Int_t           isCC;
   Int_t           mode;
   Double_t        ThetaLep;
   Double_t        PhiLep;
   Double_t        Elep;
   std::vector<float>   *neutMom;
   std::vector<float>   *vertex;
   std::vector<int>     *GeniePDG;
   std::vector<std::vector<float> > *GenieMomentum;
   std::vector<std::string>  *GenieProc;
   Int_t           ptype;
   Int_t           tptype;
   Int_t           ndecay;
   std::vector<float>   *ParentVertex;
   std::vector<float>   *nuParentMomAtDecay;
   std::vector<float>   *nuParentMomAtProd;
   std::vector<float>   *nuParentMomTargetExit;
   std::vector<std::vector<float> > *leptonPos;
   std::vector<std::vector<float> > *leptonMom;
   Int_t           NPi0;
   Int_t           NPi0FinalState;
   Int_t           NGamma;
   Char_t          FoundPhotons;
   std::vector<std::vector<float> > *p1PhotonConversionPos;
   std::vector<std::vector<float> > *p1PhotonConversionMom;
   std::vector<std::vector<float> > *p2PhotonConversionPos;
   std::vector<std::vector<float> > *p2PhotonConversionMom;
   std::vector<std::vector<float> > *miscPhotonConversionPos;
   std::vector<std::vector<float> > *miscPhotonConversionMom;
   std::vector<std::vector<float> > *PionPos;
   std::vector<std::vector<float> > *PionMom;
   std::vector<std::vector<std::vector<float> > > *ChargedPionPos;
   std::vector<std::vector<std::vector<float> > > *ChargedPionMom;
   std::vector<int>     *ChargedPionSign;
   std::vector<std::vector<float> > *MultiWeight;

   // List of branches
   TBranch        *b_iflux;   //!
   TBranch        *b_nuchan;   //!
   TBranch        *b_inno;   //!
   TBranch        *b_enugen;   //!
   TBranch        *b_nuleng;   //!
   TBranch        *b_isCC;   //!
   TBranch        *b_mode;   //!
   TBranch        *b_ThetaLep;   //!
   TBranch        *b_PhiLep;   //!
   TBranch        *b_Elep;   //!
   TBranch        *b_neutMom;   //!
   TBranch        *b_vertex;   //!
   TBranch        *b_GeniePDG;   //!
   TBranch        *b_GenieMomentum;   //!
   TBranch        *b_GenieProc;   //!
   TBranch        *b_ptype;   //!
   TBranch        *b_tptype;   //!
   TBranch        *b_ndecay;   //!
   TBranch        *b_ParentVertex;   //!
   TBranch        *b_nuParentMomAtDecay;   //!
   TBranch        *b_nuParentMomAtProd;   //!
   TBranch        *b_nuParentMomTargetExit;   //!
   TBranch        *b_leptonPos;   //!
   TBranch        *b_leptonMom;   //!
   TBranch        *b_NPi0;   //!
   TBranch        *b_NPi0FinalState;   //!
   TBranch        *b_NGamma;   //!
   TBranch        *b_FoundAllPhotons;   //!
   TBranch        *b_p1PhotonConversionPos;   //!
   TBranch        *b_p1PhotonConversionMom;   //!
   TBranch        *b_p2PhotonConversionPos;   //!
   TBranch        *b_p2PhotonConversionMom;   //!
   TBranch        *b_miscPhotonConversionPos;   //!
   TBranch        *b_miscPhotonConversionMom;   //!
   TBranch        *b_PionPos;   //!
   TBranch        *b_PionMom;   //!
   TBranch        *b_ChargedPionPos;   //!
   TBranch        *b_ChargedPionMom;   //!
   TBranch        *b_ChargedPionSign;   //!
   TBranch        *b_MultiWeight;   //!

   // Reprocessing(TTree *tree=0);
   // virtual ~Reprocessing();
   // virtual Int_t    Cut(Long64_t entry);
   // virtual Int_t    GetEntry(Long64_t entry);
   // virtual Long64_t LoadTree(Long64_t entry);
   // virtual void     Init(TTree *tree);
   // // virtual void     Loop();
   // virtual Bool_t   Notify();
   // virtual void     Show(Long64_t entry = -1);

    Reprocessing(TString file = "");
    virtual ~Reprocessing();
    virtual Int_t    Cut(Long64_t entry);
    virtual Int_t    GetEntry(Long64_t entry);
    virtual Long64_t LoadTree(Long64_t entry);
    virtual void     Init(TTree *tree);
    virtual void     Loop(std::string signal,
                            int iDet = 1,
                            int iLoc = 4,
                            Long64_t max_entry = -1,
                            bool skipMultiWeights = false,
                            bool verbose = false, 
                            double scale = 1);
    virtual Bool_t   Notify();
    virtual void     Show(Long64_t entry = -1);
    virtual TString  InFile();
    virtual void CalcLepton( TVector3& );

    Utils utils;
    
  };
} //namespace lar1
#endif


#ifdef Reprocessing_cxx

namespace lar1{
  Reprocessing::Reprocessing(TString file) : fChain(0) {

    // if parameter tree is not specified (or zero), connect the file
    // used to generate this class and read the Tree.
    // gROOT -> ProcessLine(".L LorentzVectorLight.h+");

    TTree *tree;
    if (file.Length() == 0){
      std::cout << "Opening test file ntuple_example.root" << std::endl;
      infile = new TFile("ntuple_example.root");
      TDirectory * dir = (TDirectory*)infile->Get("ntuple_example.root:/nuana");
      dir->GetObject("EventsTot",tree);
    }
    else{ 
      std::cout << "Opening requested file " << file << std::endl;
      infile = new TFile( file );
      infile->GetObject("nuana/EventsTot",tree);
    }
    
    if (!infile || !infile->IsOpen()) {
      std::cout << "Could not find specified file!!" << std::endl;
      exit(1);
    }
    
    Init(tree);
    
    // // Initialize the setup booleans:
    // oscNeutrinos     = true;
    // oscAntiNeutrinos = true;
    // detNeutrinos     = true;
    // detAntiNeutrinos = true;
    // specialNameText  = "";


  }


Reprocessing::~Reprocessing()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t Reprocessing::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t Reprocessing::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void Reprocessing::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   neutMom = 0;
   vertex = 0;
   GeniePDG = 0;
   GenieMomentum = 0;
   GenieProc = 0;
   ParentVertex = 0;
   nuParentMomAtDecay = 0;
   nuParentMomAtProd = 0;
   nuParentMomTargetExit = 0;
   leptonPos = 0;
   leptonMom = 0;
   p1PhotonConversionPos = 0;
   p1PhotonConversionMom = 0;
   p2PhotonConversionPos = 0;
   p2PhotonConversionMom = 0;
   miscPhotonConversionPos = 0;
   miscPhotonConversionMom = 0;
   PionPos = 0;
   PionMom = 0;
   ChargedPionPos = 0;
   ChargedPionMom = 0;
   ChargedPionSign = 0;
   MultiWeight = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("iflux", &iflux, &b_iflux);
   fChain->SetBranchAddress("nuchan", &nuchan, &b_nuchan);
   fChain->SetBranchAddress("inno", &inno, &b_inno);
   fChain->SetBranchAddress("enugen", &enugen, &b_enugen);
   fChain->SetBranchAddress("nuleng", &nuleng, &b_nuleng);
   fChain->SetBranchAddress("isCC", &isCC, &b_isCC);
   fChain->SetBranchAddress("mode", &mode, &b_mode);
   fChain->SetBranchAddress("ThetaLep", &ThetaLep, &b_ThetaLep);
   fChain->SetBranchAddress("PhiLep", &PhiLep, &b_PhiLep);
   fChain->SetBranchAddress("Elep", &Elep, &b_Elep);
   fChain->SetBranchAddress("neutMom", &neutMom, &b_neutMom);
   fChain->SetBranchAddress("vertex", &vertex, &b_vertex);
   fChain->SetBranchAddress("GeniePDG", &GeniePDG, &b_GeniePDG);
   fChain->SetBranchAddress("GenieMomentum", &GenieMomentum, &b_GenieMomentum);
   fChain->SetBranchAddress("GenieProc", &GenieProc, &b_GenieProc);
   fChain->SetBranchAddress("ptype", &ptype, &b_ptype);
   fChain->SetBranchAddress("tptype", &tptype, &b_tptype);
   fChain->SetBranchAddress("ndecay", &ndecay, &b_ndecay);
   fChain->SetBranchAddress("ParentVertex", &ParentVertex, &b_ParentVertex);
   fChain->SetBranchAddress("nuParentMomAtDecay", &nuParentMomAtDecay, &b_nuParentMomAtDecay);
   fChain->SetBranchAddress("nuParentMomAtProd", &nuParentMomAtProd, &b_nuParentMomAtProd);
   fChain->SetBranchAddress("nuParentMomTargetExit", &nuParentMomTargetExit, &b_nuParentMomTargetExit);
   fChain->SetBranchAddress("leptonPos", &leptonPos, &b_leptonPos);
   fChain->SetBranchAddress("leptonMom", &leptonMom, &b_leptonMom);
   fChain->SetBranchAddress("NPi0", &NPi0, &b_NPi0);
   fChain->SetBranchAddress("NPi0FinalState", &NPi0FinalState, &b_NPi0FinalState);
   fChain->SetBranchAddress("NGamma", &NGamma, &b_NGamma);
   fChain->SetBranchAddress("FoundPhotons", &FoundPhotons, &b_FoundAllPhotons);
   fChain->SetBranchAddress("p1PhotonConversionPos", &p1PhotonConversionPos, &b_p1PhotonConversionPos);
   fChain->SetBranchAddress("p1PhotonConversionMom", &p1PhotonConversionMom, &b_p1PhotonConversionMom);
   fChain->SetBranchAddress("p2PhotonConversionPos", &p2PhotonConversionPos, &b_p2PhotonConversionPos);
   fChain->SetBranchAddress("p2PhotonConversionMom", &p2PhotonConversionMom, &b_p2PhotonConversionMom);
   fChain->SetBranchAddress("miscPhotonConversionPos", &miscPhotonConversionPos, &b_miscPhotonConversionPos);
   fChain->SetBranchAddress("miscPhotonConversionMom", &miscPhotonConversionMom, &b_miscPhotonConversionMom);
   fChain->SetBranchAddress("PionPos", &PionPos, &b_PionPos);
   fChain->SetBranchAddress("PionMom", &PionMom, &b_PionMom);
   fChain->SetBranchAddress("ChargedPionPos", &ChargedPionPos, &b_ChargedPionPos);
   fChain->SetBranchAddress("ChargedPionMom", &ChargedPionMom, &b_ChargedPionMom);
   fChain->SetBranchAddress("ChargedPionSign", &ChargedPionSign, &b_ChargedPionSign);
   fChain->SetBranchAddress("MultiWeight", &MultiWeight, &b_MultiWeight);
   Notify();
}

Bool_t Reprocessing::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void Reprocessing::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t Reprocessing::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}


TString Reprocessing::InFile(){
  return infile->GetName();
}

void Reprocessing::CalcLepton( TVector3 & lepDir)
{

  // loop over Genie vectors and find the lepton.  
  // These are 11, 12, 13, 14 and negatives.

  for( unsigned long i = 0; i < GeniePDG->size(); i++ ){
    //std::cout << "GENIE PDG: " << GeniePDG->at(i) << std::endl;
    if( abs(GeniePDG->at(i)) == 11 ||
        abs(GeniePDG->at(i)) == 13 ){
      
      lepDir.SetX(GenieMomentum->at(i).at(1));
      lepDir.SetY(GenieMomentum->at(i).at(2));
      lepDir.SetZ(GenieMomentum->at(i).at(3));
      ThetaLep = utils.GetTheta( lepDir.X(), lepDir.Y(), lepDir.Z() );
      PhiLep = utils.GetPhi( lepDir.X(), lepDir.Y() );
      Elep = GenieMomentum->at(i).at(0);
      return;      
    }
  }

  for( unsigned long i = 0; i < GeniePDG->size(); i++ ){
    //std::cout << "GENIE PDG: " << GeniePDG->at(i) << std::endl;
    if( abs(GeniePDG->at(i)) == 11 || abs(GeniePDG->at(i)) == 12 ||
        abs(GeniePDG->at(i)) == 13 || abs(GeniePDG->at(i)) == 14 ){
      
      lepDir.SetX(GenieMomentum->at(i).at(1));
      lepDir.SetY(GenieMomentum->at(i).at(2));
      lepDir.SetZ(GenieMomentum->at(i).at(3));
    }
  }

  ThetaLep = utils.GetTheta( lepDir.X(), lepDir.Y(), lepDir.Z() );
  PhiLep = utils.GetPhi( lepDir.X(), lepDir.Y() );

  return;
}

} //namespace lar1

#endif // #ifdef Reprocessing_cxx
