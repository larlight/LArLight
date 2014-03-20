#ifndef SHOWERANGLECLUSTER_CC
#define SHOWERANGLECLUSTER_CC

#include "ShowerAngleCluster.hh"

namespace larlight{

  ShowerAngleCluster::ShowerAngleCluster()  : ana_base() 
  {

    fh_omega_single=nullptr;
    ftree_cluster=nullptr;
    _name="ShowerAngleCluster"; 
    _in_cluster_type=DATA::FuzzyCluster;
    geo  = (larutil::Geometry*)(larutil::Geometry::GetME());
    larp = (larutil::LArProperties*)(larutil::LArProperties::GetME());
    detp = (larutil::DetectorProperties*)(larutil::DetectorProperties::GetME());
    gser = (larutil::GeometryUtilities*)(larutil::GeometryUtilities::GetME());
    fCMergeAlg = 0;
    this->reconfigure();
    
  }

  void ShowerAngleCluster::reconfigure()
  {
    fInputClusterType = larlight::DATA::FuzzyCluster;
    fCParAlg.reconfigure();
    fExternalStartPoints = false;
    fMinHitListSize = 15;
    if(!fCMergeAlg) fCMergeAlg = new larreco::ClusterMergeAlg();
  }

  void ShowerAngleCluster::SetInputClusterType(DATA::DATA_TYPE type)
  {
    if( type != DATA::FuzzyCluster &&
	type != DATA::ShowerAngleCluster &&
	type != DATA::CrawlerCluster &&
	type != DATA::DBCluster )
      {
	print(MSG::ERROR,__FUNCTION__,
	      Form("Not supporting \"%s\" cluster type...",DATA::DATA_TREE_NAME[type].c_str()));
	return;	
      }
    _in_cluster_type = type;
  }

  bool ShowerAngleCluster::initialize() {

    // this will not change on a run per run basis.
    fNPlanes = geo->Nplanes();
    // fWirePitch = geo->WirePitch(0,1,0);    //wire pitch in cm
    fTimeTick=detp->SamplingRate()/1000.; 
    
    fh_omega_single= new TH1F("fh_omega_single","Theta distribution Hit",720,-180., 180.);

    fCMergeAlg->set_verbosity(this->get_verbosity());
    fCMergeAlg->initialize();
    //    fCMergeAlg->SetAngleCut(99999.);
    //    fCMergeAlg->SetSquaredDistanceCut(999999.);
    fCMergeAlg->SetQTotalCut(0.);
    return true;
  }
  
