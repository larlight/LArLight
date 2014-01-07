#ifndef CLUSTERMERGEALG_CC
#define CLUSTERMERGEALG_CC

#include "ClusterMergeAlg.hh"

namespace larlight {

  ClusterMergeAlg::ClusterMergeAlg() : ana_base() {
    
    _name="ClusterMergeAlg"; 
    _fout=0;     
    _verbose=false;
    
    SetAngleCut(180.);

    SetSquaredDistanceCut(1e9);

    _min_distance_unit = -1;

    _det_params_prepared = false;

    _merge_tree = 0;

    _min_hits_to_consider = 200;


    ClearEventInfo();

  };
  

  void ClusterMergeAlg::ReportConfig() const {

    std::ostringstream msg;
    msg
      << std::endl
      << " ClusterMergeAlg Configuration:              " << std::endl
      << "---------------------------------------------" << std::endl;
    msg 
      << " Verbose Mode ... " << (_verbose ? "enabled!" : "disabled!") << std::endl
      << " Wire => Cm Conversion: " << _wire_2_cm << std::endl
      << " Time => Cm Conversion: " << _time_2_cm << std::endl
      << std::endl
      << " Squared-Distance Cut: " << _max_2D_dist2 << std::endl
      << " Angle Difference Cut: " << _max_allowed_2D_angle_diff << std::endl
      << std::endl
      << "---------------------------------------------" << std::endl;

    print(MSG::NORMAL,__FUNCTION__,msg.str());

  }

  bool ClusterMergeAlg::initialize() {

    PrepareTTree();

    PrepareHistos();
    
    return true;
  }

  bool ClusterMergeAlg::finalize() {
    
    if(_hit_angles_forwards)
      {delete _hit_angles_forwards; _hit_angles_forwards=0;}
    if(_hit_angles_backwards)
      {delete _hit_angles_backwards; _hit_angles_backwards=0;}
    if(_compare_angle_definitions)
      {delete _compare_angle_definitions; _compare_angle_definitions=0;}
    

    if(_fout) {

      _fout->cd();
      _merge_tree->Write();

    }
  
    return true;
  }
  
  /**
     In case ClusterMergeAlg is run as analysis module in ana_processor, it does following tasks:
     (0) Clear event-wise information (input & output)
     (1) Read-in cluster information into cluster_merge_info struct
     (2) Process read-in cluster information for merging
  */
  bool ClusterMergeAlg::analyze(storage_manager* storage) {

    // Step (0) ... Clear input cluster information
    
    ClearEventInfo();

    // Step (1) ... loop over input cluster sets and store relevant information into the cluster_merge_info

    const event_cluster* ev_cluster = (const event_cluster*)(storage->get_data(DATA::ShowerAngleCluster));

    const std::vector<cluster> cluster_collection = ev_cluster->GetClusterCollection();

    for(auto const i_cluster: cluster_collection)

      AppendClusterInfo(i_cluster,i_cluster.Hits());

    
    // Step (2) ... Run algorithm
    ProcessMergeAlg();

    return true;

  }

  void ClusterMergeAlg::AppendClusterInfo(const cluster &cl, 
					   const std::vector<larlight::hit> &in_hit_v) {

    PrepareDetParams();

    cluster_merge_info ci;
    ci.cluster_index = cl.ID();
    ci.view       = cl.View();
    ci.start_wire = cl.StartPos()[0];
    ci.start_time = cl.StartPos()[1];
    ci.end_wire   = cl.EndPos()[0];
    ci.end_time   = cl.EndPos()[1];
    ci.angle      = cl.dTdW();
    ci.n_hits     = in_hit_v.size();


    //trying to compare andrzej's angle (which comes from a loop over hits and calls to a geometry package) to the angle calculated from start-to-end point and conversion to the proper units
    //it seems they're TOTALLY different. not sure what to do about it. for now, i'm just excluding these events from multiplicity plots
    /*
    double tmpangle = (ci.end_wire-ci.start_wire)/
      pow(pow( (ci.end_wire-ci.start_wire),2 ) + 
	  pow( (ci.end_time-ci.start_time),2 ),0.5 );
    tmpangle = acos(tmpangle);
    tmpangle = tmpangle * 180. / 3.14159;
    _compare_angle_definitions->Fill(ci.angle,tmpangle);
    std::cout<<Form("angle = %f, tmpangle = %f\n",ci.angle,tmpangle)
	     <<std::endl;
    */

    //this function modifies ci
    AppendHitInfo(ci, in_hit_v);

    if(ci.view == GEO::kU) _u_clusters.push_back(ci);
    else if(ci.view == GEO::kV) _v_clusters.push_back(ci);
    else if(ci.view == GEO::kW) _w_clusters.push_back(ci);
    else print(MSG::ERROR,__FUNCTION__,Form("Invalid plane ID: %d",ci.view));

    if(_cluster_merged_index.size() <= (size_t)(ci.cluster_index)) {

      _cluster_merged_index.resize((size_t)(ci.cluster_index+1),-1);

      print(MSG::INFO,__FUNCTION__,Form("Extending the input cluster index ... length = %zu",_cluster_merged_index.size()));
    }

  }
  
