#ifndef CLUSTERMERGER_CC
#define CLUSTERMERGER_CC

#include "ClusterMerger.hh"

namespace larlight {

  bool ClusterMerger::initialize() {

    if(_cluster_type != DATA::FuzzyCluster &&
       _cluster_type != DATA::CrawlerCluster &&
       _cluster_type != DATA::ShowerAngleCluster)

      throw ::cluster::RecoUtilException(Form("Not supported cluster type: %s",
					      DATA::DATA_TREE_NAME[_cluster_type].c_str())
					 );

    if(_fout) _mgr.SetAnaFile(_fout);

    return true;
  }
  
  bool ClusterMerger::analyze(storage_manager* storage) {

    const event_cluster* ev_cluster = (event_cluster*)(storage->get_data(_cluster_type));

    if(!ev_cluster) 
      throw ::cluster::RecoUtilException(Form("Data type %s not found!",
					      DATA::DATA_TREE_NAME[_cluster_type].c_str())
					 );

    DATA::DATA_TYPE hit_type = ev_cluster->get_hit_type();
    
    const event_hit* ev_hit = (event_hit*)(storage->get_data(hit_type));
    if(!ev_hit)
      throw ::cluster::RecoUtilException(Form("Data type %s not found!",
					      DATA::DATA_TREE_NAME[hit_type].c_str())
					 );

    std::vector<std::vector<larutil::PxHit> > local_clusters(ev_cluster->size(),
							     std::vector<larutil::PxHit>()
							     );

    for(size_t i=0; i<ev_cluster->size(); ++i) {

      std::vector<unsigned short> hit_indexes(ev_cluster->at(i).association(hit_type));

      local_clusters.at(i).reserve(hit_indexes.size());

      for(auto index : hit_indexes) {
	larutil::PxHit h;
	h.w = ev_hit->at(index).Wire() * larutil::GeometryUtilities::GetME()->WireToCm();
	h.t = ev_hit->at(index).PeakTime() * larutil::GeometryUtilities::GetME()->TimeToCm();
	h.plane = larutil::Geometry::GetME()->ChannelToPlane(ev_hit->at(index).Channel());
	h.charge = ev_hit->at(index).Charge();
	local_clusters.at(i).push_back(h);
      }

    }
    std::cout<<Form("Processing event %d...",ev_cluster->event_id())<<std::endl;
    _mgr.SetClusters(local_clusters);

    _mgr.Process();
  
    return true;
  }

  bool ClusterMerger::finalize() {

    return true;
  }

}
#endif
