/**
 * \file TableBase.hh
 *
 * \ingroup ProcessDB
 * 
 * \brief Class def header for a class TableBase
 *
 * @author kterao
 */

/** \addtogroup ProcessDB

    @{*/

#ifndef TABLEBASE_HH
#define TABLEBASE_HH

#include <TTimeStamp.h>
#include "ProcessDBIManager.hh"

/**
   \class TabelBase
   The base class for SQL table definition. 
   It has a minimum set of shared utitlity functions.
*/
class TableBase {

public:

  /// Default constructor
  TableBase();

  /// Default destructor
  virtual ~TableBase(){};  

  /// Method to create a table if not exists yet ... TO BE IMPLEMENTED BY DAUGHTERS
  bool CreateTable() const;

  /// Method to remove a table
  bool DropTable() const;

  /// Table name getter
  virtual std::string GetTableName(bool with_db=false) const;

  /// A method to query & dump result
  void QueryAndDump(std::string cmd) const;

  /// Table existence query
  bool ExistTable(std::string name="") const;

  /// Open (if not) & retrieve DBI connection instance
  bool Connect(DB::DB_t     type,
	       std::string  host,
	       unsigned int port=0,
	       std::string  db="",
	       std::string  user="",
	       std::string passwd="");

  /// Setter for a connection key
  bool SetConnKey(size_t key);

  /// A function to reset all
  virtual void Initialize();

protected:

  /// Internal getter function for connection handle
  inline ProcessDBI* GetConnection() const
  { return (ProcessDBIManager::GetME()->GetConnection(_conn_key));};

  std::string _db;         ///< database name ... stored to return table name w/ databse
  std::string _tablename;  ///< bare table name
  std::string _table_def;  ///< bare table definition string
  size_t _conn_key;        ///< a connection key to retrieve ProcessDBI pointer through ProcessDBIManager::GetConnection function

};

#endif

/** @} */ // end of doxygen group 
