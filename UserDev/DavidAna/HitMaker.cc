#ifndef HITMAKER_CC
#define HITMAKER_CC

#include "HitMaker.hh"

//Author:  David Caratelli
//Contact: dcaratelli@nevis.columbia.edu
//Date:    Feb 2014
//*********************************************************************
//This module takes waveforms and very simply transforms them into hits.
//Charge is counted up in terms of ADCs, not electrons.
//Various planes determined poorly since tpcfifo data does not store
//that information.
//Not all hit info is recorded .Only important part for me now is:
//time, charge, wire number.
//Doing this to be able to use HitViewer 
//*********************************************************************

namespace larlight {

  bool HitMaker::initialize() {

    //collection plane baseline
    _baseline = 400;
    _event_num = 0;
    return true;
  }
  
  bool HitMaker::analyze(storage_manager* storage) {

    _event_num += 1;
    std::cout << "Event Number: " << _event_num << std::endl;


    if (_event_num < 5 ){
      larlight::GEO::View_t plane0 = larlight::GEO::kU;
      larlight::GEO::View_t plane1 = larlight::GEO::kV;
      larlight::GEO::View_t plane2 = larlight::GEO::kZ;
      //make vector to hold new hits
      //storage->create_data_ptr(DATA::FFTHit);
      larlight::event_hit *hits = (event_hit*)(storage->get_data(DATA::FFTHit));
      hits->clear();
      hits->set_event_id(_event_num);
      int adcs_over = 0;
      //make sure not empty...if so report
      //if(!hits) {
      // print(MSG::ERROR,__FUNCTION__,"Data storage did not find associated hits!");
      // return false;
      //}
      
      //read waveforms from event
      larlight::event_tpcfifo *event_wf = (event_tpcfifo*)(storage->get_data(DATA::TPCFIFO));
      //make sure not empty...if so report
      if(!event_wf) {
	print(MSG::ERROR,__FUNCTION__,"Data storage did not find associated waveforms!");
	return false;
      }
      
      //Loop over all waveforms
      for (size_t i=0; i<event_wf->size(); i++){
	
	//charge on waveform
	Double_t pulse_ADCs = 0;
	Double_t peak_time  = 0;
	//get tpc_data
	larlight::tpcfifo* tpc_data = (&(event_wf->at(i)));      

	//Check for empty waveforms!
	if(tpc_data->size()<1){
	  Message::send(MSG::ERROR,__FUNCTION__,
			Form("Found 0-length waveform: Event %d ... Ch. %d",event_wf->event_number(),tpc_data->channel_number()));
	  continue;
	}
	UInt_t chan = tpc_data->channel_number();		
	//determine if collection plane
	if ( larutil::Geometry::GetME()->SignalType(chan) == larlight::GEO::kCollection ){
	    //make new (empty hit)
	    larlight::hit thishit0;
	    larlight::hit thishit1;
	    larlight::hit thishit2;
	    //loop over samples
	    for (UShort_t adc_index=0; adc_index<tpc_data->size(); adc_index++){
		int adcs = tpc_data->at(adc_index);
		if ( ((adcs-_baseline) >= 10) ) {
		  pulse_ADCs   += (adcs-_baseline);
		  adcs_over += 1;
		}
		//find if pulse peak
		if ( pulse_ADCs > peak_time )
		  peak_time = tpc_data->readout_sample_number_RAW()+adc_index;
	      }//for all bins in this waveform
	    
	    //set hit properties
	    //	  std::cout << "charge: " << pulse_ADCs << std::endl;
	    thishit0.set_view(plane0);
	    thishit0.set_charge(pulse_ADCs, pulse_ADCs);
	    thishit0.set_times(peak_time, peak_time, peak_time);
	    thishit0.set_channel(chan);
	    thishit0.set_wire(larutil::Geometry::GetME()->ChannelToWire(chan));
	    thishit0.set_times_err(0.0,0.0,0.0);
	    thishit0.set_charge_err(0.0,0.0);
	    thishit0.set_multiplicity(0);
	    thishit0.set_fit_goodness(1);
	    //add hit to hit vector
	    hits->push_back(thishit0);
	    //
	    thishit1.set_view(plane1);
	    thishit1.set_charge(pulse_ADCs, pulse_ADCs);
	    thishit1.set_times(peak_time, peak_time, peak_time);
	    thishit1.set_channel(tpc_data->channel_number());
	    thishit1.set_wire(tpc_data->channel_number());
	    thishit1.set_times_err(0.0,0.0,0.0);
	    thishit1.set_charge_err(0.0,0.0);
	    thishit1.set_multiplicity(0);
	    thishit1.set_fit_goodness(1);
	    //add hit to hit vector
	    hits->push_back(thishit1);
	    //
	    thishit2.set_view(plane2);
	    thishit2.set_charge(pulse_ADCs, pulse_ADCs);
	    thishit2.set_times(peak_time, peak_time, peak_time);
	    thishit2.set_channel(tpc_data->channel_number());
	    thishit2.set_wire(tpc_data->channel_number());
	    thishit2.set_times_err(0.0,0.0,0.0);
	    thishit2.set_charge_err(0.0,0.0);
	    thishit2.set_multiplicity(0);
	    thishit2.set_fit_goodness(1);
	    //add hit to hit vector
	    hits->push_back(thishit2);
	    
	  }//if collection plane
	
      }//loop over waveforms in event
      //std::cout << adcs_over << std::endl;
      std::cout << "Hits on Coll. Plane: " << (int)((hits->size())/3) << std::endl;
    
      //eliminate waveform
      //event_wf->clear();
    }
    return true;
  }

  bool HitMaker::finalize() {

    return true;
  }
}
#endif
