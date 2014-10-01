#ifndef SHOWERDISTANCESTUDY_CC
#define SHOWERDISTANCESTUDY_CC

#include "ShowerDistanceStudy.hh"
#include "ShowerContainmentCut.hh"

namespace larlight {

  bool ShowerDistanceStudy::initialize() {

    _ana_tree=0;
    PrepareTTree();

    return true;
  }
  
  bool ShowerDistanceStudy::analyze(storage_manager* storage) {
 
	_reco_start1.clear() ;
	_reco_start2.clear() ;
	_direction1.clear() ;
	_direction2.clear() ;

	auto my_mcshower = (const event_mcshower*)(storage->get_data(DATA::MCShower)) ; 
	auto my_recoshower = (const event_shower*)(storage->get_data(DATA::Shower)) ; 	
	
	if(my_recoshower->size()!=2)
			return -1; 

	auto bestPlane = my_recoshower->at(0).best_plane() ;

//	std::cout<<"\nEnergy size: "<<my_recoshower->at(0).Energy().at(bestPlane) ;

	_mother1Energy = my_mcshower->at(0).MotherMomentum().at(3) ;
	_mother2Energy = my_mcshower->at(1).MotherMomentum().at(3) ;
	_daughter1_Energy = my_mcshower->at(0).DaughterMomentum().at(3) ;
	_daughter2_Energy = my_mcshower->at(1).DaughterMomentum().at(3) ;

	_motherMomentumX = my_mcshower->at(0).MotherMomentum().at(0) ;
	_motherMomentumY = my_mcshower->at(0).MotherMomentum().at(1) ;
	_motherMomentumZ = my_mcshower->at(0).MotherMomentum().at(2) ;

	_reco_daughter1_Energy = my_recoshower->at(0).Energy().at(bestPlane) ;
	
	std::vector<double> dist1 ;
	std::vector<double> dist2 ;
	std::vector<double> momentum1;
	std::vector<double> momentum2;


	for(size_t i = 0; i<3 ; i++){
		dist1.push_back(my_mcshower->at(0).DaughterPosition().at(i)) ;
		dist2.push_back(my_mcshower->at(1).DaughterPosition().at(i)) ;
		momentum1.push_back(my_mcshower->at(0).DaughterMomentum().at(i)) ;
		momentum2.push_back(my_mcshower->at(1).DaughterMomentum().at(i)) ;

		}

//		std::cout<<"Reco X position: "<<my_recoshower->at(0).ShowerStart().X() <<std::endl;
//		std::cout<<"MC X position: "<<my_mcshower->at(0).DaughterPosition().at(0) <<std::endl;
//		std::cout<<"reco shower size L: "<<my_recoshower->size() <<std::endl;

		//ShowerStart() and Direction() are TVector3-- put their x,y,z components into a 
		//vector to pass to DistanceToWall
		_reco_start1.push_back( my_recoshower->at(0).ShowerStart().X());
		_reco_start1.push_back( my_recoshower->at(0).ShowerStart().Y());
		_reco_start1.push_back( my_recoshower->at(0).ShowerStart().Z());

		_reco_start2.push_back( my_recoshower->at(1).ShowerStart().X());
		_reco_start2.push_back( my_recoshower->at(1).ShowerStart().Y());
		_reco_start2.push_back( my_recoshower->at(1).ShowerStart().Z());

		//Issue is Direction gives unit vector in direction--already unit vector
		_direction1.push_back( my_recoshower->at(0).Direction().X());
		_direction1.push_back( my_recoshower->at(0).Direction().Y());
		_direction1.push_back( my_recoshower->at(0).Direction().Z());

		_direction2.push_back( my_recoshower->at(1).Direction().X());
		_direction2.push_back( my_recoshower->at(1).Direction().Y());
		_direction2.push_back( my_recoshower->at(1).Direction().Z());


	
	 showerana::ShowerContainmentCut showerObject ;

	_dist1_ToWall = showerObject.DistanceToWall(dist1) ;
	_dist1_AlongTraj = showerObject.DistanceToWall(dist1,momentum1);

	_dist2_ToWall = showerObject.DistanceToWall(dist2) ;
	_dist2_AlongTraj = showerObject.DistanceToWall(dist2,momentum2);

	_reco_dist1_ToWall = showerObject.DistanceToWall(_reco_start1) ;
	_reco_dist2_ToWall = showerObject.DistanceToWall(_reco_start2) ;

	_reco_dist1_AlongTraj = showerObject.DistanceToWall(_reco_start1,_direction1) ;
	_reco_dist2_AlongTraj = showerObject.DistanceToWall(_reco_start2,_direction2) ;



	//Also, calculate pi0 mass 
	  double pi0_4dot_prod = -2* (my_mcshower->at(0).DaughterMomentum().at(0)* my_mcshower->at(1).DaughterMomentum().at(0) 
							+ my_mcshower->at(0).DaughterMomentum().at(1)* my_mcshower->at(1).DaughterMomentum().at(1) 
							+ my_mcshower->at(0).DaughterMomentum().at(2)* my_mcshower->at(1).DaughterMomentum().at(2)
							- _daughter1_Energy * _daughter2_Energy) ; 

    _pi0_mass = pow(pi0_4dot_prod,0.5); 


	if(_ana_tree)
		_ana_tree->Fill();
 
    return true;
  }
void ShowerDistanceStudy::PrepareTTree() {

    if(!_ana_tree) {
      _ana_tree = new TTree("ana_tree","");


	  _ana_tree->Branch("_pi0_mass",&_pi0_mass,"_pi0_mass/D");
	  _ana_tree->Branch("_mother1Energy",&_mother1Energy,"_mother1Energy/D");
	  _ana_tree->Branch("_mother2Energy",&_mother2Energy,"_mother2Energy/D");
	  _ana_tree->Branch("_daughter1_Energy",&_daughter1_Energy,"_daughter1_Energy/D");
	  _ana_tree->Branch("_daughter2_Energy",&_daughter2_Energy,"_daughter2_Energy/D");

	  _ana_tree->Branch("_motherMomentumX",&_motherMomentumX,"_motherMomentumX/D");
	  _ana_tree->Branch("_motherMomentumY",&_motherMomentumY,"_motherMomentumY/D");
	  _ana_tree->Branch("_motherMomentumZ",&_motherMomentumZ,"_motherMomentumZ/D");

	  _ana_tree->Branch("_reco_daughter1_Energy",&_reco_daughter1_Energy,"_reco_daughter1_Energy/D");

	  _ana_tree->Branch("_dist1_ToWall",&_dist1_ToWall,"_dist1_ToWall/D");
	  _ana_tree->Branch("_dist1_AlongTraj",&_dist1_AlongTraj,"_dist1_AlongTraj/D");
	  _ana_tree->Branch("_dist2_ToWall",&_dist2_ToWall,"_dist2_ToWall/D");
	  _ana_tree->Branch("_dist2_AlongTraj",&_dist2_AlongTraj,"_dist2_AlongTraj/D");

	  _ana_tree->Branch("_reco_dist1_ToWall",&_reco_dist1_ToWall,"_reco_dist1_ToWall/D");
	  _ana_tree->Branch("_reco_dist1_AlongTraj",&_reco_dist1_AlongTraj,"_reco_dist1_AlongTraj/D");
	  _ana_tree->Branch("_reco_dist2_ToWall",&_reco_dist2_ToWall,"_reco_dist2_ToWall/D");
	  _ana_tree->Branch("_reco_dist2_AlongTraj",&_reco_dist2_AlongTraj,"_reco_dist2_AlongTraj/D");


	  

    }
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


    return true;
  }

}
#endif
