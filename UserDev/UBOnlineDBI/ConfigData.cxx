#ifndef CONFIGDATA_CXX
#define CONFIGDATA_CXX

#include "ConfigData.hh"

namespace ubpsql {
  
  ConfigData::ConfigData(std::string name,
			 int id,
			 int crate,
			 int channel)
  {
    if( name.empty() || id < 0 || crate < 0 || channel < 0 )

      throw TableDataError();
    
    fName    = name;
    fID      = id;
    fCrate   = crate;
    fChannel = channel;
  }

  void ConfigData::ls() const
  {
    std::cout
      << std::endl
      << "Sub-Configuration \"" << fName.c_str() << "\"" << std::endl
      << "ID      : " << fID << std::endl
      << "Crate   : " << fCrate << std::endl
      << "Channel : " << fChannel << std::endl
      << std::endl
      << "... with following parameter values:" << std::endl;

      for(auto const& v : *this)

	std::cout << v.first.c_str() << " ... " << v.second.c_str() << std::endl;
    
      std::cout<<std::endl;
  }

  std::string ConfigData::FhiclDump() const
  {
    std::string fcl("");

    fcl += fName + ":\n{\n";

    for(auto const& param : *this)

      fcl += param.first + ": " + param.second + "\n";

    fcl += "}\n";
    
    return fcl;

  }


}

#endif
