#ifndef TIMEQCTABLE_CC
#define TIMEQCTABLE_CC

#include "TimeQCTable.hh"

TimeQCTable::TimeQCTable() : QCTableBase() 
{ 
  _tablename="TimeQCTable"; 
  _table_def  = Form("%s SMALLINT NOT NULL,",QC::kQCFieldName[QC::kMonKey].c_str());
  _table_def += Form("%s TIMESTAMP NOT NULL,",QC::kQCFieldName[QC::kTimeBegin].c_str());
  _table_def += Form("%s TIMESTAMP NULL,",QC::kQCFieldName[QC::kTimeEnd].c_str());
  _table_def += Form("%s DOUBLE PRECISION NOT NULL,",QC::kQCFieldName[QC::kMean].c_str());
  _table_def += Form("%s DOUBLE PRECISION NOT NULL,",QC::kQCFieldName[QC::kSigma].c_str());
  _table_def += Form("%s VARCHAR(100) NOT NULL,",QC::kQCFieldName[QC::kReference].c_str());
  _table_def += Form("%s TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,",QC::kQCFieldName[QC::kLogTime].c_str());
  _table_def += Form("KEY %s, KEY %s, KEY %s, PRIMARY KEY (%s, %s, %s)",
		     QC::kQCFieldName[QC::kMonKey].c_str(),
		     QC::kQCFieldName[QC::kTimeBegin].c_str(),
		     QC::kQCFieldName[QC::kTimeEnd].c_str(),
		     QC::kQCFieldName[QC::kMonKey].c_str(),
		     QC::kQCFieldName[QC::kTimeBegin].c_str(),
		     QC::kQCFieldName[QC::kTimeEnd].c_str());
  Initialize(); 
}

bool TimeQCTable::Fetch() 
{
  if(_res_key==DB::INVALID_KEY) {
    
    std::cerr << "Fetch(): call this function upon successful Load() call!" << std::endl;
    
    return false;
  }
  
  if(!(GetConnection()->FetchRow(_res_key))) return false;
  
  TDatime ts = GetConnection()->GetRow(_res_key)->GetTimestamp(1);
  _tstart.Set(ts.GetYear(),ts.GetMonth(),ts.GetDay(),ts.GetHour(),ts.GetMinute(),ts.GetSecond(),0,true,0);
  ts = GetConnection()->GetRow(_res_key)->GetTimestamp(2);
  _tend.Set(ts.GetYear(),ts.GetMonth(),ts.GetDay(),ts.GetHour(),ts.GetMinute(),ts.GetSecond(),0,true,0);
  _mean   = GetConnection()->GetRow(_res_key)->GetDouble(3);
  _sigma  = GetConnection()->GetRow(_res_key)->GetDouble(4);
  _ref    = GetConnection()->GetRow(_res_key)->GetString(5);
  
  return true;
}

void TimeQCTable::Initialize() {
  
  QCTableBase::Initialize();
  
}

#endif
