#ifndef QUICMCSHOWERANA_CC
#define QUICMCSHOWERANA_CC

#include "QuickMCShowerAna.hh"

namespace kaleko {

  bool QuickMCShowerAna::initialize() {
    
    _cluster_type = ::larlight::DATA::Cluster;

    _ana_tree=0;

    PrepareTTree();

    if(!fGSer) fGSer = (larutil::GeometryUtilities*)(larutil::GeometryUtilities::GetME());

    return true;
  }
  
  bool QuickMCShowerAna::analyze(larlight::storage_manager* storage) {

    _mcshower_match_alg.Prepare(storage,_cluster_type);
    
    //grab the reconstructed showers
    larlight::event_shower* ev_shower = (larlight::event_shower*)storage->get_data(larlight::DATA::Shower);
    if(!ev_shower) {
      print(larlight::MSG::ERROR,__FUNCTION__,Form("Did not find specified data product, Shower!"));
      return false;
    }
    
    //grab the MC showers
    larlight::event_mcshower* ev_mcshower = (larlight::event_mcshower*)storage->get_data(larlight::DATA::MCShower);
    if(!ev_mcshower) {
      print(larlight::MSG::ERROR,__FUNCTION__,Form("Did not find specified data product, MCShower!"));
      return false;
    }

    //grab the reconstructed clusters
    larlight::event_cluster* ev_cluster = (larlight::event_cluster*)storage->get_data(_cluster_type);
    if(!ev_cluster) {
      print(larlight::MSG::ERROR,__FUNCTION__,Form("Did not find specified data product, Cluster!"));
      return false;
    }

    //grab the reconstructed hits
    larlight::event_hit* ev_hit = (larlight::event_hit*)storage->get_data(larlight::DATA::GausHit);
    if(!ev_hit) {
      print(larlight::MSG::ERROR,__FUNCTION__,Form("Did not find specified data product, Hit!"));
      return false;
    }


    if(_true_electrons_false_pi0){
      if(ev_mcshower->size() != 1) {
	std::cout<<"Why are there "<<ev_mcshower->size()<<" mcshowers? For elecrons, should only have 1. Skipping this event."<<std::endl;
	return false;
      }
      if(ev_shower->size() != 1) {
	std::cout<<"Why are there "<<ev_shower->size()<<" showers? For electrons, should only have 1. Skipping this event."<<std::endl;
	return false;
      }
    }
    else{
      if(ev_mcshower->size() != 2) {
	std::cout<<"Why are there "<<ev_mcshower->size()<<" mcshowers? For pi0's, should have 2. Skipping this event."<<std::endl;
	return false;
      }
      if(ev_shower->size() != 2) {
	std::cout<<"Why are there "<<ev_shower->size()<<" showers? For pi0's, should have 2. Skipping this event."<<std::endl;
	return false;
      }
    }

    //loop over the reconstructed showers,
    //use MCShowerMatchAlg to get the mcshower index that matches
    for(size_t ishower = 0; ishower < ev_shower->size(); ++ishower){

      ClearTTreeVars();
     
      //make vector of associations (need by mcshowermatchalg)
      cluster_indices.clear();
      for(size_t iclus = 0; iclus < ev_shower->at(ishower).association(_cluster_type).size(); ++iclus){
	cluster_indices.push_back(ev_shower->at(ishower).association(_cluster_type).at(iclus));
      }
      
      //find the mcshower index that matches this shower
      _mcshower_match_alg.Match(cluster_indices,mcshower_index,_correctness);
      //   std::cout<<"done matching this shower. mcshowerindex is "<<mcshower_index<<", correctness is "<<_correctness<<std::endl;
      
      //use the mcshower index to fill ttree stuff
      _event_id=ev_mcshower->event_id();
      _mom_energy=ev_mcshower->at(mcshower_index).MotherMomentum().at(3);
      _daught_energy=ev_mcshower->at(mcshower_index).DaughterMomentum().at(3);
      _reco_energy=ev_shower->at(ishower).MIPEnergy().at(0);
      
      double mom_dir_magnitude = 
	pow(ev_mcshower->at(mcshower_index).DaughterMomentum().at(0),2)+
	pow(ev_mcshower->at(mcshower_index).DaughterMomentum().at(1),2)+
	pow(ev_mcshower->at(mcshower_index).DaughterMomentum().at(2),2);
      
      mom_dir_magnitude = pow(mom_dir_magnitude,0.5);
      
      for(int k = 0; k < 3; ++k){
	//mcshower direction is made into a unit vector
	_mc_shower_direction.push_back(
	  ev_mcshower->at(mcshower_index).DaughterMomentum().at(k)
	  / mom_dir_magnitude);

	//reco shower direction is already unit vector
	_reco_shower_direction.push_back(ev_shower->at(ishower).Direction()[k]);
	
	_mc_shower_start_vtx.push_back(
	  ev_mcshower->at(mcshower_index).MotherPosition().at(k)
	);
	
	_reco_shower_start_vtx.push_back(
	  ev_shower->at(ishower).ShowerStart()[k]
	);

	_best_plane = ev_shower->at(mcshower_index).best_plane();

      }

      /*
      std::cout
	<< _mc_shower_direction.at(0)<<" : "
	<< _mc_shower_direction.at(1)<<" : "
	<< _mc_shower_direction.at(2)<<" ... "
	<< mom_dir_magnitude << std::endl;
      */
    
      _dot_prod = _mc_shower_direction[0]*_reco_shower_direction[0]+
	_mc_shower_direction[1]*_reco_shower_direction[1]+
	_mc_shower_direction[2]*_reco_shower_direction[2];
      
      _acos_dot_prod = TMath::ACos(_dot_prod);
      
      _dist = pow(
	pow(_mc_shower_start_vtx.at(0)-_reco_shower_start_vtx.at(0),2)+
	pow(_mc_shower_start_vtx.at(1)-_reco_shower_start_vtx.at(1),2)+
	pow(_mc_shower_start_vtx.at(2)-_reco_shower_start_vtx.at(2),2),
	0.5);



      //i also care about some CPAN stuff for the 3 clusters that made the shower
      //loop over the clusters that made this shower and calculate CPAN for each
      if(cluster_indices.size() != 3)
	std::cout<<"something is fucked up."<<std::endl;
      for (unsigned int iclus = 0; iclus < cluster_indices.size(); ++iclus){
	auto cl = ev_cluster->at(cluster_indices.at(iclus));
	std::vector<unsigned int> hit_index = cl.association(larlight::DATA::GausHit);
	::cluster::ClusterParamsAlg bestclus_CPAN;
	_cru_helper.GenerateCPAN( hit_index, ev_hit, bestclus_CPAN);
	bestclus_CPAN.DisableFANN();
	bestclus_CPAN.SetVerbose(false);
	bestclus_CPAN.FillParams(true,true,true,true,true,true);

	//this is in degrees
	_2Dclusterangle_perplane.at(cl.View())=bestclus_CPAN.GetParams().angle_2d;
	
	//get 2d angle from MC 3D
	//make _mc_shower_direction into TVector3
	TVector3 myvec(_mc_shower_direction.at(0),
		       _mc_shower_direction.at(1),
		       _mc_shower_direction.at(2));

	//this is in radians... put it in degrees
	_2Dclusterangle_FROM3DMC_perplane.at(cl.View()) = 
	  fGSer->Get2DangleFrom3D(cl.View(),myvec) * TMath::RadToDeg();
	
      }//done calculating the 3 CPANs
      
      //fill TTree (once per shower)
      if(_ana_tree)
	_ana_tree->Fill();   
      
    }//end loop over showers
    
    return true;
    
  }//end analyze
    
