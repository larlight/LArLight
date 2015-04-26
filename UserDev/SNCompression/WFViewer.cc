#ifndef WFVIEWER_CC
#define WFVIEWER_CC

#include "WFViewer.hh"

namespace larlight {


  //********************************
  WFViewer::WFViewer(): ana_base(), _hHits_U(nullptr), _hHits_V(nullptr), _hHits_Y(nullptr)
  //********************************
  {
    //Class Name
    _name = "WFViewer";

    _w2cm = larutil::GeometryUtilities::GetME()->WireToCm();
    _t2cm = larutil::GeometryUtilities::GetME()->TimeToCm();
    
    _useCmCm = true;
    
    _evtNum = 0;
    
  }

  //********************************
  bool WFViewer::initialize()
  //********************************
  {

    if (!_useCmCm){
      _w2cm = 1;
      _t2cm = 1;
    }

    return true;
  }
  

  //**********************************************
  bool WFViewer::analyze(storage_manager* storage)
  //**********************************************
  {

    //clean up histograms if they already exist (from previous event)
    if (_hHits_U) {delete _hHits_U; };  
    if (_hHits_V) {delete _hHits_V; };  
    if (_hHits_Y) {delete _hHits_Y; };  


    //if all ok, plot wire vs. time for hits
    _hHits_U = Prepare2DHisto(Form("Event %i - WF ADCs U-Plane",_evtNum),
			      0, larutil::Geometry::GetME()->Nwires(0)*_w2cm, 0, 9600*_t2cm, 0);
    _hHits_V = Prepare2DHisto(Form("Event %i - WF ADCs V-Plane",_evtNum),
			      0, larutil::Geometry::GetME()->Nwires(1)*_w2cm, 0, 9600*_t2cm, 1);
    _hHits_Y = Prepare2DHisto(Form("Event %i - WF ADCs Y-Plane",_evtNum),
			      0, larutil::Geometry::GetME()->Nwires(2)*_w2cm, 0, 9600*_t2cm, 2);

    
    //read waveforms from event
    larlight::event_tpcfifo *event_wf = (event_tpcfifo*)(storage->get_data(DATA::TPCFIFO));
    //make sure not empty...if so report
    if(!event_wf) {
      print(MSG::ERROR,__FUNCTION__,"Data storage did not find associated waveforms!");
      return false;
    }
    std::cout << "N waveforms: " << event_wf->size() << std::endl;
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
      if (chan >= 8254)
	continue;
      int time = 0;//tpc_data->readout_sample_number_RAW()+1;
      //determine baseline based on plane-type
      if ( larutil::Geometry::GetME()->SignalType(chan) == larlight::GEO::kCollection )
	_baseline = 400;
      else if ( larutil::Geometry::GetME()->SignalType(chan) == larlight::GEO::kInduction )
	_baseline = 2048;

      if ( larlight::GEO::kU == tpc_data->plane() ){
	for (size_t u=0; u < tpc_data->size(); u++){
	  std::cout << "Ticks: " << tpc_data->size() << std::endl;
	  //if (tpc_data->at(u) <= _baseline) continue;
	  _hHits_U->Fill( larutil::Geometry::GetME()->ChannelToWire(chan)*_w2cm, (time+u)*_t2cm, tpc_data->at(u) );
	}
      }
      if ( larlight::GEO::kV == tpc_data->plane() ){
	for (size_t v=0; v < tpc_data->size(); v++){
	  std::cout << "Ticks: " << tpc_data->size() << std::endl;
	  //if (tpc_data->at(v) <= _baseline) continue;
	  _hHits_V->Fill( larutil::Geometry::GetME()->ChannelToWire(chan)*_w2cm, (time+v)*_t2cm, tpc_data->at(v) );
	}
      }
      if ( larlight::GEO::kZ == tpc_data->plane() ){
	for (size_t y=0; y < tpc_data->size(); y++){
	  std::cout << "Ticks: " << tpc_data->size() << std::endl;
	  //if (tpc_data->at(y) <= _baseline) continue;
	  _hHits_Y->Fill( larutil::Geometry::GetME()->ChannelToWire(chan)*_w2cm, (time+y)*_t2cm, tpc_data->at(y) );
	}
      }

    }//loop over all waveforms

    _evtNum += 1;
    
    return true;
  }

  //****************************************************************
  TH2S* WFViewer::Prepare2DHisto(std::string name, 
				 double wiremin, double wiremax,
				 double timemin, double timemax, int pl)
  //****************************************************************
  {
    
    TH2S* h = new TH2S(name.c_str(),name.c_str(),
		 larutil::Geometry::GetME()->Nwires(pl),  wiremin, wiremax,
		 9600,  timemin, timemax);

    h->SetXTitle("Wire [cm]        ");
    h->SetYTitle("Time [cm]");
        
    return h;
  }

  bool WFViewer::finalize() {
  
    return true;
  }
}
#endif
