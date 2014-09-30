#ifndef PDGSELECTION_CC
#define PDGSELECTION_CC

#include "PDGSelection.hh"

namespace larlight {

  PDGSelection::PDGSelection() : _pdg_list(PDGSelection::kPARTICLE_GROUP_MAX,std::set<int>())
  { 
    _name="PDGSelection"; 
    _fout=0;
    Reset();
  }

  void PDGSelection::Reset()
  {

    for(auto& list : _pdg_list) list.clear();

    _nevent_analyzed = 0;
    _nevent_selected = 0;

  }

  void PDGSelection::Select(int const pdg_code, 
			    PDGSelection::PARTICLE_GROUP const part_type)
  {
    if(part_type == PDGSelection::kPARTICLE_GROUP_MAX) 
      
      throw ::showerana::ShowerAnaException(Form("<<%s>> Invalid particle type: %d",
						 __FUNCTION__,
						 part_type)
					    );

    _pdg_list.at(part_type).insert(pdg_code);

  }

  bool PDGSelection::initialize() {

    _nevent_analyzed = 0;

    _nevent_selected = 0;

    // Append kANY to kPRIMARY and kSECONDARY
    for(auto const& id : _pdg_list.at(kANY)) {

      _pdg_list.at(kPRIMARY).insert(id);
      _pdg_list.at(kSECONDARY).insert(id);

    }

    return true;
  }
  
  bool PDGSelection::analyze(storage_manager* storage) {

    // Log # of analyze() function calls
    ++_nevent_analyzed;

    // Process if a user specified to look into a primary particle list
    if(_pdg_list.at(kPRIMARY).size()) {

      // Save typing by making a local reference to the subject list of PDG code
      auto const& pdg_list = _pdg_list.at(kPRIMARY);

      // Pull out larlight::event_mctruth data product
      auto ev_mct = (const event_mctruth*)(storage->get_data(DATA::MCTruth));

      // Check if this is a valid data product
      if(!ev_mct) 
	
	throw ::showerana::ShowerAnaException(Form("<<%s>> MCTruth information not found!",
						   __FUNCTION__)
					      );

      // Loop over a vector of larlight::mctruth data product
      for(auto const& mct : *ev_mct) {
	
	// Loop over a vector of larlight::mcpart that is contained in this larlight::mctruth
	for(auto const& mcp : mct.GetParticles()) {

	  // Check if this particle's PDG code is in the list for selection candidates
	  if( pdg_list.find(mcp.PdgCode()) != pdg_list.end() ) {

	    // If so, increment the selection counter & return true
	    ++_nevent_selected;
	    return true;
	    
	  }
	}
      }
    }  

    // Process if a user specified to look into a secondary particle list
    if(_pdg_list.at(kSECONDARY).size()) {

      // Save typing by making a local reference to the subject list of PDG code
      auto const& pdg_list = _pdg_list.at(kSECONDARY);

      // Pull out larlight::event_mcpart data product
      auto ev_mcp = (const event_mcpart*)(storage->get_data(DATA::MCParticle));

      // Check data product validity
      if(!ev_mcp) 

	throw ::showerana::ShowerAnaException(Form("<<%s>> MCTruth information not found!",
						   __FUNCTION__)
					      );

      // Loop over a vector of larlight::mcpart data proudct
      for(auto const& mcp : *ev_mcp) 

	// Check if this is a non-primary particle && PDG code is in the list for selection candidates
	if( mcp.Mother() && pdg_list.find(mcp.PdgCode()) != pdg_list.end() ) {

	  // If so, increment the selection counter & return true
	  ++_nevent_selected;
	  return true;

	}

    }

    // Reaching this point means this event did not pass a selection criteria. Return false.
    return false;
  }

  bool PDGSelection::finalize() {

    print(MSG::NORMAL,__FUNCTION__,Form("Selected %zu out of %zu events...",
					_nevent_selected,
					_nevent_analyzed)
	  );

    return true;
  }

}
#endif
