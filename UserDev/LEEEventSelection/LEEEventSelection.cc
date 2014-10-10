#ifndef LEEEVENTSELECTION_CC
#define LEEEVENTSELECTION_CC

#include "LEEEventSelection.hh"

namespace larlight {

  bool LEEEventSelection::initialize() {

    _truthshower = ::ubsens::data::TruthShower();
    _recoshower = ::ubsens::data::RecoShower();
    
    _mgr.SetIOMode(::ubsens::data::DataManager::WRITE);
    _mgr.SetOutputFilename("aho.root");
    _mgr.Open();
    
    return true;
  }
  
  bool LEEEventSelection::analyze(storage_manager* storage) {


    //grab the MC showers
    larlight::event_mcshower* ev_mcshower = (larlight::event_mcshower*)storage->get_data(larlight::DATA::MCShower);
    if(!ev_mcshower) {
      print(larlight::MSG::ERROR,__FUNCTION__,Form("Did not find specified data product, MCShower!"));
      return false;
    }
    
    //grab the reconstructed showers, but don't quit if they DNE
    larlight::event_shower* ev_shower = (larlight::event_shower*)storage->get_data(larlight::DATA::Shower);
    if(!ev_shower && _include_reco_showers)  {
      print(larlight::MSG::WARNING,__FUNCTION__,Form("Did not find specified data product, Shower! AND you wanted to use them! Consider using SetIncludeRecoShowers(False)."));
    }

    

    //here decide if event passes event selection cut (in fid volume, IE)
    //NO! WRITE A FILTER THAT RUNS BEFORE THIS! USE FILTER MODE!

    //here code some stuff following kazu's ubsens example.py
    //including writing to an output file
    
    
    //loop over mcshowers, save into ubsens dataholder
    for (auto imcs: *ev_mcshower){
      _truthshower.Reset();

      _truthshower.MotherPDGID     ( imcs.MotherPDGID()      );
      _truthshower.MotherVtx       ( imcs.MotherPosition()   );
      _truthshower.MotherMomentum  ( imcs.MotherMomentum()   );
      _truthshower.DaughterVtx     ( imcs.DaughterPosition() );
      _truthshower.DaughterMomentum( imcs.DaughterMomentum() );

      _mgr.GetWriteableData().AppendTruthShower(_truthshower);
    }


    //if using reco showers, loop over showers, save into ubsens dataholder
    if( _include_reco_showers ){
      
      for (auto ish: *ev_shower){
	_recoshower.Reset();
	
	Int_t bp = ish.best_plane();
	_recoshower.DCosStart(   ish.Direction(),     ish.DirectionErr()     );
	_recoshower.XYZStart(    ish.ShowerStart(),   ish.ShowerStartErr()   );
	_recoshower.TotalEnergy( ish.Energy().at(bp), ish.EnergyErr().at(bp) );
	_recoshower.dEdx(        ish.dEdx().at(bp),   ish.dEdxErr().at(bp)   );
	
	_mgr.GetWriteableData().AppendRecoShower(_recoshower);
      }
     
    }

    _mgr.SaveEntry();
    

    return true;
  }

  bool LEEEventSelection::finalize() {
    
    _mgr.Close();
    
    return true;
  }
}
#endif
