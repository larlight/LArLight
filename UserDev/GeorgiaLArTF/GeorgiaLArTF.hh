/**
 * \file GeorgiaLArTF.hh
 *
 * \ingroup GeorgiaLArTF
 * 
 * \brief Class def header for a class GeorgiaLArTF
 *
 * @author georgia
 */

/** \addtogroup GeorgiaLArTF

    @{*/

#ifndef GEORGIALARTF_HH
#define GEORGIALARTF_HH

#include "ana_base.hh"
#include <TH1D.h>
#include <TH2D.h>
#include <TGraph.h>

namespace larlight {

  class GeorgiaLArTF : public ana_base{
  
  public:

    /// Default constructor
    GeorgiaLArTF() : ana_base() { _name="GeorgiaLArTF"; }

    /// Default destructor
    virtual ~GeorgiaLArTF(){};

    virtual bool initialize();

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();

    protected:
    
    private:

    //nu stream
    TH1D *pmt_nu_evno;
    TH1D *pmt_nu_evfrm;
    TH1D *pmt_nu_add;
    TH1D *pmt_nu_mod;
    TH1D *pmt_nu_trfrm;
    TH1D *pmt_nu_trslc;
    TH1D *pmt_nu_ch;
    TH1D *pmt_nu_nrd;
    TH2D *pmt_nu_rdtype;
    
    TGraph *pmt_nu_frdiff;
    TGraph *pmt_nu_slcdiff;

    //trig stream
    TH1D *trig_trfrm;
    TH1D *trig_trslc;
    TH1D *trig_trid;
    TH1D *trig_pmid;
    TH1D *trig_trpc;
    TH1D *trig_extrn;
    TH1D *trig_actv;
    TH1D *trig_gate1;
    TH1D *trig_gate2;
    TH1D *trig_veto;
    TH1D *trig_calib;
    TH1D *trig_rem64;
    TH1D *trig_rem16;

    TGraph *trig_frdiff;
    TGraph *trig_slcdiff;

    //some variables which come from configuration file
    const int samples_per_frame = 102400;


  };
}
#endif

//**************************************************************************
// 
// For Analysis framework documentation, read Manual.pdf here:
//
// http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
//
//**************************************************************************

/** @} */ // end of doxygen group 
