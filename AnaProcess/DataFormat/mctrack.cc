#ifndef MCTRACK_CC
#define MCTRACK_CC

#include "mctrack.hh"

namespace larlight{

  //***********************************************************************
  mctrack::mctrack(DATA::DATA_TYPE type) : std::vector<larlight::mcstep>(), 
					   data_base(type)
  //***********************************************************************
  {
    if(_type!=DATA::MCTrajectory) {

      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Provided data type (%s) not supported! Reset to default.",DATA::DATA_TREE_NAME[_type].c_str()));

      _type=DATA::MCTrajectory;

    }
    clear_data();
  }


}

#endif
