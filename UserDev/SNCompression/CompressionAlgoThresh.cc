#ifndef COMPRESSIONALGOTHRESH_CXX
#define COMPRESSIONALGOTHRESH_CXX

#include "CompressionAlgoThresh.hh"

namespace compress {
  
  void CompressionAlgoThresh::ApplyCompression(const std::vector<unsigned short> &waveform, int mode, const UInt_t ch){

    //Algorithm Computation starts here!
    PrepareAlgo();
    //assign the waveform vector for this specific instance of the algorithm
    SetInWF(waveform);
    //calculate the baseline from the firxt X samples
    FindBaseline();
    //loop over wf...if a segment moves above threshold -> add to output vector
    //keep track on whether already triggered or not
    bool active = false;
    int startTick = 0;
    for (size_t i=0; i < _InWF.size(); i++){
      if (_debug) {
	std::string x;
	std::cout << "next tick? Enter any key:" << std::endl;
	std::cin >> x;
	std::cout << "thanks!" << std::endl;
      }
      if (_verbose) {
	std::cout << "current ADC tick: " << _InWF.at(i) << std::endl;
	std::cout << "Baseline: " << _Baseline << std::endl;
	std::cout << "Active? " << active << std::endl;
	std::cout << "tmpWF size: " << _tmpWF.size() << std::endl;
      }
      // if the baseline-subtracted ADC tick is greater than
      // the threshold in absolute value -> fire compression!
      if ( fabs((_InWF.at(i) - _Baseline)) > _Threshold ){
	if (!active) { startTick = i; } // if it is the first Tick above baseline -> start a new waveform
	active = true;
	_tmpWF.push_back(_InWF.at(i));
      }
      else{
	if ( (active) and (_tmpWF.size() > 0) ){
	  _OutWF.push_back(_tmpWF);
	  _OutWFStartTick.push_back(startTick); // if we just exited an above-threshold region -> save waveform and reset
	  _tmpWF.clear();
	  active = false;
	}
      }
    }

    return;
  }
  
  void CompressionAlgoThresh::FindBaseline(){
    
    _Baseline = 0.;
    for (int i=0; i < 4; i++)
      _Baseline += _InWF.at(i);
    _Baseline /= 4;
    
    return;
  }
}

#endif
