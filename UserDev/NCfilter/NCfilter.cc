#ifndef NCFILTER_CC
#define NCFILTER_CC

#include "NCfilter.hh"
#include "ClusterParamsAlg.hh"
#include "LArUtilBase.hh"


namespace larlight {

  bool NCfilter::initialize() {

    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you have a histogram to fill in the event loop, for example,
    // here is a good place to create one on the heap (i.e. "new TH1D"). 
    //

    return true;
  }
  
  bool NCfilter::analyze(storage_manager* storage) {
    
    
    // grab the incoming clusters. 
    // these should be clusters that are very primative. like DB or hough
    auto Incoming_cluster = (const event_cluster*)(storage->get_data(DATA::DBCluster));
    auto const& hit_type = Incoming_cluster->get_hit_type();
    auto hits = (const event_hit*)(storage->get_data(hit_type));
    
    if(!hits || !Incoming_cluster) {
      print(MSG::ERROR,__FUNCTION__,"No DBCluster or associated hits found!");
      return false;
    }
    if(!(Incoming_cluster->size())){
      print(MSG::WARNING,__FUNCTION__,Form("Event %d has no DBCluster...",Incoming_cluster->event_id()));
      return true;
    }
    else if(!(hits->size())){
      print(MSG::ERROR,__FUNCTION__,Form("Event %d has no hits (but there's DBCluster!!!)",Incoming_cluster->event_id()));
      return false;
    }
    // make the output cluster
    auto Output_cluster = (event_cluster*)(storage->get_data(DATA::RyanCluster));
    
    // set event variables
    if(Output_cluster->size())
      print(MSG::WARNING,__FUNCTION__,"DATA::RyanCluster is not empty. Clearing it...");
    Output_cluster->clear_data();
    Output_cluster->set_event_id(Incoming_cluster->event_id());
    Output_cluster->set_run(Incoming_cluster->run());
    Output_cluster->set_subrun(Incoming_cluster->subrun());
    


	// Need some bool to check about the quality of the protoclusters
        // need some flags to see which planes to pursue  clustering in
        std::vector<bool> flagplanes(3,false);
    int nplanes = larutil::Geometry::GetME()->Nplanes();
    std::vector<std::pair<double,double>> AvgPairSI(nplanes);	



//============Starting with  first pass alg============

// make a vector of clusters
	std::vector<larlight::cluster> clustervect;
    for(auto const& c : *Incoming_cluster) {
	clustervect.push_back(c);
	}
	std::vector<larlight::hit> hitsvect;
    for(auto const& h : *hits) {
	hitsvect.push_back(h);
	}
	AvgPairSI = fDivReg.SplitLineC(clustervect, hitsvect);

//====================Ending with alg===================

	
// make the regions
	
   std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>>  blah;
   blah = fForceRegions.ForceTwoRegions(hitsvect, AvgPairSI);
 
//   std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>>  blah = ForcedRegions(hits,AvgPairSI);
    
    std::cout<< "\033[94m lets see what things do  "<< blah[0].first.size()<<"  , \033[00m"<<blah[0].second.size()<<std::endl;
    std::cout<< "\033[95m lets see what things do  "<< blah[1].first.size()<<"  , \033[00m"<<blah[1].second.size()<<std::endl;
    std::cout<< "\033[96m lets see what things do  "<< blah[2].first.size()<<"  , \033[00m"<<blah[2].second.size()<<std::endl;
//Thatt 
	// build in a quality check
// Need to now check what we have?
// if the force reg failed try new approach.

// first check that each region has more than a certain amount of hits
// This can be a function to check over all of the planes 

        //============================ 

// firstpass is a vector of pairs that has the hit ass index in them... so these are effectivly clusters of hits. 





        //============================ 


















    // Lets fill these hits out into clusters. 
    
    for( unsigned int a=0 ; a<blah.size();a++){

      ::larlight::cluster lite_cluster;
      std::vector<unsigned int> hit_ass;

      // Skip if there's no hit
      if(!(blah[a].first.size()) || !(blah[a].second.size())) continue;

      //
      // Save clusters
      //

      // Clear data products
      hit_ass.clear();
      hit_ass.reserve(blah[a].first.size());
      lite_cluster.clear_data();

      // Make association
      for( auto const& hit : blah[a].first) hit_ass.push_back(hit);

      // Add association
      lite_cluster.add_association(hit_type,hit_ass);

      // Add a cluster to the output
      Output_cluster->push_back(lite_cluster);

      // Clear data products
      hit_ass.clear();
      hit_ass.reserve(blah[a].second.size());
      lite_cluster.clear_data();

      // Make association
      for( auto const& hit : blah[a].second) hit_ass.push_back(hit);

      // Add association
      lite_cluster.add_association(hit_type,hit_ass);

      // Add a cluster to the output
      Output_cluster->push_back(lite_cluster);

    }// loop over balh

    return true;
  }

