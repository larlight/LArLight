#ifndef TOYANA_CC
#define TOYANA_CC

#include "ToyAna.hh"

void ToyAna::RunEventLoop(TTree* tree)
{
  //
  // This function runs an event loop on input TTree and attempts to read ToyEvent data product
  //

  // Step 1) check if ToyEvent branch exists
  if(!(tree->GetBranch("toy_event_branch"))) {

    std::cerr
      << "\033[93m" 
      << "Input TTree has no branch called \"toy_event_branch\"!" 
      << "\033[00m" << std::endl;
    return;
  }
  
  // Step 2) add a handle to read-out data
  ToyEvent* toy_event = new ToyEvent;
  tree->SetBranchAddress("toy_event_branch",&toy_event);
  
  // Step 3) start event loop
  for(size_t i=0; i<tree->GetEntries(); ++i) {

    std::cout << "TTree entry: " << i << std::endl;
    tree->GetEntry(i);

    std::cout << "  Found " << toy_event->size() << " particles!" << std::endl;
    std::cout << "  Listing PDG ID of particles..." << std::flush;

    for(auto const& toy_data : *toy_event)
      
      std::cout << toy_data.pdg_id() << " ";

    std::cout << std::endl;
    std::cout << "done with entry " << i << std::endl;
    std::cout << std::endl;

  }
  
}


#endif
