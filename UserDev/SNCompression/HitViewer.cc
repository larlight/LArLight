#ifndef HITVIEWER_CC
#define HITVIEWER_CC

#include "HitViewer.hh"

namespace larlight {


  //********************************
  HitViewer::HitViewer(): ana_base(), _hHits_U(), _hHits_V(), _hHits_Y()
  //********************************
  {
    //Class Name
    _name = "HitViewer";
    //set initialization for pointers
    _hHits_U = 0;
    _hHits_V = 0;
    _hHits_Y = 0;

    _w2cm = larutil::GeometryUtilities::GetME()->WireToCm();
    _t2cm = larutil::GeometryUtilities::GetME()->TimeToCm();
    
  }

  //********************************
  bool HitViewer::initialize()
  //********************************
  {

    return true;
  }
  

  //**********************************************
  bool HitViewer::analyze(storage_manager* storage)
  //**********************************************
  {

    std::cout << "Analyzing..." << std::endl;

    //clean up histograms if they already exist (from previous event)
    if (_hHits_U) {delete _hHits_U; _hHits_U = 0;};  
    if (_hHits_V) {delete _hHits_V; _hHits_V = 0;};  
    if (_hHits_Y) {delete _hHits_Y; _hHits_Y = 0;};  


    //Get Hits
    const event_hit *hits = (event_hit*)(storage->get_data(DATA::FFTHit));

    std::cout << "Just got hits! " << std::endl;
    std::cout << "Number of Hits in event: " << hits->size() << std::endl;

    //Define axis ranges
    std::vector<double> chmax, chmin, wiremax, wiremin, timemax, timemin;
    //    chmax = wiremax = timemax = 999999.;
    //    chmin = wiremin = timemin = -1;
    //Find axis boundary
    hits->get_axis_range(chmax, chmin, wiremax, wiremin, timemax, timemin);
    //proceed only if values actually reset
    if ( wiremax[0] <= -1 )
      {
	print(MSG::WARNING,__FUNCTION__,
	      "Did not find any reconstructed hits in view 0. Skipping this event...");
	return true;
      }
    
    //if all ok, plot wire vs. time for hits
    _hHits_U = Prepare2DHisto("HitHistU", wiremin[0]*_w2cm, wiremax[0]*_w2cm, timemin[0]*_t2cm, timemax[0]*_t2cm);
    _hHits_V = Prepare2DHisto("HitHistV", wiremin[1]*_w2cm, wiremax[1]*_w2cm, timemin[1]*_t2cm, timemax[1]*_t2cm);
    _hHits_Y = Prepare2DHisto("HitHistZ", wiremin[2]*_w2cm, wiremax[2]*_w2cm, timemin[2]*_t2cm, timemax[2]*_t2cm);
    
    //loop over hits
    for (size_t i=0; i<hits->size(); i++)
      {
	const hit *this_hit = (&(hits->at(i)));
	//place in right plane
	if ( this_hit->View()==0 )
	  _hHits_U->Fill( this_hit->Wire()*_w2cm, this_hit->PeakTime()*_t2cm, this_hit->Charge() );
	if ( this_hit->View()==1 )
	  _hHits_V->Fill( this_hit->Wire()*_w2cm, this_hit->PeakTime()*_t2cm, this_hit->Charge() );
	if ( this_hit->View()==2 )
	  _hHits_Y->Fill( this_hit->Wire()*_w2cm, this_hit->PeakTime()*_t2cm, this_hit->Charge() );
	
      }//end loop over hits
    
    
    return true;
  }

  //****************************************************************
  TH2D* HitViewer::Prepare2DHisto(std::string name, 
				      double wiremin, double wiremax,
				      double timemin, double timemax)
  //****************************************************************
  {
    
    TH2D* h=0;
    if(h) delete h;
    
    int wirewidth = 50;//(int)(mywiremax-mywiremin)/12;
    int timewidth = 50;//(int)(mytimemax-mytimemin)/12; 

    h = new TH2D(name.c_str(),"2D Viewer; Wire  [cm]; Time [cm];",
		 100,  wiremin-10, wiremax+10,
		 100,  timemin-10, timemax+10);
    
    return h;
  }

  bool HitViewer::finalize() {
  
    return true;
  }
}
#endif
