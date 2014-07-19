/**
 * \file ConfigReader.hh
 *
 * \ingroup UBOnlineDBI
 * 
 * \brief RunControl DB interface class for reading from the database
 *
 * @author kazuhiro
 */

/** \addtogroup UBOnlineDBI

    @{*/
#ifndef CONFIGREADER_HH
#define CONFIGREADER_HH

#include "DBAppBase.hh"
#include "ConfigData.hh"
namespace ubpsql {
  /**
     \class ConfigReader
     RunControl DB interface class for reading from the database.
     To write something into the database, one should use RCWriter.
  */
  class ConfigReader : public DBAppBase {
    
  public:
    
    /// Default constructor
    ConfigReader() : DBAppBase() 
    { _user_type = kCUSTOM_USER; }
    //{ _user_type = kUBDAQ_READER; }
    
    /// Default destructor
    virtual ~ConfigReader(){}

    /// Getter for last run number
    unsigned int GetLastRun();

    /// Getter for last sub-run number
    unsigned int GetLastSubRun(unsigned int run);

    /// Getter for a list of config ID from the DB
    std::vector<int> GetMainConfigIDs();

    /// Getter for a list of config ID for each sub-config types to a given RunConfigID
    std::map<std::string,int> GetRunConfig(int run_config_id);

    bool ExistSubConfig(std::string sub_config_name);

    /// Getter for a list of sub-config minimum set of parameters
    std::vector<std::string> GetSubConfigParameterNames(std::string const& sub_config_name);

    /// Getter for a list of sub-config parameter values
    /**
    std::map<std::string,std::string> GetSubConfigParameterValues(std::string  const& sub_config_name,
								  unsigned int const  sub_config_id,  
								  unsigned int const  crate_id);
    */
    /// Getter for the maximum sub-config ID number used 
    int MaxSubConfigID(std::string sub_config_name);

    /// Getter for a list of crate IDs
    std::vector<int> GetCrateIDs(std::string  const& sub_config_name,
				 unsigned int const sub_config_id);

    /// 
    //bool RCWriter::ExistConfigType(const std::string config_type);

    /// 
    //bool RCWriter::ExistConfigType(const std::string config_type,
    //				   const std::vector<std::string> &stringkeylist);

    
  protected:

    std::vector<std::string> _scfg_type_2_name;

    
    
  };
}

#endif
/** @} */ // end of doxygen group 

