#ifndef COMPRESSIONALGOBASE_CXX
#define COMPRESSIONALGOBASE_CXX

#include "CompressionAlgoBase.hh"

namespace compress {
  
  void CompressionAlgoBase::ApplyCompression(const std::vector<unsigned short> &waveform){
    
    _OutWF.push_back(waveform);
    _OutWFStartTick.push_back(1);
    return;
  }
  
}

#endif
