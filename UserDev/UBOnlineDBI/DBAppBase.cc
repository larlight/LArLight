#ifndef DBAPPBASE_CC
#define DBAPPBASE_CC

#include "DBAppBase.hh"

namespace ubpsql {

  bool DBAppBase::Connect(){
    
    if(!_conn) _conn = (DBConn*)(DBConn::GetME(_user_type));

    if(_conn->Connect()) return true;

    throw ConnectionError();
    
    return false;
  }

}

#endif
