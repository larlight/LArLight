#ifndef TABLEBASE_CC
#define TABLEBASE_CC

#include "TableBase.hh"

TableBase::TableBase()
{
  _tablename="TableBase";
  _table_def="";
  Initialize();
}

std::string TableBase::GetTableName(bool with_db) const
{
  if(_conn_key==DB::INVALID_KEY) return "";
  if(with_db) return Form("%s.%s",_db.c_str(),_tablename.c_str());
  else return _tablename.c_str();
}

bool TableBase::CreateTable() const
{

  if(_conn_key==DB::INVALID_KEY) return false;

  std::string query = Form("CREATE TABLE %s (%s) IF NOT EXISTS",GetTableName().c_str(),_table_def.c_str());

  return GetConnection()->Exec(query.c_str());

}

bool TableBase::DropTable() const 
{

  if(_conn_key==DB::INVALID_KEY) return false;

  return GetConnection()->Exec(Form("DROP TABLE %s IF EXISTS",GetTableName().c_str()));

}

bool TableBase::ExistTable(std::string name) const
{

  if(name.empty())

    name = GetTableName();

  if(!GetConnection()) {

    std::cerr << "Connection not established yet ... exiting..." << std::endl;
    
    return false;
  }
  
  size_t key = GetConnection()->Query(Form("SELECT table_name FROM information_schema.tables WHERE table_name = '%s'",name.c_str()));
  
  bool exist = ( key==DB::INVALID_KEY ? false : GetConnection()->GetRowCount(key));
  
  if(key!=DB::INVALID_KEY) GetConnection()->ClearResult(key);
  
  return exist;
  
}

void TableBase::QueryAndDump(std::string cmd) const
{

  if(GetConnection()) {

    size_t key = GetConnection()->Query(cmd);
    
    if(key!=DB::INVALID_KEY) {
      
      for(int row_index=0; row_index < GetConnection()->GetRowCount(key); ++row_index) {
	
	GetConnection()->FetchRow(key);
	
	for(int column_index=0; column_index < GetConnection()->GetFieldCount(key); ++column_index) 
	  
	  std::cout << GetConnection()->GetRow(key)->GetString(column_index) << "   ";
	
	std::cout << std::endl;
	
      }
      GetConnection()->ClearResult(key);
    }  
  } else

    std::cerr << "No connection established yet..." << std::endl;
}

/// Open (if not) & retrieve DBI connection instance
bool TableBase::Connect(DB::DB_t     type,
			std::string  host,
			unsigned int port,
			std::string  db,
			std::string  user,
			std::string  passwd)
{
  
  size_t key = ProcessDBIManager::GetME()->Connect(type,host,port,db,user,passwd);
  
  if(key==DB::INVALID_KEY) return false;
  
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
  _conn_key = DB::INVALID_KEY;
}

#endif

