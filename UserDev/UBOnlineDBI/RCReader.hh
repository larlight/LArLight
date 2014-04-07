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

    /// Getter for last run number
    unsigned int GetLastRun();

    /// Getter for last sub-run number
    unsigned int GetLastSubRun(unsigned int run);

    /// Getter for a list of config ID from the DB
    std::vector<int> GetRunConfigIDList();

    /// Getter for a list of config ID for each sub-config types to a given RunConfigID
    std::map<int,int> GetSubConfigIDList(int run_config_id);
    

  };
}

#endif
/** @} */ // end of doxygen group 

