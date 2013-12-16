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
    larlight::ClusterAnaPrep::get()->set_q_frac_cut(1.);
    std::cout<<"Setting set_q_frac_cut to 1."<<std::endl;
    return true;
  }
  
  bool KalekoClusAna::analyze(larlight::storage_manager* storage) {
  
    //
    // Do your event-by-event analysis here. This function is called for 
    // each event in the loop. You have "storage" pointer which contains 
    // event-wise data. For a reference of pmt_wf_collection class instance, 
    // see the class index in the documentation.


    larlight::ClusterAnaPrep::get()->analyze(storage);

    const std::vector<larlight::cluster_ana_info> *u_clusters;
    u_clusters = larlight::ClusterAnaPrep::get()->get_cluster_info(larlight::GEO::kU);
    std::cout<<"The number of u_clusters is : "
	     <<(int)u_clusters->size()
	     <<std::endl;
    std::cout<<"The start wire of the 0th cluster is : "
	     <<u_clusters->at(0).start_wire
	     <<" and the start time is : "
	     <<u_clusters->at(0).start_time
	     <<" while the min start time is : "
	     <<u_clusters->at(0).start_time_min
	     <<std::endl;
    std::cout<<"The total sum charge in this clsuter is : "
	     <<u_clusters->at(0).sum_charge
	     <<std::endl;

    std::cout<<"Other available variables in Analysis/ana_info_struct.hh"
	     <<std::endl;

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
}
#endif
