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
    Double_t        POT;
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
    std::vector<float>   *neutMom_temp;
    std::vector<float>   *vertex_temp;
    std::vector<int>     *GeniePDG;
    std::vector<std::vector<float> > *GenieMomentum_temp;
    std::vector<std::string>  *GenieProc;
    Int_t           ptype;
    Int_t           tptype;
    Int_t           ndecay;
    std::vector<float>   *neutVertexInWindow_temp;
    std::vector<float>   *ParentVertex_temp;
    std::vector<float>   *nuParentMomAtDecay_temp;
    std::vector<float>   *nuParentMomAtProd_temp;
    std::vector<float>   *nuParentMomTargetExit_temp;
    std::vector<std::vector<float> > *leptonPos_temp;
    std::vector<std::vector<float> > *leptonMom_temp;
    Int_t           NPi0;
    Int_t           NPi0FinalState;
    Int_t           NGamma;
    Char_t          FoundPhotons;
    std::vector<std::vector<float> > *p1PhotonConversionPos_temp;
    std::vector<std::vector<float> > *p1PhotonConversionMom_temp;
    std::vector<std::vector<float> > *p2PhotonConversionPos_temp;
    std::vector<std::vector<float> > *p2PhotonConversionMom_temp;
    std::vector<std::vector<float> > *miscPhotonConversionPos_temp;
    std::vector<std::vector<float> > *miscPhotonConversionMom_temp;
    std::vector<std::vector<float> > *PionPos_temp;
    std::vector<std::vector<float> > *PionMom_temp;
    std::vector<std::vector<std::vector<float> > > *ChargedPionPos_temp;
    std::vector<std::vector<std::vector<float> > > *ChargedPionMom_temp;
    std::vector<int>     *ChargedPionSign;
    std::vector<std::vector<float> > *MultiWeight;

    // These are the data members that you should work with
    // They replace the vectors with LorentzVectors or TVector3s as
    // appropriate, and should make things easier to digest.
    // I left them as pointers for consistency with other
    // vectors that don't need to be repackaged
    TLorentzVector                            * neutMom;
    TVector3                                  * vertex;
    std::vector<TLorentzVector>               * GenieMomentum;
    TVector3                                  * neutVertexInWindow;
    TVector3                                  * ParentVertex;
    TVector3                                  * nuParentMomAtDecay;
    TVector3                                  * nuParentMomAtProd;
    TVector3                                  * nuParentMomTargetExit;
    std::vector<TLorentzVector>               * leptonPos;
    std::vector<TLorentzVector>               * leptonMom;
    std::vector<TLorentzVector>               * p1PhotonConversionPos;
    std::vector<TLorentzVector>               * p1PhotonConversionMom;
    std::vector<TLorentzVector>               * p2PhotonConversionPos;
    std::vector<TLorentzVector>               * p2PhotonConversionMom;
    std::vector<TLorentzVector>               * miscPhotonConversionPos;
    std::vector<TLorentzVector>               * miscPhotonConversionMom;
    std::vector<TLorentzVector>               * PionPos;
    std::vector<TLorentzVector>               * PionMom;
    std::vector<std::vector<TLorentzVector> > * ChargedPionPos;
    std::vector<std::vector<TLorentzVector> > * ChargedPionMom;

    // List of branches
    TBranch        *b_POT;   //!
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
    TBranch        *b_neutVertexInWindow;   //!
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

    Reprocessing(TString file = "");
    virtual ~Reprocessing();
    virtual Int_t    Cut(Long64_t entry);
    virtual Int_t    GetEntry(Long64_t entry);
    virtual Long64_t LoadTree(Long64_t entry);
    virtual void     Init(TTree *tree);
    virtual void     Loop( std::string signal,
                           int iDet = 1,
                           int iLoc = 4,
                           Long64_t max_entry = -1,
                           bool skipMultiWeights = false,
                           bool verbose = false, 
                           double scale = 1);
    virtual Bool_t   Notify();
    virtual void     Show(Long64_t entry = -1);
    virtual TString  InFile();
    virtual void     CalcLepton( TVector3& );

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
    neutMom                 = new TLorentzVector(0,0,0,0);
    vertex                  = new TVector3(0,0,0);
    GenieMomentum           = new std::vector<TLorentzVector>;
    neutVertexInWindow      = new TVector3(0,0,0);
    ParentVertex            = new TVector3(0,0,0);
    nuParentMomAtDecay      = new TVector3(0,0,0);
    nuParentMomAtProd       = new TVector3(0,0,0);
    nuParentMomTargetExit   = new TVector3(0,0,0);
    leptonPos               = new std::vector<TLorentzVector>;
    leptonMom               = new std::vector<TLorentzVector>;
    p1PhotonConversionPos   = new std::vector<TLorentzVector>;
    p1PhotonConversionMom   = new std::vector<TLorentzVector>;
    p2PhotonConversionPos   = new std::vector<TLorentzVector>;
    p2PhotonConversionMom   = new std::vector<TLorentzVector>;
    miscPhotonConversionPos = new std::vector<TLorentzVector>;
    miscPhotonConversionMom = new std::vector<TLorentzVector>;
    PionPos                 = new std::vector<TLorentzVector>;
    PionMom                 = new std::vector<TLorentzVector>;
    ChargedPionPos          = new std::vector< std::vector<TLorentzVector> >;
    ChargedPionMom          = new std::vector< std::vector<TLorentzVector> >;

  }


