#ifndef QCTABLEBASE_CC
#define QCTABLEBASE_CC

#include "QCTableBase.hh"

void QCTableBase::Initialize()
{

  _res_key = DB::INVALID_KEY;

  ClearValues();
  
  InitializeCuts();

}

void QCTableBase::InitializeCuts()
{
  _cut_time.first   = TTimeStamp(2000,1,1,0,0,0);
  _cut_time.second  = TTimeStamp(2000,1,1,0,0,0);
  _cut_mean.first   = _cut_mean.second   = 0;
  _cut_sigma.first  = _cut_sigma.second  = 0;
  _cut_run.first    = _cut_run.second    = 0;
  _cut_ch.first     = _cut_ch.second     = 0;
  _cut_subrun.first = _cut_subrun.second = 0;
  _cut_ref = "";
}

bool QCTableBase::DropTable() const 
{

  if(_conn_key==DB::INVALID_KEY) return false;

  return GetConnection()->Exec(Form("DROP TABLE %s IF EXISTS",GetTableName().c_str()));

}

void QCTableBase::ClearValues()
{
  _mean = _sigma  = 0;
  _run  = _subrun = -1;
  _ch   = 0xffff;
  _ref  = "";
}

void QCTableBase::ClearResult()
{
  if(_res_key!=DB::INVALID_KEY) {
    GetConnection()->ClearResult(_res_key);
    _res_key = DB::INVALID_KEY;
  }
}

bool QCTableBase::Load(QC::MonKey_t type, size_t limit)
{

  if(_conn_key==DB::INVALID_KEY) return false;

  std::string condition = Form("%s=%d",QC::kQCFieldName[QC::kMonKey].c_str(),type);
  
  /// Apply time cut if set
  if(_cut_time.first.GetSec() != _cut_time.second.GetSec())

    condition = Form("%s AND ('%s'<%s AND %s <'%s')",
		     condition.c_str(),
		     _cut_time.first.AsString("s"),
		     QC::kQCFieldName[QC::kTimeBegin].c_str(),
		     QC::kQCFieldName[QC::kTimeEnd].c_str(),
		     _cut_time.second.AsString("s"));
  
  /// Apply mean value cut if set
  if(_cut_mean.first!=_cut_mean.second)

    condition = Form("%s AND %g < %s AND %s < %g", 
		     condition.c_str(),
		     _cut_mean.first,
		     QC::kQCFieldName[QC::kMean].c_str(),
		     QC::kQCFieldName[QC::kMean].c_str(),
		     _cut_mean.second);

  /// Apply sigma value cut if set
  if(_cut_sigma.first!=_cut_sigma.second)

    condition = Form("%s AND %g < %s AND %s < %g", 
		     condition.c_str(),
		     _cut_sigma.first,
		     QC::kQCFieldName[QC::kSigma].c_str(),
		     QC::kQCFieldName[QC::kSigma].c_str(),
		     _cut_sigma.second);

  /// Apply run number cut if set
  if(_cut_run.first!=_cut_run.second)

    condition = Form("%s AND %d < %s AND %s < %d", 
		     condition.c_str(),
		     _cut_run.first,
		     QC::kQCFieldName[QC::kRun].c_str(),
		     QC::kQCFieldName[QC::kRun].c_str(),
		     _cut_run.second);

  /// Apply subrun number cut if set
  if(_cut_subrun.first!=_cut_subrun.second)

    condition = Form("%s AND %d < %s AND %s < %d", 
		     condition.c_str(),
		     _cut_subrun.first,
		     QC::kQCFieldName[QC::kSubRun].c_str(),
		     QC::kQCFieldName[QC::kSubRun].c_str(),
		     _cut_subrun.second);

  /// Apply channel number cut if set
  if(_cut_ch.first!=_cut_ch.second)

    condition = Form("%s AND %d < %s AND %s < %d", 
		     condition.c_str(),
		     _cut_ch.first,
		     QC::kQCFieldName[QC::kChannel].c_str(),
		     QC::kQCFieldName[QC::kChannel].c_str(),
		     _cut_ch.second);

  std::string query = Form("SELECT * FROM %s WHERE %s ORDER BY %s",
			   GetTableName().c_str(),
			   condition.c_str(),
			   QC::kQCFieldName[QC::kLogTime].c_str());
  if(limit)
    query = Form("%s LIMIT %zu", query.c_str(), limit);

  _res_key = GetConnection()->Query(query);
  ClearValues();

  return (_res_key == DB::INVALID_KEY);

}

#endif
