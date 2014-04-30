#ifndef EVB_ALGO_TPC_CC
#define EVB_ALGO_TPC_CC

#include "evb_algo_tpc.hh"

namespace larlight {

  bool evb_algo_tpc::initialize()
  {
    bool status = evb_algo_base::initialize();

    _fem_trig_frame.clear();
    _fem_event_frame.clear();
    _fem_trig_sample.clear();

    _fem_trig_frame.reserve(_storage->get_entries());
    _fem_trig_sample.reserve(_storage->get_entries());
    _fem_event_frame.reserve(_storage->get_entries());

    _num_fem = 0;

    _first_event_number = DATA::INVALID_UINT;
    _last_event_number  = DATA::INVALID_UINT;
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

    // Fill Data

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

    if(_analyze) {
      //
      // Fill Analysis Data
      //
      // Record 1st event number if not yet
      if(_first_event_number == DATA::INVALID_UINT)
	
	_first_event_number = in_event_tpcfifo->event_number();
      
      // For 1st event entries, record # of FEMs
      if(_first_event_number == in_event_tpcfifo->event_number())
	
	_num_fem++;
      
      // For new event, create event vector entries
      if(_last_event_number != in_event_tpcfifo->event_number()) {
	
	_fem_trig_frame.push_back(std::vector<UInt_t>());
	_fem_event_frame.push_back(std::vector<UInt_t>());
	_fem_trig_sample.push_back(std::vector<UInt_t>());
	
	// If not 1st event,  reserve of FEM entries
	if(_first_event_number != in_event_tpcfifo->event_number()) {
	  (*_fem_trig_frame.rbegin()).reserve(_num_fem);
	  (*_fem_event_frame.rbegin()).reserve(_num_fem);
	  (*_fem_trig_sample.rbegin()).reserve(_num_fem);
	}
      }
      
      (*_fem_trig_frame.rbegin()).push_back(in_event_tpcfifo->fem_trig_frame_number());
      (*_fem_event_frame.rbegin()).push_back(in_event_tpcfifo->event_frame_number());
      (*_fem_trig_sample.rbegin()).push_back(in_event_tpcfifo->fem_trig_sample_number_RAW());
      
      _last_event_number = in_event_tpcfifo->event_number();
    }

    return true;
  }

  bool evb_algo_tpc::check_event_quality()
  {
    if((*_fem_trig_frame.rbegin()).size() != _num_fem) 

      throw evb_exception(Form("This event has different # of FEMs (%zu) than normal events (%d)",
			       (*_fem_trig_frame.rbegin()).size(),
			       _num_fem)
			  );
    return true;
  }
  
  bool evb_algo_tpc::finalize()
  {
    bool status = evb_algo_base::finalize();

    if(_analyze) {

      if(_fout)
	_fout->cd();

      TGraph* gTrigFrameTPC  = new TGraph(_fem_trig_frame.size()  * _num_fem);
      TGraph* gEventFrameTPC = new TGraph(_fem_event_frame.size() * _num_fem);
      TGraph* gTrigSampleTPC = new TGraph(_fem_trig_sample.size() * _num_fem);
      
      UInt_t ctr = 0;
      
      TH1D* hTrigFrameTPC  = new TH1D("hTrigFrameTPC","",99,-49.5,49.5);
      TH1D* hEventFrameTPC = new TH1D("hEventFrameTPC","",99,-49.5,49.5);
      TH1D* hTrigSampleTPC = new TH1D("hTrigSampleTPC","",99,-49.5,49.5);
      
      for(size_t i=0; i<_fem_trig_frame.size(); ++i) {
	
	for(size_t j=1; j<_fem_trig_frame.at(i).size(); ++j) {
	  /*
	  gTrigFrameTPC->SetPoint(ctr,i,_fem_trig_frame.at(i).at(j));
	  gTrigSampleTPC->SetPoint(ctr,i,_fem_trig_sample.at(i).at(j));
	  gEventFrameTPC->SetPoint(ctr,i,_fem_event_frame.at(i).at(j));
	  */

	  gTrigFrameTPC->SetPoint  ( ctr, i, _fem_trig_frame.at(i).at(j)  - _fem_trig_frame.at(i).at(0)  );
	  gTrigSampleTPC->SetPoint ( ctr, i, _fem_trig_sample.at(i).at(j) - _fem_trig_sample.at(i).at(0) );
	  gEventFrameTPC->SetPoint ( ctr, i, _fem_event_frame.at(i).at(j) - _fem_event_frame.at(i).at(0) );

	  hTrigFrameTPC->Fill  ( (double)(_fem_trig_frame.at(i).at(j) ) - (double)(_fem_trig_frame.at(i).at(0) ) );
	  hEventFrameTPC->Fill ( (double)(_fem_event_frame.at(i).at(j)) - (double)(_fem_event_frame.at(i).at(0)) );
	  hTrigSampleTPC->Fill ( (double)(_fem_trig_sample.at(i).at(j)) - (double)(_fem_trig_sample.at(i).at(0)) );

	  ctr++;

	}
      }

      std::cout
	<< std::endl
	<< "---- TPC Event Builder Algorithm ----"
	<< std::endl
	<< Form(" - # of FEM modules: %d",_num_fem) << std::endl
	<< Form(" - Mean diff of FEM-trigger frame  : %g +/- %g",hTrigFrameTPC->GetMean(),hTrigFrameTPC->GetRMS())<<std::endl
	<< Form(" - Mean diff of FEM-trigger frame  : %g +/- %g",hEventFrameTPC->GetMean(),hEventFrameTPC->GetRMS())<<std::endl
	<< Form(" - Mean diff of FEM-trigger sample : %g +/- %g",hTrigSampleTPC->GetMean(),hTrigSampleTPC->GetRMS())<<std::endl
	<< std::endl;

      delete hTrigFrameTPC;
      delete hEventFrameTPC;
      delete hTrigSampleTPC;
      
      if(_fout) {
	gTrigFrameTPC->SetName("gTrigFrameTPC");
	gTrigFrameTPC->SetMarkerStyle(20);
	gTrigFrameTPC->SetMarkerSize(1);
	gTrigFrameTPC->Write();
	
	gEventFrameTPC->SetName("gEventFrameTPC");
	gEventFrameTPC->SetMarkerStyle(20);
	gEventFrameTPC->SetMarkerSize(1);
	gEventFrameTPC->Write();
	
	gTrigSampleTPC->SetName("gTrigSampleTPC");
	gTrigSampleTPC->SetMarkerStyle(20);
	gTrigSampleTPC->SetMarkerSize(1);
	gTrigSampleTPC->Write();
      }
    }
    return status;
  }

}
#endif
