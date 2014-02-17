
#include "Base-TypeDef.hh"

int main(){

  std::cout 
    << std::endl
    << "This is \"Base\" package simple test routine." << std::endl
    << std::endl;

  // Test routine for Base package.
  
  // (1) simple: create larlight_base

  larlight::larlight_base me;
  me.set_verbosity(larlight::MSG::DEBUG);
  
  // (2) Send messages
  
  me.print(larlight::MSG::DEBUG,"... sending DEBUG message");
  me.print(larlight::MSG::INFO,"... sending INFO message");
  me.print(larlight::MSG::NORMAL,"... sending NORMAL message");
  me.print(larlight::MSG::WARNING,"... sending WARNING message");
  me.print(larlight::MSG::ERROR,"... sending ERROR message");

  std::cout
    << std::endl
    << "End of test routine." << std::endl
    << std::endl;

  return 0;

}
