/**
 * \file TestEff.hh
 *
 * \ingroup TestEff
 * 
 * \brief Class def header for a class TestEff
 *
 * @author andrzej
 */

/** \addtogroup TestEff

    @{*/

#ifndef TESTEFF_HH
#define TESTEFF_HH

#include <TH1D.h>

#include "ana_base.hh"
#include "ClusterParamsAlgNew.hh"


namespace larlight {
  /**
     \class TestEff
     User custom analysis class made by andrzej
   */
  class TestEff : public ana_base{
  
  public:

    /// Default constructor
    TestEff() : ana_base() { _name="TestEff"; }

    /// Default destructor
    virtual ~TestEff(){ h1=0; };

    /** IMPLEMENT in TestEff.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in TestEff.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in TestEff.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    protected:

    ::cluster::ClusterParamsAlgNew  fCPAlg; 

    larutil::GeometryUtilities  *fGSer;
    /// Example histogram to be saved in analysis root file (separate from data product)
    TH1D* h1;
   
    TH1D* sumCharge;
    TH1D* meanCharge;
    TH1D* meanX;
    TH1D* meanY; 
    TH1D* rmsX;
    TH1D* rmsY;
    TH1D* chargeWgtX; 
    TH1D* chargeWgtY;
    TH1D* clusterAngle2d; 
    TH1D* angle2d;
    TH1D* openAngle, *openAngleChargeWgt;
    TH1D* closeAngle, *closeAngleChargeWgt; 
   
    TH1D* hitDensity1d ;
    TH1D* hitDensity2d ;
    TH1D* modifiedHitDens ;
    



    TH1D *showerness, *trackness ;

    TH1D *recostartwire, *recostarttime;
    TH1D *recostartwirenodir, *recostarttimenodir;
    
   TH1D *reco2Dangle, *reco2DangleGood;
   TH1D *recoDir;
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
