#define Reprocessing_cxx
#include "Reprocessing.hh"

#define print(a) ( std::cout << a << std::endl)


void lar1::Reprocessing::Loop(std::string signal,
                              int iDet,
                              int iLoc,
                              Long64_t max_entry,
                              bool skipMultiWeights,
                              bool verbose,
                              double scale ){

    // Signal options are:
    // nue  - electron neutrino appearance backgrounds
    // numu - muon neutrino disappearance 
    // fosc - electron neutrion appearace signal events
    // phot - single photon backgrounds.
    // NCpi0 - sample of events for NC disap. using NCpi0
    // numi - switch to run over numi events (correct genie sim, no larg4)
    //
    
    //These are the options available for the reprocessing:  
    /*
      enum beamMode {kNu, kNubar, kNu_Fosc, kNubar_Fosc };
      enum detectorGeom {kND, kUB, kIC, kND_long, kMB};
      enum detectorLoc  {k100m, k150m, k175m, k200m, k470m, k600m_onaxis, k600m_offaxis};
    */


    // Check to make sure the signal is one of the above:
    if (signal != "nue"   && 
        signal != "numu"  &&
        signal != "fosc"  &&
        signal != "phot"  &&
        signal != "ncpi0" &&
        signal != "numi"  ){
      std::cout << "Error: incorrect signal \"" << signal << "\"  used." << std::endl;
      std::cout << "Please see NtupleReprocessing.C for a list of available signals." << std::endl;
      return;
    }

    if (signal == "numi"){
      utils.setMC_Generation(2);
    }

    //---------------------------------------------
    // some things to be configured by the user:
    // 
    double electronIDeff = 0.8;
    double muonIDeff     = 0.8;
    double photonMisID   = 0.06;
    double muonCCMisID   = 0.001;

    // NC photon vertex energy cuts
    double vtxEcut = 99999;     // 0.025;   // GeV
    double convDistCut = 99999; // 5.0;     // cm

    double egammaThreshold = 0.0; // 0.140 // GeV

    Double_t prot_thresh = 0.02;

    Double_t detect_dist = 0;   // 10000=ND, 47000=MicroBooNE, 70000=FD

    switch (iLoc)
    {
      case k100m:
        detect_dist = 10000;
        break;
      case k150m:
        detect_dist = 15000;
        break;
      case k175m:
        detect_dist = 17500;
        break;
      case k200m:
        detect_dist = 20000;
        break;
      case k470m:
        detect_dist = 47000;
        break;
      case k600m_onaxis:
        detect_dist = 60000;
        break;
      case k600m_offaxis:
        detect_dist = 60000;
        break;
      default: 
        detect_dist = -1;
    }


    //---------------------------------------------

    //   In a ROOT session, you can do:
    //      Root > .L NtupleReprocessing.C+ (+ to compile - this is necessary)
    //      Root > NtupleReprocessing t
    //      Root > t.GetEntry(12); // Fill t data members with entry number 12
    //      Root > t.Show();       // Show values of entry 12
    //      Root > t.Show(16);     // Read and show values of entry 16
    //      Root > t.Loop();       // Loop on all entries
    //
    
    //    This is the loop skeleton where:
    //    jentry is the global entry number in the chain
    //    ientry is the entry number in the current Tree
    //    Note that the argument to GetEntry must be:
    //    jentry for TChain::GetEntry
    //    ientry for TTree::GetEntry and TBranch::GetEntry
    //
    //    To read only selected branches, Insert statements like:
    //  METHOD1:
    //    fChain->SetBranchStatus("*",0);  // disable all branches
    //    fChain->SetBranchStatus("branchname",1);  // activate branchname
    //  METHOD2: replace line
    //    fChain->GetEntry(jentry);       //read all branches
    //    b_branchname->GetEntry(ientry); //read only this branch
   
    /**
       In this section, I describe how this object creates ntuples that could be used in sensitivity analysis.
       
       Each event is examined and given a weight.  In the sensitivity code, every event is added to a histogram of 
       events in each energy bin, weighted by it's weight of course.  So, to get only nu_e events (for example)
       every event that is not nue is given a weight of 0.
       This method allows midIDs and efficiencies to be accounted for right here.  Accordingly, no efficiency should
       be applied in the sensitivity code.  MisIDs can be accounted for in a way like this example:
       
       If there is a NC pi0 with only 1 photon contained, there is a 6% chance that it will be be ID'd as a nue event.
       So, give that event a weight (variable wgt) of 0.06.
       *Important* you probably also want to change the energy bin!  Based on the photon's energy (which is in the ntuple)
       you can come up with some reconstructed neutrino energy in whatever way you please.  Store this result in the variable
       "energy" for the sensitivity code to use your reconstructed/smeared energy.
       (enugen will retain the true neutrino energy.  It'd be so nice if we had that in a real experiment...)
       
       To create those nice histograms like the miniboone plots, we need to put events into the correct backgrounds
       The total nue ntuple will look at every event and use everything, but smaller histograms will be made
       based up the following lists.  So if an event fits these backgrounds, set it's variable ibkg accordingly
       
       (you can add more to these and refine them as you please.  But let me (corey) know so I can update the code downstream)
       
       Nu_e background
       
       kNueFromNueCC_muon:        ibkg == 1
       kNueFromNueCC_chargeKaon:  ibkg == 2
       kNueFromNueCC_neutKaon:    ibkg == 3
       kNueFromEScatter:          ibkg == 4
       kNueFromNC_pi0:            ibkg == 5
       kNueFromNC_delta0:         ibkg == 6
       kNueFromNumuCC:            ibkg == 7
       kDirt:                     ibkg == 8
       kOther:                    ibkg == 9
       
       Nu_mu background
       
       kNumuFromNumuCC:   ibkg == 1
       kNumuFromNueCC:    ibkg == 2
       kNumuFromNC:       ibkg == 3
    **/

    if (fChain == 0) return;


    //----------------------------------
    // Create the output file
    //----------------------------------
    std::cout << "input file: " << InFile().Remove(InFile().Length()-5) << std::endl;

    TString outfile = InFile().Remove(InFile().Length()-5) + "_processed_";
    outfile = outfile + signal + ".root";

    std::cout << "outfile file: " << outfile << std::endl;
    TFile *f = new TFile(outfile, "RECREATE");
    if (f->IsZombie()){
      std::cout << "Error, couldn't create output file." << std::endl;
      return;
    }  

    // Get the edges of this detector:
    double xmin, xmax, ymin, ymax, zmin, zmax;
    utils.GetDetBoundary(iDet, xmin, xmax, ymin, ymax, zmin, zmax);

    double ShowerContainedDistance = 0;
    double ShowerDistanceToStart = 0;
    double ShowerDistanceToStartYZ = 0;

    double ncpi0en=0;
    TLorentzVector shower1P, shower2P;
    TLorentzVector shower1X, shower2X;
    TVector3 MuonExitPos(0,0,0), MuonExitMom(0,0,0);

    // These variables are filled event by event:
    double efficiency = 0.0;
    double fluxweight = 1.0;
    double electron_cand_energy = 0.0;
    double electron_cand_angle = 0.0;
    double enuccqe = 0.0;
    double enucalo1 = 0.0;
    double enucalo2 = 0.0;
    std::vector<double> ConversionDist;
    bool isFid, isActive;
    double vertexEnergy;
    double showerGap;
    int iChan = 0, iTop = 0;
    int ibkg = 0;
    int NChargedKaon = 0;
    int NNeutralKaon = 0;

    double wgt;

    double ThetaLepSmeared,PhiLepSmeared,ElepSmeared;

    //Get the POT weight.  Need to get iflux
    b_iflux->GetEntry(0); //iflux should now be filled
    double potweight = 1.0;
    if (signal != "numi"){
      potweight = utils.GetPOTNorm( iflux, iLoc );
      std::cout << "POT weight = " << potweight << std::endl;
    }

    // Create the output TTree
    TTree *newt = new TTree("EventsTot", "Event info for ALL types");

    //===========================================================
    // This is all of the information that winds up in the tree, 
    // which is in turn what the sensitivity code gets.
    // If you make additions, let me know so I can update the 
    // sensitivity code accordingly. (Corey A.)

    // double Vx, Vy, Vz;

    //--------------------------------------------------
    // These are variables already in the input ntuple
    //
    newt->Branch("iflux", &iflux, "iflux/I");            
    newt->Branch("ibkg", &ibkg, "ibkg/I");          
    newt->Branch("iDet", &iDet, "iDet/I");
    newt->Branch("nuchan", &nuchan, "nuchan/I");         // NUANCE interaction codes
    newt->Branch("inno", &inno, "inno/I");               // Neutrino type +-12,+-14,
    newt->Branch("enugen", &enugen, "enugen/D");         // Neutrino energy
    newt->Branch("nuleng", &nuleng, "nuleng/D");         // True neutrino length
    newt->Branch("tptype", &tptype, "tptype/I");            // Neutrino parent ID
    newt->Branch("ndecay",&ndecay,"ndecay/I");           
    /* nudecay
       1 _K0L -> nue pi- e+
       2 _K0L -> nuebar pi+ e-
       3  K0L -> numu pi- mu+
       4  K0L -> numubar pi+ mu-
       5  K+  -> numu mu+
       6  K+  -> nue pi0 e+
       7  K+  -> numu pi0 mu+
       8  K-  -> numubar mu-
       9  K-  -> nuebar pi0 e-
       10  K-  -> numubar pi0 mu-
       11  mu+ -> numubar nue e+
       12  mu- -> numu nuebar e-
       13  pi+ -> numu mu+
       14  pi- -> numubar mu-
    */
    newt->Branch("NPi0FinalState", &NPi0FinalState, "NPi0FinalState/I");
    newt->Branch("NGamma", &NGamma, "NGamma/I");
    newt->Branch("wgt", &wgt, "wgt/D");
    newt->Branch("isCC", &isCC, "isCC/I");
    newt->Branch("mode", &mode, "mode/I");             
    // newt->Branch("Vx", &Vx, "Vx/D");             // Neutrino interaction vertex
    // newt->Branch("Vy", &Vy, "Vy/D");
    // newt->Branch("Vz", &Vz, "Vz/D"); 
    // newt->Branch("ParVx",&ParVx,"ParVx/D");      // Neutrino creation vertex
    // newt->Branch("ParVy",&ParVy,"ParVy/D");
    // newt->Branch("ParVz",&ParVz,"ParVz/D");
    // newt->Branch("ParPx",&ParPx,"ParPx/D");      // Neutrino parent momentum
    // newt->Branch("ParPy",&ParPy,"ParPy/D");
    // newt->Branch("ParPz",&ParPz,"ParPz/D");
    newt->Branch("LepTheta", &ThetaLep, "LepTheta/D");
    newt->Branch("LepPhi", &PhiLep, "LepPhi/D");
    newt->Branch("LepThetaSmeared", &ThetaLepSmeared, "LepThetaSmeared/D");
    newt->Branch("LepPhiSmeared", &PhiLepSmeared, "LepPhiSmeared/D");
    newt->Branch("LepE", &Elep, "LepE/D");
    newt->Branch("LepESmeared", &ElepSmeared, "LepESmeared/D");
    newt->Branch("ShowerContainedDistance",&ShowerContainedDistance,"ShowerContainedDistance/D");
    newt->Branch("ShowerDistanceToStart",  &ShowerDistanceToStart,  "ShowerDistanceToStart/D");
    newt->Branch("ShowerDistanceToStartYZ",&ShowerDistanceToStartYZ,"ShowerDistanceToStartYZ/D");
  
    newt->Branch("Eff", &efficiency, "Efficiency/D");                                // Reconstruction efficiency
    newt->Branch("FluxWgt", &fluxweight, "FluxWeight/D");                            // Flux reweight
    newt->Branch("Potweight",&potweight, "Potweight/D");
    // newt->Branch("ConversionDist", "ConversionDist", &ConversionDist, 32000, 0);     // Photon conversion distance in pizero events
    // newt->Branch("LepPx",&LepPx,"LepPx/D");                                          // lepton kinematics
    // newt->Branch("LepPy",&LepPy,"LepPy/D");
    // newt->Branch("LepPz",&LepPz,"LepPz/D");
    // newt->Branch("LepAngle",&LepAngle,"LepAngle/D");
    newt->Branch("ElecCandEnergy",&electron_cand_energy,"ElectronCandEnergy/D");     // True energy of electron/photon/muon that is taken for an electron
    newt->Branch("ElecCandAngle",&electron_cand_angle,"ElectronCandAngle/D");        // True angle of electron/photon/muon that is taken for an electron
    newt->Branch("Eccqe",&enuccqe,"Eccqe/D");                                        // Neutrino energy assuming CCQE kinematics
    newt->Branch("Ecalo1",&enucalo1,"Ecalo1/D");                                     // Neutrino energy using calorimetric reconstruction
    newt->Branch("Ecalo2",&enucalo2,"Ecalo2/D");
    newt->Branch("NCpi0En",&ncpi0en,"NCpi0En/D");                                     // pi0energy
    newt->Branch("VertexEnergy",&vertexEnergy);
    newt->Branch("showerGap",&showerGap);
    newt->Branch("Pi0Shower1P",&shower1P);
    newt->Branch("Pi0Shower2P",&shower2P);
    newt->Branch("Pi0Shower1X",&shower1X);
    newt->Branch("Pi0Shower2X",&shower2X);
    newt->Branch("NChargedKaon", &NChargedKaon,"NChargedKaon/I");
    newt->Branch("NNeutralKaon", &NNeutralKaon,"NNeutralKaon/I");

    newt->Branch("Topology",&iTop,"Topology/I");
    newt->Branch("Channel",&iChan,"Channel/I");
    newt->Branch("IsFiducial",&isFid);
    newt->Branch("IsActive",&isActive);
    newt->Branch("MuonExitPos",&MuonExitPos);
    newt->Branch("MuonExitMom",&MuonExitMom);


    if (signal != "fosc" || skipMultiWeights)
      newt->Branch("MultiWeight",&MultiWeight);


    //-----------------------------------------------------------------
    // These are variables that do event rates for the topology counters
    // Enumerating the channels and topologies.
    // Can have CC, NC, or other type events

    // Can further distinguish:
    // iChan = 1000 //CC with 0 charged pions, 0 protons, 0 neutrons
    // iChan = 1001 //CC with 0 charged pions, 0 protons, 1 or more neutrons
    // iChan = 1010 //CC with 0 charged pions, 1 proton,  0 neutrons
    // iChan = 1011 //CC with 0 charged pions, 1 proton,  1 or more neutrons
    // iChan = 1020 //CC with 0 charged pions, 2 protons, 0 neutrons
    // iChan = 1021 //CC with 0 charged pions, 2 protons, 1 or more neutrons
    // iChan = 1030 //CC with 0 charged pions, 3+ protons, 0 neutrons
    // iChan = 1031 //CC with 0 charged pions, 3+ protons, 1 or more neutrons
    // 
    // 
    // iChan = 1100 //CC with 1 charged pions, 0 protons, 0 neutrons
    // iChan = 1101 //CC with 1 charged pions, 0 protons, 1 or more neutrons
    // iChan = 1110 //CC with 1 charged pions, 1 proton,  0 neutrons
    // iChan = 1111 //CC with 1 charged pions, 1 proton,  1 or more neutrons
    // iChan = 1120 //CC with 1 charged pions, 2+ protons, 0 neutrons
    // iChan = 1121 //CC with 1 charged pions, 2+ protons, 1 or more neutrons
    // 
    // iChan = 1200 //CC with 2+ charged pions, 0 protons, 0 neutrons
    // iChan = 1201 //CC with 2+ charged pions, 0 protons, 1 or more neutrons
    // iChan = 1210 //CC with 2+ charged pions, 1 proton,  0 neutrons
    // iChan = 1211 //CC with 2+ charged pions, 1 proton,  1 or more neutrons
    // iChan = 1220 //CC with 2+ charged pions, 2+ protons, 0 neutrons
    // iChan = 1221 //CC with 2+ charged pions, 2+ protons, 1 or more neutrons
    // 
    // 
    // Enumerate NC events:
    // iChan = 2000 //NC with 0 charged pions, 0 protons, 0 neutrons
    // iChan = 2001 //NC with 0 charged pions, 0 protons, 1 or more neutrons
    // iChan = 2010 //NC with 0 charged pions, 1 proton,  0 neutrons
    // iChan = 2011 //NC with 0 charged pions, 1 proton,  1 or more neutrons
    // iChan = 2020 //NC with 0 charged pions, 2 protons, 0 neutrons
    // iChan = 2021 //NC with 0 charged pions, 2 protons, 1 or more neutrons
    // iChan = 2030 //NC with 0 charged pions, 3+ protons, 0 neutrons
    // iChan = 2031 //NC with 0 charged pions, 3+ protons, 1 or more neutrons
    // 
    // 
    // iChan = 2100 //NC with 1 charged pions, 0 protons, 0 neutrons
    // iChan = 2101 //NC with 1 charged pions, 0 protons, 1 or more neutrons
    // iChan = 2110 //NC with 1 charged pions, 1 proton,  0 neutrons
    // iChan = 2111 //NC with 1 charged pions, 1 proton,  1 or more neutrons
    // iChan = 2120 //NC with 1 charged pions, 2+ protons, 0 neutrons
    // iChan = 2121 //NC with 1 charged pions, 2+ protons, 1 or more neutrons
    // 
    // iChan = 2200 //NC with 2+ charged pions, 0 protons, 0 neutrons
    // iChan = 2201 //NC with 2+ charged pions, 0 protons, 1 or more neutrons
    // iChan = 2210 //NC with 2+ charged pions, 1 proton,  0 neutrons
    // iChan = 2211 //NC with 2+ charged pions, 1 proton,  1 or more neutrons
    // iChan = 2220 //NC with 2+ charged pions, 2+ protons, 0 neutrons
    // iChan = 2221 //NC with 2+ charged pions, 2+ protons, 1 or more neutrons
    // 
    // And anything else gets iChan = 0 for now.
    // 
    // Can also enumerate on some topologies:
    // 
    // So far just catching the following events:
    // iTop = 0: nu + Ar -> 0 EM shower + X
    // iTop = 1: nu + Ar -> 1 EM shower + X
    // iTop = 2: nu + Ar -> 2 EM shower + X
    // iTop = 3: nu + Ar -> 3 EM shower + X
    // iTop = 4: nu + Ar -> 4 EM shower + X
    // Showers can come from electrons or contained photons (convert inside fiducial volume)
    // CC events ought to have iTop >= 1


    //-----------------------------------------------------------------
    // initialize counters and histograms

    double muonCC = 0;
    double electronCC = 0;
    double NCpizero = 0;
    double foundBothPhotons = 0, foundOnePhoton = 0, foundNoPhotons = 0; 
    double multiPizeroEvents = 0;
    double NCsinglePhoton = 0;
    // std::vector<double> singlePhotonE, singlePhotonTheta;

    double NCpizeroForNCDis = 0;

    double emin = 0.0, emax = 3.0;
    if (signal == "numi") emax = 6.0;
    int ebins = 120;

    // These histograms contain the background plots for all of the analysis produced
    // but in particular nue.


    // true neutrino energy
    TH1D *nueFromMuonDecay = new TH1D("NueFromMuonDecay","NueFromMuonDecay;Generated Neutrino Energy (GeV);Events",ebins,emin,emax);
    TH1D *nueFromK0Decay = new TH1D("NueFromK0Decay","NueFromK0Decay;Generated Neutrino Energy (GeV);Events",ebins,emin,emax);
    TH1D *nueFromKPlusDecay = new TH1D("NueFromKPlusDecay","NueFromKPlusDecay;Generated Neutrino Energy (GeV);Events",ebins,emin,emax);
    TH1D *singlePhotonPi0 = new TH1D("SinglePhotonPi0", "SinglePhotonPi0;Generated Neutrino Energy (GeV);Events",ebins,emin,emax);
    TH1D *singlePhotonNC = new TH1D("SinglePhotonNC", "SinglePhotonNC;Generated Neutrino Energy (GeV);Events",ebins,emin,emax);
    TH1D *numuCC = new TH1D("NumuCC","NumuCC;Generated Neutrino Energy (GeV);Events",ebins,emin,emax);
    TH1D *numuElec = new TH1D("NumuElec","NumuElec;Generated Neutrino Energy (GeV);Events",ebins,emin,emax);
    TH1D *nueTotal = new TH1D("NueTrueEnergy","NueTrueEnergy;Generated Neutrino Energy (GeV);Events",ebins,emin,emax);
   
    nueFromMuonDecay->SetFillColor(29);
    nueFromK0Decay->SetFillColor(32);
    nueFromKPlusDecay->SetFillColor(30);
    singlePhotonPi0->SetFillColor(46);
    singlePhotonNC->SetFillColor(42);
    numuCC->SetFillColor(38);
    numuElec->SetFillColor(39);

    THStack *nueBackgrounds = new THStack("NueBackgrounds","Electron Backgrounds, Neutrino Energy;Generated Neutrino Energy (GeV);Events");
    nueBackgrounds->Add(numuCC);
    nueBackgrounds->Add(numuElec);
    nueBackgrounds->Add(singlePhotonNC);
    nueBackgrounds->Add(singlePhotonPi0);
    nueBackgrounds->Add(nueFromK0Decay);
    nueBackgrounds->Add(nueFromKPlusDecay);
    nueBackgrounds->Add(nueFromMuonDecay);


    // true lepton/photon energy
    TH1D *nueFromMuonDecayLepE = new TH1D("NueFromMuonDecayLepE","NueFromMuonDecay;Electron Energy (GeV);Events",ebins,emin,emax);
    TH1D *nueFromK0DecayLepE = new TH1D("NueFromK0DecayLepE","NueFromK0Decay;Electron Energy (GeV);Events",ebins,emin,emax);
    TH1D *nueFromKPlusDecayLepE = new TH1D("NueFromKPlusDecayLepE","NueFromKPlusDecay;Electron Energy (GeV);Events",ebins,emin,emax);
    TH1D *singlePhotonPi0PhoE = new TH1D("SinglePhotonPi0PhoE", "SinglePhotonPi0;Photon Energy (GeV);Events",ebins,emin,emax);
    TH1D *singlePhotonNCPhoE = new TH1D("SinglePhotonNCPhoE", "SinglePhotonPi0;Photon Energy (GeV);Events",ebins,emin,emax);
    TH1D *numuCCLepE = new TH1D("NumuCCLepE", "NumuCCLepE;Muon Energy (GeV);Events",ebins,emin,emax);
    TH1D *numuElecLepE = new TH1D("NumuElecLepE","NumuElecLepE;Generated Neutrino Energy (GeV);Events",ebins,emin,emax);
    
    nueFromMuonDecayLepE->SetFillColor(29);
    nueFromK0DecayLepE->SetFillColor(32);
    nueFromKPlusDecayLepE->SetFillColor(30);
    singlePhotonPi0PhoE->SetFillColor(46);
    singlePhotonNCPhoE->SetFillColor(42);
    numuCCLepE->SetFillColor(38);
    numuElecLepE->SetFillColor(39);

    THStack *nueBackgroundsLepE = new THStack("NueBackgroundsLepE","Electron Backgrounds, Lepton Energy;True Lepton/Photon Energy (GeV);Events");
    nueBackgroundsLepE->Add(numuCCLepE);
    nueBackgroundsLepE->Add(numuElecLepE);
    nueBackgroundsLepE->Add(singlePhotonNCPhoE);
    nueBackgroundsLepE->Add(singlePhotonPi0PhoE);
    nueBackgroundsLepE->Add(nueFromK0DecayLepE);
    nueBackgroundsLepE->Add(nueFromKPlusDecayLepE);
    nueBackgroundsLepE->Add(nueFromMuonDecayLepE);


    // ccqe energy
    TH1D *nueFromMuonDecayCCQE = new TH1D("NueFromMuonDecayCCQE","NueFromMuonDecay;E_{#nu}^{CCQE} (GeV);Events",ebins,emin,emax);
    TH1D *nueFromK0DecayCCQE = new TH1D("NueFromK0DecayCCQE","NueFromK0Decay;E_{#nu}^{CCQE} (GeV);Events",ebins,emin,emax);
    TH1D *nueFromKPlusDecayCCQE = new TH1D("NueFromKPlusDecayCCQE","NueFromKPlusDecay;E_{#nu}^{CCQE} (GeV);Events",ebins,emin,emax);
    TH1D *singlePhotonPi0CCQE = new TH1D("SinglePhotonPi0CCQE", "SinglePhotonPi0;E_{#nu}^{CCQE} (GeV);Events",ebins,emin,emax);
    TH1D *singlePhotonNCCCQE = new TH1D("SinglePhotonNCCCQE", "SinglePhotonPi0;E_{#nu}^{CCQE} (GeV);Events",ebins,emin,emax);
    TH1D *numuCCCCQE = new TH1D("NumuCCCCQE", "NumuCCCCQE;E_{#nu}^{CCQE} (GeV);Events",ebins,emin,emax);
    TH1D *numuElecCCQE = new TH1D("NumuElecCCQE","NumuElecCCQE;Generated Neutrino Energy (GeV);Events",ebins,emin,emax);

    nueFromMuonDecayCCQE->SetFillColor(29);
    nueFromK0DecayCCQE->SetFillColor(32);
    nueFromKPlusDecayCCQE->SetFillColor(30);
    singlePhotonPi0CCQE->SetFillColor(46);
    singlePhotonNCCCQE->SetFillColor(42);
    numuCCCCQE->SetFillColor(38);
    numuElecCCQE->SetFillColor(39);

    THStack *nueBackgroundsCCQE = new THStack("NueBackgroundsCCQE","Electron Backgrounds, CCQE Energy;E_{#nu}^{CCQE} (GeV);Events");
    nueBackgroundsCCQE->Add(numuCCCCQE);
    nueBackgroundsCCQE->Add(numuElecCCQE);
    nueBackgroundsCCQE->Add(singlePhotonNCCCQE);
    nueBackgroundsCCQE->Add(singlePhotonPi0CCQE);
    nueBackgroundsCCQE->Add(nueFromK0DecayCCQE);
    nueBackgroundsCCQE->Add(nueFromKPlusDecayCCQE);
    nueBackgroundsCCQE->Add(nueFromMuonDecayCCQE);


    // calorimetric energy
    TH1D *nueFromMuonDecayCalo = new TH1D("NueFromMuonDecayCalo","NueFromMuonDecay;E_{#nu}^{Calo} (GeV);Events",ebins,emin,emax);
    TH1D *nueFromK0DecayCalo = new TH1D("NueFromK0DecayCalo","NueFromK0Decay;E_{#nu}^{Calo} (GeV);Events",ebins,emin,emax);
    TH1D *nueFromKPlusDecayCalo = new TH1D("NueFromKPlusDecayCalo","NueFromKPlusDecay;E_{#nu}^{Calo} (GeV);Events",ebins,emin,emax);
    TH1D *singlePhotonPi0Calo = new TH1D("SinglePhotonPi0Calo", "SinglePhotonPi0;E_{#nu}^{Calo} (GeV);Events",ebins,emin,emax);
    TH1D *singlePhotonNCCalo = new TH1D("SinglePhotonNCCalo", "SinglePhotonPi0;E_{#nu}^{Calo} (GeV);Events",ebins,emin,emax);
    TH1D *numuCCCalo = new TH1D("NumuCCCalo", "NumuCCCalo;E_{#nu}^{Calo} (GeV);Events",ebins,emin,emax);
    TH1D *numuElecCalo = new TH1D("NumuElecCalo","NumuElecCalo;E_{#nu}^{Calo} (GeV);Events",ebins,emin,emax);

    nueFromMuonDecayCalo->SetFillColor(29);
    nueFromK0DecayCalo->SetFillColor(32);
    nueFromKPlusDecayCalo->SetFillColor(30);
    singlePhotonPi0Calo->SetFillColor(46);
    singlePhotonNCCalo->SetFillColor(42);
    numuCCCalo->SetFillColor(38);
    numuElecCalo->SetFillColor(39);

    THStack *nueBackgroundsCalo = new THStack("NueBackgroundsCalo","Electron Backgrounds, Calo Energy;E_{#nu}^{Calo} (GeV);Events");
    nueBackgroundsCalo->Add(numuCCCalo);
    nueBackgroundsCalo->Add(numuElecCalo);
    nueBackgroundsCalo->Add(singlePhotonNCCalo);
    nueBackgroundsCalo->Add(singlePhotonPi0Calo);
    nueBackgroundsCalo->Add(nueFromK0DecayCalo);
    nueBackgroundsCalo->Add(nueFromKPlusDecayCalo);
    nueBackgroundsCalo->Add(nueFromMuonDecayCalo);


    // these counters keep track of backgrounds in the low-energy region (200-475 MeV CCQE energy)
    Double_t NnueFromMuonDecay_LE = 0;
    Double_t NnueFromK0Decay_LE = 0;
    Double_t NnueFromKPlusDecay_LE = 0;
    Double_t NsinglePhotonPi0_LE = 0;
    Double_t NsinglePhotonNC_LE = 0;
    Double_t NnumuCC_LE = 0;
    Double_t NnumuElec_LE = 0;


    // Some plots for detector comparisons
    // The "beam spot" plots: event vertex in the fiducial volume (no cuts)
    TH2D * beamSpot = new TH2D("BeamSpot","Event vertex Distribution",250,xmin,xmax,250,ymin,ymax);

    // vertex energy in NC pizero single photon events
    TH1D *NCpizeroVtxKE = new TH1D("NCpizeroVtxKE","NCpizeroVtxKE;Vtx Kinetic Energy (GeV)",50,0,500);
    TH1D *NCpizeroConvD = new TH1D("NCpizeroConvD","NCpizeroConvD;Photon Conversion Distance (cm)",100,0,100);
    TH2D *NCpizeroVtxKEConvD = new TH2D("NCpizeroVtxKEConvD","NCpizeroVtxKEConvD;Vtx Kinetic Energy (GeV);Photon Conv. Dist. (cm)",50,0,500,100,0,100);

    // True Pion, Muon energy
    TH1D *ChargedPionEnergy = new TH1D("ChargedPionEnergy", "ChargedPionEnergy; Pion Energy (GeV);Events", 60, 0.0, 3);
    TH1D *MuonEnergy = new TH1D("MuonEnergy", "MuonEnergy; Muon Kinetic Energy (GeV);Events", 60, 0.0, 3);
    ChargedPionEnergy -> SetFillColor(29);
    MuonEnergy -> SetFillColor(32);

    TH2D * elecAngleVsEnergy = new TH2D("ElectronAngleVsEnergy",
                                        "ElectronAngleVsEnergy;Electron Energy (GeV);Angle (Deg)",
                                        ebins,emin,emax,30,0,180);

    TH1D  * ElectDistToStartHist   =new TH1D("ShowerDistanceToStart","ShowerDistanceToStart",150,0,1100);
    TH1D  * ElectDistToStartYZHist =new TH1D("ShowerDistanceToStartYZ","ShowerDistanceToStartYZ",150,0,1100);
    
    TH2D *photonCCQE             = new TH2D("PhotonCCQE","PhotonCCQE;CCQE Energy (GeV);Photon Energy (GeV)",ebins,emin,emax,20*ebins,emin,emax);
    TH2D *CcqeVsTrueE            = new TH2D("CcqeVsTrueE","CcqeVsTrueE;Generated Neutrino Energy (GeV);CCQE Energy (GeV)",30,0,3,30,0,3);
    TH2D *Calo1VsTrueE           = new TH2D("Calo1VsTrueE","Calo1VsTrueE;Generated Neutrino Energy (GeV);Calorimetric Energy - all (GeV)",30,0,3,30,0,3);
    TH2D *Calo2VsTrueE           = new TH2D("Calo2VsTrueE","Calo2VsTrueE;Generated Neutrino Energy (GeV);Calorimetric Energy - no neutrals (GeV)",30,0,3,30,0,3);
    TH3D *photonConv             = new TH3D("PhotonConv","PhotonConv;X;Y;Z",50,xmin,xmax,50,ymin,ymax,100,zmin,zmax);
    TH1D *photonConvX            = new TH1D("PhotonConvX","PhotonConvX;X",50,xmin, xmax);
    TH1D *photonConvY            = new TH1D("PhotonConvY","PhotonConvY;Y",50,ymin, ymax);
    TH1D *photonConvZ            = new TH1D("PhotonConvZ","PhotonConvY;Z",50,zmin, zmax);
    TH3D *ncpi0bkgV              = new TH3D("ncpi0bkgV","ncpi0bkgV;X;Y;Z",50,xmin,xmax,50,ymin,ymax,100,zmin,zmax);
    TH1D *ncpi0bkgX              = new TH1D("ncpi0bkgX","ncpi0bkgX;X",50,xmin,xmax);
    TH1D *ncpi0bkgY              = new TH1D("ncpi0bkgY","ncpi0bkgY;Y",50,ymin,ymax);
    TH1D *ncpi0bkgZ              = new TH1D("ncpi0bkgZ","ncpi0bkgZ;Z",50,zmin,zmax);
    TH2D* singlePhotonNCEnuVsPhotE = new TH2D("singlePhotonNCEnuVsPhotE", "Single photon NC Events;Single Photon Energy;Generated Neutrino Energy (GeV)",100,0,3,100,0,3);

    TH1D * vertexX               = new TH1D("vertexX","vertexX",100, xmin*1.5,xmax*1.5);
    TH1D * vertexY               = new TH1D("vertexY","vertexY",100, ymin*1.5,ymax*1.5);
    TH1D * vertexZ               = new TH1D("vertexZ","vertexZ",100, zmin*1.5,zmax*1.5);
    
    // These are commented out but please don't delete them.
    // Theyre very useful for debugging numi files.
    // TH1D * neutVertexInWindowX   = new TH1D("neutVertexInWindowX","neutVertexInWindowX",100, -5,5);
    // TH1D * neutVertexInWindowY   = new TH1D("neutVertexInWindowY","neutVertexInWindowY",100, -5,5);
    // TH1D * neutVertexInWindowZ   = new TH1D("neutVertexInWindowZ","neutVertexInWindowZ",100, -5,12);
    // TH3D * neutVertexInWindow3D  = new TH3D("neutVertexInWindow3D","neutVertexInWindow3D",100,-5,5,100,-5,5,100,-5,12);

    // Ints to keep track of continue/bails
    int N_continue_CC_muon        = 0;
    int N_continue_Pi0mismatch    = 0;
    int N_continue_Gamma_mismatch = 0;
    int N_continue_foundPhotons   = 0;


    //====================================================
    // Loop over entries in incoming ntuple
    //====================================================
    Long64_t nentries = fChain->GetEntriesFast();
    Long64_t nbytes = 0, nb = 0;
    if( max_entry == -1 ) max_entry = nentries;

    for( Long64_t jentry=0; jentry<nentries && jentry<max_entry; jentry++ ){
    // for( Long64_t jentry=430000; jentry<nentries && jentry<max_entry; jentry++ ){

      // Get the entry
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = GetEntry(jentry);   nbytes += nb;
      iTop = 0;
      iChan = 0;
      NChargedKaon = 0;
      NNeutralKaon = 0;

      if (verbose || jentry % 50000 == 0) 
        std::cout << "========== Event " << ientry << " of " 
                  << nentries << " ==========" << std::endl;
      if (verbose){
        std::cout << "Debugging information ... \n"
                  << "\t inno: " << inno << "\n"
                  << "\t Elep: " << Elep << "\n"
                  << "\t leptonMom->size(): " << leptonMom->size() << "\n"
                  << "\t leptonPos->size(): " << leptonPos->size() << "\n"
                  << "\t p1PhotonConversionMom->size(): " << p1PhotonConversionMom->size() << "\n"
                  << "\t p2PhotonConversionMom->size(): " << p2PhotonConversionMom->size() << "\n"
                  << "\t NPi0FinalState: " << NPi0FinalState << "\n"
                  << "\t miscPhotonConversionPos->size(): " << miscPhotonConversionPos->size() << "\n"
                  << "\t NGamma: " << NGamma << "\n"
                  << "\t egammaThreshold " << egammaThreshold << "\n"
                  << "\t fluxweight: " << fluxweight << "\n";
      }
      if (verbose){
        std::cout << "Genie PDG and 4 vector: \n";
        for (int i = 0; i < GeniePDG->size(); i ++){
          std::cout << "\tPDG : " << GeniePDG->at(i)<< ", ("
                    << GenieMomentum->at(i).E() << ", "
                    << GenieMomentum->at(i).X() << ", "
                    << GenieMomentum->at(i).Y() << ", "
                    << GenieMomentum->at(i).Z() << ")\n";
        }
      }
      if (verbose) std::cout << "interaction = ";
      if( isCC && verbose) std::cout << "CC" << std::endl;
      else if (verbose) std::cout << "NC" << std::endl;

      isFid = utils.IsFiducial( iDet, (*vertex) );
      isActive = utils.IsActive( iDet, (*vertex) );
      if (verbose) std::cout << std::setprecision(5) 
                             << "vertex = (" << vertex->X() << ", " << vertex->Y() 
                             << ", " << vertex->Z() << ") cm" << std::endl;
      if( isActive && verbose) std::cout << "In active detector!" << std::endl;
      if( isFid && verbose) std::cout << "Fiducial event!" << std::endl;


      if (NPi0FinalState != p1PhotonConversionMom->size()){
        std::cout << "Found a case when they are not equal: \n"
                  << "NPi0FinalState: " << NPi0FinalState << "\n"
                  << "p1PhotonConversionMom->size(): " << p1PhotonConversionMom->size()
                  << std::endl;
      }

      // Clear ntuple variables
      ConversionDist.clear();
      efficiency = 0.0;
      wgt = 0.0;
      electron_cand_energy = 0.0;
      electron_cand_angle = 0.0;
      enuccqe = 0.0;
      enucalo1 = 0.0;
      enucalo2 = 0.0;
      ShowerContainedDistance = -1;
      ShowerDistanceToStart = -1;
      ShowerDistanceToStartYZ = -1;
      vertexEnergy = 0;
      showerGap = 0;


      // Calculate the FS lepton's 4-vector and return neutrino path length
      // nuleng = CalcLepton( detect_dist );

      // Get flux weight from FluxRW utilities
      if ( signal != "numi" ) {
        fluxweight = utils.GetFluxWeight(enugen, iflux, inno, ndecay );
      }
      else{ 
        fluxweight = 1.0;
      }
      if( !isActive ) continue;



      // if (!(neutVertexInWindow->Y() > -2.001 && neutVertexInWindow->Y() < -1.999)) continue;

      // Lump in the Pot weight to flux weight so it isn't lost:
      fluxweight *= potweight;

      vertexX               -> Fill(vertex->X(),fluxweight);
      vertexY               -> Fill(vertex->Y(),fluxweight);
      vertexZ               -> Fill(vertex->Z(),fluxweight);


      TVector3 lepDir(1,1,1);
      CalcLepton(lepDir);



      beamSpot -> Fill(vertex->X(),vertex->Y(),fluxweight);

      Double_t photon_energy 
        = utils.TotalPhotonEnergy(iDet, 
                                  p1PhotonConversionPos,  p1PhotonConversionMom, 
                                  p2PhotonConversionPos,  p2PhotonConversionMom,
                                  miscPhotonConversionPos,miscPhotonConversionMom);

      if (utils.IsFiducial( iDet, (*vertex) ,5);)
        vertexEnergy = 1000*utils.VertexEnergy( GeniePDG, GenieMomentum );


      if ( verbose )
        std::cout << "total secondary photon energy: " 
                  << photon_energy << " GeV" << std::endl;
      






      //----------------------------------------------
      // Sort out the genie MC truth info
      // Analyze the charged pion events and event topologies
      //----------------------------------------------
      //Loop over genie final state particles and look for pions:
      int chargedPion = 0;
      int proton = 0;
      int neutron = 0;
      int neutralPion = 0;
      double protonMass = 0.9382; // GeV
      // double pionMass = 0.1395; // GeV

      if (verbose){
        std::cout << "Genie vector sizes: \n"
                  << "\t PDG : " << GeniePDG ->size() << "\n"
                  << "\t   E : " << GenieMomentum->size() << "\n"
                  << "\t  Px : " << GenieMomentum->size() << "\n"
                  << "\t  Py : " << GenieMomentum->size() << "\n"
                  << "\t  Pz : " << GenieMomentum->size() << "\n";
      }
      //Loop over genie final state particles and look for pions:
      for (unsigned int i = 0; i < GeniePDG->size(); ++i)
      {
        double e = GenieMomentum->at(i).E();
        double p = sqrt(GenieMomentum->at(i).X()*GenieMomentum->at(i).X() 
                      + GenieMomentum->at(i).Y()*GenieMomentum->at(i).Y()
                      + GenieMomentum->at(i).Z()*GenieMomentum->at(i).Z() );
                 
        if (GeniePDG->at(i) == 211 || GeniePDG->at(i) == -211){
          if (!isCC) ChargedPionEnergy -> Fill(e, fluxweight);
          chargedPion ++;
        }
        if (GeniePDG->at(i) == 111 ){
          neutralPion ++;
        }
        if (GeniePDG->at(i) == 13 || GeniePDG->at(i) == -13){
          MuonEnergy -> Fill(e, fluxweight);
        }
        if (GeniePDG->at(i) == 2212){ //protons
          //apply a cut on protons:
          double e = GenieMomentum->at(i).E() - protonMass;
          if (e > 0.021) {
            proton ++;
          }
          if (e < 0) print("Error! Energy less than 0!");
        }
        if (GeniePDG->at(i) == 2112){ //neutrons
          neutron ++;
        }
        if (abs(GeniePDG->at(i)) == 311 ){
          NChargedKaon ++;
        }
        if (GeniePDG->at(i) == 310 || GeniePDG->at(i) == 130){
          NNeutralKaon ++;
        }
      }


       //Now set the channel:
      if (isCC) iChan = 1000;
      else if (!isCC) iChan = 2000;

      //Add in the charged pions:
      if (chargedPion == 0){} // Do nothing
      else if (chargedPion == 1) iChan += 100;
      else if (chargedPion >= 2) iChan += 200;
      //Account for the protons:
      if (proton == 0){}  // Do nothing 
      else if (proton == 1) iChan += 10;
      else if (proton == 2) iChan += 20;
      else if (proton >= 3) iChan += 30;
      //Lastly, account for the neutrons:
      if (neutron == 0) {}// Do nothing
      else if (neutron >= 1) iChan += 1;


      //----------------------------------------------
      // Sort the pesky NC pi0.
      // Require that the event went through larg4, so fosc and numi skipped
      //----------------------------------------------
      int totalGammas = 0;
      double photonE = 0;
      double photonTheta = 0;
      double photonConvDist = 0;
      TVector3 photonPos;
      TVector3 photonMom;

      if( signal != "fosc" && signal != "numi" && NPi0FinalState > 0 ){ 

        if (!isCC && verbose) std::cout << "NC event with pizeros: " << NPi0FinalState << std::endl;
        if ( isCC && verbose) std::cout << "CC event with pizeros: " << NPi0FinalState << std::endl;
        if( NPi0FinalState > 1 && !isCC )  multiPizeroEvents += fluxweight;

        if (!FoundPhotons){ 
          std::cout << "Found photons is false, bailing." << std::endl;
          N_continue_foundPhotons++;
          continue;
        }

        // Loop over the final state pi0 and get the conversion info of the photons
        for( int i = 0; i < NPi0FinalState; ++i ){
        
          if ( verbose ) std::cout << "pizero: " << i << std::endl;
          if( !isCC ) NCpizero += fluxweight;


          // Find the position and distance from the vertex for photon conversion:
          TVector3 photon1Pos( p1PhotonConversionPos->at(i).Vect() );
          TVector3 photon1Mom( p1PhotonConversionMom->at(i).Vect() );
          
          TVector3 dist1 = photon1Pos - (*vertex);

          if ( verbose ) {
            std::cout << std::setprecision(5) 
                      << "Photon 1 converted at (" << photon1Pos.X()
                      << ", " << photon1Pos.Y() << ", " << photon1Pos.Z() 
                      << ") cm" << std::endl;
            std::cout << "  in fiducial volume: " 
                      << utils.IsFiducial( iDet, photon1Pos ) 
                      << std::endl;
            std::cout << "  converted " << dist1.Mag() 
                      << " cm from the vertex with energy " 
                      << p1PhotonConversionMom->at(i).E() 
                      << " MeV" << std::endl;
          }

          TVector3 photon2Pos( p2PhotonConversionPos->at(i).Vect() );
          TVector3 photon2Mom( p2PhotonConversionMom->at(i).Vect() );
            
          TVector3 dist2 = photon2Pos - (*vertex);

          if ( verbose ) {
            std::cout << std::setprecision(5) 
                      << "Photon 2 converted at (" << photon2Pos.X()
                      << ", " << photon2Pos.Y() << ", " << photon2Pos.Z() 
                      << ") cm" << std::endl;
            std::cout << "  in fiducial volume: " 
                      << utils.IsFiducial( iDet, photon2Pos ) 
                      << std::endl;
            std::cout << "  converted " << dist2.Mag() 
                      << " cm from the vertex with energy " 
                      << p2PhotonConversionMom->at(i).E() 
                      << " MeV" << std::endl;
          }

          int nfound = 0;
          if(   utils.IsFiducial( iDet, photon1Pos )
             && p1PhotonConversionMom->at(i).E() > egammaThreshold )
          { 
            ConversionDist.push_back( dist1.Mag() ); 
            totalGammas++; 
            photonE = p1PhotonConversionMom->at(i).E() ;
            photonTheta = utils.GetTheta( photon1Mom );
            photonConvDist = dist1.Mag();
            photonPos = photon1Pos;
            photonMom = photon1Mom;
            showerGap = dist1.Mag();
            nfound ++;
          }
          if(   utils.IsFiducial( iDet, photon2Pos ) 
             && p2PhotonConversionMom->at(i).E() > egammaThreshold )
          {  
            ConversionDist.push_back( dist2.Mag() );
            totalGammas++;
            photonE = p2PhotonConversionMom->at(i).E(); 
            photonTheta = utils.GetTheta( photon2Mom );
            photonConvDist = dist2.Mag();
            photonPos = photon2Pos;
            photonMom = photon2Mom;
            showerGap = dist2.Mag();
            nfound++;
          }
              
          if( !isCC ){
            if( nfound == 2 ) foundBothPhotons+=fluxweight;
            if( nfound == 1 ) foundOnePhoton+=fluxweight;
            if( nfound == 0 ) foundNoPhotons+=fluxweight;
          }
        }
      }
      iTop += totalGammas;


      // Loop over any misc gammas and get them too
      if (NGamma > 0 && signal != "fosc" && signal != "numi"){
        for (int i_gamma = 0; i_gamma < NGamma; i_gamma ++){

          // Find the position and distance from the vertex for photon conversion:
          TVector3 photon1Pos( miscPhotonConversionPos->at(i_gamma).Vect() );
          TVector3 photon1Mom( miscPhotonConversionMom->at(i_gamma).Vect() );
          
          TVector3 dist1 = photon1Pos - (*vertex);

          if ( verbose ) {
            std::cout << std::setprecision(5) 
                      << "Misc photon 1 converted at (" << photon1Pos.X()
                      << ", " << photon1Pos.Y() << ", " << photon1Pos.Z() 
                      << ") cm" << std::endl;
            std::cout << "  in fiducial volume: " 
                      << utils.IsFiducial( iDet, photon1Pos ) 
                      << std::endl;
            std::cout << "  converted " << dist1.Mag() 
                      << " cm from the vertex with energy " 
                      << miscPhotonConversionMom->at(i_gamma).E() 
                      << " MeV" << std::endl;
          }
          if(   utils.IsFiducial( iDet, photon1Pos )
             && miscPhotonConversionMom->at(i_gamma).E() > egammaThreshold )
          { 
            ConversionDist.push_back( dist1.Mag() ); 
            totalGammas++; 
            photonE = miscPhotonConversionMom->at(i_gamma).E() ;
            photonTheta = utils.GetTheta( photon1Mom );
            photonConvDist = dist1.Mag();
            photonPos = photon1Pos;
            photonMom = photon1Mom;
            showerGap = dist1.Mag();
          }
        }
      }
      if(totalGammas == 1 && !isCC)
        singlePhotonNCEnuVsPhotE->Fill(photonE,enugen);


      //----------------------------------------------
      // Track the muon till it exits the detector:
      //----------------------------------------------
      bool contained = true;
      unsigned int muonPosIndex = 0;
      // Make sure default values are 0:
      MuonExitPos *= 0.0;
      MuonExitMom *= 0.0;
      if (signal != "fosc" && signal != "numi" && isCC && abs(inno) == 14)  {
        while ( contained && muonPosIndex < leptonPos->size() ){
          //fiducial cut at 5cm
          contained = utils.IsActive(iDet,leptonPos->at(muonPosIndex).Vect(),5);
          muonPosIndex++;
        }
        // Take the data from the previous point.  It's now exiting the detector.
        // Or has stopped!
        MuonExitPos = TVector3( leptonPos->at(muonPosIndex-1).Vect());
        MuonExitMom = TVector3( leptonMom->at(muonPosIndex-1).Vect());
      }


      // Now actually start sorting out events
      // But do it by the category they are in (nue, numu, etc)
      
      // Broken up like this I intend to do them all in one pass eventually

      if (signal == "nue"){
        // This block looks at how events are sorted into the nue backgrounds.
        // #############################
        // Look at intrinsic nues first:
        // #############################
        if (isCC && abs(inno) == 12 && isFid && Elep > egammaThreshold){
          efficiency = electronIDeff;
          wgt = fluxweight*efficiency;
          electron_cand_energy = Elep;
          electron_cand_angle = ThetaLep;
          enuccqe = utils.NuEnergyCCQE( 1000*electron_cand_energy, 
                    sqrt(pow(1000*electron_cand_energy,2) - pow(0.511,2)), 
                    electron_cand_angle, 0.511, iflux )/1000.0;

          enucalo1 = utils.NuEnergyCalo(GeniePDG, GenieMomentum, true, true );
          enucalo2 = utils.NuEnergyCalo(GeniePDG, GenieMomentum, 
                                        false, false, prot_thresh)
                                      + photon_energy;

          if (ndecay < 5 && ndecay > 0){  // K0
            ibkg = 3;
            nueFromK0Decay->Fill( enugen, wgt );
            nueFromK0DecayLepE->Fill( Elep, wgt );
            nueFromK0DecayCCQE->Fill( enuccqe, wgt );
            nueFromK0DecayCalo->Fill( enucalo2, wgt );
            if( enucalo2 > 0.2 && enucalo2 < 0.475 ) NnueFromK0Decay_LE += wgt;
          }
          else if (ndecay > 4 && ndecay < 11){  // K+
            ibkg = 2;
            nueFromKPlusDecay->Fill( enugen, wgt );
            nueFromKPlusDecayLepE->Fill( Elep, wgt );
            nueFromKPlusDecayCCQE->Fill( enuccqe, wgt );
            nueFromKPlusDecayCalo->Fill( enucalo2, wgt );
            if( enucalo2 > 0.2 && enucalo2 < 0.475 ) NnueFromKPlusDecay_LE += wgt;
          }
          else if (ndecay == 11 || ndecay == 12){  // mu
            ibkg = 1;
            nueFromMuonDecay->Fill( enugen, wgt ); 
            nueFromMuonDecayLepE->Fill( Elep, wgt ); 
            nueFromMuonDecayCCQE->Fill( enuccqe, wgt );
            nueFromMuonDecayCalo->Fill( enucalo2, wgt );
            if( enucalo2 > 0.2 && enucalo2 < 0.475 ) NnueFromMuonDecay_LE += wgt;
          }
          ShowerContainedDistance = utils.GetContainedLength(*vertex, lepDir, iDet);
          ShowerDistanceToStart   = utils.GetLengthToStart(  *vertex, lepDir, iDet);
          ShowerDistanceToStartYZ = utils.GetYZLengthToStart(*vertex, lepDir, iDet);

          if (wgt != 0.0){
            if (verbose){
            std::cout << "  For this interaction,\n" 
                      << "    etrue = " << enugen << "\n"
                      << "    eccqe = " << enuccqe << "\n"
                      << "    elep = "  << Elep << "\n"
                      << "    ecalo1 = " << enucalo1 << "\n"
                      << "    ecalo2 = " << enucalo2 << "\n";

            }

            // if (enucalo2 < 0.05) return;
          }

        } // end of intrinsic electrons

        // #############################
        // Look at misID from numu CC:
        // #############################
        if (isCC && abs(inno) == 14){

          if (leptonMom->at(0).E() < egammaThreshold ){
            continue;
          }
          ShowerContainedDistance = utils.GetContainedLength(*vertex, lepDir, iDet);
          ShowerDistanceToStart   = utils.GetLengthToStart(  *vertex, lepDir, iDet);
          ShowerDistanceToStartYZ = utils.GetYZLengthToStart(*vertex, lepDir, iDet);

          ibkg = 7;
          efficiency = muonCCMisID;
          wgt = fluxweight*efficiency;
          
          electron_cand_energy = leptonMom->at(0).E();
          electron_cand_angle = ThetaLep;


          enuccqe = utils.NuEnergyCCQE( 1000*electron_cand_energy, 
                                        sqrt(pow(1000*electron_cand_energy,2) 
                                      - pow(0.511,2)), 
                                        electron_cand_angle, 0.511, iflux )/1000.0;
          enucalo1 = utils.NuEnergyCalo( GeniePDG, GenieMomentum, true, true );
          enucalo2 = utils.NuEnergyCalo( GeniePDG, GenieMomentum, false, false, 
                                         prot_thresh ) + photon_energy;
          Elep = electron_cand_energy;

          numuCC->Fill( enugen, wgt );
          numuCCLepE->Fill( Elep, wgt );
          numuCCCCQE->Fill( enuccqe, wgt );
          numuCCCalo->Fill( enucalo2, wgt );
          if( enucalo2 > 0.2 && enucalo2 < 0.475 ) NnumuCC_LE += wgt;


        } // end of numu CC misid as nue CC


        // #############################
        // Look at misID from NC:
        // #############################
        if (!isCC && totalGammas == 1 && photonE > egammaThreshold){



          ShowerContainedDistance = utils.GetContainedLength(photonPos, photonMom, iDet);
          ShowerDistanceToStart   = utils.GetLengthToStart(  photonPos, photonMom, iDet);
          ShowerDistanceToStartYZ = utils.GetYZLengthToStart(photonPos, photonMom, iDet);
          electron_cand_energy = photonE;
          electron_cand_angle = photonTheta;
          


          enuccqe = utils.NuEnergyCCQE( 1000*electron_cand_energy, 
                                        sqrt(pow(1000*electron_cand_energy,2) 
                                      - pow(0.511,2)), 
                                        electron_cand_angle, 0.511, iflux )/1000.0;
          enucalo1 = utils.NuEnergyCalo( GeniePDG, GenieMomentum, true, true );
          enucalo2 = utils.NuEnergyCalo( GeniePDG, GenieMomentum, false, false, 
                                         prot_thresh ) + photon_energy;
          Elep = electron_cand_energy;


          // In principle, can reject photons with a gap.
          // So, reject photons with vertex energy above the cut
          // AND a gap longer than the cut
          if ( photonConvDist > convDistCut && vertexEnergy > vtxEcut){
            std::cout << "Continuing because of cuts.....\n\n\n";
            std::cout << "  photonConvDist is " << photonConvDist << "\n"
                      << "  vertexEnergy is " << vertexEnergy << "\n";
            continue;

          }

          efficiency = photonMisID;
          wgt = fluxweight*efficiency;

          if(NPi0FinalState > 0 ){
            ibkg = 5;
            if (isFid){
              NCpizeroVtxKE->Fill( vertexEnergy, wgt );
              NCpizeroConvD->Fill( photonConvDist, wgt );
              NCpizeroVtxKEConvD->Fill( vertexEnergy, photonConvDist, wgt );
              if( enucalo2 > 0.2 && enucalo2 < 0.475 )   // analyze LE region
                NsinglePhotonPi0_LE += wgt;
              ncpi0bkgV->Fill( vertex->X(), vertex->Y(), vertex->Z(), wgt );
              ncpi0bkgX->Fill( vertex->X(), wgt );
              ncpi0bkgY->Fill( vertex->Y(), wgt );
              ncpi0bkgZ->Fill( vertex->Z(), wgt );
              singlePhotonPi0->Fill( enugen, wgt ); 
              singlePhotonPi0PhoE->Fill( photonE, wgt );
              singlePhotonPi0CCQE->Fill( enuccqe, wgt );
              singlePhotonPi0Calo->Fill( enucalo2, wgt );
            }
          }
          else if (NGamma > 0){
            ibkg = 6;
            
            singlePhotonNC -> Fill(enugen, wgt);
            singlePhotonNCPhoE -> Fill(photonE, wgt);
            singlePhotonNCCalo -> Fill(enucalo2, wgt);
            singlePhotonNCCCQE -> Fill(enuccqe, wgt);
            NCsinglePhoton += wgt;
            if( enucalo2 > 0.2 && enucalo2 < 0.475 ) NsinglePhotonNC_LE += wgt;
          }

          photonConv->Fill( photonPos.X(), photonPos.Y(), photonPos.Z() );
          photonConvX->Fill( photonPos.X() );
          photonConvY->Fill( photonPos.Y() );
          photonConvZ->Fill( photonPos.Z() );
          photonCCQE->Fill( enuccqe, photonE, wgt );

          // NCsinglePhoton += wgt;
          // singlePhotonE.push_back( photonE );
          // singlePhotonTheta.push_back( photonTheta );

        }
        // #############################
        // Add misID from numu - e scattering:
        // #############################
        if( isFid && abs(inno) == 14 && nuchan == 98 ){
          iTop ++;  //Should be an electron in these events
          iChan = 2000; //This is an other case!

          ibkg = 4;
          efficiency = electronIDeff;
          wgt = fluxweight*efficiency;
        
          CalcLepton(lepDir);

          // std::cout << "This is numu electron scattering, elep is: " << Elep;
          // exit(0);

          electron_cand_energy = Elep;           // !!! don't have electron energy for these events!!
          electron_cand_angle = ThetaLep;               // !!! don't have electron angle, assume small
          enuccqe = utils.NuEnergyCCQE( 1000*electron_cand_energy, 
                                        sqrt(pow(1000*electron_cand_energy,2) 
                                        - pow(0.511,2)), 
                                        electron_cand_angle, 0.511, iflux )/1000.0;

          enucalo1 = utils.NuEnergyCalo( GeniePDG, GenieMomentum, true, true );
          enucalo2 = utils.NuEnergyCalo( GeniePDG, GenieMomentum, false, false, 
                                         prot_thresh ) + photon_energy;

          numuElec->Fill( enugen, wgt );
          numuElecLepE->Fill( enugen, wgt );       // !!! don't have electron energy for these events!!
          numuElecCCQE->Fill( enuccqe, wgt );
          numuElecCalo->Fill( enucalo2, wgt );
          if( enucalo2 > 0.2 && enucalo2 < 0.475 ) 
            NnumuElec_LE += wgt;

          ShowerContainedDistance = utils.GetContainedLength(*vertex, lepDir, iDet);
          ShowerDistanceToStart   = utils.GetLengthToStart(  *vertex, lepDir, iDet);
          ShowerDistanceToStartYZ = utils.GetYZLengthToStart(*vertex, lepDir, iDet);

        } 


        if (wgt != 0.0){
          elecAngleVsEnergy      -> Fill( Elep, ThetaLep, wgt);
          ElectDistToStartHist   -> Fill( ShowerDistanceToStart,wgt);
          ElectDistToStartYZHist -> Fill( ShowerDistanceToStartYZ,wgt);
          CcqeVsTrueE            -> Fill( enugen, enuccqe, wgt );
          Calo1VsTrueE           -> Fill( enugen, enucalo1, wgt );
          Calo2VsTrueE           -> Fill( enugen, enucalo2, wgt );
          nueTotal               -> Fill( enugen,wgt);

        }

      } // end of signal == nue


      if (signal == "numu"){

        if ( isFid && isCC && abs(inno) == 14 && 
             leptonMom->at(0).E() > egammaThreshold ){

          efficiency = muonIDeff;
          wgt = fluxweight*efficiency;

          electron_cand_energy = leptonMom->at(0).E();
          electron_cand_angle = ThetaLep;

          enuccqe = utils.NuEnergyCCQE( 1000*electron_cand_energy,
                                        sqrt(pow(1000*electron_cand_energy,2)
                                           - pow(105.7,2)),
                                        electron_cand_angle,
                                        105.7,
                                        iflux )/1000.0;
          enucalo1 = utils.NuEnergyCalo( GeniePDG,
                                         GenieMomentum,
                                         true,
                                         true );
          enucalo2 = utils.NuEnergyCalo( GeniePDG,
                                         GenieMomentum,
                                         false,
                                         false,
                                         prot_thresh ) + photon_energy;
          numuCC->Fill(enugen, wgt);
          numuCCLepE->Fill( leptonMom->at(0).E(), wgt );
        }
      }
      if (signal == "fosc"){

      }

      if (signal == "phot"){
        if (!isCC && totalGammas == 1 && photonE > egammaThreshold){

          ShowerContainedDistance = utils.GetContainedLength(photonPos, photonMom, iDet);
          ShowerDistanceToStart   = utils.GetLengthToStart(  photonPos, photonMom, iDet);
          ShowerDistanceToStartYZ = utils.GetYZLengthToStart(photonPos, photonMom, iDet);
          electron_cand_energy = photonE;
          electron_cand_angle = photonTheta;

          Elep = electron_cand_energy;


          // In principle, can reject photons with a gap.
          // So, reject photons with vertex energy above the cut
          // AND a gap longer than the cut
          if ( photonConvDist > convDistCut && vertexEnergy > vtxEcut){
            std::cout << "Continuing because of cuts.....\n\n\n";
            std::cout << "  photonConvDist is " << photonConvDist << "\n"
                      << "  vertexEnergy is " << vertexEnergy << "\n";
            continue;

          }

          efficiency = electronIDeff;
          wgt = fluxweight*efficiency;

          if(NPi0FinalState > 0 ){
            ibkg = 5;
            if (isFid){
              NCpizeroVtxKE->Fill( vertexEnergy, wgt );
              NCpizeroConvD->Fill( photonConvDist, wgt );
              NCpizeroVtxKEConvD->Fill( vertexEnergy, photonConvDist, wgt );
              if( enucalo2 > 0.2 && enucalo2 < 0.475 )   // analyze LE region
                NsinglePhotonPi0_LE += wgt;
              ncpi0bkgV->Fill( vertex->X(), vertex->Y(), vertex->Z(), wgt );
              ncpi0bkgX->Fill( vertex->X(), wgt );
              ncpi0bkgY->Fill( vertex->Y(), wgt );
              ncpi0bkgZ->Fill( vertex->Z(), wgt );
              singlePhotonPi0->Fill( enugen, wgt ); 
              singlePhotonPi0PhoE->Fill( photonE, wgt );
            }
          }
          else if (NGamma > 0){
            singlePhotonNC -> Fill(enugen, wgt);
            singlePhotonNCPhoE -> Fill(photonE, wgt);
          }

          photonConv->Fill( photonPos.X(), photonPos.Y(), photonPos.Z() );
          photonConvX->Fill( photonPos.X() );
          photonConvY->Fill( photonPos.Y() );
          photonConvZ->Fill( photonPos.Z() );
          photonCCQE->Fill( enuccqe, photonE, wgt );

          // NCsinglePhoton += wgt;
          // singlePhotonE.push_back( photonE );
          // singlePhotonTheta.push_back( photonTheta );
        }        
      }

      if (signal == "numi")
      {
        if (isCC && abs(inno) == 12 && isFid && Elep > egammaThreshold){
          efficiency = electronIDeff;
          wgt = fluxweight*efficiency;
          electron_cand_energy = Elep;
          electron_cand_angle = ThetaLep;
          enuccqe = utils.NuEnergyCCQE( 1000*electron_cand_energy, 
                    sqrt(pow(1000*electron_cand_energy,2) - pow(0.511,2)), 
                    electron_cand_angle, 0.511, iflux )/1000.0;

          enucalo1 = utils.NuEnergyCalo(GeniePDG, GenieMomentum, true, true );
          enucalo2 = utils.NuEnergyCalo(GeniePDG, GenieMomentum, 
                                        false, false, prot_thresh)
                                      + photon_energy;

          if (ndecay < 5 && ndecay > 0){  // K0
            ibkg = 3;
            nueFromK0Decay->Fill( enugen, wgt );
            nueFromK0DecayLepE->Fill( Elep, wgt );
            nueFromK0DecayCCQE->Fill( enuccqe, wgt );
            nueFromK0DecayCalo->Fill( enucalo2, wgt );
            if( enucalo2 > 0.2 && enucalo2 < 0.475 ) NnueFromK0Decay_LE += wgt;
          }
          else if (ndecay > 4 && ndecay < 11){  // K+
            ibkg = 2;
            nueFromKPlusDecay->Fill( enugen, wgt );
            nueFromKPlusDecayLepE->Fill( Elep, wgt );
            nueFromKPlusDecayCCQE->Fill( enuccqe, wgt );
            nueFromKPlusDecayCalo->Fill( enucalo2, wgt );
            if( enucalo2 > 0.2 && enucalo2 < 0.475 ) NnueFromKPlusDecay_LE += wgt;
          }
          else if (ndecay == 11 || ndecay == 12){  // mu
            ibkg = 1;
            nueFromMuonDecay->Fill( enugen, wgt ); 
            nueFromMuonDecayLepE->Fill( Elep, wgt ); 
            nueFromMuonDecayCCQE->Fill( enuccqe, wgt );
            nueFromMuonDecayCalo->Fill( enucalo2, wgt );
            if( enucalo2 > 0.2 && enucalo2 < 0.475 ) NnueFromMuonDecay_LE += wgt;
          }
          ShowerContainedDistance = utils.GetContainedLength(*vertex, lepDir, iDet);
          ShowerDistanceToStart   = utils.GetLengthToStart(  *vertex, lepDir, iDet);
          ShowerDistanceToStartYZ = utils.GetYZLengthToStart(*vertex, lepDir, iDet);
        }

        if ( isFid && isCC && abs(inno) == 14 && 
             Elep > egammaThreshold ){

          efficiency = muonIDeff;
          wgt = fluxweight*efficiency;

          electron_cand_energy = Elep;
          electron_cand_angle = ThetaLep;

          enuccqe = utils.NuEnergyCCQE( 1000*electron_cand_energy,
                                        sqrt(pow(1000*electron_cand_energy,2)
                                           - pow(105.7,2)),
                                        electron_cand_angle,
                                        105.7,
                                        iflux )/1000.0;
          enucalo1 = utils.NuEnergyCalo( GeniePDG,
                                         GenieMomentum,
                                         true,
                                         true );
          enucalo2 = utils.NuEnergyCalo( GeniePDG,
                                         GenieMomentum,
                                         false,
                                         false,
                                         prot_thresh ) + photon_energy;
          numuCC->Fill(enugen, wgt);
          numuCCLepE->Fill( Elep, wgt );
        }
      }
        
      if (wgt != 0){
        CcqeVsTrueE            -> Fill( enugen, enuccqe, wgt );
        Calo1VsTrueE           -> Fill( enugen, enucalo1, wgt );
        Calo2VsTrueE           -> Fill( enugen, enucalo2, wgt );
        nueTotal               -> Fill( enugen,wgt);
      }      

      newt->Fill();

    } // end loop over entries

    //-- write histograms
    nueBackgrounds->Write();
    nueBackgroundsLepE->Write();
    nueBackgroundsCCQE->Write();
    nueBackgroundsCalo->Write();

    f->Write();
    
    f->Close();

    std::cout << "==================================================" << std::endl;
    std::cout << "Rates for detector " << iDet << std::endl;
    std::cout << "Active volume = " << utils.GetActiveMass( iDet ) << " tons" << std::endl;
    std::cout << "Fiducial volume = " << utils.GetFidMass( iDet ) << " tons" << std::endl;
    std::cout << "==================================================" << std::endl;
    std::cout << "Found " << NCpizeroForNCDis << " pizeros in NC events for dis. study." << std::endl;
    std::cout << "Found " << NCpizero << " pizeros in NC events." << std::endl;
    std::cout << "Found both photons: " << foundBothPhotons << std::endl;
    std::cout << "Found one photon: " << foundOnePhoton << std::endl;
    std::cout << "Found no photons: " << foundNoPhotons << std::endl;
    std::cout << "Multi-pizero events: " << multiPizeroEvents << std::endl;
    std::cout << "==================================================" << std::endl;
    std::cout << "Analyzing " << signal << " events" << std::endl;
    std::cout << "Found " << electronCC << " total electron CC events." << std::endl;
    std::cout << "Found " << muonCC << " total numu CC events." << std::endl;
    std::cout << "Found " << NCsinglePhoton << " single photon NC events." << std::endl;
    std::cout << "==================================================" << std::endl;
    std::cout << "Nue Background Events at Low Energy: 200 - 475 MeV" << std::endl;
    std::cout << "  nue from muon decay  : " << NnueFromMuonDecay_LE << std::endl;
    std::cout << "  nue from Kplus decay : " << NnueFromKPlusDecay_LE << std::endl;
    std::cout << "  nue from K0 decay    : " << NnueFromK0Decay_LE << std::endl;
    std::cout << "  numu CC              : " << NnumuCC_LE << std::endl;
    std::cout << "  numu+elec scattering : " << NnumuElec_LE << std::endl;
    std::cout << "  NC pi0 single photons: " << NsinglePhotonPi0_LE << std::endl;
    std::cout << "  NC single photons    : " << NsinglePhotonNC_LE << std::endl;
    std::cout << "  TOTAL                : " << NnueFromMuonDecay_LE+NnueFromK0Decay_LE+NnueFromKPlusDecay_LE+NsinglePhotonPi0_LE+
      NsinglePhotonNC_LE+NnumuCC_LE+NnumuElec_LE << std::endl;

    // for( unsigned long i = 0; i < singlePhotonE.size(); i++ ){
    // std::cout << " sinlgle photon: " << 1000*singlePhotonE[i] << " MeV, " << singlePhotonTheta[i] << " rad" << std::endl;
    //}
    std::cout << "==================================================" << std::endl;
    std::cout << "Output in " << outfile << std::endl;
    std::cout << "Had to bail on : " << "\n"
              << "N_continue_Pi0mismatch    : " << N_continue_Pi0mismatch << "\n"
              << "N_continue_Gamma_mismatch : " << N_continue_Gamma_mismatch << "\n"
              << "N_continue_foundPhotons   : " << N_continue_foundPhotons << "\n"
              << "N_continue_CC_muon        : " << N_continue_CC_muon << "\n";



}

