#ifndef MCGETTER_CC
#define MCGETTER_CC

#include "MCgetter.hh"

namespace larlight {

  void MCgetter::fillParticleMap(const event_mcpart *event_part){

    //clear map before fillint
    _particleMap.clear();
    //Fill the Map for particles and trackIDs
    for (size_t i=0; i<event_part->size(); i++)
      _particleMap[event_part->at(i).TrackId()] = i;

    return;

  }

  void MCgetter::drawDaughters(const event_mcpart *event_part, mcpart particle ){

    if (_particleMap.size() == 0){
      std::cout << "Particle Map not filled...nowhere to search for daughters..." << std::endl;
      return;
    }

    std::cout << particle.Process()
	      << "\tTrackID:\t" << particle.TrackId()
	      << "\tPdgCode:\t" << particle.PdgCode()
	      << "\tEnergy:\t"  << particle.Trajectory().at(0).E()
	      << "\tTime:\t"    << particle.Trajectory().at(0).T()
	      << "\tNDaughters:\t"  << particle.Daughters().size()
	      << std::endl;
    std::cout << "Daughters:" << std::endl;

    std::set<Int_t> daughters = particle.Daughters();
  
    //loop over all daughters
    for (std::set<int>::iterator it=daughters.begin(); it!=daughters.end(); ++it){

      mcpart tmpPart = event_part->at( _particleMap.find(*it)->second );

      //only if energy > 0.1 MeV
      if ( tmpPart.Trajectory().at(0).E() > _Ecut )
	std::cout << tmpPart.Process()
		  << "\tTrackID:\t" << tmpPart.TrackId()
		  << "\tPdgCode:\t" << tmpPart.PdgCode()
		  << "\tEnergy:\t"  << tmpPart.Trajectory().at(0).E()
		  << "\tTime:\t"    << tmpPart.Trajectory().at(0).T()
		  << "\tNDaughters:\t"  << tmpPart.Daughters().size()
		  << std::endl;
    }
    std::cout << std::endl << std::endl;

    int track;
    std::cout << "Enter Track Number to Show Daughters for: [0 = quit]" << std::endl;
    std::cin  >> track;

    //if quitting
    if ( track == 0 )
      return;
    
    //otherwise find particle with this trackID
    mcpart shownext = event_part->at( _particleMap.find(track)->second );
    drawDaughters(event_part, shownext);
    return;
  }


}
#endif
