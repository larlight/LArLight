#ifndef CALORIMETRY_CC
#define CALORIMETRY_CC

#include "calorimetry.hh"

namespace larlight {

  //**************************************************************
  calorimetry::calorimetry(DATA::DATA_TYPE type) : data_base(type)
  //**************************************************************
  {
    if(_type!=DATA::Calorimetry) {

      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Provided data type (%s) not supported! Reset to default.",DATA::DATA_TREE_NAME[_type].c_str()));

      _type=DATA::Calorimetry;
    }

    clear_data();
  }

  //*************************************
  void calorimetry::clear_data() 
  //*************************************
  {
    data_base::clear_data();
    fdEdx.clear();
    fdQdx.clear();
    fResidualRange.clear();
    fDeadWireResR.clear();
    fTrkPitch.clear();
    fRange = -1;
  }

  //************************************************************************************************
  event_calorimetry::event_calorimetry(DATA::DATA_TYPE type) : std::vector<larlight::calorimetry>(),
							       data_base(type)
  //************************************************************************************************
  {
    if(_type!=DATA::Calorimetry) {

      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Provided data type (%s) not supported! Reset to default.",DATA::DATA_TREE_NAME[_type].c_str()));

      _type=DATA::Calorimetry;
    }

    clear_data();
  }

}

#endif