  bool ShowerAngleCluster::analyze(larlight::storage_manager* storage) {

    // Get an input cluster 
    auto clusters = (const event_cluster*)(storage->get_data(_in_cluster_type));

    std::cout<<"in showeranglecluster, just got event_cluster collection... there are "<<clusters->size()<<" clusters."<<std::endl;
    // Check if it is valid. If not, return false
    if(!clusters) {
      
      print(MSG::ERROR,__FUNCTION__,
	    Form("Did not find input cluster type: \"%s\"",
		 DATA::DATA_TREE_NAME[_in_cluster_type].c_str()));
      return false;
    }

    // Get associated hit type and hit data container
    const DATA::DATA_TYPE hit_type = clusters->get_hit_type();    
    auto hits = (event_hit*)(storage->get_data(hit_type));

    // Check if it is valid. 
    if(!hits) {      
      print(MSG::ERROR,__FUNCTION__,
	    Form("Did not find associated hits of type: \"%s\"",
		 DATA::DATA_TREE_NAME[hit_type].c_str()));
      return false;
    }

    // Get an output cluster data container. Clear it in case it's already filled.
    //auto out_clusters = (event_cluster*)(storage->get_data(DATA::ShowerAngleCluster));
    auto out_clusters = (event_cluster*)(storage->get_data(DATA::ShowerAngleCluster));
    out_clusters->clear();


    std::vector<larlight::cluster> localvec(0,larlight::cluster());
    localvec.clear();
    std::vector<larlight::cluster> secondlocalvec(0,larlight::cluster());
    localvec.clear();


    // Prepare attribute vectors (clear & resize std::vector)
    ClearandResizeVectors(clusters->size());

    // Loop over input clusters
    for(size_t iClust=0; iClust<clusters->size(); ++iClust) {
      const std::vector<UShort_t> cl_hits_index(clusters->at(iClust).association(hit_type));
      std::vector<larlight::hit*> cl_hits;
      cl_hits.reserve(cl_hits_index.size());
      for(auto index : cl_hits_index)
	
	cl_hits.push_back(&(hits->at(index)));

      larlight::cluster out_cl(MainClusterLoop(clusters->at(iClust), 
					       cl_hits, 
					       iClust, 
					       localvec.size())
			       );

      if( fCParAlg.isShower(lineslopetest[iClust],
			    fWireVertex[iClust],
			    fTimeVertex[iClust],
			    fWireEnd[iClust],
			    fTimeEnd[iClust],
			    cl_hits) 
	  ) {
	
	
	//now that the cluster is shower-like, run the merging alg, which
	//returns a vector saying which clusters should be merged

	//here, push back to a local vector, later run that vector through mergealg to reduce it
	out_cl.add_association(hit_type,cl_hits_index);

	localvec.push_back(out_cl);	
      }
    }
     
    fCMergeAlg->ClearEventInfo();
    
    std::cout<<"putting in "<<localvec.size()<<" localvec clusters!"<<std::endl;

    for(size_t i_lv = 0; i_lv < localvec.size(); ++i_lv)
	fCMergeAlg->AppendClusterInfo(localvec[i_lv],hits);

    fCMergeAlg->ProcessMergeAlg();
    
    const std::vector<std::vector<unsigned int> > cluster_sets = fCMergeAlg->GetClusterSets();
    std::cout<<"got back "<<cluster_sets.size()<<" groups of clusters to be merged together"<<std::endl;


    //hit associations for future merged clusters
    std::vector<unsigned short> merged_hit_ass;

    //loop over cluster sets vector to determine cluster IDs to merge
    for(auto const& cluster_id_set : cluster_sets) {
    
      //don't i need to clear merged_hit_ass here? this isn't done in ClusterMerge
      merged_hit_ass.clear();

      //declare output cluster
      cluster merged_cluster(clusters->data_type());

      GEO::View_t view = GEO::kUnknown;
      
      for(auto const& cluster_id : cluster_id_set) {
	
	const cluster i_cluster = clusters->at(cluster_id);
	
	std::vector<unsigned short> hit_index;

	hit_index = i_cluster.association(hit_type);

	for(auto const& index : hit_index)
	  
	  merged_hit_ass.push_back(index);      
      
        if(view == GEO::kUnknown) view = clusters->at(cluster_id).View();
	
      } //end looping over original clusters to be merged
  
    
      merged_cluster.add_association(hit_type,merged_hit_ass);
      
      merged_cluster.set_charge(-1);
      merged_cluster.set_dtdw(-1);
      merged_cluster.set_dtdw_err(-1);
      merged_cluster.set_dqdw(-1);
      merged_cluster.set_dqdw_err(-1);
      merged_cluster.set_id(out_clusters->size());
      merged_cluster.set_view(view);
    
      std::vector<double> whatever(2,-1);
      
      merged_cluster.set_start_vtx(whatever);
      merged_cluster.set_start_vtx_err(whatever);
      merged_cluster.set_end_vtx(whatever);
      merged_cluster.set_end_vtx_err(whatever);
      //    merged_cluster.set_event_id(clusters->event_id());
      
      secondlocalvec.push_back(merged_cluster);

    }
  
    
    
      //now loop over secondlocalvec and run merged clusters through MainClusterLoop again
    for(size_t iClust=0; iClust<secondlocalvec.size(); ++iClust) {
      const std::vector<UShort_t> merged_cl_hits_index(secondlocalvec[iClust].association(hit_type));
      std::vector<larlight::hit*> merged_cl_hits;
      merged_cl_hits.reserve(merged_cl_hits_index.size());
      for(auto index: merged_cl_hits_index)
	merged_cl_hits.push_back(&(hits->at(index)));
      larlight::cluster merged_out_cl(MainClusterLoop(secondlocalvec[iClust],
						      merged_cl_hits,
						      iClust,
						      out_clusters->size()) //not sure what this out_clusters->size() is for
				      );

      //do i need to ask if isShower again?

      out_clusters->push_back(merged_out_cl);
    }  
    //now out_clusters is saved by storage manager automatically 
    
    std::cout<<"in the end, i am left with "<< out_clusters->size() <<" merged clusters."<<std::endl;
    return true;
  }

