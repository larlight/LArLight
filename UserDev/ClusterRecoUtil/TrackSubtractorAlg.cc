

#include "TrackSubtractorAlg.hh"

namespace cluster{



  int TrackSubtractorAlg::AddCluster(const larlight::cluster &i_cluster,
                                     const larlight::event_hit *hits){

    larlight::DATA::DATA_TYPE hit_type = i_cluster.get_hit_type();
    if(!hits) {
      throw cluster::RecoUtilException("Invalid hit (didn't find data!)");
      return -1;
    }else if(hit_type != hits->data_type()){
      throw cluster::RecoUtilException("Unmatched hit type!");
      return -1;
    }

    const std::vector<UShort_t> &hit_index_v = i_cluster.association(hit_type);


    cluster_vec.push_back(i_cluster);
    cluster_hits.push_back(std::vector < const larlight:: hit*>() );
    cluster_hits.back().reserve(hits->size());

    for(auto const index : hit_index_v)

      cluster_hits.back().push_back( (const larlight::hit *)(&(hits->at(index))));

    return 0;

  }


  // int TrackSubtractorAlg::LoadCluster(const larlight::cluster &i_cluster,
  //           const larlight::event_hit *hits)
  // {
    
  //   larlight::DATA::DATA_TYPE hit_type = i_cluster.get_hit_type();
  //   if(!hits) {
  //     throw cluster::RecoUtilException("Invalid hit (didn't find data!)");
  //     return -1;
  //   }else if(hit_type != hits->data_type()){
  //     throw cluster::RecoUtilException("Unmatched hit type!");
  //     return -1;
  //   }

  //   cluster_hits.clear();
  //   const std::vector<UShort_t> &hit_index_v = i_cluster.association(hit_type);

  //   UChar_t plane = larutil::Geometry::GetME()->ChannelToPlane(hits->at(*hit_index_v.begin()).Channel());

  //   cluster_hits.reserve(hit_index_v.size());

  //   for(auto const index : hit_index_v)

  //     cluster_hits.push_back((const larlight::hit*)(&(hits->at(index))));

    
  //  return Execute(hits->event_id(), i_cluster.ID(), plane);
  // }

  int TrackSubtractorAlg::LoadAllHits(const larlight::event_hit *hits, const UChar_t plane_id)
  {
    
    all_hits.clear();
    all_hits.reserve(hits->size());

    for(auto &h : *hits) {

      if( larutil::Geometry::GetME()->ChannelToPlane(h.Channel()) == plane_id )
        all_hits.push_back((const larlight::hit*)(&h));

    }

    return 0;

    // return Execute(hits->event_id(), -1, plane_id);
  }


}