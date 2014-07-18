#ifndef DARKPHOTONANAC_CC
#define DARKPHOTONANAC_CC

#include "DarkPhotonAnaC.hh"
#include "LineDist.hh"

struct PhotonPair{ 
  MinaTestParticle part1;
  MinaTestParticle part2;
  double dist;
};

//struct PhotonGroup : std::vector<MinaTestParticle> {};

void DarkPhotonAnaC::RunEventLoop(TTree* tree)
{
  //
  // This function runs an event loop on input TTree and attempts to read MinaTestEvent data product
  //

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
 
  std::cout << "Check 0" << std::endl;
  // Definition of output TTree!
  TFile* fout = TFile::Open("DarkPhotonAnalyzed_new.root", "RECREATE");
  //TH1D* fPDGCodeHist = new TH1D("pdgcodes",";PDG Code;", 5000, -2500, 2500);
  TH1D* fPhotonCountHist = new TH1D("photoncount",";Number of Photons;", 50, 0, 50);
  TH1D* fElectronCountHist = new TH1D("electroncount", ";Number of Electrons;", 10, 0, 10);
  TH1D* fEnergyHist = new TH1D("photonenergy",";Photon Energy",400,0,40000);
  TH1D* fMultiPhotonHist = new TH1D("photongroup",";Number of Photons in Multi Event;", 50, 0, 50);
  TH1D* fPhotonDistHist = new TH1D("photondist",";Distances between Photons;",500,0,500);
  TH1D* fPhoton_ElectronHist = new TH1D("photonsWelectron","# of Photons with One Electron", 50, 0, 50);
  TH1D* fPhotonMotherHist = new TH1D("multimotherPDG",";TrackID of Photon Mother",0,500,500);
  TH1D* fTrackLengthHist = new TH1D("photontracklength",";Photon Track Length (cm);", 500, 0, 500);
  // Step 3) start event loop

  int ThreeOrMore = 0;
  int ExactlyThree = 0;


  for(size_t i=0; i<tree->GetEntries(); ++i) {

    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    std::cout << "TTree entry: " << i << std::endl;
    tree->GetEntry(i);

    std::cout << "  Found " << mina_test_event->size() << " particles!" << std::endl;
    std::cout << "  Listing PDG ID of particles..." << std::flush;

    std::cout << " " << std::endl;
    
    int photon_count = 0;
    int e_count = 0;

    std::vector<MinaTestParticle> Pho_;
    Pho_.clear();

    for(auto const& mina_test_particle : *mina_test_event) {
      
      std::cout << mina_test_particle.fPDG() << " ";
      if (mina_test_particle.fPDG() == 22) { 
	++photon_count; //Count the number of photons in the event
	fEnergyHist->Fill(mina_test_particle.fEnergy());
	std::cout << "Photon Track ID: " << mina_test_particle.fTrackId() << std::endl;
	std::cout << "Photon Energy: " << mina_test_particle.fEnergy() << std::endl;
	std::cout << "Photon Mother Track ID: " << mina_test_particle.fMotherTID() << std::endl;
	std::cout << "Photon Mother PDG: " << mina_test_particle.fMotherPDG() << std::endl;
	fPhotonMotherHist->Fill(mina_test_particle.fMotherTID());
	fTrackLengthHist->Fill(mina_test_particle.fTrackLength());
	Pho_.push_back(mina_test_particle);
      }
      if (mina_test_particle.fPDG() == 11) {
	++e_count;
      }

    }

    std::cout << "photon count" << photon_count << std::endl;
    std::cout << "electron count" << e_count << std::endl;

    if (photon_count >= 3)
      ++ThreeOrMore;
    if (photon_count == 3)
      ++ExactlyThree;
    fPhotonCountHist->Fill(photon_count);
    fElectronCountHist->Fill(e_count);

    if (e_count == 1)
      fPhoton_ElectronHist->Fill(photon_count);
   
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

    std::cout << "size Pho_ " << Pho_.size() << std::endl;

    if (!Pho_.empty()){
      for(std::vector<MinaTestParticle>::iterator p1 = Pho_.begin(); p1 != Pho_.end()-1; ++p1) {
	//loop over all but the last particle in the loop
	
	if (p1 == Pho_.end()-1){
	  break;
	}
	
	for(std::vector<MinaTestParticle>::iterator p2 = p1+1; p2 != Pho_.end(); ++p2) {
	  fPhotonDistHist->Fill(Dist.Calc(*p1,*p2));
	  if (Dist.Calc(*p1,*p2) < 30.){
	    TempPair.dist = Dist.Calc(*p1,*p2);
	    TempPair.part1 = *p1;
	    TempPair.part2 = *p2;
	    Distance_.push_back(TempPair);
	  }
	}
	
	
      }
    } //end of if !Pho_.empty()

    std::cout << "Size of Distance_" << Distance_.size() << std::endl;
			      

    //Merge the relevant distances into retraceable groups
    //Again a kludgey clearing.  
    std::vector<std::vector<MinaTestParticle> > Pgroups_;
    Pgroups_.clear();

    if (!Distance_.empty()){

      for(std::vector<PhotonPair>::iterator pair1 = Distance_.begin(); pair1 != Distance_.end()-1; ++pair1 ) {
	
	if (Distance_.size() < 20)
	  std::cout << "In pair 1 loop" << std::endl;
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

	    if (Distance_.size() < 20) {
	      std::cout << "In pair 2 loop" << std::endl;
	      std::cout << "Pair 1: " << (*pair1).part1.fTrackId() << " " << (*pair1).part2.fTrackId() << std::endl;
	      std::cout << "Pair 2: " << (*pair2).part1.fTrackId() << " " << (*pair2).part2.fTrackId() << std::endl;
	    }
	    
	  
	  //first check whether any groups already exist; if not, make the first one where relevant.
	    if (Pgroups_.empty()){
	      std::cout << "EMPTY" << std::endl;
	      if ( (*pair1).part1.fTrackId() == (*pair2).part1.fTrackId() || (*pair1).part2.fTrackId() == (*pair2).part1.fTrackId() ) {
		std::vector<MinaTestParticle> group = {(*pair1).part1, (*pair1).part2, (*pair2).part2};
		Pgroups_.push_back(group);
		std::cout << "Creating group" << std::endl;
	      }
	      //the first of these cases should never happen due to ordering; maybe check at some point
	      else if ((*pair1).part1.fTrackId() == (*pair2).part2.fTrackId() || (*pair1).part2.fTrackId() == (*pair2).part2.fTrackId() ) {
		std::vector<MinaTestParticle> group = {(*pair1).part1, (*pair1).part2, (*pair2).part1};
		Pgroups_.push_back(group);
		std::cout << "Creating group" << std::endl;
	      }
	    }
	  
	  //If groups already exist, merge the pair into an existing group where relevant
	    else {
	      bool found2 = false;
	      if (Distance_.size() < 20){
		std::cout << "In OUTER else loop" << std::endl;
		std::cout << "PGROUP SIZE " << Pgroups_.size() << std::endl;
	      }
	      for (std::vector<MinaTestParticle>  &g : Pgroups_){
		if (found2 == true)
		  break;
		if (Distance_.size() < 20)
		  std::cout << "In inner else loop, size g: " << g.size() << std::endl;
			  
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
		      std::cout << "Adding particle ID " << (*pair2).part2.fTrackId() << std::endl;
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
		      std::cout << "Adding particle ID " << (*pair2).part1.fTrackId() << std::endl;
		    }
		    found2 = true;
		    
		    break;
		  }
		} //End loop over particles in group     

	      } //End loop over groups
	      
	    } //End else statment 
	    
	  } //End loop over second pair
	  
	} //End check for whether second pair is empty
	
      } //End loop over all pairs

    } //End check for whether distance is empty

    std::cout << "Check 2" << std::endl;

    std::cout << "Pgroup size" << Pgroups_.size() << std::endl;
    
    //Now analyze each group by mother particle.
    for (std::vector<MinaTestParticle> const& g : Pgroups_){
      std::cout << "NEW GROUP!!!!!" << std::endl;
      std::cout << "Total photons in event: " << photon_count << std::endl;
      std::cout << "Total photons in group: " << g.size() << std::endl;
      fMultiPhotonHist->Fill(g.size());
      //for (MinaTestParticle const& part : g){
      //  std::cout << "Mother TrackID" << part.fMotherTID() << std::endl;
      //}
    }

    std::cout << std::endl;
    std::cout << "done with entry " << i << std::endl;
    std::cout << std::endl;
  } 

  std::cout << "EVENTS with 3 or more photons: " << ThreeOrMore << std::endl;
  std::cout << "EVENTS with exactly 3 photons: " << ExactlyThree << std::endl;
  std::cout << "Check 3" << std::endl;
  fPhotonCountHist->Write();
  fElectronCountHist->Write(); 
  fEnergyHist->Write();
  fMultiPhotonHist->Write();
  fPhotonDistHist->Write();
  fPhoton_ElectronHist->Write();
  fPhotonMotherHist->Write();
  fTrackLengthHist->Write();

  fout->Close();
  
}


#endif
