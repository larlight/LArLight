/**
 * \file EvtDisplay.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class EvtDisplay
 *
 * @author SHELL_USER_NAME
 */

/** \addtogroup Analysis

    @{*/

#ifndef EVTDISPLAY_HH
#define EVTDISPLAY_HH

#include "ana_base.hh"

namespace larlight {
  /**
     \class EvtDisplay
     User custom analysis class made by SHELL_USER_NAME
   */
  class EvtDisplay : public ana_base{
  
  public:

    /// Default constructor
    EvtDisplay(){ _name="EvtDisplay"; _fout=0;};

    /// Default destructor
    virtual ~EvtDisplay(){};

    /** IMPLEMENT in EvtDisplay.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in EvtDisplay.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in EvtDisplay.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    protected:

    int _evt_num;

  };
}
#endif

/** @} */ // end of doxygen group 
