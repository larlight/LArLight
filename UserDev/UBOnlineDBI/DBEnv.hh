#ifndef DBENV_HH
#define DBENV_HH


#include <string>

namespace ubpsql{

  /// ENUM to define allowed connection user type
  enum DBI_USER_t {    
    kCUSTOM_USER=0,  ///< for custom user connection
    kUBDAQ_READER,   ///< for READER access to DAQ database
    kUBDAQ_WRITER,   ///< for WRITER access to DAQ database
    kDBI_USER_MAX
  };

  /// Default DB Connection Info String
  const std::string DBI_CONFIG[kDBI_USER_MAX] =
    {
      /// kCUSTOM_USER
      "host=localhost dbname=testdb user=kazuhiro connect_timeout=5",
      /// kUBDAQ_READER
      "host=192.168.144.189 dbname=rctestskazu user=uboonedaq_writer password=argon!uBooNE connect_timeout=5",
      /// kUBDAQ_WRITER
      "host=192.168.144.189 dbname=rctestskazu user=uboonedaq_writer password=argon!uBooNE connect_timeout=5"
    };

}

#endif
