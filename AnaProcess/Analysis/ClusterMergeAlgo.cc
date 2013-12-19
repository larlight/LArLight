#ifndef CLUSTERMERGEALGO_CC
#define CLUSTERMERGEALGO_CC

#include "ClusterMergeAlgo.hh"

namespace larlight {

  bool ClusterMergeAlgo::initialize() {

    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you need, you have an output root file pointer "_fout".
    larlight::ClusterAnaPrep::get()->initialize();

    max_allowed_2D_angle_diff = 10.;
    max_allowed_2D_startendpt_diff = 100.;
    return true;
  }
  
  bool ClusterMergeAlgo::analyze(larlight::storage_manager* storage) {
  
    //Run ClusterAnaPrep on this event, which does a loop over the hits, etc.
    //First, initialize it's q_frac_cut parameter
    larlight::ClusterAnaPrep::get()->set_q_frac_cut(1.);
    std::cout<<"Setting set_q_frac_cut to 1."<<std::endl;
    //Then, run it over the event.
    larlight::ClusterAnaPrep::get()->analyze(storage);

    //Looking at the cluster_ana_info outputs
    //u_clusters->clear(); v_clusters->clear(); w_clusters->clear();
    u_clusters = larlight::ClusterAnaPrep::get()->get_cluster_info(larlight::GEO::kU);
    v_clusters = larlight::ClusterAnaPrep::get()->get_cluster_info(larlight::GEO::kV);    
    w_clusters = larlight::ClusterAnaPrep::get()->get_cluster_info(larlight::GEO::kW);
    
    std::cout<<Form("The number of u_clusters, v_clusters, w_clusters is : (%d,%d,%d)",
		    (int)u_clusters->size(),(int)v_clusters->size(),(int)w_clusters->size())
	     <<std::endl;
    
    
    //Loop over all possible combinations of clusters in u-view
    for(int iclus = 0; iclus < (int)u_clusters->size(); ++iclus){
      for(int jclus = iclus+1; jclus < (int)u_clusters->size(); ++jclus){
	std::cout<<"Comparing U-cluster "<<iclus<<" and "<<jclus<<" ..."<<std::endl;	
	CompareClusters(u_clusters->at(iclus),u_clusters->at(jclus));
      }//end loop over jclus u_clusters
    }//end loop over iclus u_clusters

    //Loop over all possible combinations of clusters in v-view
    for(int iclus = 0; iclus < (int)v_clusters->size(); ++iclus){
      for(int jclus = iclus+1; jclus < (int)v_clusters->size(); ++jclus){
	std::cout<<"Comparing V-cluster "<<iclus<<" and "<<jclus<<" ..."<<std::endl;	
	CompareClusters(v_clusters->at(iclus),v_clusters->at(jclus));
      }//end loop over jclus v_clusters
    }//end loop over iclus v_clusters

    //Loop over all possible combinations of clusters in w-view
    for(int iclus = 0; iclus < (int)w_clusters->size(); ++iclus){
      for(int jclus = iclus+1; jclus < (int)w_clusters->size(); ++jclus){
	std::cout<<"Comparing W-cluster "<<iclus<<" and "<<jclus<<" ..."<<std::endl;	
	CompareClusters(w_clusters->at(iclus),w_clusters->at(jclus));
      }//end loop over jclus w_clusters
    }//end loop over iclus w_clusters
    
    return true;
  }

  bool ClusterMergeAlgo::finalize() {
    larlight::ClusterAnaPrep::get()->finalize();  
    // This function is called at the end of event loop.
    // Do all variable finalization you wish to do here.
    // If you need, you can store your ROOT class instance in the output
    // file. You have an access to the output file through "_fout" pointer.
  
    return true;
  }
  
  bool ClusterMergeAlgo::CompareClusters(larlight::cluster_ana_info clusA,
				      larlight::cluster_ana_info clusB){
    
    /*
      std::cout<<"Cluster "<<iclus<<" has info:"<<std::endl;
      PrintClusterVars(u_clusters->at(iclus));
      std::cout<<"While cluster "<<jclus<<" has info:"<<std::endl;
      PrintClusterVars(u_clusters->at(jclus));
    */
    
    if( Angle2DCompatibility(clusA.angle,clusB.angle,max_allowed_2D_angle_diff) )
	  std::cout<<"The two clusters are ANGLE compatible!"<<std::endl;
	
    if( ShortestDistanceCompatibility(clusA.start_time, clusA.start_wire,
				      clusA.end_time, clusA.end_wire,
				      clusB.start_time, clusB.start_wire,
				      clusB.end_time, clusB.end_wire,
				      max_allowed_2D_startendpt_diff) )
      std::cout<<"The two clusters are STARTENDPOINT compatible!"<<std::endl;
    
    return true;
  }


