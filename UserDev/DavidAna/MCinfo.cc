#ifndef MCINFO_CC
#define MCINFO_CC

#include "MCinfo.hh"
#include "math.h"

namespace larlight {

  bool MCinfo::initialize() {
    
    _evtN = 0;

    return true;
  }
  
  bool MCinfo::analyze(storage_manager* storage) {

    //std::cout << "Event Number: " << _evtN << std::endl;
    
    const event_mcpart *event_part = (event_mcpart*)(storage->get_data(DATA::MCParticle));
    
    //Loop over all particles in event
    for (size_t i=0; i<event_part->size(); i++){
      
      mcpart particle = event_part->at(i);
      
      //std::cout << "Particle PDG: " << particle.PdgCode() << std::endl;
      if ( particle.Process() != "primary" )
	std::cout << "Particle Process: " << particle.Process() << std::endl;
    
    }//End for each particle
    
    _evtN += 1;

    return true;
    
  }//End Analyze Loop
  
  bool MCinfo::finalize() {
    
    return true;
  }
}
#endif