  void ClusterMergeAlg::AppendHitInfo(cluster_merge_info &ci, const std::vector<larlight::hit> &in_hit_v)
  {
    //This function loops over hits and changes cluster_merge_info if it decides it wants to
    //For now, if it wants to siwtch the start/endpoint, it clears the cluster_info so it won't be
    //included in the rest of the routine. Ideally, I want to switch start/endpoint *and* the angle
    //but for now I can't figure how how the hell to deal with angle switch. It's complicated.
    RefineStartPointsShowerShape(ci, in_hit_v);

  };

  void ClusterMergeAlg::RefineStartPointsShowerShape(cluster_merge_info &ci, const std::vector<larlight::hit> &in_hit_v)
  {

    double i_hit_angle = -1.;
    _hit_angles_forwards->Reset();
    _hit_angles_backwards->Reset();
    
    //only bother refining stat/endpoints if the # of hits in the cluster is big enough
    if((int)in_hit_v.size() > _min_hits_to_consider){
      
      //loop over hits, make histogram of angle b/t hit and cluster axis
      //(line connecting the start point and end point)
      for(auto i_hit: in_hit_v){
	//dot product to calculate angle... is TVector2 faster?
	i_hit_angle = 
	  ( (ci.end_wire - ci.start_wire) * (i_hit.Wire()     - ci.start_wire) ) +
	  ( (ci.end_time - ci.start_time) * (i_hit.PeakTime() - ci.start_time) );
	//divide by magnitude
	i_hit_angle /= 
	  pow(( pow((ci.end_wire -ci.start_wire),2) + pow((ci.end_time     -ci.start_time),2) ),0.5) *
	  pow(( pow((i_hit.Wire()-ci.start_wire),2) + pow((i_hit.PeakTime()-ci.start_time),2) ),0.5);
	
	//      std::cout<<Form("cluster info: start: (%f,%f), end: (%f,%f), hit info: (%u,%f), calculated cos(angle) = %f",ci.start_wire,ci.start_time,ci.end_wire,ci.end_time,i_hit.Wire(),i_hit.PeakTime(),i_hit_angle)<<std::endl;
	
	_hit_angles_forwards->Fill(i_hit_angle);
	
	//now, pretend start and end points are swapped and make a histogram for that
	i_hit_angle = 
	  ( (ci.start_wire - ci.end_wire) * (i_hit.Wire()     - ci.end_wire) ) +
	  ( (ci.start_time - ci.end_time) * (i_hit.PeakTime() - ci.end_time) );
	//divide by magnitude
	i_hit_angle /= 
	  pow(( pow((ci.start_wire-ci.end_wire),2) + pow((ci.start_time   -ci.end_time),2) ),0.5) *
	  pow(( pow((i_hit.Wire() -ci.end_wire),2) + pow((i_hit.PeakTime()-ci.end_time),2) ),0.5);
	
	_hit_angles_backwards->Fill(i_hit_angle);
	
      }//end looping over hits in this cluster
      
      double f_mean = _hit_angles_forwards ->GetMean();
      double b_mean = _hit_angles_backwards->GetMean();
      double f_RMS  = _hit_angles_forwards ->GetRMS();
      double b_RMS  = _hit_angles_backwards->GetRMS();
      
      bool should_switch = (b_mean > f_mean) && (b_RMS < f_RMS);

      if(should_switch){

	double new_start_wire = ci.end_wire;
	double new_end_wire   = ci.start_wire;
	double new_start_time = ci.end_time;
	double new_end_time   = ci.start_time;
	
	ci.start_wire = new_start_wire;
	ci.start_time = new_start_time;
	ci.end_wire   = new_end_wire;
	ci.end_time   = new_end_time;
	// ci.angle      = something goes here
	//TEMPORARY workaround to angle problem.
	//angle2dcompatibility will ask if n_hits==-1, allow +/-180 on the angle
	ci.n_hits = -1;
      }

    } // end if there are enough hits in the cluster

  } // end RefineStartstuff function
  
