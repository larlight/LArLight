/**
 * \file pulse_viewer.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class pulse_viewer
 *
 * @author kazuhiro
 */

/** \addtogroup Analysis

    @{*/

#ifndef PULSE_VIEWER_HH
#define PULSE_VIEWER_HH

#include <TH1D.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TPad.h>
#include <TPaveText.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TGaxis.h>
#include <TColor.h>

#include <set>
#include <algorithm>

#include "ana_base.hh"

namespace larlight {

  /**
     \class pulse_viewer
     This is a class to visualize reconstructed pulse information together with the raw waveform on a histogram.
     This helps to debug/understand pulse reconstruction algorithm output.
     It is meant to be used interactively and not in a batch mode as it does not store any objects in an output.
     It allows a user to specify a range for various pulse-wise and event-wise parameters so that one can
     filter out irrelevant pulses from visualization routine (which can take time).
     See FEMPulseStudy/mac/pulse_viewer.py for an example usage.
   */
  class pulse_viewer : public ana_base{

  public:

    /// Default constructor
    pulse_viewer();

    /// Default destructor
    virtual ~pulse_viewer(){};

    /// Initialization method to be called before the analysis event loop.
    virtual bool initialize();

    /// Analyze a data event-by-event  
    virtual bool analyze(storage_manager* storage);

    /// Finalize method to be called after all events processed.
    virtual bool finalize();

    /** CORE METHOD 1
	Given the channel number, this method iterates over the locally stored
	values for a reconstructed pulse. It visualize these information on
	a canvas with the original waveform which is returned as TH1D object pointer.
	The canvas can be retrieved through pulse_viewer::get_canvas() method after
	you call this function.
     */
    TH1D* next_pulse(UShort_t ch);

    /** CORE METHOD 2
	Reverse iteration over reconstructed pulse as opposed to the iteration in an
	ordinary order. See pulse_viewer::next_pulse() for details.
    */
    TH1D* previous_pulse(UShort_t ch);


    /// Getter for TCanvas object pointer on which waveform & pulse info are visualized
    const TCanvas* get_canvas() const {return _cWF;};

    /// Getter for a set of channel numbers on which hist pulses are found.
    const std::set<UShort_t> get_hit_channels() const {return _channels;};

    /** Iterator for a set of channels for which reconstructed pulses are found.
	It is same as a user's own iterator over a set of channel numbers that
	can be obtained from pulse_viewer::get_hit_channels(). This method is 
	provided for reducing a coding need for writing users' own iterator loop.
     */
    UShort_t next_channel();

    /// Method to display cut values. 
    void display_cut_ranges();

    //
    // Setters
    //

    /// Returns # of reconstructed pulse for a specified channel number
    short get_npulse(UShort_t ch);

    /// Setter for the range of pusle start time to select pulses of interest
    void set_range_pulse_start_time(double v1, double v2)   {_cut_tstart.first=v1;   _cut_tstart.second=v2;   };

    /// Setter for the range of pusle end time to select pulses of interest..
    void set_range_pulse_end_time(double v1, double v2)     {_cut_tend.first=v1;     _cut_tend.second=v2;     };

    /// Setter for the range of pusle amplitude to select pulses of interest..
    void set_range_pulse_amp(double v1, double v2)          {_cut_amp.first=v1;      _cut_amp.second=v2;      };

    /// Setter for the range of integrated charge to select pulses of interest..
    void set_range_pulse_charge(double v1,double v2)        {_cut_charge.first=v1;   _cut_charge.second=v2;   };

    /// Setter for the range of pedestal mean value to select pulses of interest..
    void set_range_ped_mean(double v1, double v2)           {_cut_pedbase.first=v1;  _cut_pedbase.second=v2;  };

    /// Setter for the range of pedestal rms to select pulses of interest..
    void set_range_ped_rms(double v1, double v2)            {_cut_pedrms.first=v1;   _cut_pedrms.second=v2;   };

    /// Setter for the range of event id to select pulses of interest.
    void set_range_event_id(UInt_t v1, UInt_t v2) {_cut_event_id.first=v1; _cut_event_id.second=v2; };

    /// Setter for the range of channel numbers to select pulses of interest.
    void set_range_channel(UShort_t v1, UShort_t v2) {_cut_channels.first=v1; _cut_channels.second=v2;};

    /// Setter for the range of event charge sum to select pulses of interest.
    void set_range_sum_charge(double v1, double v2)         {_cut_sum_charge.first=v1; _cut_sum_charge.second=v2;};

    /// Setter for the range of event peak sum to select pulses of interest.
    void set_range_sum_peak(double v1, double v2)           {_cut_sum_peak.first=v1; _cut_sum_peak.second=v2;};

    /// Setter for the range of event-wise number of pulses to select pulses of interest.
    void set_range_npulse(double v1, double v2)             {_cut_npulse.first=v1; _cut_npulse.second=v2;};


    //
    // Getters
    //

    /// Getter for the range of pusle start time to select pulses of interest
    const std::pair<double,double> range_pulse_start_time() const   {return _cut_tstart;};

    /// Getter for the range of pusle end time to select pulses of interest..
    const std::pair<double,double> range_pulse_end_time() const     {return _cut_tend;};

    /// Getter for the range of pusle amplitude to select pulses of interest..
    const std::pair<double,double> range_pulse_amp() const          {return _cut_amp;};

