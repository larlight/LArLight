#ifndef RCWRITER_CC
#define RCWRITER_CC

#include "RCWriter.hh"

namespace ubpsql {

  bool RCWriter::CreateConfigType(const std::string config_type,
				  const unsigned int nchannels,
				  const std::vector<std::string> stringkeylist)
  {
    if(!Connect()) return false;

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


  bool RCWriter::InsertNewRun(unsigned int config_id)
  {
    if(!Connect()) return false;
    PGresult* res = _conn->Execute(Form("SELECT InsertNewRun(%d);",config_id));
    if(!res) return false;

    PQclear(res);
    return true;
  }

  bool RCWriter::InsertNewSubRun(unsigned int config_id, unsigned int run)
  {
    if(!Connect()) return false;
    PGresult* res = _conn->Execute(Form("SELECT InsertNewSubRun(%d,%d);",config_id,run));
    if(!res) return false;
    
    PQclear(res);
    return true;
  }

}

#endif
