#ifndef RCWRITER_CC
#define RCWRITER_CC

#include "RCWriter.hh"

namespace ubpsql {

  bool RCWriter::CreateConfigType(const std::string config_type,
				  const unsigned int nchannels,
				  const std::vector<std::string> stringkeylist)
  {
    if(!Connect()) {
      Print(MSG::kERROR,__FUNCTION__,
	    "Failed to connect to the database...");
      return false;
    }

    // Form a command string
    std::string cmd(Form("SELECT CreateConfigurationType('%s',",config_type.c_str()));

    // Append hstore parameters
    cmd+=(Form("' \"nchannels\"=>\"%d\"",nchannels));
    for(size_t index=0; index<stringkeylist.size(); ++index)

      cmd += Form(", \"f%zu\"=>\"%s\"",index,stringkeylist.at(index).c_str());

    cmd += " ');";
    
    PGresult* res = _conn->Execute(cmd);
    if(!res) return false;
    PQclear(res);
    return true;
  }

}

#endif
