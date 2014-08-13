#ifndef CMATCHPERFORMANCE_CC
#define CMATCHPERFORMANCE_CC

#include "CMatchPerformance.hh"

namespace larlight {

  bool CMatchPerformance::initialize() {

    fBTAlgo.SetMaxEnergyCut(1900/2.);

    auto geo = ::larutil::Geometry::GetME();
    _view_to_plane.clear();
    _view_to_plane.resize(geo->Nplanes(),DATA::INVALID_UCHAR);

    for(UChar_t plane=0; plane<geo->Nplanes(); ++plane) {

      UChar_t view = geo->PlaneToView(plane);

      if(_view_to_plane.size() <= view) _view_to_plane.resize(view+1,DATA::INVALID_UCHAR);

      _view_to_plane.at(view) = plane;

    }

    hMatchQEff = new TH1D("hMatchChargeEff",
			  "MC Matched Pair Efficiency; Efficiency; Num. MCShower;",
			  101,-0.005,1.005);

    hMatchQEffEvent = new TH1D("hMatchChargeEffEvent",
			       "MC Matched Pair Efficiency; Efficiency; Num. MCShower;",
			       101,-0.005,1.005);

    hMatchNumEff = new TH1D("hMatchNumEff",
			    "Number of MCShower / Reco-Matched Pair; Efficiency; Events;",
			    101,-0.005,1.005);

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

    try{

      fMgr.Process();
    }catch( ::cmtool::CMTException &e){
      e.what();
      return false;
    }

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
    std::vector<double> cluster_charge;
    qfrac_v.reserve(ev_cluster->size());
    cluster_charge.reserve(ev_cluster->size());
    for(auto const& c : *ev_cluster) {

      // Create hit list
      std::vector<const larlight::hit*> hits;
      const std::vector<unsigned short> &hit_indices = c.association(hit_type);
      hits.reserve(hit_indices.size());

      cluster_charge.push_back(0);
      for(auto const& index : hit_indices) {

	hits.push_back(&(ev_hit->at(index)));
	(*cluster_charge.rbegin()) += ev_hit->at(index).Charge();
	
      }

      qfrac_v.push_back(fBTAlgo.MatchHitsAll(hits, simch_map, shower_idmap));

    }

    //
    // Find the best matched pair (and its charge) per MCShower
    //

    std::vector<UInt_t> mcshower_id;    // index number of MCShowers (in event_mcshower) that are taken into account 
    fBTAlgo.UniqueShowerID(shower_idmap,mcshower_id);

    std::vector<std::vector<double> > bmatch_q  (mcshower_id.size(),std::vector<double>(geo->Nplanes(),0));
    std::vector<std::vector<size_t> > bmatch_id (mcshower_id.size(),std::vector<size_t>(geo->Nplanes(),0));

    for(size_t shower_index=0; shower_index < mcshower_id.size(); ++shower_index) {

      for(size_t cluster_index=0; cluster_index < ev_cluster->size(); ++cluster_index) {

	if((*(qfrac_v.at(cluster_index).rbegin())) < 0) continue;

	auto plane = _view_to_plane.at(ev_cluster->at(cluster_index).View());

	double q = qfrac_v.at(cluster_index).at(shower_index) * cluster_charge.at(cluster_index);

	if( bmatch_q.at(shower_index).at(plane) < q ) {

	  bmatch_q.at(shower_index).at(plane)  = q;
	  bmatch_id.at(shower_index).at(plane) = cluster_index;

	}
	
      }
    }

    //
    // Evaluate efficiency
    //
    double max_eff=0;
    for(auto const& match : reco_match_v) {

      // Compute efficiency per MCShower
      std::vector<double> match_eff(mcshower_id.size(),1);

      for(auto const& cluster_index : match) {

	for(size_t shower_index=0; shower_index < mcshower_id.size(); ++shower_index) {

	  unsigned char plane = _view_to_plane.at(ev_cluster->at(cluster_index).View());

	  double q = cluster_charge.at(cluster_index)* qfrac_v.at(cluster_index).at(shower_index);

	  match_eff.at(shower_index) *= q / bmatch_q.at(shower_index).at(plane);
	}

      }

      // Find the best qratio
      double qratio_max = -1;
      for(auto const& eff : match_eff)

	if(qratio_max < eff) qratio_max = eff;

      if(max_eff < qratio_max) max_eff = qratio_max;

      hMatchQEff->Fill(qratio_max);
    }

    if(reco_match_v.size()) {

      hMatchQEffEvent->Fill(max_eff);

      hMatchNumEff->Fill( (float)(mcshower_id.size()) / (float)(reco_match_v.size()) );
    }

    return true;
  }

  bool CMatchPerformance::finalize() {

    if(_fout) {

      _fout->cd();
      
      hMatchQEff->Write();

      hMatchNumEff->Write();

      hMatchQEffEvent->Write();

    }

    return true;
  }

}
#endif
