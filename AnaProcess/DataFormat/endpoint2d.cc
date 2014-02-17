#ifndef ENDPOINT2D_CC
#define ENDPOINT2D_CC

#include "endpoint2d.hh"

namespace larlight{

  //************************************************************
  endpoint2d::endpoint2d(DATA::DATA_TYPE type) : data_base(type)
  //************************************************************
  {
    if(_type!=DATA::EndPoint2D &&
       _type!=DATA::FeatureEndPoint2D &&
       _type!=DATA::HarrisEndPoint2D) {
      
      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Provided data type (%s) not supported! Reset to default.",DATA::DATA_TREE_NAME[_type].c_str()));
      _type=DATA::Vertex;
    }
    
    clear_data();
  }
  
  //************************************************************
  endpoint2d::endpoint2d(Double_t    driftTime,
			 UInt_t      wire,
			 Double_t    strength,
			 Int_t       id,
			 GEO::View_t view,
			 Double_t    totalQ,
			 DATA::DATA_TYPE type)
    : data_base(type), 
      fDriftTime(driftTime),
      fWire(wire),
      fID(id),
      fStrength(strength),
      fView(view),
      fTotalCharge(totalQ)
  //************************************************************
  {
    if(_type!=DATA::EndPoint2D &&
       _type!=DATA::FeatureEndPoint2D &&
       _type!=DATA::HarrisEndPoint2D) {
      
      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Provided data type (%s) not supported! Reset to default.",DATA::DATA_TREE_NAME[_type].c_str()));
      _type=DATA::EndPoint2D;
    }
  }


  //************************************************************
  void endpoint2d::clear_data()
  //************************************************************
  {
    data_base::clear_data();
    fDriftTime=DATA::INVALID_DOUBLE;
    fWire=DATA::INVALID_UINT;
    fStrength=DATA::INVALID_DOUBLE;
    fID=DATA::INVALID_INT;
    fView=GEO::kUnknown;
    fTotalCharge=DATA::INVALID_DOUBLE;
  }

  //************************************************************************
  event_endpoint2d::event_endpoint2d(DATA::DATA_TYPE type) : event_base(type)
  //************************************************************************
  {
    if(_type!=DATA::EndPoint2D &&
       _type!=DATA::FeatureEndPoint2D &&
       _type!=DATA::HarrisEndPoint2D) {
      
      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Provided data type (%s) not supported! Reset to default.",DATA::DATA_TREE_NAME[_type].c_str()));
      _type=DATA::Vertex;
    }
    
    clear_data();
  }

}

#endif
