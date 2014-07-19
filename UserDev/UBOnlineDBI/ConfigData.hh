/**
 * \file ConfigData.hh
 *
 * \ingroup UBOnlineDBI
 * 
 * \brief Class def header for a class ConfigData
 *
 * @author kazuhiro
 */

/** \addtogroup UBOnlineDBI

    @{*/
#ifndef CONFIGDATA_HH
#define CONFIGDATA_HH

#include "DBException.hh"
#include <iostream>
#include <string>
#include <map>

namespace ubpsql {
  /**
     \class ConfigData
     User defined class ConfigData ... these comments are used to generate
     doxygen documentation!
  */
  class ConfigData : public std::map<std::string,std::string> {
    
  public:
    
    /// Default constructor
    ConfigData(std::string name="",
	       int id=-1,
	       int crate=-1,
	       int channel=-1);
    
    /// Default destructor
    virtual ~ConfigData(){};
    
    const std::string Name() const { return fName; }
    
    int ID()      const { return fID;      }
    int Crate()   const { return fCrate;   }
    int Channel() const { return fChannel; }

    std::string FhiclDump() const;

    void ls() const;

    void append(std::string key,
		std::string value)
    {
      this->insert(std::make_pair(key,value));
    }
    
  private:
    
    std::string fName;
    int         fID;
    int         fCrate;
    int         fChannel;
    
  };
}

#endif
/** @} */ // end of doxygen group 

