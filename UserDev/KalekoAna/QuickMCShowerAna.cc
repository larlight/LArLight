#ifndef QUICMCSHOWERANA_CC
#define QUICMCSHOWERANA_CC

#include "QuickMCShowerAna.hh"

namespace kaleko {

  bool QuickMCShowerAna::initialize() {
    
    _ana_tree=0;
    PrepareTTree();

    return true;
  }
  
  bool QuickMCShowerAna::analyze(larlight::storage_manager* storage) {
    
    ClearTTreeVars();
    
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
    
    if(ev_mcshower->size() != 1) {
      std::cout<<"why are there "<<ev_mcshower->size()<<" mcshowers?"<<std::endl;
      return false;
    }
    if(ev_shower->size() != 1) {
      std::cout<<"why are there "<<ev_shower->size()<<" showers?"<<std::endl;
      return false;
    }
    
    _mom_energy=ev_mcshower->at(0).MotherMomentum().at(3);
    _reco_energy=ev_shower->at(0).MIPEnergy().at(0);
    
    //fill TTree (once per event)
    if(_ana_tree)
      _ana_tree->Fill();   
    
    return true;

  }

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
      
      _ana_tree->Branch("mom_energy",&_mom_energy,"mom_energy/D");
      _ana_tree->Branch("reco_energy",&_reco_energy,"reco_energy/D");
      
    }
  }

  void QuickMCShowerAna::ClearTTreeVars() {

    _mom_energy = -1.;
    _reco_energy = -1.;

  }

}
#endif
