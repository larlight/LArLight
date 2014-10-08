#ifndef CLUSTERVIEWER_CC
#define CLUSTERVIEWER_CC

#include "ClusterViewer.hh"

namespace larlight {
  //################################################################
  ClusterViewer::ClusterViewer() : ana_base(),
				   _algo("ClusterViewer")
  //################################################################
  {
    // Class name
    _name = "ClusterViewer";
    cluster_type=DATA::FuzzyCluster;
    //default is to use hits viewer (need to hadd files first for MC showerviewer)
    SetDrawShowers(false);
    //default is now to use log-z scale for hits viewer
    SetHitsLogZ(true);
    SetDrawStartEnd(false);
  }

  //#######################################
  void ClusterViewer::SetDrawShowers(bool on)
  //#######################################
  {
    _showerColor = on;
    _algo.ShowShowers(_showerColor);
  }


  //#######################################
  void ClusterViewer::SetHitsLogZ(bool flag)
  //#######################################
  {
    _algo.SetHitsLogZ(flag);
  }

  //#######################################
  void ClusterViewer::SetClusterType(DATA::DATA_TYPE type)
  //#######################################
  {
    if( type != DATA::Cluster &&
	type != DATA::DBCluster &&
	type != DATA::MCShowerCluster &&
	type != DATA::FuzzyCluster &&
	type != DATA::CrawlerCluster &&
	type != DATA::ShowerAngleCluster &&
	type != DATA::HoughCluster &&
	type != DATA::RyanCluster )

      throw ::cluster::ViewerException(Form("Not a valid cluster type: %s",
					    DATA::DATA_TREE_NAME[cluster_type].c_str())
				       );

    cluster_type=type;

  }
  
  //################################################################
  bool ClusterViewer::initialize()
  //################################################################
  {
    _algo.Reset();
    return true;
  }
  
