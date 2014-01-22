#ifndef EVB_ALGO_TRIGGER_CC
#define EVB_ALGO_TRIGGER_CC

#include "evb_algo_trigger.hh"

namespace larlight {

  bool evb_algo_trigger::initialize()
  {
    bool status = evb_algo_base::initialize();

    return status;
  }

  bool evb_algo_trigger::build(storage_manager *out_storage, UInt_t id)
  {
    
    trigger* out_trigger = (trigger*)(out_storage->get_data(_type));

    trigger* in_trigger  = (trigger*)(_storage->get_data(_type));

    // Check event counter

    if( id==DATA::INVALID_UINT ) out_trigger->set_event_id( _current_event_number );


    if( id!=DATA::INVALID_UINT && id != in_trigger->trig_number() ) {

      print(MSG::ERROR,__FUNCTION__,
	    Form("Slave Stream (data=\"%s\"): Event ID mismatch between output (%d) and input (%d)", 
		 DATA::DATA_TREE_NAME[_type].c_str(),
		 id,
		 in_trigger->trig_number()));

      return false;

    }

    // Fill data

    out_trigger->set_remainder_16MHz    ( in_trigger->remainder_16MHz()        );
    out_trigger->set_remainder_64MHz    ( in_trigger->remainder_64MHz()        );
    out_trigger->set_trig_frame_number  ( in_trigger->trig_frame_number()      );
    out_trigger->set_trig_sample_number ( in_trigger->trig_sample_number_RAW() );
    out_trigger->set_trig_number        ( in_trigger->trig_number()            );
    out_trigger->set_pmt_data           ( in_trigger->pmt_data()               );

    out_trigger->set_trig_bits( in_trigger->trig_pc(),
				in_trigger->trig_ext(),
				in_trigger->active(),
				in_trigger->gate1_in(),
				in_trigger->gate2_in(),
				in_trigger->veto_in(),
				in_trigger->calib() );
    
    return true;
  }
  
  bool evb_algo_trigger::finalize()
  {
    bool status = evb_algo_base::finalize();

    return status;
  }

}
#endif
