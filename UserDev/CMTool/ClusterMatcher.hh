/**
 * \file ClusterMatcher.hh
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class ClusterMatcher
 *
 * @author kazuhiro
 */

/** \addtogroup CMTool

    @{*/

#ifndef CLUSTERMATCHER_HH
#define CLUSTERMATCHER_HH

#include "ana_base.hh"
#include "CMatchManager.hh"
#include "CRUHelper.hh"
namespace larlight {
  /**
     \class ClusterMatcher
     User custom analysis class made by kazuhiro
   */
  class ClusterMatcher : public ana_base{
  
  public:

    /// Default constructor
    ClusterMatcher(){ _name="ClusterMatcher"; _fout=0; _cluster_type = DATA::FuzzyCluster; _write_output=false;}

    /// Default destructor
    virtual ~ClusterMatcher(){}

    /** IMPLEMENT in ClusterMatcher.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in ClusterMatcher.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in ClusterMatcher.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void SetClusterType(DATA::DATA_TYPE type) {_cluster_type = type;}

    ::cmtool::CMatchManager& GetManager() { return _mgr; }

    void SaveOutputCluster(bool doit=true) { _write_output = doit; }
    
  protected:

    bool _write_output;

    ::cmtool::CMatchManager _mgr;

    DATA::DATA_TYPE _cluster_type;

    CRUHelper _cru_helper;

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