  bool ClusterMergeAlgo::Angle2DCompatibility(double angle1, double angle2, double max_allowed_2D_angle_diff){
    
    if(std::abs(angle1-angle2)     < max_allowed_2D_angle_diff ||
       std::abs(angle1-angle2-180) < max_allowed_2D_angle_diff ||
       std::abs(angle1-angle2+180) < max_allowed_2D_angle_diff )
      return true;
    else
      return false;

  }//end Angle2DCompatibility

  bool ClusterMergeAlgo::ShortestDistanceCompatibility(double t_start1, double w_start1, double t_end1, double w_end1,
						       double t_start2, double w_start2, double t_end2, double w_end2,
						       double max_allowed_2D_startendpt_diff){
    //old version of code, delete once newer version is working
    /*
    //if end of 1 matches start of 2
    double tmp_dist1 = std::sqrt(std::pow(t_end1-t_start2,2)+std::pow(w_end1-w_start2,2));
    //if end of 1 matches end of 2
    double tmp_dist2 = std::sqrt(std::pow(t_end1-t_end2,2)+std::pow(w_end1-w_end2,2));
    //if start of 1 matches end of 2
    double tmp_dist3 = std::sqrt(std::pow(t_start1-t_end2,2)+std::pow(w_start1-w_end2,2));
    //if start of 1 matches start of 2
    double tmp_dist4 = std::sqrt(std::pow(t_start1-t_start2,2)+std::pow(w_start1-w_start2,2));

    if(tmp_dist1 < max_allowed_2D_startendpt_diff)
      std::cout<<"The end of the first cluster matched the start of the 2nd. Dist = "
	       <<tmp_dist1
	       <<std::endl;
    if(tmp_dist2 < max_allowed_2D_startendpt_diff)
      std::cout<<"The end of the first cluster matched the end of the 2nd. Dist = "
	       <<tmp_dist2
	       <<std::endl;
    if(tmp_dist3 < max_allowed_2D_startendpt_diff)
      std::cout<<"The start of the first cluster matched the end of the 2nd. Dist = "
	       <<tmp_dist3
	       <<std::endl;
    if(tmp_dist4 < max_allowed_2D_startendpt_diff)
      std::cout<<"The start of the first cluster matched the start of the 2nd. Dist = "
	       <<tmp_dist4
	       <<std::endl;

    if(tmp_dist1 < max_allowed_2D_startendpt_diff || 
       tmp_dist2 < max_allowed_2D_startendpt_diff || 
       tmp_dist3 < max_allowed_2D_startendpt_diff || 
       tmp_dist4 < max_allowed_2D_startendpt_diff )
      return true;
    else
      return false;
    */
    
    //code based off sample from 
    //http://stackoverflow.com/questions/849211/shortest-distance-between-a-point-and-a-line-segment
    //note to self: rewrite this with TVector2 and compare time differences... 
    //TVector2 code might be more understandable
    
    double shortest_distance2 = 9999999.;
    double shortest_distance2_tmp = 0.;
    double l2 = -1.;
    double t = -1.;
    

    //Line segment: from ("V")=(w_start1,t_start1) to ("W")=(w_end1,t_end1)
    l2 = std::pow(w_end1-w_start1,2)+std::pow(t_end1-t_start1,2);
    
    //Find shortest distance between point ("P")=(w_start2,t_start2) to this line segment
    std::cout<<"Comparing start of second cluster to line segment of first..."<<std::endl;
    t = ( (w_start2-w_start1)*(w_end1-w_start1) + (t_start2-t_start1)*(t_end1-t_start1) ) / l2;
    if(t<0.0) shortest_distance2_tmp = std::pow(w_start2-w_start1,2)+std::pow(t_start2-t_start1,2);
    else if (t>1.0) shortest_distance2_tmp = std::pow(w_start2-w_end1,2)+std::pow(t_start2-t_end1,2);
    else shortest_distance2_tmp = std::pow(w_start2-(w_start1+t*(w_end1-w_start1)),2)+
	   std::pow(t_start2-(t_start1+t*(t_end1-t_start1)),2);
    if(shortest_distance2_tmp < shortest_distance2) shortest_distance2 = shortest_distance2_tmp;
    std::cout<<"shortest_distance2_tmp is "<<shortest_distance2_tmp<<std::endl;

    //Find shortest distance between point ("P")=(w_end2,t_end2) to this line segment
    std::cout<<"Comparing end of second cluster to line segment of first..."<<std::endl;
    t = ( (w_end2-w_start1)*(w_end1-w_start1) + (t_end2-t_start1)*(t_end1-t_start1) ) / l2;
    if(t<0.0) shortest_distance2_tmp = std::pow(w_end2-w_start1,2)+std::pow(t_end2-t_start1,2);
    else if (t>1.0) shortest_distance2_tmp = std::pow(w_end2-w_end1,2)+std::pow(t_end2-t_end1,2);
    else shortest_distance2_tmp = std::pow(w_end2-(w_start1+t*(w_end1-w_start1)),2)+
	   std::pow(t_end2-(t_start1+t*(t_end1-t_start1)),2);
    if(shortest_distance2_tmp < shortest_distance2) shortest_distance2 = shortest_distance2_tmp;
    std::cout<<"shortest_distance2_tmp is "<<shortest_distance2_tmp<<std::endl;
    
    //Line segment: from ("V")=(w_start2,t_start2) to ("W")=(w_end2,t_end2)
    l2 = std::pow(w_start2-w_start1,2)+std::pow(t_start2-t_start1,2);
    
    //Find shortest distance between point ("P")=(w_start1,t_start1) and this line segment
    std::cout<<"Comparing start of first cluster to line segment of second..."<<std::endl;
    t = ( (w_start1-w_start2)*(w_end2-w_start2) + (t_start1-t_start2)*(t_end2-t_start2) ) / l2;
    if(t<0.0) shortest_distance2_tmp = std::pow(w_start2-w_start1,2)+std::pow(t_start2-t_start1,2);
    else if (t>1.0) shortest_distance2_tmp = std::pow(w_end2-w_start1,2)+std::pow(t_end2-t_start1,2);
    else shortest_distance2_tmp = std::pow(w_start1-(w_start2+t*(w_end2-w_start2)),2)+
	   std::pow(t_start1-(t_start2+t*(t_end2-t_start2)),2);
    if(shortest_distance2_tmp < shortest_distance2) shortest_distance2 = shortest_distance2_tmp;
    std::cout<<"shortest_distance2_tmp is "<<shortest_distance2_tmp<<std::endl;

    //Find shortest distance between point ("P")=(w_end1,t_end1) and this line segment
    std::cout<<"Comparing end of first cluster to line segment of second..."<<std::endl;
    t = ( (w_end1-w_start2)*(w_end2-w_start2) + (t_end1-t_start2)*(t_end2-t_start2) ) / l2;
    if(t<0.0) shortest_distance2_tmp = std::pow(w_start2-w_end1,2)+std::pow(t_start2-t_end1,2);
    else if (t>1.0) shortest_distance2_tmp = std::pow(w_end2-w_end1,2)+std::pow(t_end2-t_end1,2);
    else shortest_distance2_tmp = std::pow(w_end1-(w_start2+t*(w_end2-w_start2)),2)+
	   std::pow(t_end1-(t_start2+t*(t_end2-t_start2)),2);
    if(shortest_distance2_tmp < shortest_distance2) shortest_distance2 = shortest_distance2_tmp;
    std::cout<<"shortest_distance2_tmp is "<<shortest_distance2_tmp<<std::endl;

    std::cout<<"After all of those comparisons, shortest_distance2 is "<<shortest_distance2<<std::endl;
    
    return true;
  }//end startend2dcompatibility
  
  
  
