#ifndef PULSE_RECO_CC
#define PULSE_RECO_CC

#include "pulse_reco.hh"

//***************************************************************
pulse_reco::pulse_reco() : ana_base(),  _reco_algo_v(), _ped_algo(){
//***************************************************************

  _reco_algo_v.clear();

  _ped_nsample_cosmic = 1;

  _ped_nsample_beam   = 8;

  _ped_method = kHEAD;

  _name="pulse_reco";

};

//***************************************************************
bool pulse_reco::initialize(){
//***************************************************************
  
  if(!(_reco_algo_v.size())) {

    Message::send(MSG::ERROR,__PRETTY_FUNCTION__,"Pulse reconstruction algorithm not set!");
		  
    return false;
  }

  return true;

}

//***************************************************************
bool pulse_reco::analyze(storage_manager* storage){
//***************************************************************

  pmt_wf_collection *waveforms = (pmt_wf_collection*)(storage->get_data(DATA_STRUCT::PMT_WF_COLLECTION));

  bool status = true;
  
  for(pmt_wf_collection::iterator iter(waveforms->begin());
      iter!=waveforms->end();
      ++iter){
    
    //
    // Step 0: skipe 0-length waveform with a warning message
    //    
    if((*iter).size()<1){
      
      Message::send(MSG::WARNING,__PRETTY_FUNCTION__,
		    Form("Found 0-length waveform vector! Skipping Event = %d, Ch. = %d ...",
			 waveforms->event_id(), (*iter).channel_number()));
      continue;
    }
    
    //
    // Step 1: apply pedestal estimation
    //  
    double ped_mean = 0;
    double sigma  = 0;
    // Figure out whether this is a beam readout or not
    size_t ped_nsample = ( is_beam(&(*iter)) ? _ped_nsample_beam : _ped_nsample_cosmic);

    switch(_ped_method){

    case kHEAD:

      _ped_algo.compute_pedestal(&(*iter), 0, ped_nsample);

      ped_mean = _ped_algo.mean();

      sigma  = _ped_algo.sigma();

      break;

    case kTAIL:

      _ped_algo.compute_pedestal(&(*iter), ((*iter).size()-ped_nsample), ped_nsample);
      
      ped_mean = _ped_algo.mean();

      sigma  = _ped_algo.sigma();

      break;

    case kBOTH:

      _ped_algo.compute_pedestal(&(*iter), 0, ped_nsample);

      ped_mean = _ped_algo.mean();

      sigma  = _ped_algo.sigma();

      _ped_algo.compute_pedestal(&(*iter), ((*iter).size()-ped_nsample), ped_nsample);
      
      if( sigma > _ped_algo.sigma() ) {

	ped_mean = _ped_algo.mean();
	
	sigma  = _ped_algo.sigma();

      }
      
      break;
    }

    //
    // Step 2: apply reco algos
    //
    for(auto reco_algo : _reco_algo_v){

      reco_algo->set_ped_mean(ped_mean);
      
      reco_algo->set_ped_rms (sigma);
    
      status = status && reco_algo->reco(&(*iter));

      //
      // Step 3: loop over reconstructed pulse ane store.
      //
      pulse_collection *pulses = (pulse_collection*)(storage->get_data(reco_algo->storage_type()));

      for(size_t i=0; i < reco_algo->get_npulse(); ++i) {
	
	// Fill output data product for waveform-wise info
	pulse_info pulse;
	
	pulse.set_timeslice        ( (*iter).timeslice()        );
	pulse.set_channel_number   ( (*iter).channel_number()   );
	pulse.set_channel_frame_id ( (*iter).channel_frame_id() );
	pulse.set_disc_id          ( (*iter).disc_id()          );
	
	pulse.set_ped_mean   ( reco_algo->ped_mean()            );
	pulse.set_ped_rms    ( reco_algo->ped_rms()             );
	pulse.set_charge     ( reco_algo->get_pulse(i)->area    );
	pulse.set_pulse_peak ( reco_algo->get_pulse(i)->peak    );
	pulse.set_start_time ( reco_algo->get_pulse(i)->t_start );
	pulse.set_max_time   ( reco_algo->get_pulse(i)->t_max   );
	pulse.set_end_time   ( reco_algo->get_pulse(i)->t_end   );
	
	pulses->push_back(pulse);

	// Accumulate event-wise charge/amplitude sum
	pulses->set_sum_charge ( pulses->sum_charge() + reco_algo->get_pulse(i)->area);
	pulses->set_sum_peak   ( pulses->sum_peak()   + reco_algo->get_pulse(i)->peak);
	pulses->set_npulse     ( pulses->npulse()     + 1                            );

      } // end of reco-ed pulse loop
    } // end of reco algorithm loop
  } // end of PMT waveform loop

  return status;
  
  }

//***************************************************************
bool pulse_reco::finalize() {
//***************************************************************

  _fout->cd();
  
  return true;

}

#endif
