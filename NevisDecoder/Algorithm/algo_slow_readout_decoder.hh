/**
 * \file algo_slow_readout_decoder.hh
 *
 * \ingroup Algorithm
 * 
 * \brief Class def header of the decoder algorithm for slow readout
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Algorithm

    @{*/
#ifndef ALGO_SLOW_READOUT_DECODER_HH
#define ALGO_SLOW_READOUT_DECODER_HH

#include "algo_base.hh"

namespace larlight {

  /**
     \class algo_slow_readout_decoder
     Algorithm class to decode data from PMT slow readout stream.
     It expects a stream of 32-bit words (ordered) to be fed into
     process_word method. This class takes care of decoding & grouping
     channel/event wise data & storing them.
   */
  class algo_slow_readout_decoder : public algo_base {

  public:

    enum PROCESS_FLAG{
      READ_HEADER=0, ///< Process status flag ... reading event header 
      READ_CHANNEL   /// Process status flag ... reading channel adc info 
    };

  public:

    /// Default constructor
    algo_slow_readout_decoder();

    /// Default destructor
    virtual ~algo_slow_readout_decoder(){};

    /// Setter for a boolean to print out adc values as decoding proceeds
    void print_adcval(bool doit=true){_print_adcval=doit;};

    /// Implementation of algo_base::process_word
    virtual bool process_word(const UInt_t word);

    /// Implementation of algo_base::process_word
    virtual inline FEM::FEM_WORD get_word_class(const UInt_t word) const {
      if     ( (word & 0xf000) == 0xf000 )
	return FEM::FEM_HEADER;
      else if( (word & 0xf000) == 0x4000 )
	return FEM::FEM_FIRST_WORD;
      else if( (word & 0xf000) == 0x9000 )
	return FEM::CHANNEL_HEADER;
      else if( (word & 0xf000) == 0xa000 || (word & 0xf000) == 0x8000)
	return FEM::CHANNEL_WORD;
      else if( (word & 0xf000) == 0xb000 )
	return FEM::CHANNEL_LAST_WORD;
      else if( (word & 0xf000) == 0xc000 )
	//return FEM::FEM_LAST_WORD;
	return FEM::EVENT_LAST_WORD;
      else
	return FEM::UNDEFINED_WORD;
    }

    /** A method to correct roll-over effect in the pmt/trigger frame number.
	Provide a reference frame number in ref_id, subject frame number in subject_id,
	and roll-over number in diff. Return is the corrected number.
	For discriminator roll-over effect, it's 0x7 as of Aug. 12th 2013, for instance.
     */
    UInt_t round_diff(UInt_t ref_id, UInt_t subject_id, UInt_t diff) const;

    /// Implementation of algo_base::check_event_quality
    virtual bool check_event_quality();

    /** For a possible use, add a function to return currently processing ch_data member pointer.
	Note that event data member should be available already to the parent class as it is provided.
    */
    const fifo*   get_current_ch_data(){return &_ch_data;};

    /// Override function to reset the instance
    virtual void reset();

    /// Implementation of a method to inquire a status whether the event data empty or not
    virtual bool is_event_empty(){ 
      if(_event_data) return (_event_data->event_id()==FEM::INVALID_WORD);
      else return true;
    };

  protected:

    /** Initialization of channel wise data holder. */
    virtual void init_ch_info();

    /** Initialization of event wise data holder. */
    virtual void init_event_info();

    /// A method to process event header word passed from process_word method
    virtual bool process_header(const UInt_t word);

    /// A method to decode event header word passed from process_header 
    virtual bool decode_event_header(const UInt_t *event_header);

    /// A method to process channel word passed from process_word method
    virtual bool process_ch_word(const UInt_t word, 
				 UInt_t &last_word);

    /// A method to decode channel word passed from process_ch_word
    virtual bool decode_ch_word(const UInt_t word, 
				const FEM::FEM_WORD last_word_class);

    /// A method to print out ADC values on the stdout stream
    void print_adc_values();

    /// A method to append PMT-wise data (fifo) to event data
    void store_ch_data();

    /// A method to a correction to the sample number for beam-gate event.
    void apply_beamgate_correction();

    //
    // Constants
    //
    // Comment out the following if you deal with an older format with 1 less header
#define INCLUDE_EXTRA_HEADER
#ifdef INCLUDE_EXTRA_HEADER
    static const size_t FEM_HEADER_COUNT=6; ///< Number of event header words
  #else
    static const size_t FEM_HEADER_COUNT=5; ///< Number of event header words
  #endif
    static const size_t CHANNEL_HEADER_COUNT=2; ///< Number of channel header words

    // variables
    UInt_t  _event_header_words[FEM_HEADER_COUNT]; ///< Event header word holder
    pmtfifo _ch_data;                              ///< Channel data holder.
    event_pmtfifo *_event_data;                    ///< Event waveform
    FEM::DISCRIMINATOR _last_disc_id;              ///< Holder of last channel data's disc. id
    UShort_t _last_channel_number;                 ///< Holder of last channel data's channel number

    //
    // Run control variable
    //
    PROCESS_FLAG _process;        ///< Process status flag holder
    bool _continue_to_next_event; ///< Boolean to let a user continue to next event for any issue.
    bool _print_adcval;           ///< Boolean to print out adc values
    bool _beam_event;             ///< Boolean for beam event
    UInt_t _beam_ref_nwords; ///< Number of ADC samples in beam ref channel
    UInt_t _beam_ref_sample; ///< Sample number in beam ref channel
    UInt_t _beam_ref_frame;  ///< Frame ID from beam ref channel 
    UInt_t _last_word;       ///< Last word processed
    size_t _channel_header_count; ///< A counter for channel header words
    size_t _event_header_count;   ///< A counter for event header words

  };
}
#endif 
/** @} */ // end of doxygen group 