  void ClusterMergeAlg::PrepareTTree() {

    if(!_merge_tree) {

      _merge_tree = new TTree("merge_tree","");

      _merge_tree->Branch("u_clus_mult", &u_clus_mult, "u_clus_mult/I");
      _merge_tree->Branch("v_clus_mult", &v_clus_mult, "v_clus_mult/I");
      _merge_tree->Branch("w_clus_mult", &w_clus_mult, "w_clus_mult/I");
      _merge_tree->Branch("u_angles", "std::vector<double>", &u_angles);
      _merge_tree->Branch("v_angles", "std::vector<double>", &v_angles);
      _merge_tree->Branch("w_angles", "std::vector<double>", &w_angles);

    }
  }

  void ClusterMergeAlg::PrepareHistos() {

    //need to intialize the pointers to zero or the shit hits the fan!
    _hit_angles_forwards=0;
    _hit_angles_backwards=0;
    _compare_angle_definitions=0;
    
    if(!_hit_angles_forwards)
      _hit_angles_forwards = new TH1F("_hit_angles_forwards","_hit_angles_forwards",200,-1,1);
    
    if(!_hit_angles_backwards)
      _hit_angles_backwards = new TH1F("_hit_angles_backwards","_hit_angles_backwards",200,-1,1);
    
    if(!_compare_angle_definitions)
      _compare_angle_definitions = new TH2F("_compare_angle_definitions","angle def comparison",100,-180,180,100,-180,180);
  }
  
  void ClusterMergeAlg::PrepareDetParams() {

    if(!_det_params_prepared) {

      _wire_2_cm = 0.3;
      _time_2_cm = 0.0818566;

      if(_min_distance_unit < 0) {

        double smaller_factor = (_wire_2_cm < _time_2_cm) ? _wire_2_cm : _time_2_cm;

        _min_distance_unit = pow(smaller_factor,2);

      }

      ReportConfig();
      _det_params_prepared = true;
    }
  } 

  void ClusterMergeAlg::CalculateTTreeVars() {
    //loop over _cluster_sets_v
    //for each element (vector), check the view of all the inner clusters
    //if they all match (which they should, by construction), add to the
    //appropriate *_clus_mult variable

    for(auto const i_cluster_set: _cluster_sets_v){
      
      bool set_is_in_u_plane = false;
      bool set_is_in_v_plane = false;
      bool set_is_in_w_plane = false;
      
      //find what view the first cluster_index in this set is from
      //(assuming the algo works, and all clusters in this set are from same view)
	
      //check if this cluster ID is in the _u_clusters vector
      //also push back the angle, which is another ttree variable
      for(auto const i_u_clus: _u_clusters){
	if(i_cluster_set[0] == i_u_clus.cluster_index)
	  set_is_in_u_plane = true;
	u_angles.push_back(i_u_clus.angle);
      }
      //check if this cluster ID is in the _v_clusters vector
      //also push back the angle, which is another ttree variable
      for(auto const i_v_clus: _v_clusters){
	if(i_cluster_set[0] == i_v_clus.cluster_index)
	  set_is_in_v_plane = true;
	v_angles.push_back(i_v_clus.angle);
      }
      //check if this cluster ID is in the _w_clusters vector
      //also push back the angle, which is another ttree variable
      for(auto const i_w_clus: _w_clusters){
	if(i_cluster_set[0] == i_w_clus.cluster_index)
	  set_is_in_w_plane = true;
	w_angles.push_back(i_w_clus.angle);
      }
      
      //now, I know what view this cluster grouping belongs to, ++ multiplicity
      //note i'm only doing ++, not += i_cluster_set.size() because this is the
      //multiplicity *after* all of the elements of i_cluster_set are theoretically merged
      if(set_is_in_u_plane) u_clus_mult++;
      if(set_is_in_v_plane) v_clus_mult++;
      if(set_is_in_w_plane) w_clus_mult++;
    }
    
  } //end CalculateTTreeVars
  
