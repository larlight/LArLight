/**
 * \file DBAppBase.hh
 *
 * \ingroup UBOnlineDBI
 * 
 * \brief Base class for DB application class
 *
 * @author kazuhiro
 */

/** \addtogroup UBOnlineDBI

    @{*/
#ifndef DBAPPBASE_HH
#define DBAPPBASE_HH

#include <TString.h>

#include "DBConn.hh"

namespace ubpsql {
  /**
     \class DBAppBase
     Base class for DB interface application classes
  */
  class DBAppBase : public DBBase{
    
  public:
    
    /// Default constructor
    DBAppBase(){ _user_type = kCUSTOM_USER; _conn=0; };
    
    /// Default destructor
    virtual ~DBAppBase(){};

    /// Method to make a connection to the database
    bool Connect();

  protected:

    DBI_USER_t _user_type;

    DBConn* _conn;
    
  };
}

#endif
/** @} */ // end of doxygen group 

