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
  TFile* fout = TFile::Open("DarkPhotonAnalyzed.root");
  //TH1D* fPDGCodeHist = new TH1D("pdgcodes",";PDG Code;", 5000, -2500, 2500);
  TH1D* fPhotonCountHist = new TH1D("photoncount",";Number of Photons;", 50, 0, 50);
  TH1D* fElectronCountHist = new TH1D("electroncount", ";Number of Electrons;", 10, 0, 10);
  TH1D* fEnergyHist = new TH1D("photonenergy",";Photon Energy",1000,0,4000);
  TH1D* fMultiPhotonHist = new TH1D("photongroup",";Number of Photons in Multi Event;", 50, 0, 50);
  TH1D* fPhotonDistHist = new TH1D("photondist",";Distances between Photons;",2000,0,2000);
  TH1D* fPhoton_ElectronHist = new TH1D("photonsWelectron","# of Photons with One Electron", 50, 0, 50);
  TH1D* fMultiMotherHist = new TH1D("multimotherPDG",";PDG of Multiple Photon Mother",-2500,2500,5000);
  TH1D* fTrackLengthHist = new TH1D("photontracklength",";Photon Track Length (cm);", 200, 0, 2000);
  // Step 3) start event loop

  for(size_t i=0; i<tree->GetEntries(); ++i) {

    std::cout << "TTree entry: " << i << std::endl;
    tree->GetEntry(i);

    std::cout << "  Found " << mina_test_event->size() << " particles!" << std::endl;
    std::cout << "  Listing PDG ID of particles..." << std::flush;
    
    int photon_count = 0;
    int e_count = 0;

    std::vector<MinaTestParticle> Pho_;

    for(auto const& mina_test_particle : *mina_test_event) {
      
      std::cout << mina_test_particle.fPDG() << " ";
      if (mina_test_particle.fPDG() == 22) { 
	++photon_count; //Count the number of photons in the event
	fEnergyHist->Fill(mina_test_particle.fEnergy());
	fTrackLengthHist->Fill(mina_test_particle.fTrackLength());
	Pho_.push_back(mina_test_particle);
      }
      if (mina_test_particle.fPDG() == 11)
	++e_count;

    }

    fPhotonCountHist->Fill(photon_count);
    fElectronCountHist->Fill(e_count);

    if (e_count == 1)
      fPhoton_ElectronHist->Fill(photon_count);
   
    std::vector<PhotonPair> Distance_;
    
    //kludgey initialization of TempPair
    PhotonPair TempPair;
    TempPair.dist = 0.;
    TempPair.part1.clear();
    TempPair.part2.clear();

    //Assign the distances between particles, if they're close enough 
    //This makes Distance_ an ORDERED list of pairs. (lower, higher)

    LineDist Dist;

    for(std::vector<MinaTestParticle>::iterator p1 = Pho_.begin(); p1 != --Pho_.end(); ++p1) {
      //loop over all but the last particle in the loop
      for(std::vector<MinaTestParticle>::iterator p2 = ++p1; p2 != Pho_.end(); ++p2) {
	fPhotonDistHist->Fill(Dist.Calc(*p1,*p2));
	if (Dist.Calc(*p1,*p2) < 30.){
	  TempPair.dist = Dist.Calc(*p1,*p2);
	  TempPair.part1 = *p1;
	  TempPair.part2 = *p2;
	  Distance_.push_back(TempPair);
	}
      }
      
    }
			      

    //Merge the relevant distances into retraceable groups
    //Again a kludgey clearing.  
    std::vector<std::vector<MinaTestParticle> > Pgroups_;
    Pgroups_.clear();

    for(std::vector<PhotonPair>::iterator pair1 = Distance_.begin(); pair1 != --Distance_.end(); ++pair1 ) {
	
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
      
	  
	//Loop over all of the other pairs
	for(std::vector<PhotonPair>::iterator pair2 = ++pair1; pair2 != Distance_.end(); ++pair2){

	  //first check whether any groups already exist; if not, make the first one where relevant.
	  if (Pgroups_.empty()){
	    if ( (*pair1).part1.fTrackId() == (*pair2).part1.fTrackId() || (*pair1).part2.fTrackId() == (*pair2).part1.fTrackId() ) {
	      std::vector<MinaTestParticle> group = {(*pair1).part1, (*pair1).part2, (*pair2).part2};
	      Pgroups_.push_back(group);
	    }
	    //the first of these cases should never happen due to ordering; maybe check at some point
	    else if ((*pair1).part1.fTrackId() == (*pair2).part2.fTrackId() || (*pair1).part2.fTrackId() == (*pair2).part2.fTrackId() ) {
	      std::vector<MinaTestParticle> group = {(*pair1).part1, (*pair1).part2, (*pair2).part1};
	      Pgroups_.push_back(group);
	    }
	  }

	  //If groups already exist, merge the pair into an existing group where relevant
	  else {
	    bool found2 = false;
	    for (std::vector<MinaTestParticle>  &g : Pgroups_){
	      if (found2 == true)
		break;
	      for (MinaTestParticle const &part : g){
		if ((*pair1).part1.fTrackId() == part.fTrackId()){
		  MinaTestParticle TMPpart = (*pair1).part2;
		  g.push_back(TMPpart);
		  found2 = true;
		  break;
		}	
		else if ((*pair1).part2.fTrackId() == part.fTrackId()){
		  g.push_back((*pair1).part1);
		  found2 = true;
		  break;
		}
	      } //End loop over particles in group     
	    } //End loop over groups
	    
	  } //End else statment 
	
	} //End loop over second pair

    } //End loop over all pairs
    
    
    //Now analyze each group by mother particle.
    for (std::vector<MinaTestParticle> const& g : Pgroups_){
      std::cout << "NEW GROUP!!!!!";
      for (MinaTestParticle const& part : g){
	std::cout << "Mother TrackID" << part.fMotherTID() << std::endl;
      }
    }

    std::cout << std::endl;
    std::cout << "done with entry " << i << std::endl;
    std::cout << std::endl;
  } 

  fPhotonCountHist->Write();
  fElectronCountHist->Write(); 
  fEnergyHist->Write();
  fMultiPhotonHist->Write();
  fPhotonDistHist->Write();
  fPhoton_ElectronHist->Write();
  fMultiMotherHist->Write();
  fTrackLengthHist->Write();

  fout->Close();
  
}


#endif
