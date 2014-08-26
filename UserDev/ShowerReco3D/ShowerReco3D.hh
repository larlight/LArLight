/**
 * \file ShowerReco3D.hh
 *
 * \ingroup ShowerReco3D
 * 
 * \brief Class def header for a class ShowerReco3D
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerReco3D

    @{*/

#ifndef SHOWERRECO3D_HH
#define SHOWERRECO3D_HH

#include "ana_base.hh"
#include "DataFormat-TypeDef.hh"
#include "ShowerRecoAlgBase.hh"
#include "CRUHelper.hh"
#include "CMatchManager.hh"

namespace larlight {
  /**
     \class ShowerReco3D
     User custom analysis class made by kazuhiro
   */
  class ShowerReco3D : public ana_base{
  
  public:

    /// Default constructor
    ShowerReco3D();

    /// Default destructor
    virtual ~ShowerReco3D(){ delete fMatchMgr; }

    /** IMPLEMENT in ShowerReco3D.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in ShowerReco3D.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in ShowerReco3D.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    /// Attach algo
    void SetShowerAlgo(::showerreco::ShowerRecoAlgBase *alg) { fShowerAlgo = alg;}

    /// hack! remove me later
    storage_manager* GetCurrentData() {return _mgr;};
    
    /// Function to set an input cluster type to work with
    void SetClusterType(DATA::DATA_TYPE type) { fClusterType = type; }

    /// Getter for MatchManager instance, to attach algorithms
    ::cmtool::CMatchManager& GetManager() { return *fMatchMgr; }

  protected:

    /// Type of input clusters
    DATA::DATA_TYPE fClusterType;

    /// CRUHelper converts framework dependent data product to PxUtil
    ::cluster::CRUHelper fCRUHelper;

    /// Shower reconstruction algorithm
    ::showerreco::ShowerRecoAlgBase *fShowerAlgo;

    /// Cluster matching code
    ::cmtool::CMatchManager *fMatchMgr;

    /// hack! remove me later
    storage_manager* _mgr;
  };
}
#endif

/** @} */ // end of doxygen group 
