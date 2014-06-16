#ifndef MCSHOWERQUALITY_CC
#define MCSHOWERQUALITY_CC

#include "MCShowerQuality.hh"

namespace larlight {

  MCShowerQuality::MCShowerQuality()
  { 
    _name="MCShowerQuality"; 
    _fout = 0;
    _energy_min = 20;
    _dx = 30;
  };

  bool MCShowerQuality::initialize() {

    _hCount = new TH1D("hCount",
		       "# of MCShower per Event; Count; Events",
		       10,-0.5,9.5);
    
    _hEnergy = new TH2D("hEnergy",
			"Mother vs. Daughter Energy; Mother Energy [MeV]; Daughter Energy [MeV]",
			200,0,2000,200,0,2000);

    _hDEDX = new TH2D("hDEDX",
		      Form("dEdX (%g cm) vs. Mother Energy; dEdX; Mother Energy [MeV]",_dx/10.),
		      200,0,500,
		      200,0,2000);
    
    _hEnergyFrac = new TH1D("hEnergyFrac",
			    "Daughter/Mother Energy Fraction; Fraction; MCShower",
			    200,0,1);

    return true;
  }
  
  bool MCShowerQuality::analyze(storage_manager* storage) {

    const event_mcshower* ev_mcs = (const event_mcshower*)(storage->get_data(DATA::MCShower));

    UInt_t ctr=0;
    for(auto const &mcs : *ev_mcs) {

      double mom_e = mcs.MotherMomentum().at(3) * 1.e3;
      double chi_e = mcs.DaughterMomentum().at(3);

      std::vector<float> vtx = mcs.DaughterPosition();

      if(mom_e < _energy_min) continue;

      ctr++;

      _hEnergy->Fill(mom_e, chi_e);
      
      _hEnergyFrac->Fill(chi_e / mom_e);

      float de = 0;
      float dx_cut = pow(_dx,2);
      float dx = 0;

      for(auto const &edep : mcs.DaughterPoints()) {

	dx = pow(vtx.at(0) - edep.at(0),2) + pow(vtx.at(1) - edep.at(1),2) + pow(vtx.at(2) - edep.at(2),2);

	if(dx > dx_cut) continue;

	de += edep.at(3);
	
      }

      //std::cout<<mom_e<<" : "<<de<<" : "<<_dx<<std::endl;
      _hDEDX->Fill(de/(_dx*0.1),mom_e);

    }

    _hCount->Fill(ctr);
  
    return true;
  }

  bool MCShowerQuality::finalize() {

    if(_fout) {

      _hCount->Write();
      _hEnergy->Write();
      _hDEDX->Write();
      _hEnergyFrac->Write();

    }
    return true;
  }

}
#endif
