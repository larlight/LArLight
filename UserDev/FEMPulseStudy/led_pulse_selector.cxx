#ifndef LED_PULSE_SELECTOR_CC
#define LED_PULSE_SELECTOR_CC

#include "led_pulse_selector.hh"

namespace larlight {

  led_pulse_selector::led_pulse_selector() : _ref_time() { 
    _name="led_pulse_selector"; 
    _pulse_type=DATA::PMTPulse_ThresWin;
    _ref_ch=FEM::INVALID_CH;
    _frame_size=FEM::EMPTY_WORD;
    _time_cut=FEM::EMPTY_WORD;
    clear_event();
  }

  void led_pulse_selector::clear_event(){
    _ref_time.clear();
  }

  bool led_pulse_selector::initialize() {

    bool status = true;
    if(_ref_ch==FEM::INVALID_CH) {

      Message::send(MSG::ERROR,__PRETTY_FUNCTION__,"Reference channel number is not set! Aborting...");

      status=false;

    }

    if(_frame_size == FEM::EMPTY_WORD){

      Message::send(MSG::ERROR,__PRETTY_FUNCTION__,"Frame size not set! Aborting...");

      status = false;

    }

    if(_time_cut == FEM::EMPTY_WORD){

      Message::send(MSG::ERROR,__PRETTY_FUNCTION__,"Timing cut not set! Aborting...");

      status = false;

    }

    return status;
  }

  bool led_pulse_selector::analyze(storage_manager* storage) {

    event_pulse *pulses = (event_pulse*)(storage->get_data(_pulse_type));

    if(pulses->size()==0) {
      Message::send(MSG::ERROR,__FUNCTION__,"No Pulse Found!");
      return false;
    }

    clear_event();

    // Perform analyze() in the base class
    bool status = pulse_selector_simple::analyze(storage);

    if(status){

      // Look for reference time from the specified channel number
      for(event_pulse::const_iterator iter(pulses->begin());
	  iter!=pulses->end();
	  ++iter){
	if((*iter).channel_number() != _ref_ch) continue;

	_ref_time.insert((*iter).readout_frame_number() * _frame_size + (*iter).readout_sample_number());

      }

    }

    return status;
  }


  bool led_pulse_selector::select_pulse(const pulse* data){

    // Run base class selection first
    bool store = pulse_selector_simple::select_pulse(data);

    if(!store) return store;

    store= false;
    UInt_t time_stamp = data->readout_frame_number() * _frame_size + data->readout_sample_number();
    for(auto ts : _ref_time){

      if(ts > time_stamp) {

	  if( (ts-time_stamp) < _time_cut ) store = true; 

	  break;

	}else if( (time_stamp - ts) < _time_cut){

	  store = true;

	  break;

	}
      }

    return store;
  }

  bool led_pulse_selector::finalize() {

    // This function is called at the end of event loop.
    // Do all variable finalization you wish to do here.
    // If you need, you can store your ROOT class instance in the output
    // file. You have an access to the output file through "_fout" pointer.
    //amplitude->Write();
    return true;
  }
}

#endif