  void ClusterMergeAlgo::PrintClusterVars(larlight::cluster_ana_info clus_info){
    std::cout<<std::endl;
    std::cout<<"***********CLUSTER INFO PARAMETERS***********"<<std::endl;
    /*
      std::cout<<"cluster_index = "
      <<clus_info.cluster_index
	     <<std::endl;
    */
    std::cout<<"start_wire = "
	     <<clus_info.start_wire
	     <<std::endl;
    std::cout<<"start_time = "
	     <<clus_info.start_time
	     <<std::endl;
    std::cout<<"end wire = "
	     <<clus_info.end_wire
	     <<std::endl;
    std::cout<<"end_time = "
	     <<clus_info.end_time
	     <<std::endl;
    /*
    std::cout<<"angle = "
	     <<clus_info.angle
	     <<std::endl;
    	  std::cout<<"start_time_max = "
	     <<clus_info.start_time_max
	     <<std::endl;
    std::cout<<"start_time_min = "
	     <<clus_info.start_time_min
	     <<std::endl;
    std::cout<<"peak_time_max = "
	     <<clus_info.peak_time_max
	     <<std::endl;
    std::cout<<"peak_time_min = "
	     <<clus_info.peak_time_min
	     <<std::endl;
    std::cout<<"end_time_max = "
	     <<clus_info.end_time_max
	     <<std::endl;
    std::cout<<"end_time_min = "
	     <<clus_info.end_time_min
	     <<std::endl;
    std::cout<<"sum_charge = "
	     <<clus_info.sum_charge
	     <<std::endl;
    */
    std::cout<<std::endl;
  
  }//end PrintClusterVars function


}

#endif
