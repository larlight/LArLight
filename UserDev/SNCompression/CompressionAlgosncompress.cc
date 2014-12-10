#ifndef COMPRESSIONALGOSNCOMPRESS_CC
#define COMPRESSIONALGOSNCOMPRESS_CC

#include "CompressionAlgosncompress.hh"

namespace compress {
  

  void CompressionAlgosncompress::ApplyCompression(const std::vector<unsigned short> &waveform, int mode){
    std::cout << "this began" << std::endl;
    double base = 0;
    int buffer = 0;

    std::cout << "mode: " << mode << std::endl;

    //0 -> Collection
    if(mode == 0){
      base = 400;
      buffer = 15;
    }
    else if(mode == 1){
      base = 2048;
      buffer = 15;
    }
    else{
      base = 2048;
      buffer = 15;
    }

    std::vector<unsigned short> outputwf;

    int diff = 0;
    // keep track of bin position for baseline calculation:
    int i = 0;
    // keep track of bin position for variance calculation:
    int ii = 0;
    // keep track of whether 3-block segment is "interesting" -> default is false
    bool interesting = false;
    
    double baseline[3]; 
    double variance[3]; 
    int save = 0;
    
    for(int pos = 0; pos < 150; pos+= 3){

      int thistick = pos*_block;

      if (debug)
	std::cout << "position is: " << pos << std::endl;
      save = 0;
      for(int j = 0; j<3; j++){
	variance[j] = 0;
	baseline[j] = 0;
      }
      //find baseline
      for(i = thistick;i < thistick + _block; i++){
	baseline[0] += waveform[i];
      }
      for(; i< thistick +(_block*2) ; i++){
	baseline[1] += waveform[i];
      }
      for( ; i< thistick +(_block*3) ; i++){
	baseline[2] += waveform[i];
      }

      for(int j = 0; j<3; j++)
	baseline[j] = baseline[j]/_block;
      
      //find variance
      for(ii = thistick; ii< thistick + (_block); ii++){
	diff = waveform[ii] - baseline[0];
	if(diff < _block -1) {variance[0] += diff*diff;} 
	else{ variance[0] += 4095;}
      }
      for(; ii< thistick + (_block*2); ii++){
	diff = waveform[ii] - baseline[0];
	if(diff < _block -1) {variance[1] += diff*diff;} 
	else{ variance[1] += 4095;}
      } 
      for( ; ii< thistick +(_block*3); ii++){
	diff = waveform[ii] - baseline[0];
	if(diff < _block -1) {variance[2] += diff*diff;} 
	else{ variance[2] += 4095;}
      }

      for(int j = 0; j<3; j++)
	variance[j] = variance[j]/_block;
      

      if (debug){
	std::cout << "Baseline. Block 1: " << baseline[0] << "\tBlock 2: " << baseline[1] << "\tBlock 3: " << baseline[2] << std::endl;
	std::cout << "Variance. Block 1: " << variance[0] << "\tBlock 2: " << variance[1] << "\tBlock 3: " << variance[2] << std::endl;
      }
      

      // Now determine if these 3 blocks are interesting.
      // if so, try and look for a waveform within
      if ( (baseline[2] - baseline[1] < _deltaB && baseline[2] - baseline[0] < _deltaB && baseline[1] - baseline[0] < _deltaB) &&
	   (variance[2] - variance[1] < _deltaV && variance[2] - variance[0] < _deltaV && variance[1] - variance[0] < _deltaV) ){
	// no -> boring
	base = baseline[1];
	interesting = false;
      }
      else{
	// yes! interesting
	interesting = true;
    }
    
      if (interesting){
	// Then go through the 3 blocks again trying to find a waveform to save
	
	// save will keep track of tick at which waveform goes above threshold
	// == 0 if not -> use as logic method to decide if to push back or not
	save = 0;
	// also keep track of each new sub-waveform to push back to output waveform
	outputwf.clear();
	// loop over all 3*_block ticks together applyting threshold _c
	// after basline subtraction w/ baseline "base"
	for (int j=0; j < _block*3; j++){
	  if (waveform[thistick+j] > base + _c){
	    // yay -> active
	    // if save == 0 it means it's a new pulse! (previous tick was quiet)
	    if ( save == 0){
	      save = thistick+j;
	      // also, since we just started...add "backwards ticks" to account for padding
	      for (int back=buffer; back > 0; back--)
		outputwf.push_back(waveform[thistick+j-back]);
	    }
	    // add bin content to temporary output waveform
	    outputwf.push_back(waveform[thistick+j]);
	  }
	  
	  else{
	    // we are in a sub-threshold region.
	    // 2 possibilities:
	    // 1) we were in a sub-threshold region at the previous tick -> then just carry on
	    // 2) we were in an active region in the previous tick -> we just "finished" this mini-waveform.
	    //    then Complete padding and save to output
	    if ( save > 0 ){
	      // finish padding
	      for (int back=1; back < buffer; back++)
		outputwf.push_back(waveform[thistick+j+back]);
	      // push back waveform and time-tick
	      _OutWF.push_back(outputwf);
	      _OutWFStartTick.push_back(save-buffer);
	      // finally clear outputwf for next round
	      outputwf.clear();
	      // turn off save mode
	      save = 0;
	    }
	  }
	  
	}

      }//if interesting!

    }//for all 3-block segments

	/*
	for(int j = 0; j<3 ; j++){
	  save = pos;
	  for(; i< pos + 4*_block && i<300; i++){
	    //  std::cout << i << std::endl;
	    if(waveform[i] > base + _c){
	      
	      if(i > save+5){
		save = i;
		if( save > 0){
		for(int k =save - buffer; k<save + buffer; k++){
		  if (debug)
		    std::cout << k << "<- ex " << i << std::endl;
		  outputwf.push_back(waveform[k]);
		}
		_OutWFStartTick.push_back(save - buffer);
		}
	      }
	      
	    }
	    else if(waveform[i] < base - _c){
	      
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
      }// if interesting!
      
      // std::cout << "i is " << i << std::endl;
      //std::cout << "waveform is " << waveform[i] << std::endl;

      if(debug  && i<50){
	std::cout << "The block is "  << i << std::endl;
	std::cout << "The saved time is " << save << std::endl;
	std::cout << "The baseline is " << base << std::endl;
      }


    }
     
    _OutWF.push_back(outputwf);
    // Set the start tick for the output waveform to be 0
    // if this were something actually serious/useful
    // we should set the start-tick to be the time-tick
    // in the old waveform at which the new waveform starts
    // _OutWFStartTick.push_back(0);
    */

    return;
  }

}

#endif
