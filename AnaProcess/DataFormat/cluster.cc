#ifndef CLUSTER_CC
#define CLUSTER_CC

#include "cluster.hh"

namespace larlight{
  //############################################################################
  /*
  void cluster::get_axis_range(double &wiremax, double &wiremin,
			       double &timemax, double &timemin) const
  //############################################################################
  {
    
    //vtx is a vector of hits for one cluster?
    for(const auto &hit : fHitVector) {
      
      if(hit.Channel() > wiremax) wiremax = hit.Channel();
      if(hit.Channel() < wiremin) wiremin = hit.Channel();
      if(hit.EndTime() > timemax) timemax = hit.EndTime();
      if(hit.StartTime() < timemin) timemin = hit.StartTime();
      
    }

    return;
  }
  */

  //############################################################################

  void event_cluster::get_axis_range(std::vector<double> &wiremax, 
				     std::vector<double> &wiremin, 
				     std::vector<double> &timemax, 
				     std::vector<double> &timemin) const
  //############################################################################
  {
    std::cout<<"inside of ev_clus get_axis_range"<<std::endl;
    std::cout<<"wiremax[0] is "<<wiremax[0]<<std::endl;
    wiremax[0]=5.1;
    //    std::cout<<"just edited it to = 5.1"<<std::endl;
    //    std::cout<<"now wiremax[0] is "<<wiremax[0]<<std::endl;
    //fCluster_v is a vector of all clusters in the event
    for(auto clus : fCluster_v){
      //      if( clus.View() == 0 )
	//	clus.get_axis_range(wiremax,wiremin,timemax,timemin);
    }
    return;
  }
}  
  
#endif
  
