//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Nov 15 08:09:14 2013 by ROOT version 5.34/05
// from TChain bezier/bezier Tree
//////////////////////////////////////////////////////////

#ifndef aho_h
#define aho_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class aho {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           entry;
   Int_t           ntracks;
   Int_t           trackId[1];   //[ntracks]
   Double_t        startx[1];   //[ntracks]
   Double_t        starty[1];   //[ntracks]
   Double_t        startz[1];   //[ntracks]
   Double_t        startd[1];   //[ntracks]
   Double_t        endx[1];   //[ntracks]
   Double_t        endy[1];   //[ntracks]
   Double_t        endz[1];   //[ntracks]
   Double_t        endd[1];   //[ntracks]
   Double_t        theta[1];   //[ntracks]
   Double_t        phi[1];   //[ntracks]
   Double_t        theta_xz[1];   //[ntracks]
   Double_t        ttheta_yz[1];   //[ntracks]
   Double_t        mom[1];   //[ntracks]
   Double_t        len[1];   //[ntracks]

   // List of branches
   TBranch        *b_entry;   //!
   TBranch        *b_ntracks;   //!
   TBranch        *b_trackId;   //!
   TBranch        *b_startx;   //!
   TBranch        *b_starty;   //!
   TBranch        *b_startz;   //!
   TBranch        *b_startd;   //!
   TBranch        *b_endx;   //!
   TBranch        *b_endy;   //!
   TBranch        *b_endz;   //!
   TBranch        *b_endd;   //!
   TBranch        *b_theta;   //!
   TBranch        *b_phi;   //!
   TBranch        *b_theta_xz;   //!
   TBranch        *b_ttheta_yz;   //!
   TBranch        *b_mom;   //!
   TBranch        *b_len;   //!

   aho(TTree *tree=0);
   virtual ~aho();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef aho_cxx
aho::aho(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {

#ifdef SINGLE_TREE
      // The following code should be used if you want this class to access
      // a single tree instead of a chain
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("out.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("out.root");
      }
      f->GetObject("bezier",tree);

#else // SINGLE_TREE

      // The following code should be used if you want this class to access a chain
      // of trees.
      TChain * chain = new TChain("bezier","bezier Tree");
      chain->Add("out.root/bezier");
      tree = chain;
#endif // SINGLE_TREE

   }
   Init(tree);
}

aho::~aho()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t aho::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t aho::LoadTree(Long64_t entry)
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

void aho::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("entry", &entry, &b_entry);
   fChain->SetBranchAddress("ntracks", &ntracks, &b_ntracks);
   fChain->SetBranchAddress("trackId", &trackId, &b_trackId);
   fChain->SetBranchAddress("startx", &startx, &b_startx);
   fChain->SetBranchAddress("starty", &starty, &b_starty);
   fChain->SetBranchAddress("startz", &startz, &b_startz);
   fChain->SetBranchAddress("startd", &startd, &b_startd);
   fChain->SetBranchAddress("endx", &endx, &b_endx);
   fChain->SetBranchAddress("endy", &endy, &b_endy);
   fChain->SetBranchAddress("endz", &endz, &b_endz);
   fChain->SetBranchAddress("endd", &endd, &b_endd);
   fChain->SetBranchAddress("theta", &theta, &b_theta);
   fChain->SetBranchAddress("phi", &phi, &b_phi);
   fChain->SetBranchAddress("theta_xz", &theta_xz, &b_theta_xz);
   fChain->SetBranchAddress("ttheta_yz", &ttheta_yz, &b_ttheta_yz);
   fChain->SetBranchAddress("mom", &mom, &b_mom);
   fChain->SetBranchAddress("len", &len, &b_len);
   Notify();
}

Bool_t aho::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void aho::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t aho::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef aho_cxx