  void ClusterMergeAlg::ClearInputInfo() {

    _u_clusters.clear();
    _v_clusters.clear();
    _w_clusters.clear();
    _cluster_merged_index.clear();

  }

  void ClusterMergeAlg::ClearOutputInfo() {

    _cluster_sets_v.clear();
    u_angles.clear();
    v_angles.clear();
    w_angles.clear();
  }

  void ClusterMergeAlg::ClearTTreeInfo() {

    u_clus_mult = 0;
    v_clus_mult = 0;
    w_clus_mult = 0;
    u_angles.clear();
    v_angles.clear();
    w_angles.clear();

  }

  void ClusterMergeAlg::ClearEventInfo() {

    // Clear input event data holders
    ClearInputInfo();

    // Clear result data holders
    ClearOutputInfo();

    /// Clear TTree variables
    ClearTTreeInfo();

  }

  void ClusterMergeAlg::ProcessMergeAlg() {

    // Clear all algorithm's output
    ClearOutputInfo();
	

    //Loop over all possible combinations of clusters in u-view
    for(int iclus = 0; iclus < (int)_u_clusters.size(); ++iclus){

      for(int jclus = iclus+1; jclus < (int)_u_clusters.size(); ++jclus){

	BuildClusterSets(_u_clusters.at(iclus),_u_clusters.at(jclus));

      }//end loop over jclus _u_clusters

    }//end loop over iclus _u_clusters

    //Loop over all possible combinations of clusters in v-view
    for(int iclus = 0; iclus < (int)_v_clusters.size(); ++iclus){

      for(int jclus = iclus+1; jclus < (int)_v_clusters.size(); ++jclus){

	BuildClusterSets(_v_clusters.at(iclus),_v_clusters.at(jclus));

      }//end loop over jclus _v_clusters

    }//end loop over iclus _v_clusters

    //Loop over all possible combinations of clusters in w-view
    for(int iclus = 0; iclus < (int)_w_clusters.size(); ++iclus){

      for(int jclus = iclus+1; jclus < (int)_w_clusters.size(); ++jclus){

	BuildClusterSets(_w_clusters.at(iclus),_w_clusters.at(jclus));

      }//end loop over jclus _w_clusters

    }//end loop over iclus _w_clusters
    
    //make sure all of the un-mergable clusters are in _cluster_sets_v, individually
    FinalizeClusterSets();

    //calculate the quality control TTree vars that require calculation
    CalculateTTreeVars();

    if(_merge_tree) _merge_tree->Fill();

  }
  
  bool ClusterMergeAlg::CompareClusters(const cluster_merge_info &clusA,
					 const cluster_merge_info &clusB){
    
    if(_verbose) {
      print(MSG::NORMAL,__FUNCTION__,"Printing out two input cluster information...");
      PrintClusterVars(clusA);
      PrintClusterVars(clusB);
    }

    bool merge_clusters = true;
    
    
    merge_clusters = merge_clusters && Angle2DCompatibility(clusA, clusB);
    
    merge_clusters = merge_clusters && ShortestDistanceCompatibility(clusA, clusB);
    
    
    return merge_clusters;
    
  }


  bool ClusterMergeAlg::Angle2DCompatibility(const cluster_merge_info &cluster_a, 
					      const cluster_merge_info &cluster_b) const {
    bool compatible = false;

    //I don't think these conversion factors are needed. I'm 99% sure.
    double angle1 = cluster_a.angle;// * _time_2_cm / _wire_2_cm;
    double angle2 = cluster_b.angle;// * _time_2_cm / _wire_2_cm;
    
    //"if refinestart function wanted to switch the start/end point of either cluster
    //then accept the +/- 180 stuff on the angle cut. else, don't accept +/- 180 stuff"
    if(cluster_a.n_hits == -1 || cluster_b.n_hits == -1)
      compatible = (abs(angle1-angle2)     < _max_allowed_2D_angle_diff ||
		    abs(angle1-angle2-180) < _max_allowed_2D_angle_diff ||
		    abs(angle1-angle2+180) < _max_allowed_2D_angle_diff   );
    
    else
      compatible = (abs(angle1-angle2)     < _max_allowed_2D_angle_diff );
    
    
    if(_verbose) {
      
      if(compatible) print(MSG::NORMAL,__FUNCTION__," Compatible in angle.");
      else print(MSG::NORMAL,__FUNCTION__," NOT compatible in angle.");
      
    }
    
    return compatible;

  }//end Angle2DCompatibility

