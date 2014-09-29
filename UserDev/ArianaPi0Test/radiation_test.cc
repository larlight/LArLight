#ifndef RADIATION_TEST_CC
#define RADIATION_TEST_CC

#include "radiation_test.hh"
#include "radiation_information.hh"
#include "math.h"
#include <random>

namespace larlight {

  bool radiation_test::initialize() {

	 h1= new TH1D("h1","Energy containment; energy containment; number pi0s",100,0,1);

    _ana_tree=0;
    PrepareTTree();

    return true;
  }
  
  bool radiation_test::analyze(storage_manager* storage) {

	Clear();

    auto my_mcshower = (const event_mcshower*)(storage->get_data(DATA::MCShower)) ;

    if(my_mcshower->size()!=2)
        return -1;

    //First shower; this is the origin of the gamma--not where 
    //it becomes visible 
    _vertex_X = my_mcshower->at(0).MotherPosition().at(0);
    _vertex_Y = my_mcshower->at(0).MotherPosition().at(1);
    _vertex_Z = my_mcshower->at(0).MotherPosition().at(2);

	
	//4 momentum of first daughter gamma
    auto _daugh1_Px = my_mcshower->at(0).DaughterMomentum().at(0);
    auto _daugh1_Py = my_mcshower->at(0).DaughterMomentum().at(1);
    auto _daugh1_Pz = my_mcshower->at(0).DaughterMomentum().at(2);
	auto _daugh1_E = my_mcshower->at(0).DaughterMomentum().at(3);

	//4 momentum of second daughter gamma
    auto _daugh2_Px = my_mcshower->at(1).DaughterMomentum().at(0);
    auto _daugh2_Py = my_mcshower->at(1).DaughterMomentum().at(1);
    auto _daugh2_Pz = my_mcshower->at(1).DaughterMomentum().at(2);
    auto _daugh2_E = my_mcshower->at(1).DaughterMomentum().at(3);

	mother1_E =  my_mcshower->at(0).MotherMomentum().at(3) ;	
	mother2_E =  my_mcshower->at(1).MotherMomentum().at(3) ;	

	//Calculate energy containment
	double energy1 = _daugh1_E/ my_mcshower->at(0).MotherMomentum().at(3) ; 
	double energy2 = _daugh2_E/ my_mcshower->at(1).MotherMomentum().at(3); 

	double Pmag_d1 = pow( (pow(_daugh1_Px,2) + pow(_daugh1_Py,2) + pow(_daugh1_Pz,2) ),0.5); 
	double Pmag_d2 = pow( (pow(_daugh2_Px,2) + pow(_daugh2_Py,2) + pow(_daugh2_Pz,2) ),0.5); 

	//For pi0 mass--use daughter momenta before they are normalized below 
	double pi0_4dot_prod = -2* ( _daugh1_Px * _daugh2_Px + _daugh1_Py * _daugh2_Py 
		 	 +  _daugh1_Pz * _daugh2_Pz - _daugh1_E * _daugh2_E ) ; 
  
	//Baseline pi0-- calculate for all events with no cuts
	_new_pi0_mass = pow(pi0_4dot_prod,0.5); 

	//Calculate angle between the pi0s 
	double pi0_dot_prod =   ( _daugh1_Px * _daugh2_Px + _daugh1_Py * _daugh2_Py 
				 	 +  _daugh1_Pz * _daugh2_Pz  ) ; 
	pi0_dot_prod /= (Pmag_d1*Pmag_d2) ;
	
	_pi0_angle = acos ( pi0_dot_prod) ; 
	_pi0_angle *= 180/ TMath::Pi() ;
//	std::cout<<"Angle between showers is : "<<_pi0_angle
//			<<" with energy ratios: "<<energy1<<", "<<energy2<<std::endl;


	//Normalize to get directionality for radiation length test
	_daugh1_Px /= Pmag_d1 ; 
	_daugh1_Py /= Pmag_d1 ; 
	_daugh1_Pz /= Pmag_d1 ; 

	_daugh2_Px /= Pmag_d2 ; 
	_daugh2_Py /= Pmag_d2 ; 
	_daugh2_Pz /= Pmag_d2 ; 

	//Calculate theta and phi from directionality of momenta
	double theta_d1, phi_d1 ;
	double theta_d2, phi_d2 ;
	
	theta_d1 = asin( _daugh1_Py ); 
	phi_d1 = asin( _daugh1_Px / cos (theta_d1) ) ;

	theta_d2 = asin( _daugh2_Py ); 
	phi_d2 = asin( _daugh2_Px / cos (theta_d2) ) ;

//Calculate distance to the wall so that we don't need radiation length
	std::vector<double> daugh1(3,0);
	daugh1[0] = _daugh1_Px ;
	daugh1[1] = _daugh1_Py ;
	daugh1[2] = _daugh1_Pz ;

	std::cout<<"Distance to wall from vertex in dir of showers 1 and 2: "
<<DistanceToWall(my_mcshower->at(0).DaughterPosition(),daugh1) << "\n" ; //<<", " 
//	<<DistanceToWall(my_mcghterMomentum()/Pmag_d2,my_mcshower->at(1).MotherPosition())<<std::endl ;


  //rad_lengths is the projected length of shower along direction of
  //daughter momentum. Initialize it to 1 radiation length in LAr
  double rad_lengths = _LAr_radLength; 

  bool inDetector = true;
 _energy_containment = energy1 * energy2 ;

	//These are to test whether the events near 0 in energy containment plot
	//are due to daughter vertex being near edge of detector/outside it
    auto _Dvertex_X = my_mcshower->at(0).DaughterPosition().at(0);
    auto _Dvertex_Y = my_mcshower->at(0).DaughterPosition().at(1);
    auto _Dvertex_Z = my_mcshower->at(0).DaughterPosition().at(2);


 while(inDetector){	

	//recalculate coordinates for each interger multiple of rad length
	x_new1 = rad_lengths * cos(theta_d1) * sin(phi_d1) ;
	y_new1 = rad_lengths * sin(theta_d1) ;
	z_new1 = rad_lengths * cos(theta_d1) * cos(phi_d1) ; 

	x_new2= rad_lengths * cos(theta_d2) * sin(phi_d2) ;
	y_new2 = rad_lengths * sin(theta_d2) ;
	z_new2 = rad_lengths * cos(theta_d2) * cos(phi_d2) ; 
	
	x_new1 += _vertex_X ;
	y_new1 += _vertex_Y ;
	z_new1 += _vertex_Z ;
	
	x_new2 += _vertex_X ;
	y_new2 += _vertex_Y ;
	z_new2 += _vertex_Z ;

	//the distance from vertex should always be inside detector 
    if(x_new1 > 0  && x_new1 < 256  && y_new1 > -115  && y_new1 < 115 && z_new1 > 0 && z_new1 < 1037 
  	 && x_new2 > 0  && x_new2 < 256  && y_new2 > -115  && y_new2 < 115 && z_new2 > 0 && z_new2 < 1037){
		if(rad_lengths == _LAr_radLength){
			count1++; 
			_pi0_mass_1length.push_back(_new_pi0_mass); 
			_Econ1.push_back(_energy_containment) ;
		  }
		else if(rad_lengths == 2*_LAr_radLength ){ 
			count2++;
			_pi0_mass_2lengths.push_back(_new_pi0_mass); 
			_Econ2.push_back(_energy_containment) ;
			}
		else if(rad_lengths == 3*_LAr_radLength ){
			count3++;
			_pi0_mass_3lengths.push_back(_new_pi0_mass);
			_Econ3.push_back(_energy_containment) ;
		  }
		else if(rad_lengths == 4*_LAr_radLength){
			count4++;
			_pi0_mass_4lengths.push_back(_new_pi0_mass);
			_Econ4.push_back(_energy_containment) ;
		  }	
		else if(rad_lengths == 5*_LAr_radLength){
			count5++;
			_pi0_mass_5lengths.push_back(_new_pi0_mass);
			_Econ5.push_back(_energy_containment) ;
			if(_energy_containment < 0.05){
  			MinDistVtxToBoundary(_Dvertex_X,_Dvertex_Y,_Dvertex_Z,x_dist,y_dist, z_dist) ;
			v_x_dist.push_back(x_dist);
			v_y_dist.push_back(y_dist);
			v_z_dist.push_back(z_dist);
			 }
		/*	if(_energy_containment < 0.05 ){
			std::cout<<"\nENERGY CONT: "<<energy1*energy2 ;
			std::cout<<"\nVertex: "<<_vertex_X<<", "<<_vertex_Y<<", "<<_vertex_Z ;
			std::cout<<"\nxnew 1 and 2 :"<<x_new1<<", "<<y_new1<<", "<<z_new1
						<<"\n\t\t"<<x_new2<<", "<<y_new2<<", "<<z_new2  ;
			std::cout<<"\ndaughter and mother 1 energy: \n" <<_daugh1_E <<", \n"<<my_mcshower->at(0).MotherMomentum().at(3) <<std::endl;
			std::cout<<"daughter and mother 2 energy: \n" <<_daugh2_E <<", \n"<<my_mcshower->at(1).MotherMomentum().at(3) <<std::endl;
				}*/
			}
		else{
			count6++;
			_pi0_mass_morelengths.push_back(_new_pi0_mass);
			_Econ_more.push_back(_energy_containment) ;
//			std::cout<<"\n\nradiation x,y,z new1: "<<x_new1<<", "<<y_new1<<", "<<z_new1 <<std::endl;
//			std::cout<<"radiation x,y,z new2: "<<x_new2<<", "<<y_new2<<", "<<z_new2 <<std::endl;
//		    std::cout<<"vertex1 x, y, z : "<<_vertex_X<<", "<<_vertex_Y<<", "<<_vertex_Z<<std::endl;
			inDetector= false;
			}

		} 

	else
		inDetector = false ;


	rad_lengths += _LAr_radLength ;
  }

	count0++;
	_Econ0.push_back(_energy_containment);

	_pi0_mass_wo_cut.push_back(_new_pi0_mass)  ;

   if(_ana_tree)
      _ana_tree->Fill();

    return true;
  }

void radiation_test::PrepareTTree() {

    if(!_ana_tree) {
      _ana_tree = new TTree("ana_tree","");

	  _ana_tree->Branch("v_x_dist","std::vector<double>",&v_x_dist);
	  _ana_tree->Branch("v_y_dist","std::vector<double>",&v_y_dist);
	  _ana_tree->Branch("v_z_dist","std::vector<double>",&v_z_dist);

	  _ana_tree->Branch("x_dist",&x_dist,"x_dist/D");
	  _ana_tree->Branch("y_dist",&y_dist,"y_dist/D");
	  _ana_tree->Branch("z_dist",&z_dist,"z_dist/D");
	  _ana_tree->Branch("mother1_E",&mother1_E,"mother1_E/D"); 
	  _ana_tree->Branch("mother2_E",&mother2_E,"mother2_E/D"); 
	  _ana_tree->Branch("_Econ0","std::vector<double>",&_Econ0);
	  _ana_tree->Branch("_Econ1","std::vector<double>",&_Econ1);
	  _ana_tree->Branch("_Econ2","std::vector<double>",&_Econ2);
	  _ana_tree->Branch("_Econ3","std::vector<double>",&_Econ3);
	  _ana_tree->Branch("_Econ4","std::vector<double>",&_Econ4);
	  _ana_tree->Branch("_Econ5","std::vector<double>",&_Econ5);
	  _ana_tree->Branch("_Econ_more","std::vector<double>",&_Econ_more);

	  _ana_tree->Branch("_pi0_mass_wo_cut","std::vector<double>",&_pi0_mass_wo_cut);
	  _ana_tree->Branch("_pi0_mass_1length","std::vector<double>",&_pi0_mass_1length);
	  _ana_tree->Branch("_pi0_mass_2lengths","std::vector<double>",&_pi0_mass_2lengths);
	  _ana_tree->Branch("_pi0_mass_3lengths","std::vector<double>",&_pi0_mass_3lengths);
	  _ana_tree->Branch("_pi0_mass_4lengths","std::vector<double>",&_pi0_mass_4lengths);
	  _ana_tree->Branch("_pi0_mass_5lengths","std::vector<double>",&_pi0_mass_5lengths);
	  _ana_tree->Branch("_pi0_mass_morelengths","std::vector<double>",&_pi0_mass_morelengths);

	  _ana_tree->Branch("_pi0_angle",&_pi0_angle,"_pi0_angle/D");

      _ana_tree->Branch("vertex_X",&_vertex_X,"_vertex_X/D");
      _ana_tree->Branch("vertex_Y",&_vertex_Y,"_vertex_Y/D");
      _ana_tree->Branch("vertex_Z",&_vertex_Z,"_vertex_Z/D");


    }
 }

void radiation_test::MinDistVtxToBoundary(const double x,const double y,const double z, double& x_dist, double& y_dist, double& z_dist){

 if(x > 256.35 || x < 0 || y > 116.5 || y < -116.5 || z <0 || z >1036.8 ){
		x_dist = -999 ;
		y_dist = -999 ;
		z_dist = -999 ; 
		std::cout<<"\nOUTSIDE DETECTOR! \n"; 
		}
 else{	
	double x1_dist = 256.35 - x ;
	double y1_dist = 0 ;
	double z1_dist = 1036.8 - z ; 

 	if(y >= 0 )  
		y1_dist = 116.5 - y ;
	 else
		y1_dist = -116.5 - y ;	


	if(x1_dist > x )
		x_dist = x ;
	else
		x_dist = x1_dist ; 

		y_dist = y1_dist ;

	if(z1_dist > z )
		z_dist = z;
	else
		z_dist = z1_dist  ;
	}
}

double radiation_test::DistanceToWall(std::vector<double> const& point,
      	        std::vector<double> const& dir) {

//	std::cout<<"\nPoint 0 1 2: "<< point[0]<<", "<<point[1]<<", "<<point[2] <<std::endl;
//	std::cout<<"Dir  : "<<dir[0]<<", "<<dir[1]<<", "<<dir[2]<<std::endl;
	
	//Calculate theta and phi from directionality of momenta
	double theta, phi ;
	
	theta = asin( dir[1] ); 
	phi = asin( dir[0] / cos (theta) ) ;

	double r = 1; 
	double x_new, y_new, z_new ;

	x_new = r * cos(theta) * sin(phi) ;
	y_new = r * sin(theta) ;
	z_new = r * cos(theta) * cos(phi) ; 
		
	double z_dist ;	

	if(z_new > 0)
		z_dist = 1036.8 - point[2] ;
	else
		z_dist = point[2] - 1036.8; 
		

		std::cout<<"\nz_new nad point [2 ] " << z_new<<", "<<point[2]<<std::endl;
		std::cout<<"Z dist is  : "<<z_dist <<std::endl ;
		r = z_dist / cos(theta) / cos(phi) ;
	return r ;
}



void radiation_test::Clear(){

	v_x_dist.clear();
	v_y_dist.clear();
	v_z_dist.clear();

	_Econ0.clear();
	_Econ1.clear();
	_Econ2.clear();
	_Econ3.clear();
	_Econ4.clear();
	_Econ5.clear();
	_Econ_more.clear();
	
	_pi0_mass_wo_cut.clear();
	_pi0_mass_1length.clear();
	_pi0_mass_2lengths.clear();
	_pi0_mass_3lengths.clear();
	_pi0_mass_4lengths.clear();
	_pi0_mass_5lengths.clear();
	_pi0_mass_morelengths.clear();

	}


bool radiation_test::finalize() {
	if(_fout) {

      _fout->cd();
      if(_ana_tree)
    _ana_tree->Write();
	

    }
    else
      print(larlight::MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");

    delete _ana_tree;
	std::cout<<"counts 0-5 : "<<count0<<", "<<count1<<", "<<count2<<", "<<count3
							 <<", \n\t\t"<<count4<<", "<<count5<<", "<<count6<<"\n"<<std::endl;


    return true;
  }

}
#endif
