#ifndef CHARGEANA_CC
#define CHARGEANA_CC

#include "ChargeAna.hh"

namespace larlight {

  bool ChargeAna::initialize() {

    //Define Variables
    _event_num = 0;
    _coll_baseline = 400;
    _hitthreshold  = 7; //ADC counts
    //Define Histograms
    UInt_t CollWires = larutil::Geometry::GetME()->Nwires(2); // Collection is Plane "2"
    TotCharge      = new TH1D("TotCharge",      "Tot Charge in Evt.; Charge [ADCs]",             100,    10000,       10000);
    ChargePerWire  = new TH1D("ChargePerWire",  "Charge on Wire; Wire num; Charge [ADCs]", CollWires,        0,   CollWires);
    ChargePerPulse = new TH1D("ChargePerPulse", "Charge on Pulse; Charge[ADCs]",                  30,        0,        1000); 
    LowEnPulses    = new TH1D("LowEnPulses",    "Charge on Pulse; Charge[ADCs]",                  15,        0,         100); 
    NumHits        = new TH1D("NumHits",        "Number of Hits for Event",                       20,        0,         100);

    return true;
  }
  
  bool ChargeAna::analyze(storage_manager* storage) {

    _event_num += 1;

    //Total Charge on this event
    int tot_ADCs    = 0;
    //Total Charge on individual wire
    int wire_ADCs   = 0;
    int pulse_ADCs  = 0;
    //Total number of hits
    int num_hits    = 0;
    
    const event_tpcfifo *event_wf = (event_tpcfifo*)(storage->get_data(DATA::TPCFIFO));
    
    //waveform counter
    int wfnum = 0;
    //time counted
    int time_counted = 0;

    //Loop over all waveforms
    for (size_t i=0; i<event_wf->size(); i++){
      
      wfnum += 1;
      
      //Check for empty waveforms!
      const tpcfifo* tpc_data = (&(event_wf->at(i)));
      if(tpc_data->size()<1){
	Message::send(MSG::ERROR,__FUNCTION__,
		      Form("Found 0-length waveform: Event %d ... Ch. %d",event_wf->event_number(),tpc_data->channel_number()));
	continue;
      }
      UInt_t chan    = tpc_data->channel_number();
      int wf_time    = tpc_data->readout_sample_number_RAW();
      int chargeADCs = 0;

      //reset charge counted if new channel number (new wire)
      if ( i > 0 ){
	if ( (event_wf->at(i)).channel_number() != (event_wf->at(i-1)).channel_number() ){
	  time_counted = 0;
	}
      }
      //determine if collection plane
      if ( larutil::Geometry::GetME()->SignalType(chan) == larlight::GEO::kCollection )
	{

	  bool found_pulse = false;

	  for (unsigned int adc_index=0; adc_index<tpc_data->size(); adc_index++)
	    {
	      short adcs = tpc_data->at(adc_index);

	      //find pulse and count ADCs
	      //for now if 2 ADCs above baseline then we found a pulse
	      //AND if not double counted (by looking at time)
	      if ( ((adcs-_coll_baseline) >= _hitthreshold ) )
		{
		  if ( adc_index+wf_time > time_counted )
		    {
		      //update time that has been counted
		      time_counted = adc_index+wf_time;
		      chargeADCs   = (int)(adcs-_coll_baseline);
		      found_pulse  = true;
		      tot_ADCs    += chargeADCs;
		      wire_ADCs   += chargeADCs;
		      pulse_ADCs  += chargeADCs;
		    }
		}
	      else //went from "pulse" to "no pulse" ->  write new pulse info
		{
		  if (found_pulse) {
		    //		    std::cout << "Just finished finding a new hit! Charge on Hit: " << pulse_ADCs << std::endl;
		    ChargePerPulse->Fill((double)pulse_ADCs);
		    //		    std::cout << "Added value to histogram..." << std::endl;
		    if ( pulse_ADCs < 100 ) { LowEnPulses->Fill(pulse_ADCs); }
		    num_hits+=1;
		    pulse_ADCs = 0;
		  }
		  found_pulse = false;
		}

	    }//for all bins in this waveform
	  ChargePerWire->AddBinContent( larutil::Geometry::GetME()->ChannelToWire(chan), wire_ADCs);
	  wire_ADCs = 0;

	}//if collection plane

    }//loop over all waveforms in event      

    //    std::cout << "Num Hits: " << num_hits << std::endl;
    NumHits->Fill(num_hits);
    num_hits = 0;

    TotCharge->Fill(tot_ADCs);    
    tot_ADCs = 0;

    return true;
  }

  bool ChargeAna::finalize() {

    NumHits->Write();
    TotCharge->Write();
    ChargePerPulse->Write();
    LowEnPulses->Write();
    ChargePerWire->Write();
    return true;
  }
}
#endif
