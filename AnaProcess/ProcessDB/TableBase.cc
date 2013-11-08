#ifndef TABLEBASE_CC
#define TABLEBASE_CC

#include "TableBase.hh"

TableBase::TableBase(std::string name)
{
  _tablename=name;
  Initialize();
}

std::string TableBase::GetTableName(bool with_db) const
{
  if(_conn_key==ProcessDBI::INVALID_KEY) return "";
  if(with_db) return Form("%s.%s",_db.c_str(),_tablename.c_str());
  else return _tablename.c_str();
}

bool TableBase::ExistTable(std::string name) const
{

  if(name.empty())

    name = GetTableName();
  
  size_t key = Query(Form("SELECT table_name FROM information_schema.tables WHERE table_name = '%s'",name.c_str()));

  bool exist = ( key==ProcessDBI::INVALID_KEY ? false : GetConnection()->GetRowCount(key));

  if(key!=ProcessDBI::INVALID_KEY) GetConnection()->ClearResult(key);

  return exist;

}

ProcessDBI* TableBase::GetConnection() const
{

  if(_conn_key==ProcessDBI::INVALID_KEY) {
    
    std::cerr << "Connection not yet established!" << std::endl;
    
    return 0;
  }

  return ProcessDBIManager::GetME()->GetConnection(_conn_key);

}

size_t TableBase::Query(std::string cmd) const
{

  size_t key = GetConnection()->Query(cmd);

  if(key==ProcessDBI::INVALID_KEY) {
    
    std::cerr << "Query unsuccessful..." << std::endl;
    
    return ProcessDBI::INVALID_KEY;
  }
  else return key;
}

void TableBase::QueryAndDump(std::string cmd) const
{

  size_t key = Query(cmd);

  if(key!=ProcessDBI::INVALID_KEY) {

    for(int row_index=0; row_index < GetConnection()->GetRowCount(key); ++row_index) {

      GetConnection()->FetchRow(key);

      for(int column_index=0; column_index < GetConnection()->GetFieldCount(key); ++column_index) 

	std::cout << GetConnection()->GetRow(key)->GetString(column_index) << "   ";

      std::cout << std::endl;

    }
    GetConnection()->ClearResult(key);
  }  

}

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
  _conn_key = ProcessDBI::INVALID_KEY;
}

#endif

