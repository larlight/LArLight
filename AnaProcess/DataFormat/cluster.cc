#ifndef CLUSTER_CC
#define CLUSTER_CC

#include "cluster.hh"

namespace larlight{
  //############################################################################
  void cluster::get_axis_range(std::vector<double> &wiremax, 
			       std::vector<double> &wiremin, 
			       std::vector<double> &timemax, 
			       std::vector<double> &timemin) const
  //############################################################################
  {

    //vtx is a vector of hits for one cluster?
    for(const auto &hit : fHitVector) {

      int vw = hit.View();
      
      if(hit.Channel() > wiremax[vw]) wiremax[vw] = hit.Channel();
      if(hit.Channel() < wiremin[vw]) wiremin[vw] = hit.Channel();
      if(hit.EndTime() > timemax[vw]) timemax[vw] = hit.EndTime();
      if(hit.StartTime() < timemin[vw]) timemin[vw] = hit.StartTime();
      
    }

    return;
  }


  //############################################################################

  void event_cluster::get_axis_range(std::vector<double> &wiremax, 
				     std::vector<double> &wiremin, 
				     std::vector<double> &timemax, 
				     std::vector<double> &timemin) const
  //############################################################################
  {
    //fCluster_v is a vector of all clusters in the event
    for(auto clus : fCluster_v){

	clus.get_axis_range(wiremax,wiremin,timemax,timemin);

    }

    return;
  }
}  
  
#endif
  
