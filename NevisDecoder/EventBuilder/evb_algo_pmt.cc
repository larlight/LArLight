#ifndef EVB_ALGO_PMT_CC
#define EVB_ALGO_PMT_CC

#include "evb_algo_pmt.hh"

namespace larlight {

  bool evb_algo_pmt::initialize()
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
    out_event_pmtfifo->set_module_address     ( in_event_pmtfifo->module_address()     );
    out_event_pmtfifo->set_module_id          ( in_event_pmtfifo->module_id()          );

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

    if(_analyze) {
      //
      // Fill Analysis Data
      //
      // Record 1st event number if not yet
      if(_first_event_number == DATA::INVALID_UINT)
	
	_first_event_number = in_event_pmtfifo->event_number();
      
      // For 1st event entries, record # of FEMs
      if(_first_event_number == in_event_pmtfifo->event_number())
	
	_num_fem++;
      
      // For new event, create event vector entries
      if(_last_event_number != in_event_pmtfifo->event_number()) {
	
	_fem_trig_frame.push_back(std::vector<UInt_t>());
	_fem_event_frame.push_back(std::vector<UInt_t>());
	_fem_trig_sample.push_back(std::vector<UInt_t>());
	
	// If not 1st event, do check of last event & reserve of FEM entries
	if(_first_event_number != in_event_pmtfifo->event_number()) {
	  
	  // Check previous event
	  if(_num_fem != _fem_trig_frame.at(_fem_trig_frame.size()-2).size())
	    
	    throw evb_exception(Form("Previous event (%d) has different # of FEMs (%zu) than other events (%d)",
				     _last_event_number,
				     _fem_trig_frame.at(_fem_trig_frame.size()-2).size(),
				     _num_fem)
				);
	  
	  (*_fem_trig_frame.rbegin()).reserve(_num_fem);
	  (*_fem_event_frame.rbegin()).reserve(_num_fem);
	  (*_fem_trig_sample.rbegin()).reserve(_num_fem);
	}
      }
      
      (*_fem_trig_frame.rbegin()).push_back(in_event_pmtfifo->fem_trig_frame_number());
      (*_fem_event_frame.rbegin()).push_back(in_event_pmtfifo->event_frame_number());
      (*_fem_trig_sample.rbegin()).push_back(in_event_pmtfifo->fem_trig_sample_number_RAW());
      
      _last_event_number = in_event_pmtfifo->event_number();
    }
    
    return true;
  }
  
  bool evb_algo_pmt::finalize()
  {
    bool status = evb_algo_base::finalize();

    if(_analyze) {

      if(_fout)
	_fout->cd();

      TGraph* gTrigFramePMT  = new TGraph(_fem_trig_frame.size()  * _num_fem);
      TGraph* gEventFramePMT = new TGraph(_fem_event_frame.size() * _num_fem);
      TGraph* gTrigSamplePMT = new TGraph(_fem_trig_sample.size() * _num_fem);
      
      UInt_t ctr = 0;
      
      TH1D* hTrigFramePMT  = new TH1D("hTrigFramePMT","",99,-49.5,49.5);
      TH1D* hEventFramePMT = new TH1D("hEventFramePMT","",99,-49.5,49.5);
      TH1D* hTrigSamplePMT = new TH1D("hTrigSamplePMT","",99,-49.5,49.5);
      
      for(size_t i=0; i<_fem_trig_frame.size(); ++i) {
	
	for(size_t j=1; j<_fem_trig_frame.at(i).size(); ++j) {
	  /*
	  gTrigFramePMT->SetPoint(ctr,i,_fem_trig_frame.at(i).at(j));
	  gTrigSamplePMT->SetPoint(ctr,i,_fem_trig_sample.at(i).at(j));
	  gEventFramePMT->SetPoint(ctr,i,_fem_event_frame.at(i).at(j));
	  */

	  gTrigFramePMT->SetPoint  ( ctr, i, _fem_trig_frame.at(i).at(j)  - _fem_trig_frame.at(i).at(0)  );
	  gTrigSamplePMT->SetPoint ( ctr, i, _fem_trig_sample.at(i).at(j) - _fem_trig_sample.at(i).at(0) );
	  gEventFramePMT->SetPoint ( ctr, i, _fem_event_frame.at(i).at(j) - _fem_event_frame.at(i).at(0) );

	  hTrigFramePMT->Fill  ( (double)(_fem_trig_frame.at(i).at(j) ) - (double)(_fem_trig_frame.at(i).at(0) ) );
	  hEventFramePMT->Fill ( (double)(_fem_event_frame.at(i).at(j)) - (double)(_fem_event_frame.at(i).at(0)) );
	  hTrigSamplePMT->Fill ( (double)(_fem_trig_sample.at(i).at(j)) - (double)(_fem_trig_sample.at(i).at(0)) );

	  ctr++;

	}
      }

      std::cout
	<< std::endl
	<< "---- PMT Event Builder Algorithm ----"
	<< std::endl
	<< Form(" - # of FEM modules: %d",_num_fem) << std::endl
	<< Form(" - Mean diff of FEM-trigger frame  : %g +/- %g",hTrigFramePMT->GetMean(),hTrigFramePMT->GetRMS())<<std::endl
	<< Form(" - Mean diff of FEM-trigger frame  : %g +/- %g",hEventFramePMT->GetMean(),hEventFramePMT->GetRMS())<<std::endl
	<< Form(" - Mean diff of FEM-trigger sample : %g +/- %g",hTrigSamplePMT->GetMean(),hTrigSamplePMT->GetRMS())<<std::endl
	<< std::endl;

      delete hTrigFramePMT;
      delete hEventFramePMT;
      delete hTrigSamplePMT;
      
      if(_fout) {
	gTrigFramePMT->SetName("gTrigFramePMT");
	gTrigFramePMT->SetMarkerStyle(20);
	gTrigFramePMT->SetMarkerSize(1);
	gTrigFramePMT->Write();
	
	gEventFramePMT->SetName("gEventFramePMT");
	gEventFramePMT->SetMarkerStyle(20);
	gEventFramePMT->SetMarkerSize(1);
	gEventFramePMT->Write();
	
	gTrigSamplePMT->SetName("gTrigSamplePMT");
	gTrigSamplePMT->SetMarkerStyle(20);
	gTrigSamplePMT->SetMarkerSize(1);
	gTrigSamplePMT->Write();
      }
    }

    return status;
  }

}
#endif
