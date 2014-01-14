#ifndef BIN_WORD_SEARCH_CC
#define BIN_WORD_SEARCH_CC

#include "bin_word_search.hh"

namespace larlight {

  bin_word_search::bin_word_search(){
    
    _continue_mode = false;
    _target = FEM::INVALID_WORD;
    _nwords=20;
    _bytes = SHORT;
  }
  
  Bool_t bin_word_search::run(){
    
    _fin.set_verbosity(MSG::ERROR);
    _fin.open();
    
    if(!_fin.is_open()) return false;

    UInt_t word1,word2;

    const size_t max_nwords = _nwords*2+1;
    
    std::deque<UInt_t> words(max_nwords);
    
    size_t total_word_counter = 0;
    size_t after_word_counter = 0;
    
    while(1){
      
      word1 = _fin.read_word();
      
      if(_fin.eof()) break;
      
      if(total_word_counter >= max_nwords)
	words.pop_front();
      
      if(_bytes == INT){
	
	words.push_back(word1);
	
	total_word_counter++;
	
	if(word1 == _target || after_word_counter>0)
	  after_word_counter++;
	
	if(after_word_counter == (_nwords+1)) {
	  if(!_continue_mode) break;
	  else{
	    print_words(&words);
	    after_word_counter = 0;
	  }
	}
      }
      else {
	
	word2 = (word1>>16);
	word1 = (word1 & 0xffff);
	
	words.push_back(word1);
	total_word_counter++;
	
	if(word1 == _target || after_word_counter>0)
	  after_word_counter++;
	
	if(after_word_counter == (_nwords+1)) {
	  if(!_continue_mode) break;
	  else{
	    print_words(&words);
	    after_word_counter = 0;
	  }
	}
	
	words.push_back(word2);
	total_word_counter++;
	if(word2 == _target || after_word_counter>0)
	  after_word_counter++;
	
	if(after_word_counter == (_nwords+1)) {
	  if(!_continue_mode) break;
	  else{
	    print_words(&words);
	    after_word_counter = 0;
	  }
	}
      }
      
    }
    
    if(after_word_counter) print_words(&words);
    
    return true;
  }
  
  void bin_word_search::print_words(std::deque<UInt_t> *words){
    
    int output_word_counter = 0;
    std::string msg;
    for(std::deque<UInt_t>::const_iterator iter(words->begin());
	iter != words->end();
	++iter){
      
      if((*iter) == _target)
	msg += "\033[95m";
      
      if(_bytes == INT) msg += Form("%08x ",(*iter));
      else              msg += Form("%04x ",(*iter));
      
      if((*iter) == _target)
	msg += "\033[0m";
      
      output_word_counter++;
      
      if(output_word_counter%8 == 0)
	msg+="\n";
    }
    
    std::cout<<msg.c_str()<<std::endl<<std::endl;
    
  }
}
#endif
