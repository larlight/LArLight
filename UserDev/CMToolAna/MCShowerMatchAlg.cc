#ifndef MCSHOWERMATCHALG_CC
#define MCSHOWERMATCHALG_CC

#include "MCShowerMatchAlg.hh"

namespace larlight {

  MCShowerMatchAlg::MCShowerMatchAlg() {

    _name = "MCShowerMatchAlg";
    _view_to_plane.clear();
    fBTAlgo.SetMaxEnergyCut(2000);
    fBTAlgo.SetMinEnergyCut(20);
  }

  bool MCShowerMatchAlg::Prepare(::larlight::storage_manager* storage,
			      const DATA::DATA_TYPE &cluster_type)
  {
    auto geo = ::larutil::Geometry::GetME();

    if(!_view_to_plane.size()) {
      _view_to_plane.resize(geo->Nplanes(),DATA::INVALID_UCHAR);
      
      for(UChar_t plane=0; plane<geo->Nplanes(); ++plane) {
	
	UChar_t view = geo->PlaneToView(plane);
	
	if(_view_to_plane.size() <= view) _view_to_plane.resize(view+1,DATA::INVALID_UCHAR);
	
	_view_to_plane.at(view) = plane;
      }
    }

    //
    // Prepare back-tracking using McshowerLookback
    //
    // (1) Load MCShower and SimCh data products
    // (2) Convert them to MCShower/SimCh map necessary for back-tracking
    //

    // Load MCShower and SimCh
    auto ev_mcshower = (const event_mcshower*)(storage->get_data(DATA::MCShower));

    auto ev_simch    = (const event_simch*)(storage->get_data(DATA::SimChannel));

    if(!ev_mcshower || !ev_simch) {

      print(MSG::ERROR,__FUNCTION__,"MCShower/SimCh data product not found!");

      return false;

    }

    // Convert to necessary map for back-tracking
    std::map<UInt_t,UInt_t> shower_idmap;

    std::map<UShort_t, larlight::simch> simch_map;

    fWatch.Start();

    fBTAlgo.FillShowerMap(ev_mcshower, shower_idmap);

    fBTAlgo.FillSimchMap(ev_simch, simch_map);

    if(this->get_verbosity()==MSG::DEBUG)

      print(MSG::DEBUG,Form("MCShowerMatchAlg::%s",__FUNCTION__),
	    Form("Time to prepare McshowerLookback: %g",fWatch.RealTime()));

    // Preparation done
    
    //
    // Perform back-tracking
    //
    // (1) Load cluster/hit data product
    // (2) Loop over all clusters and find charge fraction to each MCShower
    // (3) For each MCShower, find a cluster per plane with the highest charge
    
    // Load data products
    auto ev_cluster = (const event_cluster*)(storage->get_data(cluster_type));
    
    if(!ev_cluster) {
      
      print(MSG::ERROR,__FUNCTION__,Form("Data product \"%s\" not found...",
					 DATA::DATA_TREE_NAME[cluster_type].c_str()
					 )
	    );
      return false;
    }else if(ev_cluster->size()<1) return false;

    const DATA::DATA_TYPE hit_type = ev_cluster->get_hit_type();

    auto ev_hit = (const event_hit*)(storage->get_data(hit_type));

    if(!ev_hit) {

      print(MSG::ERROR,__FUNCTION__,Form("Data product \"%s\" not found...",
					 DATA::DATA_TREE_NAME[hit_type].c_str()
					 )
	    );
      return false;
    }

    // Loop over clusters & get charge fraction per MCShower
    _qfrac_v.clear();
    _cluster_charge.clear();
    _cluster_plane_id.clear();

    _qfrac_v.reserve(ev_cluster->size());
    _cluster_charge.reserve(ev_cluster->size());
    _cluster_plane_id.reserve(ev_cluster->size());

    fWatch.Start();
    for(auto const& c : *ev_cluster) {

      // Create hit list
      std::vector<const larlight::hit*> hits;
      auto const& hit_indices = c.association(hit_type);
      hits.reserve(hit_indices.size());

      _cluster_charge.push_back(0);
      for(auto const& index : hit_indices) {

	hits.push_back(&(ev_hit->at(index)));
	(*_cluster_charge.rbegin()) += ev_hit->at(index).Charge();
	
      }

      _cluster_plane_id.push_back(_view_to_plane.at(c.View()));

      _qfrac_v.push_back(fBTAlgo.MatchHitsAll(hits, simch_map, shower_idmap));

    }

    if(this->get_verbosity()==MSG::DEBUG)

      print(MSG::DEBUG,Form("MCShowerMatchAlg::%s",__FUNCTION__),
	    Form("Time to back-track all clusters: %g",fWatch.RealTime()));

    //
    // Find the best matched pair (and its charge) per MCShower
    //
    // index number of MCShowers (in event_mcshower) that are taken into account 
    _mcshower_id.clear();
    fBTAlgo.UniqueShowerID(shower_idmap,_mcshower_id);

    _bmatch_q.clear();
    _bmatch_id.clear();

    _bmatch_q.resize(_mcshower_id.size(),std::vector<double>(geo->Nplanes(),0));
    _bmatch_id.resize(_mcshower_id.size(),std::vector<size_t>(geo->Nplanes(),0));

    fWatch.Start();
    for(size_t shower_index=0; shower_index < _mcshower_id.size(); ++shower_index) {

      for(size_t cluster_index=0; cluster_index < ev_cluster->size(); ++cluster_index) {

	if((*(_qfrac_v.at(cluster_index).rbegin())) < 0) continue;

	auto plane = _cluster_plane_id.at(cluster_index);

	double q = _qfrac_v.at(cluster_index).at(shower_index) * _cluster_charge.at(cluster_index);

	if( _bmatch_q.at(shower_index).at(plane) < q ) {

	  _bmatch_q.at(shower_index).at(plane)  = q;
	  _bmatch_id.at(shower_index).at(plane) = cluster_index;
	  
	}
	
      }
    }

    if(this->get_verbosity()==MSG::DEBUG)
      
      print(MSG::DEBUG,Form("MCShowerMatchAlg::%s",__FUNCTION__),
	    Form("Time to find best matched pair(s): %g",fWatch.RealTime()));

    return true;
  }
  
  void MCShowerMatchAlg::Match(const std::vector<unsigned int> &cluster_indices,
			       size_t &mcshower_index,
			       double &correctness) const
  {    

    if(!cluster_indices.size()) throw CMToolAnaException("Input cluster indices empty!");
    if(!_cluster_charge.size()) throw CMToolAnaException("Preparation not done yet!");

    // Compute efficiency per MCShower
    std::vector<double> match_eff(_mcshower_id.size(),1);
    
    for(auto const& cluster_index : cluster_indices) {
      
      for(size_t shower_index=0; shower_index < _mcshower_id.size(); ++shower_index) {
	
	unsigned char plane = _cluster_plane_id.at(cluster_index);
	
	double q = _cluster_charge.at(cluster_index) * _qfrac_v.at(cluster_index).at(shower_index);
	
	match_eff.at(shower_index) *= q / _bmatch_q.at(shower_index).at(plane);
      }
      
    }
    
    // Find the best qratio
    correctness=-1;
    for(size_t shower_index=0; shower_index < _mcshower_id.size(); ++shower_index) {
      
      if(match_eff.at(shower_index) < correctness) continue;

      correctness = match_eff.at(shower_index);
      
      mcshower_index = _mcshower_id.at(shower_index);

    }
    
  }
}
#endif
