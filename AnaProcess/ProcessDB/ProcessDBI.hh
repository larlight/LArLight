/**
 * \file ProcessDBI.hh
 *
 * \ingroup ProcessDB
 * 
 * \brief Class def header for a class ProcessDBI
 *
 * @author kterao
 */

/** \addtogroup ProcessDB

    @{*/
#ifndef PROCESSDBI_HH
#define PROCESSDBI_HH


#include <iostream>
#include <TSystem.h>
#include <TSQLServer.h>
#include <TSQLStatement.h>
#include "MonConsts.h"

namespace procdb {

  /**
     \class ProcessDBI
     Database interface class for tasks related to processing.
     A user can connect to the database server and try custom query using this class.
     If one wants to interface with a database, however, better have his/her own scripts as there are
     many useful APIs available.
     This class is meant to be used by a custom table class which should be a useful interface for users.
  */
  class ProcessDBI{
    
  public:
    
    /// Default constructor ... by default the type is set to PostgreSQL for uboone
    ProcessDBI();
    
    /// Default destructor
    virtual ~ProcessDBI(){};
    
    /// Getter for SQL db type
    DB::DB_t ServerType() const {return _type;};
    
    /// Flag to download query result to a local memory
    void StoreResult(bool store=true) {_download_res=store;};
    
    /// A method to connect to the server
    bool Connect(DB::DB_t type,
		 std::string host,
		 unsigned int port=0,
		 std::string db="",
		 std::string user="",
		 std::string passwd="");
    
    /// A method to reconnect using previously provided connection information in ProcessDBI::Connect method
    bool Reconnect();
    
    /// A method to clear results
    void ClearResult(size_t index);
    
    /// A method to get # of Query results so far
    size_t GetResCount() const {return _res_v.size();};
    
    /// A method to disconnect
    void Disconnect();
    
    /// A method to check if a valid connection exists or not
    bool IsConnected() const;
    
    /// A method to execute SQL query which expects no value return. The return boolean is true if query is successful.
    bool Exec(std::string cmd);
    
    /// A method to set number of re-trial upon ProcessDBI::Reconnect call
    void SetRetrialN(unsigned short n) {_ntrial=n;};
    
    /// A method to run a query which expects an output. Here a key is given to access the result of Query through ProcessDBI::GetRow or ProcessDBI::FetchRow function call
    size_t Query(std::string cmd);
    
    /// A method to check if the provided result key is valid or not
    bool IsValidKey(size_t index) const {return (index < _res_v.size());};
    
    /// A method to check if the result with a specified key is valid or not
    bool IsValidResult(size_t index) const
    {  return IsValidKey(index) ? (bool)(_res_v.at(index)) : false; };
    
    /// A method to fetch a new row from the query result
    TSQLStatement* FetchRow(size_t index);
    
    /// A method to get a row from the query result
    TSQLStatement* GetRow(size_t index) const;
    
    /// A method to get the number of affected row count by the previous query.
    int GetRowCount(size_t index) const;
    
    /// A method to get the number of field in the specified query result.
    int GetFieldCount(size_t index) const;
    
    /// A method to return the connected server address (db type + host + port + db)
    std::string GetServerName() const {return _server;};
    
    /// A method to return the connected user name
    std::string GetUserName() const {return _user;};
    
  private:
    
    TSQLServer*    _conn;   ///< SQL connection instance
    DB::DB_t       _type;   ///< Type of SQL server
    unsigned short _ntrial; ///< Number of trial to establish a connection upon Reconnect() call
    std::string    _server; ///< Connection server (host+port+db) info stored upon Connect() call
    std::string    _user;   ///< Connection user name stored upon Connect() call
    std::string    _passwd; ///< Connection password stored upon Connect() call
    bool      _download_res; ///< Store query result locally upon success 
    /// Vector of TSQLStatement pointer created upon a successful Query() function call
    std::vector<TSQLStatement*> _res_v; 
    
  };
}
#endif
/** @} */ // end of doxygen group 
  
