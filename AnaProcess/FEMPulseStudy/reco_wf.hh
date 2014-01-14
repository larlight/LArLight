/**
 * \file reco_wf.hh
 *
 * \ingroup FEMPulseStudy
 * 
 * \brief Class def header for reco_wf class
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup FEMPulseStudy

    @{*/
#ifndef RECO_WF_HH
#define RECO_WF_HH

#include <map>
#include <set>
#include "ana_base.hh"
#include <TH2D.h>

namespace larlight {

  /**
     \class reco_wf
     Analysis class to run waveform reconstruction.
     It combines multiple waveforms (if there are) into one big waveform
     based on sample number, frame id, and discriminator id.
   */
  class reco_wf : public ana_base {

  public:

    /// Default constructor
    reco_wf();

    /// Default destructor
    ~reco_wf(){};

    /// Implementation of ana_base::initialize
    virtual bool initialize();

    /// Implementation of ana_base::analyze
    virtual bool analyze(storage_manager* data);

    /// Implementation of ana_base::finalize
    virtual bool finalize();

    /// Getter for a reconstructed PMT-waveform
    const std::vector<UShort_t>* get_ch_waveform(UInt_t ch);

    /// Getter for a set of read-out channels from data
    const std::set<UInt_t>* get_ch_set()const {return &_channels;};

    /// Setter for readout frame size
    void set_frame_width(UInt_t w){_frame_width=w;};

    /// Getter for the very first frame number among all in the event
    UInt_t get_start_frame() const {return _ref_frame;};

    /// Getter for the very first sample number among all in the event
    UInt_t get_start_slice() const {return _ref_slice;};

    /// A 2D histogram generator which contains reco-ed waveform for all PMTs.
    TH2D* get_histo();

  private:

    /// Initialize members
    void reset();

    /// Clear event-wise members
    void clear_event();

    /// A collection of reco-ed waveforms
    std::map<UInt_t,std::vector<UShort_t> > _wf_map; 
    std::set<UInt_t> _channels; ///< A set of channel numbers 
    UInt_t _frame_width;     ///< The readout frame width

    UInt_t _event_id;        ///< Event ID number
    UInt_t _ref_slice;       ///< First time slice number in the event
    UInt_t _ref_frame;       ///< First frame number in the event

    TH2D* _wf_histo;              ///< Histogram of reco-ed waveforms

  };
}

#endif
/** @} */ // end of doxygen group 
