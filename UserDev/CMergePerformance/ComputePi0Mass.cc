#ifndef COMPUTEPI0MASS_CC
#define COMPUTEPI0MASS_CC

#include "ComputePi0Mass.hh"

namespace larlight {

  bool ComputePi0Mass::initialize() {

    _shower_type = DATA::Shower;

    hPi0MassPeak = new TH1D("hPi0MassPeak","Pi0 Mass Peak in MeV",100,0,300);

    return true;
  }
  
  bool ComputePi0Mass::analyze(storage_manager* storage) {

    // Load the Showers... need to run shower reconstruction first!
    auto ev_shower = (const event_shower*)(storage->get_data(_shower_type));
    if(!ev_shower) {
      print(MSG::ERROR,__FUNCTION__,Form("Data product \"%s\" not found...",
					 DATA::DATA_TREE_NAME[_shower_type].c_str()));
      return false;
    }else if(ev_shower->size()<1) return false;
    
    /*
    if(ev_shower->size()==2){
      std::cout<<std::endl; std::cout<<std::endl;
      std::cout<<std::endl; std::cout<<std::endl;
      std::cout<<std::endl; std::cout<<std::endl;
      std::cout<<std::endl; std::cout<<std::endl;
      std::cout<<"this event has "<<ev_shower->size()<<" showers."<<std::endl;
      std::cout<<std::endl; std::cout<<std::endl;
      std::cout<<std::endl; std::cout<<std::endl;
      std::cout<<std::endl; std::cout<<std::endl;
      std::cout<<std::endl; std::cout<<std::endl;
    }
    */
    
    //skip event if !2 showers found.
    if(ev_shower->size() != 2) return true;
    
    /*
    if(_debug){
      std::cout<<Form("shower1 energy in each plane is (%f, %f, %f), direction is (%f, %f, %f)\n",
      		      ev_shower->at(0).Energy().at(0),
      		      ev_shower->at(0).Energy().at(1),
      		      ev_shower->at(0).Energy().at(2),
      		      ev_shower->at(0).Direction().X(),
      		      ev_shower->at(0).Direction().Y(),
      		      ev_shower->at(0).Direction().Z()
      		      );
    }
    std::cout<<"flag2"<<std::endl;
    

    //then use that shower object to calculate the pi0 mass.
    //for now just always use energy from plane 2. need to figure out how to use best plane
    std::cout<<"energy size is "<<ev_shower->at(0).Energy().size()<<std::endl;
    std::cout<<Form("Direction is (%f, %f, %f)\n",
		    ev_shower->at(0).Direction().X(),
		    ev_shower->at(0).Direction().Y(),
		    ev_shower->at(0).Direction().Z());
    */

    
    float mass = Pi0MassFormula3D(ev_shower->at(0).Energy().at(2),
				  ev_shower->at(1).Energy().at(2),
				  ev_shower->at(0).Direction(),
				  ev_shower->at(1).Direction());
    /*
    std::cout<<std::endl;
    std::cout<<std::endl;
    std::cout<<std::endl;
    std::cout<<"pi0 mass is "<<mass<<"!"<<std::endl;
    std::cout<<std::endl;
    std::cout<<std::endl;
    std::cout<<std::endl;
    //now add pi0 mass to histo
    */
    hPi0MassPeak->Fill(mass);
    
    return true;


  }

  bool ComputePi0Mass::finalize() {

    if(_fout) { 
      _fout->cd(); 
      hPi0MassPeak->Write();
      delete hPi0MassPeak;
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
