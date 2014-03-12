



#ifndef VERTEX_CC
#define VERTEX_CC

#include "vertex.hh"

namespace larlight {

  //************************************************************
  vertex::vertex(DATA::DATA_TYPE type) : data_base(type)
  //************************************************************
  {
    if(_type!=DATA::Vertex &&
       _type!=DATA::FeatureVertex &&
       _type!=DATA::HarrisVertex) {
      
      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Provided data type (%s) not supported! Reset to default.",DATA::DATA_TREE_NAME[_type].c_str()));
      _type=DATA::Vertex;
    }
    
    clear_data();
    
  }

  //************************************************************
  vertex::vertex(Double_t* xyz,
		 Int_t      id,
		 DATA::DATA_TYPE type) 
    : data_base(type),
      fID(id)
  //************************************************************
  {
    if(_type!=DATA::Vertex &&
       _type!=DATA::FeatureVertex &&
       _type!=DATA::HarrisVertex) {
      
      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Provided data type (%s) not supported! Reset to default.",DATA::DATA_TREE_NAME[_type].c_str()));
      _type=DATA::Vertex;
    }

    fXYZ[0] = xyz[0];
    fXYZ[1] = xyz[1];
    fXYZ[2] = xyz[2];
  }

  //************************************************************
  void vertex::XYZ(Double_t *xyz) const
  //************************************************************
  {
    xyz[0] = fXYZ[0];
    xyz[1] = fXYZ[1];
    xyz[2] = fXYZ[2];
  }

  //************************************************************
  void vertex::clear_data()
  //************************************************************
  {
    data_base::clear_data();
    fXYZ[0] = DATA::INVALID_DOUBLE;
    fXYZ[1] = DATA::INVALID_DOUBLE;
    fXYZ[2] = DATA::INVALID_DOUBLE;
    fID = DATA::INVALID_INT;
  }


  //**********************************************************************************
  event_vertex::event_vertex(DATA::DATA_TYPE type) : std::vector<larlight::vertex>(),
						     event_base(type)
  //**********************************************************************************
  {
    if(_type!=DATA::Vertex &&
       _type!=DATA::FeatureVertex &&
       _type!=DATA::HarrisVertex) {

      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Provided data type (%s) not supported! Reset to default.",DATA::DATA_TREE_NAME[_type].c_str()));
      _type=DATA::Vertex;
    }

    clear_data();
  }

}

#endif
