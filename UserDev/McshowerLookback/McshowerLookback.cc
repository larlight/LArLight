#ifndef MCSHOWERLOOKBACK_CC
#define MCSHOWERLOOKBACK_CC

#include "McshowerLookback.hh"

McshowerLookback::McshowerLookback(){
  std::cout<<"Hello world!"<<std::endl;
}

////////////////
//Make map of trackIDs for each shower
////////////////
void McshowerLookback::FillShowerMap(larlight::event_mcshower* my_mcshow, std::map<unsigned short, unsigned long> &shower_idmap){
  int num_mcshow=0;
  for(auto this_mcshow : *my_mcshow){
    num_mcshow++;
    auto this_mom = (std::vector<Double_t>)this_mcshow.MotherMomentum();
    for(auto this_trackid : (std::vector<UInt_t>)this_mcshow.DaughterTrackID()){
      //std::cout<<this_trackid<<std::endl;                                                                                                                      
      shower_idmap[this_trackid]=num_mcshow;
    }
  }
}

////////////////
//Make map of simchannel objects for each channel
////////////////
void McshowerLookback::FillSimchMap(larlight::event_simch* my_simch, std::map<unsigned long, larlight::simch> &simch_map){
  for(auto this_simch : *my_simch){
    simch_map[this_simch.Channel()]=this_simch;
  }
}

////////////////
//Find % of each shower in a single hit
////////////////
//std::vector MatchHitsAll(larlight::hit* this_hit, std::map<unsigned short, larlight::simch>* simch_map, std::map<unsigned short, unsigned long>* shower_idmap){
//  auto simch_iter = simch_map.find(this_hit.Channel());
//  //No match for hit                                                                                                                                         
//  if(simch_iter==simch_map.end())
//    std::cerr<<"Hit has no matched simchannel!!"<<std::endl;
//  //Matched, so now try to match ide TrackIDs with shower TrackIDs                                                                                           
//  else{
//    double tot_ides_charge = 0;
//    double part_ides_charge[(const int)(num_mcshow+1)];
//    for(int i=0;i<num_mcshow+1;i++)
//      part_ides_charge[i]=0;
//  }
//  auto matchedsimch = (*simch_iter).second;
//  auto matchedide = (std::vector<larlight::ide>)matchedsimch.TrackIDsAndEnergies(this_hit.StartTime(),this_hit.EndTime());
//  //Assign ide charge to a particular MCShower using channel:mcshower mapping                                                                              
//  for(auto this_ide : matchedide){
//    unsigned int this_trackID = abs(this_ide.trackID);
//    tot_ides_charge+=this_ide.numElectrons;
//    auto show_iter=shower_idmap.find(this_trackID);
//    if(show_iter==shower_idmap.end()){
//      part_ides_charge[0]+=this_ide.numElectrons;
//    }
//    else{
//      part_ides_charge[(*show_iter).second]+=this_ide.numElectrons;
//    }
//  }

////NEED TO ADD VECTOR BUSINESS.

//}

#endif
