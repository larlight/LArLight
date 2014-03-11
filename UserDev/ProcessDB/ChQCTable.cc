#ifndef CHQCTABLE_CC
#define CHQCTABLE_CC

#include "ChQCTable.hh"

namespace procdb {
  
  ChQCTable::ChQCTable() : RunQCTable() 
  { 
    _tablename="ChQCTable"; 
    Initialize(); 
  }
  
  const std::string ChQCTable::GetTableDef(DB::DB_t type) const
  {
    
    std::string table_def = Form("%s SMALLINT NOT NULL,",QCDB::kQCFieldName[QCDB::kMonKey].c_str());
    table_def += Form("%s INT NOT NULL,",QCDB::kQCFieldName[QCDB::kRun].c_str());
    table_def += Form("%s INT NOT NULL,",QCDB::kQCFieldName[QCDB::kSubRun].c_str());
    table_def += Form("%s SMALLINT NOT NULL,",QCDB::kQCFieldName[QCDB::kChannel].c_str());
    switch(type){
    case DB::kMySQL:
      table_def += Form("%s DOUBLE NOT NULL,",QCDB::kQCFieldName[QCDB::kMean].c_str());
      table_def += Form("%s DOUBLE NOT NULL,",QCDB::kQCFieldName[QCDB::kSigma].c_str());
      break;
    case DB::kPostgreSQL:
      table_def += Form("%s DOUBLE PRECISION NOT NULL,",QCDB::kQCFieldName[QCDB::kMean].c_str());
      table_def += Form("%s DOUBLE PRECISION NOT NULL,",QCDB::kQCFieldName[QCDB::kSigma].c_str());
      break;
    }
    table_def += Form("%s VARCHAR(100) NOT NULL,",QCDB::kQCFieldName[QCDB::kReference].c_str());
    table_def += Form("%s TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,",QCDB::kQCFieldName[QCDB::kLogTime].c_str());
    table_def += Form("KEY (%s), KEY (%s), KEY (%s), KEY (%s), PRIMARY KEY (%s, %s, %s, %s)",
		      QCDB::kQCFieldName[QCDB::kMonKey].c_str(),
		      QCDB::kQCFieldName[QCDB::kRun].c_str(),
		      QCDB::kQCFieldName[QCDB::kSubRun].c_str(),
		      QCDB::kQCFieldName[QCDB::kChannel].c_str(),
		      QCDB::kQCFieldName[QCDB::kMonKey].c_str(),
		      QCDB::kQCFieldName[QCDB::kRun].c_str(),
		      QCDB::kQCFieldName[QCDB::kSubRun].c_str(),
		      QCDB::kQCFieldName[QCDB::kChannel].c_str());
    return table_def;
  }
  
  bool ChQCTable::Fill(QCDB::MonKey_t type, Int_t run, Int_t subrun, Short_t ch, Double_t mean, Double_t sigma, std::string ref) const
  {
    if(!GetConnection()) {
      
      std::cerr << "Connection not found..." << std::endl;
      
      return false;
    }
    
    std::string query = Form("INSERT INTO %s (%s,%s,%s,%s,%s,%s,%s)",
			     GetTableName().c_str(),
			     QCDB::kQCFieldName[QCDB::kMonKey].c_str(),
			     QCDB::kQCFieldName[QCDB::kRun].c_str(),
			     QCDB::kQCFieldName[QCDB::kSubRun].c_str(),
			     QCDB::kQCFieldName[QCDB::kChannel].c_str(),
			     QCDB::kQCFieldName[QCDB::kMean].c_str(),
			     QCDB::kQCFieldName[QCDB::kSigma].c_str(),
			     QCDB::kQCFieldName[QCDB::kReference].c_str());
    query += Form(" Values (%d, %d, %d, %d, %g, %g, '%s')",
		  type,
		  run,
		  subrun,
		  ch,
		  mean,
		  sigma,
		  ref.c_str());
    
    return GetConnection()->Exec(query);
    
  }
  
  bool ChQCTable::Fetch() 
  {
    if(_res_key==DB::INVALID_KEY) {
      
      std::cerr << "Fetch(): call this function upon successful Load() call!" << std::endl;
      
      return false;
    }
    
    if(!(GetConnection()->FetchRow(_res_key))) return false;
    
    _run    = GetConnection()->GetRow(_res_key)->GetInt(1);
    _subrun = GetConnection()->GetRow(_res_key)->GetInt(2);
    _ch     = GetConnection()->GetRow(_res_key)->GetInt(3);
    _mean   = GetConnection()->GetRow(_res_key)->GetDouble(4);
    _sigma  = GetConnection()->GetRow(_res_key)->GetDouble(5);
    _ref    = GetConnection()->GetRow(_res_key)->GetString(6);
    _tlog   = GetConnection()->GetRow(_res_key)->GetTimestamp(7);
    
    return true;
  }
  
  void ChQCTable::Initialize() {
    
    RunQCTable::Initialize();
    
  }
}
#endif