  bool ClusterMergeAlg::ShortestDistanceCompatibility(const cluster_merge_info &clus_info_A,
						       const cluster_merge_info &clus_info_B) const {

    double w_start1 = clus_info_A.start_wire * _wire_2_cm;
    double t_start1 = clus_info_A.start_time * _time_2_cm;
    double w_end1   = clus_info_A.end_wire   * _wire_2_cm;
    double t_end1   = clus_info_A.end_time   * _time_2_cm;

    double w_start2 = clus_info_B.start_wire * _wire_2_cm;
    double t_start2 = clus_info_B.start_time * _time_2_cm;
    double w_end2   = clus_info_B.end_wire   * _wire_2_cm;
    double t_end2   = clus_info_B.end_time   * _time_2_cm;
    
    //First, pretend the first cluster is a 2D line segment, from its start point to end point
    //Find the shortest distance between start point of the second cluster to this line segment.
    //Repeat for end point of second cluster to this line segment.
    //Then, pretend second cluster is a 2D line segment, from its start point to end point.
    //Find the shortest distance between start point of the first cluster to this line segment.
    //Repeat for end point of first cluster to this line segment.
    //If the shortest of these four distances is less than the cutoff, 
    //return true... the clusters are merge-compatible. else, return false.
    
    // Step 1: inspect (w_start1, t_start1) vs. line (w_start2, t_start2) => (w_end2, t_end2)
    double shortest_distance2 = ShortestDistanceSquared(w_start1, t_start1,
							w_start2, t_start2, w_end2, t_end2);
    
    // Step 2: inspect (w_end1, t_end1) vs. line (w_start2, t_start2) => (w_end2, t_end2)
    double shortest_distance2_tmp = ShortestDistanceSquared(w_end1, t_end1,
							    w_start2, t_start2, w_end2, t_end2);

    shortest_distance2 = (shortest_distance2_tmp < shortest_distance2) ? shortest_distance2_tmp : shortest_distance2;

    // Step 3: inspect (w_start2, t_start2) vs. line (w_start1, t_start1) => (w_end1, t_end1)
    shortest_distance2_tmp = ShortestDistanceSquared(w_start2, t_start2,
						     w_start1, t_start1, w_end1, t_end1);

    shortest_distance2 = (shortest_distance2_tmp < shortest_distance2) ? shortest_distance2_tmp : shortest_distance2;

    // Step 4: inspect (w_end2, t_end2) vs. line (w_start1, t_start1) => (w_end1, t_end1)
    shortest_distance2_tmp = ShortestDistanceSquared(w_end2, t_end2,
						     w_start1, t_start1, w_end1, t_end1);
    
    shortest_distance2 = (shortest_distance2_tmp < shortest_distance2) ? shortest_distance2_tmp : shortest_distance2;

    bool compatible = shortest_distance2 < _max_2D_dist2;

    if(_verbose) {

      if(compatible) print(MSG::NORMAL,__FUNCTION__,Form(" Compatible in distance (%g)",shortest_distance2));
      else print(MSG::NORMAL,__FUNCTION__,Form(" NOT compatible in distance (%g).",shortest_distance2));

    }

    return compatible;

  }//end startend2dcompatibility

