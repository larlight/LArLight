#ifndef SHOWERDISTANCESTUDY_CC
#define SHOWERDISTANCESTUDY_CC

#include "ShowerDistanceStudy.hh"
#include "DistToBoxWall.hh"

namespace larlight {

  bool ShowerDistanceStudy::initialize() {

    _ana_tree=0;
    PrepareTTree();

	_count0=0; 
	_count1=0; 

    return true;
  }
  
  bool ShowerDistanceStudy::analyze(storage_manager* storage) {

	Clear(); 

//	std::cout<<"\n\nNEW EVENT:" ;
	auto my_mctruth = (const event_mctruth*)(storage->get_data(DATA::MCTruth)) ;

	std::vector<double> _elecMom ;
	std::vector<double> _elecVtx ;
		
	_count0++;


     for(auto const& mct : * my_mctruth) {
        
	    for(auto const& mcp : mct.GetParticles()) {
			_elecVtx.clear();
			_elecMom.clear();

//			std::cout<<"\nList of pdg codes: "<<mcp.PdgCode();

			if(mcp.PdgCode() == 11 ) {

				_elecEnergy = mcp.Trajectory().at(0).E(); 
				_elec_VtxX = mcp.Trajectory().at(0).X(); 
				_elec_VtxY = mcp.Trajectory().at(0).Y(); 
				_elec_VtxZ = mcp.Trajectory().at(0).Z(); 

				_elecVtx = { _elec_VtxX, _elec_VtxY, _elec_VtxZ } ;
				_elecMom = {mcp.Trajectory().at(0).Px(), mcp.Trajectory().at(0).Py(), mcp.Trajectory().at(0).Pz()};

				geoalgo::DistToBoxWall showerObject ;
			
				_dist_ToWall 		= showerObject.DistanceToWall(_elecVtx) ;
				_dist_AlongTraj 	= showerObject.DistanceToWall(_elecVtx,_elecMom,1);
				_dist_BackAlongTraj = showerObject.DistanceToWall(_elecVtx,_elecMom,0);

    	  		}
			}   
		if(_ana_tree)
			_ana_tree->Fill();

      	}   

	
//	auto my_mcshower = (const event_mcshower*)(storage->get_data(DATA::MCShower)) ; 

//	for( auto const & mcs : *my_mcshower) {
//
//
//		/// Define vectors to fill for distance alg
//		auto _kid_vtx 		= mcs.DaughterPosition(); 
//		auto _kid_momentum  = mcs.DaughterMomentum() ;
//		auto _mom_momentum  = mcs.MotherMomentum();
//
//		_motherEnergy = _mom_momentum.at(3) ; 
//		_daughterEnergy = _kid_momentum.at(3) ;
//
//
//		_count1++ ; 
//		geoalgo::DistToBoxWall showerObject ;
//	
//		_dist_ToWall 		= showerObject.DistanceToWall(_kid_vtx) ;
//		_dist_AlongTraj 	= showerObject.DistanceToWall(_kid_vtx,_kid_momentum,1);
//		_dist_BackAlongTraj = showerObject.DistanceToWall(_kid_vtx,_kid_momentum,0);
//
//		_dist_ToWallFromOut = showerObject.DistanceToWallFromOut( _kid_vtx ) ;
//
//
//	if(_ana_tree)
//		_ana_tree->Fill();
//	}
 
    return true;
  }
void ShowerDistanceStudy::PrepareTTree() {

    if(!_ana_tree) {
      _ana_tree = new TTree("ana_tree","");


//	  _ana_tree->Branch("_motherEnergy",&_motherEnergy,"_motherEnergy/D");
//	  _ana_tree->Branch("_daughterEnergy",&_daughterEnergy,"_daughterEnergy/D");

	  _ana_tree->Branch("_elecEnergy",&_elecEnergy,"_elecEnergy/D");
	  _ana_tree->Branch("_elec_VtxX",&_elec_VtxX,"elec_VtxX/D");
	  _ana_tree->Branch("_elec_VtxY",&_elec_VtxY,"elec_VtxY/D");
	  _ana_tree->Branch("_elec_VtxZ",&_elec_VtxZ,"elec_VtxZ/D");

	  _ana_tree->Branch("_dist_ToWall",&_dist_ToWall,"dist_ToWall/D");
	  _ana_tree->Branch("_dist_AlongTraj",&_dist_AlongTraj,"dist_AlongTraj/D");
	  _ana_tree->Branch("_dist_BackAlongTraj",&_dist_BackAlongTraj,"dist_BackAlongTraj/D");
	  

    }
 }

void ShowerDistanceStudy::Clear() {


  }	

 void Reset(){

 }


 bool ShowerDistanceStudy::finalize() {

    if(_fout) {

      _fout->cd();
      if(_ana_tree)
    	_ana_tree->Write();
      }
     else
       print(larlight::MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");

      delete _ana_tree;

	std::cout<<"All events and number of showers: "<<_count0<<", "<<_count1<<std::endl;

    return true;
  }

}
#endif
