#ifndef ALGO_TPC_HUFFMAN_HH
#define ALGO_TPC_HUFFMAN_HH

#include "algo_tpc_xmit.hh"
#include <deque>

namespace larlight {

  class algo_tpc_huffman : public algo_tpc_xmit {

  public:

    algo_tpc_huffman();

    ~algo_tpc_huffman(){};

    /// Override of algo_base::get_word_class method
    virtual inline FEM::FEM_WORD get_word_class(const UInt_t word) const {
      // One of core functions to identify PMT binary word format
      if( word == 0x0 ) return FEM::UNDEFINED_WORD;
      else if( (word & 0xffffffff) == 0xffffffff ) // Unique marker
	return FEM::EVENT_HEADER;
      else if( (word & 0xffffffff) == 0xe0000000 ) // Unique marker
	return FEM::EVENT_LAST_WORD;
      if( (word & 0xffff) == 0xffff )              // Unique marker
	return FEM::FEM_HEADER;
      else if( (word & 0xf000) == 0xf000 )         // Could be ADC word
	return FEM::FEM_HEADER;
      else if( !((word>>15) & 0x1) ) {

	if( (word & 0xf000) == 0x0000 )            // Uncompressed ADC word
	  return FEM::CHANNEL_WORD;           
	else if( (word & 0xf000) == 0x4000 )       // Channel first word
	  return FEM::CHANNEL_HEADER;
	else if( (word & 0xf000) == 0x5000 )       // Channel last word
	  return FEM::CHANNEL_LAST_WORD;
	else
	  return FEM::UNDEFINED_WORD;              // Undefined
      }
      else if( ((word>>15) & 0x1) ){
	if( (word & 0xffff) == 0xc000 )            // end of FEM word
	  return FEM::FEM_LAST_WORD;
	else
	  return FEM::CHANNEL_WORD;                // compressed ADC word
      }else
	return FEM::UNDEFINED_WORD;
    };


    /**
       Re-implementation of process_word. All machinaries based on
       word maker are useless in case of huffman coded TPC data
       because an entire 15 bits are used for compressed data bits.
     */
    virtual bool process_word(const UInt_t word);

    virtual void reset();

  protected:  
    
    virtual bool check_event_quality();

    virtual void clear_event();

    virtual bool process_event_header
    (const UInt_t word, UInt_t &last_word);

    virtual bool process_fem_header
    (const UInt_t word,UInt_t &last_word);

    virtual bool process_ch_word
    (const UInt_t word,UInt_t &last_word);

    virtual bool process_fem_last_word
    (const UInt_t word,UInt_t &last_word);

    virtual bool process_event_last_word
    (const UInt_t word,UInt_t &last_word);

  protected:

    bool _search_header;
    bool _ch_last_word_allow;
  };

}

#endif
