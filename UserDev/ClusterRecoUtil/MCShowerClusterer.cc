#ifndef MCSHOWERCLUSTERER_CC
#define MCSHOWERCLUSTERER_CC

#include "MCShowerClusterer.hh"

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
	    hitq += this_ide.numElectrons;
	  }

	  if(!hit_found) continue;

	  // Create a hit
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

}
#endif
