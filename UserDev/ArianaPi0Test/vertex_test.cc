#ifndef VERTEX_TEST_CC
#define VERTEX_TEST_CC

#include "vertex_test.hh"
#include <random>

namespace larlight {

  bool vertex_test::initialize() {
	      
    _ana_tree=0;
	_count = 0 ;
    PrepareTTree();

    return true;
  }

 
  bool vertex_test::analyze(storage_manager* storage) {

  
//    ClearTTreeVars();

    auto my_mcshower = (const event_mcshower*)(storage->get_data(DATA::MCShower)) ; 
    auto my_mctruth = (const event_mctruth*)(storage->get_data(DATA::MCTruth)) ;


	if(my_mcshower->size()!=2)
		return -1;

	std::cout<<"How often does this get entered? \n";
	_count++ ;
//	std::cout<<"Count is now : "<<_count<<std::endl;

	//first shower; this is the origin of the gamma probably? Not where 
	//it becomes visible 
    _vertex_X = my_mcshower->at(0).MotherPosition().at(0);
    _vertex_Y = my_mcshower->at(0).MotherPosition().at(1);
    _vertex_Z = my_mcshower->at(0).MotherPosition().at(2);

	//Smear around your x,y,z verticies 
    std::normal_distribution<double> distributionX(_vertex_X,1);
    std::normal_distribution<double> distributionY(_vertex_Y,1);
    std::normal_distribution<double> distributionZ(_vertex_Z,1);
	
	std::default_random_engine generatorX ;
//	std::default_random_engine generatorY ;
//	std::default_random_engine generatorZ ;

	double new_vertex_X = distributionX(generatorX) ;
	double new_vertex_Y = distributionY(generatorX) ;
	double new_vertex_Z	= distributionZ(generatorX) ;

	std::cout<<"Mother vertices: "<<_vertex_X<<", "<<_vertex_Y<<", "<<_vertex_Z<<std::endl;
	std::cout<<"Newer  vertices: "<<new_vertex_X<<", "<<new_vertex_Y<<", "<<new_vertex_Z<<std::endl;
	
	shower_energy = my_mcshower->at(0).MotherMomentum().at(3);
	
	//auto test  = my_mctruth->at(0).GetParticles().at(0).Trajectory().at(0).X();
	start_point_X = my_mctruth->at(0).GetParticles().at(0).Trajectory().at(0).X();
	start_point_Y = my_mctruth->at(0).GetParticles().at(0).Trajectory().at(0).Y();
	start_point_Z = my_mctruth->at(0).GetParticles().at(0).Trajectory().at(0).Z();

	auto daugh1_X = my_mcshower->at(0).DaughterPosition().at(0);
    auto daugh1_Y = my_mcshower->at(0).DaughterPosition().at(1);
    auto daugh1_Z = my_mcshower->at(0).DaughterPosition().at(2);
    auto daugh1_T = my_mcshower->at(0).DaughterPosition().at(3);

	auto daugh2_X = my_mcshower->at(1).DaughterPosition().at(0);
  auto daugh2_Y = my_mcshower->at(1).DaughterPosition().at(1);
  auto daugh2_Z = my_mcshower->at(1).DaughterPosition().at(2);
  auto daugh2_T = my_mcshower->at(1).DaughterPosition().at(3);

	std::cout<<"Daughter1 vertices: "<<daugh1_X<<", "<<daugh1_Y<<", "<<daugh1_Z<<", "<<daugh1_T<<std::endl;
//	std::cout<<"Daughter2 vertices: "<<daugh2_X<<", "<<daugh2_Y<<", "<<daugh2_Z<<", "<<daugh2_T<<std::endl;

	dist = pow((pow(_vertex_X - daugh1_X,2) + pow(_vertex_Y - daugh1_Y,2) + pow(_vertex_Z - daugh1_Z,2)),0.5);
	_new_dist = pow((pow(new_vertex_X - daugh1_X,2) + pow(new_vertex_Y - daugh1_Y,2) + pow(new_vertex_Z - daugh1_Z,2)),0.5);
	
	Mother_PDG = my_mcshower->at(0).MotherPDGID();
    std::cout<<"start point x, y, z : "<<start_point_X<<", "<<start_point_Y<<", "<<start_point_Z<<std::endl;
	std::cout<<"shower energy: "<<shower_energy <<std::endl;
//	std::cout<<"Mother PDG: "<<Mother_PDG<<std::endl;
	std::cout<<"DISTANCE! : "<<dist <<std::endl<<std::endl;

    if(_ana_tree)
      _ana_tree->Fill(); 

    return true;
  }

void vertex_test::PrepareTTree() {

    if(!_ana_tree) {
      _ana_tree = new TTree("ana_tree","");

	  _ana_tree->Branch("_new_dist",&_new_dist,"_new_dist/D");
	  _ana_tree->Branch("dist",&dist,"dist/D");
      _ana_tree->Branch("_vertex_X",&_vertex_X,"_vertex_X/D");
      _ana_tree->Branch("_vertex_Y",&_vertex_Y,"_vertex_Y/D");
      _ana_tree->Branch("_vertex_Z",&_vertex_Z,"_vertex_Z/D");

	  _ana_tree->Branch("shower_energy",&shower_energy,"shower_energy/D");

	  _ana_tree->Branch("start_point_X",&start_point_X,"start_point_X/D");
	  _ana_tree->Branch("start_point_Y",&start_point_Y,"start_point_Y/D");
	  _ana_tree->Branch("start_point_Z",&start_point_Z,"start_point_Z/D");

	  _ana_tree->Branch("PDG",&Mother_PDG,"PDG/I");

    }
  }

 void vertex_test::ClearTTreeVars(){

  }




  bool vertex_test::finalize() {

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
