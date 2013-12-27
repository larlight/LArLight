#ifndef CLUSTERANAPREP_CC
#define CLUSTERANAPREP_CC

#include "ClusterAnaPrep.hh"

namespace larlight {

  ClusterAnaPrep* ClusterAnaPrep::_me = 0;

  //***********************************************
  ClusterAnaPrep::ClusterAnaPrep() : ana_base() 
  //***********************************************
  { 
    _name="ClusterAnaPrep"; 
    _fout=0;
    clear_event_info();
    
    _hMultU = _hMultV = _hMultW = 0;
    _h2DAngleU = _h2DAngleV = _h2DAngleW = 0;
    _hStartPosU = _hStartPosV = _hStartPosW = 0;
    _hEndPosU = _hEndPosV = _hEndPosW = 0;

    _q_frac_cut = 0.8;

    _sigma_cut  = 100;

    _initialized=false;
    _finalized=false;
  }

  //*************************************
  void ClusterAnaPrep::clear_event_info()
  //*************************************
  { 
    _uclusters.clear(); 
    _vclusters.clear(); 
    _wclusters.clear(); 

    _run    = -1;
    _subrun = -1;
    _event  = -1;
  }

  //********************************************************************************************
  const std::vector<larlight::cluster_ana_info>* ClusterAnaPrep::get_cluster_info(GEO::View_t v) const
  //********************************************************************************************
  {
    if(v==GEO::kU) return &_uclusters;

    if(v==GEO::kV) return &_vclusters;

    if(v==GEO::kW) return &_wclusters;

    else

      print(MSG::ERROR,__FUNCTION__,Form("Invalid View_t: %d",v));
	
    return 0;
  }

  //***********************************************
  bool ClusterAnaPrep::initialize() 
  //***********************************************
  {
    if(_initialized) return true;

    _skipped_cluster_cnt = 0;
    
    clear_event_info();

    if(!_hMultU) _hMultU = new TH1D("hMultU","",50,-0.5,49.5);
    else _hMultU->Reset();

    if(!_hMultV) _hMultV = new TH1D("hMultV","",50,-0.5,49.5);
    else _hMultV->Reset();

    if(!_hMultW) _hMultW = new TH1D("hMultW","",50,-0.5,49.5);
    else _hMultW->Reset();


    if(!_hStartPosU) _hStartPosU = new TH2D("hStartPosU","",100,0,1,100,0,1);
    else _hStartPosU->Reset();

    if(!_hStartPosV) _hStartPosV = new TH2D("hStartPosV","",100,0,1,100,0,1);
    else _hStartPosV->Reset();

    if(!_hStartPosW) _hStartPosW = new TH2D("hStartPosW","",100,0,1,100,0,1);
    else _hStartPosW->Reset();


    if(!_hEndPosU) _hEndPosU = new TH2D("hEndPosU","",100,0,1,100,0,1);
    else _hEndPosU->Reset();

    if(!_hEndPosV) _hEndPosV = new TH2D("hEndPosV","",100,0,1,100,0,1);
    else _hEndPosV->Reset();

    if(!_hEndPosW) _hEndPosW = new TH2D("hEndPosW","",100,0,1,100,0,1);
    else _hEndPosW->Reset();



    if(!_h2DAngleU) _h2DAngleU  = new TH1D("h2DAngleU","",100,0,1);
    else _h2DAngleU->Reset();

    if(!_h2DAngleV) _h2DAngleV  = new TH1D("h2DAngleV","",100,0,1);
    else _h2DAngleV->Reset();

    if(!_h2DAngleW) _h2DAngleW  = new TH1D("h2DAngleW","",100,0,1);
    else _h2DAngleW->Reset();

    _initialized = true;
    _finalized   = false;
    return true;
  }

