#ifndef COMPRESSIONALGOSNCOMPRESS_CC
#define COMPRESSIONALGOSNCOMPRESS_CC

#include "CompressionAlgosncompress.hh"

namespace compress {
  
  void CompressionAlgosncompress::ApplyCompression(const std::vector<unsigned short> &waveform, int mode){

    int base = 0;
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
    int c = 3;
    
    short baseline[3]; 
    short variance[3]; 
    int save = 0;
    
    for(int pos = 0; pos< 9600; pos+= 64){
      save = 0;
      for(int j = 0; j<3; j++){
	variance[j] = 0;
	baseline[j] = 0;
      }
      //find baseline
      for(i = pos;i < pos + 64; i++){
	baseline[0] += waveform[i];
      }
      for(; i< pos +128 ; i++){
	baseline[1] += waveform[i];
      }
      for( ; i< pos +192 ; i++){
	baseline[2] += waveform[i];
      }
      
      //find variance
      for(; i< pos + 64; i++){
	diff = abs(waveform[i] - baseline[0]);
	if(diff < 63) {variance[1] += diff*diff;} 
	else{ variance[1] += 4095;}
      }
      for(; i< pos + 128; i++){
	diff = abs(waveform[i] - baseline[0]);
	if(diff < 63) {variance[2] += diff*diff;} 
	else{ variance[2] += 4095;}
      } 
      for( ; i< pos +192; i++){
	diff = abs(waveform[i] - baseline[0]);
	if(diff < 63) {variance[3] += diff*diff;} 
	else{ variance[3] += 4095;}
      }
      
      for(int j = 0; j<3; j++){
	baseline[j] = baseline[j]/64;
	variance[j] = variance[j]/64;
      }
      
      for(int j = 0; j<3 ; j++){
	for(; i< pos + 256; i++){
	  if(waveform[j] > base + c){
	    save = i;
	  }
	  else if(waveform[j] < base - c){
	    save = i;
	  }
	}
      }

      //for now calling the whole range of pre and post N to be an 8-tick block
      if(save > 0){
	for(int j =save - buffer; j<save + buffer; j++){
	  outputwf.push_back(waveform[j]);
	}
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
    _OutWFStartTick.push_back(0);

    return;
  }

}

#endif
