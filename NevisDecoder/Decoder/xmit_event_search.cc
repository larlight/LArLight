#ifndef XMIT_EVENT_SEARCH_CC
#define XMIT_EVENT_SEARCH_CC

#include "xmit_event_search.hh"

namespace larlight {

  const size_t xmit_event_search::XMIT_INDEX_EVENT_ID = 3;
  const size_t xmit_event_search::SLOW_INDEX_EVENT_ID = 2;

  xmit_event_search::xmit_event_search() : 
    larlight_base() 
  {
    _continue_mode=false;
    _target_id=FEM::INVALID_WORD;
    _fin.set_format(FORMAT::BINARY);
    _fin.set_mode(bin_io_handler::READ);
    _read_by_block=true;
    _index_event_id=0x0;
    _slow_readout=false;
    _algo = 0;

  }

  bool xmit_event_search::run() {

    if(_target_id==FEM::INVALID_WORD){
      Message::send(MSG::ERROR,__FUNCTION__,"Event ID not specified!");
      return false;
    }

    _index_event_id = _slow_readout ? SLOW_INDEX_EVENT_ID : XMIT_INDEX_EVENT_ID;

    if(_algo) delete _algo;

    _algo = (_slow_readout) ? (new algo_slow_readout_decoder) : (new algo_xmit_decoder);  

    _target_id = (0xf0000000 + ((_target_id & 0xfff) << 16) + 0x0000f000 + ((_target_id >> 12) & 0xfff));

    std::vector<UInt_t> word_array(4,0);
    UInt_t word=0x0;
    bool fire=false;

    _fin.set_verbosity(MSG::ERROR);
    _fin.open();

    if(!_fin.is_open()) return false;

    while(1){

      word = (_read_by_block) ? _fin.read_multi_word() : _fin.read_word();
      word = _fin.read_word();

      if(_fin.eof()) break;

      if(fire && new_event(word)){

	word_array.push_back(word);

	if(!_continue_mode) break;

	print_word(&word_array);

	fire=false;
	word_array.clear();
	word_array.reserve(_index_event_id+1);
	for(size_t i=0; i<=_index_event_id; i++)
	  word_array.push_back(0x0);
      }

      if(!fire){

	for(size_t i=0; i<_index_event_id; i++)
	  word_array[i]=word_array[i+1];

	word_array[_index_event_id] = word;

	if(new_event(word_array[0]) && word_array[_index_event_id]==_target_id){
	  fire=true;
	}
      }else
	word_array.push_back(word);
    }

    if(!_continue_mode){
      print_word(&word_array);
    }

    _fin.close();
    return true;
  }

  void xmit_event_search::print_word(std::vector<UInt_t> *in_array){

    size_t ctr=0;
    std::string msg("");

    std::vector<UInt_t> word_array;
    word_array.reserve(in_array->size() * 2);
    for(std::vector<UInt_t>::const_iterator iter(in_array->begin());
	iter!=in_array->end();
	++iter){
      word_array.push_back( (*iter) & 0xffff);
      word_array.push_back( (*iter) >> 16 );
    }

    for(std::vector<UInt_t>::const_iterator iter(word_array.begin());
	iter!=word_array.end();
	++iter){

      FEM::FEM_WORD word_type=_algo->get_word_class((*iter));

      switch(word_type){
      case FEM::UNDEFINED_WORD:
      case FEM::CHANNEL_WORD:
	msg+=Form(" %04x ",(*iter));
	break;
      case FEM::EVENT_HEADER:
      case FEM::FEM_HEADER:
      case FEM::EVENT_LAST_WORD:
	msg+=" \033[93m";
	msg+=Form("%04x ",(*iter));
	msg+="\033[0m";
	  break;
      case FEM::FEM_FIRST_WORD:
      case FEM::FEM_LAST_WORD:
	msg+=" \033[91m";
	msg+=Form("%04x ",(*iter));
	msg+="\033[0m";
	break;
      case FEM::CHANNEL_HEADER:
      case FEM::CHANNEL_LAST_WORD:
	msg+=" \033[95m";
	msg+=Form("%04x ",(*iter));
	msg+="\033[0m";
	break;
      }
      ctr+=1;
      if(ctr && ctr%8==0){
	std::cout<<msg.c_str()<<std::endl;
	msg="";
      }
    }
    if(msg.size()>0) std::cout<<msg.c_str()<<std::endl;
    std::cout<<" ... continue to next event ... " <<std::endl;
  }

}


#endif
