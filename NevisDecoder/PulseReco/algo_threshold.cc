#ifndef ALGO_THRESHOLD_CC
#define ALGO_THRESHOLD_CC

#include "algo_threshold.hh"

//***************************************************************
algo_threshold::algo_threshold() : preco_algo_base() {
//***************************************************************

  _name = "algo_threshold";

  _adc_thres = 3;
  
  _nsigma = 5;

  reset();

}

void algo_threshold::reset(){

  preco_algo_base::reset();

}

//***************************************************************
bool algo_threshold::reco(const std::vector<UShort_t> *wf) {
//***************************************************************

  bool fire = false;

  double counter=0;

  double threshold = ( _adc_thres > (_nsigma * _ped_rms) ? _adc_thres : (_nsigma * _ped_rms) );

  threshold += _ped_mean;

  reset();

  for(auto value : *wf){
    
    if( !fire && ((double)value) > threshold ){

      // Found a new pulse

      fire = true;

      _pulse.t_start = counter;

    }
    
    if( fire && ((double)value) < threshold ){
      
      // Found the end of a pulse

      fire = false;

      _pulse.t_end = counter - 1;
      
      _pulse_v.push_back(_pulse);

      _pulse.reset_param();

    }

    if(fire){

      // Add this adc count to the integral

      _pulse.area += ((double)value - (double)_ped_mean);

      if(_pulse.peak < ((double)value - (double)_ped_mean)) {

	// Found a new maximum
	
	_pulse.peak = ((double)value - (double)_ped_mean);

	_pulse.t_max = counter;

      }

    }
    
    counter++;
  }

  if(fire){

    // Take care of a pulse that did not finish within the readout window.
    
    fire = false;
    
    _pulse.t_end = counter - 1;
    
    _pulse_v.push_back(_pulse);
    
    _pulse.reset_param();

  }

  return true;

}

#endif
