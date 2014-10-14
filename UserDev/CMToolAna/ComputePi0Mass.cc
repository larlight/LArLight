#ifndef COMPUTEPI0MASS_CC
#define COMPUTEPI0MASS_CC

#include "ComputePi0Mass.hh"

namespace larlight {
  
  ComputePi0Mass* ComputePi0Mass::me = 0;

  bool ComputePi0Mass::initialize() {

    hPi0MassPeak = new TH1D("hPi0MassPeak","Pi0 Mass Peak in MeV",100,0,300);

    hDaughterVtxDist = new TH1D("hDaughterVtxDist",
				"Pi0 Daughter Start Point Distance (Reco vs. MC); Distance [cm]; Showers",
				100,0,10);

    hDaughterAngleDiff = new TH1D("hDaughterAngleDiff",
				  "Pi0 Daughter 3D Angle Diff (Reco vs. MC); Opening Angle [degrees]; Showers",
				  100,0,180);
    return true;
  }
  
  bool ComputePi0Mass::analyze(storage_manager* storage) {

    _mass = -99999.;

    // Load the Showers... need to run shower reconstruction first!
    auto ev_shower  = (const event_shower*)(storage->get_data(_shower_type));
    auto ev_cluster = (const event_cluster*)(storage->get_data(_cluster_type));

    if(!ev_shower ) {
      print(MSG::ERROR,__FUNCTION__,Form("Data product \"%s\" not found...",
					 DATA::DATA_TREE_NAME[_shower_type].c_str()));
      return false;
    }else if(ev_shower->size()<1) {
      if(get_verbosity() <= MSG::INFO)
	print(MSG::INFO,__FUNCTION__,Form("There are 0 showers in this event! Skipping......"));      
      return false;
    }
    if(!ev_cluster ) {
      print(MSG::ERROR,__FUNCTION__,Form("Data product \"%s\" not found...",
					 DATA::DATA_TREE_NAME[_cluster_type].c_str()));
      return false;
    }else if(ev_cluster->size()<1) {
      print(MSG::ERROR,__FUNCTION__,Form("There are 0 clusters in this event! Skipping......"));      
      return false;
    }else if(!(ev_shower->at(0).association(_cluster_type).size())){
      print(MSG::ERROR,__FUNCTION__,Form("Association %s => %s not found! Skipping...",
					 DATA::DATA_TREE_NAME[_cluster_type].c_str(),
					 DATA::DATA_TREE_NAME[_shower_type].c_str()));
      return false;
    }

    //skip event if !2 showers found.
    if(ev_shower->size() != 2) return false;

    size_t best_cluster1 = 0;
    for(auto const& index : ev_shower->at(0).association(_cluster_type)) {
      if(ev_shower->at(0).best_plane() == ev_cluster->at(index).View()) break;
      ++best_cluster1;
    }

    size_t best_cluster2 = 0;
    for(auto const& index : ev_shower->at(1).association(_cluster_type)) {
      if(ev_shower->at(1).best_plane() == ev_cluster->at(index).View()) break;
      ++best_cluster2;
    }
    _mass = Pi0MassFormula3D( fSECaloAlg.ShowerEnergy(ev_shower->at(0).Energy().at(best_cluster1)).first,
			      fSECaloAlg.ShowerEnergy(ev_shower->at(1).Energy().at(best_cluster2)).first,
    			      ev_shower->at(0).Direction(),
    			      ev_shower->at(1).Direction() );
    
    hPi0MassPeak->Fill(_mass);

    // Now let's also inspect shower angle
    auto ev_mcshower = (const event_mcshower*)(storage->get_data(DATA::MCShower));
    if(!ev_mcshower) return true;
    
    // For a given Shower, compute corresponding MCShower
    fBTAlg.Prepare(storage,_cluster_type);
    
    for(auto const& s : *ev_shower) {

      size_t mcs_index = 0;
      double correctness = -1;
      fBTAlg.Match( s.association(_cluster_type), mcs_index, correctness );

      if(correctness) {

	auto const& mcs = ev_mcshower->at(mcs_index);
	auto reco_dir = s.Direction();
	auto reco_vtx = s.ShowerStart();

	auto mc_dir = mcs.DaughterMomentum();
	auto mc_vtx = mcs.DaughterPosition();

	double mag = sqrt(pow(mc_dir[0],2)+pow(mc_dir[1],2)+pow(mc_dir[2],2));
	mc_dir[0] /= mag;
	mc_dir[1] /= mag;
	mc_dir[2] /= mag;

	hDaughterVtxDist->Fill( sqrt( pow(mc_vtx[0] - reco_vtx[0],2) +
				      pow(mc_vtx[1] - reco_vtx[1],2) +
				      pow(mc_vtx[2] - reco_vtx[2],2) 
				      )
				);

	double dot_prod = 0;
	dot_prod += reco_dir[0] * mc_dir[0];
	dot_prod += reco_dir[1] * mc_dir[1];
	dot_prod += reco_dir[2] * mc_dir[2];
	
	hDaughterAngleDiff->Fill( acos(dot_prod) / TMath::Pi() * 180 );

      }

    }
    
    return true;
  }

  bool ComputePi0Mass::finalize() {

    if(_fout) { 
      _fout->cd(); 
      hPi0MassPeak->Write();
      hDaughterVtxDist->Write();
      hDaughterAngleDiff->Write();
    }
    else 
      print(MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
    
  
    return true;
  }


  //Get PI0 Mass from photon directions and energy
  float ComputePi0Mass::Pi0MassFormula3D( float Energy1, float Energy2, TVector3 Direction3D_1, TVector3 Direction3D_2){
    
    float angle_3D = acos( Direction3D_1 * Direction3D_2 );

    //using a formula from 
    //http://www.hep.princeton.edu/~mcdonald/examples/piondecay.pdf      
    return pow(4. * Energy1 * Energy2 * pow(sin( 0.5 * angle_3D ),2) , 0.5);

  }

}
#endif