  double ClusterMergeAlg::ShortestDistanceSquared(double point_x, double point_y, 
						   double start_x, double start_y,
						   double end_x,   double end_y  ) const {

    //This code finds the shortest distance between a point and a line segment.    
    //code based off sample from 
    //http://stackoverflow.com/questions/849211/shortest-distance-between-a-point-and-a-line-segment
    //note to self: rewrite this with TVector2 and compare time differences... 
    //TVector2 code might be more understandable

    double distance_squared = -1;

    // Line segment: from ("V") = (start_x, start_y) to ("W")=(end_x, end_y)
    double length_squared = pow((end_x - start_x), 2) + pow((end_y - start_y), 2);

    // Treat the case start & end point overlaps
    if(length_squared < _min_distance_unit) {
      
      std::ostringstream msg;
      msg
        << std::endl
        << Form(" Provided very short line segment: (%g,%g) => (%g,%g)",start_x,start_y,end_x,end_y) << std::endl
	<< " Likely this means one of two clusters have start & end point identical." << std::endl
        << " Check the cluster output!" << std::endl
        << std::endl
        << Form(" At this time, the algorithm uses a point (%g,%g)",start_x,start_y) << std::endl
        << " to represent this cluster's location." << std::endl
        << std::endl;

      print(MSG::WARNING,__FUNCTION__,msg.str());

      return (pow((point_x - start_x),2) + pow((point_y - start_y),2));

    }

    //Find shortest distance between point ("P")=(point_x,point_y) to this line segment
    double t = ( (point_x - start_x)*(end_x - start_x) + (point_y - start_y)*(end_y - start_y) ) / length_squared;

    if(t<0.0) distance_squared = pow((point_x - start_x), 2) + pow((point_y - start_y), 2);

    else if (t>1.0) distance_squared = pow((point_x - end_x), 2) + pow(point_y - end_y, 2);

    else distance_squared = pow((point_x - (start_x + t*(end_x - start_x))), 2) + pow((point_y - (start_y + t*(end_y - start_y))),2);
	   
    return distance_squared;
  }  
  
  void ClusterMergeAlg::PrintClusterVars(cluster_merge_info clus_info) const{

    std::ostringstream msg;

    msg
      << std::endl
      << "***********CLUSTER INFO PARAMETERS***********" << std::endl
      << " ID: " << clus_info.cluster_index << std::endl
      << " start_wire : " << clus_info.start_wire << std::endl
      << " start_time : " << clus_info.start_time << std::endl
      << " end wire   : " << clus_info.end_wire << std::endl
      << " end_time   : " << clus_info.end_time << std::endl
      << " angle      : " << clus_info.angle << std::endl
      << std::endl;
  
    print(MSG::NORMAL,__FUNCTION__,msg.str());

  }//end PrintClusterVars function

  int ClusterMergeAlg::AppendToClusterSets(unsigned int cluster_index, int merged_index) {

    // This function append the provided cluster_index into the _cluster_sets_v.
    // If merged_index is not provided (default=-1), then we assume this means to append a new merged cluster set.
    // Note we also log the merged index in _cluster_merged_index vector to support fast search in isInClusterSets() function.

    // First, check if this cluster_index is already in the set or not.
    if(!(isInClusterSets(cluster_index)<0)) {

      print(MSG::ERROR,__FUNCTION__,Form(" Cluster ID = %d already in the set!",cluster_index));

      return isInClusterSets(cluster_index);
      
    }

    if(merged_index < 0) {

      std::vector<unsigned int> tmp(1,cluster_index);

      _cluster_merged_index[cluster_index] = (unsigned int)(_cluster_sets_v.size());

      _cluster_sets_v.push_back(tmp);

    }
    else if(merged_index < (int)(_cluster_sets_v.size())) {

      _cluster_merged_index[cluster_index] = merged_index;

      _cluster_sets_v[merged_index].push_back(cluster_index);

    }
    else

      print(MSG::ERROR,__FUNCTION__,
	    Form(" Requested to merge the cluster ID = %ud into the set = %d which does not exist!",cluster_index,merged_index));

    return isInClusterSets(cluster_index);
  }


