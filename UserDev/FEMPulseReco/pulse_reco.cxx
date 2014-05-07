#ifndef PULSE_RECO_CXX
#define PULSE_RECO_CXX

#include "pulse_reco.hh"

namespace larlight {

  //***************************************************************
  pulse_reco::pulse_reco() : ana_base(),  _reco_algo_v(), _ped_algo(){
  //***************************************************************

    _reco_algo_v.clear();

    _ped_nsample_cosmic = 1;

    _ped_nsample_beam   = 8;

    _ped_method = kHEAD;

    _name="pulse_reco";

    _input_type = DATA::PMTFIFO;

  };

  //***************************************************************
  bool pulse_reco::initialize(){
  //***************************************************************

    if(!(_reco_algo_v.size())) {

      print(MSG::ERROR,__PRETTY_FUNCTION__,"Pulse reconstruction algorithm not set!");

      return false;
    }

    if(_input_type == DATA::PMTFIFO) print(MSG::NORMAL,__FUNCTION__,"Analyzing PMT waveform...");
    else if(_input_type == DATA::TPCFIFO) print(MSG::NORMAL,__FUNCTION__,"Analyzing TPC waveform...");
    else {

      print (MSG::ERROR,__FUNCTION__,
	     Form("Invalid input data type (=%d) ... must be PMT/TPC waveform (=%d/%d)",
		  _input_type,DATA::PMTFIFO,DATA::TPCFIFO));

      return false;
    }

    return true;

  }

  //***************************************************************
  bool pulse_reco::analyze(storage_manager* storage){
  //***************************************************************

    bool status = true;
    if(_input_type == DATA::PMTFIFO) {

      event_pmtfifo *pmt_waveforms = (event_pmtfifo*)(storage->get_data(_input_type));

      for(auto const wf : *pmt_waveforms) {

	//
	// Step 0: skipe 0-length waveform with a warning message
	//    
	if(wf.size()<1){
	  
	  print(MSG::WARNING,__PRETTY_FUNCTION__,
		Form("Found 0-length waveform vector! Skipping Event = %d, Ch. = %d ...",
		     pmt_waveforms->event_id(), wf.channel_number()));
	  continue;
	}

	status = status && run_reco(&wf);

	store_reco(storage, &wf, wf.disc_id());
	
      }

    } // end of PMT waveform loop
    else if(_input_type == DATA::TPCFIFO) {

      event_tpcfifo *tpc_waveforms = (event_tpcfifo*)(storage->get_data(_input_type));

      bool status = true;
      
      for(auto const wf : *tpc_waveforms) {

	//
	// Step 0: skipe 0-length waveform with a warning message
	//    
	if(wf.size()<1){
	  
	  print(MSG::WARNING,__PRETTY_FUNCTION__,
		Form("Found 0-length waveform vector! Skipping Event = %d, Ch. = %d ...",
		     tpc_waveforms->event_id(), wf.channel_number()));
	  continue;
	}

	status = status && run_reco(&wf);

	store_reco(storage, &wf, FEM::DISC_MAX);
	
      }
      
    } // end of PMT waveform loop
    else {

      print(MSG::ERROR,__FUNCTION__,
	    Form("Unsupported data type: %d = \"%s\"",_input_type,DATA::DATA_TREE_NAME[_input_type].c_str()));
      status=false;

    }

    return status;
    
  }

  //**********************************************************
  bool pulse_reco::run_reco(const std::vector<UShort_t>* wf)
  //**********************************************************
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
      
      _ped_algo.compute_pedestal(wf, (wf->size()-ped_nsample), ped_nsample);
      
      ped_mean = _ped_algo.mean();
      
      sigma  = _ped_algo.sigma();
      
      break;
      
    case kBOTH:
      
      _ped_algo.compute_pedestal(wf, 0, ped_nsample);
      
      ped_mean = _ped_algo.mean();
      
      sigma  = _ped_algo.sigma();
      
      _ped_algo.compute_pedestal(wf, (wf->size()-ped_nsample), ped_nsample);
      
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
      
      status = status && reco_algo->reco(wf);
    }
    return status;
  }

  //**********************************************************************************
  void pulse_reco::store_reco(storage_manager* storage,
			      const fifo* wf,
			      FEM::DISCRIMINATOR disc_id)
  //**********************************************************************************
  {

    //
    // Store reconstructed pulses
    //
    for(auto reco_algo : _reco_algo_v){

      event_pulse* pulses = (event_pulse*)(storage->get_data(reco_algo->storage_type()));

      for(size_t i=0; i < reco_algo->get_npulse(); ++i) {
	
	// Fill output data product for waveform-wise info
	pulse my_pulse(pulses->data_type());
	
	my_pulse.set_channel_number   ( wf->channel_number());
	
	my_pulse.set_readout_sample_number ( wf->readout_sample_number_64MHz() );
	my_pulse.set_readout_frame_number  ( wf->readout_frame_number()        );
	my_pulse.set_disc_id               ( disc_id                           );
	  
	my_pulse.set_ped_mean   ( reco_algo->ped_mean()            );
	my_pulse.set_ped_rms    ( reco_algo->ped_rms()             );
	my_pulse.set_charge     ( reco_algo->get_pulse(i)->area    );
	my_pulse.set_pulse_peak ( reco_algo->get_pulse(i)->peak    );
	my_pulse.set_start_time ( reco_algo->get_pulse(i)->t_start );
	my_pulse.set_max_time   ( reco_algo->get_pulse(i)->t_max   );
	my_pulse.set_end_time   ( reco_algo->get_pulse(i)->t_end   );

	pulses->push_back(my_pulse);

	// Accumulate event-wise charge/amplitude sum
	pulses->set_sum_charge ( pulses->sum_charge() + reco_algo->get_pulse(i)->area);
	pulses->set_sum_peak   ( pulses->sum_peak()   + reco_algo->get_pulse(i)->peak);
	pulses->set_npulse     ( pulses->npulse()     + 1                            );
	
      } // end of reco-ed pulse loop
    } // end of reco algorithm loop
   
  }

  //***************************************************************
  bool pulse_reco::finalize() {
  //***************************************************************

    _fout->cd();

    return true;

  }
  
}
#endif
