/**
 * \file RCWriter.hh
 *
 * \ingroup UBOnlineDBI
 * 
 * \brief RunControl DB interface class for writing into the database
 *
 * @author kazuhiro
 */

/** \addtogroup UBOnlineDBI

    @{*/
#ifndef RCWRITER_HH
#define RCWRITER_HH

#include "RCReader.hh"

namespace ubpsql {
  /**
     \class RCWriter
     RunControl DB interface for writing into the database.
     Inherits from RCReader to make use of read functions.
  */
  class RCWriter : public RCReader{
    
  public:
    
    /// Default constructor
    RCWriter() : RCReader() { _user_type = kUBDAQ_WRITER; };
    
    /// Default destructor
    virtual ~RCWriter(){};
    
  };
}

#endif
/** @} */ // end of doxygen group 

