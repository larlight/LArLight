#ifndef CLUSTER_INFO_STRUCT_HH
#define CLUSTER_INFO_STRUCT_HH

#include "Base-TypeDef.hh"

namespace larreco{

  /**
     \struct cluster_ana_info
     A utility struct for cluster-wise analysis information for matching     
   */
  struct cluster_ana_info {

    //View of this specific cluster
    larlight::GEO::View_t view;
    //Unique (to the event) ID number for this cluster
    size_t cluster_index;

    //Wire number corresponding to calculated cluster start point
    double start_wire;
    //Time corresponding to calculated cluster start point
    double start_time;
    //Wire number corresponding to calculated cluster end point
    double end_wire;
    //Time corresponding to calculated cluster start point
    double end_time;
    //Calculated 2D cluster ANGLE. 
    //This is in degrees, from cm/cm slope ... specifically, it's calculated by 
    //multiplying (end_time - start_time) by drift velocity and
    //converting (end_wire - start_wire) into units of centimeters by using 
    //geometry (wire spacing).
    double angle;

    //Largest hit start-time for all hits in the cluster
    double start_time_max;
    //Smallest hit start-time for all hits in the cluster
    double start_time_min;
    //Largest hit peak-time for all hits in the cluster
    double peak_time_max;
    //Smallest hit peak-time for all hits in the cluster
    double peak_time_min;
    //Largest hit end-time for all hits in the cluster
    double end_time_max;
    //Smallest hit end-time for all hits in the cluster
    double end_time_min;
    //Total charge of all hits in the cluster
    double sum_charge;


    cluster_ana_info(){
      start_wire = start_time = -1;
      end_wire = end_time = -1;
      angle = 0;
      start_time_max = start_time_min = -1;
      peak_time_max = peak_time_min = -1;
      end_time_max = end_time_min = -1;
      sum_charge = 0;
      view = larlight::GEO::kUnknown;
      cluster_index = 0xffff;

    };

  };


  /**
     \struct cluster_merge_info
     A utility struct for cluster-wise analysis information for merging
   */
  struct cluster_merge_info {

    unsigned int cluster_index; ///< Input cluster ID
    larlight::GEO::View_t  view;          ///< Wire plane ID

    double start_wire;          ///< Vertex wire
    double start_time;          ///< Vertex time
    double end_wire;            ///< End point wire
    double end_time;            ///< End point time

    double start_wire_err;      ///< Vertex wire error
    double start_time_err;      ///< Vertex time error
    double end_wire_err;        ///< End point wire error
    double end_time_err;        ///< End point time error

    double angle;               ///< 2D angle

    int n_hits;                 ///< Total number of hits in cluster

    double q_total;             ///< Total charge of all hits in cluster

    /// Default constructor
    cluster_merge_info() {
      
      cluster_index = 0xffffffff;
      view = larlight::GEO::kUnknown;
      start_wire = start_time = end_wire = end_time = -1;
      start_wire_err = start_time_err = end_wire_err =end_time_err = -1;
      n_hits = -1;
      q_total = 0;

    };

  };

}
#endif
