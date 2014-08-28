#ifndef MCSHOWERCLUSTERER_CC
#define MCSHOWERCLUSTERER_CC

#include "MCShowerClusterer.hh"
//tmp debug
#include <algorithm>

namespace larlight {

  bool MCShowerClusterer::initialize() {

    _nplanes = ::larutil::Geometry::GetME()->Nplanes();

    //all hits will have the same width
    //guessing at this value now from looking at fEndTime-fStartTime for single e- shower hits
    _hitwidth = 6000.;

    return true;
  }
  
  bool MCShowerClusterer::analyze(storage_manager* storage) {

    auto geo = ::larutil::Geometry::GetME();
    //auto geoutil = ::larutil::GeometryUtilities::GetME();

    // Get MCShower objects from storage
    auto mcshower_v = (event_mcshower* )( storage->get_data(DATA::MCShower) );

    auto simch_v = (event_simch*)(storage->get_data(DATA::SimChannel) );

    // Check MCShower object exists
    if( !mcshower_v ) {
      print(MSG::ERROR,__FUNCTION__,"MCShower object DNE! Skipping event...");
      return false;
    }
    if( !simch_v ) {
      print(MSG::ERROR,__FUNCTION__,"SimChannel object DNE! Skipping event...");
      return false;
    }

    // Output hits
    auto out_hit_v = (event_hit*)(storage->get_data(DATA::MCShowerHit));
    if(!out_hit_v) {
      print(MSG::ERROR,__FUNCTION__,"DATA::MCShowerHit could not be retrieved!");
      return false;
    }
    if(out_hit_v->size())
      print(MSG::WARNING,__FUNCTION__,"DATA::MCShowerHit is not empty. Clearing it...");
    out_hit_v->clear_data();
    out_hit_v->set_event_id(mcshower_v->event_id());
    out_hit_v->set_run(mcshower_v->run());
    out_hit_v->set_subrun(mcshower_v->subrun());

    // Output clusters
    auto out_clus_v = (event_cluster*)(storage->get_data(DATA::MCShowerCluster));
    if(!out_clus_v) {
      print(MSG::ERROR,__FUNCTION__,"DATA::MCShowerCluster could not be retrieved!");
      return false;
    }
    if(out_clus_v->size())
      print(MSG::WARNING,__FUNCTION__,"DATA::MCShowerCluster is not empty. Clearing it...");
    out_clus_v->clear_data();
    out_clus_v->reserve(mcshower_v->size() * _nplanes);
    out_clus_v->set_event_id(mcshower_v->event_id());
    out_clus_v->set_run(mcshower_v->run());
    out_clus_v->set_subrun(mcshower_v->subrun());

    // Loop over MCShowers
    for(size_t shower_index = 0; shower_index < mcshower_v->size(); ++shower_index) {
      
      // Get shower object
      auto const& ishower = mcshower_v->at(shower_index);

      // Get a set of daughter track IDs
      std::set<unsigned int> daughters;
      for(auto const& id : ishower.DaughterTrackID())
	daughters.insert(id);

      // Create an output cluster container
      std::vector<larlight::cluster> clusters(geo->Nplanes(),larlight::cluster());
      for(size_t plane=0; plane<geo->Nplanes(); ++plane)
	clusters.at(plane).set_view(geo->PlaneToView(plane));

      // Create an output cluster-hit association container
      std::vector<std::vector<unsigned int> > assn_ch(geo->Nplanes(),std::vector<unsigned int>());

      // Loop over channels
      for(auto const& sch : *simch_v) {

	// Get channel number
	auto const& ch = sch.Channel();

	auto const& w  = geo->ChannelToWire(ch);

	auto const& plane = geo->ChannelToPlane(ch);

	auto const& view  = geo->PlaneToView(plane);

	// Retrieve data for all energy deposition in this channel
	auto& tdc_ide_map = sch.TDCIDEMap();

	// Loop over all energy deposition data
	for(auto const& tdc_ide : tdc_ide_map) {

	  // Get timing
	  auto const& tdc = tdc_ide.first;

	  double hitq = 0;
	  bool   hit_found = false;
	  // Loop over all energy deposition @ this timing
	  for(auto const& this_ide : tdc_ide.second) {

	    unsigned int daughter_track = 0;
	    if(this_ide.trackID > 0) daughter_track = this_ide.trackID;
	    else daughter_track = (-1 * this_ide.trackID);

	    // If nothing to do with this MCShower, ignore.
	    if(daughters.find(daughter_track) == daughters.end()) continue;
	    
	    hit_found = true;

	    //hard-coding fit functions DC has figured out to 
	    //convert number of electrons to charge
	    //USING AMPLITUDE FOR NOW
	    //http://www.nevis.columbia.edu/~dcaratelli/showandtell/U_Wire_MaxWire_vs_Qin_078_20.png
	    //CAN DO SIMILAR WITH AREA IF YOU WANT
	    if(view==::larlight::GEO::kU)
	      hitq += (this_ide.numElectrons/1000)*1.33 - 0.443;
	    else if(view==::larlight::GEO::kV)
	      hitq += (this_ide.numElectrons/1000)*.938 - 0.222;
	    else if(view==::larlight::GEO::kW)
	      hitq += (this_ide.numElectrons/1000)*1.79 + 0.507;
	    else {std::cout<<"wtf"<<std::endl;return false;}

	  }


	  if(!hit_found) continue;


	  //instead of merging hits, try cutting on amplitude
	  //wes says cut on 6 for collection and 3 for induction
	  if( (view == ::larlight::GEO::kU && hitq < 3) ||
	      (view == ::larlight::GEO::kV && hitq < 3) ||
	      (view == ::larlight::GEO::kW && hitq < 6) ) 
	    continue;

	  // Create a hit (and association)
	  // ONLY IF IT DOESN'T OVERLAP AN EXISTING HIT, ELSE MODIFY THAT EXISTING HIT
	  int overlap_index = DoesHitOverlapExisting(out_hit_v,ch,tdc-(_hitwidth/2),tdc+(_hitwidth/2));
	  if(overlap_index == -1){

	    ::larlight::hit h;
	    h.set_wire    ( w    );
	    h.set_channel ( ch   );
	    h.set_times   ( tdc - (_hitwidth/2), 
			    tdc, 
			    tdc + (_hitwidth/2) );
	    h.set_view    ( view );

	    h.set_charge  ( hitq, hitq );
	    
	    switch(view) {
	      
	    case ::larlight::GEO::kU:
	    case ::larlight::GEO::kV:
	      h.set_sigtype( ::larlight::GEO::kInduction   );
	      break;
	    case ::larlight::GEO::kW:
	      //case ::larlight::GEO::kZ:
	      h.set_sigtype( ::larlight::GEO::kCollection  );
	      break;
	    default:
	      h.set_sigtype( ::larlight::GEO::kMysteryType );
	    }
	    
	    out_hit_v->push_back(h);
	    
	    assn_ch.at(plane).push_back(out_hit_v->size()-1);
	    
	  } //kaleko end doesoverlap
	  
	  //if this hit overlaps an existing hit, modify the existing hit
	  else{
	    Double_t newstart = std::min(tdc-(_hitwidth/2),out_hit_v->at(overlap_index).StartTime());
	    Double_t newend   = std::max(tdc+(_hitwidth/2),out_hit_v->at(overlap_index).EndTime());
	    //to-do: implement weighted average by charge here
	    Double_t newpeak  = newstart + ((newend-newstart)/2);
	    Double_t newq     = out_hit_v->at(overlap_index).Charge() + hitq;
	    out_hit_v->at(overlap_index).set_times(newstart,newpeak,newend);
	    out_hit_v->at(overlap_index).set_charge(newq,newq);
	  }


	  //whether or not hit overlaps other hits, add to cluster charge
	  clusters.at(plane).set_charge(clusters.at(plane).Charge() + hitq);

	}
      }
      
      //
      // Store
      //

      // MCShower->Cluster association
      std::vector<unsigned int> assn_cs;
      assn_cs.reserve(geo->Nplanes());

      for(size_t plane = 0; plane<geo->Nplanes(); ++plane) {

	if(!(assn_ch.at(plane).size())) continue;

	clusters.at(plane).set_id(out_clus_v->size());

	clusters.at(plane).set_view(geo->PlaneToView(plane));
	
	clusters.at(plane).add_association(DATA::MCShowerHit,assn_ch.at(plane));

	assn_cs.push_back(out_clus_v->size());

	out_clus_v->push_back(clusters.at(plane));

      }

      mcshower_v->at(shower_index).add_association(DATA::MCShowerCluster,assn_cs);

    }
 
    return true;
  }

  bool MCShowerClusterer::finalize() {

    return true;
  }

  int MCShowerClusterer::DoesHitOverlapExisting(event_hit* original_hit_v,UInt_t channel,Double_t start,Double_t end) {


    if(!_group_overlapping_hits) return -1;

    //check if there are overlapping hits on the same channel with this fancy c++11 find_if thing
    //(if existing hit duration includes the current hit start time, or
    // if existing hit duration includes the current hit end time)

    auto it = std::find_if(original_hit_v->begin(),
			   original_hit_v->end(),
			   [&channel,&start,&end](const ::larlight::hit &ihit) {return ihit.Channel() == channel && ( (ihit.StartTime()<start && ihit.EndTime()>start) || (ihit.StartTime()<end && ihit.EndTime()>end) );});


    //if overlapping hit found, return the index in original_hit_v of that hit
    if(it != original_hit_v->end())
      return (int)(it-original_hit_v->begin());

    //return -1 if hit does not overlap any existing hits
    return -1;
  }
}
#endif
