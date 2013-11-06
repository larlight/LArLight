#ifndef PROCESSDBI_CC
#define PROCESSDBI_CC

#include "ProcessDBI.hh"

ProcessDBI::ProcessDBI(TYPE_SQL_SERVER type) : _res_v() 
{
  _type   = type; 
  _conn   = 0; 
  _ntrial = 1;
  _download_res = true;
};

bool ProcessDBI::Connect(std::string host,
			 unsigned int port,
			 std::string db,
			 std::string user,
			 std::string passwd)
{

  if(_conn) Disconnect();

  std::string server="";
  switch(_type){
  case MYSQL:
    server="mysql://";
    break;
  case POSTGRESQL:
    server="pgsql://";
    break;
  case ORACLE:
    server="oracle://";
    break;
  }

  if(port)
    server = Form("%s%s:%d",server.c_str(),host.c_str(),port);
  else
    server = Form("%s%s",server.c_str(),host.c_str());

  std::cout
    << Form("SERVER: %s ... DB: %s ... USER: %s",server.c_str(),db.c_str(),user.c_str())
    << std::endl;

  server = Form("%s/%s",server.c_str(),db.c_str());

  _server = server;
  _user   = user;
  _passwd = passwd;

  _conn = TSQLServer::Connect(_server.c_str(),_user.c_str(),_passwd.c_str());

  if(IsConnected()) return true;

  else return Reconnect();
  
}

bool ProcessDBI::Reconnect()
{
  if(_conn) Disconnect();

  for(size_t trial=0; trial<_ntrial && !IsConnected(); trial++) {

    gSystem->Sleep(30000);
    
    _conn = TSQLServer::Connect(_server.c_str(),_user.c_str(),_passwd.c_str());

  }

  return IsConnected();
  
}

bool ProcessDBI::IsConnected() const
{
  if(_conn) 

    return _conn->IsConnected();

  else return false;
}

void ProcessDBI::ClearResult(size_t index)
{

  if(IsValidResult(index)) {

    delete _res_v[index];

    _res_v[index]=0;

  }
}

void ProcessDBI::Disconnect()
{
  if(_conn) {

    for(size_t index=0; index<_res_v.size(); index++)

      ClearResult(index);

    _res_v.clear();

    _conn->Close();

  }

  delete _conn;
  _conn = 0;
}

bool ProcessDBI::Exec(std::string cmd)
{

  if(!_conn) return false;

  else if (!IsConnected())
    
    if(!Reconnect()) return false;

  _conn->Exec(cmd.c_str());

  if(_conn->IsError()){

    std::cerr
      << std::endl
      << Form("Query      : %s",cmd.c_str())           << std::endl
      << Form("Error Code : %d",_conn->GetErrorCode()) << std::endl
      << Form("Error MSG  : %s",_conn->GetErrorMsg())  << std::endl
      << std::endl;
    return false;

  }

  return true;
}

size_t ProcessDBI::Query(std::string cmd)
{
  
  if(!_conn) return INVALID_KEY;

  else if (!IsConnected()) return INVALID_KEY;

  TSQLStatement* res = (TSQLStatement*)(_conn->Statement(cmd.c_str()));

  if(!res) return INVALID_KEY;

  res->Process();

  if(res->IsError()) {

    std::cerr
      << std::endl
      << Form("Query      : %s",cmd.c_str())           << std::endl
      << Form("Error Code : %d",res->GetErrorCode()) << std::endl
      << Form("Error MSG  : %s",res->GetErrorMsg())  << std::endl
      << std::endl;
    
    return INVALID_KEY;
  }

  if(_download_res) res->StoreResult();

  _res_v.push_back(res);
  
  return (size_t)(_res_v.size()-1);
}

TSQLStatement* ProcessDBI::FetchRow(size_t index)
{

  if(!IsValidResult(index)) return 0;

  if(_res_v[index]->NextResultRow())

    return _res_v[index];

  ClearResult(index);

  return 0;

}

TSQLStatement* ProcessDBI::GetRow(size_t index) const
{
  
  if(!IsValidResult(index)) return 0;

  return _res_v.at(index);

}

int ProcessDBI::GetRowCount(size_t index) const
{

  if(!IsValidResult(index)) return -1;

  return _res_v[index]->GetNumAffectedRows();

}

int ProcessDBI::GetFieldCount(size_t index) const
{
  
  if(!IsValidResult(index)) return -1;

  return _res_v[index]->GetNumFields();

}

#endif
