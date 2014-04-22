//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Sep  6 16:58:09 2013 by ROOT version 5.34/05
// from TTree EventsTot/Event info for ALL types
// found on file: ntuple_example.root
//////////////////////////////////////////////////////////

#ifndef NtupleReprocessing_h
#define NtupleReprocessing_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <vector>
#include "LorentzVectorLight.hh"
#include <iostream>
#include <iomanip>
#include "Utils.hh"

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class NtupleReprocessing {

 public :

  TFile          *infile;
  TTree          *fChain;   // !pointer to the analyzed TTree or TChain
  Int_t           fCurrent; // !current Tree number in a TChain
  
  // Declaration of leaf types
  Int_t           iflux;
  Int_t           ibkg;
  Int_t           nuchan;
  Int_t           inno;
  Double_t        enugen;
  Double_t        energy;
  Double_t        nuleng;
  Int_t           parid;
  Int_t           ndecay;
  Double_t        wgt;
  Int_t           NPi0;
  Int_t           NPi0FinalState;
  Int_t           NGamma;
  Char_t          FoundPhotons;
  Int_t           isCC;
  Int_t           mode;
  //Entries involving genie FSP:
  std::vector<Int_t>     *geniePDG;
  std::vector<Double_t>  *genieE;
  std::vector<Double_t>  *geniePx;
  std::vector<Double_t>  *geniePy;
  std::vector<Double_t>  *geniePz; 
  Double_t        Vx;
  Double_t        Vy;
  Double_t        Vz;
  Double_t        ParVx;
  Double_t        ParVy;
  Double_t        ParVz;
  Double_t        ParPx;
  Double_t        ParPy;
  Double_t        ParPz;
  Double_t        LepPx;    // Added these to store the lepton mometum.
  Double_t        LepPy;    // Added these to store the lepton mometum.
  Double_t        LepPz;    // Added these to store the lepton mometum.
  Double_t        LepAngle; // This will store the scattering angle between lepton and neutrino.
  Double_t        ThetaLep;
  Double_t        PhiLep;
  Double_t        ThetaLepSmeared;
  Double_t        PhiLepSmeared;
  Double_t        Elep;
  Double_t        ElepSmeared;
  std::vector<gan::LorentzVectorLight> *MuonPos;
  std::vector<gan::LorentzVectorLight> *MuonMom;
  std::vector<gan::LorentzVectorLight> *p1PhotonConversionPos;
  std::vector<gan::LorentzVectorLight> *p1PhotonConversionMom;
  std::vector<gan::LorentzVectorLight> *p2PhotonConversionPos;
  std::vector<gan::LorentzVectorLight> *p2PhotonConversionMom;
  std::vector<gan::LorentzVectorLight> *miscPhotonConversionPos;
  std::vector<gan::LorentzVectorLight> *miscPhotonConversionMom;
  std::vector<gan::LorentzVectorLight> *PionPos;
  std::vector<gan::LorentzVectorLight> *PionMom;
  // vector<vector< gan::LorentzVectorLight> > * chargedPionPos;
  // vector<vector< gan::LorentzVectorLight> > * chargedPionMom;
  // vector<gan::LorentzVectorLight> * chargedPionSign;

  // List of branches
  TBranch        *b_iflux;   //!
  TBranch        *b_ibkg;   //!
  TBranch        *b_nuchan;   //!
  TBranch        *b_inno;   //!
  TBranch        *b_enugen;   //!
  TBranch        *b_energy;   //!
  TBranch        *b_nuleng;   //!
  TBranch        *b_parid;   //!
  TBranch        *b_ndecay;   //!
  TBranch        *b_wgt;   //!
  TBranch        *b_NPi0;   //!
  TBranch        *b_NPi0FinalState;   //!
  TBranch        *b_NGamma;   //!
  TBranch        *b_FoundAllPhotons;   //!
  TBranch        *b_isCC;   //!
  TBranch        *b_mode;   //!
  TBranch        *b_Vx;   //!
  TBranch        *b_Vy;   //!
  TBranch        *b_Vz;   //!
  TBranch        *b_ParVx;   //!
  TBranch        *b_ParVy;   //!
  TBranch        *b_ParVz;   //!
  TBranch        *b_ParPx;   //!
  TBranch        *b_ParPy;   //!
  TBranch        *b_ParPz;   //!
  TBranch        *b_geniePDG; //!
  TBranch        *b_genieE; //!
  TBranch        *b_geniePx; //!
  TBranch        *b_geniePy; //!
  TBranch        *b_geniePz; //!
  TBranch        *b_ThetaLep;   //!
  TBranch        *b_PhiLep;   //!
  TBranch        *b_ThetaLepSmeared;   //!
  TBranch        *b_PhiLepSmeared;   //!
  TBranch        *b_Elep;   //!
  TBranch        *b_ElepSmeared;   //!
  TBranch        *b_MuonPos;   //!
  TBranch        *b_MuonMom;   //!
  TBranch        *b_p1PhotonConversionPos;   //!
  TBranch        *b_p1PhotonConversionMom;   //!
  TBranch        *b_p2PhotonConversionPos;   //!
  TBranch        *b_p2PhotonConversionMom;   //!
  TBranch        *b_miscPhotonConversionPos;   //!
  TBranch        *b_miscPhotonConversionMom;   //!
  TBranch        *b_PionPos;   //!
  TBranch        *b_PionMom;   //!
  // TBranch        *b_chargedPionPos;
  // TBranch        *b_chargedPionMom;
  // TBranch        *b_chargedPionSign;

  NtupleReprocessing(TString file = "");
  virtual ~NtupleReprocessing();
  virtual Int_t    Cut(Long64_t entry);
  virtual Int_t    GetEntry(Long64_t entry);
  virtual Long64_t LoadTree(Long64_t entry);
  virtual void     Init(TTree *tree);
  virtual void     Loop(std::string, Int_t iDet = 1, Long64_t max_entry = -1, bool verbose = false, double scale = 1);
  virtual Bool_t   Notify();
  virtual void     Show(Long64_t entry = -1);
  virtual TString  InFile();
  virtual Double_t CalcLepton( Double_t detect_dist );

  // Call these methods to disable oscillation or detection
  // of certain signs of neutrinos.  For example,
  // DisableOscSign(1) will disable neutrinos from oscillating
  // DisableDetSign(-1) will disable antineutrinos from being detected
  // The Osc version is to change oscillation signal, the Det version 
  // is to simulate perfect sign selection
  virtual Int_t    DisableOscSign(int sign);
  virtual Int_t    DisableDetSign(int sign);
  void SetSpecialNameText(std::string s);

  Utils utils;
  
  bool oscNeutrinos, oscAntiNeutrinos;
  bool detNeutrinos, detAntiNeutrinos;

  std::string specialNameText;

};

