#ifndef CLUSTERPARAMSEXECUTOR_CC
#define CLUSTERPARAMSEXECUTOR_CC

#include "ClusterParamsExecutor.hh"

namespace cluster {


  ClusterParamsExecutor::ClusterParamsExecutor() : ClusterParamsAlgNew()
  {
    hCurrentHit = 0;
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

    cluster_hits.reserve(hit_index_v.size());
    if(hCurrentHit) delete hCurrentHit;

    double wire_min=1e9;
    double wire_max=0;
    double time_min=1e9;
    double time_max=0;
    for(auto const index : hit_index_v) {
      
      double hit_wire = hits->at(index).Wire();
      double hit_time = hits->at(index).PeakTime();
      
      wire_min = hit_wire < wire_min ? hit_wire : wire_min;
      wire_max = hit_wire > wire_max ? hit_wire : wire_max;
      time_min = hit_time < time_min ? hit_time : time_min;
      time_max = hit_time > time_max ? hit_time : time_max;

    }

    wire_min -= ((wire_max - wire_min)*0.25);
    wire_max += ((wire_max - wire_min)*0.25);
    time_min -= ((time_max - time_min)*0.25);
    time_max += ((time_max - time_min)*0.25);
    //if(wire_min<0) wire_min = 0;
    //if(time_min<0) time_min = 0;

    hCurrentHit = new TH2D("hCurrentHit",
			   Form("Event %d Cluster %d (Plane=%d); Wire [cm]; Time [cm]",hits->event_id(),i_cluster.ID(),plane),
			   100, fWire2Cm.at(plane)*wire_min, fWire2Cm.at(plane)*wire_max,
			   100, fTime2Cm*time_min, fTime2Cm*time_max);
    			   //int(wire_max - wire_min), fWire2Cm.at(plane)*wire_min, fWire2Cm.at(plane)*wire_max,
			   //int(time_max - time_min), fTime2Cm*time_min, fTime2Cm*time_max);
    hCurrentHit->SetTitleFont(22);
    hCurrentHit->GetXaxis()->SetTitleFont(22);
    hCurrentHit->GetXaxis()->SetTitleSize(0.05);
    hCurrentHit->GetXaxis()->SetLabelFont(22);
    hCurrentHit->GetXaxis()->SetLabelSize(0.05);
    hCurrentHit->GetYaxis()->SetTitleFont(22);
    hCurrentHit->GetYaxis()->SetTitleSize(0.05);
    hCurrentHit->GetYaxis()->SetLabelFont(22);
    hCurrentHit->GetYaxis()->SetLabelSize(0.05);

    for(auto const index : hit_index_v) {

      cluster_hits.push_back((const larlight::hit*)(&(hits->at(index))));

      hCurrentHit->Fill(hits->at(index).Wire()*fWire2Cm.at(plane), hits->at(index).PeakTime()*fTime2Cm, hits->at(index).Charge());
    }
    
    this->Initialize();
    this->SetHits(cluster_hits);

  }


}

#endif
