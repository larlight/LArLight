#ifndef EVB_ALGO_PMT_CC
#define EVB_ALGO_PMT_CC

#include "evb_algo_pmt.hh"

namespace larlight {

  bool evb_algo_pmt::initialize()
  {
    bool status = evb_algo_base::initialize();

    return status;
  }

  bool evb_algo_pmt::build(storage_manager *out_storage, UInt_t id)
  {
    print(MSG::DEBUG,__FUNCTION__,Form("Building %d %d...",id,_current_event_number));
    event_pmtfifo* out_event_pmtfifo = (event_pmtfifo*)(out_storage->get_data(_type));

    event_pmtfifo* in_event_pmtfifo  = (event_pmtfifo*)(_storage->get_data(_type));

    // Check event counter

    if( id==DATA::INVALID_UINT ) out_event_pmtfifo->set_event_id( _current_event_number );
    
    if( id!=DATA::INVALID_UINT && id != in_event_pmtfifo->event_number() ) {

      print(MSG::ERROR,__FUNCTION__,
	    Form("Slave Stream (data=\"%s\"): Event ID mismatch between output (%d) and input (%d)", 
		 DATA::DATA_TREE_NAME[_type].c_str(),
		 id,
		 in_event_pmtfifo->event_number()));

      return false;

    }

    // Fill data

    out_event_pmtfifo->set_event_number       ( in_event_pmtfifo->event_number()       );
    out_event_pmtfifo->set_module_address     ( DATA::INVALID_UINT                  );
    out_event_pmtfifo->set_module_id          ( DATA::INVALID_UINT                  );

    if(out_event_pmtfifo->checksum() == FEM::INVALID_WORD) out_event_pmtfifo->set_checksum(0);
    if(out_event_pmtfifo->nwords()   == FEM::INVALID_WORD) out_event_pmtfifo->set_nwords(0);
    
    out_event_pmtfifo->set_checksum           ( out_event_pmtfifo->checksum() + in_event_pmtfifo->checksum() );
    out_event_pmtfifo->set_nwords             ( out_event_pmtfifo->nwords()   + in_event_pmtfifo->nwords()   );

    if(out_event_pmtfifo->event_frame_number() == FEM::INVALID_WORD)

      out_event_pmtfifo->set_event_frame_number ( in_event_pmtfifo->event_frame_number() );

    if(out_event_pmtfifo->fem_trig_frame_number() == FEM::INVALID_WORD)

      out_event_pmtfifo->set_fem_trig_frame_number ( in_event_pmtfifo->fem_trig_frame_number() );

    if(out_event_pmtfifo->fem_trig_sample_number_RAW() == FEM::INVALID_WORD)

      out_event_pmtfifo->set_fem_trig_sample_number ( in_event_pmtfifo->fem_trig_sample_number_RAW() );

    for(auto in_fifo : *in_event_pmtfifo) {

      out_event_pmtfifo->push_back(in_fifo);

    }
    
    return true;
  }
  
  bool evb_algo_pmt::finalize()
  {
    bool status = evb_algo_base::finalize();

    return status;
  }

}
#endif
