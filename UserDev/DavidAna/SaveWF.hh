/**
 * \file SaveWF.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class SaveWF
 *
 * @author davidc1
 */

/** \addtogroup Analysis

    @{*/

#ifndef SAVEWF_HH
#define SAVEWF_HH

#include "ana_base.hh"
#include "Geometry.hh"
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
     \class SaveWF
     User custom analysis class made by davidc1
   */
  class SaveWF : public ana_base{
  
  public:

    /// Default constructor
    SaveWF(){ _name="SaveWF"; _fout=0;};

    /// Default destructor
    virtual ~SaveWF(){};

    /** IMPLEMENT in SaveWF.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in SaveWF.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in SaveWF.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    protected:

    TH1D      *compression;
    TH1D      *noise;
    TH1D      *rms;
    int       event_num;
    int       _NSamples;
    double    _VarCut;
    int       _baseline;
    int       _PedInduction;
    int       _PedCollection;

  };
}
#endif

/** @} */ // end of doxygen group 
