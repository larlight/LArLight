//
// type-def file for ROOT
//

#ifndef UBONLINEDBI_TYPEDEF_HH
#define UBONLINEDBI_TYPEDEF_HH

// Header files included here
#include "DBEnv.hh"
#include "DBBase.hh"
#include "DBException.hh"
#include "DBConn.hh"
#include "RCReader.hh"
#include "RCWriter.hh"

// Class forward declaration here
namespace ubpsql{

  // Basic classes
  class DBBase;          // Base class (has message function)
  class DBException;     // Exception base
  class InvalidUser;     // Exception implementation for invalid user
  class ConnectionError; // Exception implementation for connection error

  // Connection handler
  class DBConn;

  // Application classes
  class DBAppBase;       // Base class for application classes
  class RCReader;        // RunControl database read interface
  class RCWriter;        // RunControl database write interface

  //ADD_EMPTY_CLASS ... do not change this comment line

}

#endif




