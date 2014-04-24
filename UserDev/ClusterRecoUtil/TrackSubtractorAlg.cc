

#include "TrackSubtractorAlg.hh"

namespace clusteralg
{

  void TrackSubtractorAlg::Initialize(){
    larutil::LArUtilManager::Reconfigure(larlight::GEO::kArgoNeuT);
    fGSer = NULL;
    if(!fGSer){
      std::cout << "Initializing fGSer!\n";
      fGSer = (larutil::GeometryUtilities*)(larutil::GeometryUtilities::GetME());
    }
    all_hitsHIST   = 0;
    track_hitsHIST = 0;
    fCPAN.Initialize();
    std::cout << "Done initializing!" << std::endl;
  }

  int TrackSubtractorAlg::AddCluster(const UChar_t plane_id,
                                     const larlight::cluster &i_cluster,
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
    if( larutil::Geometry::GetME()->ChannelToPlane(
      hits->at(hit_index_v.front()).Channel()) != plane_id )
      return 0;

    cluster_vec.push_back(i_cluster);
    cluster_hits.push_back(std::vector < const larlight:: hit*>() );
    cluster_hits.back().reserve(hits->size());

    for(auto const index : hit_index_v)

      cluster_hits.back().push_back( (const larlight::hit *)(&(hits->at(index))));

    if (isTrackLike(i_cluster, cluster_hits.back())){
      for (auto &hit : cluster_hits.back())
        track_hits.push_back(hit);
    }


    return 0;

  }

  int TrackSubtractorAlg::LoadAllHits(const larlight::event_hit *hits, const UChar_t plane_id)
  {
   
    remaining_hits.clear();
    cluster_hits.clear();
    cluster_vec.clear();

    all_hits.clear();
    all_hits.reserve(hits->size());

    track_hits.clear();
    track_hits.reserve(hits->size());

    for(auto &h : *hits) {

      if( larutil::Geometry::GetME()->ChannelToPlane(h.Channel()) == plane_id )
        all_hits.push_back((const larlight::hit*)(&h));

    }

    return 0;

    // return Execute(hits->event_id(), -1, plane_id);
  }

  bool TrackSubtractorAlg::isTrackLike(const larlight::cluster &i_cluster,
                   std::vector<const larlight::hit* > & hits){
    
    if (hits.size() < 30 )
      return false;

    // double charge_mean = 0;
    // double charge_rms  = 0;
    // double n = hits.size();

    // for (auto &hit : hits){
    //   charge_mean += hit->Charge();
    // }

    // charge_mean /= n;

    // for (auto &hit : hits){
    //   charge_rms += (hit->Charge() - charge_mean)
    //                *(hit->Charge() - charge_mean);
    // }
    // charge_rms /= (n-1);
    // charge_rms = sqrt(charge_rms);

    // if (charge_rms / charge_mean > 0.5 ) return false;

    // return true;

    fCPAN.SetHits(hits);
    fCPAN.setNeuralNetPath("~/LArLight/FANN/trained_nets/cascade_argo_clusters.net");
    fCPAN.EnableFANN();
    fCPAN.FillParams();

    std::cout << "This cluster has trackness " << fCPAN.fParams.trackness 
              << " and showerness " << fCPAN.fParams.showerness << std::endl;

    if (fCPAN.fParams.trackness == 1) return true;
    else return false;

  }

  int TrackSubtractorAlg::ResolveHits(){
    return 0;
  }

  void TrackSubtractorAlg::FillHists(){



    double wire_min=1e9;
    double wire_max=0;
    double time_min=1e9;
    double time_max=0;
    for(auto const h : all_hits) {
      
      double hit_wire = h->Wire();
      double hit_time = h->PeakTime();
      
      wire_min = hit_wire < wire_min ? hit_wire : wire_min;
      wire_max = hit_wire > wire_max ? hit_wire : wire_max;
      time_min = hit_time < time_min ? hit_time : time_min;
      time_max = hit_time > time_max ? hit_time : time_max;

    }

    wire_min -= ((wire_max - wire_min)*0.25);
    wire_max += ((wire_max - wire_min)*0.25);
    time_min -= ((time_max - time_min)*0.25);
    time_max += ((time_max - time_min)*0.25);

    std::cout << "Wire min, max: " << wire_min << ", " << wire_max << std::endl;
    std::cout << "Time min, max: " << time_min << ", " << time_max << std::endl;

    std::cout << "Wire to Cm: " << fGSer->WireToCm() << std::endl;
    std::cout << "Time to Cm: " << fGSer->TimeToCm() << std::endl;


    all_hitsHIST = new TH2D("allhitsHIST",
         "All Hits in event",
         100, (fGSer->WireToCm())*wire_min, (fGSer->WireToCm())*wire_max,
         100, (fGSer->TimeToCm())*time_min, (fGSer->TimeToCm())*time_max);

    all_hitsHIST->SetTitleFont(22);
    all_hitsHIST->GetXaxis()->SetTitleFont(22);
    all_hitsHIST->GetXaxis()->SetTitleSize(0.05);
    all_hitsHIST->GetXaxis()->SetLabelFont(22);
    all_hitsHIST->GetXaxis()->SetLabelSize(0.05);
    all_hitsHIST->GetYaxis()->SetTitleFont(22);
    all_hitsHIST->GetYaxis()->SetTitleSize(0.05);
    all_hitsHIST->GetYaxis()->SetLabelFont(22);
    all_hitsHIST->GetYaxis()->SetLabelSize(0.05);

    track_hitsHIST = new TH2D("trackhitsHIST",
         "Track Hits in event",
         100, (fGSer->WireToCm())*wire_min, (fGSer->WireToCm())*wire_max,
         100, (fGSer->TimeToCm())*time_min, (fGSer->TimeToCm())*time_max);

    track_hitsHIST->SetTitleFont(22);
    track_hitsHIST->GetXaxis()->SetTitleFont(22);
    track_hitsHIST->GetXaxis()->SetTitleSize(0.05);
    track_hitsHIST->GetXaxis()->SetLabelFont(22);
    track_hitsHIST->GetXaxis()->SetLabelSize(0.05);
    track_hitsHIST->GetYaxis()->SetTitleFont(22);
    track_hitsHIST->GetYaxis()->SetTitleSize(0.05);
    track_hitsHIST->GetYaxis()->SetLabelFont(22);
    track_hitsHIST->GetYaxis()->SetLabelSize(0.05);

    for(auto h : all_hits) {
      all_hitsHIST->Fill(h->Wire()*(fGSer->WireToCm()), 
                        h->PeakTime()*(fGSer->TimeToCm()), 
                        h->Charge());
    }

    for(auto h : track_hits) {
      track_hitsHIST->Fill(h->Wire()*(fGSer->WireToCm()), 
                        h->PeakTime()*(fGSer->TimeToCm()), 
                        h->Charge());
    }


    return;

  }


}