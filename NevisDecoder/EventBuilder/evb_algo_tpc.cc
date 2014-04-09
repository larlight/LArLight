#ifndef EVB_ALGO_TPC_CC
#define EVB_ALGO_TPC_CC

#include "evb_algo_tpc.hh"

namespace larlight {

  bool evb_algo_tpc::initialize()
  {
    bool status = evb_algo_base::initialize();

    return status;
  }

  bool evb_algo_tpc::build(storage_manager *out_storage, UInt_t id)
  {
    
    event_tpcfifo* out_event_tpcfifo = (event_tpcfifo*)(out_storage->get_data(_type));

    event_tpcfifo* in_event_tpcfifo  = (event_tpcfifo*)(_storage->get_data(_type));

    // Check event counter

    if( id==DATA::INVALID_UINT ) out_event_tpcfifo->set_event_id( _current_event_number );

    if( id!=DATA::INVALID_UINT && id != in_event_tpcfifo->event_number() ) {

      print(MSG::ERROR,__FUNCTION__,
	    Form("Slave Stream (data=\"%s\"): Event ID mismatch between output (%d) and input (%d)", 
		 DATA::DATA_TREE_NAME[_type].c_str(),
		 id,
		 in_event_tpcfifo->event_number()));

      return false;

    }

    // Fill data

    out_event_tpcfifo->set_event_number       ( in_event_tpcfifo->event_number()       );
    out_event_tpcfifo->set_module_address     ( in_event_tpcfifo->module_address()     );
    out_event_tpcfifo->set_module_id          ( in_event_tpcfifo->module_id()          );

    if(out_event_tpcfifo->checksum() == FEM::INVALID_WORD) out_event_tpcfifo->set_checksum(0);
    if(out_event_tpcfifo->nwords()   == FEM::INVALID_WORD) out_event_tpcfifo->set_nwords(0);
    
    out_event_tpcfifo->set_checksum           ( out_event_tpcfifo->checksum() + in_event_tpcfifo->checksum() );
    out_event_tpcfifo->set_nwords             ( out_event_tpcfifo->nwords()   + in_event_tpcfifo->nwords()   );

    if(out_event_tpcfifo->event_frame_number() == FEM::INVALID_WORD)

      out_event_tpcfifo->set_event_frame_number ( in_event_tpcfifo->event_frame_number() );

    if(out_event_tpcfifo->fem_trig_frame_number() == FEM::INVALID_WORD)

      out_event_tpcfifo->set_fem_trig_frame_number ( in_event_tpcfifo->fem_trig_frame_number() );

    if(out_event_tpcfifo->fem_trig_sample_number_RAW() == FEM::INVALID_WORD)

      out_event_tpcfifo->set_fem_trig_sample_number ( in_event_tpcfifo->fem_trig_sample_number_RAW() );

    for(auto in_fifo : *in_event_tpcfifo) {

      out_event_tpcfifo->push_back(in_fifo);

    }
    
    return true;
  }
  
  bool evb_algo_tpc::finalize()
  {
    bool status = evb_algo_base::finalize();

    return status;
  }

}
#endif
