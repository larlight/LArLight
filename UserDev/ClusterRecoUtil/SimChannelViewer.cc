#ifndef SIMCHANNELVIEWER_CC
#define SIMCHANNELVIEWER_CC

#include "SimChannelViewer.hh"

namespace cluster{

  SimChannelViewer::SimChannelViewer()
  {
    hSimChView = 0;
  }
  
  
  void SimChannelViewer::GenerateHisto(const larlight::event_simch *esch, int plane)
  {
    if(!esch){
      //throw cluster::RecoUtilException("Invalid event_simch! Didn't find data!");
      std::cout<<"bawefaewfawoeifhaoweifhawoef"<<std::endl;
      return;
    }
    if(hSimChView) delete hSimChView;
    
    //get the geo/detector singletons only once
    const larutil::Geometry* mygeo = larutil::Geometry::GetME();
    const larutil::GeometryUtilities* mygeoutil = larutil::GeometryUtilities::GetME();
    const larutil::DetectorProperties* mydetprop = larutil::DetectorProperties::GetME();

    double fTimetoCm = mygeoutil->TimeToCm();
    double fWiretoCm = mygeoutil->WireToCm();
    double fElectronsToADC = mydetprop->ElectronsToADC();

    int min_wire = 99999;
    int max_wire = -99999;
    int min_TDC = 99999;
    int max_TDC = -99999;
    
    //find minimum and maximum channel for histo boundaries
    for(auto const sch : *esch) {

      if((int)mygeo->ChannelToPlane(sch.Channel()) != plane)
	continue;
      
      fTDCIDEs = sch.TDCIDEMap();
      
      std::map<UShort_t, std::vector< larlight::ide > >::iterator myit;

      for(myit=fTDCIDEs.begin(); myit!=fTDCIDEs.end(); ++myit)
	{
	  if(myit->first < min_TDC) min_TDC = (int)myit->first;
	  if(myit->first > max_TDC) max_TDC = (int)myit->first;
	}

      if((int)mygeo->ChannelToWire(sch.Channel()) < min_wire)
	min_wire = mygeo->ChannelToWire(sch.Channel());
      if((int)mygeo->ChannelToWire(sch.Channel()) > max_wire)
	max_wire = mygeo->ChannelToWire(sch.Channel());

    } //end first loop over event_simchannel
    
    //convert to cm stuff
    min_wire *= fWiretoCm;
    max_wire *= fWiretoCm;
    min_TDC *= fTimetoCm;
    max_TDC *= fTimetoCm;
    
    /*    
    hSimChView = new TH2D("hSimChView",
			  Form("Event %d; Wire [cm]; Time[cm]",esch->event_id()),
			  (max_wire-min_wire),min_wire,max_wire,
			  (max_TDC-min_TDC),min_TDC,max_TDC);
    
    */
    hSimChView = new TH2D("hSimChView",
			  Form("Event %d (Plane %d); Wire [cm]; Time[cm]",esch->event_id(),plane),
			  100,min_wire*0.75,max_wire*1.25,
			  100,min_TDC*0.75,max_TDC*1.25);

    //loop again over stuff to fill the hSimChView histo
    for(auto const sch : *esch) {
      
      if((int)mygeo->ChannelToPlane(sch.Channel()) != plane)
	continue;
      
      //sch is one wire
      fTDCIDEs = sch.TDCIDEMap();
      
      std::map<UShort_t, std::vector< larlight::ide > >::iterator myit;
      
      double totElectrons = 0;
      //loop over this wire's TDC ticks
      for(myit=fTDCIDEs.begin(); myit!=fTDCIDEs.end(); ++myit)
	{
	  //for each TDC tick add up the total number of electrons deposited
	  for(int i = 0; i < (int)myit->second.size(); i++)
	    totElectrons += myit->second.at(i).numElectrons;
	  
	  //fill the corresponding bin in the end histo 
	  hSimChView->
	    Fill( (mygeo->ChannelToWire(sch.Channel())) *fWiretoCm,
		  (myit->first)                         *fTimetoCm,
		  (totElectrons)                        *fElectronsToADC);
	  
	  //kaleko checking something real quick
	  //	  if(hSimChView->GetBinContent(sch.Channel(),myit->first))
	  //	    std::cout<<"something went wrong. same bin twice?"<<std::endl;
	  
	} 
    } //end second loop over event_simchannel
    
  }
    
} 

#endif
