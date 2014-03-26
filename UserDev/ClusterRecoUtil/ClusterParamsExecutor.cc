#ifndef CLUSTERPARAMSEXECUTOR_CC
#define CLUSTERPARAMSEXECUTOR_CC

#include "ClusterParamsExecutor.hh"

namespace cluster {


  ClusterParamsExecutor::ClusterParamsExecutor() : ClusterParamsAlgNew()
  {
    hCurrentHit = 0;
    hHit.reserve(larutil::Geometry::GetME()->Nplanes());
    
    for(size_t i=0; i<larutil::Geometry::GetME()->Nplanes(); ++i) {
      
      hHit.push_back(new TH2D(Form("hClusterHit_%zu",i),
			      Form("Cluster Hits (Plane=%zu);Wire;Time",i),
			      400,-0.5,(larutil::Geometry::GetME()->Nwires(i)-0.5)*fWire2Cm.at(i),
			      300,0,larutil::DetectorProperties::GetME()->ReadOutWindowSize()*fTime2Cm));

    }

  }

  void ClusterParamsExecutor::LoadCluster(const larlight::cluster &i_cluster,
					  const larlight::event_hit *hits)
  {
    
    larlight::DATA::DATA_TYPE hit_type = i_cluster.get_hit_type();
    if(!hits) {
      throw cluster::RecoUtilException("Invalid hit (didn't find data!)");
      return;
    }else if(hit_type != hits->data_type()){
      throw cluster::RecoUtilException("Unmatched hit type!");
      return;
    }

    cluster_hits.clear();
    const std::vector<UShort_t> &hit_index_v = i_cluster.association(hit_type);

    UChar_t plane = larutil::Geometry::GetME()->ChannelToPlane(hits->at(*hit_index_v.begin()).Channel());
    hHit.at(plane)->Reset();
    hCurrentHit = hHit.at(plane);

    cluster_hits.reserve(hit_index_v.size());
    for(auto const index : hit_index_v) {

      cluster_hits.push_back((const larlight::hit*)(&(hits->at(index))));
      
      hHit.at(plane)->Fill(hits->at(index).Wire()*fWire2Cm.at(plane), hits->at(index).PeakTime()*fTime2Cm, hits->at(index).Charge());
      
    }
    
    this->Initialize();
    this->SetHits(cluster_hits);

  }


}

#endif
