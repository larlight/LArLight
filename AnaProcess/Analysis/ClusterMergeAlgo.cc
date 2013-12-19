#ifndef CLUSTERMERGEALGO_CC
#define CLUSTERMERGEALGO_CC

#include "ClusterMergeAlgo.hh"

namespace larlight {

  ClusterMergeAlgo* ClusterMergeAlgo::_me=0;

  bool ClusterMergeAlgo::initialize() {

    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you need, you have an output root file pointer "_fout".
    ClusterAnaPrep::get()->initialize();

    max_allowed_2D_angle_diff = 10.;
    max_2D_dist2 = 999999.;
    return true;
  }
  
  //analyze function builds the _cluster_sets_v vector, used by ClusterMerge.cc
  bool ClusterMergeAlgo::analyze(storage_manager* storage) {
  
    //Run ClusterAnaPrep on this event, which does a loop over the hits, etc.
    //Note, ClusterAnaPrep should automatically NOT run if it has already been done by previous ana module
    //First, initialize it's q_frac_cut parameter (should I do this in the initialize() function?)
    ClusterAnaPrep::get()->set_q_frac_cut(1.);
    //    std::cout<<"Setting set_q_frac_cut to 1."<<std::endl;

    //Next, reset the _cluster_sets_v vector<vector<int>> which is used by ClusterMerge
    _cluster_sets_v.clear();

    //Then, run ClusterAnaPrep over the event.
    ClusterAnaPrep::get()->analyze(storage);

    //the following clear()'s not needed?
    //u_clusters->clear(); v_clusters->clear(); w_clusters->clear();
    u_clusters = ClusterAnaPrep::get()->get_cluster_info(GEO::kU);
    v_clusters = ClusterAnaPrep::get()->get_cluster_info(GEO::kV);    
    w_clusters = ClusterAnaPrep::get()->get_cluster_info(GEO::kW);
    
    //Loop over all possible combinations of clusters in u-view
    for(int iclus = 0; iclus < (int)u_clusters->size(); ++iclus){
      for(int jclus = iclus+1; jclus < (int)u_clusters->size(); ++jclus){
	BuildClusterSets(u_clusters->at(iclus),u_clusters->at(jclus));
      }//end loop over jclus u_clusters
    }//end loop over iclus u_clusters

    //Loop over all possible combinations of clusters in v-view
    for(int iclus = 0; iclus < (int)v_clusters->size(); ++iclus){
      for(int jclus = iclus+1; jclus < (int)v_clusters->size(); ++jclus){
	BuildClusterSets(v_clusters->at(iclus),v_clusters->at(jclus));
      }//end loop over jclus v_clusters
    }//end loop over iclus v_clusters

    //Loop over all possible combinations of clusters in w-view
    for(int iclus = 0; iclus < (int)w_clusters->size(); ++iclus){
      for(int jclus = iclus+1; jclus < (int)w_clusters->size(); ++jclus){
	BuildClusterSets(w_clusters->at(iclus),w_clusters->at(jclus));
      }//end loop over jclus w_clusters
    }//end loop over iclus w_clusters
    
    //make sure all of the un-mergable clusters are in _cluster_sets_v, individually
    FinalizeClusterSets(u_clusters,v_clusters,w_clusters);

    return true;
  }

  bool ClusterMergeAlgo::finalize() {
    ClusterAnaPrep::get()->finalize();  
    // This function is called at the end of event loop.
    // Do all variable finalization you wish to do here.
    // If you need, you can store your ROOT class instance in the output
    // file. You have an access to the output file through "_fout" pointer.
  
    return true;
  }
  
