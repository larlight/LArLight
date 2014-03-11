#ifndef DBCONN_CC
#define DBCONN_CC

#include "DBConn.hh"

namespace ubpsql{

  std::vector<DBConn*> DBConn::_me_v(kDBI_USER_MAX,NULL);

  DBConn::DBConn(DBI_USER_t user_type)
  { 
    if(user_type == kDBI_USER_MAX) throw InvalidUser();

    _user_type = user_type;
    _conn            = 0;
    _retrial_period  = 5;
    _retrial_number  = 5;
    _query_timeout   = 20;
    _user_shell = getenv("USER");

    Configure( DBI_CONFIG[_user_type] );
  }

  bool DBConn::Connect()
  {
    if(_conn) {

      if(!IsConnected()) PQreset(_conn);
      else return true;
    }
    
    Print(MSG::kNORMAL,"Connecting to PostgreSQL server using:");
    Print(MSG::kNORMAL,_conn_config.c_str());
    unsigned char ntrials=0;
    while(true) {

      if(ntrials) {
	
	if(ntrials > _retrial_number) {
	  
	  Print(MSG::kERROR,"Connection failed!");
	  return false;

	}

	Print(MSG::kWARNING,"Connection attempt failed. Attempting to re-connect...");

      }

      switch(PQping( _conn_config.c_str())) {
      case PQPING_OK:
	Print(MSG::kINFO,__FUNCTION__,"Server available. Attempting to connect...");
	break;
      case PQPING_REJECT:
	Print(MSG::kERROR,__FUNCTION__,"Server busy! Aborting...");
	return false;
      case PQPING_NO_RESPONSE:
	Print(MSG::kERROR,__FUNCTION__,"Server unavailable! Aborting...");
	return false;
      case PQPING_NO_ATTEMPT:
	std::ostringstream msg;
	msg << "Invalid config string: \"" << _conn_config.c_str() << "\"";
	Print(MSG::kERROR,__FUNCTION__,msg.str());
	return false;
      };

      _conn = PQconnectdb(_conn_config.c_str());

      usleep(1000);

      if(IsConnected()) return true;

      // Wait for _retrial_period seconds
      unsigned int sleep_time = 0;
      while(sleep_time < _retrial_period) {

	usleep(1e6);
	sleep_time++;

	std::cout<<"  Waiting for connection to be established  "
		 << (_retrial_period - sleep_time)
		 << " ... \r"
		 << std::flush;
	
      }
      if(IsConnected(true)) return true;
      std::cout<<std::endl;
      Close();
      ntrials+=1;
    }
    return IsConnected();
  }

  bool DBConn::IsConnected(bool verbose)
  {
    if(!_conn) return false;

    bool connected = false;
    switch(PQstatus(_conn)) {
    case CONNECTION_OK:
      if(verbose) Print(MSG::kNORMAL,__FUNCTION__,"Connection exists..."); 
      connected=true;
      break;
    case CONNECTION_NEEDED:
      if(verbose) Print(MSG::kNORMAL,__FUNCTION__,"Connection request not made yet..."); break;
    case CONNECTION_BAD:
      if(verbose) Print(MSG::kERROR,__FUNCTION__,"Failed establishing a connection..."); break;
    case CONNECTION_STARTED:
      if(verbose) Print(MSG::kWARNING,__FUNCTION__,"Connection request sent. Waiting for reply..."); break;
    case CONNECTION_MADE:
      if(verbose) Print(MSG::kWARNING,__FUNCTION__,"Connection found. Requesting authentification..."); break;
    case CONNECTION_AWAITING_RESPONSE:
      if(verbose) Print(MSG::kWARNING,__FUNCTION__,"Connection found. Waiting for authentification response..."); break;
    case CONNECTION_AUTH_OK:
      if(verbose) Print(MSG::kWARNING,__FUNCTION__,"Connection found. Authentification done. Waiting to start up..."); break;
    case CONNECTION_SSL_STARTUP:
      if(verbose) Print(MSG::kWARNING,__FUNCTION__,"Connection found. Starting up..."); break;
    case CONNECTION_SETENV:
      if(verbose) Print(MSG::kWARNING,__FUNCTION__,"Connection found. Back-end environment start up running..."); break;
    }
    return connected;
  }

  bool DBConn::Close()
  {
    if(_conn) {
      PQfinish(_conn);
      _conn = 0;
    }else{
      Print(MSG::kWARNING,__FUNCTION__,"Connection already closed...");
    }    
    return true;
  }

  PGresult* DBConn::Execute(const std::string &cmd)
  {
    if(!IsConnected() && !Connect()) return 0;
    
    Print(MSG::kINFO,__FUNCTION__,cmd);
    PGresult* res = PQexec(_conn,cmd.c_str());
    
    const unsigned int unit_sleep_time_us=500;
    unsigned int sleep_time_us=0;
    bool done=false;
    bool remove_res=true;

    while(sleep_time_us/1e6 < _query_timeout && !done) {
      usleep(unit_sleep_time_us);
      sleep_time_us+=unit_sleep_time_us;

      switch(PQresultStatus(res)) {
      case PGRES_NONFATAL_ERROR:
	Print(MSG::kWARNING,__FUNCTION__,PQresultErrorMessage(res));
      case PGRES_SINGLE_TUPLE:
      case PGRES_TUPLES_OK:
	done=true;
	remove_res=false;
	break;
      case PGRES_EMPTY_QUERY:
      case PGRES_BAD_RESPONSE:
      case PGRES_FATAL_ERROR:
	Print(MSG::kERROR,__FUNCTION__,PQresultErrorMessage(res));
      case PGRES_COMMAND_OK:
	done = remove_res = true;
	break;
      case PGRES_COPY_OUT:
      case PGRES_COPY_IN:
      case PGRES_COPY_BOTH:
	if(!(sleep_time_us%1000000))
	  Print(MSG::kNORMAL,__FUNCTION__,"Waiting to finish assessing the query w/ server...");
	break;
      }
    }

    if(remove_res) {
      PQclear(res);
      res=0;
    }

    return res;
  }

}

#endif
