#ifndef MCTRUTH_CC
#define MCTRUTH_CC

#include "mctruth.hh"

namespace larlight {

  //********************************************************************
  mctruth::mctruth(DATA::DATA_TYPE type) : data_base(type),
					   fMCNeutrino(DATA::MCNeutrino)
  //********************************************************************
  {

    if(_type!=DATA::MCTruth) {

      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Provided data type (%s) not supported! Reset to default.",DATA::DATA_TREE_NAME[_type].c_str()));
      
      _type=DATA::MCTruth;
    }
    
    clear_data();
  }

  //***************************************************
  void mctruth::clear_data()
  //***************************************************
  {
    data_base::clear_data();
    fPartList.clear();
    fMCNeutrino.clear_data();
    fNeutrinoSet=false;
    fOrigin=MC::kUnknown;
  }

  //*************************************
  void mctruth::SetNeutrino(Int_t CCNC,
			    Int_t mode,
			    Int_t interactionType,
			    Int_t target,
			    Int_t nucleon,
			    Int_t quark,
			    Double_t w,
			    Double_t x,
			    Double_t y,
			    Double_t qsqr)
  //*************************************
  {

    if( !fNeutrinoSet ){
      fNeutrinoSet = true;
      // loop over the mcpart list and get the outgoing lepton
      // assume this is a neutral current interaction to begin with
      // which means the outgoing lepton is the incoming neutrino
      mcpart nu  = fPartList[0];
      mcpart lep = fPartList[0];

      // start at i = 1 because i = 0 is the incoming neutrino
      for(unsigned int i = 1; i < fPartList.size(); ++i){
        if(fPartList[i].Mother() == nu.TrackId() &&
           (fPartList[i].PdgCode()  == nu.PdgCode() ||
            abs(fPartList[i].PdgCode()) == abs(nu.PdgCode())-1) ){
          lep = fPartList[i];
          break;
	}
      }//done looping over particles


      fMCNeutrino = mcnu(nu, lep,
			 CCNC, mode, interactionType,
			 target, nucleon, quark,
			 w, x, y, qsqr);

    } // end if mcnu is not already set

    else

      Message::send(MSG::ERROR,__FUNCTION__,"Attempt to set neutrino when already set");

    return;

  }

  //************************************************************************************
  event_mctruth::event_mctruth(DATA::DATA_TYPE type) : std::vector<larlight::mctruth>(),
						       data_base(type)
  //************************************************************************************
  {
    if(_type!=DATA::MCTruth) {

      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Provided data type (%s) not supported! Reset to default.",DATA::DATA_TREE_NAME[_type].c_str()));
      
      _type=DATA::MCTruth;
    }
    clear_data();
  }

}
#endif
  
