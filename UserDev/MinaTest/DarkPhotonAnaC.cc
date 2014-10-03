#ifndef DARKPHOTONANAC_CC
#define DARKPHOTONANAC_CC

#include "DarkPhotonAnaC.hh"
#include "LineDist.hh"

struct PhotonPair{ 
  MinaTestParticle part1;
  MinaTestParticle part2;
  double dist;
};

double DarkPhotonAnaC::RunPOTLoop(TTree* tree)
{
  std::cout << "In POT loop" << std::endl;
  
  //Check whether pot branch exists
  if(!(tree->GetBranch("Toy_POT_branch"))) {

    std::cerr
      << "\033[93m" 
      << "Input TTree has no branch called \"Toy_POT_branch\"!" 
      << "\033[00m" << std::endl;
    return 0.;
  }

  double* pot = new double;
  tree->SetBranchAddress("Toy_POT_branch",&pot);
  
  double TotalPOT = 0.;

   for(size_t i=0; i<tree->GetEntries(); ++i) {
     tree->GetEntry(i);

     TotalPOT += *pot;
     
   }
   return TotalPOT;
}
//struct PhotonGroup : std::vector<MinaTestParticle> {};

void DarkPhotonAnaC::RunEventLoop(TTree* tree)
{
  //
  // This function runs an event loop on input TTree and attempts to read MinaTestEvent data product
  //

  std::cout.precision(4);
  std::cout << "In event loop" << std::endl;
 
  // Step 1) check if MinaTestEvent branch exists
  if(!(tree->GetBranch("Toy_event_branch"))) {

    std::cerr
      << "\033[93m" 
      << "Input TTree has no branch called \"Toy_event_branch\"!" 
      << "\033[00m" << std::endl;
    return;
  }
  
  // Step 2) add a handle to read-out data
  MinaTestEvent* mina_test_event = new MinaTestEvent;
  tree->SetBranchAddress("Toy_event_branch",&mina_test_event);
 
  // Definition of output TTree!
  TFile* fout = TFile::Open("DarkPhotonAnalyzed_Cosmics.root", "RECREATE");
  //TH1D* fPDGCodeHist = new TH1D("pdgcodes",";PDG Code;", 5000, -2500, 2500);
  //Histograms for ALL particles in the events we're looking at.
  TH1D* fPhotonCountHist = new TH1D("fphotoncount",";Number of Photons;", 50, 0, 50);
  TH1D* fElectronCountHist = new TH1D("felectroncount", ";Number of Electrons;", 10, 0, 10);
  TH1D* fEnergyHist = new TH1D("fphotonenergy",";Photon Energy (MeV);",1000,0,1000);
  TH1D* fMultiPhotonHist = new TH1D("fphotongroup","Number of Photons in Area Groups", 50, 0, 50);
  TH1D* fPhotonDistHist = new TH1D("fphotondist",";Distances between Photons (cm);",5000,0,500);
  //TH1D* fPhoton_ElectronHist = new TH1D("fphotonsWelectron","# of Photons with One Electron", 50, 0, 50);
  TH1D* fPhotonMotherHist = new TH1D("fphotonmotherTID",";TrackID of Photon Mother;",500,0,500);
  TH1D* fMotherPDGHist = new TH1D("fphotonmotherPDG",";PDG of Photon Mother;",5000, -2500, 2500);
  TH1D* fTrackLengthHist = new TH1D("fphotontracklength",";Photon Track Length (cm);", 500, 0, 500);

  //Histogram for different cases.
  TH1D* fCaseHist = new TH1D("fCaseHist",";Number of Different Cases;",10,0,5);

  //Histograms made for the different cases
  //CASE A: 3 or more photons with at least one photon being primary
  TH1D* A_Dist = new TH1D("Adist","All Photon Distance (cm) CASE A", 5000,0,500);
  TH1D* A_MPDG = new TH1D("AmotherPDG","All Photon Mother PDG CASE A", 5000, -2500, 2500);
  TH1D* A_Energy = new TH1D("Aenergy", "All Photon Energy (MeV) CASE A", 1000, 0, 1000);
  TH1D* A_Count = new TH1D("Acount", "Total Photons in Event CASE A", 50, 0, 50);
  TH1D* A_Primary = new TH1D("Aprimary", "Number of Primary Photons CASE A", 50, 0, 50);

  TH1D* B_Dist_Total = new TH1D("BdistTotal","All Photon Distance (cm) CASE B", 5000,0,500);
  TH1D* B_Dist_Pi0Photons = new TH1D("BdistPi0Photons","Photon with Pi0 as Mother Distance (cm) CASE B", 5000,0,500);
  TH1D* B_Dist_OtherPhotons = new TH1D("BdistOtherPhotons","Other Photon Distance (cm) CASE B", 5000,0,500);
  TH1D* B_MPDG = new TH1D("BmotherPDG","Photon Mother PDG CASE B", 5000, -2500, 2500);
  TH1D* B_Energy_Pi0Photons = new TH1D("BenergyPi0Photons", "Photon Energy (MeV) with Pi0 as Mother CASE B", 1000, 0, 1000);
  TH1D* B_Energy_OtherPhotons = new TH1D("BenergyOtherPhotons", "Other Photon Energy (MeV) CASE B", 1000, 0, 1000);
  TH1D* B_Count = new TH1D("Bcount", "Total Photons CASE B", 50, 0, 50);
  TH1D* B_pi0 = new TH1D("Bpi0", "Number of Photons with Pi0 as Mother CASE B", 50, 0, 50);

  TH1D* C_Dist = new TH1D("Cdist","Photon Distance (cm) CASE C", 5000,0,500);
  TH1D* C_MPDG = new TH1D("CmotherPDG","Photon Mother PDG CASE C", 5000, -2500, 2500);
  TH1D* C_Energy = new TH1D("Cenergy", "Photon Energy (MeV) CASE C", 1000, 0, 1000);
  TH1D* C_Count = new TH1D("Ccount", "Total Photons CASE C", 50, 0, 50);

  TH1D* N_Dist = new TH1D("Ndist","Pure Nuclear Scattering Photon Distance (cm)", 5000,0,500);
  TH1D* N_MPDG = new TH1D("NmotherPDG","Pure Nuclear Scattering Photon Mother PDG", 5000, -2500, 2500);
  TH1D* N_Energy = new TH1D("Nenergy", "Pure Nuclear Scattering Photon Energy (MeV)", 1000, 0, 1000);
  TH1D* N_Count = new TH1D("Ncount", "Total Photons in Pure Nuclear Scattering Events", 50, 0, 50);
  // Step 3) start event loop

  int ThreeOrMore = 0;
  int ExactlyThree = 0;
  int ThreePrimary = 0;
  int EnergyPiCut = 0;
  int EnergyPiCut3 = 0;
  int EnergyNCut = 0;

  for(size_t i=0; i<tree->GetEntries(); ++i) {

    //std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    //std::cout << "TTree entry: " << i << std::endl;
    tree->GetEntry(i);

    //std::cout << "  Found " << mina_test_event->size() << " particles!" << std::endl;
    //std::cout << "  Listing PDG ID of particles..." << std::flush;

    //std::cout << " " << std::endl;
    
    int photon_count = 0;
    int e_count = 0;
    int primary_count = 0;
    int pi0_mother_count = 0;
    int non_nuclear_count = 0;
    int energy_pi_count = 0;
    int energy_n_count = 0;

    std::vector<MinaTestParticle> Pho_;
    Pho_.clear();

    for(auto const& mina_test_particle : *mina_test_event) {
      
//      std::cout << mina_test_particle.fPDG() << " ";
      if (mina_test_particle.fPDG() == 22) { 
	++photon_count; //Count the number of photons in the event
	fEnergyHist->Fill(mina_test_particle.fEnergy());
//	std::cout << "Photon Track ID: " << mina_test_particle.fTrackId() << std::endl;
//	std::cout << "Photon Energy (MeV): " << std::fixed << mina_test_particle.fEnergy() << std::endl;
//	std::cout << 3.14159 << std::endl;
//	std::cout << "Photon Mother Track ID: " << mina_test_particle.fMotherTID() << std::endl;
//	std::cout << "Photon Mother PDG: " << mina_test_particle.fMotherPDG() << std::endl;
//	std::cout << "Photon Initial Position X: " << mina_test_particle.fStartXYZT()[0] << std::endl;
	fMotherPDGHist->Fill(mina_test_particle.fMotherPDG());
	fPhotonMotherHist->Fill(mina_test_particle.fMotherTID());
	fTrackLengthHist->Fill(mina_test_particle.fTrackLength());
	Pho_.push_back(mina_test_particle);
	if (mina_test_particle.fMotherPDG() == 111) 
	  ++pi0_mother_count;
	if (mina_test_particle.fMotherPDG() == 0)
	  ++primary_count;
	if (mina_test_particle.fMotherPDG() == 22){
	  std::cout << "!!!!!!!!FOUND PHOTON WITH PHOTON AS MOTHER!!!!!!!!" << std::endl;
	  std::cout << "Particle Track ID: " << mina_test_particle.fTrackId() << std::endl;
	  std::cout << "Particle Mother Track ID: " << mina_test_particle.fMotherTID() << std::endl;
	}
	if (mina_test_particle.fMotherPDG() != 2112 
	    && mina_test_particle.fMotherPDG() != 2212 
	    && mina_test_particle.fMotherPDG() != 13 
	    && mina_test_particle.fMotherPDG() != 211
	    && mina_test_particle.fMotherPDG() != -211)
	  ++non_nuclear_count;
      }
      if (mina_test_particle.fPDG() == 11) {
	++e_count;
      }
    }

//    std::cout << "photon count" << photon_count << std::endl;
//    std::cout << "electron count" << e_count << std::endl;

    //SORT BASED ON THE THREE CASES, A, B, and C. 
    //Fill all the histograms except the distance ones (which will be filled later)
    //CASE A: 3 or more photons, with one or more of the photons as primary
    bool filled = false;
    if (photon_count >= 3 && primary_count != 0 && pi0_mother_count == 0){
      fCaseHist->Fill(1);
      filled = true;
      A_Primary->Fill(primary_count);
      A_Count->Fill(photon_count);
      for (auto const& A_particle : *mina_test_event){
	if (A_particle.fPDG() == 22){
	  A_MPDG->Fill(A_particle.fMotherPDG());
	  A_Energy->Fill(A_particle.fEnergy());
	}
      }
    }
    if (photon_count >= 3 && pi0_mother_count >= 2 && primary_count == 0){
      fCaseHist->Fill(2);
      filled = true;
      B_Count->Fill(photon_count);
      B_pi0->Fill(pi0_mother_count);
      for (auto const& B_particle : *mina_test_event){
	if (B_particle.fPDG() == 22){
	  B_MPDG->Fill(B_particle.fMotherPDG());
	  if (B_particle.fEnergy() >= 10)
	    ++energy_pi_count;
	  if (B_particle.fMotherPDG() == 111) //ONLY includes energy and number of particles with pi0 mother
	    B_Energy_Pi0Photons->Fill(B_particle.fEnergy());
	  else 
	    B_Energy_OtherPhotons->Fill(B_particle.fEnergy());
	}
      }
    }
    if (e_count == 1){
//      fCaseHist->Fill(3);
//      filled = true;
      C_Count->Fill(photon_count);
      for (auto const& C_particle : *mina_test_event){
	if (C_particle.fPDG() == 22){
	  C_MPDG->Fill(C_particle.fMotherPDG());
	  C_Energy->Fill(C_particle.fEnergy());
	}
      }
    }
    //CASE N: PURE NUCLEAR SCATTERING.
    if (photon_count >= 3 && pi0_mother_count == 0 && primary_count == 0 && non_nuclear_count == 0){
      fCaseHist->Fill(4);
      filled = true;
      N_Count->Fill(photon_count);
      for (auto const& N_particle : *mina_test_event){
	if (N_particle.fPDG() == 22){
	  N_MPDG->Fill(N_particle.fMotherPDG());
	  N_Energy->Fill(N_particle.fEnergy());
	  if (N_particle.fEnergy() >= 10)
	    ++energy_n_count;
	}
      }
    }

    if (filled == false && photon_count < 3)
      fCaseHist->Fill(0);
    if (filled == false && photon_count >= 3)
      fCaseHist->Fill(0.5);
    

    if (photon_count >= 3)
      ++ThreeOrMore;
    if (photon_count == 3)
      ++ExactlyThree;
    if (primary_count == 3)
      ++ThreePrimary;
    if (energy_pi_count >= 3)
      ++EnergyPiCut;
    if (energy_pi_count == 3)
      ++EnergyPiCut3;
    if (energy_n_count >= 3)
      ++EnergyNCut;


    fPhotonCountHist->Fill(photon_count);
    fElectronCountHist->Fill(e_count);

    //if (e_count == 1)
    //fPhoton_ElectronHist->Fill(photon_count);
   
    std::vector<PhotonPair> Distance_;
    Distance_.clear();
    
    //kludgey initialization of TempPair
    PhotonPair TempPair;
    TempPair.dist = 0.;
    TempPair.part1.clear();
    TempPair.part2.clear();

    //Assign the distances between particles, if they're close enough 
    //This makes Distance_ an ORDERED list of pairs. (lower, higher)

    LineDist Dist;

//    std::cout << "size Pho_ " << Pho_.size() << std::endl;

    if (!Pho_.empty()){
      for(std::vector<MinaTestParticle>::iterator p1 = Pho_.begin(); p1 != Pho_.end()-1; ++p1) {
	//loop over all but the last particle in the loop
	
	if (p1 == Pho_.end()-1){
	  break;
	}
	
	for(std::vector<MinaTestParticle>::iterator p2 = p1+1; p2 != Pho_.end(); ++p2) {
	  fPhotonDistHist->Fill(Dist.Calc(*p1,*p2));
	  //write to distance histograms for particles in certain events
	  if (photon_count >= 3 && primary_count != 0)
	    A_Dist->Fill(Dist.Calc(*p1,*p2));
	  if (photon_count >= 3 && pi0_mother_count >= 2 && primary_count == 0){
	    B_Dist_Total->Fill(Dist.Calc(*p1,*p2));
	    if ((*p1).fMotherPDG() == 111 && (*p2).fMotherPDG() == 111)
	      B_Dist_Pi0Photons->Fill(Dist.Calc(*p1,*p2));
	    else if ((*p1).fMotherPDG() != 111 && (*p2).fMotherPDG() != 111)
	      B_Dist_OtherPhotons->Fill(Dist.Calc(*p1,*p2));
	  }
	  if (photon_count >=3 && primary_count == 0 && pi0_mother_count == 0 && non_nuclear_count == 0)
	    N_Dist->Fill(Dist.Calc(*p1,*p2));
	  if (e_count == 1)
	    C_Dist->Fill(Dist.Calc(*p1,*p2));
	  //end writing to histograms for particles in certain types of events
	  //std::cout << "DISTANCE BETWEEN PHOTONS: " << Dist.Calc(*p1,*p2) << std::endl;
	  if (Dist.Calc(*p1,*p2) < 1.){
	    TempPair.dist = Dist.Calc(*p1,*p2);
	    TempPair.part1 = *p1;
	    TempPair.part2 = *p2;
	    Distance_.push_back(TempPair);
	  }
	}
	
	
      }
    } //end of if !Pho_.empty()

//    std::cout << "Size of Distance_" << Distance_.size() << std::endl;
			      

    //Merge the relevant distances into retraceable groups
    //Again a kludgey clearing.  
    std::vector<std::vector<MinaTestParticle> > Pgroups_;
    Pgroups_.clear();

    if (!Distance_.empty()){

      for(std::vector<PhotonPair>::iterator pair1 = Distance_.begin(); pair1 != Distance_.end()-1; ++pair1 ) {
	
	//	if (Distance_.size() < 20)
	  //std::cout << "In pair 1 loop" << std::endl;
	//If pair1 is already in a group, continue (we have already picked it up below)
	if (!Pgroups_.empty()){
	  bool found1 = false;
	  for (std::vector<MinaTestParticle>  const& g : Pgroups_){
	    if (found1 == true)
	      break;
	    for (MinaTestParticle const& part : g){
	      if ((*pair1).part1.fTrackId() == part.fTrackId() || (*pair1).part2.fTrackId() == part.fTrackId()){
		found1 = true;
		break;
	      }
	    }
	  }      
	  if (found1 == true)     
	    continue; //Continue the OUTER loop
	}
	
	  
	if (pair1 != Distance_.end()-1){
	//Loop over all of the other pairs
	  for(std::vector<PhotonPair>::iterator pair2 = pair1+1; pair2 != Distance_.end(); ++pair2){

//	    if (Distance_.size() < 20) {
//	      std::cout << "In pair 2 loop" << std::endl;
//	      std::cout << "Pair 1: " << (*pair1).part1.fTrackId() << " " << (*pair1).part2.fTrackId() << std::endl;
//	      std::cout << "Pair 2: " << (*pair2).part1.fTrackId() << " " << (*pair2).part2.fTrackId() << std::endl;
//	    }
	    
	  
	  //first check whether any groups already exist; if not, make the first one where relevant.
	    if (Pgroups_.empty()){
//	      std::cout << "EMPTY" << std::endl;
	      if ( (*pair1).part1.fTrackId() == (*pair2).part1.fTrackId() || (*pair1).part2.fTrackId() == (*pair2).part1.fTrackId() ) {
		std::vector<MinaTestParticle> group = {(*pair1).part1, (*pair1).part2, (*pair2).part2};
		Pgroups_.push_back(group);
//		std::cout << "Creating group" << std::endl;
	      }
	      //the first of these cases should never happen due to ordering; maybe check at some point
	      else if ((*pair1).part1.fTrackId() == (*pair2).part2.fTrackId() || (*pair1).part2.fTrackId() == (*pair2).part2.fTrackId() ) {
		std::vector<MinaTestParticle> group = {(*pair1).part1, (*pair1).part2, (*pair2).part1};
		Pgroups_.push_back(group);
//		std::cout << "Creating group" << std::endl;
	      }
	    }
	  
	  //If groups already exist, merge the pair into an existing group where relevant
	    else {
	      bool found2 = false;
//	      if (Distance_.size() < 20){
//		std::cout << "In OUTER else loop" << std::endl;
//		std::cout << "PGROUP SIZE " << Pgroups_.size() << std::endl;
//	      }
	      for (std::vector<MinaTestParticle>  &g : Pgroups_){
		if (found2 == true)
		  break;
//		if (Distance_.size() < 20)
//		  std::cout << "In inner else loop, size g: " << g.size() << std::endl;			  
		for (MinaTestParticle const &part : g){
		  if ((*pair2).part1.fTrackId() == part.fTrackId()){
		    bool p2found = false;
		    //std::cout << "Particle ID to match " << part.fTrackId() << std::endl;
		    for (MinaTestParticle const &p : g){
		      if ((*pair2).part2.fTrackId() == p.fTrackId())
			p2found = true;
		    }
		    if (!p2found){
		      g.push_back((*pair2).part2);
//		      std::cout << "Adding particle ID " << (*pair2).part2.fTrackId() << std::endl;
		    }
		    found2 = true;
		    break;
		  }	
		  else if ((*pair2).part2.fTrackId() == part.fTrackId()){
		    bool p1found = false;
		    for (MinaTestParticle const &p : g){
		      if ((*pair2).part1.fTrackId() == p.fTrackId())
			p1found = true;
		    }
		    if (!p1found){
		      g.push_back((*pair2).part1);
//		      std::cout << "Adding particle ID " << (*pair2).part1.fTrackId() << std::endl;
		    }
		    found2 = true;
		    
		    break;
		  }
		} //End loop over particles in group     

	      } //End loop over groups

	      //CREATE A NEW GROUP IF IT WASN'T FOUND IN AN OLD GROUP
	      if (found2 == false){
		if ( (*pair1).part1.fTrackId() == (*pair2).part1.fTrackId() || (*pair1).part2.fTrackId() == (*pair2).part1.fTrackId() ) {
		  std::vector<MinaTestParticle> group = {(*pair1).part1, (*pair1).part2, (*pair2).part2};
		  Pgroups_.push_back(group);
		  std::cout << "Creating another group" << std::endl;
		}
		//the first of these cases should never happen due to ordering; maybe check at some point
		else if ((*pair1).part1.fTrackId() == (*pair2).part2.fTrackId() || (*pair1).part2.fTrackId() == (*pair2).part2.fTrackId() ) {
		  std::vector<MinaTestParticle> group = {(*pair1).part1, (*pair1).part2, (*pair2).part1};
		  Pgroups_.push_back(group);
		  std::cout << "Creating another group" << std::endl;
		}
		
	      }
	      
	    } //End else statment 
	    
	  } //End loop over second pair
	  
	} //End check for whether second pair is empty
	
      } //End loop over all pairs

    } //End check for whether distance is empty

    std::cout << "Pgroup size" << Pgroups_.size() << std::endl;
    
    //Now analyze each group by mother particle.
    for (std::vector<MinaTestParticle> const& g : Pgroups_){
      std::cout << "NEW GROUP!!!!!" << std::endl;
      std::cout << "Total photons in event: " << photon_count << std::endl;
      std::cout << "Total photons in group: " << g.size() << std::endl;
      fMultiPhotonHist->Fill(g.size());
    }

//    std::cout << std::endl;
//    std::cout << "done with entry " << i << std::endl;
//    std::cout << std::endl;
  } 

  std::cout << "EVENTS with 3 or more photons: " << ThreeOrMore << std::endl;
  std::cout << "EVENTS with exactly 3 photons: " << ExactlyThree << std::endl;
  std::cout << "EVENTS with exactly 3 primary photons: " << ThreePrimary << std::endl;
  std::cout << "Pi0 EVENTS with photon energy over 10 MeV: " << EnergyPiCut << std::endl;
  std::cout << "Pi0 EVENTS with photon energy over 10 MeV AND three photons: " << EnergyPiCut3 << std::endl;
  std::cout << "Nuclear scattering EVENTS with energy over 10 MeV: " << EnergyNCut << std::endl;
  //std::cout << "Check 3" << std::endl;
  fPhotonCountHist->Write();
  fElectronCountHist->Write(); 
  fEnergyHist->Write();
  fMultiPhotonHist->Write();
  fPhotonDistHist->Write();
  //fPhoton_ElectronHist->Write();
  fPhotonMotherHist->Write();
  fTrackLengthHist->Write();
  fMotherPDGHist->Write();
  fCaseHist->Write();

  A_Dist->Write();
  A_MPDG->Write();
  A_Energy->Write();
  A_Count->Write();
  A_Primary->Write();

  B_Dist_Total->Write();
  B_Dist_Pi0Photons->Write();
  B_Dist_OtherPhotons->Write();
  B_MPDG->Write();
  B_Energy_Pi0Photons->Write();
  B_Energy_OtherPhotons->Write();
  B_Count->Write();
  B_pi0->Write();

  C_Dist->Write();
  C_MPDG->Write();
  C_Energy->Write();
  C_Count->Write();

  N_Dist->Write();
  N_MPDG->Write();
  N_Energy->Write();
  N_Count->Write();


  fout->Close();
  
}


#endif
