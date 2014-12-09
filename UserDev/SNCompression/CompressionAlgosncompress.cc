#ifndef COMPRESSIONALGOSNCOMPRESS_CC
#define COMPRESSIONALGOSNCOMPRESS_CC

#include "CompressionAlgosncompress.hh"

namespace compress {
  

  void CompressionAlgosncompress::ApplyCompression(const std::vector<unsigned short> &waveform, int mode){
    std::cout << "this began" << std::endl;
    double base = 0;
    int buffer = 0;

    //0 -> Collection
    if(mode == 0){
      base = 400;
      buffer = 3;
    }
    else if(mode == 1){
      base = 2048;
      buffer = 5;
    }
    else{
      base = 2048;
      buffer = 10;
    }

    std::vector<unsigned short> outputwf;

    int diff = 0;
    int i = 0;
    int deltaB = 1;
    int deltaV = 4;
    int c = 10;
    
    double baseline[3]; 
    double variance[3]; 
    int save = 0;
    
    for(int pos = 0; pos< 9600; pos+= _block*3){
      save = 0;
      for(int j = 0; j<3; j++){
	variance[j] = 0;
	baseline[j] = 0;
      }
      //find baseline
      for(i = pos;i < pos + _block; i++){
	baseline[0] += waveform[i];
      }
      for(; i< pos +(_block*2) ; i++){
	baseline[1] += waveform[i];
      }
      for( ; i< pos +(_block*3) ; i++){
	baseline[2] += waveform[i];
      }
      
      //find variance
      for(; i< pos + (_block); i++){
	diff = abs(waveform[i] - baseline[0]);
	if(diff < _block -1) {variance[1] += diff*diff;} 
	else{ variance[1] += 4095;}
      }
      for(; i< pos + (_block*2); i++){
	diff = abs(waveform[i] - baseline[0]);
	if(diff < _block -1) {variance[2] += diff*diff;} 
	else{ variance[2] += 4095;}
      } 
      for( ; i< pos +(_block*3); i++){
	diff = abs(waveform[i] - baseline[0]);
	if(diff < _block -1) {variance[3] += diff*diff;} 
	else{ variance[3] += 4095;}
      }
      
      for(int j = 0; j<3; j++){
	baseline[j] = baseline[j]/_block;
	variance[j] = variance[j]/_block;
      }
      
      for(int j = 0; j<3 ; j++){
	save = pos;
	for(; i< pos + 4*_block && i<9600; i++){
	  //  std::cout << i << std::endl;
	  if(waveform[i] > base + c){
	    
	    if(i > save+5){
	      save = i;
	      if( save > 0){
		for(int k =save - buffer; k<save + buffer; k++){
		  std::cout << k << "<- ex i" << std::endl;
		  outputwf.push_back(waveform[k]);
		}
		  _OutWFStartTick.push_back(save - buffer);
	      }
	    }
	    
	  }
	  else if(waveform[i] < base - c){
	    
	    if(i > save+5){
	      save = i;
	      if( save > 0){
		for(int k =save - buffer; k<save + buffer; k++){
		  //	   std::cout << k << "<- exi i" << std::endl;
		   outputwf.push_back(waveform[k]);
		}
		  _OutWFStartTick.push_back(save - buffer);
	      }
	    }
	  }
	}
      }

      // std::cout << "i is " << i << std::endl;
      //std::cout << "waveform is " << waveform[i] << std::endl;

      if(debug  && i<50){
	std::cout << "The block is "  << i << std::endl;
	std::cout << "The saved time is " << save << std::endl;
	std::cout << "The baseline is " << base << std::endl;
      }

      if(baseline[2] - baseline[1] < deltaB && baseline[2] - baseline[0] < deltaB && baseline[1] - baseline[0] < deltaB){
	if(variance[2] - variance[1] < deltaV && variance[2] - variance[0] < deltaV && variance[1] - variance[0] < deltaV){
	  base = baseline[1];
	}
      }
    }
     
    _OutWF.push_back(outputwf);
    // Set the start tick for the output waveform to be 0
    // if this were something actually serious/useful
    // we should set the start-tick to be the time-tick
    // in the old waveform at which the new waveform starts
    // _OutWFStartTick.push_back(0);

    return;
  }

}

#endif
