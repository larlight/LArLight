#ifndef SHOWERQUALITY_CC
#define SHOWERQUALITY_CC

#include "ShowerQuality.hh"

namespace larlight {

  bool ShowerQuality::initialize() {

    for(size_t i=0; i<4; ++i) {

      vdEdX.push_back(new TH1D(Form("hdEdX_%zu",i),
			       Form("dE/dX for Plane %zu; dE/dX; Showers",i),
			       100,0,10));

    }
    
    return true;
  }
  
  bool ShowerQuality::analyze(storage_manager* storage) {

    auto ev_mcs = (const event_mcshower*)(storage->get_data(DATA::MCShower));
    auto ev_shower = (const event_shower*)(storage->get_data(DATA::Shower));
    auto ev_cluster = (const event_cluster*)(storage->get_data(DATA::Cluster));
    if(!ev_mcs || !ev_shower || !ev_cluster) return false;

    if(ev_mcs->size() == 0) return false;

    auto mc_vtx = ev_mcs->at(0).DaughterPosition();

    auto mc_dir = ev_mcs->at(0).DaughterMomentum();

    if(ev_mcs->at(0).DaughterMomentum().at(3) / ev_mcs->at(0).MotherMomentum().at(3) < _energy_containment)
      return false;

    mc_dir.at(0) = mc_dir.at(0)/mc_dir.at(3);
    mc_dir.at(1) = mc_dir.at(1)/mc_dir.at(3);
    mc_dir.at(2) = mc_dir.at(2)/mc_dir.at(3);

    for(auto const& s : *ev_shower) {

      auto reco_vtx = s.ShowerStart();

      auto reco_dir = s.Direction();

      double dist = sqrt( pow(mc_vtx[0] - reco_vtx[0],2) +
			  pow(mc_vtx[1] - reco_vtx[1],2) +
			  pow(mc_vtx[2] - reco_vtx[2],2) );

      double dot_prod = 0;

      dot_prod += reco_dir[0] * mc_dir[0];
      dot_prod += reco_dir[1] * mc_dir[1];
      dot_prod += reco_dir[2] * mc_dir[2];

      double angle_diff = acos(dot_prod) / 3.14*180;

      if(dist > _dist_max || angle_diff > _angle_max) return false;

      size_t ctr=0;
      for(auto const& index : s.association(DATA::Cluster)) {

	auto const& c = ev_cluster->at(index);
	
	size_t plane = c.View();

	vdEdX.at(plane)->Fill(s.dEdx().at(ctr));

	if(plane == s.best_plane())
	  vdEdX.at(3)->Fill(s.dEdx().at(ctr));

	ctr++;
      }
    }
  
    return true;
  }

  bool ShowerQuality::finalize() {

    if(_fout) {

      _fout->cd();

      for(auto &h : vdEdX) h->Write();

    }
  
    return true;
  }

}
#endif
