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

#include "ProcessDBIManager.hh"

/**
   \class TabelBase
   The base class for SQL table definition. 
   It has a minimum set of shared utitlity functions.
*/
class TableBase {

public:

  /// Default constructor
  TableBase(std::string name);

  /// Default destructor
  virtual ~TableBase(){};  

  /// Table name getter
  virtual std::string GetTableName(bool with_db=false) const;

  /// A method to query database
  size_t Query(std::string cmd) const;

  /// A method to query & dump result
  void QueryAndDump(std::string cmd) const;

  /// Table existence query
  bool ExistTable(std::string name="") const;

  /// Table creation query
  bool CreateTable() const;

  /// Open (if not) & retrieve DBI connection instance
  bool Connect(std::string host,unsigned int port=0,std::string db="",std::string user="",std::string passwd="");

  /// Setter for a connection key
  bool SetConnKey(size_t key);

  /// A function to reset all
  virtual void Initialize();

protected:
  
  inline ProcessDBI* GetConnection() const;

  std::string _db;         ///< database name ... stored to return table name w/ databse
  std::string _tablename;  ///< bare table name
  size_t _conn_key;        ///< a connection key to retrieve ProcessDBI pointer through ProcessDBIManager::GetConnection function
  
};

#endif

/** @} */ // end of doxygen group 
