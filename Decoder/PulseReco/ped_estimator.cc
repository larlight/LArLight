#ifndef PED_ESTIMATOR_CC
#define PED_ESTIMATOR_CC

#include "ped_estimator.hh"

//***************************************************************
ped_estimator::ped_estimator(){
//***************************************************************

  _mean  = -1;

  _sigma = -1;

}

//***************************************************************
void ped_estimator::compute_pedestal(const std::vector<UShort_t>* wf, size_t start, size_t nsample){
//***************************************************************
  
  _mean  = -1;
  _sigma = -1;
  
  if( (start + nsample) > wf->size() ){
    Message::send(MSG::WARNING,__FUNCTION__,
		  Form("Wavelength too short (%zu ADC samples) to compute pedestal! (minimum %zu)",
		       wf->size(),(start + nsample))
		  );
    return;
  }

  for(size_t index=start; index < (start + nsample); ++index)

    _mean += wf->at(index);

  _mean = _mean / ((double)nsample);

  for(size_t index=0; index < (start+nsample); ++index)
    
    _sigma += pow( (wf->at(index) - _mean), 2 );

  _sigma = sqrt(_sigma/((double)(nsample)));

  return;

}

#endif