  bool NCfilter::finalize() {

    // This function is called at the end of event loop.
    // Do all variable finalization you wish to do here.
    // If you need, you can store your ROOT class instance in the output
    // file. You have an access to the output file through "_fout" pointer.
    //
    // Say you made a histogram pointer h1 to store. You can do this:
    //
    // if(_fout) { _fout->cd(); h1->Write(); }
    //
    // else 
    //   print(MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
    //
  
    return true;
  }

//---------------------
// Adding some fuctions 
//---------------------

  //---------------------------------------------------------------------------------------------
  std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>>  NCfilter::ForceRegions(larlight::event_hit const* hits, 
												      std::vector<std::pair<double,double>> APP){
    
    double T2CM = larutil::GeometryUtilities::GetME()->TimeToCm();	
    double W2CM = larutil::GeometryUtilities::GetME()->WireToCm();	
    std::vector<unsigned int> startervector;
    std::pair<std::vector<unsigned int>,std::vector<unsigned int>> planepair(startervector,startervector);
    std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>> vectplanepair(3,planepair);
    // loop over all the hits. 	
    //for(auto const& hit : *hits) {
    for(unsigned int a=0;  a < hits->size();a++) {
      //                std::cout<< "\033[92m"<<a<<std::endl;
      //which plane?
      //int cplane= hit.View();
      int cplane= hits->at(a).View();
      //              std::cout<< "\033[91m"<<APP[cplane].first<<std::endl;
      if(APP[cplane].first==-999||APP[cplane].second==-999){ std::cout<<" \033[98m GOT A -999"; continue;}
      double regionval= (-APP[cplane].first * hits->at(a).Wire()*W2CM+ hits->at(a).PeakTime()*T2CM - APP[cplane].second) / sqrt(APP[cplane].first*APP[cplane].first +1);
      // sort into proper places using viewPP)
      if(regionval>0.0000) vectplanepair[cplane].first.push_back(a);
      if(regionval<0.0000) vectplanepair[cplane].second.push_back(a);
    }// for loop over hit index
    
    return vectplanepair;
    
  }// FOrce Regions

 
  //---------------------------------------------------------------------------------------------
  //---------------------------------------------------------------------------------------------

/*
std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>>  NCfilter::CheckQuality(larlight::event_hit *const hits,std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>> protocluster, std::vector<bool> Flags){


        // First Check based on bool.... if there is already a bool? Might not need this in here... Should do it in the code to fill clusters
	fucnflag0= false;
	fucnflag1= false;
	fucnflag2= false;
	
	

        // Checking the time profile
        for( unsigned int a=0 ; a<protocluster.size();a++){


                for(unsigned int k =0 ; k<protocluster[a].first.size(); k++){


                                }// protocluster for loop over a first cluster  

                }// for loop over protocluster


	}// End of Check Quality Func

*/








}
#endif