  bool QuickMCShowerAna::finalize() {

    if(_fout) {
      
      _fout->cd();
      if(_ana_tree)
	_ana_tree->Write();
      
    }
    else 
      print(larlight::MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
    
    delete _ana_tree;
    
    return true;
  }
  
  void QuickMCShowerAna::PrepareTTree() {


    if(!_ana_tree) {
      _ana_tree = new TTree("ana_tree","");
      
      _ana_tree->Branch("event_id",&_event_id,"event_id/I");
      _ana_tree->Branch("mom_energy",&_mom_energy,"mom_energy/D");
      _ana_tree->Branch("daught_energy",&_daught_energy,"daught_energy/D");
      _ana_tree->Branch("reco_energy",&_reco_energy,"reco_energy/D");
      _ana_tree->Branch("mc_show_dir","std::vector<double>",&_mc_shower_direction);
      _ana_tree->Branch("reco_show_dir","std::vector<double>",&_reco_shower_direction);
      _ana_tree->Branch("dot_prod",&_dot_prod,"dot_prod/D");
      _ana_tree->Branch("acos_dot_prod",&_acos_dot_prod,"acos_dot_prod/D");
      _ana_tree->Branch("mc_show_start_vtx","std::vector<double>",&_mc_shower_start_vtx);
      _ana_tree->Branch("reco_show_start_vtx","std::vector<double>",&_reco_shower_start_vtx);
      _ana_tree->Branch("dist",&_dist,"dist/D");
      _ana_tree->Branch("correctness",&_correctness,"correctness/D");
      _ana_tree->Branch("clus_angle_2d_perplane","std::vector<double>",&_2Dclusterangle_perplane);
      _ana_tree->Branch("clus_angle_2d_from3DMC_perplane","std::vector<double>",&_2Dclusterangle_FROM3DMC_perplane);
      _ana_tree->Branch("best_plane",&_best_plane,"best_plane/I");
    }
  }

  void QuickMCShowerAna::ClearTTreeVars() {

    _event_id = -1;
    _mom_energy = -1.;
    _daught_energy = -1.;
    _reco_energy = -1.;
    _mc_shower_direction.clear();
    _reco_shower_direction.clear();
    _dot_prod=-2.;
    _acos_dot_prod = 999.;
    _mc_shower_start_vtx.clear();
    _reco_shower_start_vtx.clear();
    _dist = -999.;
    _correctness = -999.;
    _2Dclusterangle_perplane.clear();
    _2Dclusterangle_FROM3DMC_perplane.clear();
    _best_plane = -999;
    
    for (size_t david = 0; david < 3; david++){
      _2Dclusterangle_perplane.push_back(-999.);
      _2Dclusterangle_FROM3DMC_perplane.push_back(-999.);
    }
  }

}
#endif
