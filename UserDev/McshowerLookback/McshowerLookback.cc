#ifndef MCSHOWERLOOKBACK_CC
#define MCSHOWERLOOKBACK_CC

#include "McshowerLookback.hh"

namespace larlight {
  ///////////////////////////////////////////////////
  //Make map of trackIDs for each shower           //
  ///////////////////////////////////////////////////
  void McshowerLookback::FillShowerMap(larlight::event_mcshower* my_mcshow, 
				       std::map<UInt_t,UInt_t> &shower_idmap)
  {
    UInt_t mcshower_index = 0;
    
    for(auto this_mcshow : *my_mcshow){

      //if mcshower mother is too low energy, don't put this shower index into the mapping
      //MotherMomentum().at(3) looks like it's just nonsense .... sum the components squared
      double this_mother_energy = 0;
      for(int i = 0; i < 3; ++i)
	this_mother_energy += pow(this_mcshow.MotherMomentum().at(i),2);
      this_mother_energy = pow(this_mother_energy, 0.5);

      if(this_mother_energy < _cutoff_energy) continue;
      
      for(auto this_trackid : (std::vector<UInt_t>)this_mcshow.DaughterTrackID())
	shower_idmap.insert(std::pair<UInt_t,UInt_t>(this_trackid,mcshower_index));
      
      mcshower_index++;
    }
    
  }
  
  
  ///////////////////////////////////////////////////
  //Make map of simchannel objects for each channel//
  ///////////////////////////////////////////////////
  void McshowerLookback::FillSimchMap(larlight::event_simch* my_simch, 
				      std::map<UShort_t, larlight::simch> &simch_map){
    
    for(auto this_simch : *my_simch)
      simch_map.insert(std::pair<UShort_t,larlight::simch>(this_simch.Channel(),this_simch));
    
  }
  
  ///////////////////////////////////////////////////
  //Find % of each shower in a single hit          //
  ///////////////////////////////////////////////////
  std::vector<float> McshowerLookback::MatchHitsAll(const larlight::hit &this_hit, 
						    const std::map<UShort_t, larlight::simch> &simch_map, 
						    const std::map<UInt_t,UInt_t> &shower_idmap, 
						    const std::vector<UInt_t> &MCShower_indices)
  {
    ////////////////////////////////////////////////////////////////
    //setup variables for holding charge info from various mcshowers
    ////////////////////////////////////////////////////////////////
    
    //total MC charge deposited on that hit's wire during that hit's start->end time
    float tot_ides_charge = 0;
    //amount of MC charge deposited "" "" belonging to a given MCShower
    std::vector<float> part_ides_charge((size_t)MCShower_indices.size(),0.);
    //amount of MC charge deposited "" "" belonging to MCShower not in the map
    float part_ides_charge_unknown_MCShower = 0;
    
    ////////////////////////////////////////////////////////////////
    //Search for simchannels on hit's channel number
    ////////////////////////////////////////////////////////////////
    
    auto simch_iter = simch_map.find(this_hit.Channel());
    if(simch_iter==simch_map.end()) 
      std::cerr<<"Hit has no matched simchannel!!"<<std::endl;
    
    else{
      
      ////////////////////////////////////////////////////////////////
      //Found the right simchannel, so try to find ides on this simch
      ////////////////////////////////////////////////////////////////
      
      //(*simch_iter).second is the larlight::simch object
      //maybe i can speed up code here by not making this big vector<ide> object... how?
      std::vector<larlight::ide> matchedides((*simch_iter).second.TrackIDsAndEnergies(this_hit.StartTime(),this_hit.EndTime()));
      
      //sometimes you find a reco hit that doesn't correspond to an IDE
      //probably the IDE does not fall within the reco-d hit range (start->end time)
      if(matchedides.size()==0){
	std::cerr<<"Warning. This reco hit didn't correspond to any IDE object. Returning a null vector."<<std::endl;
	std::vector<float> bad(0,-1);
	return bad;
      }
      
      ////////////////////////////////////////////////////////////////
      //Look for the trackIDs in MCShower that belong to each found IDE
      ////////////////////////////////////////////////////////////////
      
      for(auto const &this_ide : matchedides){
	
	unsigned int this_trackID = abs(this_ide.trackID);
	tot_ides_charge+=this_ide.numElectrons;
	
	auto show_iter=shower_idmap.find(this_trackID);
	
	//if you can't find the IDE's trackID in the MCShower mapping
	//(it was a particle not belonging to a shower, IE excited argon decay photon
	//or a particle in an MCShower with energy below the cutoff energy)
	if(show_iter==shower_idmap.end()){
	  part_ides_charge_unknown_MCShower+=this_ide.numElectrons;
	}
	else{
	  //let's say MCShower_indices is (0,    1,   3) [shower 2 had too little energy to count]
	  //part_ides should be like      (0.2, 0.6, 0.1, 0.1)
	  //if it is 20% from shower0, 60% shower1, 10% shower3, and 10% from shower2 OR unknown showers
	  auto it = std::find(MCShower_indices.begin(),
			      MCShower_indices.end(),
			      (*show_iter).second);
	  if(it != MCShower_indices.end())
	    part_ides_charge.at(*it)+=this_ide.numElectrons;
	  else
	    part_ides_charge.back()+=this_ide.numElectrons;
	}
	
      }//end loop over matchedides
      
    }//end finding simch correspond to reco hit's cahnnel
    
    if(tot_ides_charge<=0) {
      std::cerr<<"something is bad. tot_ides_charge is "<<tot_ides_charge<<"... less than one?!"<<std::endl;
      std::vector<float> bad(0,-1);
      return bad;
    }
    
    //debug
    //  std::cout<<"debug: finished and filled part_ides_charge is (";
    //  for(int j = 0; j < part_ides_charge.size(); ++j)
    //    std::cout<<part_ides_charge.at(j)<<", ";
    //  std::cout<<"), with size "<<part_ides_charge.size()<<std::endl;
    
    //compute the actual fractions function returns
    std::vector<float> fractions;  
    for(int i=0;i<(const int)MCShower_indices.size();i++){
      fractions.push_back(part_ides_charge.at(i)/tot_ides_charge);
    }
    fractions.push_back(part_ides_charge_unknown_MCShower/tot_ides_charge);
    
    return fractions;
    
  }//end MatchHitsAll()
  
}
#endif
  
