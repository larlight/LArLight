#ifndef MCSHOWERSTUDY_CC
#define MCSHOWERSTUDY_CC

#include "MCShowerStudy.hh"

namespace larlight {
  
  bool MCShowerStudy::initialize() {
    
    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you have a histogram to fill in the event loop, for example,
    // here is a good place to create one on the heap (i.e. "new TH1D"). 
    //

    _mcclus_tree = 0;
    PrepareTTree();

    //setting variables that i should be getting from geometry service...
    _wire_2_cm = 0.3; 
    _time_2_cm = 0.0818566;

    return true;
  }
  
  bool MCShowerStudy::analyze(storage_manager* storage) {
    
    //
    // Do your event-by-event analysis here. This function is called for 
    // each event in the loop. You have "storage" pointer which contains 
    // event-wise data. To see what is available, check the "Manual.pdf":
    //
    // http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183


    //grab the reconstructed clusters
    larlight::event_cluster* ev_cluster = (event_cluster*)storage->get_data(DATA::ShowerAngleCluster);
    if(!ev_cluster) {
      print(larlight::MSG::ERROR,__FUNCTION__,Form("Did not find specified data product, ShowerAngleCluster!"));
      return false;
    }
    
    //grab the MC showers
    larlight::event_mcshower* ev_mcshower = (event_mcshower*)storage->get_data(DATA::MCShower);
    if(!ev_mcshower) {
      print(larlight::MSG::ERROR,__FUNCTION__,Form("Did not find specified data product, MCShower!"));
      return false;
    }
    
    //clear the TTree variables before doing any calculations (or, set to non-physical values
    //to indicate that somehting has gone wrong if you find those values in your output tree)
    ClearTTreeVars();


    //***********************************************************//
    //implement your own functions w/ loops, etc seperately here!// 
    //they should calculate variables (in this scope) that you   //
    //want to save to the ttree (filled once per analyze loop)   // 
    //***********************************************************//

    //kaleko testing how well his refine-direction works
    RefineDirectionTest(ev_cluster, ev_mcshower);
    



    //fill TTree (make sure all variables going into it are defined in this scope)
    if(_mcclus_tree) _mcclus_tree->Fill();
  
  
    return true;
  } //end of analyze function
  
