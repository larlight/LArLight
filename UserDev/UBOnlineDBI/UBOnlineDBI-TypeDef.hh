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
#include "ConfigReader.hh"
#include "ConfigWriter.hh"

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
  class ConfigReader;    // Run configuration database read interface
  class ConfigWriter;    // Run configuration database write interface

  class ConfigData;
  //ADD_EMPTY_CLASS ... do not change this comment line

}

#endif





