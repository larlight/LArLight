#ifndef CLUSTERVIEWERALGO_CC
#define CLUSTERVIEWERALGO_CC

#include "ClusterViewerAlgo.hh"

namespace larlight {
  //################################################################
  ClusterViewerAlgo::ClusterViewerAlgo() : ana_base()
  //################################################################
  {
    // Class name
    _name = "ClusterViewerAlgo";

    _hAllHits.clear();
    _hClusterHits.clear();
    _gClusterStart.clear();
    _gClusterEnd.clear();
  }

  //##########################################################
  void ClusterViewerAlgo::SetClusterType(DATA::DATA_TYPE type)
  //##########################################################
  {
    if( type != DATA::Cluster &&
	type != DATA::FuzzyCluster &&
	type != DATA::CrawlerCluster &&
	type != DATA::ShowerAngleCluster &&
	type != DATA::HoughCluster )

      print(MSG::ERROR,__FUNCTION__,Form("Not a valid cluster type: %s",DATA::DATA_TREE_NAME[type].c_str()));
    
    else
      cluster_type=type;

  }
  
  //#############################
  bool ClusterViewerAlgo::Reset()
  //#############################
  {
    
    // Clean up histograms if they already exist (from previous event)
    for(auto &h : _hAllHits)      { delete h; h=0; }
    for(auto &h : _hClusterHits)  { delete h; h=0; }
    for(auto &g : _gClusterStart) { delete g; g=0; }
    for(auto &g : _gClusterEnd)   { delete g; g=0; }

    _hAllHits.clear();
    _hClusterHits.clear();
    _gClusterStart.clear();
    _gClusterEnd.clear();
    _cluster_v.clear();
  }

  //############################################################################
  void ClusterViewerAlgo::AppendCluster( const std::vector<larutil::PxHit>& hits,
					 const cluster::cluster_params& cluster)
  //############################################################################
  {
    //
    // Obtain event-wise data object pointers
    //
    //  event_mc*      ev_mc   = (event_mc*)      ( storage->get_data(DATA::MCTruth)            );
    event_cluster* ev_clus = (event_cluster*) ( storage->get_data(cluster_type));
    event_hit*     ev_hit  = (event_hit*) ( storage->get_data(ev_clus->get_hit_type()));

    // Define utility variables to hold max/min of each axis range, for each of 3 views
    //vector of length 3 initialized to -1
    std::vector<double> chmax, chmin, wiremax, wiremin, timemax, timemin;
    
    // Find max/min boundary for all axis (clusters)
    
    if(ev_clus && ev_hit) {
      ev_hit->get_axis_range(chmax, chmin, wiremax, wiremin, timemax, timemin);
    }

    if(!wiremax.size()) {

      print(MSG::ERROR,__FUNCTION__,
	    "Something went wrong. Did not find hit or not even event_hit... Aborting!");

      return false;
    }
    
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
      
      _hHits_0 = Prepare2DHisto("hitshist0",wiremin[0],wiremax[0],timemin[0],timemax[0]);
      _hHits_1 = Prepare2DHisto("hitshist1",wiremin[1],wiremax[1],timemin[1],timemax[1]);
      _hHits_2 = Prepare2DHisto("hitshist2",wiremin[2],wiremax[2],timemin[2],timemax[2]);

      int tmpcounter = 0;
      //loop over the clusters in the event
      for(auto clus : *ev_clus) {
	tmpcounter++;

	//check from which view the cluster comes
	iview = clus.View();

	//prepare the cluster th2d histogram
	TH2D* h=0;
	h = Prepare2DHisto(Form("_hRecoCluster_%03d",tmpcounter), 
			   wiremin[iview], 
			   wiremax[iview], 
			   timemin[iview], 
			   timemax[iview]);

	TGraph* g_start=0;
	if(g_start) delete g_start;
	g_start = PrepareGraph();
	TGraph* g_end=0;
	if(g_end) delete g_end;
	g_end = PrepareGraph();

	//prepare the cluster start/endpoint graph
	//clear any old points that might be in the graph
	//	g->Set(0);
	//fill the graph with start and end point
	g_start->SetPoint(0,clus.StartPos()[0],clus.StartPos()[1]);
	g_end->SetPoint(0,clus.EndPos()[0],clus.EndPos()[1]);

	//then loop through its hits, and plot wire/time for each hit in the right _hRecoCluster_v_blah histo
	for(auto const index :clus.association(clus.get_hit_type())) {
	  

	  
	  if ((ev_hit->at(index)).View() != iview) std::cout<<"diff views? dunno what's going on."<<std::endl;
	  //fill those hits into the cluster plot
	  h->Fill((ev_hit->at(index)).Wire(),(ev_hit->at(index)).PeakTime());
	  
	  //fill the hits histogram, weight is the charge of the hit
	  if((ev_hit->at(index)).View() == 0)
	    _hHits_0->Fill((ev_hit->at(index)).Wire(),(ev_hit->at(index)).PeakTime(),(ev_hit->at(index)).Charge());
	  if((ev_hit->at(index)).View() == 1)
	    _hHits_1->Fill((ev_hit->at(index)).Wire(),(ev_hit->at(index)).PeakTime(),(ev_hit->at(index)).Charge());
	  if((ev_hit->at(index)).View() == 2)
	    _hHits_2->Fill((ev_hit->at(index)).Wire(),(ev_hit->at(index)).PeakTime(),(ev_hit->at(index)).Charge());

	} //end looping over hits

	
	//need to make each cluster a different color... 
	//there's probably a better way to do this. implement later
	h->SetMarkerStyle(kFullStar);
	h->SetMarkerColor(tmpcounter);	

	if((int)iview == 0){
	  _hRecoCluster_v_0.push_back(h);
	  _hClusterGraph_v_0_start.push_back(g_start);
	  _hClusterGraph_v_0_end.push_back(g_end);
	}
	else if ((int)iview == 1){
	  _hRecoCluster_v_1.push_back(h);
	  _hClusterGraph_v_1_start.push_back(g_start);	 
	  _hClusterGraph_v_1_end.push_back(g_end);
	}
	else if ((int)iview == 2){
	  _hRecoCluster_v_2.push_back(h);
	  _hClusterGraph_v_2_start.push_back(g_start);
	  _hClusterGraph_v_2_end.push_back(g_end);
	}
	else
	  std::cout<<"iview is not 0, 1, or 2... wtf?"<<std::endl;

      }//end loop over clusters in the event

    }
    
    return true;
  };
  
  //################################################################
  TH2D* ClusterViewerAlgo::Prepare2DHisto(std::string name, 
				      double wiremin, double wiremax,
				      double timemin, double timemax)
  //################################################################
  {
    
    TH2D* h=0;
    if(h) delete h;
    
    double mywiremin=0.9*wiremin;
    double mywiremax=1.1*wiremax;
    double mytimemin=0.9*timemin;
    double mytimemax=1.1*timemax;

    h = new TH2D(name.c_str(),"2D Viewer; Wire; Time;",
		 50,  mywiremin, mywiremax,
		 50,  mytimemin, mytimemax);
    
    return h;
  }
  
  
  //################################################################
  TGraph* ClusterViewerAlgo::PrepareGraph()
  //################################################################
  {
    TGraph* g = new TGraph;
    g->Set(0);
    return g;
  }

}
#endif

