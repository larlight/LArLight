/**
 * \file algo_xmit_decoder.hh
 *
 * \ingroup Algorithm
 * 
 * \brief algo_xmit_decoder class def header file.
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Algorithm

    @{*/

#ifndef ALGO_XMIT_DECODER_HH
#define ALGO_XMIT_DECODER_HH

#include "algo_slow_readout_decoder.hh"

namespace larlight {

  /**
     \class algo_xmit_decoder
     Algorithm to decode 32-bit words from PMT xmit readout.
     Data words should be fed into process_word method for decoding.
     This class takes care of decoding, grouping of PMT/event data,
     and storing them through storage_manager.
   */
  class algo_xmit_decoder : public algo_slow_readout_decoder {

  public:

    /// Default constructor
    algo_xmit_decoder() : algo_slow_readout_decoder(){};

    /// Default destructor
    virtual ~algo_xmit_decoder(){};

    /// Override of algo_slow_readout_decoder::process_header method
    virtual bool process_header(const UInt_t word);

    /// Override of algo_base::get_word_class method
    virtual inline FEM::FEM_WORD get_word_class(const UInt_t word) const {
      // One of core functions to identify PMT binary word format
      if( (word & 0xffffffff) == 0xffffffff )
	return FEM::EVENT_HEADER;
      if( (word & 0xffff) == 0xffff )
	return FEM::FEM_HEADER;
      else if( (word & 0xf000) == 0xf000 )
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
      else if( (word & 0xf000) == 0xe000 )
	return FEM::EVENT_LAST_WORD;
      else
	return FEM::UNDEFINED_WORD;
    };

  };
}
#endif 

/** @} */ // end of doxygen group 
