#ifndef DATA_BASE_CC
#define DATA_BASE_CC

#include "data_base.hh"

namespace larlight {

  //*******************************************************************************************
  void data_base::add_association(DATA::DATA_TYPE type, const std::vector<unsigned int> ass)
  //*******************************************************************************************
  {

    if( _ass.find(type) == _ass.end() )
      
      _ass[type]=std::vector<std::vector<unsigned int> >();
    
    _ass[type].push_back(ass);
    
    
  }
  
  //*************************************************************
  size_t data_base::size_association(DATA::DATA_TYPE type) const
  //*************************************************************
  {
    
    if( _ass.find(type) == _ass.end() ) return 0;
    
    return ((_ass.find(type))->second).size();
    //return _ass[type].size();
    
  }
  
  //*************************************************************************************************
  const std::vector<unsigned int> data_base::association(DATA::DATA_TYPE type, size_t index) const
  //*************************************************************************************************
  {
    
    size_t ass_length = size_association(type);
    
    if( !(ass_length) ) {
      
      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("There is no association to %s",DATA::DATA_TREE_NAME[type].c_str()));
      
      return std::vector<unsigned int>();
      
    }
    
    if( ass_length <= index ) {
      
      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("There are only %zu associations. No association @ index=%zu",ass_length,index));
      
      return std::vector<unsigned int>();
      
    }
    
    return ((_ass.find(type))->second).at(index);
  }

  //**************************
  void event_base::clear_data()
  //**************************
  {
    data_base::clear_data();
    fRunNumber = fSubRunNumber = fEventID = DATA::INVALID_UINT;
  }

}

#endif
