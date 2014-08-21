#ifndef PULSE_RECO_CC
#define PULSE_RECO_CC

#include "pulse_reco.hh"

namespace larlight {

  //*************************************
  pulse_reco::pulse_reco() : ana_base() {
  //*************************************

    _name="pulse_reco";

    _input_type = DATA::PMTFIFO;

  };

  //***************************************************************
  bool pulse_reco::initialize(){
  //***************************************************************

    if(_input_type == DATA::PMTFIFO) print(MSG::NORMAL,__FUNCTION__,"Analyzing PMT waveform...");

    else if(_input_type == DATA::TPCFIFO) print(MSG::NORMAL,__FUNCTION__,"Analyzing TPC waveform...");

    else {

      print (MSG::ERROR,__FUNCTION__,
	     Form("Invalid input data type (=%d) ... must be PMT/TPC waveform (=%d/%d)",
		  _input_type,DATA::PMTFIFO,DATA::TPCFIFO));

      return false;
    }

    _output_data_type.clear();
    for(size_t i=0; i<_mgr.algo_count(); ++i) {

      auto const name = _mgr.get_algo(i).name();

      if(name == "algo_threshold") {

	if(_input_type==DATA::PMTFIFO) _output_data_type.push_back(DATA::PMTPulse_ThresWin);
	else _output_data_type.push_back(DATA::TPCPulse_ThresWin);

      }else if(name == "algo_fixed_window"){

	if(_input_type==DATA::PMTFIFO) _output_data_type.push_back(DATA::PMTPulse_FixedWin);
	else _output_data_type.push_back(DATA::TPCPulse_FixedWin);	

      }else{

	print (MSG::WARNING,__FUNCTION__,
	       Form("Unrecognized algorithm name: %s ... will use a generic storage type DATA::Pulse!",
		    name.c_str())
	       );

	_output_data_type.push_back(DATA::Pulse);

      }

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

	status = status && _mgr.run_reco(wf);

	store_reco(storage, &wf, wf.disc_id());
	
      }

    } // end of PMT waveform loop
    else if(_input_type == DATA::TPCFIFO) {

      auto const& tpc_waveforms = (event_tpcfifo*)(storage->get_data(_input_type));

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

	status = status && _mgr.run_reco(wf);

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

  //**********************************************************************************
  void pulse_reco::store_reco(storage_manager* storage,
			      const fifo* wf,
			      FEM::DISCRIMINATOR disc_id)
  //**********************************************************************************
  {

    //
    // Store reconstructed pulses
    //
    for(size_t i=0; i<_mgr.algo_count(); ++i) {

      auto const& algo = _mgr.get_algo(i);

      event_pulse* pulses = (event_pulse*)(storage->get_data(_output_data_type.at(i)));

      for(size_t i=0; i < algo.get_npulse(); ++i) {
	
	// Fill output data product for waveform-wise info
	pulse my_pulse(pulses->data_type());
	
	my_pulse.set_channel_number   ( wf->channel_number());
	
	my_pulse.set_readout_sample_number ( wf->readout_sample_number_64MHz() );
	my_pulse.set_readout_frame_number  ( wf->readout_frame_number()        );
	my_pulse.set_disc_id               ( disc_id                           );
	  
	my_pulse.set_ped_mean   ( algo.ped_mean()            );
	my_pulse.set_ped_rms    ( algo.ped_rms()             );
	my_pulse.set_charge     ( algo.get_pulse(i).area    );
	my_pulse.set_pulse_peak ( algo.get_pulse(i).peak    );
	my_pulse.set_start_time ( algo.get_pulse(i).t_start );
	my_pulse.set_max_time   ( algo.get_pulse(i).t_max   );
	my_pulse.set_end_time   ( algo.get_pulse(i).t_end   );

	pulses->push_back(my_pulse);

	// Accumulate event-wise charge/amplitude sum
	pulses->set_sum_charge ( pulses->sum_charge() + algo.get_pulse(i).area);
	pulses->set_sum_peak   ( pulses->sum_peak()   + algo.get_pulse(i).peak);
	pulses->set_npulse     ( pulses->npulse()     + 1                            );
	
      } // end of reco-ed pulse loop
    } // end of reco algorithm loop
   
  }

  //***************************************************************
  bool pulse_reco::finalize() {
  //***************************************************************

    return true;

  }
  
}
#endif
