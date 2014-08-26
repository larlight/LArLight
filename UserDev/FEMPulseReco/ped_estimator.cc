#ifndef PED_ESTIMATOR_CC
#define PED_ESTIMATOR_CC

#include "ped_estimator.hh"

namespace optreco {

  //***************************************************************
  ped_estimator::ped_estimator(){
  //***************************************************************

    _mean  = -1;

    _sigma = -1;

  }

  //*************************************************************************
  void ped_estimator::compute_pedestal(const std::vector<unsigned short>& wf, 
				       size_t start, 
				       size_t nsample){
  //*************************************************************************

    _mean  = -1;
    _sigma = -1;

    if( (start + nsample) > wf.size() ){

      std::cerr << "\033[93m" << "WARNING: " << "\033[00m"
		<< Form("Wavelength too short (%zu ADC samples) to compute pedestal! (minimum %zu)",
			wf.size(),(start + nsample))
		<< std::endl;

      return;
    }

    for(size_t index=start; index < (start + nsample); ++index)

      _mean += wf.at(index);

    _mean = _mean / ((double)nsample);

    for(size_t index=0; index < (start+nsample); ++index)

      _sigma += pow( (wf.at(index) - _mean), 2 );

    _sigma = sqrt(_sigma/((double)(nsample)));

    return;

  }
}

#endif