#endif

#ifdef NtupleReprocessing_cxx
NtupleReprocessing::NtupleReprocessing(TString file) : fChain(0) {

  // if parameter tree is not specified (or zero), connect the file
  // used to generate this class and read the Tree.
  gROOT -> ProcessLine(".L LorentzVectorLight.h+");

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
  
  // Initialize the setup booleans:
  oscNeutrinos     = true;
  oscAntiNeutrinos = true;
  detNeutrinos     = true;
  detAntiNeutrinos = true;
  specialNameText  = "";


}

NtupleReprocessing::~NtupleReprocessing()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t NtupleReprocessing::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   int returnVal = fChain->GetEntry(entry);
   return returnVal;
}

Long64_t NtupleReprocessing::LoadTree(Long64_t entry)
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

void NtupleReprocessing::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   MuonPos = 0;
   MuonMom = 0;
   p1PhotonConversionPos = 0;
   p1PhotonConversionMom = 0;
   p2PhotonConversionPos = 0;
   p2PhotonConversionMom = 0;
   miscPhotonConversionPos = 0;
   miscPhotonConversionMom = 0;
   PionPos = 0;
   PionMom = 0;
   geniePDG = 0;
   genieE = 0;
   geniePx = 0;
   geniePy = 0;
   geniePz = 0;

   // chargedPionSign = 0;
   // chargedPionPos = 0;
   // chargedPionMom = 0;

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("iflux", &iflux, &b_iflux);
   fChain->SetBranchAddress("ibkg", &ibkg, &b_ibkg);
   fChain->SetBranchAddress("nuchan", &nuchan, &b_nuchan);
   fChain->SetBranchAddress("inno", &inno, &b_inno);
   fChain->SetBranchAddress("enugen", &enugen, &b_enugen);
   fChain->SetBranchAddress("energy", &energy, &b_energy);
   fChain->SetBranchAddress("nuleng", &nuleng, &b_nuleng);
   fChain->SetBranchAddress("parid", &parid, &b_parid);
   fChain->SetBranchAddress("ndecay", &ndecay, &b_ndecay);
   fChain->SetBranchAddress("wgt", &wgt, &b_wgt);
   fChain->SetBranchAddress("NPi0", &NPi0, &b_NPi0);
   fChain->SetBranchAddress("NPi0FinalState", &NPi0FinalState, &b_NPi0FinalState);
   fChain->SetBranchAddress("NGamma", &NGamma, &b_NGamma);
   fChain->SetBranchAddress("FoundPhotons", &FoundPhotons, &b_FoundAllPhotons);
   fChain->SetBranchAddress("isCC", &isCC, &b_isCC);
   fChain->SetBranchAddress("mode", &mode, &b_mode);
   fChain->SetBranchAddress("Vx", &Vx, &b_Vx);
   fChain->SetBranchAddress("Vy", &Vy, &b_Vy);
   fChain->SetBranchAddress("Vz", &Vz, &b_Vz);

   fChain->SetBranchAddress("GeniePDG", &geniePDG, &b_geniePDG);
   fChain->SetBranchAddress("GenieE",   &genieE, &b_genieE);
   fChain->SetBranchAddress("GeniePx", &geniePx, &b_geniePx);
   fChain->SetBranchAddress("GeniePy", &geniePy, &b_geniePy);
   fChain->SetBranchAddress("GeniePz", &geniePz, &b_geniePz);

   fChain->SetBranchAddress("ParVx", &ParVx, &b_ParVx);
   fChain->SetBranchAddress("ParVy", &ParVy, &b_ParVy);
   fChain->SetBranchAddress("ParVz", &ParVz, &b_ParVz);
   fChain->SetBranchAddress("ParPx", &ParPx, &b_ParPx);
   fChain->SetBranchAddress("ParPy", &ParPy, &b_ParPy);
   fChain->SetBranchAddress("ParPz", &ParPz, &b_ParPz);
   fChain->SetBranchAddress("ThetaLep", &ThetaLep, &b_ThetaLep);
   fChain->SetBranchAddress("PhiLep", &PhiLep, &b_PhiLep);
   fChain->SetBranchAddress("ThetaLepSmeared", &ThetaLepSmeared, &b_ThetaLepSmeared);
   fChain->SetBranchAddress("PhiLepSmeared", &PhiLepSmeared, &b_PhiLepSmeared);
   fChain->SetBranchAddress("Elep", &Elep, &b_Elep);
   fChain->SetBranchAddress("ElepSmeared", &ElepSmeared, &b_ElepSmeared);
   fChain->SetBranchAddress("MuonPos", &MuonPos, &b_MuonPos);
   fChain->SetBranchAddress("MuonMom", &MuonMom, &b_MuonMom);
   fChain->SetBranchAddress("p1PhotonConversionPos", &p1PhotonConversionPos, &b_p1PhotonConversionPos);
   fChain->SetBranchAddress("p1PhotonConversionMom", &p1PhotonConversionMom, &b_p1PhotonConversionMom);
   fChain->SetBranchAddress("p2PhotonConversionPos", &p2PhotonConversionPos, &b_p2PhotonConversionPos);
   fChain->SetBranchAddress("p2PhotonConversionMom", &p2PhotonConversionMom, &b_p2PhotonConversionMom);
   fChain->SetBranchAddress("miscPhotonConversionPos", &miscPhotonConversionPos, &b_miscPhotonConversionPos);
   fChain->SetBranchAddress("miscPhotonConversionMom", &miscPhotonConversionMom, &b_miscPhotonConversionMom);
   fChain->SetBranchAddress("PionPos", &PionPos, &b_PionPos);
   fChain->SetBranchAddress("PionMom", &PionMom, &b_PionMom);

   // fChain->SetBranchAddress("chargedPionPos",&chargedPionPos,&b_chargedPionPos);
   // fChain->SetBranchAddress("chargedPionMom",&chargedPionMom,&b_chargedPionMom);
   // fChain->SetBranchAddress("chargedPionSign",&chargedPionSign,&b_chargedPionSign);

   Notify();
}