  bool MCShowerStudy::finalize() {
    
    // This function is called at the end of event loop.
    // Do all variable finalization you wish to do here.
    // If you need, you can store your ROOT class instance in the output
    // file. You have an access to the output file through "_fout" pointer.
    //
    // Say you made a histogram pointer h1 to store. You can do this:
    //
    // if(_fout) { _fout->cd(); h1->Write(); }
    //
    // else 
    //   print(MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
    //
  
    if(_fout) {

      _fout->cd();
      if(_mcclus_tree)
	_mcclus_tree->Write();

    }
    else 
      print(MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");

    return true;
  }


  void MCShowerStudy::PrepareTTree() {
    //define branches on your ttree here for your calculated parameters

    if(!_mcclus_tree) {
      _mcclus_tree = new TTree("mcclus_tree","");
      _mcclus_tree->Branch("refinestartend_FOM", "std::vector<double>", &_refinestartend_FOM);
    }


  }
  void MCShowerStudy::ClearTTreeVars() {
    
    _refinestartend_FOM.clear();

  }


  
  bool MCShowerStudy::RefineDirectionTest(larlight::event_cluster* ev_cluster, larlight::event_mcshower* ev_mcshower){
    
    //for multi shower events, can use bryce's code that matches reco clusters with showers.. 
    //demand that 90% of the hits in my reco shower came from one MC shower, then use that MC shower for the comparison


    //for kaleko's current study, this only works for single-shower events, and single-cluster (fully merged)
    if(ev_mcshower->size() != 1){
      print(larlight::MSG::ERROR,__FUNCTION__,Form("This event has more than one MCShower in it! In fact, it has %d! Wtf?",(int)ev_mcshower->size()));
      return false;
    }

    if(ev_cluster->size() != 3){
      print(larlight::MSG::ERROR,__FUNCTION__,Form("This event has != 3 reco clusters in it! In fact, it has %d! Wtf?",(int)ev_cluster->size()));
      return false;
    }
    
    //will have to use geometry utility for this
    const larutil::GeometryUtilities *myGeoUtil = larutil::GeometryUtilities::GetME();
    
    
    //assuming one true shower, this will hold the PxPoint of the mother start position
    //one entry for each view
    std::vector<larutil::PxPoint> mother_sw_view(3,larutil::PxPoint());
    
    //loop over MCShowers in the event (there should only be one, for now)
    for(auto i_mcshower: *ev_mcshower){
      
      //check to make sure i'm choosing the right 3 out of the 4 vector
      Double_t pos[3]={0.};
      pos[0]=i_mcshower.MotherPosition()[0];
      pos[1]=i_mcshower.MotherPosition()[1];
      pos[2]=i_mcshower.MotherPosition()[2];
      //pos[0] = 50.;  // x= 50cm
      //pos[1] = 0.;    // y= 0cm
      //pos[2] = 100.; // z= 100cm


      for(size_t i = 0; i < mother_sw_view.size(); ++i)
	mother_sw_view.at(i)=myGeoUtil->Get2DPointProjection(pos,i);



      //debugging stuff
      std::ostringstream msg;
      msg << Form("Motherposition of MCShower is (%f,%f,%f).\n",
		  i_mcshower.MotherPosition()[0],
		  i_mcshower.MotherPosition()[1],
		  i_mcshower.MotherPosition()[2]);
      
      msg << Form("This MCShower's start (wire,time) on view0 is (%f,%f)\n",
		  mother_sw_view[0].w,mother_sw_view[0].t);
      msg << Form("This MCShower's start (wire,time) on view1 is (%f,%f)\n",
		  mother_sw_view[1].w,mother_sw_view[1].t);
      msg << Form("This MCShower's start (wire,time) on view2 is (%f,%f)\n",
		  mother_sw_view[2].w,mother_sw_view[2].t);

      print(larlight::MSG::DEBUG,__FUNCTION__,msg.str());
               
    }

    
    //loop over reconstructed clusters in the event (there should only be three, for now)
    //debugging stuff
    std::ostringstream msg;
    
    for(auto const i_cluster: *ev_cluster){

      int iview = (int)i_cluster.View();
      
      double dw1, dt1, da1, db1, dw2, dt2, da2, db2;
      //calculate start/end refining figure of merit for this cluster
      dw1 = mother_sw_view[iview].w - i_cluster.StartPos()[0];
      dt1 = mother_sw_view[iview].t - i_cluster.StartPos()[1];

      da1 = dw1 * _wire_2_cm;
      db1 = dt1 * _time_2_cm;

      dw2 = mother_sw_view[iview].w - i_cluster.EndPos()[0];
      dt2 = mother_sw_view[iview].t - i_cluster.EndPos()[1];

      da2 = dw2 * _wire_2_cm;
      db2 = dt2 * _time_2_cm;
      
      //this is the variable that i want to go into the ttree... for single cluster events,
      //this will be a vector of length 3 (one per cluster, one cluster per view)
      double myFOM = std::sqrt( da1*da1 + db1*db1 ) - std::sqrt( da2*da2 + db2*db2 );
      _refinestartend_FOM.push_back(myFOM);

      //debugging stuff
      msg << Form("This cluster's start (wire,time) on view %d is (%f,%f)\n",
		  iview, i_cluster.StartPos()[0], i_cluster.StartPos()[1]);
      msg << Form("So, the _refinestartend_FOM value is %f", myFOM);

    }

    print(larlight::MSG::DEBUG,__FUNCTION__,msg.str());

    return true;
    
 
  }
  




} // end namespace larlight

#endif
