#ifndef RCREADER_CC
#define RCREADER_CC

#include "RCReader.hh"

namespace ubpsql {

  bool RCReader::Connect(){
    
    if(!_conn) _conn = (DBIHandler*)(DBIHandler::GetME(_user_type));

    return _conn->Connect();

  }

  
}

#endif
