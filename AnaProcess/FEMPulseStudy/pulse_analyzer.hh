/**
 * \file pulse_analyzer.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class pulse_analyzer
 *
 * @author kazuhiro
 */

/** \addtogroup Analysis

    @{*/

#ifndef PULSE_ANALYZER_HH
#define PULSE_ANALYZER_HH

#include <TTree.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TLine.h>
#include <set>
#include <algorithm>
#include <map>
#include <vector>
#include "ana_base.hh"
#include <TH1D.h>
#include <TH2D.h>
#include <math.h>
#include <TGraph.h>
#include <TGraphErrors.h>


#include "ana_base.hh"

namespace larlight {

  /**
     \class pulse_analyzer
     This is a class to visualize reconstructed pulse information together with the raw waveform on a histogram.
     This helps to debug/understand pulse reconstruction algorithm output.
     It is meant to be used interactively and not in a batch mode as it does not store any objects in an output.
     It allows a user to specify a range for various pulse-wise and event-wise parameters so that one can
     filter out irrelevant pulses from visualization routine (which can take time).
     See FEMPulseStudy/mac/pulse_analyzer.py for an example usage.
   */
  class pulse_analyzer : public ana_base{

  public:

    /// Default constructor
    pulse_analyzer();

    /// Default destructor
    virtual ~pulse_analyzer(){};

    /// Initialization method to be called before the analysis event loop.
    virtual bool initialize();

    /// Analyze a data event-by-event  
    virtual bool analyze(storage_manager* storage);

    /// Finalize method to be called after all events processed.
    virtual bool finalize();

    void set_pulse_type(DATA::DATA_TYPE type){_pulse_type=type;};

  private:

    UInt_t _event_id;       ///< event id holder
    UInt_t _ch_frame_id;
    UInt_t _ch_sample_id;
    UInt_t _disc_id;

    DATA::DATA_TYPE _pulse_type;

    TTree *_ch_tree;

    int _ch, _nsample;

    double _wf_charge, _wf_peak, _wf_ts, _wf_tm, _wf_te, _ped_mean, _ped_rms;

  };

}
#endif

/** @} */ // end of doxygen group 
