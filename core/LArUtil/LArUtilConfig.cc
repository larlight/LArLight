#ifndef LARUTILCONFIG_CC
#define LARUTILCONFIG_CC

#include "LArUtilConfig.hh"

namespace larutil {

  larlight::GEO::DetId_t LArUtilConfig::_detector = larlight::GEO::kMicroBooNE;

  bool LArUtilConfig::SetDetector(larlight::GEO::DetId_t type)
  {
    if(_detector == type) return true;
    bool status = true;
    switch(type) {

    case larlight::GEO::kArgoNeuT: 
    case larlight::GEO::kMicroBooNE:
      _detector = type;
      break;
    case larlight::GEO::kBo:
    case larlight::GEO::kLBNE10kt:
    case larlight::GEO::kJP250L:
    case larlight::GEO::kLBNE35t:
    case larlight::GEO::kLBNE34kt:
    case larlight::GEO::kCSU40L:
    case larlight::GEO::kLArIAT:
    case larlight::GEO::kICARUS:
    case larlight::GEO::kDetIdMax:
      larlight::Message::send(larlight::MSG::ERROR,__FUNCTION__,
			      Form("Detector type: %d not supported!",type));
      status = false;
    }
    return status;
  }

}

#endif
