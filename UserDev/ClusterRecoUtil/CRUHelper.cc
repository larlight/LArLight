#ifndef CRUHELPER_CC
#define CRUHELPER_CC

#include "CRUHelper.hh"

namespace cluster {

  /// Generate: from 1 set of hits => 1 CPAN using indexes (association)
  void CRUHelper::GenerateCPAN(const std::vector<unsigned short>& hit_index,
			       const ::larlight::event_hit* hits,
			       ClusterParamsAlgNew &cpan) const
  {
    std::vector<larutil::PxHit> pxhits;
    GeneratePxHit(hit_index,hits,pxhits);
    cpan.SetHits(pxhits);
  }
  
  /// Generate: CPAN vector from event storage by specifying cluster type
  void CRUHelper::GenerateCPAN(::larlight::storage_manager* storage,
			       const ::larlight::DATA::DATA_TYPE cluster_type,
			       std::vector<cluster::ClusterParamsAlgNew> &cpan_v) const
  {
    std::vector<std::vector<larutil::PxHit> > pxhits_v;
    GeneratePxHit(storage,cluster_type,pxhits_v);
    cpan_v.clear();
    cpan_v.resize(pxhits_v.size(),ClusterParamsAlgNew());

    for(size_t i=0; i<pxhits_v.size(); ++i)

      cpan_v.at(i).SetHits(pxhits_v.at(i));

  }
  
  /// Generate: from 1 set of hits => 1 set of PxHits using indexes (association)
  void CRUHelper::GeneratePxHit(const std::vector<unsigned short>& hit_index,
				const ::larlight::event_hit* hits,
				std::vector<larutil::PxHit> &pxhits) const
  {

    if(!(hit_index.size())) throw RecoUtilException(Form("Hit list empty! (%s)",__FUNCTION__));
    
    pxhits.clear();
    pxhits.reserve(hit_index.size());

    auto geo  = ::larutil::Geometry::GetME();
    auto geou = ::larutil::GeometryUtilities::GetME();

    UChar_t plane = geo->ChannelToPlane(hits->at(hit_index.at(0)).Channel());

    for(auto const& index : hit_index) {
      
      auto const& hit = hits->at(index);

      ::larutil::PxHit h;

      h.t = hit.PeakTime() * geou->TimeToCm();
      h.w = hit.Wire()     * geou->WireToCm();

      h.charge = hit.Charge();
      h.plane  = plane;

      pxhits.push_back(h);
    }      

  }
  
  /// Generate: vector of PxHit sets from event storage by specifying cluster type
  void CRUHelper::GeneratePxHit(::larlight::storage_manager* storage,
				const ::larlight::DATA::DATA_TYPE cluster_type,
				std::vector<std::vector<larutil::PxHit> > &pxhits_v) const
  {

    pxhits_v.clear();

    switch(cluster_type) {
      
    case ::larlight::DATA::Cluster:
    case ::larlight::DATA::ShowerAngleCluster:
    case ::larlight::DATA::DBCluster:
    case ::larlight::DATA::FuzzyCluster:
    case ::larlight::DATA::CrawlerCluster:
      break;
    default:
      throw RecoUtilException(Form("Not cluster data type: %s",
				   ::larlight::DATA::DATA_TREE_NAME[cluster_type].c_str()
				   )
			      );
    }

    auto ev_cluster = (const ::larlight::event_cluster*)(storage->get_data(cluster_type));

    if(!ev_cluster)

      throw RecoUtilException(Form("Cluster type not in storage: %s",
				   ::larlight::DATA::DATA_TREE_NAME[cluster_type].c_str()
				   )
			      );

    const ::larlight::DATA::DATA_TYPE hit_type = ev_cluster->get_hit_type();
    
    auto ev_hits = (const ::larlight::event_hit*)(storage->get_data(hit_type));

    if(!ev_hits) 

      throw RecoUtilException(Form("Associated hit type not in storage: %s",
				   ::larlight::DATA::DATA_TREE_NAME[hit_type].c_str()
				   )
			      );

    pxhits_v.reserve(ev_cluster->size());

    for(auto const& c : *ev_cluster) {
      
      auto &hit_index = c.association(hit_type);

      std::vector<larutil::PxHit> pxhits;

      GeneratePxHit(hit_index, ev_hits, pxhits);

      pxhits_v.push_back(pxhits);

    }
  }

}

#endif
