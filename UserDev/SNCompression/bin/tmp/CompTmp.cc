#ifndef COMPTMP_CC
#define COMPTMP_CC

#include "CompTmp.hh"

namespace compress {
  
  void CompTmp::ApplyCompression(const std::vector<unsigned short> &waveform){

    /// This algorithm simply returns the original waveform as the compressed one! Not very useful!
    
    _OutWF.push_back(waveform);
    _OutWFStartTick.push_back(0);

    return;
  }

}

#endif