  bool ShowerAngleCluster::finalize() {
    delete fh_omega_single;
    return true;
  }

  // ************************************* //
  void ShowerAngleCluster::ClearandResizeVectors(unsigned int nClusters) 
  {
  
   fVerticalness.clear();

   lineslopetest.clear();
   lineinterctest.clear();
  
   xangle.clear();
   fWireVertex.clear();
   fTimeVertex.clear();
   fWireEnd.clear();
   fTimeEnd.clear();
   xangle.resize(nClusters); 


   fWireVertex.resize(nClusters);
   fTimeVertex.resize(nClusters);
   fWireEnd.resize(nClusters);
   fTimeEnd.resize(nClusters);
    
   fVerticalness.resize(nClusters);
   lineslopetest.resize(nClusters);
   lineinterctest.resize(nClusters);
  // startflag.resize(nClusters);
   
   fErrors.resize(nClusters);
  }

  larlight::cluster ShowerAngleCluster::MainClusterLoop(const larlight::cluster &inCluster,
							const std::vector<larlight::hit*> &hitlist,
							unsigned int iClustInput,
							unsigned int iClustOutput) {
    
    std::vector<double> spos(inCluster.StartPos());
    std::vector<double> sposerr(inCluster.SigmaStartPos());
      
    std::vector<double> epos(inCluster.EndPos());
    std::vector<double> eposerr(inCluster.SigmaEndPos());

    // Start positions are determined elsewhere and accepted here
    
    ///! Change for accepting DBCluster and cheatcluster, so that it doesn't get fooled.
    //startflag.push_back(false); 
    
    //std::cout << " hitlist size: " << hitlist.size() << std::endl;
      
    double lineslope, lineintercept,goodness,wire_start,time_start,wire_end,time_end;
    int nofshowerclusters=0;
    // //std::cout << "++++ hitlist size " << hitlist.size() << std::endl;
    
    //////////////////////////////////
    // fCParAlg.Find2DAxisRoughHighCharge(lineslope,lineintercept,goodness,hitlist);
    //  //std::cout << "%%%%%%%% lineslope, intercept " << lineslope << " "<< lineintercept << std::endl;
    
    fCParAlg.Find2DAxisRough(lineslope,lineintercept,goodness,hitlist);
    fVerticalness[iClustInput]=goodness;
    //if(hitlist_high.size()<=3 )
	//continue;

    fCParAlg.Find2DStartPointsHighCharge( hitlist,wire_start,time_start,wire_end,time_end);
    
    fWireVertex[iClustInput]=wire_start;
    fTimeVertex[iClustInput]=time_start;

    double wstn=0,tstn=0,wendn=0,tendn=0;
    fCParAlg.FindTrunk(hitlist,wstn,tstn,wendn,tendn,lineslope,lineintercept);
    int fDirection = (wstn<wendn)  ? 1 : -1 ;     // if current starting point is less then end point then direction is 1.
    

    double HiBin,LowBin,invHiBin,invLowBin;
    fCParAlg.FindDirectionWeights(lineslope,wstn,tstn,wendn,tendn,hitlist,HiBin,LowBin,invHiBin,invLowBin); 
    
    if(invHiBin+invLowBin> 1000)
      nofshowerclusters++;
    
    fDirection=fCParAlg.DecideClusterDirection(hitlist,lineslope,wstn,tstn,wendn,tendn);
     //std::cout << "%%%%%%%% direction start points: (" << wstn<<","<<tstn<<"), end: ( "<<wendn << ","<<tendn <<")" << "Direction: " << fDirection << std::endl;
    wire_start=wstn;
    time_start=tstn;
    wire_end=wendn;
    time_end=tendn;
    //fCParAlg.RefineStartPointsHough(hitlist, wire_start,time_start,wire_end,time_end,fDirection); 
     //std::cout << "%%%%%%%% Hough line refine start points: (" << wire_start<<","<<time_start<<"), end: ( "<<wire_end << ","<<time_end <<")" << "Direction: " << fDirection << std::endl; 
    if(fExternalStartPoints==false)  // start points have been handed over from external
    {
      fWireVertex[iClustInput]=wire_start;
      fTimeVertex[iClustInput]=time_start;
      fWireEnd[iClustInput]=wire_end;
      fTimeEnd[iClustInput]=time_end; 
    }
    else
    {
     fWireVertex[iClustInput]=spos[0];
     fTimeVertex[iClustInput]=spos[1]; 
     fWireEnd[iClustInput]=epos[0];
     fTimeEnd[iClustInput]=epos[1];
    }
    
    
    
    lineslopetest[iClustInput]=lineslope; 
    lineinterctest[iClustInput]=lineintercept;
      
	  
    xangle[iClustInput]=Get2DAngleForHit( fWireVertex[iClustInput],fTimeVertex[iClustInput], hitlist);
  
  
    //std::cout << " in save loop, \"iplane\" " << iClustInput <<  std::endl;
    //std::cout << " hitlist size: " << hitlist.size() <<  std::endl;
     
  
    if(fVerticalness[iClustInput]<1) 
      fErrors[iClustInput]=0.1;
    else
      fErrors[iClustInput]=10; 
  
      
    // Following 2 lines commented out: we have an access to the plane through viewfix variable
    //unsigned int xplane;
    //xplane=hitlist[0]->WireID().Plane;

    larlight::GEO::View_t viewfix = hitlist.at(0)->View();
    
    //std::cout << " wire " << fWireVertex[iClustInput] << std::endl;
    //std::cout << " fErrors " << fErrors[iClustInput] << std::endl;
    //std::cout << " time " << fTimeVertex[iClustInput] << std::endl;
    //std::cout << " wirelast " << fWireEnd[iClustInput] << std::endl;
    //std::cout << " timelast " <<  fTimeEnd[iClustInput] << std::endl; 
    //std::cout << " xangle " <<  xangle[iClustInput] << std::endl; 
    //std::cout <<"lineslope " <<  lineslopetest[iClustInput] << std::endl; 
    //std::cout <<"lineinterc " << lineinterctest[iClustInput] <<std::endl;	
    //std::cout <<"plane  " << xplane <<std::endl;	

    /*
    larlight::cluster outCluster(fWireVertex[iClustInput], fErrors[iClustInput],
				 fTimeVertex[iClustInput], fErrors[iClustInput],
				 fWireEnd[iClustInput], fWireEnd[iClustInput]*0.05,
				 fTimeEnd[iClustInput], fTimeEnd[iClustInput]*0.05,  
				 xangle[iClustInput], xangle[iClustInput]*0.05, lineslopetest[iClustInput],lineinterctest[iClustInput],5.,
				 viewfix,
				 iClustOutput);
    */
    larlight::cluster outCluster;
    std::vector<Double_t> start(2,0);
    std::vector<Double_t> end(2,0);
    std::vector<Double_t> start_err(2,0);
    std::vector<Double_t> end_err(2,0);
    start[0]=fWireVertex[iClustInput];
    start[1]=fTimeVertex[iClustInput];
    end[0]=fWireEnd[iClustInput];
    end[1]=fTimeEnd[iClustInput];
    start_err[0]=start_err[1]=fErrors[iClustInput];
    end_err[0]=end[0]*0.05;
    end_err[1]=end[1]*0.05;
    outCluster.set_start_vtx(start);
    outCluster.set_start_vtx_err(start_err);
    outCluster.set_end_vtx(end);
    outCluster.set_end_vtx_err(end_err);
    outCluster.set_dtdw(xangle.at(iClustInput));
    outCluster.set_dtdw_err(xangle.at(iClustInput) * 0.05);
    outCluster.set_dqdw(lineslopetest.at(iClustInput));
    outCluster.set_dqdw_err(lineinterctest.at(iClustInput) * 0.05);
    outCluster.set_charge(5.);
    outCluster.set_view(viewfix);
    outCluster.set_id(iClustOutput);

    
    //     ShowerAngleCluster->push_back(temp);
    //   
    //     util::CreateAssn(*this, evt, *(ShowerAngleCluster.get()), hitlist, *(assn.get()));
    return outCluster;
  }

