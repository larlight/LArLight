#ifndef ALGO_SN_TPC_HUFFMAN_HH
#define ALGO_SN_TPC_HUFFMAN_HH

#include "algo_tpc_huffman.hh"

namespace larlight {

  class algo_sn_tpc_huffman : public algo_tpc_huffman {

  public:

    algo_sn_tpc_huffman();

    virtual ~algo_sn_tpc_huffman(){}

    /// Override of algo_base::get_word_class method
    virtual FEM::FEM_WORD get_word_class(const UInt_t word) const;

    virtual bool process_word(const UInt_t word);

  protected:  

    // virtual bool process_fem_header
    // (const UInt_t word,UInt_t &last_word) override;

    virtual bool process_event_last_word
    (const UInt_t word,UInt_t &last_word);

    virtual bool process_ch_word
    (const UInt_t word,UInt_t &last_word);

  };

}

#endif
