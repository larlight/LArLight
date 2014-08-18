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
    auto geoutil = ::larutil::GeometryUtilities::GetME();

    // Get MCShower objects from storage
    auto mcshower_v = (event_mcshower* )( storage->get_data(DATA::MCShower) );

    // Check MCShower object exists
    if( !mcshower_v ) {
      std::cerr<<"MCShower object DNE! Skipping event..."<<std::endl;
      return false;
    }

    // Output hits
    ::larlight::event_hit out_hit_v;
    out_hit_v.set_event_id(mcshower_v->event_id());
    out_hit_v.set_run(mcshower_v->run());
    out_hit_v.set_subrun(mcshower_v->subrun());

    // Output clusters
    ::larlight::event_cluster out_clus_v;
    out_clus_v.reserve(mcshower_v->size() * _nplanes);
    out_clus_v.set_event_id(mcshower_v->event_id());
    out_clus_v.set_run(mcshower_v->run());
    out_clus_v.set_subrun(mcshower_v->subrun());

    unsigned short hit_id_for_assn = 0;

    // Loop over MCShowers
    for(size_t ishower = 0; ishower < mcshower_v->size(); ++ishower){

      // Get the charge deposition points of all daughters
      // Outer vector size is # of charge deposition points
      // Inner vector size is 4: the actual vertex (x,y,z,E)
      const std::vector<std::vector<Float_t> > qdeps = mcshower_v->at(ishower).DaughterPoints();

      // Loop over planes
      for(Int_t iplane = 0; iplane < _nplanes; ++iplane){

	std::vector<unsigned short> tmp_ass;
	tmp_ass.clear();

	// Loop over the charge deposition points
	for(size_t iqdep = 0; iqdep < qdeps.size(); ++iqdep){
	  
	  // Make a larlight::hit object for each charge deposition point, for each plane
	  ::larlight::hit ihit;
	  
	  // Use Geometry utilities to find the wire, time & channel for this plane
	  Double_t tmpxyz[3] = 
	    { qdeps.at(iqdep).at(0), qdeps.at(iqdep).at(1), qdeps.at(iqdep).at(2) };
	  Double_t *tmp = tmpxyz;
	  larutil::PxPoint ipxp = geoutil->Get2DPointProjection(tmp,iplane);
	  
	  // Set the wire, time, channel, charge, plane for this hit
	  ihit.set_wire(ipxp.w);
	  ihit.set_times(ipxp.t-(_hitwidth/2),ipxp.t,ipxp.t+(_hitwidth/2));
	  ihit.set_charge(qdeps.at(iqdep).at(3),qdeps.at(iqdep).at(3));
	  ihit.set_channel(geo->PlaneWireToChannel(iplane,ipxp.w));
	  ihit.set_view(geo->PlaneToView(iplane));
	  
	  // Push this hit object into the event_hits vector
	  out_hit_v.push_back(ihit);

	  // Keep track of its index in out_hit_v for the cluster association later
	  tmp_ass.push_back(hit_id_for_assn);
	  hit_id_for_assn++;

	}//end loop over charge deposition points, iqdep

	// One output cluster per MCShower per plane
	::larlight::cluster iclus;
	iclus.set_view(geo->PlaneToView(iplane));
	iclus.add_association(DATA::Hit,tmp_ass);
	
	// Add the output cluster to the event_cluster vector
	out_clus_v.push_back(iclus);

      } //end loop over planes, iplane
      
    } //end loop over MCShowers, ishower

    //Write the hits and clusters to the output file
    (*((event_hit*)(storage->get_data(DATA::Hit)))) = out_hit_v;
    (*((event_cluster*)(storage->get_data(DATA::Cluster)))) = out_clus_v;
 
    return true;
  }

  bool MCShowerClusterer::finalize() {

    return true;
  }

}
#endif
