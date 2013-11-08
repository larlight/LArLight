#ifndef CHQCTABLE_CC
#define CHQCTABLE_CC

#include "ChQCTable.hh"

ChQCTable::ChQCTable() : RunQCTable() 
{ 
  _tablename="ChQCTable"; 
  _tablename="RunQCTable"; 
  _table_def  = Form("%s SMALLINT NOT NULL,",QC::kQCFieldName[QC::kMonKey].c_str());
  _table_def += Form("%s INT NOT NULL,",QC::kQCFieldName[QC::kRun].c_str());
  _table_def += Form("%s INT NOT NULL,",QC::kQCFieldName[QC::kSubRun].c_str());
  _table_def += Form("%s SMALLINT NOT NULL,",QC::kQCFieldName[QC::kChannel].c_str());
  _table_def += Form("%s DOUBLE PRECISION NOT NULL,",QC::kQCFieldName[QC::kMean].c_str());
  _table_def += Form("%s DOUBLE PRECISION NOT NULL,",QC::kQCFieldName[QC::kSigma].c_str());
  _table_def += Form("%s VARCHAR(100) NOT NULL,",QC::kQCFieldName[QC::kReference].c_str());
  _table_def += Form("%s TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,",QC::kQCFieldName[QC::kLogTime].c_str());
  _table_def += Form("KEY %s, KEY %s, KEY %s, KEY %s, PRIMARY KEY (%s, %s, %s, %s)",
		     QC::kQCFieldName[QC::kMonKey].c_str(),
		     QC::kQCFieldName[QC::kRun].c_str(),
		     QC::kQCFieldName[QC::kSubRun].c_str(),
		     QC::kQCFieldName[QC::kChannel].c_str(),
		     QC::kQCFieldName[QC::kMonKey].c_str(),
		     QC::kQCFieldName[QC::kRun].c_str(),
		     QC::kQCFieldName[QC::kSubRun].c_str(),
		     QC::kQCFieldName[QC::kChannel].c_str());
  Initialize(); 
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
  
  return true;
}

void ChQCTable::Initialize() {
  
  RunQCTable::Initialize();
  
}

#endif
