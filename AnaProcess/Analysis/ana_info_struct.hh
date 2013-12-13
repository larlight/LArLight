#ifndef ANA_INFO_STRUCT_HH
#define ANA_INFO_STRUCT_HH

#include "constants.hh"

namespace larlight {

  /**
     \struct cluster_ana_info
     A utility struct for cluster-wise analysis information.
   */
  struct cluster_ana_info {

    double start_wire;
    double start_time;
    double end_wire;
    double end_time;
    double angle;

    double start_time_max;
    double start_time_min;
    double peak_time_max;
    double peak_time_min;
    double end_time_max;
    double end_time_min;
    double sum_charge;
    GEO::View_t view;
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
