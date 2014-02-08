#ifndef WIRE_CC
#define WIRE_CC

#include "wire.hh"

namespace larlight {
  
  //***************************************************
  wire::wire(DATA::DATA_TYPE type)  : data_base(type), 
				      fSignal() 
  //***************************************************
  {
    if(_type!=DATA::Wire) {
    
      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Provided data type (%s) not supported! Reset to default.",DATA::DATA_TREE_NAME[_type].c_str()));
      
      _type=DATA::Wire;
    }

    clear_data();
  }

  //*********************
  void wire::clear_data()
  //*********************
  {
    fSignal.clear();
    fView       = GEO::kUnknown;
    fSignalType = GEO::kMysteryType;
    fChannel    = 0xffffffff;
  }

  //***************************************************************************
  event_wire::event_wire(DATA::DATA_TYPE type) : std::vector<larlight::wire>(), 
						 data_base(type)
  //***************************************************************************
  {
    if(_type!=DATA::Wire) {
    
      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Provided data type (%s) not supported! Reset to default.",DATA::DATA_TREE_NAME[_type].c_str()));
      
      _type=DATA::Wire;
    }
    
    clear_data();
  }

}
#endif
