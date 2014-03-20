/**
 * \file MCShowerParams.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class MCShowerParams
 *
 * @author SHELL_USER_NAME
 */

/** \addtogroup Analysis

    @{*/

#ifndef MCSHOWERPARAMS_HH
#define MCSHOWERPARAMS_HH

#include "ana_base.hh"

namespace larlight {
  /**
     \class MCShowerParams
     User custom analysis class made by SHELL_USER_NAME
   */
  class MCShowerParams : public ana_base{
  
  public:

    /// Default constructor
    MCShowerParams(){ _name="MCShowerParams"; _fout=0;};

    /// Default destructor
    virtual ~MCShowerParams(){};

    /** IMPLEMENT in MCShowerParams.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in MCShowerParams.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in MCShowerParams.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    protected:
    
    int _event_num;
    int _evt_plt;
    TH1D *hMotherE;
    TH1D *hDaughterE;
    TH3D *hShowerMap;
    TH2D *hXYEnergy;
    TH2D *hXZEnergy;
    TH2D *hYZEnergy;
    TGraph *hDvsM_E;
  };
}
#endif

/** @} */ // end of doxygen group 
