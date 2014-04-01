#ifndef CLUSTERMERGEALGNEW_CC
#define CLUSTERMERGEALGNEW_CC

#include "ClusterMergeAlgNew.hh"

namespace cluster {

  ClusterMergeAlgNew::ClusterMergeAlgNew() : fAlgoSwitch(kAlgoMax,false)
  {
    
    fAlgoSwitch.at(kStartPoint) = true;
    fAlgoSwitch.at(kPolygonCollision) = true;

    //Set default parameter values, user can change them with setter functions.
    _min_distance_unit = 0.;
    _max_2D_dist2 = 0.;
    _max_allowed_2D_angle_diff = 999.;

    //default verbosity. user can change with setter functions
    _verbose=false;

  
  }

  bool ClusterMergeAlgNew::Merge(const cluster::cluster_params &param_a, 
				 const cluster::cluster_params &param_b)
  {    
    
    bool merge = true;

    if(merge && fAlgoSwitch.at(kStartPoint))

      merge = TestStartPoint(param_a, param_b);

    if(merge && fAlgoSwitch.at(kAngleCompat))
      
      merge = Angle2DCompatibility(param_a, param_b);

    if(merge && fAlgoSwitch.at(kPolygonCollision))

      merge = TestPolygonCollision(param_a, param_b);
    
    return merge;
  }

  bool ClusterMergeAlgNew::TestStartPoint(const cluster::cluster_params &param_a, 
					  const cluster::cluster_params &param_b)
  {
    
    double w_start1 = param_a.start_point.w;
    double t_start1 = param_a.start_point.t;
    double w_end1   = param_a.end_point.w;
    double t_end1   = param_a.end_point.t;
    
    double w_start2 = param_b.start_point.w;
    double t_start2 = param_b.start_point.t;
    double w_end2   = param_b.end_point.w;
    double t_end2   = param_b.end_point.t;
    
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
      
      if(compatible) print(larlight::MSG::NORMAL,__FUNCTION__,Form(" Compatible in distance (%g)",shortest_distance2));
      else print(larlight::MSG::NORMAL,__FUNCTION__,Form(" NOT compatible in distance (%g).",shortest_distance2));
      
    }
    
    return compatible;
    
  }
  
  double ClusterMergeAlgNew::ShortestDistanceSquared(double point_x, double point_y, 
						     double start_x, double start_y,
						     double end_x,   double end_y  ) const {
    
    //This code finds the shortest distance between a point and a line segment.    
    //code based off sample from 
    //http://stackoverflow.com/questions/849211/shortest-distance-between-a-point-and-a-line-segment
    
    double distance_squared = -1;
    
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

      print(larlight::MSG::WARNING,__FUNCTION__,msg.str());

      return (pow((point_x - start_x),2) + pow((point_y - start_y),2));

    }

    
    double t = ( (point_x - start_x)*(end_x - start_x) + (point_y - start_y)*(end_y - start_y) ) / length_squared;
    
    if(t<0.0) distance_squared = pow((point_x - start_x), 2) + pow((point_y - start_y), 2);
    
    else if (t>1.0) distance_squared = pow((point_x - end_x), 2) + pow(point_y - end_y, 2);
    
    else distance_squared = pow((point_x - (start_x + t*(end_x - start_x))), 2) + pow((point_y - (start_y + t*(end_y - start_y))),2);
	   
    return distance_squared;
    

  }


  bool ClusterMergeAlgNew::Angle2DCompatibility(const cluster::cluster_params &param_a,
						const cluster::cluster_params &param_b)
  {
    
    //pretty sure we don't need conversion factors here. already in cm/cm units
    double angle1 = param_a.angle_2d;
    double angle2 = param_b.angle_2d;

    bool compatible = false;
    
    compatible = ( abs(angle1-angle2)     < _max_allowed_2D_angle_diff );
    //if you want to allow a 180 degree ambiguity (to accept if one cluster was reco'd backwards)
    /*
		   abs(angle1-angle2-180) < _max_allowed_2D_angle_diff ||
		   abs(angle1-angle2+180) < _max_allowed_2D_angle_diff   );
    */
    
    if(_verbose) {
      
      if(compatible) print(larlight::MSG::NORMAL,__FUNCTION__," Compatible in angle.");
      else print(larlight::MSG::NORMAL,__FUNCTION__," NOT compatible in angle.");
      
    }
    
    return compatible;

  }









}

#endif
