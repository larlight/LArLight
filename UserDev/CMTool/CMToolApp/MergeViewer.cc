#ifndef MERGEVIEWER_CC
#define MERGEVIEWER_CC

#include "MergeViewer.hh"

namespace larlight {
  
  //################################################################
  MergeViewer::MergeViewer() : ::larlight::ClusterMerger(),
			       _algo("MergeViewer")
  //################################################################
  {
    // Class name
    _name = "MergeViewer";
    
    // Default values for stuff
    SetDrawPolygon(false);
    SetPrintClusterInfo(false);
    SetMinHitsToDraw(0);
    SetDrawTracks(true);
    SetDrawOnlyTracks(false);
    SetDrawStartEnd(false);
  }
  
  //################################################################
  bool MergeViewer::initialize()
  //################################################################
  {
    return ClusterMerger::initialize();
  }
  
  //################################################################
  bool MergeViewer::analyze(storage_manager* storage)
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
    
    // Find hit range & fill all-hits vector
    std::vector<std::pair<double,double> > xrange(nplanes,std::pair<double,double>(1e9,0));
    std::vector<std::pair<double,double> > yrange(nplanes,std::pair<double,double>(1e9,0));
    std::vector<std::vector<std::pair<double,double> > > hits_xy(nplanes,std::vector<std::pair<double,double> >());
    std::vector<std::vector<double> > hits_charge(nplanes,std::vector<double>());
    for(auto const &h : *ev_hit) {
      
      UChar_t plane = geo->ChannelToPlane(h.Channel());
      double x = h.Wire() * wire2cm;
      double y = h.PeakTime() * time2cm;
      
      if(xrange.at(plane).first > x ) xrange.at(plane).first = x;
      if(xrange.at(plane).second < x ) xrange.at(plane).second = x;
      
      if(yrange.at(plane).first > y ) yrange.at(plane).first = y;
      if(yrange.at(plane).second < y ) yrange.at(plane).second = y;
      
      hits_xy.at(plane).push_back(std::pair<double,double>(x,y));
      hits_charge.at(plane).push_back(h.Charge());
    }
    
    // Inform the algorithm about the range
    for(size_t i=0; i<nplanes; ++i)
      _algo.SetRange(i,xrange.at(i),yrange.at(i));
    
    // Provide plane-hits to the algorithm
    for(size_t i=0; i<nplanes; ++i)
      _algo.AddHits(i,hits_xy.at(i),hits_charge.at(i));
    
    
    // Now run ClusterMerger on this event
    ClusterMerger::analyze(storage);
    
    // Find hits-per-cluster
    for(auto const &cl : ClusterMerger::GetManager().GetClusters()) {
      
      UChar_t plane = cl.Plane();
      
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
	    if( _draw_polygon and _showStartEnd )
	      _algo.AddCluster(plane,
			       cluster_hits,
			       cluster_start,
			       cluster_end,
			       cluster_polygon);
	    else if ( _draw_polygon and !_showStartEnd )
	      _algo.AddCluster(plane,
			       cluster_hits,
			       cluster_polygon);
	    else if ( !_draw_polygon and _showStartEnd )
	      _algo.AddCluster(plane,
			       cluster_hits,
			       cluster_start,
			       cluster_end);
	    else if ( !_draw_polygon and !_showStartEnd )
	      _algo.AddCluster(plane,
			       cluster_hits);
	  }//end if draw only tracks
	}//end if it is not a track
	
	else{//if it is a track, draw it if _draw_tracks is true
	  if(_draw_tracks){
	    if( _draw_polygon and _showStartEnd )
	      _algo.AddCluster(plane,
			       cluster_hits,
			       cluster_start,
			       cluster_end,
			       cluster_polygon);
	    else if ( _draw_polygon and !_showStartEnd )
	      _algo.AddCluster(plane,
			       cluster_hits,
			       cluster_polygon);
	    else if ( !_draw_polygon and _showStartEnd )
	      _algo.AddCluster(plane,
			       cluster_hits,
			       cluster_start,
			       cluster_end);
	    else if ( !_draw_polygon and !_showStartEnd )
	      _algo.AddCluster(plane,
			       cluster_hits);
	  }//end if(_draw_tracks)
	}//end if it is a track
      }//end only drawing clusters with more than min hits
    }//end loop over clusters
    
    return true;
  };

  bool MergeViewer::finalize() {

    return ClusterMerger::finalize();

  }

  void MergeViewer::PrintClusterInfo(UChar_t plane, size_t index){

    int index_offset = 0;
    //get the clusters (after merging)
    for (auto const &cl : ClusterMerger::GetManager().GetClusters()) {
      if (cl.Plane() == plane) break;
      else index_offset++;
    }
    
    index = index + index_offset;
    auto cl = ClusterMerger::GetManager().GetClusters().at(index);
    std::cout<<"**********PRINTING CLUSTER INFO**********"<<std::endl;
    std::cout<<"Angle 2D = "<<cl.GetParams().angle_2d<<std::endl;
    std::cout<<"Opening Angle = "<<cl.GetParams().opening_angle<<std::endl;
    std::cout<<"Sum charge = "<<cl.GetParams().sum_charge<<std::endl;
    std::cout<<"Length = "<<cl.GetParams().length<<std::endl;
    std::cout<<"Width = "<<cl.GetParams().width<<std::endl;
    std::cout<<"N_Hits = "<<cl.GetParams().N_Hits<<std::endl;
    std::cout<<"eigenvalue_principal = "<<cl.GetParams().eigenvalue_principal<<std::endl;
    std::cout<<"modified hit density = "<<cl.GetParams().modified_hit_density<<std::endl;
    std::cout<<"multi hit wires = "<<cl.GetParams().multi_hit_wires<<std::endl;
  }
  
}
#endif

