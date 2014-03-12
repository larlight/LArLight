/**
 * \file BaselineAna.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class BaselineAna
 *
 * @author SHELL_USER_NAME
 */

/** \addtogroup Analysis

    @{*/

#ifndef BASELINEANA_HH
#define BASELINEANA_HH

#include "ana_base.hh"

namespace larlight {
  /**
     \class BaselineAna
     User custom analysis class made by SHELL_USER_NAME
   */
  class BaselineAna : public ana_base{
  
  public:

    /// Default constructor
    BaselineAna(){ _name="BaselineAna"; _fout=0;};

    /// Default destructor
    virtual ~BaselineAna(){};

    /** IMPLEMENT in BaselineAna.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in BaselineAna.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in BaselineAna.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    protected:

    TH1D *_BaseHisto;
    TH1D *_ChNumHist;
    int event_num;    

  };
}
#endif

/** @} */ // end of doxygen group 