    /// Getter for the range of integrated charge to select pulses of interest..
    const std::pair<double,double> range_pulse_charge() const       {return _cut_charge;};

    /// Getter for the range of pedestal mean value to select pulses of interest..
    const std::pair<double,double> range_ped_mean() const           {return _cut_pedbase;};

    /// Getter for the range of pedestal rms to select pulses of interest..
    const std::pair<double,double> range_ped_rms() const            {return _cut_pedrms;};

    /// Getter for the range of event id to select pulses of interest.
    const std::pair<UInt_t,UInt_t> range_event_id() const {return _cut_event_id;};

    /// Getter for the range of channel numbers to select pulses of interest.
    const std::pair<UInt_t,UInt_t> range_channel() const  {return _cut_channels;};

    /// Getter for the range of event charge sum to select pulses of interest.
    const std::pair<double,double> range_sum_charge() const         {return _cut_sum_charge;};

    /// Getter for the range of event peak sum to select pulses of interest.
    const std::pair<double,double> range_sum_peak() const           {return _cut_sum_peak;};

    /// Getter for the range of event-wise number of pulses to select pulses of interest.
    const std::pair<UInt_t,UInt_t> range_npulse() const         {return _cut_npulse;};

  protected:

    /// set Kazu's style
    void set_style();

    /// reset cut values
    void reset_cuts();

    /// clear data holder map objects
    void clear_map();

    /// clear viewer related TObjects on the heap
    void clear_viewer();

    /** returns a waveform TH1D object ... to be called internally from pulse_viewer::next_pulse() and
	pulse_viewer::previous_pulse(). Note that it does not perform a check of array index validity
	as it assumes the parent function does such validity check.
    */
    TH1D* get_waveform(UShort_t ch, size_t index);

    /// a method to add entries to lots of map objects to hold pulse info
    void add_channel_entry(UShort_t ch);

    UInt_t _event_id;   ///< event id holder
    double      _sum_charge; ///< event-wise summed charge holder
    double      _sum_peak;   ///< event-wise summed peak height holder
    UInt_t    _npulse;     ///< event-wise number of reco-ed pulse holder
    std::set<UShort_t>                           _channels;             ///< set of channel numbers for reco-ed pulses
    std::set<UShort_t>::iterator                 _ch_iter;              ///< internal iterator for _channels member
    /** Map of pulse count: the first element in the pair holds the total reco-ed pulse count while the second
	element holds the internal index to be updated by pulse_viewer::next_pulse() and pulse_viewer::previous_pulse() methods.
    */
    std::map<UShort_t,std::pair<short,short> > _pulse_count;          
    std::map<UShort_t,std::vector<UInt_t> >    _pulse_frame;   ///< a set of frame id
    std::map<UShort_t,std::vector<UInt_t> >    _pulse_sample;  ///< a set of sample numbers
    std::map<UShort_t,std::vector<double> >    _pulse_tstart;  ///< a set of reco-ed pulse start time
    std::map<UShort_t,std::vector<double> >    _pulse_tend;    ///< a set of reco-ed pulse end time
    std::map<UShort_t,std::vector<double> >    _pulse_amp;     ///< a set of reco-ed pulse amplitude
    std::map<UShort_t,std::vector<double> >    _pulse_charge;  ///< a set of reco-ed pulse charge
    std::map<UShort_t,std::vector<double> >    _pulse_pedbase; ///< a set of reco-ed pulse pedestal baseline mean
    std::map<UShort_t,std::vector<double> >    _pulse_pedrms;  ///< a set of reco-ed pulse pedestal baseline standard deviation
    std::map<UShort_t,std::map<UInt_t,std::map<UInt_t,std::vector<UShort_t> > > > _waveforms; ///< waveform containers

    /// Local (yet main) canvas poitner
    TCanvas* _cWF;

    // TPads to be drawn within the main canvas
    TPad *_pTitle; ///< title pad
    TPad *_pData;  ///< data (histogram, numbers) pad

    // TPads to be drawn within the _pData pad
    TPad *_pNote;  ///< pulse parameters
    TPad *_pWF;    ///< waveform histogram

    // Texts to on the pads
    TText     *_fTitle; ///< canvas title
    TPaveText *_fNote;  ///< pulse parameters

    /// Waveform container histogram pointer
    TH1D* _hWF;

    /// TLine pointers to display pule start time, end time, pedestal baseline mean and deviations
    TLine *_lStart, *_lEnd, *_lBase, *_lRMSTop, *_lRMSBottom, *_lTop;

    /// pairs to define a range of pulse parameters for displaying pulses.
    std::pair<double,double> _cut_tstart, _cut_tend, _cut_amp, _cut_charge, _cut_pedbase, _cut_pedrms;

    /// pairs to define a range of event parameters for displaying pulses.
    std::pair<double,double> _cut_sum_charge, _cut_sum_peak;

    /// pairs to define a range of event parameters for displaying pulses.
    std::pair<UInt_t,UInt_t> _cut_npulse;  

    /// a pair to define a range of event id for displaying pulses.
    std::pair<UInt_t,UInt_t> _cut_event_id;

    /// a pair to define a range of channel numbers for displaying pulses.
    std::pair<UShort_t,UShort_t> _cut_channels;
  };
}

#endif

/** @} */ // end of doxygen group 
