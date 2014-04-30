#ifndef MCSHOWERLOOKBACK_CC
#define MCSHOWERLOOKBACK_CC

#include "McshowerLookback.hh"


///////////////////////////////////////////////////
//Make map of trackIDs for each shower           //
///////////////////////////////////////////////////
void McshowerLookback::FillShowerMap(larlight::event_mcshower* my_mcshow, 
				     std::map<UInt_t,UInt_t> &shower_idmap)
{
  UInt_t mcshower_index = 0;
  
  for(auto this_mcshow : *my_mcshow){

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
						  std::map<UShort_t, larlight::simch> &simch_map, 
						  std::map<UInt_t,UInt_t> &shower_idmap, 
						  std::vector<UInt_t> MCShower_indices)
{
  //std::cout<<"Going through Here?"<<std::endl;

  //setup variables for holding charge info from various mcshowers

  //total MC charge deposited on that hit's wire during that hit's start->end time
  float tot_ides_charge = 0;
  //amount of MC charge deposited "" "" belonging to a given MCShower
  std::vector<float> part_ides_charge((size_t)MCShower_indices.size(),0.);
  //amount of MC charge deposited "" "" belonging to MCShower not in the map
  float part_ides_charge_unknown_MCShower = 0;

  
  //Search for simchannels on hit's channel number
  auto simch_iter = simch_map.find(this_hit.Channel());
  if(simch_iter==simch_map.end()) 
    std::cerr<<"Hit has no matched simchannel!!"<<std::endl;

  else{

    //Matched, so try to find ides
    auto matchedsimch = (*simch_iter).second;
    auto matchedides = (std::vector<larlight::ide>)matchedsimch.TrackIDsAndEnergies(this_hit.StartTime(),this_hit.EndTime());

    //Assign ide charge to a particular MCShower using channel:mcshower mapping
    //loop over the (MC) IDEs (particles) that correspond to this reconstructed hit
    for(auto this_ide : matchedides){
      unsigned int this_trackID = abs(this_ide.trackID);
      tot_ides_charge+=this_ide.numElectrons;

      //find this IDE's trackID in the shower mapping to get its MCShower index
      auto show_iter=shower_idmap.find(this_trackID);
      //if you can't find the IDE's trackID in the MCShower mapping
      //(it was a particle not belonging to a shower, IE excited argon decay photon)
      if(show_iter==shower_idmap.end()){
	part_ides_charge_unknown_MCShower+=this_ide.numElectrons;
      }
      else{
	//note: if the code is crashing, it's likely here
	part_ides_charge.at((*show_iter).second)+=this_ide.numElectrons;
      }

    }//end loop over matchedides

  }//end finding simch correspond to reco hit's cahnnel

  if(tot_ides_charge<=0) {
    std::cerr<<"something is bad. tot_ides_charge is "<<tot_ides_charge<<"... less than one?!"<<std::endl;
    std::vector<float> bad(1,-1);
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


#endif
