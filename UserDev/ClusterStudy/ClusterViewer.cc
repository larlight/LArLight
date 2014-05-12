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
  }

  //#######################################
  void ClusterViewer::SetClusterType(DATA::DATA_TYPE type)
  //#######################################
  {
    if( type != DATA::Cluster &&
	type != DATA::FuzzyCluster &&
	type != DATA::CrawlerCluster &&
	type != DATA::ShowerAngleCluster &&
	type != DATA::HoughCluster )

      print(MSG::ERROR,__FUNCTION__,Form("Not a valid cluster type: %s",DATA::DATA_TREE_NAME[type].c_str()));
    
    else
      cluster_type=type;

  }
  
  //################################################################
  bool ClusterViewer::initialize()
  //################################################################
  {
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

    // Find hits-per-cluster
    for(auto const &cl : *ev_clus) {

      UChar_t plane = nplanes;

      std::vector<std::pair<double,double> > cluster_hits;
      std::pair<double,double> cluster_start  ( cl.StartPos().at(0)*wire2cm, cl.StartPos().at(1)*time2cm );
      std::pair<double,double> cluster_end    ( cl.EndPos().at(0)*wire2cm,   cl.EndPos().at(1)*time2cm     );
      
      for(auto const& index : cl.association(hit_type)) {

	cluster_hits.push_back(std::pair<double,double>( ev_hit->at(index).Wire() * wire2cm,
							 ev_hit->at(index).PeakTime() * time2cm )
			       );
	if(plane == nplanes)

	  plane = geo->ChannelToPlane(ev_hit->at(index).Channel());
      
      }

      _algo.AddCluster(plane,
		       cluster_hits,
		       cluster_start,
		       cluster_end);
    }
    
    return true;
  };
  
  bool ClusterViewer::finalize() {
    
    return true;
  }

}
#endif

