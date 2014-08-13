#ifndef MCINFO_CC
#define MCINFO_CC

#include "MCinfo.hh"
#include "math.h"

namespace larlight {

  bool MCinfo::initialize() {
    
    _evtN = 0;

    outFile.open("outData.txt", std::ofstream::out | std::ofstream::app);
    outFile << "gEpre\tgEpost\teE\tgAngle\teAngle\n";

    eEnergy = new TH1D("ComptonElectronEnergy", "Energy of Compton Electron; e- E [GeV]; Count", 100, 0, 0.22);
    eAngle = new TH1D("ComptonElectronAngle", "Angle of Compton Electron; e- Angle [deg]; Count", 100, 0, 360);
    gEnergypre = new TH1D("ComptonPhotonEnergyPre", "Energy of Compton Photon - Before Int.; gamma E [GeV]; Count", 100, 0, 0.22);
    gEnergypost = new TH1D("ComptonPhotonEnergyPost", "Energy of Compton Photon - After Int.; gamma E [GeV]; Count", 100, 0, 0.22);
    gAngle = new TH1D("ComptonPhotonAngle", "Angle of Compton Photon; gamma Angle [deg]; Count", 100, 0, 360);
    eEvseAngle = new TH2D("eEvseAngle", "e- Energy vs e Angle; Angle [deg]; e- E [GeV]", 100, 0, 360, 100, 0, 0.22);
    eEvsgAngle = new TH2D("eEvsgAngle", "e- Energy vs gamma Angle; Angle [deg]; e- E [GeV]", 100, 0, 360, 100, 0, 0.22);
    gEvsgAngle = new TH2D("gEvsgAngle", "gamma Energy vs gamma Angle; Angle [deg]; gamma E [GeV]", 100, 0, 360, 100, 0, 0.22);

    std::cout << "Acosine:" << std::endl;
    for (int x=0; x < 100; x++){
      double z = -1.+x/50.;
      std::cout << "pz/z: " << z << "\tacos: " << acos(z)*180/3.14 << std::endl;
    }

    SetVerbose(false);

    return true;
  }
  
  bool MCinfo::analyze(storage_manager* storage) {

    if (_verbose) { std::cout << "Event Number: " << _evtN << std::endl; }
    
    const event_mcpart *event_part = (event_mcpart*)(storage->get_data(DATA::MCParticle));

    //Get Photon
    mcpart phot = event_part->at(0);
    mctrajectory photTraj = phot.Trajectory();
    if (_verbose) {
      std::cout << "Photon directions: " << std::endl;
      std::cout << "\tStep\tE\tAngle" << std::endl;
      for (size_t j=0; j < photTraj.size(); j++){
	mcstep s = photTraj.at(j);
	Double_t dir = (180/3.14) * acos ( ( (s.Pz()) / sqrt( (pow(s.Px(),2) + pow(s.Py(),2) + pow(s.Pz(),2)) ) ) );  
	std::cout << "\t" << j << "\t" << s.E() << "\t" << dir << std::endl;
      }
      std::cout << std::endl;
    }
    
    //Loop over all particles in event
    for (size_t i=0; i<event_part->size(); i++){

      mcpart particle = event_part->at(i);
      
      //10 MeV threshold
      if ( (particle.Trajectory().at(0).E() > 0.01) and 
	   (particle.Process() == "compt") and 
	   (particle.Mother() == 1) ){
      //      if ( particle.Trajectory().at(0).E() > 0.01 ){
	mctrajectory traj = particle.Trajectory();
	mcstep step1 = traj.at(0);
	Double_t angle = (180/3.14) * acos ( ( (step1.Pz()) /
					       sqrt( (pow(step1.Px(),2) + pow(step1.Py(),2) + pow(step1.Pz(),2)) ) ) );  
	//TLorentzVector mom = traj.Momentum();
	if (_verbose){
	  std::cout << "\tTrackID: " << particle.TrackId() << std::endl;
	  std::cout << "\tMother: " << particle.Mother() << std::endl;
	  std::cout << "\tParticle PDG: " << particle.PdgCode() << std::endl;
	  std::cout << "\tParticle Process: " << particle.Process() << std::endl;
	  std::cout << "\tEnergy: " << step1.E() << std::endl;
	  std::cout << "\tAngle: " << angle << std::endl;
	  std::cout << std::endl;
	}
	eEnergy->Fill( step1.E() );
	eAngle->Fill( angle );
	gEnergypre->Fill( photTraj.at(0).E() );
	gEnergypost->Fill( photTraj.at(1).E() );
	mcstep gStep1 = photTraj.at(1);
	Double_t gangle = (180/3.14) * acos ( ( (gStep1.Pz()) /
						sqrt( (pow(gStep1.Px(),2) + pow(gStep1.Py(),2) + pow(gStep1.Pz(),2)) ) ) );  
	gAngle->Fill( gangle );
	eEvseAngle->Fill( angle, step1.E() );
	eEvsgAngle->Fill( angle, photTraj.at(1).E() );
	gEvsgAngle->Fill( gangle, photTraj.at(1).E() );
	outFile << photTraj.at(0).E() << "\t" << photTraj.at(1).E() << "\t" << step1.E()
		<< "\t" << gangle << "\t " << angle << "\n";
	
      }
    }//End for each particle
    
    _evtN += 1;

    return true;
    
  }//End Analyze Loop
  
  bool MCinfo::finalize() {

    outFile.close();

    eEnergy->Write();
    eAngle->Write();
    gEnergypre->Write();
    gEnergypost->Write();
    gAngle->Write();
    eEvseAngle->Write();
    eEvsgAngle->Write();
    gEvsgAngle->Write();
    
    return true;
  }
}
#endif
