/**
 * \file georgia_timing1.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class georgia_timing1
 *
 * @author ub
 */

/** \addtogroup Analysis

    @{*/

#ifndef GEORGIA_TIMING1_HH
#define GEORGIA_TIMING1_HH

#include <map>
#include <vector>
#include "ana_base.hh"
#include <TH1D.h>
#include <TH2D.h>
#include <math.h>
#include <TGraph.h>
#include <TGraphErrors.h>

namespace larlight {

  /**
     \class georgia_timing1
     User custom analysis class made by ub
  */
  class georgia_timing1 : public ana_base{
    
  public:
    
    /// Default constructor
    georgia_timing1(){ _name="georgia_timing1"; _fout=0;};
    
    /// Default destructor
    virtual ~georgia_timing1(){};
    
    /** IMPLEMENT in georgia_timing1.cc!
	Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();
    
    /** IMPLEMENT in georgia_timing1.cc! 
	Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);
    
    /** IMPLEMENT in georgia_timing1.cc! 
	Finalize method to be called after all events processed.
    */
    virtual bool finalize();
    
  private:
    
    //nu stream
    TH1D *pmt_nu_evno;
    TH1D *pmt_nu_evfrm;
    TH1D *pmt_nu_add;
    TH1D *pmt_nu_mod;
    TH1D *pmt_nu_trfrm;
    TH1D *pmt_nu_trslc;
    TH1D *pmt_nu_ch;
    TH1D *pmt_nu_nch;
    TH1D *pmt_nu_nrd;
    TH2D *pmt_nu_rdtype;
    
    //trig stream
    TH1D *trig_trfrm;
    TH1D *trig_trslc;
    TH1D *trig_trid;
    TH1D *trig_pmid;
    TH1D *trig_trpc;
    TH1D *trig_ext;
    TH1D *trig_actv;
    TH1D *trig_gate1;
    TH1D *trig_gate2;
    TH1D *trig_veto;
    TH1D *trig_calib;
    TH1D *trig_rem64;
    TH1D *trig_rem16;
    
    int _nch;
  };
}

#endif

/** @} */ // end of doxygen group 
