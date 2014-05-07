#ifndef RCREADER_CXX
#define RCREADER_CXX

#include "RCReader.hh"

namespace ubpsql {

  /// Getter for last run number
  unsigned int RCReader::GetLastRun()
  {
    if(!Connect()) return false;
    PGresult* res = _conn->Execute("SELECT GetLastRun();");
    if(!res) return std::numeric_limits<unsigned int>::max();

    unsigned int run = atoi(PQgetvalue(res,0,0));
    PQclear(res);
    return run;
  }

  /// Getter for last sub-run number
  unsigned int RCReader::GetLastSubRun(unsigned int run)
  {
    if(!Connect()) return false;
    PGresult* res = _conn->Execute(Form("SELECT GetLastSubRun(%d);",run));
    if(!res) return std::numeric_limits<unsigned int>::max();

    unsigned int subrun = atoi(PQgetvalue(res,0,0));
    PQclear(res);
    return subrun;
  }

  /// Getter for Run Config list
  std::vector<int> RCReader::GetRunConfigIDList()
  {
    std::vector<int> config_ids;
    if(!Connect()) return config_ids;
    PGresult* res = _conn->Execute("SELECT DISTINCT ConfigID FROM MainConfigTable ORDER BY ConfigID;");
    if(!res)  return config_ids;

    for(size_t i=0; i<PQntuples(res); ++i)
      config_ids.push_back(atoi(PQgetvalue(res,i,0)));

    return config_ids;
  }

  /// Getter for Sub Config Type and ID
  std::map<int,int> RCReader::GetSubConfigIDList(int run_config_id)
  {
    std::map<int,int> config_ids;
    if(!Connect()) return config_ids;
    PGresult* res = _conn->Execute("SELECT SubConfigType, SubConfigID FROM MainConfigTable ORDER BY SubConfigType;");
    if(!res) return config_ids;

    for(size_t i=0; i<PQntuples(res); ++i) {

      int subconfig_type = atoi(PQgetvalue(res,i,0));
      int subconfig_id   = atoi(PQgetvalue(res,i,1));

      if(config_ids.find(subconfig_type) != config_ids.end())

	throw TableDataError();

      else

	config_ids.insert(std::pair<int,int>(subconfig_type,subconfig_id));
      
    }

    return config_ids;
    
  }  

}

#endif
