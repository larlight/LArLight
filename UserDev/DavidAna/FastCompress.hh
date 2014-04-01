/**
 * \file FastCompress.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class FastCompress
 *
 * @author SHELL_USER_NAME
 */

/** \addtogroup Analysis

    @{*/

#ifndef FASTCOMPRESS_HH
#define FASTCOMPRESS_HH

#include "ana_base.hh"
#include <map>
#include <vector>
#include <TH1D.h>
#include <TTree.h>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <deque>
#include <iostream>
#include <fstream>
#include <string>

namespace larlight {
  /**
     \class FastCompress
     User custom analysis class made by SHELL_USER_NAME
   */
  class FastCompress : public ana_base{
  
  public:

    /// Default constructor
    FastCompress(){ _name="FastCompress"; _fout=0;};

    /// Default destructor
    virtual ~FastCompress(){};

    /** IMPLEMENT in FastCompress.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in FastCompress.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in FastCompress.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void Compress(larlight::tpcfifo* tpc_data, larlight::event_tpcfifo* new_event_wf);
    void make_new_wf_simple(larlight::tpcfifo* wf, int start, larlight::event_tpcfifo* new_event_wf);

    protected:

    TH1D* hCompress;
    int _wfnumber;
    //larlight::mctruth event_mc;
    larlight::event_tpcfifo new_event_wf;
    int    _NVarSamples;
    int    _NSamplesante;
    int    _NSamplespost;
    int    _event_num;
    double _VarCut;
    int    _baseline;
    double _compressfctr;

  };
}
#endif

/** @} */ // end of doxygen group 
