/**
 * \file CRUHelper.hh
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class CRUHelper
 *
 * @author kazuhiro
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef CRUHELPER_HH
#define CRUHELPER_HH

#include <iostream>
#include "storage_manager.hh"
#include "ClusterParamsAlgNew.hh"

namespace cluster {

  /**
     \class CRUHelper
     This class bridges the framework and ClusterRecoUtil package by being an interface
     to the framework's data products. Its mere purpose is to generate ClusterParamsAlg
     instances from hit lists (fmwk data products).
  */
  class CRUHelper{
    
  public:
    
    /// Default constructor
    CRUHelper(){};
    
    /// Default destructor
    virtual ~CRUHelper(){};

    /// Generate: from 1 set of hits => 1 CPAN using indexes (association)
    void GenerateCPAN(const std::vector<unsigned int>& hit_index,
		      const larlight::event_hit* hits,
		      ClusterParamsAlgNew &cpan) const;
    
    /// Generate: CPAN vector from event storage by specifying cluster type
    void GenerateCPAN(larlight::storage_manager* storage,
		      const larlight::DATA::DATA_TYPE cluster_type,
		      std::vector<cluster::ClusterParamsAlgNew> &cpan_v) const;

    /// Generate: from 1 set of hits => 1 set of PxHits using indexes (association)
    void GeneratePxHit(const std::vector<unsigned int>& hit_index,
		       const larlight::event_hit* hits,
		       std::vector<larutil::PxHit> &pxhits) const;

    /// Generate: vector of PxHit sets from event storage by specifying cluster type
    void GeneratePxHit(larlight::storage_manager* storage,
		       const larlight::DATA::DATA_TYPE cluster_type,
		       std::vector<std::vector<larutil::PxHit> > &pxhits_v) const;
    
  };

}

#endif
/** @} */ // end of doxygen group 

