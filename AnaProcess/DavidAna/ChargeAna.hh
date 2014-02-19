/**
 * \file ChargeAna.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class ChargeAna
 *
 * @author SHELL_USER_NAME
 */

/** \addtogroup Analysis

    @{*/

#ifndef CHARGEANA_HH
#define CHARGEANA_HH

#include "ana_base.hh"

namespace larlight {
  /**
     \class ChargeAna
     User custom analysis class made by SHELL_USER_NAME
   */
  class ChargeAna : public ana_base{
  
  public:

    /// Default constructor
    ChargeAna(){ _name="ChargeAna"; _fout=0;};

    /// Default destructor
    virtual ~ChargeAna(){};

    /** IMPLEMENT in ChargeAna.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in ChargeAna.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in ChargeAna.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    protected:

    int _event_num;
    int _coll_baseline;
    TH1D *TotCharge;
    TH1D *ChargePerWire;
    TH1D *ChargePerPulse;
    TH1D *NumHits;

  };
}
#endif

/** @} */ // end of doxygen group 
