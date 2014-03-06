/**
 * \file RCReader.hh
 *
 * \ingroup UBOnlineDBI
 * 
 * \brief RunControl DB interface class for reading from the database
 *
 * @author kazuhiro
 */

/** \addtogroup UBOnlineDBI

    @{*/
#ifndef RCREADER_HH
#define RCREADER_HH

#include "DBIHandler.hh"

namespace ubpsql {
  /**
     \class RCReader
     RunControl DB interface class for reading from the database.
     To write something into the database, one should use RCWriter.
  */
  class RCReader{
    
  public:
    
    /// Default constructor
    RCReader(){ _user_type = kUBDAQ_READER; _conn=0; };
    
    /// Default destructor
    virtual ~RCReader(){};

    /// Method to make a connection to the database
    bool Connect();

  protected:

    DBI_USER_t _user_type;

    DBIHandler* _conn;
    
  };
}

#endif
/** @} */ // end of doxygen group 

