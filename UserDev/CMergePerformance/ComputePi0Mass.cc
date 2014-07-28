#ifndef COMPUTEPI0MASS_CC
#define COMPUTEPI0MASS_CC

#include "ComputePi0Mass.hh"

namespace larlight {

  bool ComputePi0Mass::initialize() {

    _shower_type = DATA::Shower;
    
    ana_tree=0;
    PrepareTTree();

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
    
    
    //skip event if <2 showers are found
    //    if(ev_shower->size() < 2) return true;
    //for now, only consider events with ==2 showers. this std::map code makes it more complicated
    //but it's generalizable to use events with >2 showers
    if(ev_shower->size() != 2) return true;


    //if there are more than 2 showers, pick the two that have highest dedx in collection plane
    int first_shower_index = -1;
    int second_shower_index = -1;
    
    //find the indexes of the two showers with biggest dedxes
    //maps are auto sorted from low-to-high by first index
    std::map<double,unsigned int> dedx_index;
    
    int index = 0;
    for (auto const& c : *ev_shower) {
      dedx_index.insert(std::make_pair(c.dEdx().at(2),index));
      index++;
    }
    
    auto iter = (dedx_index).rbegin();
    
    //kaleko quick hack for ==2 showers
    //    first_shower_index = (*iter).second; //index of highest dedx;
    //    ++iter;
    //    second_shower_index = (*iter).second;//index of second highest dedx

    first_shower_index = 0;
    second_shower_index = 1;
    //    std::cout<<first_shower_index<<second_shower_index<<std::endl;

    _mass = Pi0MassFormula3D(ev_shower->at(first_shower_index).Energy().at(2),
			     ev_shower->at(second_shower_index).Energy().at(2),
			     ev_shower->at(first_shower_index).Direction(),
			     ev_shower->at(second_shower_index).Direction());



    //ttree variables
    _min_shower_energy_plane0 = std::min(ev_shower->at(first_shower_index).Energy().at(0),ev_shower->at(second_shower_index).Energy().at(0));
    _max_shower_energy_plane0 = std::max(ev_shower->at(first_shower_index).Energy().at(0),ev_shower->at(second_shower_index).Energy().at(0));
    _min_shower_dedx_plane0 = std::min(ev_shower->at(first_shower_index).dEdx().at(0),ev_shower->at(second_shower_index).dEdx().at(0));
    _max_shower_dedx_plane0 = std::max(ev_shower->at(first_shower_index).dEdx().at(0),ev_shower->at(second_shower_index).dEdx().at(0));


    _min_shower_energy_plane1 = std::min(ev_shower->at(first_shower_index).Energy().at(1),ev_shower->at(second_shower_index).Energy().at(1));
    _max_shower_energy_plane1 = std::max(ev_shower->at(first_shower_index).Energy().at(1),ev_shower->at(second_shower_index).Energy().at(1));
    _min_shower_dedx_plane1 = std::min(ev_shower->at(first_shower_index).dEdx().at(1),ev_shower->at(second_shower_index).dEdx().at(1));
    _max_shower_dedx_plane1 = std::max(ev_shower->at(first_shower_index).dEdx().at(1),ev_shower->at(second_shower_index).dEdx().at(1));


    _min_shower_energy_plane2 = std::min(ev_shower->at(first_shower_index).Energy().at(2),ev_shower->at(second_shower_index).Energy().at(2));
    _max_shower_energy_plane2 = std::max(ev_shower->at(first_shower_index).Energy().at(2),ev_shower->at(second_shower_index).Energy().at(2));
    _min_shower_dedx_plane2 = std::min(ev_shower->at(first_shower_index).dEdx().at(2),ev_shower->at(second_shower_index).dEdx().at(2));
    _max_shower_dedx_plane2 = std::max(ev_shower->at(first_shower_index).dEdx().at(2),ev_shower->at(second_shower_index).dEdx().at(2));



    //fill main histogram
    hPi0MassPeak->Fill(_mass);
    
    //fill ana_tree
    ana_tree->Fill();

    return true;


  }

  bool ComputePi0Mass::finalize() {


    if(_fout) { 
      _fout->cd(); 
      hPi0MassPeak->Write();
      ana_tree->Write();
      delete hPi0MassPeak;
      delete ana_tree;
    }
    else 
      print(MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
    
  
    return true;
  }


  //Get PI0 Mass from photon directions and energy
  float ComputePi0Mass::Pi0MassFormula3D( float Energy1, float Energy2, TVector3 Direction3D_1, TVector3 Direction3D_2){
    

    float kazu_correction_factor_RecoShowerE_to_TrueShowerE = 1.25*1.08;
    //    float kazu_correction_factor_RecoShowerE_to_TrueShowerE = 1.;

    float angle_3D = acos( Direction3D_1 * Direction3D_2 );

    float E1 = Energy1*kazu_correction_factor_RecoShowerE_to_TrueShowerE;
    float E2 = Energy2*kazu_correction_factor_RecoShowerE_to_TrueShowerE;

    //using a formula from 
    //http://www.hep.princeton.edu/~mcdonald/examples/piondecay.pdf      
    return pow(4. * E1 * E2 * pow(sin( 0.5 * angle_3D ),2) , 0.5);

  }

  void ComputePi0Mass::PrepareTTree() {

    if(!ana_tree) {
      ana_tree = new TTree("ana_tree","");
            
      ana_tree->Branch("pi0mass",&_mass,"pi0mass/D");

      ana_tree->Branch("min_shower_energy_plane0",&_min_shower_energy_plane0,"min_shower_energy_plane0/D");
      ana_tree->Branch("max_shower_energy_plane0",&_max_shower_energy_plane0,"max_shower_energy_plane0/D");
      ana_tree->Branch("min_shower_dedx_plane0",&_min_shower_dedx_plane0,"min_shower_dedx_plane0/D");
      ana_tree->Branch("max_shower_dedx_plane0",&_max_shower_dedx_plane0,"max_shower_dedx_plane0/D"); 


      ana_tree->Branch("min_shower_energy_plane1",&_min_shower_energy_plane1,"min_shower_energy_plane1/D");
      ana_tree->Branch("max_shower_energy_plane1",&_max_shower_energy_plane1,"max_shower_energy_plane1/D");
      ana_tree->Branch("min_shower_dedx_plane1",&_min_shower_dedx_plane1,"min_shower_dedx_plane1/D");
      ana_tree->Branch("max_shower_dedx_plane1",&_max_shower_dedx_plane1,"max_shower_dedx_plane1/D"); 


      ana_tree->Branch("min_shower_energy_plane2",&_min_shower_energy_plane2,"min_shower_energy_plane2/D");
      ana_tree->Branch("max_shower_energy_plane2",&_max_shower_energy_plane2,"max_shower_energy_plane2/D");
      ana_tree->Branch("min_shower_dedx_plane2",&_min_shower_dedx_plane2,"min_shower_dedx_plane2/D");
      ana_tree->Branch("max_shower_dedx_plane2",&_max_shower_dedx_plane2,"max_shower_dedx_plane2/D"); 
    }
  }

}
#endif
