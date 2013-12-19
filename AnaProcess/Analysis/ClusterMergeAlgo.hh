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
#include "ClusterAnaPrep.hh"
#include <cmath> //used for std::abs() absolute value


namespace larlight {
  /**
     \class ClusterMergeAlgo
     User custom analysis class made by davidkaleko
   */
  class ClusterMergeAlgo : public ana_base{

  private:
    /// Default constructor
    ClusterMergeAlgo(){ _name="ClusterMergeAlgo"; _fout=0;};
  
  public:

    /// Public pointer getter
    static ClusterMergeAlgo* get() {

      if(!_me) _me = new ClusterMergeAlgo;

      return _me;
    }
      

    /// Default destructor
    virtual ~ClusterMergeAlgo(){};

    /** IMPLEMENT in ClusterMergeAlgo.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in ClusterMergeAlgo.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in ClusterMergeAlgo.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    bool CompareClusters(cluster_ana_info clus_info_A,
			 cluster_ana_info clus_info_B);

    /// Function to compare the 2D angles of two clusters and return true if they are
    /// within the maximum allowed parameter. Includes shifting by 180 for backwards clusters.
    bool Angle2DCompatibility(double angle1, double angle2, double max_allowed_2D_angle_diff);
    
    bool ShortestDistanceCompatibility(double t_start1, double w_start1, double t_end1, double w_end1,
				       double t_start2, double w_start2, double t_end2, double w_end2,
				       double max_2D_dist2);
    

    /// Function to print to screen a specific cluser's info
    /// from ClusterPrepAna module. Used for debugging.
    void PrintClusterVars(cluster_ana_info clus_info);

    const std::vector<std::vector<unsigned int> > GetClusterSets () const {return _cluster_sets_v;};

    /// Function to push stuff back into the _cluster_sets_v vector in the right structure
    void BuildClusterSets(cluster_ana_info clus_info_A,
			  cluster_ana_info clus_info_B);

    /// Function to loop through _cluster_sets_v and manually add in the un-mergable clusters
    /// individually, because BuildClusterSets wouldn't have included them anywhere
    void FinalizeClusterSets(const std::vector<cluster_ana_info> *u_clusters,
			     const std::vector<cluster_ana_info> *v_clusters,
			     const std::vector<cluster_ana_info> *w_clusters);


    /// Utility function to check if an index is already somewhere inside of _cluster_sets_v vector
    //returns the location of the element vector in _cluster_sets_v that contains the index
    //and returns -1 if the index is not in _cluster_sets_v anywhere
    int isInClusterSets(int index);

  private:
    
    std::vector<std::vector<unsigned int> > _cluster_sets_v;
    /// Vectors to store output of ClusterPrepAna module
    const std::vector<cluster_ana_info> *u_clusters;
    const std::vector<cluster_ana_info> *v_clusters;
    const std::vector<cluster_ana_info> *w_clusters;

    static ClusterMergeAlgo* _me;

    double max_allowed_2D_angle_diff = -1; //in degrees
    double max_2D_dist2 = -1; //in ((wirenumber^2)+(seconds^2))^0.5
    
  };
}
#endif

/** @} */ // end of doxygen group 
