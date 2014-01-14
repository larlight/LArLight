/**
 * \file led_pulse_selector.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class led_pulse_selector
 *
 * @author kazuhiro
 */

/** \addtogroup Analysis

    @{*/

#ifndef LED_PULSE_SELECTOR_HH
#define LED_PULSE_SELECTOR_HH

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

#include "pulse_selector_simple.hh"

namespace larlight {

  /**
     \class led_pulse_selector
     This is a simple selection class to select LED pulse, implemented on pulse_selector_base base class.
     It expects a user to provide a reference channel number in which LED pulser's external trigger signal is fed in.
     Then this class looks for a pulse from other channels that come in coincidence with this external trigger pulse.
     A user also has to specify a time interval to claim a coincidence.
   */
  class led_pulse_selector : public pulse_selector_simple {

  public:

    /// Default constructor
    led_pulse_selector();

    /// Default destructor
    virtual ~led_pulse_selector(){};

    /// Initialization method to be called before the analysis event loop.
    virtual bool initialize();

    /// Analyze a data event-by-event  
    virtual bool analyze(storage_manager* storage);

    /// Finalize method to be called after all events processed.
    virtual bool finalize();

    /// Clear event
    void clear_event();

    void set_ref_channel(UShort_t ch){_ref_ch=ch;};

    void set_frame_size(UInt_t frame_size){_frame_size=frame_size;};

    void set_time_cut(UInt_t time_cut){_time_cut = time_cut;};

    virtual bool select_pulse(const pulse* data);

  private:

    UShort_t _ref_ch;

    UInt_t _frame_size;

    UInt_t _time_cut;

    std::set<UInt_t> _ref_time;

  };
}

#endif

/** @} */ // end of doxygen group 
