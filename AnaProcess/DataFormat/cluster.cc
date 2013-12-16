#ifndef CLUSTER_CC
#define CLUSTER_CC

#include "cluster.hh"

namespace larlight{
  //############################################################################
  void cluster::get_axis_range(std::vector<double> &chmax,
			       std::vector<double> &chmin,
			       std::vector<double> &wiremax, 
			       std::vector<double> &wiremin, 
			       std::vector<double> &timemax, 
			       std::vector<double> &timemin) const
  //############################################################################
  {

    // Make sure input vector is of size wire plane with initial value -1 (if not yet set)
    chmax.resize((GEO::kW+1),-1);
    wiremax.resize((GEO::kW+1),-1);
    timemax.resize((GEO::kW+1),-1);
    chmin.resize((GEO::kW+1),-1);
    wiremin.resize((GEO::kW+1),-1);
    timemin.resize((GEO::kW+1),-1);

    //vtx is a vector of hits for one cluster?
    for(const auto &hit : fHitVector) {

      GEO::View_t  view    =  hit.View();
      double       wire    =  (double)(hit.Wire());
      double       ch      =  (double)(hit.Channel());
      double       tstart  =  hit.StartTime();
      double       tend    =  hit.EndTime();

      if( wiremax[view] < 0 || wiremax[view] < wire )  wiremax[view] = wire;
      if( chmax[view]   < 0 || chmax[view]   < ch   )  chmax[view]   = ch;
      if( timemax[view] < 0 || timemax[view] < tend )  timemax[view] = tend;

      if( wiremin[view] < 0 || wiremin[view] > wire   )  wiremin[view] = wire;
      if( chmin[view]   < 0 || chmin[view]   > ch     )  chmin[view]   = ch;
      if( timemin[view] < 0 || timemin[view] > tstart )  timemin[view] = tstart;
      
    }

    return;
  }


  //############################################################################
  void event_cluster::get_axis_range(std::vector<double> &chmax,
				     std::vector<double> &chmin,
				     std::vector<double> &wiremax, 
				     std::vector<double> &wiremin, 
				     std::vector<double> &timemax, 
				     std::vector<double> &timemin) const
  //############################################################################
  {
    //fCluster_v is a vector of all clusters in the event
    for(auto const &cluster : fCluster_v){

      cluster.get_axis_range(chmax,chmin,wiremax,wiremin,timemax,timemin);

    }

    return;
  }
}  
  
#endif
  
