/**
 * \file ClusterMergeAlg.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class ClusterMergeAlg
 *
 * @author davidkaleko
 */

/** \addtogroup Analysis

    @{*/

#ifndef CLUSTERMERGEALG_HH
#define CLUSTERMERGEALG_HH

#include "ana_base.hh"
#include "ana_info_struct.hh"
#include <cmath> //used for std::abs() absolute value
#include <sstream>

namespace larlight {

  /**
     \class ClusterMergeAlg
     User custom analysis class made by davidkaleko
   */
  class ClusterMergeAlg : public ana_base{

  public:

    /// Default constructor
    ClusterMergeAlg();

    /// Default destructor
    virtual ~ClusterMergeAlg(){};

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

    /// Method to set verbose mode
    void VerboseMode(bool on) { _verbose = on; }

    /// Method to report current configuration
    void ReportConfig() const;

    /// Method to set cut value in degrees for angle compatibility test
    void SetAngleCut(double angle) { _max_allowed_2D_angle_diff = angle; }

    /// Method to set cut value in cm^2 for distance compatibility test
    void SetSquaredDistanceCut(double d) { _max_2D_dist2 = d; }

    /// Method to clear event-wise information (both input cluster info & output merged cluster sets)
    void ClearEventInfo();

    /// Method to add a cluster information for processing
    void AppendClusterInfo(const cluster &cl,
			   const std::vector<larlight::hit> &in_hit_v);

    /**
       Method to execute the algorithm. All parameter configuration + adding input cluster information
       should be done before calling this function. This function generate a resulting set of cluster IDs
       for merging, which can be accessed through GetClusterSets() function.
    */
    void ProcessMergeAlg();

    /// Method to extract resulting set of cluster IDs for merging computed by ProcessMergeAlg() function.
    const std::vector<std::vector<unsigned int> > GetClusterSets () const {return _cluster_sets_v;};

    /// Method to compare a compatibility between two clusters
    bool CompareClusters(const cluster_merge_info &clus_info_A,
			 const cluster_merge_info &clus_info_B);

    /**
       Function to compare the 2D angles of two clusters and return true if they are
       within the maximum allowed parameter. Includes shifting by 180 for backwards clusters.
       This is called within CompareClusters().
    */
    bool Angle2DCompatibility(const cluster_merge_info &clus_info_A,
			      const cluster_merge_info &clus_info_B) const;

    /**
       Function to compare the 2D distance of two clusters and return true if they are
       within the maximum allowed distance.The distance-squared is computed by another
       function, ShortestDistanceSquared().
       This is called within CompareClusters().
     */
    bool ShortestDistanceCompatibility(const cluster_merge_info &clus_info_A,
				       const cluster_merge_info &clus_info_B) const;

    /**
       Function to compute a distance between a 2D point (point_x, point_y) to a 2D finite line segment
       (start_x, start_y) => (end_x, end_y).
     */
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

    /// Method to set a conversion factor from wire to cm scale
    void SetWire2Cm(double f) { _wire_2_cm = f; }

    /// Method to set a conversion factor from time to cm scale
    void SetTime2Cm(double f) { _time_2_cm = f; }

    /// Method to clear output merged cluster sets (_cluster_sets_v)
    void ClearOutputInfo();

    /// Method to clear input cluster information
    void ClearInputInfo();

    /// Method to clear quality control TTree variables
    void ClearTTreeInfo();

    /// Method to prepare quality control TTree
    void PrepareTTree();

    /// Method to prepare detector parameters
    void PrepareDetParams();    

    /// Method to append hit-array-info per cluster ... called by AppendClusterInfo() function.
    void AppendHitInfo(cluster_merge_info ci, const std::vector<larlight::hit> &in_hit_v);

    /**
       For a given pair of clusters, this function calls CompareClusters() and append to the resulting
       merged cluster sets (_cluster_sets_v) by calling AppendToClusterSets() when they are compatible.
    */
    void BuildClusterSets(const cluster_merge_info &clus_info_A,
			  const cluster_merge_info &clus_info_B);

    /**
       Function to loop through _cluster_sets_v and add in the un-mergable clusters
       individually, because BuildClusterSets wouldn't have included them anywhere
    */
    void FinalizeClusterSets();

    /// A function to add a cluster to a merged sets (_cluster_sets_v)
    int AppendToClusterSets(unsigned int cluster_index, int merged_index=-1);

  protected:

    bool _verbose; ///< Verbose mode boolean
    bool _det_params_prepared; ///< Boolean to keep track of detector parameter preparation
    TTree* _merge_tree;        ///< Quality Control TTree pointer

    /**
       Book-keeping vector which length is same as input cluster array's length.
       The stored value is the merged cluster sets' index (_cluster_sets_v).
       For instance, given 5 clusters (0, 1, 2, 3, 4) as an input among which
       1,2,3 are to be merged. _cluster_merged_index may hold contents like
       [1,0,0,0,2] when _cluster_sets_v contents are [[1,2,3],[0],[4]].
     */
    std::vector<int> _cluster_merged_index;

    /**
       The result container of ProcessMergeAlg() function.
       The structure is such that the inner vector holds the cluster IDs to be merged into one cluster.
       Naturally we expect multiple merged clusters, hence it's a vector of vector.
    */
    std::vector<std::vector<unsigned int> > _cluster_sets_v;

    std::vector<larlight::cluster_merge_info> _u_clusters; ///< Input U-plane clusters' information 
    std::vector<larlight::cluster_merge_info> _v_clusters; ///< Input V-plane clusters' information
    std::vector<larlight::cluster_merge_info> _w_clusters; ///< Input W-plane clusters' information

    double _wire_2_cm; ///< Conversion factor from wire number to cm scale
    double _time_2_cm; ///< Conversion factor from time to cm scale
    double _max_allowed_2D_angle_diff; //in degrees
    double _max_2D_dist2;              //in ((wirenumber^2)+(seconds^2))^0.5
    
  };
}
#endif

/** @} */ // end of doxygen group 
