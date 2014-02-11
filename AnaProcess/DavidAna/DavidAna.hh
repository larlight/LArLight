/**
 * \file DavidAna.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class DavidAna
 *
 * @author davidc1
 */

/** \addtogroup Analysis

    @{*/

#ifndef DAVIDANA_HH
#define DAVIDANA_HH

#include "ana_base.hh"
#include <map>
#include <vector>
#include <TH1D.h>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <deque>


namespace larlight {
  /**
     \class DavidAna
     User custom analysis class made by davidc1
   */
  class DavidAna : public ana_base{
  
  public:

    /// Default constructor
    DavidAna(){ _name="DavidAna"; _fout=0;};

    /// Default destructor
    virtual ~DavidAna(){};

    /** IMPLEMENT in DavidAna.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in DavidAna.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in DavidAna.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    protected:

    TH1D      *compression;
    TH1D            *noise;
    int          event_num;
    int          _NSamples;
    double         _VarCut;
    int          _baseline;

  };
}
#endif

/** @} */ // end of doxygen group 
