/**
 * \file ClusterMergeAlgo.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class ClusterMergeAlgo
 *
 * @author davidkaleko
 */

/** \addtogroup Analysis

    @{*/

#ifndef CLUSTERMERGEALGO_HH
#define CLUSTERMERGEALGO_HH

#include "ana_base.hh"
#include "ana_info_struct.hh"
#include <cmath> //used for std::abs() absolute value
#include <sstream>

namespace larlight {

  /**
     \class ClusterMergeAlgo
     User custom analysis class made by davidkaleko
   */
  class ClusterMergeAlgo : public ana_base{

  private:
    /// Default constructor
    ClusterMergeAlgo();
  
  public:

    /// Public pointer getter
    static ClusterMergeAlgo* get() {

      if(!_me) _me = new ClusterMergeAlgo;

      return _me;
    }
      

    /// Default destructor
    virtual ~ClusterMergeAlgo(){};

    /**
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /**
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /**
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void VerboseMode(bool on) { _verbose = on; }

    void SetAngleCut(double angle) { _max_allowed_2D_angle_diff = angle; }

    void SetSquaredDistanceCut(double d) { _max_2D_dist2 = d; }

    void SetWire2Cm(double f) { _wire_2_cm = f; }
    
    void SetTime2Cm(double f) { _time_2_cm = f; }

    void ClearEventInfo();

    void AppendClusterInfo(const cluster &cl);

    void ProcessMergeAlgo();

    const std::vector<std::vector<unsigned int> > GetClusterSets () const {return _cluster_sets_v;};

    bool CompareClusters(cluster_merge_info clus_info_A,
			 cluster_merge_info clus_info_B);

    /**
       Function to compare the 2D angles of two clusters and return true if they are
       within the maximum allowed parameter. Includes shifting by 180 for backwards clusters.
    */
    bool Angle2DCompatibility(const cluster_merge_info &clus_info_A,
			      const cluster_merge_info &clus_info_B) const;
    
    bool ShortestDistanceCompatibility(const cluster_merge_info &clus_info_A,
				       const cluster_merge_info &clus_info_B) const;

    double ShortestDistanceSquared(double point_x, double point_y, 
				   double start_x, double start_y,
				   double end_x,   double end_y  ) const;

    /**
       Function to print to screen a specific cluser's info
       from ClusterPrepAna module. Used for debugging.
    */
    void PrintClusterVars(cluster_merge_info clus_info) const;

    /** 
	Utility function to check if an index is already somewhere inside of _cluster_sets_v vector
	returns the location of the element vector in _cluster_sets_v that contains the index
	and returns -1 if the index is not in _cluster_sets_v anywhere
    */
    int isInClusterSets(unsigned int index) const;
    
  protected:

    void ClearOutputInfo();

    void ClearInputInfo();

    /// Function to push stuff back into the _cluster_sets_v vector in the right structure
    void BuildClusterSets(cluster_merge_info clus_info_A,
			  cluster_merge_info clus_info_B);

    /**
       Function to loop through _cluster_sets_v and add in the un-mergable clusters
       individually, because BuildClusterSets wouldn't have included them anywhere
    */
    void FinalizeClusterSets();

    int AppendToClusterSets(unsigned int cluster_index, int merged_index=-1);

  protected:

    bool _verbose;

    std::vector<int> _cluster_merged_index;
    
    std::vector<std::vector<unsigned int> > _cluster_sets_v;
    /// Vectors to store output of ClusterPrepAna module
    std::vector<larlight::cluster_merge_info> _u_clusters;
    std::vector<larlight::cluster_merge_info> _v_clusters;
    std::vector<larlight::cluster_merge_info> _w_clusters;

    static ClusterMergeAlgo* _me;

    double _wire_2_cm;
    double _time_2_cm;
    double _max_allowed_2D_angle_diff; //in degrees
    double _max_2D_dist2;              //in ((wirenumber^2)+(seconds^2))^0.5
    
  };
}
#endif

/** @} */ // end of doxygen group 
