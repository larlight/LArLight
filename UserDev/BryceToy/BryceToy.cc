#ifndef BRYCETOY_CC
#define BRYCETOY_CC

#include "BryceToy.hh"

namespace larlight {
  
  bool BryceToy::initialize() {
    
    h = new TH2D("h11","Percent charge from dominant MCShower; Charge; Fraction",100,0,3e6,102,-0.01,1.01);

    return true;
  }
  
  bool BryceToy::analyze(storage_manager* storage) {
    
    McshowerLookback b;
    int num_mcshow=0;
    
    //Get all mcshowers for this event and make a map of trackIDs versus shower number
    auto my_mcshow = (event_mcshower*)(storage->get_data(DATA::MCShower));
    num_mcshow = my_mcshow->size();

    std::map<UInt_t, UInt_t> shower_idmap;
    b.FillShowerMap(my_mcshow, shower_idmap);    

    if(my_mcshow->size()>1){
      std::cout<<"More than 1 MCShower!"<<std::endl;
      totalmult++;
    }

    ////Old code to do the same thing in-module
    //for(auto this_mcshow : *my_mcshow){
    //  num_mcshow++;
    //  auto this_mom = (std::vector<Double_t>)this_mcshow.MotherMomentum();
    //  for(auto this_trackid : (std::vector<UInt_t>)this_mcshow.DaughterTrackID()){
    //	//std::cout<<this_trackid<<std::endl;
    //	shower_idmap[this_trackid]=num_mcshow;
    //  }
    //}    
    //std::cout<<"------------------------------"<<std::endl;
    
    //Get all simchannels for this event, and make map of simchannel versus channel
    std::map<UShort_t,larlight::simch> simch_map;    
    auto my_simch = (event_simch*)(storage->get_data(DATA::SimChannel));
    if(!my_simch) std::cerr<<"I found no simchannels!"<<std::endl;
    b.FillSimchMap(my_simch, simch_map);

    ////Old code to do the same thing in-module
    //for(auto this_simch : *my_simch){
    //  simch_map[this_simch.Channel()]=this_simch;
    //}
    
    if(num_mcshow>0){
    //////////////////
    //Match hit channels to trackIDs from mcshowers.
    //////////////////
    //Breakdown vectors that are the ultimate product
    std::vector<float> cluster_charge_breakdown;    

    //Get my cluster and hit data products
    auto my_clusters = (event_cluster*)(storage->get_data(DATA::ShowerAngleCluster));    
    DATA::DATA_TYPE hit_type = my_clusters->get_hit_type();
    auto my_hits = (event_hit*)(storage->get_data(hit_type));        
    if(!my_clusters || !my_hits) std::cerr<<"I found an empty cluster!"<<std::endl;

    //make a vector of MCShower indices
    std::vector<UInt_t> MCShower_indices;
    for(UInt_t i=0; i < my_mcshow->size(); ++i)
      MCShower_indices.push_back(i);
    

    //Loop through clusters
    for(auto this_cluster : *my_clusters) {      

      double tot_clus_charge = 0;
      double part_clus_charge[(const int)MCShower_indices.size()];
      for(int i=0;i<MCShower_indices.size();i++)
	part_clus_charge[i]=0;

      //Get hits and loop over them
      auto hit_index_vector = this_cluster.association(hit_type);	
      for(auto hit_index : hit_index_vector){
    	auto this_hit = (larlight::hit)(my_hits->at(hit_index));
	
	tot_clus_charge += this_hit.Charge(); //need to check units on this

	std::vector<float> hit_fraction_breakdown = 
	  b.MatchHitsAll(this_hit,
			 simch_map,
			 shower_idmap,
			 MCShower_indices);

	//DK: hit_fraction_breakdown is a vector of charge fractions
	//from the different MCShowers the hit belongs to.
	// For example, if the list of MCShowers indicies was (0, 1, 2)
	// and the hit's charge is 20% in MCShower 0, 70% in MCShower1, 
	// 0% in MCShower2, and 10% in an unknown MCShower, 
	// the returned vector is (0.2, 0.8, 0.0, 0.10)
	// (the unknown MCShower is always the last entry in the return vector)

	if(hit_fraction_breakdown.size()-1 != MCShower_indices.size())
	  std::cerr<<"Something horrible has happened."
		   <<"hit_fraction_breakdown.size()-1 != MCShower_indices.size()"
		   <<", the first is "<<hit_fraction_breakdown.size()-1
		   <<", the second is "<< MCShower_indices.size()
		   <<std::endl;

	for(int i=0;i<hit_fraction_breakdown.size();i++){
	  //multiply fraction (returned by MatchHitsAll) by actual charge
	  part_clus_charge[i] += 
	    (hit_fraction_breakdown.at(i) * this_hit.Charge());
	}



	//std::cout<<hit_charge_breakdown.at(0)/hit_charge_breakdown.at(num_mcshow+1)<<std::endl;
	
    	/////////////
    	//Map hit channel to simchannel, so we can get ides and trackIDs associated with hits
    	/////////////
    	//auto simch_iter = simch_map.find(this_hit.Channel());
    	////No match for hit
    	//if(simch_iter==simch_map.end())
    	//  std::cerr<<"Hit has no matched simchannel!!"<<std::endl;
    	////Matched, so now try to match ide TrackIDs with shower TrackIDs
    	//else{	    
    	//  double tot_ides_charge = 0;
    	//  double part_ides_charge[(const int)(num_mcshow+1)];
    	//  for(int i=0;i<num_mcshow+1;i++)
    	//    part_ides_charge[i]=0;
    	//  
    	//  auto matchedsimch = (*simch_iter).second;
    	//  auto matchedide = (std::vector<larlight::ide>)matchedsimch.TrackIDsAndEnergies(this_hit.StartTime(),this_hit.EndTime());	    
    	//  //Assign ide charge to a particular MCShower using channel:mcshower mapping
    	//  for(auto this_ide : matchedide){	    
    	//    unsigned int this_trackID = abs(this_ide.trackID);
    	//    tot_ides_charge+=this_ide.numElectrons;
    	//    auto show_iter=shower_idmap.find(this_trackID);
    	//    if(show_iter==shower_idmap.end()){
    	//      part_ides_charge[0]+=this_ide.numElectrons;	    		
    	//      //std::cout<<"No Match!!"<<std::endl;
    	//      //std::cout<<this_ide.numElectrons<<" "<<part_ides_charge[0]<<" "<<tot_ides_charge<<" "<<std::endl;	      
    	//    }
    	//    else{
    	//      part_ides_charge[(*show_iter).second]+=this_ide.numElectrons;	    
    	//      //if((*show_iter).second>1){
    	//      //	std::cout<<this_ide.numElectrons<<" "<<part_ides_charge[(*show_iter).second]<<" "<<tot_ides_charge<<" "<<(*show_iter).second<<std::endl;	      
    	//      //	std::cout<<"--------"<<std::endl;
    	//      //}
    	//    }
    	//  }
    	//  //std::cout<<"(";
    	//  tot_clus_charge+=tot_ides_charge;
    	//  for(int i=0;i<num_mcshow+1;i++){
    	//    part_clus_charge[i]+=part_ides_charge[i];	    
    	//    part_ides_charge[i]/=tot_ides_charge;
    	//    //std::cout<<part_ides_charge[i]<<" ";
    	//  }
    	//  //std::cout<<tot_ides_charge<<")"<<std::endl;	  
    	//}
      }
      
      //this might just be un-doing what is done in MatchHitsAll
      std::cout<<"(";
      //the +1 here is for "unknown showers"
      for(int i=0;i<num_mcshow+1;i++){
	part_clus_charge[i]/=tot_clus_charge;
	std::cout<<part_clus_charge[i]<<" ";
      }
      std::cout<<tot_clus_charge<<")"<<std::endl;	      
      double biggest=0;
      for(int i=0;i<num_mcshow;i++){
	if(part_clus_charge[i]>biggest) biggest=part_clus_charge[i];
      }
      
      h->Fill(tot_clus_charge,biggest);
    }
    std::cout<<"-------------------------------"<<std::endl;
    //h->Fill(cluster_charge);
    
    //std::cout<< Form("I found %zu clusters...",my_clusters->size())<<std::endl;
    
    }    
    return true;
  }
  
  bool BryceToy::finalize() {
    
    std::cout<<"Total multiple showers: "<<totalmult<<std::endl;
    if(_fout){
      _fout->cd();
      h->Write();
    }
    
    return true;
  }
}
#endif