  larlight::cluster ShowerAngleCluster::MergeClusterLoop( const std::vector<larlight::hit*> &hitlist,
							  unsigned int iClustOutput) {

    double lineslope, lineintercept,goodness,wire_start,time_start,wire_end,time_end;
    
    // initialize
    lineslope = lineintercept = goodness = wire_start = time_start = wire_end = time_end = 0;
    
    fCParAlg.Find2DAxisRough(lineslope,lineintercept,goodness,hitlist);
    
    fCParAlg.Find2DStartPointsHighCharge( hitlist, wire_start, time_start, wire_end, time_end);
    
    fCParAlg.FindTrunk(hitlist,wire_start,time_start,wire_end,time_end,lineslope,lineintercept);
    int fDirection = (wire_start<wire_end)  ? 1 : -1 ;     // if current starting point is less then end point then direction is 1.
    
    double HiBin,LowBin,invHiBin,invLowBin;
    fCParAlg.FindDirectionWeights(lineslope,
				  wire_start, time_start,
				  wire_end, time_end,
				  hitlist,
				  HiBin,LowBin,invHiBin,invLowBin);
    
    fDirection=fCParAlg.DecideClusterDirection(hitlist,
					       lineslope,
					       wire_start, time_start,
					       wire_end,   time_end);
    
    //fCParAlg.RefineStartPointsHough(hitlist, wire_start,time_start,wire_end,time_end,fDirection);
    
    if(fExternalStartPoints)  // start points have been handed over from external
      {
	// For merging, "external" start/end points has to be re-generated here because
	// the input clusters' hit list has been merged unless merging algorithm produces
	// a relevant start/end point on the merged cluster
	print(larlight::MSG::ERROR,__FUNCTION__,
	      "External start points not implemented for merging stage!");
      }
    
    double angle_2d = Get2DAngleForHit(wire_start, time_start, hitlist);
    
    double vtx_error = ( (goodness < 1) ? 0.1 : 10);

    larlight::GEO::View_t viewfix = hitlist.at(0)->View();

    /*
    larlight::cluster outCluster(wire_start, vtx_error,
				 time_start, vtx_error,
				 wire_end, wire_end*0.05,
				 time_end, time_end*0.05,
				 angle_2d, angle_2d*0.05, lineslope, lineintercept, 5.,
				 viewfix,
				 iClustOutput);
    */
    larlight::cluster outCluster(larlight::DATA::ShowerAngleCluster);
    std::vector<Double_t> start(2,0);
    std::vector<Double_t> end(2,0);
    std::vector<Double_t> start_err(2,0);
    std::vector<Double_t> end_err(2,0);
    start[0]=wire_start; start[1]=time_start;
    end[0]=wire_end; end[1]=time_end;
    start_err[0]=start_err[1]=vtx_error;
    end_err[0]=wire_end*0.05;
    end_err[1]=time_end*0.05;
    outCluster.set_start_vtx(start);
    outCluster.set_start_vtx_err(start_err);
    outCluster.set_end_vtx(end);
    outCluster.set_end_vtx_err(end_err);
    outCluster.set_dtdw(angle_2d);
    outCluster.set_dtdw_err(angle_2d*0.05);
    outCluster.set_dqdw(lineslope);
    outCluster.set_dqdw_err(lineintercept);
    outCluster.set_charge(5.);
    outCluster.set_view(viewfix);
    outCluster.set_id(iClustOutput);
    return outCluster;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // Method to get the 2D angle ogf a Cluster based on its starting wire and time.
  ////////////////////////////////////////////////////////////////////////////////
  double ShowerAngleCluster::Get2DAngleForHit( unsigned int swire,double stime,
					       const std::vector <larlight::hit*> &hitlist) {
    
    fh_omega_single->Reset();
    
    unsigned int wire;
    // this should changed on the loop on the cluster of the shower
    for(auto const h : hitlist) {
      // art::Ptr<recob::Hit> theHit = (*hitIter);
      double time = h->PeakTime();  
      wire=h->Wire();
      double omx=gser->Get2Dangle((double)wire,(double)swire,time,stime);
      fh_omega_single->Fill(180*omx/TMath::Pi(),h->Charge());
    }
    
    double omega = fh_omega_single->GetBinCenter(fh_omega_single->GetMaximumBin());// Mean value of the fit
    
    return omega; // in degrees.
  }  
  
}
#endif
