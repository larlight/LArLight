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

    /**
       Create New Configuration Type, i.e. PMTFEM, TPCFEM, Laser... to be used sparingly.
       Needs list of defined hstore keys that will later be checked via triggers when inserting new configs
       takes list of strings that it then converts to hstore values. Needs to know the total number of channels 
     */
    bool CreateConfigType(const std::string config_type,
			  const unsigned int nchannels,
			  const std::vector<std::string> stringkeylist);
    
    

  };
}

#endif
/** @} */ // end of doxygen group 

