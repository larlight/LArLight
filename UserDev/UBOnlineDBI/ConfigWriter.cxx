#ifndef CONFIGWRITER_CXX
#define CONFIGWRITER_CXX

#include "ConfigWriter.hh"

namespace ubpsql {

  bool ConfigWriter::CreateConfigType(const std::string config_name,
				      const std::vector<std::string> &stringkeylist)
  {
    if(!Connect()) return false;

    // Form a command string
    std::string cmd(Form("SELECT CreateConfigurationType('%s','",config_name.c_str()));

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

  bool ConfigWriter::InsertNewRun(unsigned int config_id)
  {
    if(!Connect()) return false;
    PGresult* res = _conn->Execute(Form("SELECT InsertNewRun(%d);",config_id));
    if(!res) return false;

    PQclear(res);
    return true;
  }

  bool ConfigWriter::InsertNewSubRun(unsigned int config_id, unsigned int run)
  {
    if(!Connect()) return false;
    PGresult* res = _conn->Execute(Form("SELECT InsertNewSubRun(%d,%d);",config_id,run));
    if(!res) return false;
    
    PQclear(res);
    return true;
  }

  bool ConfigWriter::InsertSubConfigValues(const ConfigData &data)
  {
    if(!Connect()) return false;
    std::string cmd(Form("SELECT InsertConfigurationSet('%s', %d, %d, %d, ",
			 data.Name().c_str(),
			 data.ID(),
			 data.Crate(),
			 data.Channel())
		    );
    cmd += "'";

    size_t ctr = data.size() - 1;
    for(auto hstore_iter = data.begin();
	hstore_iter != data.end();
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
