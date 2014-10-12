#ifndef WFVIEWER_CC
#define WFVIEWER_CC

#include "WFViewer.hh"

namespace larlight {


  //********************************
  WFViewer::WFViewer(): ana_base(), _hHits_U(), _hHits_V(), _hHits_Y()
  //********************************
  {
    //Class Name
    _name = "WFViewer";
    //set initialization for pointers
    _hHits_U = 0;
    _hHits_V = 0;
    _hHits_Y = 0;

    _w2cm = larutil::GeometryUtilities::GetME()->WireToCm();
    _t2cm = larutil::GeometryUtilities::GetME()->TimeToCm();
    
  }

  //********************************
  bool WFViewer::initialize()
  //********************************
  {

    return true;
  }
  

  //**********************************************
  bool WFViewer::analyze(storage_manager* storage)
  //**********************************************
  {

    //clean up histograms if they already exist (from previous event)
    if (_hHits_U) {delete _hHits_U; _hHits_U = 0;};  
    if (_hHits_V) {delete _hHits_V; _hHits_V = 0;};  
    if (_hHits_Y) {delete _hHits_Y; _hHits_Y = 0;};  


    //if all ok, plot wire vs. time for hits
    _hHits_U = Prepare2DHisto("WF ADCs U-Plane", 0, larutil::Geometry::GetME()->Nwires(0)*_w2cm, 0, 3200*_t2cm);
    _hHits_V = Prepare2DHisto("WF ADCs V-Plane", 0, larutil::Geometry::GetME()->Nwires(1)*_w2cm, 0, 3200*_t2cm);
    _hHits_Y = Prepare2DHisto("WF ADCs Y-Plane", 0, larutil::Geometry::GetME()->Nwires(2)*_w2cm, 0, 3200*_t2cm);

    
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
      

      //get WF plane to determine baseline
      UInt_t chan = tpc_data->channel_number();		
      int time = tpc_data->readout_sample_number_RAW()+1;
      //determine baseline based on plane-type
      if ( larutil::Geometry::GetME()->SignalType(chan) == larlight::GEO::kCollection )
	_baseline = 400;
      else if ( larutil::Geometry::GetME()->SignalType(chan) == larlight::GEO::kInduction )
	_baseline = 2048;

      if ( larlight::GEO::kU == tpc_data->plane() ){
	for (size_t u=0; u < tpc_data->size(); u++)
	  _hHits_U->Fill( larutil::Geometry::GetME()->ChannelToWire(chan)*_w2cm, (time+u)*_t2cm, tpc_data->at(u)-_baseline );
      }
      if ( larlight::GEO::kV == tpc_data->plane() ){
	for (size_t v=0; v < tpc_data->size(); v++)
	  _hHits_V->Fill( larutil::Geometry::GetME()->ChannelToWire(chan)*_w2cm, (time+v)*_t2cm, tpc_data->at(v)-_baseline );
      }
      if ( larlight::GEO::kZ == tpc_data->plane() ){
	for (size_t y=0; y < tpc_data->size(); y++)
	  _hHits_Y->Fill( larutil::Geometry::GetME()->ChannelToWire(chan)*_w2cm, (time+y)*_t2cm, tpc_data->at(y)-_baseline );
      }

    }//loop over all waveforms
    
    return true;
  }

  //****************************************************************
  TH2I* WFViewer::Prepare2DHisto(std::string name, 
				      double wiremin, double wiremax,
				      double timemin, double timemax)
  //****************************************************************
  {
    
    TH2I* h=0;
    if(h) delete h;
    
    int wirewidth = 50;//(int)(mywiremax-mywiremin)/12;
    int timewidth = 50;//(int)(mytimemax-mytimemin)/12; 

    h = new TH2I(name.c_str(),"2D Viewer; Wire  [cm]; Time [cm];",
		 wiremax-wiremin,  wiremin, wiremax,
		 timemax-timemin,  timemin, timemax);
    
    return h;
  }

  bool WFViewer::finalize() {
  
    return true;
  }
}
#endif
