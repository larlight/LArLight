#ifndef MCSHOWERPARAMS_CC
#define MCSHOWERPARAMS_CC

#include "MCShowerParams.hh"

namespace larlight {

  bool MCShowerParams::initialize() {

    //event number tracker
    _event_num = 0;

    //Histograms that will be plotted
    hMotherE   = new TH1D("MotherEnergy",    "Energy of Shower Mother; [GeV]",     30, 0, 5);
    hDaughterE = new TH1D("DaughterEnergy",  "Energy of Daughter Mother; [GeV]",   30, 0, 5);
    hDvsM_E    = new TGraph();


    //What event to do 2D plots for?
    std::cout << "What event number to do 2D plots for?";
    std::cin >> _evt_plt;

    return true;
  }
  
  bool MCShowerParams::analyze(storage_manager* storage) {

    _event_num += 1;

    //get MC data                                                                                                                                      
    larlight::event_mcshower *event_mc = (event_mcshower*)(storage->get_data(DATA::MCShower));

    //make sure MC data is there                                                                                                                       
    if(!event_mc) {
      print(MSG::ERROR,__FUNCTION__,"Data storage did not find associated MCTruth");
      return false;
    }

    //Loop over all mcshower info
    for (size_t i=0; i<event_mc->size(); i++){

      larlight::mcshower* mcsh = (&(event_mc->at(i))); 

      //Fill Histograms:
      hMotherE->Fill((mcsh->MotherMomentum()).at(3));
      hDaughterE->Fill(((mcsh->DaughterMomentum()).at(3))/1000.0);
      hDvsM_E->SetPoint(i,(mcsh->MotherMomentum()).at(3),(mcsh->MotherMomentum()).at(3));

      if (_event_num == _evt_plt){
	std::cout << "Event: " << _event_num << std::endl;
	//max for x, y, z
	float maxx = -1000000;
	float maxy = -1000000;
	float maxz = -1000000;
	float minx = 1000000;
	float miny = 1000000;
	float minz = 1000000;
	
	std::vector<std::vector<Float_t> > daughterpoints = mcsh->DaughterPoints();
	std::cout << daughterpoints.size() << std::endl;

	//finx max/min for points
	for (size_t j=0; j<daughterpoints.size(); j++){

	  std::vector<Float_t> thispoint = daughterpoints.at(j);
	  if (thispoint.at(0) < minx)
	    minx = thispoint.at(0);
	  if (thispoint.at(1) < miny)
	    miny = thispoint.at(1);
	  if (thispoint.at(2) < minz)
	    minz = thispoint.at(2);
	  if (thispoint.at(0) > maxx)
	    maxx = thispoint.at(0);
	  if (thispoint.at(1) > maxy)
	    maxy = thispoint.at(1);
	  if (thispoint.at(2) > maxz)
	    maxz = thispoint.at(2);
	}

	maxx += 0.1*(maxx-minx);
	minx -= 0.1*(maxx-minx);
	maxy += 0.1*(maxy-miny);
	miny -= 0.1*(maxy-miny);
	maxz += 0.1*(maxz-minz);
	minz -= 0.1*(maxz-minz);
	int xmax = (int)(maxx);
	int ymax = (int)(maxy);
	int zmax = (int)(maxz);
	int xmin = (int)(minx);
	int ymin = (int)(miny);
	int zmin = (int)(minz);

	hShowerMap = new TH3D("ShowerMap", "Shower Map; Z [mm]; X [mm]; Y [mm]",  
			      50, zmin, zmax, 
			      50, xmin, xmax, 
			      50, ymin, ymax);
	hXYEnergy  = new TH2D("XYEnergy",  "Daughter X-Y Energy", 50, xmin, xmax, 50, ymin, ymax);
	hXZEnergy  = new TH2D("XZEnergy",  "Daughter X-Z Energy", 50, xmin, xmax, 50, zmin, zmax);
	hYZEnergy  = new TH2D("YZEnergy",  "Daughter Y-Z Energy", 50, ymin, ymax, 50, zmin, zmax);

	for (size_t j=0; j<daughterpoints.size(); j++){
	  std::vector<Float_t> thispoint = daughterpoints.at(j);
	  hShowerMap->Fill(thispoint.at(2), thispoint.at(0), thispoint.at(1),thispoint.at(3));
	  hXYEnergy->Fill(thispoint.at(0), thispoint.at(1), thispoint.at(3));
	  hXZEnergy->Fill(thispoint.at(0), thispoint.at(2), thispoint.at(3));
	  hYZEnergy->Fill(thispoint.at(1), thispoint.at(2), thispoint.at(3));
	}
      }

    }
    
    return true;
  }

  bool MCShowerParams::finalize() {

    hMotherE->Write();
    hDaughterE->Write();
    hShowerMap->Write();
    hXYEnergy->Write();
    hXZEnergy->Write();
    hYZEnergy->Write();
    hDvsM_E->Write();

    std::cout << "To see Energy Map for Shower event selected" << std::endl;
    std::cout << "Run .x ShowerMap.cc in Root interactive terminal from /mac/ folder" << std::endl;
    std::cout << "Warning: routine will look for histograms in mcshower.root" << std::endl;


    return true;
  }
}
#endif
