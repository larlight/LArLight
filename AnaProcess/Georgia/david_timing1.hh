/**
 * \file david_timing1.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class david_timing1
 *
 * @author ub
 */

/** \addtogroup Analysis

    @{*/

#ifndef DAVID_TIMING1_HH
#define DAVID_TIMING1_HH

#include <map>
#include <vector>
#include "ana_base.hh"
#include <TH1D.h>
#include <TH2D.h>
#include <math.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <TFitResultPtr.h>
#include <TF1.h>

namespace larlight {

  /**
     \class david_timing1
     User custom analysis class made by ub
  */
  class david_timing1 : public ana_base{
    
  public:
    
    /// Default constructor
    david_timing1(){ _name="david_timing1"; _fout=0;};
    
    /// Default destructor
    virtual ~david_timing1(){};
    
    /** IMPLEMENT in david_timing1.cc!
	Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();
    
    /** IMPLEMENT in david_timing1.cc! 
	Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);
    
    /** IMPLEMENT in david_timing1.cc! 
	Finalize method to be called after all events processed.
    */
    virtual bool finalize();
    
  private:
    
    //nu stream
    
    Double_t ADCs[1500];
    
    TH1D *pmt_nu_evno;
    TH1D *fifo_trig_abs;
    TH1D *ch2_trig_abs;
    TH1D *frame_diff_1;
    TH1D *frame_diff_2;
    TH1D *ch2_readout;
    TH1D *ch2_readout_mod;
    TH1D *fifo_femtrig_samplenum;
    TH1D *hWF;
    TH1D *saturation_histo;
    TH1D *pulse_start;
    TH1D *reco_start;
    TH1D *reco_start_fit;
    TH1D *reco_start_cut;

    int _nch;
    int _nrd;

    int k;

    int pmt_nu_evtno;
    int trig_evtno;

  };
}

#endif

/** @} */ // end of doxygen group 
