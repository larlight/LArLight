#ifndef JOBQUEUETABLE_CC
#define JOBQUEUETABLE_CC

#include "JobQueueTable.hh"

const std::string JobQueueTable::GetTableDef(DB::DB_t type) const
{

  std::string table_def = "";
  switch(type){
  case DB::kMySQL:
    table_def += Form("%s INT NOT NULL AUTO_INCREMENT,",ProcDB::kQFieldName[ProcDB::kQueueID].c_str());
    break;
  case DB::kPostgreSQL:
    table_def += Form("%s SERIAL",ProcDB::kQFieldName[ProcDB::kQueueID].c_str());
    break;
  }
  table_def += Form("%s VARCHAR(200) NOT NULL,",ProcDB::kQFieldName[ProcDB::kQueueName].c_str());
  table_def += Form("%s VARCHAR(200) NOT NULL,",ProcDB::kQFieldName[ProcDB::kUserName].c_str());
  table_def += Form("%s VARCHAR(400) NOT NULL,",ProcDB::kQFieldName[ProcDB::kCMD].c_str());
  table_def += Form("%s TIMESTAMP NOT NULL,",ProcDB::kQFieldName[ProcDB::kStartTime].c_str());
  table_def += Form("%s INT NOT NULL,",ProcDB::kQFieldName[ProcDB::kPeriod].c_str());
  table_def += Form("%s INT NOT NULL,",ProcDB::kQFieldName[ProcDB::kIteration].c_str());
  table_def += Form("%s TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,",ProcDB::kQFieldName[ProcDB::kLogTime].c_str());
  
  return table_def;
}

#endif
