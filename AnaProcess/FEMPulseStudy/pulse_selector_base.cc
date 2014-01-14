#ifndef PULSE_SELECTOR_BASE_CC
#define PULSE_SELECTOR_BASE_CC

#include "pulse_selector_base.hh"

namespace larlight {

  //################################################
  pulse_selector_base::pulse_selector_base(){
  //################################################

    _name="pulse_selector_base"; 

    _fout=0; 

    _pulse_type=DATA::Pulse;

  }

  //################################################
  void pulse_selector_base::set_pulse_type(DATA::DATA_TYPE type) {
  //################################################

    if(type != DATA::Pulse &&
       type != DATA::PMTPulse_ThresWin &&
       type != DATA::PMTPulse_FixedWin &&
       type != DATA::TPCPulse_ThresWin &&
       type != DATA::TPCPulse_FixedWin )

      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("The specified data type (=%ud) does not match any pulse data type!",type)
		    );
    
    else _pulse_type = type;

  }
}

#endif