Bool_t NtupleReprocessing::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void NtupleReprocessing::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}

Int_t NtupleReprocessing::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   // Doing so silly things to make the compiler not complain:
   if (entry) return 1;
   else return 1;
}

TString NtupleReprocessing::InFile(){

  return infile->GetName();

}

Double_t NtupleReprocessing::CalcLepton( Double_t detect_dist ){

  LepPx = 0.0;
  LepPy = 0.0;
  LepPz = 0.0;
  LepAngle = 0.0;

  // loop over Genie vectors and find the lepton.  These are 11, 12, 13, 14 and negatives.

  for( unsigned long i = 0; i < geniePDG->size(); i++ ){
    //std::cout << "GENIE PDG: " << geniePDG->at(i) << std::endl;
    if( abs(geniePDG->at(i)) == 11 || abs(geniePDG->at(i)) == 12 ||
	      abs(geniePDG->at(i)) == 13 || abs(geniePDG->at(i)) == 14 ){
      
      LepPx = geniePx->at(i);
      LepPy = geniePy->at(i);
      LepPz = geniePz->at(i);
    }
  }

  ThetaLep = utils.GetTheta( LepPx, LepPy, LepPz );
  PhiLep = utils.GetPhi( LepPx, LepPy );

  TVector3 length( ParVx - Vx, ParVy - Vy, detect_dist + Vz - ParVz);  // cm
  
  return length.Mag();

}

Int_t NtupleReprocessing::DisableDetSign(Int_t sign){

  if (sign == 1 ){
    detNeutrinos = false;
  }
  if (sign == -1 ){
    detAntiNeutrinos = false;
  }
  if (!detNeutrinos && !detAntiNeutrinos){
    std::cerr << "\nWarning!  No neutrinos are set to be detected!!!\n"<<std::endl;
    return 0;
  }
  return sign;
}
Int_t NtupleReprocessing::DisableOscSign(Int_t sign){

  if (sign == 1 ){
    oscNeutrinos = false;
  }
  if (sign == -1 ){
    oscAntiNeutrinos = false;
  }
  if (!oscNeutrinos && !oscAntiNeutrinos){
    std::cerr << "\nWarning!  No neutrinos are set to be oscillated!!!\n"<<std::endl;
    return 0;
  }
  return sign;
}

void NtupleReprocessing::SetSpecialNameText(std::string s){
  specialNameText = s;
}


#endif // #ifdef NtupleReprocessing_cxx
