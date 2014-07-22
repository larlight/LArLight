#ifndef CLUSTERMERGER_CC
#define CLUSTERMERGER_CC

#include "ClusterMerger.hh"

namespace larlight {

  bool ClusterMerger::initialize() {

    if(_cluster_type != DATA::FuzzyCluster &&
       _cluster_type != DATA::CrawlerCluster &&
       _cluster_type != DATA::ShowerAngleCluster &&
       _cluster_type != DATA::Cluster)

      throw ::cluster::RecoUtilException(Form("Not supported cluster type: %s",
					      DATA::DATA_TREE_NAME[_cluster_type].c_str())
					 );

    if(_fout) _mgr.SetAnaFile(_fout);

    return true;
  }
  
  bool ClusterMerger::analyze(storage_manager* storage) {

    std::vector<std::vector<larutil::PxHit> > local_clusters;

    _cru_helper.GeneratePxHit(storage,_cluster_type,local_clusters);

    _mgr.Reset();

    _mgr.SetClusters(local_clusters);

    _mgr.Process();

    if(!_write_output) return true;

    // Proceed to write an output data product
    
    auto ev_cluster = (const event_cluster*)(storage->get_data(_cluster_type));

    const DATA::DATA_TYPE hit_type = ev_cluster->get_hit_type();

    std::vector<std::vector<unsigned short> > merged_indexes;
    _mgr.GetBookKeeper().PassResult(merged_indexes);
    
    //auto out_cluster_v =  (event_cluster*)(storage->get_data(DATA::Cluster));

    ::larlight::event_cluster out_cluster_v;
    out_cluster_v.clear();
    out_cluster_v.reserve(merged_indexes.size());
    out_cluster_v.set_event_id(ev_cluster->event_id());
    out_cluster_v.set_run(ev_cluster->run());
    out_cluster_v.set_subrun(ev_cluster->subrun());
    //tmp_index to know what plane to use
    unsigned short tmp_index = 0;

    for(auto const& indexes : merged_indexes) {

      std::vector<unsigned short> merged_association;

      for(auto const& cluster_index : indexes) {

	tmp_index = cluster_index;

	merged_association.reserve(merged_association.size() + 
				   ev_cluster->at(cluster_index).association(hit_type).size());

	for(auto const& hit_index : ev_cluster->at(cluster_index).association(hit_type)) 

	  merged_association.push_back(hit_index);
	
      }

      cluster out_cluster;
      out_cluster.set_id(out_cluster_v.size());
      out_cluster.set_view(ev_cluster->at(tmp_index).View());
      out_cluster.add_association(hit_type,merged_association);
      out_cluster_v.push_back(out_cluster);
    }
    
    (*((event_cluster*)(storage->get_data(DATA::Cluster)))) = out_cluster_v;

    
    return true;
  }

  bool ClusterMerger::finalize() {

    return true;
  }

}
#endif
