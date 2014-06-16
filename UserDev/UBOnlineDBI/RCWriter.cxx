#ifndef RCWRITER_CXX
#define RCWRITER_CXX

#include "RCWriter.hh"

namespace ubpsql {

  bool RCWriter::CreateConfigType(const std::string config_type,
				  const std::vector<std::string> &stringkeylist)
  {
    if(!Connect()) return false;

    // Form a command string
    std::string cmd(Form("SELECT CreateConfigurationType('%s','",config_type.c_str()));

    /*
    for(size_t index=0; index<stringkeylist.size(); ++index)

      cmd += Form(", \"f%zu\"=>\"%s\"",index,stringkeylist.at(index).c_str());
    */

    for(auto const& name : stringkeylist) {

      if(name == (*stringkeylist.begin()))
	cmd += Form(" \"%s\"=>\"\"",name.c_str());
      else
	cmd += Form(", \"%s\"=>\"\"",name.c_str());

    }
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

  bool RCWriter::InsertSubConfigValues(const std::string  config_type,
				       const unsigned int config_id,
				       const unsigned int crate,
				       const unsigned int channel,
				       const std::map<std::string,std::string> &hstore_values)
  {
    if(!Connect()) return false;
    std::string cmd(Form("SELECT InsertConfigurationSet('%s', %d, %d, %d, ",
			 config_type.c_str(),
			 config_id,
			 crate,
			 channel)
		    );
    cmd += "'";

    size_t ctr = hstore_values.size() - 1;
    for(auto hstore_iter = hstore_values.begin();
	hstore_iter != hstore_values.end();
	++hstore_iter) {

      if( ctr )
	cmd += Form(" \"%s\"=>\"%s\",", (*hstore_iter).first.c_str(), (*hstore_iter).second.c_str());
      else
	cmd += Form(" \"%s\"=>\"%s\"", (*hstore_iter).first.c_str(), (*hstore_iter).second.c_str());
      --ctr;
    }
    cmd += " ');";

    PGresult* res = _conn->Execute(cmd);
    if(!res) return false;
    
    PQclear(res);
    return true;
  }
}

#endif
