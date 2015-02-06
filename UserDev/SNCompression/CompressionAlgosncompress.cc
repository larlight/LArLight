#ifndef COMPRESSIONALGOSNCOMPRESS_CC
#define COMPRESSIONALGOSNCOMPRESS_CC

#include "CompressionAlgosncompress.hh"

namespace compress {
  

  CompressionAlgosncompress::CompressionAlgosncompress()
    : CompressionAlgoBase()
    , _algo_tree(nullptr)
  {
    _maxADC = 4095;
    _buffer.reserve(3);
    _buffer[0].reserve(2);
    _buffer[1].reserve(2);
    _buffer[2].reserve(2);
    std::vector<std::vector<int> > tmp(3,std::vector<int>(2,0));
    _buffer = tmp;

    //Setup tree
    if (_algo_tree) { delete _algo_tree; }
    _algo_tree = new TTree("_algo_tree","Algorithm-specific Tree");
    _algo_tree->Branch("_pl",&_pl,"pl/I");
    _algo_tree->Branch("_v1",&_v1,"v1/D");
    _algo_tree->Branch("_v2",&_v2,"v2/D");
    _algo_tree->Branch("_v3",&_v3,"v3/D");
    _algo_tree->Branch("_b1",&_b1,"b1/D");
    _algo_tree->Branch("_b2",&_b2,"b2/D");
    _algo_tree->Branch("_b3",&_b3,"b3/D");
    _algo_tree->Branch("_max",&_max,"max/D");
    _algo_tree->Branch("_interesting",&_interesting,"interesting/I");
    _algo_tree->Branch("_save",&_save,"save/I");
    
  }
  
  void CompressionAlgosncompress::SetUVYplaneBuffer(int upre, int upost, int vpre, int vpost, int ypre, int ypost){
    
    _buffer[0][0]  = upre;
    _buffer[0][1] = upost;
    _buffer[1][0]  = vpre;
    _buffer[1][1] = vpost;
    _buffer[2][0]  = ypre;
    _buffer[2][1] = ypost;
    return;
  }
  
  
  void CompressionAlgosncompress::ApplyCompression(const std::vector<unsigned short> &waveform, const int mode, const UInt_t ch){
    
    //double base = 0;
    int buffer = 0;
    
    // entries in the waveform:
    int nTicks = waveform.size();
    
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
    
    _pl = mode;

    for(int pos = 0; pos < 150; pos+= 3){

      int thistick = pos*_block;

      if (_debug)
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
	diff = waveform[ii]-baseline[0];
	if(diff < _block -1) {variance[0] += diff*diff; } 
	else{ variance[0] += 4095;}
      }
      for(; ii< thistick + (_block*2); ii++){
	diff = waveform[ii]-baseline[0];
	if(diff < _block -1) {variance[1] += diff*diff; } 
	else{ variance[1] += 4095;}
      } 
      for( ; ii< thistick +(_block*3); ii++){
	diff = waveform[ii] - baseline[0];
	if(diff < _block -1) {variance[2] += diff*diff;} 
	else{ variance[2] += 4095;}
      }

      for(int j = 0; j<3; j++)
	variance[j] = variance[j]/_block;
      
      _v1 = variance[0];
      _v2 = variance[1];
      _v3 = variance[2];
      _b1 = baseline[0];
      _b2 = baseline[1];
      _b3 = baseline[2];


      if (_debug){
	std::cout << "Baseline. Block 1: " << baseline[0] << "\tBlock 2: " << baseline[1] << "\tBlock 3: " << baseline[2] << std::endl;
	std::cout << "Variance. Block 1: " << variance[0] << "\tBlock 2: " << variance[1] << "\tBlock 3: " << variance[2] << std::endl;
      }
      

      // Now determine if these 3 blocks are interesting.
      // if so, try and look for a waveform within
      if ( ( (baseline[2] - baseline[1]) * (baseline[2] - baseline[1]) < _deltaB ) && 
	   ( (baseline[2] - baseline[0]) * (baseline[2] - baseline[0]) < _deltaB ) && 
	   ( (baseline[1] - baseline[0]) * (baseline[1] - baseline[0]) < _deltaB ) &&
	   ( (variance[2] - variance[1]) * (variance[2] - variance[1]) < _deltaV ) &&
	   ( (variance[2] - variance[0]) * (variance[2] - variance[0]) < _deltaV ) &&
	   ( (variance[1] - variance[0]) * (variance[1] - variance[0]) < _deltaV ) ){
	// no -> boring
	_baselineMap[ch] = baseline[1];
	//	base = baseline[1];
	interesting = false;
      }
      else{
	// yes! interesting
	interesting = true;
      }

      _interesting = 0;
      if (interesting) { _interesting = 1; }
      
      if (interesting && (_baselineMap.find(ch) == _baselineMap.end()) )
	std::cout << "WARNING: interesting stuff but baseline has not yet been set" << std::endl;
      if (interesting && (_baselineMap.find(ch) != _baselineMap.end()) ){

	double base = _baselineMap[ch];
	
	// reset maxima
	_max = 0;

	// Then go through the 3 blocks again trying to find a waveform to save
	
	// save will keep track of tick at which waveform goes above threshold
	// == 0 if not -> use as logic method to decide if to push back or not
	save = 0;
	_save = 0;
	// also keep track of each new sub-waveform to push back to output waveform
	outputwf.clear();
	// loop over all 3*_block ticks together applyting threshold _thresh
	// after basline subtraction w/ baseline "base"
	for (int j=0; j < _block*3; j++){

	  double thisADC = waveform[thistick+j];
	  if (thisADC-base > _max) { _max = thisADC-base; }

	  if (waveform[thistick+j] > base + _thresh){
	    _save = 1;
	    // yay -> active
	    // if save == 0 it means it's a new pulse! (previous tick was quiet)
	    // keep track of maxima
	    if ( save == 0){
	      save = thistick+j;
	      // also, since we just started...add "backwards ticks" to account for padding
	      for (int back=_buffer[mode][0]; back > 0; back--){
		//make sure bin number exists
		if ( (thistick+j-back) >= 0 ) { outputwf.push_back(waveform[thistick+j-back]); }
	      }
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
	      for (int forward=1; forward < _buffer[mode][1]; forward++){
		//make sure bin number exists
		if ( (thistick+j+forward) < nTicks ) { outputwf.push_back(waveform[thistick+j+forward]); }
	      }
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

      _algo_tree->Fill();

    }//for all 3-block segments
    
    return;
  }
  
  void CompressionAlgosncompress::EndProcess(TFile* fout){
    
    std::cout << "End process" << std::endl;
    if (fout){
      std::cout << "here we go!" << std::endl;
      if (_algo_tree)
	_algo_tree->Write();
    }
  }

}
#endif
