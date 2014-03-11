#ifndef CLUSTERMERGE_CC
#define CLUSTERMERGE_CC

#include "ClusterMerge.hh"

namespace larlight {

  bool ClusterMerge::check_algo_ptr() const {

    if(fCMergeAlg) return true;

    std::ostringstream msg;
    msg 
      << std::endl
      << "  You have not set ClusterMergeAlg pointer!" << std::endl
      << "  To run ClusterMerge module in ana_processor, you must create & attach ClusterMergeAlg" << std::endl
      << "  to the ana_processor instance, then also pass its pointer to ClusterMerge instance." << std::endl
      << "  Aborting..." << std::endl
      << std::endl;
    
    print(MSG::ERROR,__PRETTY_FUNCTION__,msg.str());

    return false;

  }

  bool ClusterMerge::initialize() {

    return check_algo_ptr();

  }
  
  bool ClusterMerge::analyze(storage_manager* storage) {

    if(!check_algo_ptr()) return false;

    //
    // Preparation
    //
    event_cluster* ev_cluster = (event_cluster*)(storage->get_data(DATA::ShowerAngleCluster));

    // cluster_sets is a vector of vector where the inner vector is a set of cluster IDs 
    // to be merged into one. I assume all cluster index to be output are in cluster_sets.
    // Say you have clusters A, B, and C, and we decide to merge A and B (call it D=A+B).
    // Then a complete set of clusters in the output means cluster_sets should have
    // one vector with A and B cluster ID, then one vector of length 1 with cluster ID of C.
    const std::vector<std::vector<unsigned int> > cluster_sets = fCMergeAlg->GetClusterSets();

    //
    // Creating output of merged clusters
    //

    // Save original cluster sets
    std::vector<cluster>       merged_cluster_v;
    merged_cluster_v.reserve(cluster_sets.size());

    // Original hit container
    event_hit* hits=0;
    DATA::DATA_TYPE  hit_type=DATA::Hit;

    // hit association
    std::vector<unsigned short> merged_hit_ass;

    // Loop over set of IDs to be merged
    for(auto const& cluster_id_set : cluster_sets) {

      // Declare output cluster
      cluster merged_cluster(ev_cluster->data_type());

      GEO::View_t view = GEO::kUnknown;

      // Loop over clusters to be merged
      for(auto const& cluster_id : cluster_id_set) {

	const cluster i_cluster = ev_cluster->at(cluster_id);

	std::vector<unsigned short> hit_index;

	// One time only operation
	if(!hits) {
	
	  if(i_cluster.size_association(DATA::Hit)) {
	    hit_index = i_cluster.association(DATA::Hit);
	    hits      = (event_hit*)(storage->get_data(DATA::Hit));
	    hit_type  = DATA::Hit;
	  }
	  else if(i_cluster.size_association(DATA::FFTHit)) {
	    hit_index = i_cluster.association(DATA::FFTHit);
	    hits      = (event_hit*)(storage->get_data(DATA::FFTHit));
	    hit_type  = DATA::FFTHit;
	  }
	  else if(i_cluster.size_association(DATA::CrawlerHit)) {
	    hit_index = i_cluster.association(DATA::CrawlerHit);
	    hits      = (event_hit*)(storage->get_data(DATA::CrawlerHit));
	    hit_type  = DATA::CrawlerHit;
	  }
	  else {
	    print(MSG::ERROR,__FUNCTION__,"Did not find any associated (and supported) hits!");
	    return false;
	  }

	  if(!hits) {
	    print(MSG::ERROR,__FUNCTION__,"Data storage did not find associated hit collection!");
	    return false;
	  }
	}

	for(auto const& index : hit_index) 

	  merged_hit_ass.push_back(index);
	
	if(view == GEO::kUnknown) view = ev_cluster->at(cluster_id).View();

      } // End of looping over original clusters to be merged

      merged_cluster.add_association(hit_type,merged_hit_ass);

      merged_cluster.set_charge(-1);
      merged_cluster.set_dtdw(-1);
      merged_cluster.set_dtdw_err(-1);
      merged_cluster.set_dqdw(-1);
      merged_cluster.set_dqdw_err(-1);
      merged_cluster.set_id(merged_cluster_v.size());
      merged_cluster.set_view(view);

      std::vector<double> whatever(2,-1);

      merged_cluster.set_start_vtx(whatever);
      merged_cluster.set_start_vtx_err(whatever);
      merged_cluster.set_end_vtx(whatever);
      merged_cluster.set_end_vtx_err(whatever);

      // Append to merged cluster vector
      merged_cluster_v.push_back(merged_cluster);

    }// End of looping over all cluster sets to be merged

    // Clear the data holder & overwrite with new cluster set

    ev_cluster->clear();

    for(auto const& merged_cluster : merged_cluster_v)

      ev_cluster->push_back(merged_cluster);
    
    return true;
  }

  bool ClusterMerge::finalize() {

    return true;
  }
}
#endif
