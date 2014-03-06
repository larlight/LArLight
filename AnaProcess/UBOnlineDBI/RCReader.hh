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

#include "DBAppBase.hh"

namespace ubpsql {
  /**
     \class RCReader
     RunControl DB interface class for reading from the database.
     To write something into the database, one should use RCWriter.
  */
  class RCReader : public DBAppBase {
    
  public:
    
    /// Default constructor
    RCReader() : DBAppBase() { _user_type = kUBDAQ_READER; };
    
    /// Default destructor
    virtual ~RCReader(){};

  };
}

#endif
/** @} */ // end of doxygen group 

