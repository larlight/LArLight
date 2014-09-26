#ifndef SHOWERQUALITY_CC
#define SHOWERQUALITY_CC

#include "ShowerQuality.hh"

namespace larlight {

  bool ShowerQuality::initialize() {

    fTree = new TTree("tree","");
    fTree->Branch("dist",&_dist,"dist/D");
    fTree->Branch("angle_diff",&_angle_diff,"angle_diff/D");
    fTree->Branch("energy_containment",&_energy_containment,"energy_containment/D");
    fTree->Branch("mc_energy",&_mc_energy,"mc_energy/D");
    fTree->Branch("energy",&_energy,"energy/D");
    fTree->Branch("mip_energy",&_mip_energy,"mip_energy/D");
    fTree->Branch("dedx",&_dEdX,"dedx/D");
    fTree->Branch("plane",&_plane,"plane/i");
    fTree->Branch("best_plane",&_best_plane,"best_plane/i");
    fTree->Branch("num_mcs",&_num_mcs,"num_mcs/i");
    
    return true;
  }
  
  bool ShowerQuality::analyze(storage_manager* storage) {

    event_clear();

    auto ev_mcs = (const event_mcshower*)(storage->get_data(DATA::MCShower));
    auto ev_shower = (const event_shower*)(storage->get_data(DATA::Shower));
    auto ev_cluster = (const event_cluster*)(storage->get_data(DATA::Cluster));
    if(!ev_mcs || !ev_shower || !ev_cluster) return false;

    std::vector<double> mc_dir;
    std::vector<double> mc_vtx;

    for(auto const& mcs : *ev_mcs)

      if(mcs.DaughterMomentum().at(3)>10) _num_mcs++;

    if(ev_mcs->size()) {
      
      mc_vtx = ev_mcs->at(0).DaughterPosition();
      
      mc_dir = ev_mcs->at(0).DaughterMomentum();
      
      _mc_energy = ev_mcs->at(0).DaughterMomentum().at(3);
      
      _energy_containment = ev_mcs->at(0).DaughterMomentum().at(3) / ev_mcs->at(0).MotherMomentum().at(3);

      mc_dir.at(0) = mc_dir.at(0)/mc_dir.at(3);
      mc_dir.at(1) = mc_dir.at(1)/mc_dir.at(3);
      mc_dir.at(2) = mc_dir.at(2)/mc_dir.at(3);

    }
      
    for(auto const& s : *ev_shower) {

      auto reco_vtx = s.ShowerStart();

      auto reco_dir = s.Direction();

      if(mc_vtx.size()) 
	_dist = sqrt( pow(mc_vtx[0] - reco_vtx[0],2) +
		      pow(mc_vtx[1] - reco_vtx[1],2) +
		      pow(mc_vtx[2] - reco_vtx[2],2) );
      
      double dot_prod = 0;

      if(mc_dir.size()) {
	dot_prod += reco_dir[0] * mc_dir[0];
	dot_prod += reco_dir[1] * mc_dir[1];
	dot_prod += reco_dir[2] * mc_dir[2];

	_angle_diff = acos(dot_prod) / 3.14*180;
      }

      _best_plane = s.best_plane();

      auto ass_index = s.association(DATA::Cluster);

      for(size_t i=0; i<ass_index.size(); ++i) {

	auto const& c = ev_cluster->at(ass_index[i]);

	plane_clear();
	
	_plane = c.View();

	_mip_energy = s.MIPEnergy().at(i);

	_energy = s.Energy().at(i);

	_dEdX = s.dEdx().at(i);
	
	fTree->Fill();
      }

    }
  
    return true;
  }

  bool ShowerQuality::finalize() {

    if(_fout) {

      _fout->cd();

      fTree->Write();

    }
  
    return true;
  }

}
#endif
