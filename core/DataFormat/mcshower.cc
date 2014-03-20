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
    fMotherVtx.clear();
    fMotherVtx.resize(4,0);
    fMotherMomentum.clear();
    fMotherMomentum.resize(4,0);
    fMotherPhi = fMotherTheta = DATA::INVALID_DOUBLE;
    fMotherAngleU = fMotherAngleV = fMotherAngleW = DATA::INVALID_FLOAT;
    fDaughterTrackID.clear();
    fDaughterMomentum.clear();
    fDaughterMomentum.resize(4,0);
    fDaughterVtx.clear();
    fDaughterVtx.resize(4,0);
    fDaughterTheta = fDaughterPhi = DATA::INVALID_DOUBLE;
    fDaughterAngleU = fDaughterAngleV = fDaughterAngleW = DATA::INVALID_FLOAT;
    fChargeU = fChargeV = fChargeW = 0;
    fEdepVtx.clear();
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
  
  void mcshower::SetDaughterPosition(const std::vector<Float_t> &vtx)
  {
    if(vtx.size()!=4) {
      Message::get()->send(MSG::ERROR,__FUNCTION__,"Input must have 4 dimension (x,y,z,t)!");
      return;
    }
    fDaughterVtx = vtx;
  }

  void mcshower::SetDaughterMomentum(const std::vector<Float_t> &mom)
  {
    if(mom.size()!=4) {
      Message::get()->send(MSG::ERROR,__FUNCTION__,"Input must have 4 dimension (Px,Py,Pz,E)!");
      return;
    }
    fDaughterMomentum = mom;
  }

  void mcshower::AddEdepVtx(const std::vector<Float_t>& vtx)
  {
    if(vtx.size()!=4) {
      Message::get()->send(MSG::ERROR,__FUNCTION__,"Input must have 4 dimension (x,y,z,E)!");
      return;
    }    
    fEdepVtx.push_back(vtx);
  }

  void mcshower::SetEdepVtx(const std::vector<std::vector<Float_t> >& vtx)
  {

    for(auto const point : vtx) {
      if(point.size()!=4) {
	Message::get()->send(MSG::ERROR,__FUNCTION__,"Input must have 4 dimension (x,y,z,E)!");
	return;
      }     
    }
    
    fEdepVtx = vtx;
  }
  
  //-------------------------------------------------------------
  Double_t mcshower::MotherAngle2D(const GEO::View_t view) const
  //-------------------------------------------------------------
  {
    switch(view) {	
    case GEO::kU: return fMotherAngleU;
    case GEO::kV: return fMotherAngleV;
    case GEO::kW: return fMotherAngleW;
    case GEO::k3D:
    case GEO::kUnknown:
      Message::get()->send(MSG::ERROR,__FUNCTION__,Form("No angle stored for view: %d",view));
      break;
    }
    return 0;
  }

  //-------------------------------------------------------------
  Float_t mcshower::DaughterAngle2D(const GEO::View_t view) const
  //-------------------------------------------------------------
  {
    switch(view) {	
    case GEO::kU: return fDaughterAngleU;
    case GEO::kV: return fDaughterAngleV;
    case GEO::kW: return fDaughterAngleW;
    case GEO::k3D:
    case GEO::kUnknown:
      Message::get()->send(MSG::ERROR,__FUNCTION__,Form("No angle stored for view: %d",view));
      break;
    }
    return 0;
  }

  //----------------------------------------------------
  Float_t mcshower::Charge(const GEO::View_t view) const
  //----------------------------------------------------
  {
    switch(view) {	
    case GEO::kU: return fChargeU;
    case GEO::kV: return fChargeV;
    case GEO::kW: return fChargeW;
    case GEO::k3D:
    case GEO::kUnknown:
      Message::get()->send(MSG::ERROR,__FUNCTION__,Form("No angle stored for view: %d",view));
      break;
    }
    return 0;
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
