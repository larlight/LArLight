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

    _threshold = 6; //ADCs
    _event_num = 0;
    return true;
  }
  
  bool HitMaker::analyze(storage_manager* storage) {

    _event_num += 1;
    std::cout << "Event Number: " << _event_num << std::endl;

    //make vector to hold new hits
    larlight::event_hit *hits = (event_hit*)(storage->get_data(DATA::FFTHit));
    hits->clear();
    hits->set_event_id(_event_num);
    
    //read waveforms from event
    larlight::event_tpcfifo *event_wf = (event_tpcfifo*)(storage->get_data(DATA::TPCFIFO));
    //make sure not empty...if so report
    if(!event_wf) {
      print(MSG::ERROR,__FUNCTION__,"Data storage did not find associated waveforms!");
      return false;
    }
    
    //Loop over all waveforms
    for (size_t i=0; i<event_wf->size(); i++){
      
      //get tpc_data
      larlight::tpcfifo* tpc_data = (&(event_wf->at(i)));      

      //Check for empty waveforms!
      if(tpc_data->size()<1){
	Message::send(MSG::ERROR,__FUNCTION__,
		      Form("Found 0-length waveform: Event %d ... Ch. %d",event_wf->event_number(),tpc_data->channel_number()));
	continue;
      }
      UInt_t chan = tpc_data->channel_number();		
      //determine baseline based on plane-type
      if ( larutil::Geometry::GetME()->SignalType(chan) == larlight::GEO::kCollection )
	_baseline = 400;
      else if ( larutil::Geometry::GetME()->SignalType(chan) == larlight::GEO::kInduction )
	_baseline = 2048;
      
      //loop over samples
      bool active = false;
      double startT = 0.;
      double endT   = 0.;
      double peakT  = 0.;
      int Qpeak  = 0;
      int Qarea  = 0;
      for (UShort_t adc_index=0; adc_index<tpc_data->size(); adc_index++){
	int adcs = tpc_data->at(adc_index);
	if ( ((adcs-_baseline) >= _threshold) ) {
	  if (!active) { startT = tpc_data->readout_sample_number_RAW()+1+adc_index; }
	  active = true;
	  Qarea   += (adcs-_baseline);
	  //find if pulse peak
	  if ( (adcs-_baseline) > Qpeak ){
	    peakT = tpc_data->readout_sample_number_RAW()+1+adc_index;
	    Qpeak = (adcs-_baseline);
	  }
	}
	else {//below threshold
	  if (active){//if we were in the middle of a hit, set end time & reset
	    endT = tpc_data->readout_sample_number_RAW()+1+adc_index;
	    //fill hit information
	    if ( Qarea > 0 ){
	      _Hit.set_view(tpc_data->plane());
	      _Hit.set_charge(Qarea, Qpeak);
	      _Hit.set_times(startT, peakT, endT);
	      _Hit.set_channel(tpc_data->channel_number());
	      _Hit.set_wire(larutil::Geometry::GetME()->ChannelToWire(chan));
	      _Hit.set_times_err(0.0,0.0,0.0);
	      _Hit.set_charge_err(0.0,0.0);
	      _Hit.set_multiplicity(0);
	      _Hit.set_fit_goodness(1);
	      //add hit to hit vector
	      hits->push_back(_Hit);
	    }//fill hit info if area > 0	    
	    //reset values
	    Qarea = 0;
	    Qpeak = 0;
	    startT = 0;
	    endT   = 0;
	    peakT  = 0;
	    active = false;
	  }//if active was ON and we were in hit
	}//if below threshold
      }//loop over bins in this WF
      
    }//loop over waveforms in event
    
    //eliminate waveform
    event_wf->clear();
    
    return true;
  }
  
  bool HitMaker::finalize() {

    return true;
  }
}
#endif
