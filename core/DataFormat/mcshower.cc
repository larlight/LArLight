#ifndef MCSHOWER_CC
#define MCSHOWER_CC

#include "mcshower.hh"

namespace larlight {

  //--------------------------------------------------------
  mcshower::mcshower(DATA::DATA_TYPE type) : data_base(type)
  //--------------------------------------------------------
  {
    if(_type != DATA::MCShower) {
      
      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Provided data type (%s) not supported! Reset to default.",DATA::DATA_TREE_NAME[_type].c_str()));
      
      _type=DATA::MCShower;
    }
    
    clear_data();
    
  }


  //-------------------------------------------------------------
  void mcshower::clear_data()
  //-------------------------------------------------------------
  {
    data_base::clear_data();
    fMotherPDGID   = DATA::INVALID_INT;
    fMotherTrackID = DATA::INVALID_UINT;
    fMotherProcess = "";
    fMotherVtx.clear();
    fMotherVtx.resize(4,0);
    fMotherMomentum.clear();
    fMotherMomentum.resize(4,0);
    fMotherPhi = fMotherTheta = DATA::INVALID_DOUBLE;
    fDaughterTrackID.clear();
    fDaughterMomentum.clear();
    fDaughterMomentum.resize(4,0);
    fDaughterVtx.clear();
    fDaughterVtx.resize(4,0);
    fDaughterTheta = fDaughterPhi = DATA::INVALID_DOUBLE;
    fPlaneCharge.clear();
  }

  void mcshower::SetMotherPoint(const std::vector<Double_t> &vtx)
  {
    if(vtx.size()!=4) {
      Message::get()->send(MSG::ERROR,__FUNCTION__,"Input must have 4 dimension (x,y,z,t)!");
      return;
    }
    fMotherVtx = vtx;
  }
  
  void mcshower::SetMotherMomentum(const std::vector<Double_t> &mom)
  {
    if(mom.size()!=4) {
      Message::get()->send(MSG::ERROR,__FUNCTION__,"Input must have 4 dimension (Px,Py,Pz,E)!");
      return;
    }
    fMotherMomentum = mom;
  }
  
  void mcshower::SetDaughterPosition(const std::vector<Double_t> &vtx)
  {
    if(vtx.size()!=4) {
      Message::get()->send(MSG::ERROR,__FUNCTION__,"Input must have 4 dimension (x,y,z,t)!");
      return;
    }
    fDaughterVtx = vtx;
  }

  void mcshower::SetDaughterMomentum(const std::vector<Double_t> &mom)
  {
    if(mom.size()!=4) {
      Message::get()->send(MSG::ERROR,__FUNCTION__,"Input must have 4 dimension (Px,Py,Pz,E)!");
      return;
    }
    fDaughterMomentum = mom;
  }

  //----------------------------------------------------
  Float_t mcshower::Charge(const size_t plane) const
  //----------------------------------------------------
  {
    if(plane > fPlaneCharge.size()) {

      Message::get()->send(MSG::ERROR,__FUNCTION__,Form("No charge stored for plane: %d",plane));
      return -1;

    }
      
    return fPlaneCharge[plane];
  }

  //--------------------------------------------------------
  event_mcshower::event_mcshower(DATA::DATA_TYPE type) 
    : std::vector<larlight::mcshower>(),
      event_base(type)
  //--------------------------------------------------------
  {
    if(_type != DATA::MCShower) {
      
      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Provided data type (%s) not supported! Reset to default.",DATA::DATA_TREE_NAME[_type].c_str()));
      
      _type=DATA::MCShower;
    }
    
    clear_data();
  }
}

#endif
