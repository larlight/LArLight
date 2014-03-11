#ifndef RCREADER_CC
#define RCREADER_CC

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


}

#endif
