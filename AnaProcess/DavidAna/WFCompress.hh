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

    void Compress(larlight::fifo* tpc_data, larlight::event_fifo* new_event_wf);

    void make_new_wf(larlight::fifo* wf, int start, int stop, larlight::event_fifo* new_event_wf);

    protected:

    TTree *_merge_tree;
    int _wfnumber;
    larlight::event_fifo new_event_wf;
    int    _NVarSamples;
    int    _NSamples;
    int    _event_num;
    double _VarCut;
    int    _baseline;
  };
}
#endif

/** @} */ // end of doxygen group 
