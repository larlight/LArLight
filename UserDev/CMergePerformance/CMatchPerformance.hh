/**
 * \file CMatchPerformance.hh
 *
 * \ingroup CMergePerformance
 * 
 * \brief Class def header for a class CMatchPerformance
 *
 * @author kazuhiro
 */

/** \addtogroup CMergePerformance

    @{*/

#ifndef CMATCHPERFORMANCE_HH
#define CMATCHPERFORMANCE_HH

#include "ana_base.hh"
#include "McshowerLookback.hh"
#include "CMatchManager.hh"
#include "CRUHelper.hh"

namespace larlight {
  /**
     \class CMatchPerformance
     User custom analysis class made by kazuhiro
   */
  class CMatchPerformance : public ana_base{
  
  public:

    /// Default constructor
    CMatchPerformance(){ 
      _name="CMatchPerformance";
      _fout=0; 
      hMatchChargeEff_MC=0; 
      hMatchNumEff_MC=0;
      _cluster_type = DATA::Cluster;
    };

    /// Default destructor
    virtual ~CMatchPerformance(){};

    /** IMPLEMENT in CMatchPerformance.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in CMatchPerformance.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in CMatchPerformance.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    ::cmtool::CMatchManager& GetManager() { return fMgr; }

    McshowerLookback& GetBTAlgo() { return fBTAlgo; }

  protected:

    DATA::DATA_TYPE _cluster_type;

    ::cluster::CRUHelper fHelper;

    ::cmtool::CMatchManager fMgr;

    McshowerLookback fBTAlgo;

    TH1D* hMatchChargeEff_MC;

    TH1D* hMatchNumEff_MC;

    std::vector<UChar_t> _view_to_plane;
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
