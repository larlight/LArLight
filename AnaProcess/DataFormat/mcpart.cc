#ifndef MCPART_CC
#define MCPART_CC

#include "mcpart.hh"

namespace larlight {

  mcpart::mcpart(DATA::DATA_TYPE type) : data_base(type) 
  {
    if(_type!=DATA::MCParticle) {

      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Provided data type (%s) not supported! Reset to default.",DATA::DATA_TREE_NAME[_type].c_str()));

      _type=DATA::MCParticle;
    }

    clear_data();
  }

  //********************************************
  mcpart::mcpart(const Int_t       trackId,
		 const Int_t       pdgid,
		 const std::string process,
		 const Int_t       mother,
		 const Double_t    mass,
		 const Int_t       status)
    : data_base(DATA::MCParticle) 
  //********************************************
  {
    clear_data();
    ftrackId = trackId;
    fpdgCode = pdgid;
    fprocess = process;
    fmother  = mother;
    fmass    = mass;
    fstatus  = status;    
  }

  //***********************
  void mcpart::clear_data() 
  //***********************
  {
    data_base::clear_data();
    fstatus  = DATA::INVALID_INT;
    ftrackId = DATA::INVALID_INT;
    fpdgCode = DATA::INVALID_INT;
    fmother  = DATA::INVALID_INT;
    fprocess = "";
    ftrajectory.clear_data();
    fmass    = DATA::INVALID_DOUBLE;
    fpolarization.Clear();
    fdaughters.clear();
    fWeight  = DATA::INVALID_DOUBLE;
    fGvtx.Clear();
  }

  //*********************************************************************************
  event_mcpart::event_mcpart(DATA::DATA_TYPE type) : std::vector<larlight::mcpart>(),
						     data_base(type) 
  //*********************************************************************************
  {
    if(_type!=DATA::MCParticle) {

      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Provided data type (%s) not supported! Reset to default.",DATA::DATA_TREE_NAME[_type].c_str()));

      _type=DATA::MCParticle;
    }

    clear_data();
  }

}

#endif
