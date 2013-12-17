#ifndef ANA_INFO_STRUCT_HH
#define ANA_INFO_STRUCT_HH

#include "constants.hh"

namespace larlight {

  /**
     \struct cluster_ana_info
     A utility struct for cluster-wise analysis information.
   */
  struct cluster_ana_info {

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
    //View of this specific cluster
    GEO::View_t view;
    //Unique (to the event) ID number for this cluster
    size_t cluster_index;

    cluster_ana_info(){
      start_wire = start_time = -1;
      end_wire = end_time = -1;
      angle = 0;
      start_time_max = start_time_min = -1;
      peak_time_max = peak_time_min = -1;
      end_time_max = end_time_min = -1;
      sum_charge = 0;
      view = GEO::kUnknown;
      cluster_index = 0xffff;
    };

  };

}
#endif
