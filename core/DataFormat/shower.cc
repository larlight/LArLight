#ifndef SHOWER_CC
#define SHOWER_CC

#include "shower.hh"

namespace larlight {

  //****************************************************
  shower::shower(DATA::DATA_TYPE type) : data_base(type) 
  //****************************************************
  {
    if( _type!=DATA::Shower &&
        _type!=DATA::RyanShower
	) {

      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Provided data type (%s) not supported! Reset to default.",DATA::DATA_TREE_NAME[_type].c_str()));

      _type = DATA::Shower;
    }    
    clear_data();
  }
  
  //*******************************************
  void shower::clear_data()
  //******************************************* 
  {
    data_base::clear_data();

    fID = -1;
    fDCosStart[0] = fDCosStart[1] = fDCosStart[2] = -1;
    fSigmaDCosStart[0] = fSigmaDCosStart[1] = fSigmaDCosStart[2] = -1;
    fTotalEnergy.clear();           ///< Calculated Energy per each plane
    fSigmaTotalEnergy.clear();           ///< Calculated Energy per each plane
    fdEdx.clear();           ///< Calculated dEdx per each plane
    fSigmadEdx.clear();           ///< Calculated dEdx per each plane
 //   fMaxWidthX = fMaxWidthY = -1;
 //   fDistanceMaxWidth=-1;
 //   fTotalCharge=-1;
  }

  //*********************************************************************************
  event_shower::event_shower(DATA::DATA_TYPE type) : std::vector<larlight::shower>(), 
						     event_base(type) 
  //*********************************************************************************
  {
    if( _type!=DATA::Shower &&
	_type!=DATA::RyanShower
	) {
      
      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Provided data type (%s) not supported! Reset to default.",DATA::DATA_TREE_NAME[_type].c_str()));
      
      _type = DATA::Shower;
    }    
    clear_data();
  }

}

#endif
