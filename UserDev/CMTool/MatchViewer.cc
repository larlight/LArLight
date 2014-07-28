#ifndef MATCHVIEWER_CC
#define MATCHVIEWER_CC

#include "MatchViewer.hh"

namespace larlight {
  
  //################################################################
  MatchViewer::MatchViewer() : ::larlight::ClusterMatcher(),
			       _algo("MatchViewer")
  //################################################################
  {
    // Class name
    _name = "MatchViewer";

    // Show Showers
    _showerColor = true;
    // Default values for stuff
    SetDrawPolygon(false);
    SetPrintClusterInfo(false);
    SetMinHitsToDraw(0);
    SetDrawTracks(true);
    SetDrawOnlyTracks(false);
  }
  
  //################################################################
  bool MatchViewer::initialize()
  //################################################################
  {
    return ClusterMatcher::initialize();
  }
  
  //################################################################
  bool MatchViewer::analyze(storage_manager* storage)
  //################################################################
  {
    _algo.Reset();

    // Now run ClusterMatcher on this event
    ClusterMatcher::analyze(storage);
    
    const ::larutil::Geometry* geo = ::larutil::Geometry::GetME();
    const ::larutil::GeometryUtilities* geo_util = ::larutil::GeometryUtilities::GetME();
    
    UChar_t nplanes = geo->Nplanes();
    double  wire2cm = geo_util->WireToCm();
    double  time2cm = geo_util->TimeToCm();
    
    //
    // Obtain event-wise data object pointers
    //
    event_cluster* ev_clus = (event_cluster*) ( storage->get_data(_cluster_type));
    if(!ev_clus)
      throw ::cluster::ViewerException(Form("Did not find %s data product!",
					    DATA::DATA_TREE_NAME[_cluster_type].c_str()
					    )
				       );
    
    const DATA::DATA_TYPE hit_type = ev_clus->get_hit_type();
    
    event_hit*     ev_hit  = (event_hit*) ( storage->get_data(hit_type));


    
    if(!ev_hit)
      throw ::cluster::ViewerException(Form("Did not find %s data product!",
					    DATA::DATA_TREE_NAME[hit_type].c_str()
					    )
				       );

    //********************DavidC
    //Begin Shower Getter stuff

    std::vector<float> hit_charge_frac;
    std::vector<UInt_t> MCShower_indices;
    int n_showers = 0;

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
    //*********************David Caratelli
    //End Shower getter stuff

    
    // Find hit range 
    std::vector<std::pair<double,double> > xrange(nplanes,std::pair<double,double>(1e9,0));
    std::vector<std::pair<double,double> > yrange(nplanes,std::pair<double,double>(1e9,0));
    for(auto const &h : *ev_hit) {
      
      UChar_t plane = geo->ChannelToPlane(h.Channel());
      double x = h.Wire() * wire2cm;
      double y = h.PeakTime() * time2cm;
      
      if(xrange.at(plane).first > x ) xrange.at(plane).first = x;
      if(xrange.at(plane).second < x ) xrange.at(plane).second = x;
      
      if(yrange.at(plane).first > y ) yrange.at(plane).first = y;
      if(yrange.at(plane).second < y ) yrange.at(plane).second = y;
      
    }
    
    // Inform the algorithm about the range
    for(size_t i=0; i<nplanes; ++i)
      _algo.SetRange(i,xrange.at(i),yrange.at(i));

    // Find hits-per-cluster
    std::vector<std::vector<std::pair<double,double> > > hits_xy(nplanes,std::vector<std::pair<double,double> >());
    std::vector<std::vector<double> > hits_charge(nplanes,std::vector<double>());
    std::vector<std::vector<std::vector<std::pair<double,double> > > > shower_xy(nplanes,
										 std::vector<std::vector<std::pair<double,double> > >(n_showers,std::vector<std::pair<double,double> >()));

    auto res = GetManager().GetBookKeeper().GetResult();
    for(auto const &c_pair : GetManager().GetBookKeeper().GetResult() ) {

      for(auto const &c_index : c_pair) {

	auto const& cl = GetManager().GetInputClusters().at(c_index);

	UChar_t plane = cl.Plane();

	// Get associated hits
	for(auto const& h_index : ev_clus->at(c_index).association(hit_type)) {

	  auto const& h = ev_hit->at(h_index);
	  
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
	}
	
	std::vector<std::pair<double,double> > cluster_hits;
	std::pair<double,double> cluster_start  ( cl.GetParams().start_point.w, cl.GetParams().start_point.t ); 
	std::pair<double,double> cluster_end    ( cl.GetParams().end_point.w,   cl.GetParams().end_point.t   ); 
      
	for(auto const& h : cl.GetHitVector())
	  
	  cluster_hits.push_back(std::pair<double,double>( h.w, h.t));
      
	std::vector<std::pair<double,double> > cluster_polygon(cl.GetParams().PolyObject.Size() + 1,
							       std::pair<double,double>(0,0)
							       );
      
	for(size_t i=0; i<cl.GetParams().PolyObject.Size(); ++i) {
	  cluster_polygon.at(i).first = cl.GetParams().PolyObject.Point(i).first;
	  cluster_polygon.at(i).second = cl.GetParams().PolyObject.Point(i).second;
	}
	
	if(cluster_polygon.size()){
	  cluster_polygon.at(cluster_polygon.size()-1).first = cluster_polygon.at(0).first;
	  cluster_polygon.at(cluster_polygon.size()-1).second = cluster_polygon.at(0).second;
	}
	
	//only draw clusters with more than _min_hits_to_draw
	if(cluster_hits.size() > _min_hits_to_draw){
	  
	  bool is_track = cl.GetParams().eigenvalue_principal > 0.99000;
	  
	  //if it's not a track, draw it only if !_draw_only_tracks
	  if(!is_track){
	    if(!_draw_only_tracks){
	      if(_draw_polygon){
		_algo.AddCluster(plane,
				 cluster_hits,
				 cluster_start,
				 cluster_end,
				 cluster_polygon);
	      }
	      else{
		_algo.AddCluster(plane,
				 cluster_hits,
				 cluster_start,
				 cluster_end);
	      }
	    }
	  }//end if it is not a track
	  
	  else{//if it is a track, draw it if _draw_tracks is true
	    if(_draw_tracks){
	      if(_draw_polygon){
		_algo.AddCluster(plane,
				 cluster_hits,
				 cluster_start,
				 cluster_end,
				 cluster_polygon);
	      }
	      else{
		_algo.AddCluster(plane,
				 cluster_hits,
				 cluster_start,
				 cluster_end);
	      }
	    }//end if(_draw_tracks)
	  }//end if it is a track
	}//end only drawing clusters with more than min hits
      }//end loop over clusters
    }

    // Provide plane-hits to the algorithm
    for(size_t i=0; i<nplanes; ++i)
      if ( _showerColor ){
	for(int s=0; s < n_showers; ++s){
	  _algo.AddShowers(i,shower_xy.at(i).at(s));
	}
      }
      else
	_algo.AddHits(i,hits_xy.at(i),hits_charge.at(i));

    return true;
  };

  bool MatchViewer::finalize() {

    return ClusterMatcher::finalize();

  }

  void MatchViewer::PrintClusterInfo(UChar_t plane, size_t index){

    int index_offset = 0;
    //get the clusters (after merging)
    for (auto const &cl : ClusterMatcher::GetManager().GetInputClusters()) {
      if (cl.Plane() == plane) break;
      else index_offset++;
    }
    
    index = index + index_offset;
    auto cl = ClusterMatcher::GetManager().GetInputClusters().at(index);
    std::cout<<"**********PRINTING CLUSTER INFO**********"<<std::endl;
    std::cout<<"Angle 2D = "<<cl.GetParams().angle_2d<<std::endl;
    std::cout<<"Opening Angle = "<<cl.GetParams().opening_angle<<std::endl;
    std::cout<<"Sum charge = "<<cl.GetParams().sum_charge<<std::endl;
    std::cout<<"Length = "<<cl.GetParams().length<<std::endl;
    std::cout<<"Width = "<<cl.GetParams().width<<std::endl;
    std::cout<<"N_Hits = "<<cl.GetParams().N_Hits<<std::endl;
    std::cout<<"eigenvalue_principal = "<<cl.GetParams().eigenvalue_principal<<std::endl;
  }
  
}
#endif

