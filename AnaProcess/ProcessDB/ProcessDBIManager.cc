#ifndef PROCESSDBIMANAGER_CC
#define PROCESSDBIMANAGER_CC
#include <ProcessDBIManager.hh>

ProcessDBIManager* ProcessDBIManager::_me = 0;

ProcessDBI* ProcessDBIManager::GetConnection(size_t key) const
{
  return ( key < _conn_v.size() ? _conn_v.at(key) : 0 );
}

/// Open (if not) & retrieve DBI connection instance
size_t ProcessDBIManager::Connect(DB::DB_t     type,
				  std::string  host,
				  unsigned int port,
				  std::string  db,
				  std::string  user,
				  std::string  passwd)
{
  size_t key = DB::INVALID_KEY;
  std::string server(Form("%s:%d/%s",host.c_str(),port,db.c_str()));
  if(_conn_key.find(server)!=_conn_key.end())
    
    if(_conn_key[server].find(user)!=_conn_key[server].end())
      
      key = _conn_key[server][user];
  
  if (GetConnection(key)) return key;
  
  ProcessDBI* conn = new ProcessDBI;
  if(conn->Connect(type, host, port, db, user, passwd)) {
    
    _conn_v.push_back(conn);

    key = _conn_v.size() - 1;
    
    if(_conn_key.find(server)==_conn_key.end())
      
      _conn_key[server]=std::map<std::string,size_t>();

    _conn_key[server][user] = key;
    
  }else{
    delete conn;
    conn = 0;
  }
  
  return key;
}


/// Close (if not) the specified connection 
void ProcessDBIManager::Disconnect(size_t key)
{
  ProcessDBI* conn = (ProcessDBI*)(GetConnection(key));
  if(conn) {
    conn->Disconnect();
    delete conn;
  }
  _conn_v[key]=0;
}

/// Close all connections
void ProcessDBIManager::DisconnectAll()
{
  for(size_t index=0; index<_conn_v.size(); ++index)
    
    Disconnect(index);
}

#endif
