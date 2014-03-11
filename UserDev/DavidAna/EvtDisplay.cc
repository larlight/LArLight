#ifndef EVTDISPLAY_CC
#define EVTDISPLAY_CC

#include "EvtDisplay.hh"

namespace larlight {

  bool EvtDisplay::initialize() {

    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you need, you have an output root file pointer "_fout".

    //event number
    _evt_num = 0;

    return true;
  }
  
  bool EvtDisplay::analyze(storage_manager* storage) {
  
    //
    // Do your event-by-event analysis here. This function is called for 
    // each event in the loop. You have "storage" pointer which contains 
    // event-wise data. For a reference of pmt_wf_collection class instance, 
    // see the class index in the documentation.

    //up event +1
    _evt_num += 1;

    //get hits
    const event_hit *hits = (event_hit*)(storage->get_data(DATA::FFTHit));

    //make vector for time, wire, charge, channels
    std::vector<double> times_pl0;
    std::vector<double> wires_pl0;
    std::vector<double> charges_pl0;
    std::vector<double> times_pl1;
    std::vector<double> wires_pl1;
    std::vector<double> charges_pl1;
    std::vector<double> times_pl2;
    std::vector<double> wires_pl2;
    std::vector<double> charges_pl2;
    std::vector<int>    channels;

    //set min & max variables
    double min_wire_pl0 = 10000;
    double min_wire_pl1 = 10000;
    double min_wire_pl2 = 10000;
    double max_wire_pl0 =     0;
    double max_wire_pl1 =     0;
    double max_wire_pl2 =     0;
    double min_time_pl0 = 10000;
    double min_time_pl1 = 10000;
    double min_time_pl2 = 10000;
    double max_time_pl0 =     0;
    double max_time_pl1 =     0;
    double max_time_pl2 =     0;

    //planes:
    //temporary..figure out better way
    //0 --> ch < 2000
    //1 --> ch < 4000
    //2 --> ch < 6000

    //choose plane!

   
    //loop over hits
    for (size_t i=0; i<hits->size(); i++){

      const hit *this_hit = (&(hits->at(i)));

      std::cout << this_hit->Channel() << std::endl;
      

      channels.push_back    (  this_hit->Channel() );

      //determine boundaries
      //AND fill vectors

      if (this_hit->Channel() < 2000){
	charges_pl0.push_back     (  this_hit->Charge()  );
	wires_pl0.push_back       (  this_hit->Wire()    );
	times_pl0.push_back       (  this_hit->PeakTime());	
	if ( this_hit->Wire() < min_wire_pl0 )
	  min_wire_pl0 = this_hit->Wire();
	if ( this_hit->Wire() > max_wire_pl0 )
	  max_wire_pl0 = this_hit->Wire();
	if (this_hit->PeakTime() < min_time_pl0 )
	  min_time_pl0 = this_hit->PeakTime();
	if (this_hit->PeakTime() > max_time_pl0 )
	  max_time_pl0 = this_hit->PeakTime();
      }
      else if (this_hit->Channel() < 4000){
	charges_pl1.push_back     (  this_hit->Charge()  );
	wires_pl1.push_back       (  this_hit->Wire()    );
	times_pl1.push_back       (  this_hit->PeakTime());	
	if ( this_hit->Wire() < min_wire_pl1 )
	  min_wire_pl1 = this_hit->Wire();
	if ( this_hit->Wire() > max_wire_pl1 )
	  max_wire_pl1 = this_hit->Wire();
	if (this_hit->PeakTime() < min_time_pl1 )
	  min_time_pl1 = this_hit->PeakTime();
	if (this_hit->PeakTime() > max_time_pl1 )
	  max_time_pl1 = this_hit->PeakTime();
      }
      else if (this_hit->Channel() < 6000){
	charges_pl2.push_back     (  this_hit->Charge()  );
	wires_pl2.push_back       (  this_hit->Wire()    );
	times_pl2.push_back       (  this_hit->PeakTime());	
	if ( this_hit->Wire() < min_wire_pl2 )
	  min_wire_pl2 = this_hit->Wire();
	if ( this_hit->Wire() > max_wire_pl2 )
	  max_wire_pl2 = this_hit->Wire();
	if (this_hit->PeakTime() < min_time_pl2 )
	  min_time_pl2 = this_hit->PeakTime();
	if (this_hit->PeakTime() > max_time_pl2 )
	  max_time_pl2 = this_hit->PeakTime();
      }
    }


    std::cout << "min time pl0: " << min_time_pl0 << std::endl;
    std::cout << "max time pl0: " << max_time_pl0 << std::endl;

    
    //sizes
    int size_pl0 = max_time_pl0-min_time_pl0;
    int size_pl1 = max_time_pl1-min_time_pl1;
    int size_pl2 = max_time_pl2-min_time_pl2;

    //name histograms
    char h0[25];
    char h1[25];
    char h2[25];
    sprintf(h0,"evt%d_plane_0",_evt_num);
    sprintf(h1,"evt%d_plane_1",_evt_num);
    sprintf(h2,"evt%d_plane_2",_evt_num);

    //create histogram 2D
    TH2D *evt_pl0 = new TH2D (h0, "Event Display", size_pl0, min_time_pl0, max_time_pl0, size_pl0, min_wire_pl0, max_wire_pl0);
    TH2D *evt_pl1 = new TH2D (h1, "Event Display", size_pl1, min_time_pl1, max_time_pl1, size_pl1, min_wire_pl1, max_wire_pl1);
    TH2D *evt_pl2 = new TH2D (h2, "Event Display", size_pl2, min_time_pl2, max_time_pl2, size_pl2, min_wire_pl2, max_wire_pl2);

    std::cout << "SIZE PL 0: " << size_pl0 << std::endl;

    //Fill histograms
    for (int j=0; j<times_pl0.size(); j++)
      evt_pl0->SetBinContent(  times_pl0.at(j),  wires_pl0.at(j),  charges_pl0.at(j)  );
    for (int j=0; j<times_pl1.size(); j++)
      evt_pl1->SetBinContent(  times_pl1.at(j),  wires_pl1.at(j),  charges_pl1.at(j)  );
    for (int j=0; j<times_pl2.size(); j++)
      evt_pl2->SetBinContent(  times_pl2.at(j),  wires_pl2.at(j),  charges_pl2.at(j)  );
      
    //Write histograms
    evt_pl0->Write("COLZ");
    evt_pl1->Write("COLZ");
    evt_pl2->Write("COLZ");


    return true;
  }

  bool EvtDisplay::finalize() {
  
    // This function is called at the end of event loop.
    // Do all variable finalization you wish to do here.
    // If you need, you can store your ROOT class instance in the output
    // file. You have an access to the output file through "_fout" pointer.
  
    return true;
  }
}
#endif