  //################################################################
  bool ClusterViewer::analyze(storage_manager* storage)
  //################################################################
  {

    _algo.Reset();
    
    const ::larutil::Geometry* geo = ::larutil::Geometry::GetME();
    const ::larutil::GeometryUtilities* geo_util = ::larutil::GeometryUtilities::GetME();
    
    UChar_t nplanes = geo->Nplanes();
    double  wire2cm = geo_util->WireToCm();
    double  time2cm = geo_util->TimeToCm();

    //
    // Obtain event-wise data object pointers
    //
    event_cluster* ev_clus = (event_cluster*) ( storage->get_data(cluster_type));
    if(!ev_clus)
      throw ::cluster::ViewerException(Form("Did not find %s data product!",
					    DATA::DATA_TREE_NAME[cluster_type].c_str()
					    )
				       );

    const DATA::DATA_TYPE hit_type = ev_clus->get_hit_type();
					  
    event_hit*     ev_hit  = (event_hit*) ( storage->get_data(hit_type));

    if(!ev_hit)
      throw ::cluster::ViewerException(Form("Did not find %s data product!",
					    DATA::DATA_TREE_NAME[hit_type].c_str()
					    )
				       );

    std::vector<float> hit_charge_frac;
    std::vector<UInt_t> MCShower_indices;
    int n_showers = 0;

    //DavidC
    if ( _showerColor ) {
      
      //grab the MC showers
      larlight::event_mcshower* ev_mcshower = (larlight::event_mcshower*) ( storage->get_data(larlight::DATA::MCShower));
      if(!ev_mcshower) {
	print(larlight::MSG::ERROR,__FUNCTION__,Form("Did not find specified data product, MCShower!"));
	return false;
      }
      
      //grab the simchannels
      larlight::event_simch* ev_simch = (larlight::event_simch*)( storage->get_data(larlight::DATA::SimChannel));
      if(!ev_simch) {
	print(larlight::MSG::ERROR,__FUNCTION__,Form("Did not find specified data product, SimChannel!"));
	return false;
      }

      _mcslb.SetMinEnergyCut(0.02);
      _mcslb.SetMaxEnergyCut(10.0);
      _shower_idmap.clear();
      _mcslb.FillShowerMap(ev_mcshower,_shower_idmap);
      _simch_map.clear();
      _mcslb.FillSimchMap(ev_simch,_simch_map);
      n_showers = ev_mcshower->size();
      for(UInt_t i=0; i < ev_mcshower->size(); ++i)
	MCShower_indices.push_back(i);
    }

    // Find hit range & fill all-hits vector
    std::vector<std::pair<double,double> > xrange(nplanes,std::pair<double,double>(1e9,0));
    std::vector<std::pair<double,double> > yrange(nplanes,std::pair<double,double>(1e9,0));
    std::vector<std::vector<std::pair<double,double> > > hits_xy(nplanes,std::vector<std::pair<double,double> >());
    std::vector<std::vector<double> > hits_charge(nplanes,std::vector<double>());
    std::vector<std::vector<std::vector<std::pair<double,double> > > > shower_xy(nplanes,
										 std::vector<std::vector<std::pair<double,double> > >(n_showers,std::vector<std::pair<double,double> >()));

    for(auto const &h : *ev_hit) {
      
      UChar_t plane = geo->ChannelToPlane(h.Channel());
      double x = h.Wire() * wire2cm;
      double y = h.PeakTime() * time2cm;
      
      if(xrange.at(plane).first > x ) xrange.at(plane).first = x;
      if(xrange.at(plane).second < x ) xrange.at(plane).second = x;

      if(yrange.at(plane).first > y ) yrange.at(plane).first = y;
      if(yrange.at(plane).second < y ) yrange.at(plane).second = y;
      
      hits_xy.at(plane).push_back(std::pair<double,double>(x,y));


      //DavidC
      if ( _showerColor ){
	hit_charge_frac.clear();
	
	hit_charge_frac = 
	  _mcslb.MatchHitsAll(h,
			      _simch_map,
			      _shower_idmap,
			      MCShower_indices);
	//find item with largest fraction
	int   max_item = 0;
	float max_frac = 0.;
	for ( size_t i=0; i < hit_charge_frac.size(); i++){
	  if ( hit_charge_frac.at(i) > max_frac ) { 
	    max_item = i;
	    max_frac = hit_charge_frac.at(i);
	  }
	}
	if ( max_item < n_showers ){
	  shower_xy.at(plane).at(max_item).push_back(std::pair<double,double>(x,y));
	}
      }
      else {
	hits_charge.at(plane).push_back(h.Charge());
      }

    }//for all hits

    // Inform the algorithm about the range
    for(size_t i=0; i<nplanes; ++i)
      _algo.SetRange(i,xrange.at(i),yrange.at(i));

    // Provide plane-hits to the algorithm
    for(size_t i=0; i<nplanes; ++i)
      if ( _showerColor ){
	for(int s=0; s < n_showers; ++s){
	  _algo.AddShowers(i,shower_xy.at(i).at(s));
	}
      }
      else
	_algo.AddHits(i,hits_xy.at(i),hits_charge.at(i));
    //DavidC--placeholder--instead of hits_charge.at(i)
    //use color dependent on _mcslb return value

    // Find hits-per-cluster
    for(auto const &cl : *ev_clus) {

      UChar_t plane = nplanes;

      std::vector<std::pair<double,double> > cluster_hits;
      
      for(auto const& index : cl.association(hit_type)) {

	cluster_hits.push_back(std::pair<double,double>( ev_hit->at(index).Wire() * wire2cm,
							 ev_hit->at(index).PeakTime() * time2cm )
			       );
	if(plane == nplanes)

	  plane = geo->ChannelToPlane(ev_hit->at(index).Channel());
      
      }

      if(plane == nplanes) {

	throw ::cluster::ViewerException("No associated hits found!");

	return true;
      }
      
      if ( _showStartEnd ){

	//for start/end point, need to calculate CPAN
       	std::vector<unsigned int> hit_index = cl.association(hit_type);
	::cluster::ClusterParamsAlg bestclus_CPAN;
	_cru_helper.GenerateCPAN( hit_index, ev_hit, bestclus_CPAN);
	bestclus_CPAN.DisableFANN();
	bestclus_CPAN.SetVerbose(false);
	bestclus_CPAN.FillParams(true,true,true,true,true,true);

	std::pair<double,double> cluster_start  ( bestclus_CPAN.GetParams().start_point.w, bestclus_CPAN.GetParams().start_point.t );
	std::pair<double,double> cluster_end    ( bestclus_CPAN.GetParams().end_point.w,   bestclus_CPAN.GetParams().end_point.t     );
	
	_algo.AddCluster(plane,
			 cluster_hits,
			 cluster_start,
			 cluster_end);
      }
      else
	_algo.AddCluster(plane,
			 cluster_hits);
      
    }

    return true;
  };
  
  bool ClusterViewer::finalize() {
    
    return true;
  }

}
#endif