  bool ClusterMergeAlgo::CompareClusters(cluster_ana_info clusA,
					 cluster_ana_info clusB){
    
    /*
      std::cout<<"Cluster "<<iclus<<" has info:"<<std::endl;
      PrintClusterVars(u_clusters->at(iclus));
      std::cout<<"While cluster "<<jclus<<" has info:"<<std::endl;
      PrintClusterVars(u_clusters->at(jclus));
    */

    //    if( Angle2DCompatibility(clusA.angle,clusB.angle,max_allowed_2D_angle_diff) )
    //	  std::cout<<"The two clusters are ANGLE compatible!"<<std::endl;
    if( ShortestDistanceCompatibility(clusA.start_time, clusA.start_wire,
    				      clusA.end_time, clusA.end_wire,
    				      clusB.start_time, clusB.start_wire,
    				      clusB.end_time, clusB.end_wire,
    				      max_2D_dist2) )
      //  std::cout<<"The two clusters are STARTENDPOINT compatible!"<<std::endl;
      return true;
    else
      return false;
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
						       double max_2D_dist2){

    //This code finds the shortest distance between a point and a line segment.
    //First, pretend the first cluster is a 2D line segment, from its start point to end point
    //Find the shortest distance between start point of the second cluster to this line segment.
    //Repeat for end point of second cluster to this line segment.
    //Then, pretend second cluster is a 2D line segment, from its start point to end point.
    //Find the shortest distance between start point of the first cluster to this line segment.
    //Repeat for end point of first cluster to this line segment.
    //If the shortest of these four distances is less than the cutoff, 
    //return true... the clusters are merge-compatible. else, return false.
    
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
    t = ( (w_start2-w_start1)*(w_end1-w_start1) + (t_start2-t_start1)*(t_end1-t_start1) ) / l2;
    if(t<0.0) shortest_distance2_tmp = std::pow(w_start2-w_start1,2)+std::pow(t_start2-t_start1,2);
    else if (t>1.0) shortest_distance2_tmp = std::pow(w_start2-w_end1,2)+std::pow(t_start2-t_end1,2);
    else shortest_distance2_tmp = std::pow(w_start2-(w_start1+t*(w_end1-w_start1)),2)+
	   std::pow(t_start2-(t_start1+t*(t_end1-t_start1)),2);
    if(shortest_distance2_tmp < shortest_distance2) shortest_distance2 = shortest_distance2_tmp;
    //    std::cout<<"start of 2nd to segment of 1st: dist2 is "<<shortest_distance2_tmp<<std::endl;

    //Find shortest distance between point ("P")=(w_end2,t_end2) to this line segment
    t = ( (w_end2-w_start1)*(w_end1-w_start1) + (t_end2-t_start1)*(t_end1-t_start1) ) / l2;
    if(t<0.0) shortest_distance2_tmp = std::pow(w_end2-w_start1,2)+std::pow(t_end2-t_start1,2);
    else if (t>1.0) shortest_distance2_tmp = std::pow(w_end2-w_end1,2)+std::pow(t_end2-t_end1,2);
    else shortest_distance2_tmp = std::pow(w_end2-(w_start1+t*(w_end1-w_start1)),2)+
	   std::pow(t_end2-(t_start1+t*(t_end1-t_start1)),2);
    if(shortest_distance2_tmp < shortest_distance2) shortest_distance2 = shortest_distance2_tmp;
    //    std::cout<<"end of 2nd to segment of 1st: dist2 is "<<shortest_distance2_tmp<<std::endl;
    
    //Line segment: from ("V")=(w_start2,t_start2) to ("W")=(w_end2,t_end2)
    l2 = std::pow(w_start2-w_start1,2)+std::pow(t_start2-t_start1,2);
    
    //Find shortest distance between point ("P")=(w_start1,t_start1) and this line segment
    t = ( (w_start1-w_start2)*(w_end2-w_start2) + (t_start1-t_start2)*(t_end2-t_start2) ) / l2;
    if(t<0.0) shortest_distance2_tmp = std::pow(w_start2-w_start1,2)+std::pow(t_start2-t_start1,2);
    else if (t>1.0) shortest_distance2_tmp = std::pow(w_end2-w_start1,2)+std::pow(t_end2-t_start1,2);
    else shortest_distance2_tmp = std::pow(w_start1-(w_start2+t*(w_end2-w_start2)),2)+
	   std::pow(t_start1-(t_start2+t*(t_end2-t_start2)),2);
    if(shortest_distance2_tmp < shortest_distance2) shortest_distance2 = shortest_distance2_tmp;
    //    std::cout<<"start of 1st to segment of 2nd: dist2 is "<<shortest_distance2_tmp<<std::endl;

    //Find shortest distance between point ("P")=(w_end1,t_end1) and this line segment
    t = ( (w_end1-w_start2)*(w_end2-w_start2) + (t_end1-t_start2)*(t_end2-t_start2) ) / l2;
    if(t<0.0) shortest_distance2_tmp = std::pow(w_start2-w_end1,2)+std::pow(t_start2-t_end1,2);
    else if (t>1.0) shortest_distance2_tmp = std::pow(w_end2-w_end1,2)+std::pow(t_end2-t_end1,2);
    else shortest_distance2_tmp = std::pow(w_end1-(w_start2+t*(w_end2-w_start2)),2)+
	   std::pow(t_end1-(t_start2+t*(t_end2-t_start2)),2);
    if(shortest_distance2_tmp < shortest_distance2) shortest_distance2 = shortest_distance2_tmp;
    //    std::cout<<"end of 1st to segment of 2nd: dist2 is "<<shortest_distance2_tmp<<std::endl;

    //    std::cout<<"After all of those comparisons, shortest_distance2 is "<<shortest_distance2<<std::endl;

    if(shortest_distance2 < max_2D_dist2)
      return true;
    else
      return false;

  }//end startend2dcompatibility
  
  
  