Reprocessing::~Reprocessing()
{
    delete neutMom;
    delete vertex;
    delete GenieMomentum;
    delete ParentVertex;
    delete neutVertexInWindow;
    delete nuParentMomAtDecay;
    delete nuParentMomAtProd;
    delete nuParentMomTargetExit;
    delete leptonPos;
    delete leptonMom;
    delete p1PhotonConversionPos;
    delete p1PhotonConversionMom;
    delete p2PhotonConversionPos;
    delete p2PhotonConversionMom;
    delete miscPhotonConversionPos;
    delete miscPhotonConversionMom;
    delete PionPos;
    delete PionMom;
    delete ChargedPionPos;
    delete ChargedPionMom;

    if (!fChain) return;
    delete fChain->GetCurrentFile();
}

Int_t Reprocessing::GetEntry(Long64_t entry)
{

    // Read contents of entry.
    if (!fChain) return 0;
    int val = fChain->GetEntry(entry);


    GenieMomentum -> clear();
    leptonPos -> clear();
    leptonMom -> clear();
    p1PhotonConversionPos -> clear();
    p1PhotonConversionMom -> clear();
    p2PhotonConversionPos -> clear();
    p2PhotonConversionMom -> clear();
    miscPhotonConversionPos -> clear();
    miscPhotonConversionMom -> clear();
    PionPos -> clear();
    PionMom -> clear();
    ChargedPionPos -> clear();
    ChargedPionMom -> clear();


    // repackage the vectors:
    neutMom -> SetXYZT( neutMom_temp->at(1),neutMom_temp->at(2),
                        neutMom_temp->at(3),neutMom_temp->at(0));
    vertex -> SetXYZ(vertex_temp->at(0),vertex_temp->at(1),vertex_temp->at(2));

    // 1D vector of lorentzVector:
    GenieMomentum -> reserve(GenieMomentum_temp->size());
    for (auto & p : *GenieMomentum_temp){
      GenieMomentum -> push_back(TLorentzVector(p.at(1),p.at(2),p.at(3),p.at(0)));
    }
    // if (neutVertexInWindow_temp != 0)
    //   neutVertexInWindow          
    //       -> SetXYZ(  neutVertexInWindow_temp->at(0),
    //                   neutVertexInWindow_temp->at(1),
    //                   neutVertexInWindow_temp->at(2));
    ParentVertex          
          -> SetXYZ(  ParentVertex_temp->at(0),
                      ParentVertex_temp->at(1),
                      ParentVertex_temp->at(2));
    nuParentMomAtDecay    
          -> SetXYZ(  nuParentMomAtDecay_temp->at(0),
                      nuParentMomAtDecay_temp->at(1),
                      nuParentMomAtDecay_temp->at(2));
    nuParentMomAtProd     
          -> SetXYZ(  nuParentMomAtProd_temp->at(0),
                      nuParentMomAtProd_temp->at(1),
                      nuParentMomAtProd_temp->at(2));
    nuParentMomTargetExit 
          -> SetXYZ(  nuParentMomTargetExit_temp->at(0),
                      nuParentMomTargetExit_temp->at(1),
                      nuParentMomTargetExit_temp->at(2));
    
    // 1D vectors of lorentz vector, same size
    leptonPos -> reserve(leptonMom_temp->size());
    leptonMom -> reserve(leptonMom_temp->size());
    for (unsigned int i = 0; i < leptonMom_temp->size(); ++ i){
      leptonPos -> push_back(TLorentzVector( 
                      leptonPos_temp->at(i).at(1),
                      leptonPos_temp->at(i).at(2),
                      leptonPos_temp->at(i).at(3),
                      leptonPos_temp->at(i).at(0)));
      leptonMom -> push_back(TLorentzVector(
                      leptonMom_temp->at(i).at(1),
                      leptonMom_temp->at(i).at(2),
                      leptonMom_temp->at(i).at(3),
                      leptonMom_temp->at(i).at(0)));
    }
    // 1D vectors of lorentz vector, same size
    // 
    
    p1PhotonConversionPos -> reserve(p1PhotonConversionPos_temp->size());
    p1PhotonConversionMom -> reserve(p1PhotonConversionMom_temp->size());
    p2PhotonConversionPos -> reserve(p2PhotonConversionPos_temp->size());
    p2PhotonConversionMom -> reserve(p2PhotonConversionMom_temp->size());
    PionPos -> reserve(PionPos_temp->size());
    PionMom -> reserve(PionMom_temp->size());
    for (unsigned int i = 0; i < PionPos_temp->size(); ++ i){
      p1PhotonConversionPos -> push_back( TLorentzVector(
                      p1PhotonConversionPos_temp->at(i).at(1),
                      p1PhotonConversionPos_temp->at(i).at(2),
                      p1PhotonConversionPos_temp->at(i).at(3),
                      p1PhotonConversionPos_temp->at(i).at(0)));
      p1PhotonConversionMom -> push_back( TLorentzVector(
                      p1PhotonConversionMom_temp->at(i).at(1),
                      p1PhotonConversionMom_temp->at(i).at(2),
                      p1PhotonConversionMom_temp->at(i).at(3),
                      p1PhotonConversionMom_temp->at(i).at(0)));
      p2PhotonConversionPos -> push_back( TLorentzVector(
                      p2PhotonConversionPos_temp->at(i).at(1),
                      p2PhotonConversionPos_temp->at(i).at(2),
                      p2PhotonConversionPos_temp->at(i).at(3),
                      p2PhotonConversionPos_temp->at(i).at(0)));
      p2PhotonConversionMom -> push_back( TLorentzVector(
                      p2PhotonConversionMom_temp->at(i).at(1),
                      p2PhotonConversionMom_temp->at(i).at(2),
                      p2PhotonConversionMom_temp->at(i).at(3),
                      p2PhotonConversionMom_temp->at(i).at(0)));
      PionPos -> push_back( TLorentzVector(
                      PionPos_temp->at(i).at(1),
                      PionPos_temp->at(i).at(2),
                      PionPos_temp->at(i).at(3),
                      PionPos_temp->at(i).at(0)));
      PionMom -> push_back( TLorentzVector(
                      PionMom_temp->at(i).at(1),
                      PionMom_temp->at(i).at(2),
                      PionMom_temp->at(i).at(3),
                      PionMom_temp->at(i).at(0)));
    }
    // 1D vectors of lorentz vector, same size
    // same as before ...    
    miscPhotonConversionPos -> reserve(miscPhotonConversionPos_temp->size());
    miscPhotonConversionMom -> reserve(miscPhotonConversionMom_temp->size());
    for (unsigned int i = 0; i < miscPhotonConversionMom_temp->size(); ++ i){
      miscPhotonConversionPos -> push_back( TLorentzVector(
                      miscPhotonConversionPos_temp->at(i).at(1),
                      miscPhotonConversionPos_temp->at(i).at(2),
                      miscPhotonConversionPos_temp->at(i).at(3),
                      miscPhotonConversionPos_temp->at(i).at(0)));
      miscPhotonConversionMom -> push_back( TLorentzVector(
                      miscPhotonConversionMom_temp->at(i).at(1),
                      miscPhotonConversionMom_temp->at(i).at(2),
                      miscPhotonConversionMom_temp->at(i).at(3),
                      miscPhotonConversionMom_temp->at(i).at(0)));
    }

    // 2D vector of LorentzVector:
    ChargedPionPos -> resize(ChargedPionPos_temp->size());
    ChargedPionMom -> resize(ChargedPionMom_temp->size());
    for (unsigned int i = 0; i < ChargedPionPos_temp->size(); ++ i){
      ChargedPionPos -> at(i).reserve(ChargedPionPos_temp->at(i).size());
      ChargedPionMom -> at(i).reserve(ChargedPionMom_temp->at(i).size());  
      for (unsigned int j = 0; j < ChargedPionPos_temp->at(i).size();++j){
        ChargedPionPos -> at(i).push_back(TLorentzVector(
                      ChargedPionPos_temp->at(i).at(j).at(1),
                      ChargedPionPos_temp->at(i).at(j).at(2),
                      ChargedPionPos_temp->at(i).at(j).at(3),
                      ChargedPionPos_temp->at(i).at(j).at(0)));
        ChargedPionMom -> at(i).push_back(TLorentzVector(
                      ChargedPionMom_temp->at(i).at(j).at(1),
                      ChargedPionMom_temp->at(i).at(j).at(2),
                      ChargedPionMom_temp->at(i).at(j).at(3),
                      ChargedPionMom_temp->at(i).at(j).at(0)));
      }
    }
    
   return val;
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
   neutMom_temp = 0;
   vertex_temp = 0;
   GeniePDG = 0;
   GenieMomentum_temp = 0;
   GenieProc = 0;
   neutVertexInWindow_temp = 0;
   ParentVertex_temp = 0;
   nuParentMomAtDecay_temp = 0;
   nuParentMomAtProd_temp = 0;
   nuParentMomTargetExit_temp = 0;
   leptonPos_temp = 0;
   leptonMom_temp = 0;
   p1PhotonConversionPos_temp = 0;
   p1PhotonConversionMom_temp = 0;
   p2PhotonConversionPos_temp = 0;
   p2PhotonConversionMom_temp = 0;
   miscPhotonConversionPos_temp = 0;
   miscPhotonConversionMom_temp = 0;
   PionPos_temp = 0;
   PionMom_temp = 0;
   ChargedPionPos_temp = 0;
   ChargedPionMom_temp = 0;
   ChargedPionSign = 0;
   MultiWeight = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("POT", &POT, &b_POT);
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
   fChain->SetBranchAddress("neutMom", &neutMom_temp, &b_neutMom);
   fChain->SetBranchAddress("vertex", &vertex_temp, &b_vertex);
   fChain->SetBranchAddress("GeniePDG", &GeniePDG, &b_GeniePDG);
   fChain->SetBranchAddress("GenieMomentum", &GenieMomentum_temp, &b_GenieMomentum);
   fChain->SetBranchAddress("GenieProc", &GenieProc, &b_GenieProc);
   fChain->SetBranchAddress("ptype", &ptype, &b_ptype);
   fChain->SetBranchAddress("tptype", &tptype, &b_tptype);
   fChain->SetBranchAddress("ndecay", &ndecay, &b_ndecay);
   fChain->SetBranchAddress("neutVertexInWindow", &neutVertexInWindow_temp, &b_neutVertexInWindow);
   fChain->SetBranchAddress("ParentVertex", &ParentVertex_temp, &b_ParentVertex);
   fChain->SetBranchAddress("nuParentMomAtDecay", &nuParentMomAtDecay_temp, &b_nuParentMomAtDecay);
   fChain->SetBranchAddress("nuParentMomAtProd", &nuParentMomAtProd_temp, &b_nuParentMomAtProd);
   fChain->SetBranchAddress("nuParentMomTargetExit", &nuParentMomTargetExit_temp, &b_nuParentMomTargetExit);
   fChain->SetBranchAddress("leptonPos", &leptonPos_temp, &b_leptonPos);
   fChain->SetBranchAddress("leptonMom", &leptonMom_temp, &b_leptonMom);
   fChain->SetBranchAddress("NPi0", &NPi0, &b_NPi0);
   fChain->SetBranchAddress("NPi0FinalState", &NPi0FinalState, &b_NPi0FinalState);
   fChain->SetBranchAddress("NGamma", &NGamma, &b_NGamma);
   fChain->SetBranchAddress("FoundPhotons", &FoundPhotons, &b_FoundAllPhotons);
   fChain->SetBranchAddress("p1PhotonConversionPos", &p1PhotonConversionPos_temp, &b_p1PhotonConversionPos);
   fChain->SetBranchAddress("p1PhotonConversionMom", &p1PhotonConversionMom_temp, &b_p1PhotonConversionMom);
   fChain->SetBranchAddress("p2PhotonConversionPos", &p2PhotonConversionPos_temp, &b_p2PhotonConversionPos);
   fChain->SetBranchAddress("p2PhotonConversionMom", &p2PhotonConversionMom_temp, &b_p2PhotonConversionMom);
   fChain->SetBranchAddress("miscPhotonConversionPos", &miscPhotonConversionPos_temp, &b_miscPhotonConversionPos);
   fChain->SetBranchAddress("miscPhotonConversionMom", &miscPhotonConversionMom_temp, &b_miscPhotonConversionMom);
   fChain->SetBranchAddress("PionPos", &PionPos_temp, &b_PionPos);
   fChain->SetBranchAddress("PionMom", &PionMom_temp, &b_PionMom);
   fChain->SetBranchAddress("ChargedPionPos", &ChargedPionPos_temp, &b_ChargedPionPos);
   fChain->SetBranchAddress("ChargedPionMom", &ChargedPionMom_temp, &b_ChargedPionMom);
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
      
      lepDir.SetX(GenieMomentum->at(i).X());
      lepDir.SetY(GenieMomentum->at(i).Y());
      lepDir.SetZ(GenieMomentum->at(i).Z());
      ThetaLep = utils.GetTheta( lepDir );
      PhiLep = utils.GetPhi( lepDir );
      Elep = GenieMomentum->at(i).E();
      return;      
    }
  }

  for( unsigned long i = 0; i < GeniePDG->size(); i++ ){
    //std::cout << "GENIE PDG: " << GeniePDG->at(i) << std::endl;
    if( abs(GeniePDG->at(i)) == 11 || abs(GeniePDG->at(i)) == 12 ||
        abs(GeniePDG->at(i)) == 13 || abs(GeniePDG->at(i)) == 14 ){
      
      lepDir.SetX(GenieMomentum->at(i).X());
      lepDir.SetY(GenieMomentum->at(i).Y());
      lepDir.SetZ(GenieMomentum->at(i).Z());
    }
  }

  ThetaLep = utils.GetTheta( lepDir );
  PhiLep = utils.GetPhi( lepDir);

  return;
}

} //namespace lar1

#endif // #ifdef Reprocessing_cxx
