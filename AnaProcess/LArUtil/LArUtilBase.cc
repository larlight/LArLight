#ifndef LARUTILBASE_CC
#define LARUTILBASE_CC

#include "LArUtilBase.hh"

namespace larutil {

  //-----------------------------------------------------
  LArUtilBase::LArUtilBase(std::string file_name,
			   std::string tree_name)
    : larlight::larlight_base(),
      _file_name(file_name),
      _tree_name(tree_name)
  //-----------------------------------------------------
  { 
    _data_tree = 0;
    _name      = "LArUtilBase";
  }

  //-----------------------------------------------------
  bool LArUtilBase::LoadData(bool force_reload)
  //-----------------------------------------------------
  {
    if(!force_reload && _data_tree) return true;

    print(larlight::MSG::INFO,__FUNCTION__,
	  Form("Reading-in data for %s",_name.c_str()));

    if(_file_name.empty() || _tree_name.empty()) {

      throw LArUtilException(Form("Either file or tree name not specified to load data for %s",_name.c_str()));
      
      return false;
    }

    if(_data_tree){

      print(larlight::MSG::NORMAL,__FUNCTION__,"Data already loaded before. Removing it...");	   
      delete _data_tree;

    }

    _data_tree = new TChain(_tree_name.c_str());
    _data_tree->AddFile(_file_name.c_str());

    try {

      SetBranchAddress();
      _data_tree->GetEntry(0);
      print(larlight::MSG::NORMAL, __FUNCTION__,
	    Form("Loading data for %s...\n     file=%s ", _name.c_str(), _file_name.c_str()));
      
    }
    catch (LArUtilException &e) {

      print(larlight::MSG::ERROR, __FUNCTION__,
	    Form("Failed loading data for %s...",_name.c_str()));

      print(larlight::MSG::ERROR, __FUNCTION__,
	    e.msg());
      
      delete _data_tree;
      _data_tree = 0;
      throw e;
      return false;
    }

    return true;
  }
  
}
#endif
