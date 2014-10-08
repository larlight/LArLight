#ifndef MCSHOWERCLUSTERVIEWER_CC
#define MCSHOWERCLUSTERVIEWER_CC

#include "MCShowerClusterViewer.hh"

namespace larlight {
  //################################################################
  MCShowerClusterViewer::MCShowerClusterViewer() : ana_base(),
						   _algo("MCShowerClusterViewer")
  //################################################################
  {
    // Class name
    _name = "MCShowerClusterViewer";
  }

  //################################################################
  bool MCShowerClusterViewer::initialize()
  //################################################################
  {
    return true;
  }
  
  //################################################################
  bool MCShowerClusterViewer::analyze(storage_manager* storage)
  //################################################################
  {
    _algo.Reset();
    
    const ::larutil::Geometry* geo = ::larutil::Geometry::GetME();
    const ::larutil::GeometryUtilities* geo_util = ::larutil::GeometryUtilities::GetME();
    
    const UChar_t nplanes = geo->Nplanes();
    double  wire2cm = geo_util->WireToCm();
    double  time2cm = geo_util->TimeToCm();

    //
    // Obtain event-wise data object pointers
    //
    event_mcshower* ev_mcs = (event_mcshower*) ( storage->get_data(DATA::MCShower) );
    if(!ev_mcs) {
      std::cout<< Form("Did not find %s data product... skipping %s::analyze()",
		       DATA::DATA_TREE_NAME[DATA::MCShower].c_str(),
		       _name.c_str())
	       << std::endl;
      return true;
    }


    //David Caratelli
    //event hits used to set x-y range of TH2D so that it is same
    //as cluster viewer panel
    //Assume data type is fuzzycluster...does not really matter
    event_cluster* ev_clus = (event_cluster*) ( storage->get_data(DATA::FuzzyCluster));
    if(!ev_clus)
      throw ::cluster::ViewerException(Form("Did not find %s data product!",
					    DATA::DATA_TREE_NAME[DATA::FuzzyCluster].c_str()
					    )
				       );
    
    const DATA::DATA_TYPE hit_type = ev_clus->get_hit_type();
    
    event_hit*     ev_hit  = (event_hit*) ( storage->get_data(hit_type));


    if(!ev_hit)
      throw ::cluster::ViewerException(Form("Did not find %s data product!",
					    DATA::DATA_TREE_NAME[hit_type].c_str()
					    )
				       );



    // Find all-hits, cluseter hits, range, everything at once
    std::vector<std::pair<double,double> > xrange(nplanes,std::pair<double,double>(1e9,0));
    std::vector<std::pair<double,double> > yrange(nplanes,std::pair<double,double>(1e9,0));
    std::vector<std::vector<std::pair<double,double> > > hits_xy(nplanes,std::vector<std::pair<double,double> >());
    std::vector<std::vector<double> > hits_charge(nplanes,std::vector<double>());
    std::vector<std::vector<std::vector<std::pair<double,double> > > > cluster_hits(nplanes,std::vector<std::vector<std::pair<double,double> > >());
    std::vector<std::vector<std::pair<double,double> > > cluster_start(nplanes,std::vector<std::pair<double,double> >());
    std::vector<std::vector<std::pair<double,double> > > cluster_end(nplanes,std::vector<std::pair<double,double> >());


    // Reserve for all-hits & cluster vectors
    size_t total_hit_count = 0;
    for(auto const &mcs : *ev_mcs)
      total_hit_count += mcs.DaughterPoints().size();

    for(UChar_t plane = 0; plane<nplanes; ++plane) {

      hits_xy.at(plane).reserve(total_hit_count);
      hits_charge.at(plane).reserve(total_hit_count);

      cluster_hits.at(plane).reserve(ev_mcs->size());
      cluster_start.at(plane).reserve(ev_mcs->size());
      cluster_end.at(plane).reserve(ev_mcs->size());
    }

    //Set Range of x-y axes
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



    for(auto const &mcs : *ev_mcs) {

      for(UChar_t plane=0; plane<nplanes; ++plane) {

	larutil::PxPoint start_vtx;
	try{
	  start_vtx = geo_util->Get2DPointProjectionCM(mcs.MotherPosition(),plane);
	}catch(larutil::LArUtilException e){
	  std::cout<<e.what()<<std::endl;
	}

	larutil::PxPoint end_vtx = start_vtx;

	std::vector<std::pair<double,double> > chits;
	chits.reserve(mcs.DaughterPoints().size());
	std::vector<double> raw_vtx(4,0);

	for(auto const& vtx : mcs.DaughterPoints()) {
	  
	  raw_vtx.at(0) = vtx.at(0);
	  raw_vtx.at(1) = vtx.at(1);
	  raw_vtx.at(2) = vtx.at(2);
	  raw_vtx.at(3) = vtx.at(3);

	  larutil::PxPoint this_vtx;
	  try{
	    this_vtx = geo_util->Get2DPointProjectionCM(raw_vtx,plane);
	  }catch(larutil::LArUtilException e){
	    std::cout<<e.what()<<std::endl;
	  }

	  double this_dist = pow(start_vtx.t - this_vtx.t,2) + pow(start_vtx.w - this_vtx.w,2);
	  
	  double end_dist  = pow(start_vtx.t - end_vtx.t,2) + pow(start_vtx.w - end_vtx.w,2);
	  
	  if(this_dist > end_dist) {

	    end_vtx.w  = this_vtx.w;
	    end_vtx.t = this_vtx.t;

	  }

	  if(xrange.at(plane).first  > this_vtx.w) xrange.at(plane).first  = this_vtx.w;
	  if(xrange.at(plane).second < this_vtx.w) xrange.at(plane).second = this_vtx.t;
	  
	  hits_xy.at(plane).push_back(std::pair<double,double>(this_vtx.w,this_vtx.t));
	  hits_charge.at(plane).push_back(vtx.at(3));
	  
	  chits.push_back(std::pair<double,double>(this_vtx.w,this_vtx.t));
	}
	
	cluster_start.at(plane).push_back(std::pair<double,double>(start_vtx.w,start_vtx.t));
	cluster_end.at(plane).push_back(std::pair<double,double>(end_vtx.w,end_vtx.t));
	cluster_hits.at(plane).push_back(chits);
      }
    }

    // Inform the algorithm about the range
    //for(size_t plane=0; plane<nplanes; ++plane)
    //  _algo.SetRange(plane,xrange.at(plane),yrange.at(plane));

    // Provide all-hits and cluster info
    for(size_t plane=0; plane<nplanes; ++plane) {
      
      _algo.AddHits(plane,hits_xy.at(plane),hits_charge.at(plane));

      for(size_t cindex = 0; cindex < cluster_hits.at(plane).size(); ++cindex)
      
	_algo.AddCluster(plane,
			 cluster_hits.at(plane).at(cindex),
			 cluster_start.at(plane).at(cindex),
			 cluster_end.at(plane).at(cindex));

    }

    return true;
  };

  //#####################################
  bool MCShowerClusterViewer::finalize() 
  //#####################################
  {
    return true;
  }


  //###########################################
  void MCShowerClusterViewer::DrawAllClusters() 
  //###########################################
  {
    if(ClusterCount(0)<1) {

      std::cout<<" No MCShower found on any plane..."<<std::endl;

      return;

    }

    _algo.DrawAllClusters();

  }

  //#####################################################################
  void MCShowerClusterViewer::DrawOneCluster(UChar_t plane, size_t index) 
  //#####################################################################
  {
    if(ClusterCount(plane)<1) {
      
      std::cout<<Form(" No MCShower found on plane %d...",plane)<<std::endl;
      
      return;
      
    }
    
    _algo.DrawOneCluster(plane,index);
    
  }

}
#endif

