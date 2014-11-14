#ifndef CALCULATEBACKGROUND_CC
#define CALCULATEBACKGROUND_CC

#include "CalculateBackground.hh"

namespace larlight {

  bool CalculateBackground::initialize() {

    if (_verbose) { _MCgetter.SetVerbose(true); }

    /// Prepare Tree
    prepareTree();

    _evtN = 0;

    return true;
  }


  void CalculateBackground::prepareTree(){

    if (_tree) delete _tree;
    _tree = new TTree("ana_tree","");
    _tree->Branch("_Energy",&_Energy,"Energy/D");
    _tree->Branch("_inTPC",&_inTPC,"inTPC/I");
    _tree->Branch("_MotherPDG",&_MotherPDG,"MotherPDG/I");
    _tree->Branch("_MotherE",&_MotherE,"MotherE/D");
    _tree->Branch("_MotherDist",&_MotherDist,"MotherDist/D");
    _tree->Branch("_AncestorPDG",&_AncestorPDG,"AncestorPDG/I");
    _tree->Branch("_AncestorE",&_AncestorE,"AncestorE/D");
    _tree->Branch("_AncestorDist",&_AncestorDist,"AncestorDist/D");
    _tree->Branch("_StartX",&_StartX,"StartX/D"); 
    _tree->Branch("_StartY",&_StartY,"StartY/D"); 
    _tree->Branch("_StartZ",&_StartZ,"StartZ/D"); 
    _tree->Branch("_EndX",&_EndX,"EndX/D"); 
    _tree->Branch("_EndY",&_EndY,"EndY/D"); 
    _tree->Branch("_EndZ",&_EndZ,"EndZ/D"); 
    _tree->Branch("ProcHist",&ProcHist);
    _tree->Branch("Process",&Process);

  }

  void CalculateBackground::SetProperties(){

    /// set volume for TrajectoryInVolume algorithm
    _inTPCAlgo.SetVolume( 0, 
			  2*(::larutil::Geometry::GetME()->DetHalfWidth()),
			  -(::larutil::Geometry::GetME()->DetHalfHeight()),
			  ::larutil::Geometry::GetME()->DetHalfHeight(),
			  0,
			  ::larutil::Geometry::GetME()->DetLength());

    setPOT(6.0e20);
    setpps(1.2e12);
    setBeamTime(1.6e-6);
    setEventTime(3.2e-3);

  }
  
  bool CalculateBackground::analyze(storage_manager* storage) {

    event_mcpart *event_part = (event_mcpart*)(storage->get_data(DATA::MCParticle));

    // make the particle map
    _MCgetter.Reset(event_part);

    if (_process.size() > 0){
      // results is vector of trackIDs

      // Search with process-looker
      //std::vector<int> results = _MCgetter.findProcess(_process);
      //std::cout << "Matches found: " << results.size() << std::endl;
      
      //search with PDG code getter
      std::vector<int> results = _MCgetter.getPDGlist();
      std::cout << "matching-PDG particles found: " << results.size() << std::endl;
      for (size_t i=0; i < results.size(); i ++){
	//apply energy cut
	mcpart part = event_part->at(_MCgetter.searchParticleMap(results.at(i)));
	if ( part.Trajectory().at(0).E() > _Ecut ){
	  std::vector<std::vector<float> > traj = _MCgetter.getStartEndTrajectory(part);
	  _StartX = traj.at(0).at(0);
	  _StartY = traj.at(0).at(1);
	  _StartZ = traj.at(0).at(2);
	  _EndX = traj.back().at(0);
	  _EndY = traj.back().at(1);
	  _EndZ = traj.back().at(2);
	  if (_verbose) {
	    std::cout << "Traj: " << std::endl;
	    for (size_t j=0; j < traj.size(); j++)
	      std::cout << "(" << traj.at(j).at(0) 
			<< ", " << traj.at(j).at(1) 
			<< ", " << traj.at(j).at(2) << ")" << std::endl;
	  }
	  //get if in TPC
	  if (_inTPCAlgo.IsInVolume(_MCgetter.getStartEndTrajectory(part))) { _inTPC = true; }
	  else { _inTPC = false; }
	  if (_verbose) { std::cout << "Passes TPC volume? " << _inTPC << std::endl; }
	  // get particle's energy
	  _Energy = part.Trajectory().at(0).E();
	  std::vector<float> partStart;
	  partStart.push_back(part.Trajectory().at(0).X());
	  partStart.push_back(part.Trajectory().at(0).Y());
	  partStart.push_back(part.Trajectory().at(0).Z());
	  // get Ancestor Information
	  mcpart ancestor = event_part->at(_MCgetter.searchParticleMap(_MCgetter.getAncestor(part)));
	  _AncestorPDG = ancestor.PdgCode();
	  _AncestorE   = ancestor.Trajectory().at(0).E();
	  // get mother Information
	  if (_MCgetter.getMother(part) >= 0){
	    mcpart mother = event_part->at(_MCgetter.searchParticleMap(part.Mother()));
	    std::vector<std::vector<float> > motherStartEnd = _MCgetter.getStartEndTrajectory(mother);
	    _MotherPDG = mother.PdgCode();
	    _MotherE   = mother.Trajectory().at(0).E();
	    _MotherDist = _pointDist.Distance(partStart, motherStartEnd.at(0), motherStartEnd.at(1));
	  }
	  else { _MotherPDG = -1; _MotherE = -1; _MotherDist = 0; }
	  // get process information
	  Process = part.Process();
	  ProcHist = _MCgetter.getFullProcess(part);
	  // get distance from Ancestor track
	  std::vector<std::vector<float> > ancestorStartEnd = _MCgetter.getStartEndTrajectory(ancestor);
	  //if ancestor IS particle, distance will be zero	  
	  if ( partStart == ancestorStartEnd.at(0) )
	    _AncestorDist = 0.;
	  else
	    _AncestorDist = _pointDist.Distance(partStart, ancestorStartEnd.at(0), ancestorStartEnd.at(1));
	  // FILL TREE!
	  _tree->Fill();
	}//if passes E cut
      }//for all particles
    }//if process size is > 0

    _evtN += 1;
    
    return true;
  }

