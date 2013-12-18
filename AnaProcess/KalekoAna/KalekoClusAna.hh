/**
 * \file KalekoClusAna.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class KalekoClusAna
 *
 * @author davidkaleko
 */

/** \addtogroup Analysis

    @{*/

#ifndef KALEKOCLUSANA_HH
#define KALEKOCLUSANA_HH

#include "ana_base.hh"
#include "ClusterAnaPrep.hh"
#include <cmath> //used for std::abs() absolute value

namespace kaleko {
  /**
     \class KalekoClusAna
     User custom analysis class made by davidkaleko
   */
  class KalekoClusAna : public larlight::ana_base{
  
  public:

    /// Default constructor
    KalekoClusAna(){ _name="KalekoClusAna"; _fout=0;};

    /// Default destructor
    virtual ~KalekoClusAna(){};

    /** IMPLEMENT in KalekoClusAna.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in KalekoClusAna.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(larlight::storage_manager* storage);

    /** IMPLEMENT in KalekoClusAna.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    bool CompareClusters(larlight::cluster_ana_info clus_info_A,
			 larlight::cluster_ana_info clus_info_B);

    /// Function to compare the 2D angles of two clusters and return true if they are
    /// within the maximum allowed parameter. Includes shifting by 180 for backwards clusters.
    bool Angle2DCompatibility(double angle1, double angle2, double max_allowed_2D_angle_diff);
    
    bool StartEnd2DCompatibility(double t_start1, double w_start1, double t_end1, double w_end1,
				 double t_start2, double w_start2, double t_end2, double w_end2,
				 double max_allowed_2D_startendpt_diff);


    /// Function to print to screen a specific cluser's info
    /// from ClusterPrepAna module. Used for debugging.
    void PrintClusterVars(larlight::cluster_ana_info clus_info);

    
  protected:
    
    /// Vectors to store output of ClusterPrepAna module
    const std::vector<larlight::cluster_ana_info> *u_clusters;
    const std::vector<larlight::cluster_ana_info> *v_clusters;
    const std::vector<larlight::cluster_ana_info> *w_clusters;

    double max_allowed_2D_angle_diff = -1; //in degrees
    double max_allowed_2D_startendpt_diff = -1; //in ((wirenumber^2)+(seconds^2))^0.5
    
  };
}
#endif

/** @} */ // end of doxygen group 
