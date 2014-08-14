#ifndef CLUSTERMATCHER_CC
#define CLUSTERMATCHER_CC

#include "ClusterMatcher.hh"

namespace larlight {


  ClusterMatcher::ClusterMatcher() : _mgr(nullptr)
  { 
    _name="ClusterMatcher"; 
    _fout=0; 
    _cluster_type = DATA::FuzzyCluster; 

    auto geom = ::larutil::Geometry::GetME();
    _mgr = new ::cmtool::CMatchManager(geom->Nplanes());
    
  }

  bool ClusterMatcher::initialize() {

    if(_cluster_type != DATA::FuzzyCluster &&
       _cluster_type != DATA::CrawlerCluster &&
       _cluster_type != DATA::ShowerAngleCluster &&
       _cluster_type != DATA::Cluster)

      throw ::cluster::RecoUtilException(Form("Not supported cluster type: %s",
					      DATA::DATA_TREE_NAME[_cluster_type].c_str())
					 );

    if(_fout) _mgr->SetAnaFile(_fout);

    return true;
  }
  
  bool ClusterMatcher::analyze(storage_manager* storage) {

    std::vector<std::vector<larutil::PxHit> > local_clusters;

    _cru_helper.GeneratePxHit(storage,_cluster_type,local_clusters);

    _mgr->Reset();

    _mgr->SetClusters(local_clusters);

    _mgr->Process();

    return true;
  }

  bool ClusterMatcher::finalize() {

    return true;
  }

}
#endif
