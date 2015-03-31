#ifndef COMPRESSIONALGOBASE_CXX
#define COMPRESSIONALGOBASE_CXX

#include "CompressionAlgoBase.hh"

namespace compress {

  CompressionAlgoBase::CompressionAlgoBase(){

    _verbose = false; 
    _InWF.clear();
    _OutWF.clear();
    _OutWFStartTick.clear();
    _OutWFStartTick.clear();
  }
  
  void CompressionAlgoBase::ApplyCompression(const std::vector<unsigned short> &waveform, int mode, const UInt_t ch){

    /// This algorithm simply returns the original waveform as the compressed one! Not very useful!
    std::cout << "Defaut Base Compression Algo called..." << std::endl;
    //Set the Output waveform to be identical to the input waveform
    _OutWF.push_back(waveform);
    // Set the start tick for the output waveform to be 0
    // if this were something actually serious/useful
    // we should set the start-tick to be the time-tick
    // in the old waveform at which the new waveform starts
    _OutWFStartTick.push_back(0);

    return;
  }
  
}

#endif
