/**
 * \file led_pulse_analyzer.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class led_pulse_analyzer
 *
 * @author kazuhiro
 */

/** \addtogroup Analysis

    @{*/

#ifndef LED_PULSE_ANALYZER_HH
#define LED_PULSE_ANALYZER_HH

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
#include "led_pulse_selector.hh"

namespace larlight {

  /**
     \class led_pulse_analyzer
     This is a class to visualize reconstructed pulse information together with the raw waveform on a histogram.
     This helps to debug/understand pulse reconstruction algorithm output.
     It is meant to be used interactively and not in a batch mode as it does not store any objects in an output.
     It allows a user to specify a range for various pulse-wise and event-wise parameters so that one can
     filter out irrelevant pulses from visualization routine (which can take time).
     See FEMPulseStudy/mac/led_pulse_analyzer.py for an example usage.
   */
  class led_pulse_analyzer : public ana_base{

  public:

    /// Default constructor
    led_pulse_analyzer();

    /// Default destructor
    virtual ~led_pulse_analyzer(){};

    /// Initialization method to be called before the analysis event loop.
    virtual bool initialize();

    /// Analyze a data event-by-event  
    virtual bool analyze(storage_manager* storage);

    /// Finalize method to be called after all events processed.
    virtual bool finalize();

    /// Clear event
    void clear_event();

    void set_pulse_type(DATA::DATA_TYPE type){_selector.set_pulse_type(type);};

    void set_ref_channel(UShort_t ch){_selector.set_ref_channel(ch);};

    void set_frame_size(UInt_t frame_size){_selector.set_frame_size(frame_size);};

    void set_time_cut(UInt_t time_cut){_selector.set_time_cut(time_cut);};

  private:

    UInt_t _event_id;       ///< event id holder
    UInt_t _frame;
    UInt_t _sample;
    UInt_t _disc_id;

    TTree *_tree;

    int _ch, _nsample;

    double _wf_charge, _wf_peak, _wf_ts, _wf_tm, _wf_te, _ped_mean, _ped_rms;

    led_pulse_selector _selector;

  };
}

#endif

/** @} */ // end of doxygen group 
