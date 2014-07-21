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
#include "ShowerRecoAlg.hh"
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
    virtual ~ShowerReco3D(){}

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

    void SetClusterType(DATA::DATA_TYPE type) { fClusterType = type; }

    ::cmtool::CMatchManager& GetManager() { return fMatchMgr; }

  protected:

    /// Type of input clusters
    DATA::DATA_TYPE fClusterType;

    /// CRUHelper converts framework dependent data product to PxUtil
    ::cluster::CRUHelper fCRUHelper;

    /// Shower reconstruction algorithm
    ::shower::ShowerRecoAlg fShowerAlgo;

    /// Cluster matching code
    ::cmtool::CMatchManager fMatchMgr;
    
  };
}
#endif

/** @} */ // end of doxygen group 