  //******************************************************  
  bool ClusterAnaPrep::analyze(storage_manager* storage)
  //******************************************************  
  {
    if(!_initialized){
      
      print(MSG::ERROR,__PRETTY_FUNCTION__,"Called analyze before initialize!");
      return false;

    }

    const event_cluster* ev_cluster = (const event_cluster*)(storage->get_data(DATA::ShowerAngleCluster));
    const std::vector<cluster> cluster_collection = ev_cluster->GetClusterCollection();

    if(same_event((data_base*)(ev_cluster))) return true;
    clear_event_info();
    _run    = ev_cluster->run();
    _subrun = ev_cluster->subrun();
    _event  = ev_cluster->event_id();

    // Initialize event-wise counter
    unsigned short skipped_event_cluster_cnt = 0;

    // Loop over clusters
    for(auto const i_cluster: cluster_collection) {

      // Check if this cluster should be evaluated or not
      if(i_cluster.SigmaStartPos()[0] > _sigma_cut) {

	print(MSG::INFO,__FUNCTION__,
	      Form("Skipping cluster %d with SigmaStartPos=%g > %g", 
		   i_cluster.ID(),
		   i_cluster.SigmaStartPos()[0],
		   _sigma_cut));
	
	skipped_event_cluster_cnt++;
	
	continue;

      }

      // Load cluster parameters
      const GEO::View_t view = i_cluster.View();
      double start_wire = i_cluster.StartPos()[0];
      double start_time = i_cluster.StartPos()[1];
      double end_wire   = i_cluster.EndPos()[0];
      double end_time   = i_cluster.EndPos()[1];
      double xangle     = i_cluster.dTdW();

      // Get Hit array & loop over. Note we include a subset of hits
      // ordered from high charge to low charge up to the point that
      // hits' sum charge exceeds set fraction.
      const std::vector<hit> hits = i_cluster.Hits();
      std::map<double,size_t> hits_qsort;
      double sum_charge = 0;
      for(size_t i=0; i<hits.size(); i++) {
	
	hits_qsort[1./(hits[i].Charge())] = i;

	sum_charge += hits[i].Charge();

      }

      // Loop over sorted hits
      cluster_ana_info ci;
      ci.view = view;
      ci.cluster_index = i_cluster.ID();
      ci.start_wire = start_wire;
      ci.start_time = start_time;
      ci.end_wire   = end_wire;
      ci.end_time   = end_time;
      ci.angle      = xangle;
      for( std::map<double,size_t>::const_iterator iter(hits_qsort.begin());
	   iter!=hits_qsort.end();
	   ++iter ) {

	size_t hit_index = (*iter).second;
	
	if( ci.start_time_min < 0 || ci.start_time_min > hits[hit_index].StartTime() ) ci.start_time_min = hits[hit_index].StartTime();
	if( ci.peak_time_min  < 0 || ci.peak_time_min  > hits[hit_index].PeakTime()  ) ci.peak_time_min  = hits[hit_index].PeakTime();
	if( ci.end_time_min   < 0 || ci.end_time_min   > hits[hit_index].EndTime()   ) ci.end_time_min   = hits[hit_index].EndTime();

	if( ci.start_time_max < 0 || ci.start_time_max < hits[hit_index].StartTime() ) ci.start_time_max = hits[hit_index].StartTime();
	if( ci.peak_time_max  < 0 || ci.peak_time_max  < hits[hit_index].PeakTime()  ) ci.peak_time_max  = hits[hit_index].PeakTime();
	if( ci.end_time_max   < 0 || ci.end_time_max   < hits[hit_index].EndTime()   ) ci.end_time_max   = hits[hit_index].EndTime();

	ci.sum_charge += hits[hit_index].Charge();
	
	// Break if summed charge reach the set fraction of the total charge
	if( (ci.sum_charge / sum_charge) > _q_frac_cut ) break;

      }

      // Evaluate/Process 2D cluster parameters
      switch(view){
      case GEO::kU:
	_hStartPosU->Fill(start_wire,start_time);
	_hEndPosU->Fill(end_wire,end_time);
	_h2DAngleU->Fill(xangle);
	_uclusters.push_back(ci);
	break;
      case GEO::kV:
	_hStartPosV->Fill(start_wire,start_time);
	_hEndPosV->Fill(end_wire,end_time);
	_h2DAngleV->Fill(xangle);
	_vclusters.push_back(ci);
	break;
      case GEO::kW:
	_hStartPosW->Fill(start_wire,start_time);
	_hEndPosW->Fill(end_wire,end_time);
	_h2DAngleW->Fill(xangle);
	_wclusters.push_back(ci);
	break;
      case GEO::k3D:
      case GEO::kUnknown:
	print(MSG::ERROR,__FUNCTION__,Form("Unexpected View_t: %d",view));
	break;
      }
    }

    if(_uclusters.size()) _hMultU->Fill(_uclusters.size());
    if(_vclusters.size()) _hMultV->Fill(_vclusters.size());
    if(_wclusters.size()) _hMultW->Fill(_wclusters.size());

    if(skipped_event_cluster_cnt) {
      print(MSG::NORMAL,__FUNCTION__,
	    Form("Skipped %d / %zu clusters in event %d",
		 skipped_event_cluster_cnt,
		 cluster_collection.size(),
		 _event));
      _skipped_cluster_cnt+=skipped_event_cluster_cnt;
    }
    return true;
  }

  //******************************************************  
  bool ClusterAnaPrep::finalize()
  //******************************************************  
  {
    if(_finalized) return true;

    if(_fout) {
      _hMultU->Write();
      _hMultV->Write();
      _hMultW->Write();
      _h2DAngleU->Write();
      _h2DAngleV->Write();
      _h2DAngleW->Write();
      _hStartPosU->Write();
      _hStartPosV->Write();
      _hStartPosW->Write();
      _hEndPosU->Write();
      _hEndPosV->Write();
      _hEndPosW->Write();
    }
    _finalized=true;
    _initialized=false;
    return true;
  }
}
#endif
