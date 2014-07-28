/**
 * \file MCShowerAna.hh
 *
 * \ingroup ShowerReco3D
 * 
 * \brief Class def header for a class MCShowerAna
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerReco3D

    @{*/

#ifndef MCSHOWERANA_HH
#define MCSHOWERANA_HH

#include "ana_base.hh"
#include "ShowerBackTracker.hh"
#include "LArUtil-TypeDef.hh"
#include <TH1D.h>
namespace larlight {
  /**
     \class MCShowerAna
     User custom analysis class made by kazuhiro
   */
  class MCShowerAna : public ana_base{
  
  public:

    /// Default constructor
    MCShowerAna();

    /// Default destructor
    virtual ~MCShowerAna(){};

    /** IMPLEMENT in MCShowerAna.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in MCShowerAna.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in MCShowerAna.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:

    TH1D *hMCX, *hMCY, *hMCZ;
    TH1D *hdcosX, *hdcosY, *hdcosZ;

    TH2D *hMCEdepMCQ;

    TH1D *hAmpQEff;

    TH1D *hMCEdepEff;
    TH1D *hMCQEff;
    
    TH1D* hdEdx;
    TH1D* hEner;
    TH1D* hEnerFrac;
    TH1D* hDetEnerFrac;
    TH1D* hMCEner;
    TH1D* hMCMotherEner;
    TH1D* recohEner;
    TH1D *recorrhEner;
    ShowerBackTracker fBTAlg;
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
