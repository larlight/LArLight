/**
 * \file RecoShowerQuality.hh
 *
 * \ingroup RecoShowerQuality
 * 
 * \brief Class def header for a class RecoShowerQuality
 *
 * @author georgia
 */

/** \addtogroup RecoShowerQuality

    @{*/

#ifndef RECOSHOWERQUALITY_HH
#define RECOSHOWERQUALITY_HH

#include "ana_base.hh"
#include <TH1D.h>
#include <map>
#include <vector>
#include <math.h>

namespace larlight {
  /**
     \class RecoShowerQuality
     User custom analysis class made by georgia
   */
  class RecoShowerQuality : public ana_base{
  
  public:

    /// Default constructor
    RecoShowerQuality() : ana_base() { _name="RecoShowerQuality"; }

    /// Default destructor
    virtual ~RecoShowerQuality(){};

    /** IMPLEMENT in RecoShowerQuality.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in RecoShowerQuality.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in RecoShowerQuality.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    protected:

    TH1D* hMCShowerMulti;
    TH1D* hMCShowerMotherMulti;
    TH1D* hMCElectronMotherMulti;
    TH1D* hMCElectronMom;
    TH1D* hMCOtherMom;
    TH1D* hMCElectronEnergy;
    TH1D* hMCDepositedEnergy;
    TH1D* hMCShowerTheta;
    TH1D* hMCShowerPhi;

    TH1D* hRecoShowerMulti;
    TH1D* hRecoShowerCharge;
    TH1D* hRecoShowerEn;
    TH1D* hRecoShowerPhi;
    TH1D* hRecoShowerTheta;

    TH1D* hShowerDirDiff;
    TH1D* hShowerPhiDiff;
    TH1D* hShowerThetaDiff;
    TH1D* hShowerPhiAbsDiff;
    TH1D* hShowerThetaAbsDiff;
    TH1D* hShowerDCosXDiff;
    TH1D* hShowerDCosYDiff;
    TH1D* hShowerDCosZDiff;

    TH2D* hShowerEnergyRatioVsEnergy;

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
