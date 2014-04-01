#ifndef CLUSTERPARAMSEXECUTOR_CC
#define CLUSTERPARAMSEXECUTOR_CC

#include "ClusterParamsExecutor.hh"

namespace cluster {


  ClusterParamsExecutor::ClusterParamsExecutor() : ClusterParamsAlgNew()
  {
    hCurrentHit = 0;

    _useHitBlurring = false;

    _blurFunction = 0;

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
    if(_blurFunction) delete _blurFunction;

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

    //for blurring in the y (time) direction:
    _blurFunction = new TF1("_blurFunction","[0]*exp(-0.5*((x-[1])/[2])**2)",0.,99999.);

    for(auto const index : hit_index_v) {

      cluster_hits.push_back((const larlight::hit*)(&(hits->at(index))));

      if(!_useHitBlurring)
	hCurrentHit->Fill(hits->at(index).Wire()*fWire2Cm.at(plane), 
			  hits->at(index).PeakTime()*fTime2Cm, 
			  hits->at(index).Charge());


      else{
	//center of peak = hit.PeakTime
	_blurFunction->SetParameter(1,hits->at(index).PeakTime()*fTime2Cm);

	//width of peak (estimated) = avg time b/t peak&start, and peak&end
	double blurfnsigma = 
	  ( (hits->at(index).PeakTime() - hits->at(index).StartTime() )
	   +(hits->at(index).EndTime()  - hits->at(index).PeakTime()) ) / 2;
	blurfnsigma *= fTime2Cm;
	_blurFunction->SetParameter(2,blurfnsigma);
	
	//height of peak = changes to normalize gaussian area to 1 (2.5066 is sqrt(2pi))
	_blurFunction->SetParameter(0, (1/(blurfnsigma*2.5066)) );

	//fill the histogram in y outward from the peak until 97% of charge is added
	Int_t peak_bin = hCurrentHit->GetYaxis()->FindBin(hits->at(index).PeakTime()*fTime2Cm);
	
	double peak_bin_low  = hCurrentHit->GetYaxis()->GetBinLowEdge(peak_bin);
	double peak_bin_high = hCurrentHit->GetYaxis()->GetBinUpEdge(peak_bin);
	double bin_width = hCurrentHit->GetYaxis()->GetBinWidth(peak_bin);

	double charge_filled = 0.; 
	unsigned int counter = 0;
	double charge_in_bin, bin_low, bin_high;
	
	while( (charge_filled)/(hits->at(index).Charge()) < 0.97
	       && counter < hCurrentHit->GetNbinsY() ){

	  //if you're dealing with the one peak bin
	  if(!counter){
	    charge_in_bin = (_blurFunction->Integral(peak_bin_low,peak_bin_high)) * hits->at(index).Charge();
	    
	    hCurrentHit->Fill(hits->at(index).Wire()*fWire2Cm.at(plane), 
			      hits->at(index).PeakTime()*fTime2Cm, 
			      charge_in_bin);
	    
	    charge_filled += charge_in_bin;
	  }

	  //if not dealing with the one peak bin, fill next bin above/below
	  else{

	    Int_t current_bin_above_peak = 
	      hCurrentHit->GetYaxis()->FindBin(hits->at(index).PeakTime()*fTime2Cm + (counter*bin_width));

	    bin_low = hCurrentHit->GetYaxis()->GetBinLowEdge(current_bin_above_peak);
	    bin_high = hCurrentHit->GetYaxis()->GetBinUpEdge(current_bin_above_peak);
	    
	    charge_in_bin = (_blurFunction->Integral(bin_low,bin_high)) * hits->at(index).Charge();

	    if(charge_in_bin > 1e-4)
	      hCurrentHit->Fill(hits->at(index).Wire()*fWire2Cm.at(plane),
				hCurrentHit->GetYaxis()->GetBinCenter(current_bin_above_peak),
				charge_in_bin);
	    charge_filled += charge_in_bin;

	    Int_t current_bin_below_peak = 
	      hCurrentHit->GetYaxis()->FindBin(hits->at(index).PeakTime()*fTime2Cm - (counter*bin_width));
	    bin_low = hCurrentHit->GetYaxis()->GetBinLowEdge(current_bin_below_peak);
	    bin_high = hCurrentHit->GetYaxis()->GetBinUpEdge(current_bin_below_peak);

	    charge_in_bin = (_blurFunction->Integral(bin_low,bin_high)) * hits->at(index).Charge();

	    if(charge_in_bin > 1e-4)
	      hCurrentHit->Fill(hits->at(index).Wire()*fWire2Cm.at(plane),
				hCurrentHit->GetYaxis()->GetBinCenter(current_bin_below_peak),
				charge_in_bin);
	    charge_filled += charge_in_bin;

	  }
	  counter++;

	}
      }
    }
    
    this->Initialize();
    this->SetHits(cluster_hits);

  }


}

#endif