  bool CalculateBackground::finalize() {

    _tree->Write();

    FillBackgroundHistogram();
    NormalizeBackgroundHistogram();
    WriteBackgroundHistogram();

    return true;
  }


  void CalculateBackground::SetBackgroundHistogram(int nbins, float xmin, float xmax){

    _Nbins = nbins;
    _Emin  = xmin;
    _Emax  = xmax;

    if (_hBackground) delete _hBackground;

      _hBackground = new TH1D("hBackground",
			      Form("Background Prediction for %f POT",_POT),
			      _Nbins, _Emin, _Emax);

  }

  
  void CalculateBackground::NormalizeBackgroundHistogram(){

    if (!_hBackground){
      std::cout << "Histogram not set up...not proceeding..." << std::endl;
      return;
    }

    // Take _hBackground and normalize based on:
    // POTs requestes vs. proptons-per-spill
    // beam time-window vs. "event-time-window"
    // Assumes each event in file is the data
    // that is recorded during an "event-time-window"
    
    double nEvt = (double)_evtN;
    for (int b=0; b < _hBackground->GetNbinsX(); b++)
      _hBackground->SetBinContent(b+1, _hBackground->GetBinContent(b+1)*(_POT/_pps)*(_beamT/_eventT)/nEvt);
    

  }
  

  void CalculateBackground::FillBackgroundHistogram(){

    //take E-distribution histogram from tree and make a TH1D background plot
    if (!_hBackground){
      std::cout << "Histogram not set up...not proceeding..." << std::endl;
      return;
    }
    
    //Double_t eVals[_tree->GetEntries()];
    //_tree->SetBranchAddress("_Energy",eVals);
    //    TBranch *Ebranch = _tree->GetBranch("_Energy");
    for (int i=0; i < _tree->GetEntries(); i++){
      _tree->GetEntry(i);
      if (_inTPC)
	_hBackground->Fill(_Energy);
      //      _hBackground->Fill(eVals[i]);
      //std::cout << "Energy: " << eVals[i] << std::endl;
    }

    return;
  }

  void CalculateBackground::WriteBackgroundHistogram(){

    if (!_hBackground){
      std::cout << "Histogram not set up...not proceeding..." << std::endl;
      return;
    }
    
    _hBackground->Write();
    
    return;
  }
  
  void CalculateBackground::SetProcess(std::vector<int> PDGs, std::vector<std::string> procs){

    _process.clear();

    if ( PDGs.size() != procs.size() )
      std::cout << "Error: PDG and Process list must have same length. No process added." << std::endl;
    else{
      std::pair<int,std::string> pairTmp;
      for (size_t i=0; i < PDGs.size(); i++){
	pairTmp = std::make_pair(PDGs.at(i),procs.at(i));
	_process.push_back(pairTmp);
      }
    }
      
    return;
  }

}
#endif
