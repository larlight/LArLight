/**
 * \file DBIHandler.hh
 *
 * \ingroup UBOnlineDBI
 * 
 * \brief Class def header for a class DBIHandler
 *
 * @author kazuhiro
 */

/** \addtogroup UBOnlineDBI

    @{*/
#ifndef DBIHANDLER_HH
#define DBIHANDLER_HH

#include <vector>
#include <libpq-fe.h>

#include "DBBase.hh"
#include "DBEnv.hh"
#include "DBException.hh"

namespace ubpsql {

  /**
     \class DBIHandler
     Utility class for handling database connection
  */
  class DBIHandler : public DBBase{

  private:

    /// Default constructor is private (pseudo-singleton)
    DBIHandler(DBI_USER_t user_type=kDBI_USER_MAX);

    /// Instance pointers holder
    static std::vector<DBIHandler*> _me_v;
    
  public:

    /// Instance pointer getter ... one can get an instance per user type
    static const DBIHandler* GetME(DBI_USER_t user_type) {

      if(!_me_v.at(user_type)) _me_v[user_type] = new DBIHandler(user_type);
      return _me_v.at(user_type);

    }
    
    /// Default destructor
    virtual ~DBIHandler(){};

    /**
       Setter for configuration string (configure host, database, user, and password in 1 string).
       Note you do not have to configure if the default connection info suffices except for
       kCUSTOM_USER which has no default connection info.
    */
    void Configure(const std::string config_string);

    /// Connection (re)open
    bool Connect();

    /// Connection close 
    bool Close();

    /// Connection status querry. If the argument is true, it prints out detailed status of connection.
    bool IsConnected(bool verbose=false);

    /// Getter for the connection info string
    const std::string GetConnectionInfo() const { return _conn_config; }

    /// Getter for current connection's user name
    const char* GetDBUser()
    { return ( _conn ? PQuser(_conn) : "" );}

    /// Getter for current connection's host name
    const char* GetDBHost()
    { return ( _conn ? PQhost(_conn) : "" );}

    /// Getter for current connection's database name
    const char* GetDBName()
    { return ( _conn ? PQdb(_conn) : "" );}

    /// Getter for current connection's password
    const char* GetDBPass()
    { return ( _conn ? PQpass(_conn) : "" );}

    /// Getter for shell user name of the connection's owner
    const char* GetShellUser()
    { return _user_shell.c_str();}

    /**
       Function to execute a query string. 
       The caller of this function is responsible for calling PQclear function to
       free the memory of PGresult* IF non-zero pointer is returned!
    */
    PGresult* Execute(const std::string &cmd);

  private:

    PGconn* _conn;            ///< PostgreSQL connection instance
    DBI_USER_t  _user_type;   ///< User type
    std::string _conn_config; ///< DB connection info string
    std::string _user_shell;  ///< Shell user name of this program executer

    /// Waiting time between re-connection trials in seconds
    unsigned char _retrial_period;

    /// Number of re-connection attempt
    unsigned char _retrial_number; 

    /// Waiting time for query processing in seconds
    unsigned char _query_timeout;
  };

}

#endif
/** @} */ // end of doxygen group 

