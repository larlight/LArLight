#ifndef CMATCHPERFORMANCE_CC
#define CMATCHPERFORMANCE_CC

#include "CMatchPerformance.hh"

namespace larlight {

  bool CMatchPerformance::initialize() {

    auto geo = ::larutil::Geometry::GetME();
    _view_to_plane.clear();
    _view_to_plane.resize(geo->Nplanes(),DATA::INVALID_UCHAR);

    for(UChar_t plane=0; plane<geo->Nplanes(); ++plane) {

      UChar_t view = geo->PlaneToView(plane);

      if(_view_to_plane.size() <= view) _view_to_plane.resize(view+1,DATA::INVALID_UCHAR);

      _view_to_plane.at(view) = plane;

    }

    hMatchChargeEff_MC = new TH1D("hMatchChargeEff",
				  "MC Matched Pair Efficiency; Efficiency; Num. MCShower;",
				  100,0,1);

    hMatchNumEff_MC = new TH1D("hMatchNumEff",
			       "Number of MCShower / Reco-Matched Pair; Efficiency; Events;",
			       100,0,1);

    return true;


  }
  
  bool CMatchPerformance::analyze(storage_manager* storage) {

    auto geo = ::larutil::Geometry::GetME();

    //
    // Run CMatchManager
    //
    // (1) Convert larlight "cluster" to a set of PxHit (fmwk independent "cluster")
    // (2) Pass it onto CMatchManager and process
    //

    std::vector<std::vector<larutil::PxHit> > clusters;

    fHelper.GeneratePxHit(storage,_cluster_type,clusters);

    fMgr.Reset();

    fMgr.SetClusters(clusters);

    fMgr.Process();

    // Done matching clusters. Get result
    auto bk = fMgr.GetBookKeeper();

    std::vector<std::vector<unsigned int> > reco_match_v;
    bk.PassResult(reco_match_v);

    //std::vector<std::vector<float> > reco_match_charge_v;
    //reco_match_charge_v.resize(reco_match_v.size(),std::vector<float>(geo->Nplanes(),0));


    
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

    fBTAlgo.FillShowerMap(ev_mcshower, shower_idmap);

    fBTAlgo.FillSimchMap(ev_simch, simch_map);

    // Preparation done

    //
    // Perform back-tracking
    //
    // (1) Load cluster/hit data product
    // (2) Loop over all clusters and find charge fraction to each MCShower
    // (3) For each MCShower, find a cluster per plane with the highest charge

    // Load data products
    auto ev_cluster = (const event_cluster*)(storage->get_data(_cluster_type));

    if(!ev_cluster) {

      print(MSG::ERROR,__FUNCTION__,Form("Data product \"%s\" not found...",
					 DATA::DATA_TREE_NAME[_cluster_type].c_str()
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
    std::vector<std::vector<float> > qfrac_v;
    qfrac_v.reserve(ev_cluster->size());
    
    for(auto const& c : *ev_cluster) {

      // Create hit list
      std::vector<const larlight::hit*> hits;
      const std::vector<unsigned short> &hit_indices = c.association(hit_type);
      hits.reserve(hit_indices.size());

      for(auto const& index : hit_indices)

	hits.push_back(&(ev_hit->at(index)));

      qfrac_v.push_back(fBTAlgo.MatchHitsAll(hits, simch_map, shower_idmap));
    }

    // Find the most representative reco-ed cluster per MCShower per plane
    std::vector<UInt_t> mcshower_id;    // index number of MCShowers (in event_mcshower) that are taken into account 
    fBTAlgo.UniqueShowerID(shower_idmap,mcshower_id);

    std::vector<std::vector<size_t> > mc_match_v(mcshower_id.size(),
						 std::vector<size_t>(geo->Nplanes(),0)
						 );
    std::vector<std::vector<float> > mc_match_charge_v(mcshower_id.size(),
						       std::vector<float>(geo->Nplanes(),0)
						       );
    std::vector<std::vector<float> > mc_sum_charge_v(mcshower_id.size(),
						     std::vector<float>(geo->Nplanes(),0)
						     );

    for(size_t cluster_index=0; cluster_index < ev_cluster->size(); ++cluster_index) {
      
      const larlight::cluster &c = ev_cluster->at(cluster_index);
      
      UChar_t plane = _view_to_plane.at(c.View());

      Double_t cluster_q = c.Charge();
      
      for(size_t mcshower_index=0; mcshower_index < mcshower_id.size(); ++mcshower_index) {
	
	float mcshower_q = cluster_q * qfrac_v.at(cluster_index).at(mcshower_index);

	if(mc_match_charge_v.at(mcshower_index).at(plane) < mcshower_q) {

	  mc_match_v.at(mcshower_index).at(plane) = cluster_index;

	  mc_match_charge_v.at(mcshower_index).at(plane) = mcshower_q;

	}
	
	mc_sum_charge_v.at(mcshower_index).at(plane) += mcshower_q;
      }

    }
    
    //
    // Fill MCTruth matching efficiency plot 
    //

    for(size_t mcshower_index=0; mcshower_index < mcshower_id.size(); ++mcshower_index) {

      float eff = 1;

      for(size_t plane=0; plane<geo->Nplanes(); ++plane)

	eff *= mc_match_charge_v.at(mcshower_index).at(plane) / mc_sum_charge_v.at(mcshower_index).at(plane);

      hMatchChargeEff_MC->Fill(eff);
      
    }

    hMatchNumEff_MC->Fill( (float)(mcshower_id.size()) / (float)(reco_match_v.size()) );

    return true;
  }

  bool CMatchPerformance::finalize() {

    if(_fout) {

      _fout->cd();
      
      hMatchChargeEff_MC->Write();

      hMatchNumEff_MC->Write();

    }

    return true;
  }

}
#endif
