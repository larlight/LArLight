#ifndef LARUTILBASE_CC
#define LARUTILBASE_CC

#include "LArUtilBase.hh"

namespace larutil {

  //-----------------------------------------------------
  LArUtilBase::LArUtilBase()
    : larlight::larlight_base(),
      _file_name(""),
      _tree_name("")
  //-----------------------------------------------------
  { 
    _name   = "LArUtilBase";
    _loaded = false;
  }

  //-----------------------------------------------------
  bool LArUtilBase::LoadData(bool force_reload)
  //-----------------------------------------------------
  {
    if(!force_reload && _loaded) return true;

    print(larlight::MSG::INFO,__FUNCTION__,
	  Form("Reading-in data for %s",_name.c_str()));

    if(_file_name.empty() || _tree_name.empty()) {

      throw LArUtilException(Form("Either file or tree name not specified to load data for %s",_name.c_str()));
      
      return false;
    }

    bool status=false;
    try {

      print(larlight::MSG::NORMAL, __FUNCTION__,
	    Form("Loading data for %s...\n     file=%s ", _name.c_str(), _file_name.c_str()));
      ClearData();
      status = ReadTree();
      if(!status)
	throw LArUtilException("Failed to load !");
    }
    catch (LArUtilException &e) {

      print(larlight::MSG::ERROR, __FUNCTION__,
	    Form("Failed loading data for %s...",_name.c_str()));

      print(larlight::MSG::ERROR, __FUNCTION__,
	    e.what());
      
      throw e;
      status = false;
    }

    return status;
  }
  
}
#endif