  void ClusterMergeAlgo::PrintClusterVars(cluster_ana_info clus_info){
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


  void ClusterMergeAlgo::BuildClusterSets(cluster_ana_info clusA, cluster_ana_info clusB){
    //this function builds the _cluster_sets_v vector, according to the format defined
    //in ClusterAlgo. here's an example of what _cluster_sets_v format is:
    //imagine an event with clusters 0, 1, 2, 3, 4, 5
    //pretend 0, 1 are found compatible for merging.
    //pretend 1, 2 are found compatible for merging.
    //pretend 4, 5 are found compatible for merging.
    //_cluster_sets_v is a vector<vector<int>>, and it should look like
    //_cluster_sets_v = ( (0, 1, 2), (4, 5), (3) )

    //if the two clusters are compatible for merging
    if( CompareClusters(clusA,clusB) ){
     
      //if this is the first compatibility found for this entire event
      if((int)_cluster_sets_v.size() == 0){
	//push both IDs in to the _cluster_sets_v vector, grouped together as one element
	std::vector<unsigned int> myvec;
	myvec.push_back(clusA.cluster_index);
	myvec.push_back(clusB.cluster_index);
	_cluster_sets_v.push_back(myvec);
      }
      
      //else if only A index already somewhere in the _cluster_sets_v vector, and B index is not
      else if(isInClusterSets(clusA.cluster_index) != -1 &&
	      isInClusterSets(clusB.cluster_index) == -1){
	//push the B index in to that element vector of _cluster_sets_v
	_cluster_sets_v.at(isInClusterSets(clusA.cluster_index)).push_back(clusB.cluster_index);
      }

      //else if only B index already somewhere in the _cluster_sets_v vector, and A index is not
      else if(isInClusterSets(clusB.cluster_index) != -1 &&
	      isInClusterSets(clusA.cluster_index) == -1){
	//push the B index in to that element vector of _cluster_sets_v
	_cluster_sets_v.at(isInClusterSets(clusB.cluster_index)).push_back(clusA.cluster_index);
      }

      //else if neither of the two are already in the _cluster_sets_v vector, anywhere
      else if(isInClusterSets(clusB.cluster_index) == -1 &&
	      isInClusterSets(clusA.cluster_index) == -1){
	//push both IDs into the _cluster_sets_v vector, grouped together, as a new element
	std::vector<unsigned int> tmp;
	tmp.push_back((unsigned int)clusA.cluster_index);
	tmp.push_back((unsigned int)clusB.cluster_index);
	_cluster_sets_v.push_back(tmp);
      }
       
      //else if both indexes are already in the _cluster_sets_v vector
      else if(isInClusterSets(clusB.cluster_index) != -1 &&
	      isInClusterSets(clusA.cluster_index) != -1){
	//if they're in the same element already, do nothing.
	//if they're in separate elements... something went wrong. 
	//they should always be in the same element, by construction 
	//(assuming the above if, else ifs, etc are done correctly)
	if(isInClusterSets(clusA.cluster_index) != isInClusterSets(clusB.cluster_index))
	  std::cout<<"SOMETHING WENT HORRIBLY WRONG!!!"<<std::endl;
      }

      //at the end of all possible cluster matching permutations, if some are not in 
      //_cluster_sets_v yet, push them back individually as individual elements...
      //this is what FinalizeClusterSets function is for

    }//end if the two clusters are compatible for merging
    
  }//end BuildClusterSets
  

  void ClusterMergeAlgo::FinalizeClusterSets(const std::vector<cluster_ana_info> *u_clusters,
					     const std::vector<cluster_ana_info> *v_clusters,
					     const std::vector<cluster_ana_info> *w_clusters){

    //loop over all cluster ID's in the event... guess I have to do it view-by-view
    for(int iclus = 0; iclus < (int)u_clusters->size(); ++iclus){    
	//if the cluster_id is not in _cluster_sets_v at all, push it back individually
      if(isInClusterSets((int)(u_clusters->at(iclus).cluster_index)) == -1){
	std::vector<unsigned int> tmpvec;
	tmpvec.push_back( (unsigned int)(u_clusters->at(iclus).cluster_index ));
	_cluster_sets_v.push_back(tmpvec);
      }
    }

    for(int iclus = 0; iclus < (int)v_clusters->size(); ++iclus){    
	//if the cluster_id is not in _cluster_sets_v at all, push it back individually
      if(isInClusterSets(v_clusters->at(iclus).cluster_index) == -1){
	std::vector<unsigned int> tmpvec;
	tmpvec.push_back( (int)v_clusters->at(iclus).cluster_index );
	_cluster_sets_v.push_back(tmpvec);
      }
    }

    for(int iclus = 0; iclus < (int)w_clusters->size(); ++iclus){    
	//if the cluster_id is not in _cluster_sets_v at all, push it back individually
      if(isInClusterSets(w_clusters->at(iclus).cluster_index) == -1){
	std::vector<unsigned int> tmpvec;
	tmpvec.push_back( (int)w_clusters->at(iclus).cluster_index );
	_cluster_sets_v.push_back(tmpvec);
      }
    }

    
  } // end FinalizeClusterSets
  
  
  
  
  
  int ClusterMergeAlgo::isInClusterSets(int index){
    //function to check if an index is in _cluster_sets_v anywhere,
    //and return where it is (-1 if it's not in _cluster_sets_v)
    
    int where_is_it = -1;

    for(size_t i=0; i<_cluster_sets_v.size() && where_is_it<0; ++i) {

      for(auto const& cluster_id : _cluster_sets_v.at(i)) {

	if((int)cluster_id == index) {
	  where_is_it = i;
	  break;
	}
      }

    }
    
    return where_is_it;
    
  }//end isInClusterSets function
  
  

}

#endif
