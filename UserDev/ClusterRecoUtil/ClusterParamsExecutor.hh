/**
 * \file ClusterParamsExecutor.hh
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class ClusterParamsExecutor
 *
 * @author kazuhiro
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef CLUSTERPARAMSEXECUTOR_HH
#define CLUSTERPARAMSEXECUTOR_HH

#include "ana_base.hh"
#include "ClusterParamsAlgNew.hh"
#include "Geometry.hh"
#include <TH2D.h>
#include <TF1.h>

namespace cluster {
  
  /**
     \class ClusterParamsExecutor
     User defined class ClusterParamsExecutor ... these comments are used to generate
     doxygen documentation!
  */
  class ClusterParamsExecutor : public ClusterParamsAlgNew {
    
  public:
    
    /// Default constructor
    ClusterParamsExecutor();
    
    /// Default destructor
    virtual ~ClusterParamsExecutor(){};
    
    void LoadCluster(const larlight::cluster &i_cluster,
		     const larlight::event_hit *hits);

    void SetUseHitBlurring(bool flag) { _useHitBlurring = flag; }

    TH2D* GetHitView()
    { return hCurrentHit;}
    
  protected:
    std::vector<const larlight::hit*> cluster_hits;
    TH2D* hCurrentHit;
    std::vector<TH2D*> hHit;

    bool _useHitBlurring;
    TF1* _blurFunction;
  };


}

#endif
/** @} */ // end of doxygen group 

