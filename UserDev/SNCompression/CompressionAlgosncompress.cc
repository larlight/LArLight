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
    _thresh.reserve(3);
    std::vector<std::vector<int> > tmp(3,std::vector<int>(2,0));
    _buffer = tmp;

    _fillTree = false;

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
    _baselines.clear();
    _variances.clear();
    
    int diff = 0;
    // keep track of bin position for baseline calculation:
    int i = 0;
    // keep track of bin position for variance calculation:
    int ii = 0;
    // keep track of whether 3-block segment is "interesting" -> default is false
    bool interesting = false;
    
    double baseline[3];
    double variance[3];
    // Tick @ which waveform starts
    int start = 0;
    
    _pl = mode;

    for(int pos = 0; pos < 150; pos+= 3){

      int thistick = pos*_block;

      if (_debug)
	std::cout << "position is: " << pos << std::endl;
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

      for(int j = 0; j<3; j++){
	variance[j] = variance[j]/_block;
	_baselines.push_back(baseline[j]);
	_variances.push_back(variance[j]);
      }
      
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
      
      if (_verbose && interesting)
	std::cout << "Interesting @ tick " << thistick << std::endl;

      _interesting = 0;
      if (interesting) { _interesting = 1; }
      
      if (_verbose && interesting && (_baselineMap.find(ch) == _baselineMap.end()) )
	std::cout << "WARNING: interesting stuff but baseline has not yet been set for ch " << ch << std::endl;
      if (interesting && (_baselineMap.find(ch) != _baselineMap.end()) ){

	double base = _baselineMap[ch];
	
	// reset maxima
	_max = 0;

	// Then go through the 3 blocks again trying to find a waveform to save
	
	// save will keep track of tick at which waveform goes above threshold
	// == 0 if not -> use as logic method to decide if to push back or not
	_save = 0;
	// also keep track of each new sub-waveform to push back to output waveform
	outputwf.clear();
	// loop over all 3*_block ticks together applyting threshold _thresh
	// after basline subtraction w/ baseline "base"
	for (int j=0; j < _block*3; j++){

	  double thisADC = waveform[thistick+j];
	  if (thisADC-base > _max) { _max = thisADC-base; }

	  if ( PassThreshold(thisADC, base) ){
	    if (_verbose) { std::cout << "+ "; }
	    _save = 1;
	    // yay -> active
	    // if start == 0 it means it's a new pulse! (previous tick was quiet)
	    // keep track of maxima
	    if ( start == 0 ){
	      start = thistick+j;
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
	    if ( start > 0 ){
	      // finish padding
	      for (int forward=1; forward < _buffer[mode][1]; forward++){
		//make sure bin number exists
		if ( (thistick+j+forward) < nTicks ) { outputwf.push_back(waveform[thistick+j+forward]); }
	      }
	      // push back waveform and time-tick
	      if (_verbose) {
		std::cout << std::endl;
		std::cout << "saving [" << start-buffer << ", " << start-buffer+outputwf.size() << "]" << std::endl;
	      }
	      _OutWF.push_back(outputwf);
	      _OutWFStartTick.push_back(start-buffer);
	      // finally clear outputwf for next round
	      outputwf.clear();
	      // turn off save mode
	      _save = 0;
	      start = 0;
	    }
	  }
	  
	}
	if (_verbose) { std::cout << std::endl; }
      }//if interesting!
      if (_fillTree)
	_algo_tree->Fill();

    }//for all 3-block segments

    if (_verbose) { std::cout << "Waveforms in event: " << _OutWF.size() << std::endl; }

    // if overlap between save regions, fix here
    // Merge compressed regions (to remove overlaps and double-counting
    int merges = 0;
    while (MergeOverlaps())
      merges += 1;    
    if (merges > 0 && _verbose)
      std::cout << "Merges: " << merges << "\t Out WFs: " << _OutWF.size() << std::endl;


    return;
  }


  bool CompressionAlgosncompress::PassThreshold(double thisADC, double base){

    // if positive threshold
    if (_thresh[_pl] >= 0){
      if (thisADC > base + _thresh[_pl])
	return true;
    }
    // if negative threshold
    else{
      if (thisADC < base + _thresh[_pl])
	return true;
    }
    
    return false;
  }
  
  void CompressionAlgosncompress::EndProcess(TFile* fout){
    
    if (fout){
      if (_algo_tree)
	_algo_tree->Write();
    }

    return;
  }


  bool CompressionAlgosncompress::MergeOverlaps()
  {
    
    // make sure the two vectors have the same size!
    if (_OutWF.size() != _OutWFStartTick.size()){
      std::cout << "FAILFAILFAIL..not equal size!!!" << std::endl;
      return false;
    }

    // if both 0-length, return true
    if ( _OutWF.size() == 0 )
      return false;

    // keep track of wether anything was merged
    bool merged = false;
    // have the last two segments been overlapped?
    bool overlap = false;
    // has the last segment been considered?
    bool last = false;
    // vectors where to store merged regions
    std::vector<std::vector<unsigned short> > mergedADCs;
    mergedADCs.clear();
    std::vector<int> mergedTimes;
    mergedTimes.clear();

    // check if there are any overlaps in the times
    for (size_t n=0; n < _OutWF.size()-1; n++){
      // if the end time of segment n is equal or larger than end time of segment n+1
      if (_OutWFStartTick[n]+_OutWF[n].size() >= _OutWFStartTick[n+1]){
	if (_verbose) { 
	  std::cout << "Overlap between [" << _OutWFStartTick[n] << ", "
		    << _OutWFStartTick[n]+_OutWF[n].size() << "] and ["
		    << _OutWFStartTick[n+1] << ", " << _OutWFStartTick[n+1]+_OutWF[n+1].size()
		    << "]" << std::endl;
	}
	merged = true;
	overlap = true;
	if (n+2 == _OutWF.size())
	  last = true; // last wf was considered
	// then merge the two
	std::vector<unsigned short> thisregion = _OutWF[n];
	// how many ticks do we need to add?
	size_t diff = (_OutWFStartTick[n] + _OutWF[n].size() - _OutWFStartTick[n+1]);
	for (size_t i= diff; i < _OutWF[n+1].size(); i++)
	  thisregion.push_back(_OutWF[n+1][i]);
	// push back the new output waveforms
	if (_verbose)
	  std::cout << "Resulting in: [" << _OutWFStartTick[n] << ", " << _OutWFStartTick[n] + thisregion.size() << "]" << std::endl;
	mergedADCs.push_back(thisregion);
	mergedTimes.push_back(_OutWFStartTick[n]);
	// skip the next one -> n -> n+1
	n = n+1;
      }// if there is an overlap
      // otherwise save the waveform
      else{
	overlap = false;
	mergedADCs.push_back(_OutWF[n]);
	mergedTimes.push_back(_OutWFStartTick[n]);
      }// if no overlap
    }// for all output waveforms found

    // if overlap is false, the last segment was not added
    if (last == false){
      mergedADCs.push_back(_OutWF.back());
      mergedTimes.push_back(_OutWFStartTick.back());
    }

    if (_verbose) { std::cout << "Number of Resulting WFs: " << mergedTimes.size() << std::endl; }

    _OutWF = mergedADCs;
    _OutWFStartTick = mergedTimes;

    return merged;
  }

}
#endif
