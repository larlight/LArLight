//
// Example C++ routine to run your analysis module, Looter.
//

#include "Looter-TypeDef.hh"
#include <iostream>
#include <vector>
#include <deque>
#include <string>

std::string gAppname;

void usage()
{
  std::cout << "  " << gAppname << "  Usage: input.root input2.root output.root" << std::endl;
}

int main(int argc, char** argv){
  using namespace std;
  deque<string> args;
  gAppname = argv[0];
  for(int i=1;i<argc;i++) args.push_back(argv[i]);
  
  vector<string> filenames;
  while(args.size()) {
    string arg = args.front(); args.pop_front();
    filenames.push_back(arg);
  }
  
  string outfilename = filenames.back(); filenames.pop_back();
  
  if(filenames.size()==0) usage();
  
  larlight::storage_manager my_storage;
  //my_storage.set_verbosity(larlight::MSG::DEBUG);
  my_storage.set_io_mode(larlight::storage_manager::WRITE);
  my_storage.set_out_filename(outfilename);
  my_storage.open();

  // Step 3: Check if a file is opened.
  if(!my_storage.is_open()) {
    std::cerr << "File open failed!" << std::endl;
    return 1;
  }
  if(!my_storage.is_ready_io()) {
    std::cerr << "I/O preparation failed!" << std::endl;
    return 1;
  }
  
  for(size_t i=0;i<filenames.size();i++) {
    Looter looter(filenames[i]);
    looter.loot(my_storage);
  }
  
  my_storage.close();

  return 0;
}
