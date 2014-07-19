#ifndef CONFIGREADER_CXX
#define CONFIGREADER_CXX

#include "ConfigReader.hh"

namespace ubpsql {

  unsigned int ConfigReader::GetLastRun()
  {
    if(!Connect()) return false;
    PGresult* res = _conn->Execute("SELECT GetLastRun();");
    if(!res) return std::numeric_limits<unsigned int>::max();

    unsigned int run = atoi(PQgetvalue(res,0,0));
    PQclear(res);
    return run;
  }

  unsigned int ConfigReader::GetLastSubRun(unsigned int run)
  {
    if(!Connect()) return false;
    PGresult* res = _conn->Execute(Form("SELECT GetLastSubRun(%d);",run));
    if(!res) return std::numeric_limits<unsigned int>::max();

    unsigned int subrun = atoi(PQgetvalue(res,0,0));
    PQclear(res);
    return subrun;
  }

  std::vector<int> ConfigReader::GetMainConfigIDs()
  {
    std::vector<int> config_ids;
    if(!Connect()) return config_ids;
    PGresult* res = _conn->Execute("SELECT DISTINCT ConfigID FROM MainConfigTable ORDER BY ConfigID;");
    if(!res)  return config_ids;

    for(size_t i=0; i<PQntuples(res); ++i)
      config_ids.push_back(atoi(PQgetvalue(res,i,0)));

    return config_ids;
  }

  std::map<std::string,int> ConfigReader::GetRunConfig(int run_config_id)
  {
    std::map<std::string,int> config_ids;
    if(!Connect()) return config_ids;
    PGresult* res = _conn->Execute(Form("SELECT ListSubConfigNameAndID('%d');",run_config_id));
    if(!res) return config_ids;

    for(size_t i=0; i<PQntuples(res); ++i) {

      int subconfig_id = atoi(PQgetvalue(res,i,1));
      std::string name = PQgetvalue(res,i,0);

      if(config_ids.find(name) != config_ids.end())

	throw TableDataError();

      else

	config_ids.insert(std::make_pair(name,subconfig_id));
      
    }

    return config_ids;    
  }  

  bool ConfigReader::ExistSubConfig(std::string sub_config_name)
  {
    if(!Connect()) return false;
    PGresult* res = _conn->Execute(Form("SELECT ExistSubConfig('%s');",sub_config_name.c_str()));
    if(!res) return false;

    return (bool)(std::atoi(PQgetvalue(res,0,0)));
  }

  std::vector<std::string> ConfigReader::GetSubConfigParameterNames(std::string const& sub_config_name)
  {
    std::vector<std::string> params;
    if(!Connect()) return params;
    PGresult* res = _conn->Execute(Form("SELECT ListSubConfigParameters('%s');",sub_config_name.c_str()));
    if(!res) return params;
    
    for(size_t i=0; i<PQntuples(res); ++i)
      params.push_back(PQgetvalue(res,i,0));
    return params;
  }

  int ConfigReader::MaxSubConfigID(std::string sub_config_name)
  {
    int subconfig_id=-2;
    if(!Connect()) return subconfig_id;
    PGresult* res = _conn->Execute(Form("SELECT MaxSubConfigID('%s');",sub_config_name.c_str()));
    if(!res) return subconfig_id;

    subconfig_id = std::atoi(PQgetvalue(res,0,0));

    return subconfig_id;
  }
 
  std::vector<int> ConfigReader::GetCrateIDs(std::string  const& sub_config_name,
					 unsigned int const sub_config_id)
  {
    std::vector<int> crates;
    if(!Connect()) return crates;
    PGresult* res = _conn->Execute(Form("SELECT ListCrateID('%s',%d);",sub_config_name.c_str(),sub_config_id));
    if(!res) return crates;

    for(size_t i=0; i<PQntuples(res); ++i)
      crates.push_back(std::atoi(PQgetvalue(res,i,0)));

    return crates;
  }

}

#endif
