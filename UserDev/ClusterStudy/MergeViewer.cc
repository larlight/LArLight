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
      } //kaleko added this bracket around if statement. did i fix it, or break it?
      
      if(_draw_polygon)
	_algo.AddCluster(plane,
			 cluster_hits,
			 cluster_start,
			 cluster_end,
			 cluster_polygon);
      else
	_algo.AddCluster(plane,
			 cluster_hits,
			 cluster_start,
			 cluster_end);
      
    }
    
    return true;
  };
  
  bool MergeViewer::finalize() {

    return ClusterMerger::finalize();

  }

}
#endif

