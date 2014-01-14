#include <Decoder-TypeDef.hh>
#include <Algorithm-TypeDef.hh>
#include <vector>
#include <TString.h>
#include <cstdlib>
void print_word(std::vector<PMT::word_t> *in_array);

int main(int argc, char** argv){


  if ( argc < 3)
    {
      std::cerr << std::endl
		<< Form("Usage: %s [FLAG] [ARG]",argv[0])  << std::endl << std::endl
		<< "Mandatory flags:"       << std::endl
		<< "-i [input file] ... specify input filename (full or relative path) here." << std::endl
		<< "-n [event id]   ... specify the target event id." << std::endl
		<< std::endl
		<< "Optional flags:"        << std::endl
		<< "-f [format]     ... specify input file format. 0=binary (default), 1=ASCII." << std::endl

		<< "-c              ... when specified, continues to EOF even after target event is found."<<std::endl
		<< std::endl;
      exit(1);
    }
  
  // Make this false if you wanna break after finding the target event.
  // Why make it true? => it helps to find possibly duplicated event id.
  bool read_through=false; 
  bool bin_format=true;
  
  bin_io_handler fin;
  fin.set_mode(bin_io_handler::READ);
  fin.set_format(FORMAT::BINARY);
  
  PMT::word_t target_id = PMT::INVALID_WORD;

  char c;
  while ( (c=getopt(argc,argv,"i:f:n:c")) != -1 ) {

    switch (c) {
    case 'i': // input file location
      fin.set_filename(optarg);
      Message::send(MSG::NORMAL,__FUNCTION__,Form("Input file: %s",optarg));
      break;
    case 'f': // input format
      if(std::atoi(optarg)==0) {
	fin.set_format(FORMAT::BINARY);
	Message::send(MSG::NORMAL,__FUNCTION__,"Set format: BINARY");
      }else if(std::atoi(optarg)==1){
	fin.set_format(FORMAT::ASCII);
	Message::send(MSG::NORMAL,__FUNCTION__,"Set format: ASCII");
      }else{
	Message::send(MSG::ERROR,__FUNCTION__,
		      Form("Invalid argument for -f: %s",optarg));
	exit(1);
      }	 
      break;
    case 'n': // target event_id
      target_id=(PMT::word_t)(std::atoi(optarg));
      Message::send(MSG::NORMAL,__FUNCTION__,
		    Form("Set target event ID: %d",target_id));
      break;
    case 'c':
      read_through=true;
      Message::send(MSG::NORMAL,__FUNCTION__,
		    "Throughout mode set ... continue scanning till the EOF.");
      break;
    }
  }
  
  if(target_id==PMT::INVALID_WORD){
    Message::send(MSG::ERROR,__FUNCTION__,"Event ID not specified!");
    exit(1);
  }

  target_id = (0xf0000000 + ((target_id & 0xfff) << 16) + 0x0000f000 + ((target_id >> 12) & 0xfff));
  
  const PMT::word_t key_header=0xffffffff;

  std::vector<PMT::word_t> word_array(4,0);
  PMT::word_t word=0x0;
  bool fire=false;
  fin.open();
  
  if(!fin.is_open()) exit(1);
  
  while(1){

    word = fin.read_word();

    if(fin.eof()) break;
    if(fire && word==key_header) {

      word_array.push_back(word);

      if(!read_through) break;

      print_word(&word_array);

      fire=false;
      word_array.clear();
      word_array.reserve(4);
      word_array.push_back(0);
      word_array.push_back(0);
      word_array.push_back(0);
      word_array.push_back(0);
    }

    if(!fire){
      word_array[0]=word_array[1];
      word_array[1]=word_array[2];
      word_array[2]=word_array[3];
      word_array[3]=word;
      
      if(word_array[0]==key_header && word_array[3]==target_id){
	fire=true;
      }
    }else
      word_array.push_back(word);
  }

  if(!read_through){
    print_word(&word_array);
  }

  fin.close();
}

void print_word(std::vector<PMT::word_t> *in_array){

  algo_xmit_decoder algo;  
  size_t ctr=0;
  std::string msg("");

  std::vector<PMT::word_t> word_array;
  word_array.reserve(in_array->size() * 2);
  for(std::vector<PMT::word_t>::const_iterator iter(in_array->begin());
      iter!=in_array->end();
      ++iter){
    word_array.push_back( (*iter) & 0xffff);
    word_array.push_back( (*iter) >> 16 );
  }

  for(std::vector<PMT::word_t>::const_iterator iter(word_array.begin());
      iter!=word_array.end();
      ++iter){

    PMT::PMT_WORD word_type=algo.get_word_class((*iter));
    
    switch(word_type){
    case PMT::UNDEFINED_WORD:
    case PMT::CHANNEL_WORD:
      msg+=Form(" %04x ",(*iter));
      break;
    case PMT::EVENT_FIRST_HEADER:
    case PMT::EVENT_HEADER:
    case PMT::EVENT_LAST_WORD:
      msg+=" \033[93m";
      msg+=Form("%04x ",(*iter));
      msg+="\033[0m";
	break;
    case PMT::FIRST_WORD:
    case PMT::FEM_LAST_WORD:
      msg+=" \033[91m";
      msg+=Form("%04x ",(*iter));
      msg+="\033[0m";
      break;
    case PMT::CHANNEL_HEADER:
    case PMT::CHANNEL_LAST_WORD:
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