  void ClusterMergeAlg::BuildClusterSets(const cluster_merge_info &clusA, 
					  const cluster_merge_info &clusB){
    //this function builds the _cluster_sets_v vector, according to the format defined
    //in ClusterAlgo. here's an example of what _cluster_sets_v format is:
    //imagine an event with clusters 0, 1, 2, 3, 4, 5
    //pretend 0, 1 are found compatible for merging (pass cuts && are in same view, EG: U).
    //pretend 1, 2 are found compatible for merging (pass cuts && are in same view, U).
    //pretend 4, 5 are found compatible for merging (pass cuts && are in same view, EG: V)
    //pretend 3 is not found compatible for merging with anything, and is in view W.
    //_cluster_sets_v is a vector<vector<int>>, and it should look like
    //_cluster_sets_v = ( (0, 1, 2), (4, 5), (3) )

    //if the two clusters are compatible for merging
    if( CompareClusters(clusA,clusB) ){
     
      //if this is the first compatibility found for this entire event
      if(_cluster_sets_v.empty()){
	//push both IDs in to the _cluster_sets_v vector, grouped together as one element
	AppendToClusterSets(clusA.cluster_index);
	AppendToClusterSets(clusB.cluster_index,0);
      }
      else {

	// Inspect if these clusters are already in the set or not.
	// Return value < 0 indicates the set does not hold the subject cluster.
	// Otherwise the function returns the index of a set that includes the cluster.
	int a_index = isInClusterSets(clusA.cluster_index);
	int b_index = isInClusterSets(clusB.cluster_index);

	// if only A index already somewhere in the _cluster_sets_v vector, and B index is not
	if(a_index >= 0 && b_index < 0) 

	  AppendToClusterSets(clusB.cluster_index, a_index);

	// else if only B index already somewhere in the _cluster_sets_v vector, and A index is not
	else if(a_index < 0 && b_index >= 0)

	  AppendToClusterSets(clusA.cluster_index, b_index);

	// else if neither of the two are already in the _cluster_sets_v vector, anywhere
	else if(a_index < 0 && b_index < 0) {
	
	  a_index = AppendToClusterSets(clusA.cluster_index);
	  AppendToClusterSets(clusB.cluster_index,a_index);
	  
	}	  

	// else if both indexes are already in the _cluster_sets_v vector but the indexes are not same
	else if(a_index != b_index){

	  print(MSG::ERROR,__FUNCTION__,"LOGIC ERROR: Found two compatible clusters in different sets!");
	  //	  std::cout<<"a_index = "<<a_index<<", b_index = "<<b_index<<std::endl;
	  //	  PrintClusterSetsV();
	}
	//at the end of all possible cluster matching permutations, if some are not in 
	//_cluster_sets_v yet, push them back individually as individual elements...
	//this is what FinalizeClusterSets function is for

      }//end if _cluster_sets_v is non-empty

    }//end if the two clusters are compatible for merging
    
  }//end BuildClusterSets

  void ClusterMergeAlg::FinalizeClusterSets() {

    //loop over all cluster ID's in the event... guess I have to do it view-by-view
    for(int iclus = 0; iclus < (int)_u_clusters.size(); ++iclus){    
      //if the cluster_id is not in _cluster_sets_v at all, push it back individually
      if(isInClusterSets(_u_clusters.at(iclus).cluster_index) < 0)

	AppendToClusterSets(_u_clusters.at(iclus).cluster_index);

    }

    for(int iclus = 0; iclus < (int)_v_clusters.size(); ++iclus){    
      //if the cluster_id is not in _cluster_sets_v at all, push it back individually
      if(isInClusterSets(_v_clusters.at(iclus).cluster_index) < 0)

	AppendToClusterSets(_v_clusters.at(iclus).cluster_index);

    }

    for(int iclus = 0; iclus < (int)_w_clusters.size(); ++iclus){    
      //if the cluster_id is not in _cluster_sets_v at all, push it back individually
      if(isInClusterSets(_w_clusters.at(iclus).cluster_index) < 0)

	AppendToClusterSets(_w_clusters.at(iclus).cluster_index);

    }

    
  } // end FinalizeClusterSets
  
  int ClusterMergeAlg::isInClusterSets(unsigned int index) const {
    //function to check if an index is in _cluster_sets_v anywhere,
    //and return where it is (-1 if it's not in _cluster_sets_v)

    // Check if the provided index does not exceed the book-keeping vector
    if((size_t)index < _cluster_merged_index.size())

      return _cluster_merged_index.at(index);

    else {

      print(MSG::ERROR,__FUNCTION__,Form(" Cluster index %d not found among the input clusters!",index));

      return -2;

    }
    
  }//end isInClusterSets function
  

  void ClusterMergeAlg::PrintClusterSetsV(){
    
    std::cout<<"_cluster_sets_v = {";
    for(auto const i_cluster_set: _cluster_sets_v){
      std::cout<<"[";
      for(auto const i_index: i_cluster_set){
	std::cout<<i_index<<", ";
      }
      std::cout<<"],";
    }
    std::cout<<"}."<<std::endl;
    
  }//end print cluster sets v
}

#endif
