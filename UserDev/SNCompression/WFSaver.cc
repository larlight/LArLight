#ifndef WFSAVER_CC
#define WFSAVER_CC

#include "WFSaver.hh"

namespace larlight {


  //********************************
  WFSaver::WFSaver(): ana_base()
  {
    //Class Name
    _name = "WFSaver";

    _w2cm = larutil::GeometryUtilities::GetME()->WireToCm();
    _t2cm = larutil::GeometryUtilities::GetME()->TimeToCm();
    
    _useCmCm = true;

    _f.open("out.csv");
    
    _evtNum = 0;
    
  }


  //**********************************************
  bool WFSaver::analyze(storage_manager* storage)
  //**********************************************
  {

    //read waveforms from event
    larlight::event_tpcfifo *event_wf = (event_tpcfifo*)(storage->get_data(DATA::TPCFIFO));
    //make sure not empty...if so report
    if(!event_wf) {
      print(MSG::ERROR,__FUNCTION__,"Data storage did not find associated waveforms!");
      return false;
    }

    int maxmax=0;
    size_t maxT=0;
    unsigned int maxW=0;
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
      

      //get WF plane to determine baseline
      UInt_t chan = tpc_data->channel_number();		
      int time = tpc_data->readout_sample_number_RAW()+1;
      //determine baseline based on plane-type
      if ( larutil::Geometry::GetME()->SignalType(chan) == larlight::GEO::kCollection )
	_baseline = 400;
      else if ( larutil::Geometry::GetME()->SignalType(chan) == larlight::GEO::kInduction )
	_baseline = 2048;

      if ( larlight::GEO::kV == tpc_data->plane() ){
	UInt_t wirenum = larutil::Geometry::GetME()->ChannelToWire(chan);
	if (wirenum > 2000) continue;
	std::cout << "wirenum: " << wirenum << std::endl;
	for (int nn=0; nn<1; nn++){
	  for (size_t y=6000; y < 8000; y++){
	    //if (tpc_data->at(y) <= _baseline) continue;
	    unsigned short tick = tpc_data->at(y);
	    int tickval = 0;
	    if (tick-2010 > 0)
	      tickval = (int)(128*log((tick-2010.))/7.);
	    //uint8_t tick8 = (uint8_t)tickval;
	    
	    if (tickval > maxmax){
	      maxmax = tickval;
	      maxT = y;
	      maxW = wirenum;
	    }

	    _f << tickval << ",";//  << tickval << ","  << tickval << ","  << tickval << ",";
	  }
	  _f << "\n";
	}//4x4
      }


    }//loop over all waveforms
    std::cout << "Max val: " << maxmax << "\tmax tick: " << maxT << "\tmax W: " << maxW << std::endl;
    _f.close();

    _evtNum += 1;
    
    return true;
  }
}
#endif
