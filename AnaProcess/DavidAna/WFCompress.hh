/**
 * \file WFCompress.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class WFCompress
 *
 * @author David Caratelli
 */

/** \addtogroup Analysis

    @{*/

#ifndef WFCOMPRESS_HH
#define WFCOMPRESS_HH

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

namespace larlight {
  /**
     \class WFCompress
     User custom analysis class made by SHELL_USER_NAME
   */
  class WFCompress : public ana_base{
  
  public:

    /// Default constructor
    WFCompress(){ _name="WFCompress"; _fout=0;};

    /// Default destructor
    virtual ~WFCompress(){};

    /** IMPLEMENT in WFCompress.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in WFCompress.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in WFCompress.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void Compress(larlight::tpcfifo* tpc_data, larlight::event_tpcfifo* new_event_wf);

    void make_new_wf(larlight::tpcfifo* wf, int start, int stop, larlight::event_tpcfifo* new_event_wf);

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
