#ifndef BIN_WORD_SEARCH_HH
#define BIN_WORD_SEARCH_HH

#include "bin_io_handler.hh"
#include <deque>
#include <TString.h>

namespace larlight {

  class bin_word_search : public larlight_base {
    
  public:
    bin_word_search();
    ~bin_word_search(){};
    
    enum WORD_BYTES {INT, SHORT};
    
    void set_target_word(UInt_t word){_target=word;};
    
    void set_filename(std::string name){_fin.set_filename(name);};
    
    void set_continue_mode(Bool_t mode){_continue_mode=mode;};
    
    void set_format(FORMAT::INPUT_FILE fmt){_fin.set_format(fmt);};
    
    void set_nwords(size_t n){_nwords=n;};
    
    void set_word_bytes(WORD_BYTES bytes){_bytes=bytes;};
    
    Bool_t run();
    
    void print_words(std::deque<UInt_t> *words);

  private:
    
    bin_io_handler _fin;
    UInt_t _target;
    Bool_t _continue_mode;
    size_t  _nwords;
    WORD_BYTES _bytes;
  };
}
#endif
