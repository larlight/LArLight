#ifndef TABLEBASE_CC
#define TABLEBASE_CC

#include "TableBase.hh"

std::string TableBase::GetTableName(bool with_db) const
{
  if(_conn_key==ProcessDBI::INVALID_KEY) return "";
  if(with_db) return Form("%s.%s",_db.c_str(),_tablename.c_str());
  else return _tablename.c_str();
}

bool TableBase::ExistTable() const
{return true;}

/// Open (if not) & retrieve DBI connection instance
bool TableBase::Connect(std::string host,
			unsigned int port,
			std::string db,
			std::string user,
			std::string passwd)
{
  
  size_t key = ProcessDBIManager::GetME()->OpenConnection(host,port,db,user,passwd);
  
  if(key==ProcessDBI::INVALID_KEY) return false;
  
  _db       = db;
  _conn_key = key;
  
  return true;
  
}

bool TableBase::SetConnKey(size_t key) 
{ 

  if(!(ProcessDBIManager::GetME()->GetConnection(key))) return false;

  _conn_key = key;

  return true;
}

/// A function to reset all
void TableBase::Initialize()
{
  _db = "";
  _tablename = "";
  _conn_key = ProcessDBI::INVALID_KEY;
}

#endif

