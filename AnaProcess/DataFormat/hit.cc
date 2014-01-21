#ifndef HIT_CC
#define HIT_CC

#include "hit.hh"

namespace larlight {

  //#################################################
  hit::hit(DATA::DATA_TYPE type) : data_base(type), 
				   fHitSignal() 
  //#################################################
  {
    if( _type != DATA::Hit &&
	_type != DATA::CrawlerHit &&
	_type != DATA::GausHit &&
	_type != DATA::APAHit &&
	_type != DATA::FFTHit ) {

      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Provided data type (%d) not a valid hit data type! Setting it to DATA::Hit...",_type));
      
      _type = DATA::Hit;
    }
    clear_data();
  }

  //##########################################################################
  hit::hit(const hit &original) : data_base(original),
				  fHitSignal(original.fHitSignal),
				  fStartTime(original.fStartTime),
				  fSigmaStartTime(original.fSigmaStartTime),
				  fPeakTime(original.fPeakTime),
				  fSigmaPeakTime(original.fSigmaPeakTime),
				  fEndTime(original.fEndTime),
				  fSigmaEndTime(original.fSigmaEndTime),
				  fCharge(original.fCharge),
				  fSigmaCharge(original.fSigmaCharge),
				  fMaxCharge(original.fMaxCharge),
				  fSigmaMaxCharge(original.fSigmaMaxCharge),
				  fMultiplicity(original.fMultiplicity),
				  fGoodnessOfFit(original.fGoodnessOfFit),
				  fView(original.fView),
				  fSignalType(original.fSignalType),
                                  fChannel(original.fChannel),
                                  fWire(original.fWire)
  //##########################################################################
  {};
  
  //##########################################################################
  void hit::clear_data()
  //##########################################################################
  {
    data_base::clear_data();
    fHitSignal.clear();
    fStartTime = fPeakTime = fEndTime = -1;
    fSigmaStartTime = fSigmaPeakTime = fSigmaEndTime = -1;
    fCharge = fMaxCharge = -1;
    fSigmaCharge = fSigmaMaxCharge = -1;
    fMultiplicity = -1;
    fGoodnessOfFit = -1;
    fView = GEO::kUnknown;
    fSignalType = GEO::kMysteryType;
    fChannel = 0xffffffff;
    fWire    = 0xffffffff;
  }

  //#################################################################################
  event_hit::event_hit(DATA::DATA_TYPE type) : std::vector<larlight::hit>(), 
					       data_base(type) 
  //#################################################################################
  {
    if( _type != DATA::Hit &&
	_type != DATA::CrawlerHit &&
	_type != DATA::GausHit &&
	_type != DATA::APAHit &&
	_type != DATA::FFTHit ) {

      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Provided data type (%d) not a valid hit data type! Setting it to DATA::Hit...",_type));
      
      _type = DATA::Hit;
    }

    clear_data();

  }


}
#endif
  
