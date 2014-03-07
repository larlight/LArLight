/**
 * \file NoiseAna.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class NoiseAna
 *
 * @author SHELL_USER_NAME
 */

/** \addtogroup Analysis

    @{*/

#ifndef NOISEANA_HH
#define NOISEANA_HH

#include "ana_base.hh"

#include "TVirtualFFT.h"
#include "TF1.h"
#include "TMath.h"

namespace larlight {
  /**
     \class NoiseAna
     User custom analysis class made by SHELL_USER_NAME
   */
  class NoiseAna : public ana_base{
  
  public:

    /// Default constructor
    NoiseAna(){ _name="NoiseAna"; _fout=0;};

    /// Default destructor
    virtual ~NoiseAna(){};

    /** IMPLEMENT in NoiseAna.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in NoiseAna.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in NoiseAna.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    protected:

    //Width in seconds of each bin
    double _BinTime;
    //minimum channel number used
    int _ChMin;
    //maximum channel number used
    int _ChMax;
    int event_num;
    TH1D *AllRMS;
    TH1D *ChannelRMS;
    TH1D *RMSperChan[30000];
    TH1D *AllChan;
    TH1D *NoiseSpec;
    TH1D *SineExmpl;
    TH1D *sine;

  };
}
#endif

/** @} */ // end of doxygen group 
