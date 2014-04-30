/**
 * \file algo_fem_decoder_base.hh
 *
 * \ingroup Algorithm
 * 
 * \brief Class def header of the decoder algorithm for slow readout
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Algorithm

    @{*/
#ifndef ALGO_FEM_DECODER_BASE_HH
#define ALGO_FEM_DECODER_BASE_HH

#include "algo_base.hh"

namespace larlight {

  /**
     \class algo_fem_decoder_base
     Abastract base class for decoding FEM module data stream.

     It expects a stream of 32-bit words (ordered) to be fed into
     process_word method. This class takes care of decoding & grouping
     channel/event wise data & storing them.

     This base class only implements the overall structure of decoding.
     It has two types of processing path: either event header or channel data word.
     The FEM header is common among TPC and PMT as they share the same FEM module.
     Channel data word is defined differently as data packet FPGA code is unique to each.
     Children class must implement channel-wise decoding methods.
   */

  class algo_fem_decoder_base : public algo_base {

  public:

    /// A local data holder struct for FEM header information
    struct InfoFEM_t {
  
      UChar_t module_address;         ///< Module address
      UChar_t module_id;              ///< Module id number
      UInt_t  event_number;           ///< Event counter 
      UInt_t  event_frame_number;     ///< Data packet frame number
      UInt_t  fem_trig_frame_number;  ///< Trigger frame number
      UInt_t  fem_trig_sample_number; ///< Trigger sample number
      UInt_t  nwords;                 ///< Number of words in an event
      UInt_t  checksum;               ///< Data checksum
      bool    quality;                ///< Event-wise quality check

      void clear_event(){

	module_address         = DATA::INVALID_UCHAR;
	module_id              = DATA::INVALID_UCHAR;
	event_number           = FEM::INVALID_WORD;
	event_frame_number     = FEM::INVALID_WORD;
	fem_trig_frame_number  = FEM::INVALID_WORD;
	fem_trig_sample_number = FEM::INVALID_WORD;
	nwords                 = 0;
	checksum               = 0;
	quality                = true;
      }

      InfoFEM_t(){ clear_event(); }

    };

  public:

    /// Default constructor
    algo_fem_decoder_base();

    /// Default destructor
    virtual ~algo_fem_decoder_base(){};

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
      else if( (word & 0xf000) == 0xa000 )
	return FEM::CHANNEL_WORD;
      else if( (word & 0xf000) == 0xb000 )
	return FEM::CHANNEL_LAST_WORD;
      else if( (word & 0xf000) == 0xc000 )
	return FEM::FEM_LAST_WORD;
      else
	return FEM::UNDEFINED_WORD;
    }

    /// Implementation of algo_base::check_event_quality
    virtual bool check_event_quality(){return true;};

    /// Override function to reset the instance
    virtual void reset();

    /// A simple function to compute a result of replacing lower "nbits" of "ref" by that of "subject"
    UInt_t roll_over(UInt_t ref, UInt_t subject, UInt_t nbits) const;

    /// A simple method to inquire if the data storage buffer is currently empty or not
    virtual bool is_event_empty(){ return (_header_info.event_number == FEM::INVALID_WORD);};

    /// A method to clear event-wise data
    virtual void clear_event(){_event_header_count=0; _header_info.clear_event();};

    /// A method to skip till it finds the next event header
    virtual void skip_to_next_event() {_search_for_next_event=true;}

  protected:

    /// A method to store event-wise data
    virtual bool store_event()=0;

    /// A method to process FEM header word passed from process_word method
    virtual bool process_fem_header(const UInt_t word, UInt_t &last_word);

    /// A method to decode fem header word passed from process_header 
    virtual bool decode_fem_header(const UInt_t *event_header);

    /**
       TO BE IMPLEMENTED!
       A method to process event header word.
    */
    virtual bool process_event_header(const UInt_t word, UInt_t &last_word)=0;

    /**
       TO BE IMPLEMENTED!
       A method to process channel word passed from process_word method.
       Channel word processing must be implemented in the children
    */
    virtual bool process_ch_word(const UInt_t word, 
				 UInt_t &last_word) = 0;

    /**
       TO BE IMPLEMENTED!
       A method to process 32-bit end-of-fem word.
    */
    virtual bool process_fem_last_word(const UInt_t word,
				       UInt_t &last_word) = 0;

    /**
       TO BE IMPLEMENTED!
       A method to process 32-bit end-of-event word.
    */
    virtual bool process_event_last_word(const UInt_t word,
					 UInt_t &last_word) = 0;

    virtual bool add_huffman_adc(std::vector<UShort_t> &wf, size_t zero_count);

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

    // variables
    UInt_t    _event_header_words[FEM_HEADER_COUNT]; ///< Event header word holder

    //
    // Run control variable
    //
    UInt_t    _last_word;              ///< Last word processed
    size_t    _event_header_count;     ///< A counter for event header words
    /// A run control boolean: skips any action till it finds the new event header.
    bool      _search_for_next_event;  
    InfoFEM_t _header_info;

    /// Algorithm dependent FEM last word
    FEM::FEM_WORD _algo_fem_last_word;
    
  };

}
#endif

/** @} */ // end of doxygen group 
