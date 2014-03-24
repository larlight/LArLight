#ifndef UTIL_CLUSTERPARAMS_H
#define UTIL_CLUSTERPARAMS_H

#include <ostream>

namespace cluster{

  class cluster_params
  {
  public:
    cluster_params(){
      //Initialize all values.
      Clear();
    } 

    ~cluster_params(){};

    /**
       This vector holds the pointer to hits. 
       This should be used for computation for speed.
    */
    std::vector<const larutil::PxHit*> fHitPtrVector;

    larutil::PxPoint start_point;      ///< start point 
    larutil::PxPoint end_point;        ///< end point 

    double sum_charge;                 ///< Sum charge of hits in ADC
    double mean_charge;                ///< Mean (average) charge of hits in ADC
    double mean_x;                     ///< Mean of hits along x, peaks only
    double mean_y;                     ///< Mean of hits along y, peaks only
    double rms_x;                      ///< rms of hits along x (wires)
    double rms_y;                      ///< rms of hits along y, (time)
    double charge_wgt_x;               ///< Mean of hits along x, charge weighted
    double charge_wgt_y;               ///< Mean of hits along y, charge weighted
    double cluster_angle_2d;           ///< Linear best fit to high-charge hits in the cluster
    double angle_2d;                   ///< Angle of axis in wire/hit view
    double opening_angle;              ///< Width of angular distubtion wrt vertx
    double opening_angle_highcharge;   ///< Same for high charge 
    double closing_angle;              ///< Width of angular distubtion wrt endpoint
    double closing_angle_highcharge;   ///< Same for high charge 
    double eigenvalue_principal;       ///< the principal eigenvalue from PCA
    double eigenvalue_secondary;       ///< the secondary eigenvalue from PCA
    double verticalness;               ///< ???

    double length;
    double width;

    double hit_density_1D;
    double hit_density_2D;
    double multi_hit_wires;
    double N_Wires;
    double modified_hit_density;
    double N_Hits;

    /**
       +1 means shower is "forward - start wire < end wire"
       -1 is backwards, 0 is undecided
    */
    int    direction;

    double showerness;                 ///< got heart
    double trackness;                  ///< got soul                                       
    double offaxis_hits;               ///< got brain

    void Clear(){
      start_point.Clear();
      end_point.Clear();
      sum_charge                        = -999.999 ;
      mean_charge                       = -999.999 ;
      mean_x                            = -999.999 ;
      mean_y                            = -999.999 ;
      rms_x                             = -999.999 ;
      rms_y                             = -999.999 ;
      charge_wgt_x                      = -999.999 ;
      charge_wgt_y                      = -999.999 ;
      eigenvalue_principal              = -999.999 ;
      eigenvalue_secondary              = -999.999 ;
      multi_hit_wires                   = -999.999 ;
      N_Wires                           = -999.999 ;
      verticalness                      = -999.999 ;
      opening_angle                     = -999.999 ;
      opening_angle_highcharge          = -999.999 ;
      closing_angle                     = -999.999 ;
      closing_angle_highcharge          = -999.999 ;
      offaxis_hits                      = -999.999 ;
      modified_hit_density              = -999.999 ;    
      length                            = -999.999 ;
      width                             = -999.999 ;
      hit_density_1D                    = -999.999 ;
      hit_density_2D                    = -999.999 ;
      angle_2d                          = -999.999 ;
      cluster_angle_2d	                = -999.999 ;
      showerness                        = -999.999 ;
      trackness                         = -999.999 ;
      fHitPtrVector.clear();
    }

    void Report(std::ostream & os = std::cout){

      os << "ClusterParams report: \n"
	       << "\tstart_point: (" << start_point.w <<","<<start_point.t<<")\n"
	       << "\tend_point: (" << end_point.w <<","<<end_point.t<<")\n"
	       << "\tsum_charge: "<< sum_charge << "\n"
         << "\tmean_charge: " << mean_charge << "\n"
         << "\tmean_x: " << mean_x << "\n"
         << "\tmean_y: " << mean_y << "\n"
         << "\trms_x: " << rms_x << "\n"
         << "\trms_y: " << rms_y << "\n"
         << "\tcharge_wgt_x: " << charge_wgt_x << "\n"
         << "\tcharge_wgt_y: " << charge_wgt_y << "\n"
         << "\teigenvalue_principal: " << eigenvalue_principal << "\n"
         << "\teigenvalue_secondary: " << eigenvalue_secondary << "\n"
         << "\tmulti_hit_wires: " << multi_hit_wires << "\n"
         << "\tN_Wires: " << N_Wires << "\n"
         << "\tverticalness: " << verticalness << "\n"
         << "\topening_angle: " << opening_angle << "\n"
         << "\topening_angle_highcharge: " << opening_angle_highcharge << "\n"
         << "\tclosing_angle: " << closing_angle << "\n"
         << "\tclosing_angle_highcharge: " << closing_angle_highcharge << "\n"
         << "\toffaxis_hits: " << offaxis_hits << "\n"
         << "\tmodified_hit_density: " << modified_hit_density << "\n"
         << "\tlength: " << length << "\n"
         << "\twidth: " << width << "\n"
         << "\thit_density_1D: " << hit_density_1D << "\n"
         << "\thit_density_2D: " << hit_density_2D << "\n"
         << "\tangle_2d: " << angle_2d << "\n"
         << "\tcluster_angle_2d: " << cluster_angle_2d << "\n"
         << "\tshowerness: " << showerness << "\n"
         << "\ttrackness: " << trackness << "\n";
    }

  };
}

#endif
