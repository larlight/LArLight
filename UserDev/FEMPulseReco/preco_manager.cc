#ifndef PRECO_MANAGER_CC
#define PRECO_MANAGER_CC

#include "preco_manager.hh"

namespace optreco {

  //***********************************************************************
  preco_manager::preco_manager() : _reco_algo_v(), _ped_algo(){
  //***********************************************************************

    _reco_algo_v.clear();

    _ped_nsample_cosmic = 1;

    _ped_nsample_beam   = 8;

    _ped_method = kHEAD;

  };

  //*****************************************************************
  bool preco_manager::run_reco(const std::vector<unsigned short>& wf)
  //*****************************************************************
  {

    bool status=true;
    //
    // Step 1: apply pedestal estimation
    //  
    double ped_mean = 0;
    double sigma  = 0;
    // Figure out whether this is a beam readout or not
    size_t ped_nsample = ( is_beam(wf) ? _ped_nsample_beam : _ped_nsample_cosmic);
    
    switch(_ped_method){
      
    case kHEAD:
      
      _ped_algo.compute_pedestal(wf, 0, ped_nsample);
      
      ped_mean = _ped_algo.mean();
      
      sigma  = _ped_algo.sigma();
      
      break;
      
    case kTAIL:
      
      _ped_algo.compute_pedestal(wf, (wf.size()-ped_nsample), ped_nsample);
      
      ped_mean = _ped_algo.mean();
      
      sigma  = _ped_algo.sigma();
      
      break;
      
    case kBOTH:
      
      _ped_algo.compute_pedestal(wf, 0, ped_nsample);
      
      ped_mean = _ped_algo.mean();
      
      sigma  = _ped_algo.sigma();
      
      _ped_algo.compute_pedestal(wf, (wf.size()-ped_nsample), ped_nsample);
      
      if( sigma > _ped_algo.sigma() ) {
	
	ped_mean = _ped_algo.mean();
	
	sigma  = _ped_algo.sigma();
	
      }
      
      break;
    }

    //
    // Step 2: apply reco algos
    //
    for(auto& reco_algo : _reco_algo_v){

      reco_algo->set_ped_mean(ped_mean);
      
      reco_algo->set_ped_rms (sigma);
      
      status = status && reco_algo->reco(wf);
    }
    return status;
  }

}
#endif
