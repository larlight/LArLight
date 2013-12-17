#ifndef KALEKOCLUSANA_CC
#define KALEKOCLUSANA_CC

#include "KalekoClusAna.hh"

namespace kaleko {

  bool KalekoClusAna::initialize() {

    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you need, you have an output root file pointer "_fout".
    larlight::ClusterAnaPrep::get()->initialize();

    max_allowed_2D_angle_diff = 10.;
    max_allowed_2D_startendpt_diff = 100.;
    return true;
  }
  
  bool KalekoClusAna::analyze(larlight::storage_manager* storage) {
  
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
    

    for(int iclus = 0; iclus < (int)u_clusters->size(); ++iclus){
      //      PrintClusterVars( u_clusters->at(iclus) );

      for(int jclus = 0; jclus < (int)u_clusters->size(); ++jclus){
	if(iclus != jclus){
	  /*
	  std::cout<<"Comparing U-cluster "<<iclus<<" and "<<jclus<<" ..."<<std::endl;
	  std::cout<<"Cluster "<<iclus<<" has info:"<<std::endl;
	  PrintClusterVars(u_clusters->at(iclus));
	  std::cout<<"While cluster "<<jclus<<" has info:"<<std::endl;
	  PrintClusterVars(u_clusters->at(jclus));
	  */

	  if(Angle2DCompatibility(u_clusters->at(iclus).angle,
				  u_clusters->at(jclus).angle,
				  max_allowed_2D_angle_diff) )
	    
	    std::cout<<"U_Clusters i:"<<iclus<<" and j:"<<jclus<<" are 2D angle-compatible!"
		     <<std::endl;
	  
	  if(StartEnd2DCompatibility(u_clusters->at(iclus).start_time,
				     u_clusters->at(iclus).start_wire,
				     u_clusters->at(iclus).end_time,
				     u_clusters->at(iclus).end_wire,
				     u_clusters->at(jclus).start_time,
				     u_clusters->at(jclus).start_wire,
				     u_clusters->at(jclus).end_time,
				     u_clusters->at(jclus).end_wire,
				     max_allowed_2D_startendpt_diff) )
	    
	    std::cout<<"U_Clusters i:"<<iclus<<" and j:"<<jclus<<" are StartEnd 2D Compatible!"
	 	     <<std::endl;
	  
	}//end if iclus != jclus
      }//end loop over jclus u_clusters
    }//end loop over iclus u_clusters

    return true;
  }

  bool KalekoClusAna::finalize() {
    larlight::ClusterAnaPrep::get()->finalize();  
    // This function is called at the end of event loop.
    // Do all variable finalization you wish to do here.
    // If you need, you can store your ROOT class instance in the output
    // file. You have an access to the output file through "_fout" pointer.
  
    return true;
  }

  bool KalekoClusAna::Angle2DCompatibility(double angle1, double angle2, double max_allowed_2D_angle_diff){
    if(std::abs(angle1-angle2)     < max_allowed_2D_angle_diff ||
       std::abs(angle1-angle2-180) < max_allowed_2D_angle_diff ||
       std::abs(angle1-angle2+180) < max_allowed_2D_angle_diff )
      return true;
    else
      return false;

  }//end Angle2DCompatibility

  bool KalekoClusAna::StartEnd2DCompatibility(double t_start1, double w_start1, double t_end1, double w_end1,
					      double t_start2, double w_start2, double t_end2, double w_end2,
					      double max_allowed_2D_startendpt_diff){
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
 
  }//end startend2dcompatibility
  


  void KalekoClusAna::PrintClusterVars(larlight::cluster_ana_info clus_info){
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
