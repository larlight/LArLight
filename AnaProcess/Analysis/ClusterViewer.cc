#ifndef CLUSTERVIEWER_CC
#define CLUSTERVIEWER_CC

#include "ClusterViewer.hh"

namespace larlight {
  //################################################################
  ClusterViewer::ClusterViewer() : ana_base(), _hRecoCluster_v_0(), _hRecoCluster_v_1(), _hRecoCluster_v_2(), _cluster_v()
  //################################################################
  {
    // Class name
    _name = "ClusterViewer";
    // Set initialization values for pointers
    _fout     = 0;
    //  _hMCStep  = 0;
    
  }
  
  //################################################################
  bool ClusterViewer::initialize()
  //################################################################
  {
    return true;
  }
  
  //################################################################
  bool ClusterViewer::analyze(storage_manager* storage)
  //################################################################
  {
    
    // Clean up histograms if they already exist (from previous event)
    //  if(_hMCStep)  {delete _hMCStep;  _hMCStep  = 0;};
    for(auto h : _hRecoCluster_v_0) {delete h; h=0;};
    for(auto h : _hRecoCluster_v_1) {delete h; h=0;};
    for(auto h : _hRecoCluster_v_2) {delete h; h=0;};
    _hRecoCluster_v_0.clear();
    _hRecoCluster_v_1.clear();
    _hRecoCluster_v_2.clear();
    _cluster_v.clear();
    
    //
    // Obtain event-wise data object pointers
    //
    //  event_mc*      ev_mc   = (event_mc*)      ( storage->get_data(DATA::MCTruth)            );
    event_cluster* ev_clus = (event_cluster*) ( storage->get_data(DATA::ShowerAngleCluster) );
    
    // Define utility variables to hold max/min of each axis range, for each of 3 views
    double wiremax[3], timemax[3], wiremin[3], timemin[3];
    for(int i = 0; i < 3; i++){
      //    wiremax[i] = -1; timemax[i] = -1; wiremin[i] = 999999.; timemin[i] = 999999.;
      wiremax[i] = 2000; timemax[i] = 2000; wiremin[i] = 0; timemin[i] = 0;
    }
    
    // Find max/min boundary for all axis (spacepoint and track)
    
    //  if(ev_clus)
    //need to implement this function for clusters
    //    ev_track->get_axis_range(xmax, xmin, ymax, ymin, zmax, zmin);
    
    //  if(ev_mc)
    
    //    ev_mc->get_axis_range(xmax, xmin, ymax, ymin, zmax, zmin);
    
    // Proceed only if minimum/maximum are set to some values other than the defaults
    if(wiremax[0] == -1) {
      
      print(MSG::WARNING,__FUNCTION__,
	    "Did not find any reconstructed clusters. Skipping this event...");
      
      return true;
    }
    
    //
    // Make & fill vertex histograms
    //
    
    // Clusters
    if(ev_clus) {
      
      std::cout<<"trying to get cluster collection"<<std::endl;
      const std::vector<cluster> clus_v = ev_clus->GetClusterCollection();
      std::cout<<"got it! time to loop over the clusters..."<<std::endl;
      
      
      int tmpcounter = 0;
      //loop over the clusters in the event
      for(auto clus : clus_v){
	tmpcounter++;
	TH2D* h = 0;
	
	h = Prepare2DHisto(Form("_hRecoCluster_%03d",tmpcounter), wiremin[0], wiremax[0], timemin[0], timemax[0]);
	
	//check from which view the cluster comes from
	iview = clus.View();
	
	//then loop through its hits, and plot wire/time for each hit in the right _hRecoCluster_v_blah histo
	ihit_v = clus.Hits();
	
	for(auto hit : ihit_v){
	  
	  if (hit.View() != iview) std::cout<<"diff views? dunno what's going on."<<std::endl;
	  
	  h->Fill(hit.Channel(),hit.PeakTime());
	  
	  //	if ((int)hit.View()==1) h_1->Fill(hit.Channel(),hit.PeakTime());
	  //	if ((int)hit.View()==2) h_2->Fill(hit.Channel(),hit.PeakTime());
	  
	}
	
	//need to make each cluster a different color      
	if((int)iview == 0)
	  _hRecoCluster_v_0.push_back(h);
	else if ((int)iview == 1)
	  _hRecoCluster_v_1.push_back(h);
	else if ((int)iview == 2)
	  _hRecoCluster_v_2.push_back(h);
	else
	  std::cout<<"iview is not 0, 1, or 2... wtf?"<<std::endl;
	
	std::cout<<"at this point in time, _hRecoCluster_v_0.size() is "
		 <<_hRecoCluster_v_0.size()<<std::endl;
	
      }//end loop over clusters in the event
      //    _hRecoSPS->SetMarkerStyle(23);
      //    _hRecoSPS->SetMarkerColor(kBlue);
    }
    
    // Tracks
    /*
      if(ev_track) {
      
      const std::vector<track> track_v = ev_track->GetTrackCollection();
      
      for(auto const& trk : track_v){
      
      TH3D* h=0;
      h=Prepare3DHisto(Form("_hRecoTrack_%03d",(int)(_hRecoTrack_v.size())),
      zmin,zmax,xmin,xmax,ymin,ymax);
      
      for(size_t i=0; i<trk.n_point(); i++) {
      
      const TVector3 vtx = trk.vertex_at(i);
      
      h->Fill(vtx[2],vtx[0],vtx[1]);
      
      }
      
      h->SetMarkerStyle(22);
      h->SetMarkerColor(kRed);
      _hRecoTrack_v.push_back(h);
      
      _track_v.push_back(track(trk));
      
      }
      
      }
    */
    /*
    // MC trajectory points
    if(ev_mc){
    
    const std::vector<part_mc> part_v = ev_mc->GetParticleCollection();
    
    for(auto const& part : part_v){
    
    // Plotting MC points from all particles ... do we have the "keepemshowerdaughers" tag on?
    //      if(part.track_id() != 1)
    
    //	continue;
    
    const std::vector<TVector3> vertex_v = part.step_vertex();
    
    _hMCStep = Prepare3DHisto("_hMCStep",
    zmin, zmax, xmin, xmax, ymin, ymax);
    
    for(auto const& vtx : vertex_v) 
    
    _hMCStep->Fill(vtx[2],vtx[0],vtx[1]);
    
    _hMCStep->SetMarkerStyle(20);
    _hMCStep->SetMarkerColor(kCyan);
    break; // Only make 1 histogram
    }
    }
    */
    return true;
  };
  
  //################################################################
  TH2D* ClusterViewer::Prepare2DHisto(std::string name, 
				      double wiremin, double wiremax,
				      double timemin, double timemax)
  //################################################################
  {
    
    TH2D* h=0;
    if(h) delete h;
    
    h = new TH2D(name.c_str(),"2D Viewer; Wire; Time;",
		 50,  wiremin, wiremax,
		 50,  timemin, timemax);
    
    return h;
  }
  
  bool ClusterViewer::finalize() {
    
    // This function is called at the end of event loop.
    // Do all variable finalization you wish to do here.
    // If you need, you can store your ROOT class instance in the output
    // file. You have an access to the output file through "_fout" pointer.
    
    return true;
  }
}
#endif
