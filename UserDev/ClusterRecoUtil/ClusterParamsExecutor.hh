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
#include "ClusterParamsAlg.hh"
#include "Geometry.hh"
#include <TH2D.h>
#include <TF1.h>

namespace cluster {
  
  /**
     \class ClusterParamsExecutor
     User defined class ClusterParamsExecutor ... these comments are used to generate
     doxygen documentation!
  */
  class ClusterParamsExecutor : public ClusterParamsAlg {
    
  public:
    
    /// Default constructor
    ClusterParamsExecutor();

    ClusterParamsExecutor(const ClusterParamsExecutor& orig)
      : ::cluster::ClusterParamsAlg::ClusterParamsAlg(orig),
        hCurrentHit(nullptr),
        hHit(),
        _useHitBlurring(orig._useHitBlurring),
        _blurFunction(orig._blurFunction)
    {}
    
    /// Default destructor
    virtual ~ClusterParamsExecutor(){};
    
    int LoadCluster(const larlight::cluster &i_cluster,
		     const larlight::event_hit *hits);

    int LoadAllHits(const larlight::event_hit *hits, const UChar_t plane_id);

    void SetUseHitBlurring(bool flag) { _useHitBlurring = flag; }

    TH2D* GetHitView()
    { return hCurrentHit;}
    
  protected:

    int Execute(Int_t event_id, Int_t cluster_id, UChar_t plane_id);
    std::vector<const larlight::hit*> cluster_hits;
    TH2D* hCurrentHit;
    std::vector<TH2D*> hHit;

    bool _useHitBlurring;
    TF1* _blurFunction;
  };


}

#endif
/** @} */ // end of doxygen group 

