
#include "ToyAna.hh"
#include <TFile.h>
#include <TChain.h>

int main(int argc, char** argv) {

  TChain* ch = new TChain("myana/toy_tree","");

  if(argc<2) {
    std::cerr 
      << "You must provide input file as 1st argument"
      << std::endl;
    return 1;
  }
  
  ch->AddFile(argv[1]);
  
  ToyAna my_ana;

  my_ana.RunEventLoop(ch);

  return 0;
}

