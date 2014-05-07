#ifndef DBBASE_CXX
#define DBBASE_CXX

#include "DBBase.hh"

namespace ubpsql {

  void DBBase::Print(const MSG::MSGLevel_t level,
		     const std::string& msg1,
		     const std::string& msg2) const 
  { 
    if(level < _msg_level) return;

    switch(level){
    case MSG::kDEBUG:
      std::cout << "\033[94m " << " [DEBUG]  " << "\033[0m "; break;
    case MSG::kINFO:
      std::cout << "\033[92m " << " [INFO]   " << "\033[0m "; break;
    case MSG::kNORMAL:
      std::cout << "\033[95m " << " [NORMAL] " << "\033[0m "; break;
    case MSG::kWARNING:
      std::cout << "\033[93m " << " [WARNING]" << "\033[0m "; break;
    case MSG::kERROR:
      std::cout << "\033[91m " << " [ERROR]  " << "\033[0m "; break;
    }
    if(msg2.empty())
      
      std::cout << msg1.c_str() << std::endl;

    else
      std::cout << "<< " << msg1.c_str() << " >> " 
		<< msg2.c_str() << std::endl;
    
  }

}

#endif
