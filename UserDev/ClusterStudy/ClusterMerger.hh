/**
 * \file ClusterMerger.hh
 *
 * \ingroup TestEff
 * 
 * \brief Class def header for a class ClusterMerger
 *
 * @author kazuhiro
 */

/** \addtogroup TestEff

    @{*/

#ifndef CLUSTERMERGER_HH
#define CLUSTERMERGER_HH

#include "ana_base.hh"
#include "CMergeManager.hh"

namespace larlight {
  /**
     \class ClusterMerger
     User custom analysis class made by kazuhiro
   */
  class ClusterMerger : public ana_base{
  
  public:

    /// Default constructor
    ClusterMerger(){ _name="ClusterMerger"; _fout=0; _cluster_type = DATA::FuzzyCluster;}

    /// Default destructor
    virtual ~ClusterMerger(){}

    /** IMPLEMENT in ClusterMerger.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in ClusterMerger.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in ClusterMerger.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void SetClusterType(DATA::DATA_TYPE type) {_cluster_type = type;}

    ::cluster::CMergeManager& GetManager() { return _mgr; }

    protected:

    ::cluster::CMergeManager _mgr;

    DATA::DATA_TYPE _cluster_type;

  };
}
#endif

//**************************************************************************
// 
// For Analysis framework documentation, read Manual.pdf here:
//
// http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
//
//**************************************************************************

/** @} */ // end of doxygen group 
