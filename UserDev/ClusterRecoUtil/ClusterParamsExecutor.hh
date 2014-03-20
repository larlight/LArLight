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

namespace cluster {
  
  /**
     \class ClusterParamsExecutor
     User defined class ClusterParamsExecutor ... these comments are used to generate
     doxygen documentation!
  */
  class ClusterParamsExecutor : public larlight::ana_base {
    
  public:
    
    /// Default constructor
    ClusterParamsExecutor(){};
    
    /// Default destructor
    virtual ~ClusterParamsExecutor(){};
    
    virtual bool initialize();
    
    virtual bool finalize();
    
    virtual bool analyze(larlight::storage_manager* storage);
    
  };

}

#endif
/** @} */ // end of doxygen group 

