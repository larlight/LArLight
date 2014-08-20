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
    auto out_hit_v = (event_hit*)(storage->get_data(DATA::Hit));
    out_hit_v->clear_data();
    out_hit_v->set_event_id(mcshower_v->event_id());
    out_hit_v->set_run(mcshower_v->run());
    out_hit_v->set_subrun(mcshower_v->subrun());

    // Output clusters
    auto out_clus_v = (event_cluster*)(storage->get_data(DATA::Cluster));
    out_clus_v->clear_data();
    out_clus_v->reserve(mcshower_v->size() * _nplanes);
    out_clus_v->set_event_id(mcshower_v->event_id());
    out_clus_v->set_run(mcshower_v->run());
    out_clus_v->set_subrun(mcshower_v->subrun());

    // Loop over MCShowers
    for(size_t ishower = 0; ishower < mcshower_v->size(); ++ishower){

      // Get the charge deposition points of all daughters
      // Outer vector size is # of charge deposition points
      // Inner vector size is 4: the actual vertex (x,y,z,E)
      auto const& qdeps = mcshower_v->at(ishower).DaughterPoints();

      // Unique local hit container
      std::vector<std::map<float,larlight::hit > > unique_hits(geo->Nchannels(),std::map<float,larlight::hit>());

      // Loop over the charge deposition points
      for(auto const& qdep : qdeps) {

	// Use Geometry utilities to find the wire, time & channel for this plane
	double tmpxyz[3] = 
	  { qdep.at(0), qdep.at(1), qdep.at(2) };

	//std::vector<unsigned int> channels(geo->Nplanes(),::larlight::DATA::INVALID_UINT);

	// Loop over planes
	for(size_t iplane = 0; iplane < geo->Nplanes(); ++iplane) {
	  
	  double *tmp = tmpxyz;
	  larutil::PxPoint ipxp = geoutil->Get2DPointProjection(tmp,iplane);

	  auto const& ch = geo->PlaneWireToChannel(iplane,ipxp.w);

	  auto hit_iter = unique_hits.at(ch).find(ipxp.t);

	  if(hit_iter == unique_hits.at(ch).end()) {

	    ::larlight::hit h;
	    h.set_wire  ( ipxp.w);
	    h.set_times ( ipxp.t - (_hitwidth/2),
			  ipxp.t,
			  ipxp.t + (_hitwidth/2) );
	    h.set_charge  ( qdep.at(3), qdep.at(3) );
	    h.set_channel ( geo->PlaneWireToChannel(iplane,ipxp.w) );
	    h.set_view    ( geo->PlaneToView(iplane) );

	    unique_hits.at(ch).insert(std::make_pair(ipxp.t, h));

	  }else{

	    (*hit_iter).second.set_charge( (*hit_iter).second.Charge() + qdep.at(3),
					   (*hit_iter).second.Charge() + qdep.at(3) );
	    
	  }
	}
      }

      std::vector<std::vector<unsigned int> > mc_hit_ass(geo->Nplanes(), std::vector<unsigned int>());
      
      for(auto const& hits : unique_hits) {

	for(auto const& hmap_iter : hits) {

	  auto const& h = hmap_iter.second;

	  mc_hit_ass.at(h.View()).push_back(out_hit_v->size());
	  out_hit_v->push_back(h);

	}

      }
      
      for(size_t iplane = 0; iplane < geo->Nplanes(); ++iplane) {

	out_clus_v->push_back(::larlight::cluster());

	(*(out_clus_v->rbegin())).set_view(geo->PlaneToView(iplane));
	(*(out_clus_v->rbegin())).add_association(::larlight::DATA::Hit,mc_hit_ass.at(iplane));

      }
      
    }
 
    return true;
  }

  bool MCShowerClusterer::finalize() {

    return true;
  }

}
#endif
