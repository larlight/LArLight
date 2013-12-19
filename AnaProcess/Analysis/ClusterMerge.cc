#ifndef CLUSTERMERGE_CC
#define CLUSTERMERGE_CC

#include "ClusterMerge.hh"

namespace larlight {

  bool ClusterMerge::initialize() {

    ClusterAnaPrep::get()->initialize();

    return true;
  }
  
  bool ClusterMerge::analyze(storage_manager* storage) {
    
    //
    // Preparation
    //
    
    ClusterAnaPrep::get()->analyze(storage);
    //    const std::vector<cluster_ana_info>* uclusters = ClusterAnaPrep::get()->get_cluster_info(GEO::kU);
    //    const std::vector<cluster_ana_info>* vclusters = ClusterAnaPrep::get()->get_cluster_info(GEO::kV);
    //    const std::vector<cluster_ana_info>* wclusters = ClusterAnaPrep::get()->get_cluster_info(GEO::kW);

    // cluster_sets is a vector of vector where the inner vector is a set of cluster IDs 
    // to be merged into one. I assume all cluster index to be output are in cluster_sets.
    // Say you have clusters A, B, and C, and we decide to merge A and B (call it D=A+B).
    // Then a complete set of clusters in the output means cluster_sets should have
    // one vector with A and B cluster ID, then one vector of length 1 with cluster ID of C.
    const std::vector<std::vector<unsigned int> > cluster_sets = ClusterMergeAlgo::get()->GetClusterSets();

    //
    // Merging operation
    //
    
    
    /*
    // In this example, I set cluster_sets such that all clusters to be merged...
    size_t nclusters = uclusters->size() + vclusters->size() + wclusters->size();
    std::vector<unsigned int> cluster_id;
    for(size_t i=0; i<nclusters; i++)

      cluster_id.push_back(i);
    
    cluster_sets.push_back(cluster_id);
    */


    //
    // Creating output of merged clusters
    //

    // Save original cluster sets
    event_cluster* ev_cluster = (event_cluster*)(storage->get_data(DATA::ShowerAngleCluster));

    const std::vector<cluster> original_cluster_v = ev_cluster->GetClusterCollection();
    std::vector<cluster>       merged_cluster_v;
    merged_cluster_v.reserve(cluster_sets.size());

    // Loop over set of IDs to be merged
    for(auto const& cluster_id_set : cluster_sets) {

      // Declare output cluster
      cluster merged_cluster;

      GEO::View_t view = GEO::kUnknown;

      // Loop over clusters to be merged
      for(auto const& cluster_id : cluster_id_set) {

	const std::vector<hit> original_hit_v = original_cluster_v[cluster_id].Hits();
	
	for(auto const& original_hit : original_hit_v)

	  merged_cluster.add_hit(original_hit);
	
	if(view == GEO::kUnknown) view = original_cluster_v[cluster_id].View();

      } // End of looping over original clusters to be merged

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

    ev_cluster->clear_clusters();

    for(auto const& merged_cluster : merged_cluster_v)

      ev_cluster->add_cluster(merged_cluster);
    
    return true;
  }

  bool ClusterMerge::finalize() {

    ClusterAnaPrep::get()->finalize();
  
    return true;
  }
}
#endif
