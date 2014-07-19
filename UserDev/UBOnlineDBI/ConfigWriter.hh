/**
 * \file ConfigWriter.hh
 *
 * \ingroup UBOnlineDBI
 * 
 * \brief RunControl DB interface class for writing into the database
 *
 * @author kazuhiro
 */

/** \addtogroup UBOnlineDBI

    @{*/
#ifndef CONFIGWRITER_HH
#define CONFIGWRITER_HH

#include "ConfigReader.hh"

namespace ubpsql {
  /**
     \class ConfigWriter
     RunControl DB interface for writing into the database.
     Inherits from ConfigReader to make use of read functions.
  */
  class ConfigWriter : public ConfigReader{
    
  public:
    
    /// Default constructor
    ConfigWriter() : ConfigReader()
    { _user_type = kCUSTOM_USER; }

    /// Default destructor
    virtual ~ConfigWriter(){};

    /**
       Create New Configuration Type, i.e. PMTFEM, TPCFEM, Laser... to be used sparingly.
       Needs list of defined hstore keys that will later be checked via triggers when inserting new configs
       takes list of strings that it then converts to hstore values. Needs to know the total number of channels 
     */
    bool CreateConfigType(const std::string config_name,
			  const std::vector<std::string> &stringkeylist);

    /// Create a new run number in the database
    bool InsertNewRun(unsigned int config_id);

    /// Create a new sub run number in the database
    bool InsertNewSubRun(unsigned int config_id, unsigned int run);

    bool InsertSubConfigValues(const ConfigData &data);
      
  };
}

#endif
/** @} */ // end of doxygen group 

