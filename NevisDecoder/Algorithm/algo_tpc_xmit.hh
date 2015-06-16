/**
 * \file algo_tpc_xmit.hh
 *
 * \ingroup Algorithm
 * 
 * \brief Class def header of the decoder algorithm for slow readout
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Algorithm

    @{*/
#ifndef ALGO_TPC_XMIT_HH
#define ALGO_TPC_XMIT_HH

#include "algo_fem_decoder_base.hh"

namespace larlight {

  /**
     \class algo_tpc_xmit
     Implementation for TPC XMIT data stream based on algo_fem_decoder_base class.

     There're lots of common things among PMT/TPC XMIT decoder, and ideally this class
     inherits from that of PMT. This is purely historical reason that I decided to develop
     this as a separate class w/o making such inheritance. It turned out PMT has a lot more
     information (not data volume) than TPC. It does not make sense to let TPC decoder
     class inherit from PMT decoder class (as TPC decoder is simpler). 
     This calls for a design review. For now, I omit this design issue and go ahead to
     implement a working version of tpc decoder.

     It expects a stream of 32-bit words (ordered) to be fed into
     process_word method. This class takes care of decoding & grouping
     channel/event wise data & storing them.
   */
  class algo_tpc_xmit : public algo_fem_decoder_base {

  public:

    /// Default constructor
    algo_tpc_xmit();

    /// Default destructor
    virtual ~algo_tpc_xmit(){};


    /// Override of algo_base::get_word_class method
    virtual inline FEM::FEM_WORD get_word_class(const UInt_t word) const {
      // One of core functions to identify PMT binary word format
      if( (word & 0xffffffff) == 0xffffffff )
	return FEM::EVENT_HEADER;
      else if( (word & 0xffffffff) == 0xe0000000 )
	return FEM::EVENT_LAST_WORD;
      if( (word & 0xffff) == 0xffff )
	return FEM::FEM_HEADER;
      else if( (word & 0xf000) == 0xf000 )
	return FEM::FEM_HEADER;
      else if( (word & 0xf000) == 0x4000 )
	return FEM::CHANNEL_HEADER;
      else if( (word & 0xf000) == 0x0000 ||
	       (word & 0xf000) == 0x8000   )
	return FEM::CHANNEL_WORD;
      else if( (word & 0xf000) == 0x5000 )
	return FEM::CHANNEL_LAST_WORD;
      else
	return FEM::UNDEFINED_WORD;
    };

  protected:

    virtual bool check_event_quality();

    virtual void clear_event();

    bool store_event();

    bool decode_ch_word                   (const UInt_t word, UInt_t &last_word);

    virtual void reset();

    virtual inline bool is_compressed(const UInt_t word){ return ( (word>>14) == 0x2 ); };

    //virtual inline bool is_compressed(const UInt_t word){ return ( word & 0x8000 ); };

    /**
       A method to process 32-bit event header word
     */
    virtual bool process_event_header(const UInt_t word,
				      UInt_t &last_word);

    /**
       A method to process 16-bit channel word
     */
    virtual bool process_ch_word(const UInt_t word, UInt_t &last_word);

    /**
       A method to process 32-bit end-of-fem word. Nothing to be done really.
    */
    virtual bool process_fem_last_word(const UInt_t word,
				       UInt_t &last_word);

    /**
       A method to process 32-bit end-of-event word. Store event @ this point.
    */
    virtual bool process_event_last_word(const UInt_t word,
					 UInt_t &last_word);

  protected:

    tpcfifo _ch_data;

    event_tpcfifo* _event_data;

    FEM::FEM_WORD _algo_fem_last_word;

  };
}
#endif

/** @} */ // end of doxygen group 
