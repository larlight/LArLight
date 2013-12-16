#ifndef CLUSTERVIEWER_CC
#define CLUSTERVIEWER_CC

#include "ClusterViewer.hh"

namespace larlight {
  //################################################################
  ClusterViewer::ClusterViewer() : ana_base(), _hRecoCluster_v_0(), _hRecoCluster_v_1(), _hRecoCluster_v_2(), _cluster_v(), _hHits_0(), _hHits_1(), _hHits_2(), _hClusterGraph_v_0(), _hClusterGraph_v_1(), _hClusterGraph_v_2()
  //################################################################
  {
    // Class name
    _name = "ClusterViewer";
    // Set initialization values for pointers
    _fout     = 0;
    _hHits_0 = 0; 
    _hHits_1 = 0;
    _hHits_2 = 0;
    //  _hMCStep  = 0;
    
  }
  
  //################################################################
  bool ClusterViewer::initialize()
  //################################################################
  {
    g = new TGraph();
    return true;
  }
  
  //################################################################
  bool ClusterViewer::analyze(storage_manager* storage)
  //################################################################
  {
    
    // Clean up histograms if they already exist (from previous event)
    if(_hHits_0)  {delete _hHits_0;  _hHits_0  = 0;}; 
    if(_hHits_1)  {delete _hHits_1;  _hHits_1  = 0;}; 
    if(_hHits_2)  {delete _hHits_2;  _hHits_2  = 0;}; 
    for(auto h : _hRecoCluster_v_0) {delete h; h=0;};
    for(auto h : _hRecoCluster_v_1) {delete h; h=0;};
    for(auto h : _hRecoCluster_v_2) {delete h; h=0;};
    for(auto h : _hClusterGraph_v_0) {delete h; h=0;};
    for(auto h : _hClusterGraph_v_1) {delete h; h=0;};
    for(auto h : _hClusterGraph_v_2) {delete h; h=0;};
    _hRecoCluster_v_0.clear();
    _hRecoCluster_v_1.clear();
    _hRecoCluster_v_2.clear();
    _hClusterGraph_v_0.clear();
    _hClusterGraph_v_1.clear();
    _hClusterGraph_v_2.clear();
    _cluster_v.clear();
    
    //
    // Obtain event-wise data object pointers
    //
    //  event_mc*      ev_mc   = (event_mc*)      ( storage->get_data(DATA::MCTruth)            );
    event_cluster* ev_clus = (event_cluster*) ( storage->get_data(DATA::ShowerAngleCluster) );
    
    // Define utility variables to hold max/min of each axis range, for each of 3 views
    //vector of length 3 initialized to -1
    std::vector<double> chmax, chmin, wiremax, wiremin, timemax, timemin;
    
    // Find max/min boundary for all axis (clusters)
    
    if(ev_clus)
      ev_clus->get_axis_range(chmax, chmin, wiremax, wiremin, timemax, timemin);
    
    // Proceed only if minimum/maximum are set to some values other than the defaults
    if(wiremax[0] == -1) {
      
      print(MSG::WARNING,__FUNCTION__,
	    "Did not find any reconstructed clusters in view 0. Skipping this event...");
      
      return true;
    }
    
    //
    // Make & fill vertex histograms
    //
    
    // Clusters
    if(ev_clus) {
      
      const std::vector<cluster> clus_v = ev_clus->GetClusterCollection();
      
      _hHits_0 = Prepare2DHisto("hitshist0",wiremin[0],wiremax[0],timemin[0],timemax[0]);
      _hHits_1 = Prepare2DHisto("hitshist1",wiremin[1],wiremax[1],timemin[1],timemax[1]);
      _hHits_2 = Prepare2DHisto("hitshist2",wiremin[2],wiremax[2],timemin[2],timemax[2]);

      int tmpcounter = 0;
      //loop over the clusters in the event
      for(auto clus : clus_v){
	tmpcounter++;

	//check from which view the cluster comes
	iview = clus.View();

	//prepare the cluster th2d histogram
	TH2D* h=0;
	if(h) delete h;
	h = Prepare2DHisto(Form("_hRecoCluster_%03d",tmpcounter), wiremin[iview], wiremax[iview], timemin[iview], timemax[iview]);

	TGraph* g=0;
	if(g) delete g;
	g = PrepareGraph();

	//prepare the cluster start/endpoint graph
	//clear any old points that might be in the graph
	//	g->Set(0);
	//fill the graph with start and end point
	g->SetPoint(0,clus.StartPos()[0],clus.StartPos()[1]);
	g->SetPoint(1,clus.EndPos()[0],clus.EndPos()[1]);
	Printf("just filled the graph with points (%f,%f), (%f,%f). view is %d\n",
	       clus.StartPos()[0], clus.StartPos()[1],
	       clus.EndPos()[0], clus.EndPos()[1], clus.View());

	//then loop through its hits, and plot wire/time for each hit in the right _hRecoCluster_v_blah histo
	ihit_v = clus.Hits();

	for(auto hit : ihit_v){
	  
	  if (hit.View() != iview) std::cout<<"diff views? dunno what's going on."<<std::endl;
	  //fill those hits into the cluster plot
	  h->Fill(hit.Wire(),hit.PeakTime());
	  
	  //fill the hits histogram, weight is the charge of the hit
	  if(hit.View() == 0)
	    _hHits_0->Fill(hit.Wire(),hit.PeakTime(),hit.Charge());
	  if(hit.View() == 1)
	    _hHits_1->Fill(hit.Wire(),hit.PeakTime(),hit.Charge());
	  if(hit.View() == 2)
	    _hHits_2->Fill(hit.Wire(),hit.PeakTime(),hit.Charge());

	} //end looping over hits

	
	//need to make each cluster a different color... 
	//there's probably a better way to do this. implement later
	h->SetMarkerStyle(23);
	h->SetMarkerColor(tmpcounter);	



	if((int)iview == 0){
	  _hRecoCluster_v_0.push_back(h);
	  _hClusterGraph_v_0.push_back(g);
	}
	else if ((int)iview == 1){
	  _hRecoCluster_v_1.push_back(h);
	  _hClusterGraph_v_1.push_back(g);
	}
	else if ((int)iview == 2){
	  _hRecoCluster_v_2.push_back(h);
	  _hClusterGraph_v_2.push_back(g);
	}
	else
	  std::cout<<"iview is not 0, 1, or 2... wtf?"<<std::endl;

      }//end loop over clusters in the event

    }
    
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
  
  
  //################################################################
  TGraph* ClusterViewer::PrepareGraph()
  //################################################################
  {
    TGraph* g=0;
    if(g) delete g;
    
    g = new TGraph();
    g->Set(0);
    
    return g;
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

