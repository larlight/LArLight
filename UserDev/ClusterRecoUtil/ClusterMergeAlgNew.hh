/**
 * \file ClusterMergeAlgNew.hh
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class ClusterMergeAlgNew
 *
 * @author kazuhiro
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef CLUSTERMERGEALGNEW_HH
#define CLUSTERMERGEALGNEW_HH

#include <iostream>
#include "LArUtil-TypeDef.hh"
#include "ClusterParams.hh"

namespace cluster {
  
  /**
     \class ClusterMergeAlgNew
     User defined class ClusterMergeAlgNew ... these comments are used to generate
     doxygen documentation!
  */
  class ClusterMergeAlgNew{

    enum MergeAlg_t {
      kStartPoint=0,
      kPolygonCollision,
      kAlgoMax
    };
    
  public:
    
    /// Default constructor
    ClusterMergeAlgNew();
    
    /// Default destructor
    virtual ~ClusterMergeAlgNew(){};
    
    bool Merge(const cluster::cluster_params &param_a, 
	       const cluster::cluster_params &param_b);

  protected:

    bool TestStartPoint(const cluster::cluster_params &param_a, 
			const cluster::cluster_params &param_b);

    bool TestPolygonCollision(const cluster::cluster_params &param_a, 
			      const cluster::cluster_params &param_b);

    std::vector<bool> fAlgoSwitch;

  };
}

#endif
/** @} */ // end of doxygen group 

