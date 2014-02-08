#ifndef MCNU_CC
#define MCNU_CC

#include "mcnu.hh"

namespace larlight {

  //***********************************************************
  mcnu::mcnu(DATA::DATA_TYPE type)  : data_base(type), 
				      fNu(DATA::MCParticle), 
				      fLepton(DATA::MCParticle)
  //***********************************************************
  {
    if(_type!=DATA::MCNeutrino) {

      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Provided data type (%s) not supported! Reset to default.",DATA::DATA_TREE_NAME[_type].c_str()));
      
      _type=DATA::MCNeutrino;
    }

    clear_data();
  }

  //***********************************************************
  mcnu::mcnu(mcpart &nu, mcpart &lep,
	     Int_t CCNC, Int_t mode, Int_t interactionType,
	     Int_t target, Int_t nucleon, Int_t quark,
	     Double_t w, Double_t x, Double_t y, Double_t qsqr)
    : data_base(DATA::MCNeutrino),
      fNu(nu),
      fLepton(lep),
      fMode(mode),
      fInteractionType(interactionType),
      fCCNC(CCNC),
      fTarget(target),
      fHitNuc(nucleon),
      fHitQuark(quark),
      fW(w),
      fX(x),
      fY(y),
    fQSqr(qsqr)
  //***********************************************************
  {}

  //**********************
  void  mcnu::clear_data()
  //**********************
  {
    data_base::clear_data();
    fNu.clear_data();
    fLepton.clear_data();

    fMode            = DATA::INVALID_INT;
    fInteractionType = DATA::INVALID_INT;
    fCCNC            = DATA::INVALID_INT;
    fTarget          = DATA::INVALID_INT;
    fHitNuc          = DATA::INVALID_INT;
    fHitQuark        = DATA::INVALID_INT;
    fW               = DATA::INVALID_DOUBLE;
    fX               = DATA::INVALID_DOUBLE;
    fY               = DATA::INVALID_DOUBLE;
    fQSqr            = DATA::INVALID_DOUBLE;
  }

}
  
#endif
