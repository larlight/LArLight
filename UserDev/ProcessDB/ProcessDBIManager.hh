/**
 * \file ProcessDBIManager.hh
 *
 * \ingroup ProcessDB
 * 
 * \brief Class def header for a class ProcessDBIManager
 *
 * @author kterao
 */

/** \addtogroup ProcessDB

    @{*/
#ifndef PROCESSDBIMANAGER_HH
#define PROCESSDBIMANAGER_HH

#include <map>
#include <ProcessDBI.hh>

namespace procdb {
  /**
     \class ProcessDBIManager
     This class manages connections established to the database server using ProcessDBI.
     It is often the case that the host wants to limit # of connections from clients.
     This singleton class manages the connection information and instance as a pair to reduce a 
     duplication. Say one database table instance requests a connection to a server S1 under user name N1.
     If this connection is not yet established, it opens a connection and returns a key to access this 
     connection. Say another database table instance also requests a connection to a server S1 under user 
     name N1. Unless the previously established connection is already closed, this manager class instance 
     returns the already-established connection instead of opening a new one.   
     
     Using this manager instance, therefore, the SQL table class developper does not have to worry about 
     duplicating a connection instance.
  */
  class ProcessDBIManager {
    
  public :
    
    /// Getter function for the singleton pointer
    static ProcessDBIManager* GetME(){
      
      if(!_me) _me = new ProcessDBIManager;
      
      return _me;
      
    };
    
    /// Retrieve a specified connection
    ProcessDBI* GetConnection(size_t key) const;
    
    /// Open (if not) & retrieve DBI connection instance
    size_t Connect(DB::DB_t     type,
		   std::string  host,
		   unsigned int port=0,
		   std::string  db="",
		   std::string  user="",
		   std::string  passwd="");
    
    /// Close (if not) the specified connection 
    void Disconnect(size_t key);
    
    /// Close all connections
    void DisconnectAll();
    
  private:
    
    /// Default constructor
    ProcessDBIManager(){};
    
    /// Default destructor
    virtual ~ProcessDBIManager(){};
    
    /// A map holding server & user-name specific connection key
    std::map<std::string,std::map<std::string,size_t> > _conn_key;
    
    /// An array of connection instance (key=index in the aray)
    std::vector<ProcessDBI*> _conn_v;
    
    /// A singleton instance pointer
    static ProcessDBIManager* _me;
    
  };
}
#endif
/** @} */ // end of doxygen group 
