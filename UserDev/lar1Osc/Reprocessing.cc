#define Reprocessing_cxx
#include "Reprocessing.hh"

#define print(a) ( std::cout << a << std::endl)


void lar1::Reprocessing::Loop(std::string signal,
                              int iDet,
                              int iLoc,
                              Long64_t max_entry,
                              bool skipMultiWeights,
                              bool verbose){

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
        signal != "rates" &&
        signal != "numi"  ){
      std::cout << "Error: incorrect signal \"" << signal << "\"  used." << std::endl;
      std::cout << "Please see Reprocessing.C for a list of available signals." << std::endl;
      return;
    }

    if (signal == "numi"){
      utils.setMC_Generation(2);
    }

    if (useTwoHornConfig){
        utils.reconfigureTwoHorn(twoHornConfig);
    }

    bool isFosc = false;
    if (signal == "fosc") isFosc = true;

    //---------------------------------------------
    // some things to be configured by the user:
    // 
    double electronIDeff = 0.8;
    double muonIDeff     = 0.8;
    double photonMisID   = 0.06;
    // double muonCCMisID   = 0.001;

    double smearingStats = 20.0;

    // NC photon vertex energy cuts
    double vtxEcut = 99999;     // 0.025;   // GeV
    double convDistCut = 99999; // 5.0;     // cm

    double egammaThreshold = 0.1; // 0.140 // GeV

    bool smearing = true;
    if (signal == "rates") smearing = false;

    Double_t prot_thresh = 0.02;


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
      **/     
       // Nu_e background
       
       int kNueFromNueCC_muon       = 1;
       int kNueFromNueCC_chargeKaon = 2;
       int kNueFromNueCC_neutKaon   = 3;
       int kNueFromEScatter         = 4;
       int kNueFromNC_pi0           = 5;
       int kNueFromNC_singlePhot    = 6;
       int kNueFromNumuCC           = 7;
       int kDirt                    = 8;
       int kOther                   = 9;
       
       // Nu_mu background
       
       int kNumuFromNumuCC          = 0;
       int kNumuFromNCPi            = 1;


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
    if (signal == "fosc") iflux ++;
    if (signal != "numi"){
      potweight = utils.GetPOTNorm( iflux, iLoc );
      std::cout << "POT weight = " << potweight << std::endl;
    }

    std::cout << "Horn configuration is: ";
    if (useTwoHornConfig){
        std::cout << "two horns";
        if (twoHornConfig != "") std::cout << " with " << twoHornConfig << std::endl;
    }
    else
        std::cout << "One horn, nominal configuration." << std::endl;

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

    TH1D *totalEventsFosc = new TH1D("totalEventsFosc","totalEventsFosc",ebins, emin, emax);
    TH1D *totalEventsNumu = new TH1D("totalEventsNumu","totalEventsNumu",ebins, emin, emax);

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
    TH1D *NCpizeroVtxKE = new TH1D("NCpizeroVtxKE","NCpizeroVtxKE;Vtx Kinetic Energy (GeV)",50,0,5);
    TH1D *NCpizeroConvD = new TH1D("NCpizeroConvD","NCpizeroConvD;Photon Conversion Distance (cm)",100,0,100);
    TH2D *NCpizeroVtxKEConvD = new TH2D("NCpizeroVtxKEConvD","NCpizeroVtxKEConvD;Vtx Kinetic Energy (GeV);Photon Conv. Dist. (cm)",50,0,5,100,0,100);

    // True Pion, Muon energy
    TH1D *ChargedPionEnergy = new TH1D("ChargedPionEnergy", "ChargedPionEnergy; Pion Energy (GeV);Events", 180, 0.0, 1.5);
    TH1D *MuonEnergy = new TH1D("MuonEnergy", "MuonEnergy; Muon Kinetic Energy (GeV);Events", 60, 0.0, 3);
    ChargedPionEnergy -> SetFillColor(29);
    MuonEnergy -> SetFillColor(32);

    TH2D * elecAngleVsEnergy = new TH2D("ElectronAngleVsEnergy",
                                        "ElectronAngleVsEnergy;Electron Energy (GeV);Angle (Deg)",
                                        ebins,emin,emax,30,0,180);

    TH1D  * ElectDistToStartHist   =new TH1D("ShowerDistanceToStart","ShowerDistanceToStart",150,0,1100);
    TH1D  * ElectDistToStartYZHist =new TH1D("ShowerDistanceToStartYZ","ShowerDistanceToStartYZ",150,0,1100);
    
    TH2D *photonCCQE             = new TH2D("PhotonCCQE","PhotonCCQE;CCQE Energy (GeV);Photon Energy (GeV)",ebins,emin,emax,20*ebins,emin,emax);
    TH2D *CcqeVsTrueE            = new TH2D("CcqeVsTrueE","CcqeVsTrueE;Generated Neutrino Energy (GeV);CCQE Energy (GeV)",150,0,3,150,0,3);
    TH2D *Calo1VsTrueE           = new TH2D("Calo1VsTrueE","Calo1VsTrueE;Generated Neutrino Energy (GeV);Calorimetric Energy - all (GeV)",150,0,3,150,0,3);
    TH2D *Calo2VsTrueE           = new TH2D("Calo2VsTrueE","Calo2VsTrueE;Generated Neutrino Energy (GeV);Calorimetric Energy - no neutrals (GeV)",150,0,3,150,0,3);
    TH3D *photonConv             = new TH3D("PhotonConv","PhotonConv;X;Y;Z",50,xmin,xmax,50,ymin,ymax,100,zmin,zmax);
    TH1D *photonConvX            = new TH1D("PhotonConvX","PhotonConvX;X",50,xmin, xmax);
    TH1D *photonConvY            = new TH1D("PhotonConvY","PhotonConvY;Y",50,ymin, ymax);
    TH1D *photonConvZ            = new TH1D("PhotonConvZ","PhotonConvY;Z",50,zmin, zmax);
    TH3D *ncpi0bkgV              = new TH3D("ncpi0bkgV","ncpi0bkgV;X;Y;Z",50,xmin,xmax,50,ymin,ymax,100,zmin,zmax);
    TH1D *ncpi0bkgX              = new TH1D("ncpi0bkgX","ncpi0bkgX;X",50,xmin,xmax);
    TH1D *ncpi0bkgY              = new TH1D("ncpi0bkgY","ncpi0bkgY;Y",50,ymin,ymax);
    TH1D *ncpi0bkgZ              = new TH1D("ncpi0bkgZ","ncpi0bkgZ;Z",50,zmin,zmax);
    TH2D *singlePhotonNCEnuVsPhotE 
      = new TH2D("singlePhotonNCEnuVsPhotE",
                 "Single photon NC Events;Single Photon Energy;Generated Neutrino Energy (GeV)"
                 ,100,0,3,100,0,3);

    TH2D *singlePhotonGapVsVertexE     
      = new TH2D("singlePhotonGapVsVertexE", 
        "Single Photon Events;Single Photon Conversion Distance;Vertex Energy (GeV)",
        100,0,5,100,0,1);
    TH2D *allPhotonGapVsVertexE     
      = new TH2D("allPhotonGapVsVertexE", 
        "All Photon Events;Photon Conversion Distance;Vertex Energy (GeV)",
        100,0,5,100,0,1);
    TH2D *vertexEvsGenereratedE  
      = new TH2D("vertexEvsGenereratedE", 
        "All Events;Vertex Energy;Generated Neutrino Energy (GeV)",
        100,0,1,100,0,3);
    TH2D *vertexEvsGenereratedEPi0  
      = new TH2D("vertexEvsGenereratedEPi0", 
        "Single photon NC Events;Vertex Energy;Generated Neutrino Energy (GeV)",
        100,0,1,100,0,3);
    TH1D *VertexEnergyPhot       
      = new TH1D("VertexEnergyPhot",
                 "Vertex Energy of Single Photons;Vertex Energy (GeV)",
                 50,0,1);
    TH1D *ConversionDistPhot       
      = new TH1D("ConversionDistPhot",
                 "Conversion Distance of Single Photons;Conversion Distance (cm)",
                 50,0,50);

    TH2D *LeptonSmearing 
      = new TH2D("LeptonSmearing","Shower Resolution;Shower Energy (GeV);Smeared Shower Energy (GeV)",
                 150,0,3,150,0,3);

    TH1D * vertexX               = new TH1D("vertexX","vertexX",100, xmin*1.5,xmax*1.5);
    TH1D * vertexY               = new TH1D("vertexY","vertexY",100, ymin*1.5,ymax*1.5);
    TH1D * vertexZ               = new TH1D("vertexZ","vertexZ",100, zmin*1.5,zmax*1.5);
    
    TH1D * convPointX    = new TH1D("convPointX","convPointX;X",50,xmin, xmax);
    TH2D * convPointXVsE = new TH2D("convPointXVsE","convPointXVsE",50,xmin,xmax,150,0,0.4);


    // Histograms that are useful for Lariat, request of Flavio:
    // TH1D * protonP = new TH1D("protonP",protonP,50,0,2000);


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

    double totalSkipped = 0;

    double nNumuMisID_total = 0;

    double nNumuCCEvents = 0;
    double nFoscEvents = 0;

    // trying angle dependant flux:
    // double X_center = 0;
    // double Y_center = -34;

    // TH1F * rates_0 = new TH1F("rates_0","rates_0",ebins,emin,emax);
    // TH1F * rates_1 = new TH1F("rates_1","rates_1",ebins,emin,emax);
    // TH1F * rates_2 = new TH1F("rates_2","rates_2",ebins,emin,emax);
    // TH1F * rates_3 = new TH1F("rates_3","rates_3",ebins,emin,emax);
    // TH1F * rates_4 = new TH1F("rates_4","rates_4",ebins,emin,emax);
    // TH1F * rates_5 = new TH1F("rates_5","rates_5",ebins,emin,emax);
    // TH1F * rates_6 = new TH1F("rates_6","rates_6",ebins,emin,emax);
    // TH1F * rates_7 = new TH1F("rates_7","rates_7",ebins,emin,emax);
    // TH1F * rates_8 = new TH1F("rates_8","rates_8",ebins,emin,emax);
    // TH1F * rates_9 = new TH1F("rates_9","rates_9",ebins,emin,emax);

    //====================================================
    // Loop over entries in incoming ntuple
    //====================================================
    Long64_t nentries = fChain->GetEntriesFast();
    Long64_t nbytes = 0, nb = 0;
    if( max_entry == -1 ) max_entry = nentries;

    int smearingStatsIndex = smearingStats;

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

      // verbose = false;
      // if ( NPi0FinalState != 0) verbose = true;


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
        for (unsigned int i = 0; i < GeniePDG->size(); i ++){
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

      isFid = utils.IsFiducial( iDet, (*vertex), signal );
      isActive = utils.IsActive( iDet, (*vertex));
      if (verbose) std::cout << std::setprecision(5) 
                             << "vertex = (" << vertex->X() << ", " << vertex->Y() 
                             << ", " << vertex->Z() << ") cm" << std::endl;
      if( isActive && verbose) std::cout << "In active detector!" << std::endl;
      if( isFid && verbose) std::cout << "Fiducial event!" << std::endl;


      if (signal != "fosc" && NPi0FinalState != p1PhotonConversionMom->size()){
        std::cout << "Found a case when they are not equal: \n"
                  << "NPi0FinalState: " << NPi0FinalState << "\n"
                  << "p1PhotonConversionMom->size(): " << p1PhotonConversionMom->size()
                  << std::endl;
      }

      // Clear ntuple variables
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




      // Get flux weight from FluxRW utilities
      if ( signal != "numi") {
        fluxweight = utils.GetFluxWeight(enugen, iflux, inno, ndecay );
        if (useTwoHornConfig){
          fluxweight *= utils.GetTwoHornWeight(enugen, isFosc, inno, ndecay,iLoc);
        }
      }
      else{ 
        fluxweight = 1.0;
      }

      // only keep active detector events
      if( !isActive ) continue;


      // // Calculate the distance from beam center in lar1nd:
      // double r = 0.0;
      // r += (vertex->X() - X_center)*(vertex->X() - X_center);
      // r += (vertex->Y() - Y_center)*(vertex->Y() - Y_center);
      // r = sqrt(r);
      // if (r < 025) rates_0 -> Fill(enugen, fluxweight);
      // else if (r < 1.25*050) rates_1 -> Fill(enugen, fluxweight);
      // else if (r < 1.25*075) rates_2 -> Fill(enugen, fluxweight);
      // else if (r < 1.25*100) rates_3 -> Fill(enugen, fluxweight);
      // else if (r < 1.25*125) rates_4 -> Fill(enugen, fluxweight);
      // else if (r < 1.25*150) rates_5 -> Fill(enugen, fluxweight);
      // else if (r < 1.25*175) rates_6 -> Fill(enugen, fluxweight);
      // else if (r < 1.25*200) rates_7 -> Fill(enugen, fluxweight);
      // else if (r < 1.25*225) rates_8 -> Fill(enugen, fluxweight);
      // else if (r < 1.25*250) rates_9 -> Fill(enugen, fluxweight);


      // if (!(neutVertexInWindow->Y() > -2.001 && neutVertexInWindow->Y() < -1.999)) continue;

      // Lump in the Pot weight to flux weight so it isn't lost:
      fluxweight *= potweight;

      vertexX               -> Fill(vertex->X(),fluxweight);
      vertexY               -> Fill(vertex->Y(),fluxweight);
      vertexZ               -> Fill(vertex->Z(),fluxweight);


      TVector3 lepDir(1,1,1);
      CalcLepton(lepDir);

      if (isCC){
        totalEventsNumu->Fill(enugen, fluxweight);
        totalEventsFosc->Fill(enugen, fluxweight);

        if (signal == "fosc") nFoscEvents += fluxweight;
        if (signal == "numu") nNumuCCEvents += fluxweight;

      }

      beamSpot -> Fill(vertex->X(),vertex->Y(),fluxweight);

      Double_t photon_energy 
        = utils.TotalPhotonEnergy(iDet, 
                                  p1PhotonConversionPos,  p1PhotonConversionMom, 
                                  p2PhotonConversionPos,  p2PhotonConversionMom,
                                  miscPhotonConversionPos,miscPhotonConversionMom);

      if (utils.IsActive( iDet, (*vertex) ,5))
        vertexEnergy = utils.VertexEnergy( GeniePDG, GenieMomentum,smearing,
                                          prot_thresh,0,0,verbose);
      else
        vertexEnergy = 0.0;


      if ( verbose ){
        std::cout << "total secondary photon energy: " 
                  << photon_energy << " GeV" << std::endl
                  << "total vertex energy: " << vertexEnergy << std::endl;
      }
      






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
      double pionMass = 0.1395; // GeV

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
        // double p = sqrt(GenieMomentum->at(i).X()*GenieMomentum->at(i).X() 
        //               + GenieMomentum->at(i).Y()*GenieMomentum->at(i).Y()
        //               + GenieMomentum->at(i).Z()*GenieMomentum->at(i).Z() );
                 
        if (GeniePDG->at(i) == 211 || GeniePDG->at(i) == -211){
          if (!isCC) ChargedPionEnergy -> Fill(e-pionMass, fluxweight);
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
/*
      switch (nuchan){
        case 53:
          std::cout << "Got nuchan 53\n";
          break;
        case 54:
          std::cout << "Got nuchan 54\n";
          break;
        case 55:
          std::cout << "Got nuchan 55\n";
          break;
        case 73:
          std::cout << "Got nuchan 73\n";
          break;
        case 95:
          std::cout << "Got nuchan 95 - cabibo surpressed hyperon\n";
          break;
        // case 97:
        //   std::cout << "Got nuchan 97 - coherent\n";
        //   break;
        case 98:
          std::cout << "Got nuchan 98 - nu-e scattering\n";
          break;
        case 99:
          std::cout << "Got nuchan 99 - inverse mu decay\n";
          break;
        default:
          break;
      }
*/

      if (signal == "rates"){
        if (isActive) newt->Fill();
        continue;
      }


      //----------------------------------------------
      // Sort the pesky NC pi0.
      // Require that the event went through larg4, so fosc and numi skipped
      //----------------------------------------------
      int totalGammas = 0;
      int totalActiveGammas = 0;
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
          if( !isCC ) NCpizero ++;


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
                      << utils.IsFiducial( iDet, photon1Pos, signal ) 
                      << std::endl;
            std::cout << "  converted " << dist1.Mag() 
                      << " cm from the vertex with energy " 
                      << p1PhotonConversionMom->at(i).E() 
                      << " MeV" << std::endl;
          }

          TVector3 photon2Pos( p2PhotonConversionPos->at(i).Vect() );
          TVector3 photon2Mom( p2PhotonConversionMom->at(i).Vect() );

          convPointX -> Fill(photon1Pos.X());
          convPointXVsE -> Fill(photon1Pos.X(),p1PhotonConversionMom->at(i).E());
          convPointX -> Fill(photon2Pos.X());
          convPointXVsE -> Fill(photon2Pos.X(),p2PhotonConversionMom->at(i).E());
            
          TVector3 dist2 = photon2Pos - (*vertex);

          if ( verbose ) {
            std::cout << std::setprecision(5) 
                      << "Photon 2 converted at (" << photon2Pos.X()
                      << ", " << photon2Pos.Y() << ", " << photon2Pos.Z() 
                      << ") cm" << std::endl;
            std::cout << "  in fiducial volume: " 
                      << utils.IsFiducial( iDet, photon2Pos, signal ) 
                      << std::endl;
            std::cout << "  converted " << dist2.Mag() 
                      << " cm from the vertex with energy " 
                      << p2PhotonConversionMom->at(i).E() 
                      << " MeV" << std::endl;
          }

          int nfound = 0;
          if(   utils.IsFiducial( iDet, photon1Pos, signal )
             && p1PhotonConversionMom->at(i).E() > egammaThreshold )
          { 
            totalGammas++; 
            photonE = p1PhotonConversionMom->at(i).E() ;
            photonTheta = utils.GetTheta( photon1Mom );
            photonConvDist = dist1.Mag();
            photonPos = photon1Pos;
            photonMom = photon1Mom;
            showerGap = dist1.Mag();
            nfound ++;
            allPhotonGapVsVertexE -> Fill(showerGap,vertexEnergy);
          }
          else if (utils.IsActive(iDet,photon1Pos,5) &&
                   p1PhotonConversionMom->at(i).E() > egammaThreshold )
          {
            totalActiveGammas ++;
          }
          if(   utils.IsFiducial( iDet, photon2Pos, signal ) 
             && p2PhotonConversionMom->at(i).E() > egammaThreshold )
          {  
            totalGammas++;
            photonE = p2PhotonConversionMom->at(i).E(); 
            photonTheta = utils.GetTheta( photon2Mom );
            photonConvDist = dist2.Mag();
            photonPos = photon2Pos;
            photonMom = photon2Mom;
            showerGap = dist2.Mag();
            nfound++;
            allPhotonGapVsVertexE -> Fill(showerGap,vertexEnergy);
          }
          else if (utils.IsActive(iDet,photon1Pos,5) &&
                   p1PhotonConversionMom->at(i).E() > egammaThreshold )
          {
            totalActiveGammas ++;
          }              
          if( !isCC ){
            if( nfound == 2 ) foundBothPhotons++;
            if( nfound == 1 ) foundOnePhoton++;
            if( nfound == 0 ) foundNoPhotons++;
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
                      << utils.IsFiducial( iDet, photon1Pos, signal ) 
                      << std::endl;
            std::cout << "  converted " << dist1.Mag() 
                      << " cm from the vertex with energy " 
                      << miscPhotonConversionMom->at(i_gamma).E() 
                      << " MeV" << std::endl;
          }
          if(   utils.IsFiducial( iDet, photon1Pos, signal )
             && miscPhotonConversionMom->at(i_gamma).E() > egammaThreshold )
          { 
            totalGammas++; 
            photonE = miscPhotonConversionMom->at(i_gamma).E() ;
            photonTheta = utils.GetTheta( photon1Mom );
            photonConvDist = dist1.Mag();
            photonPos = photon1Pos;
            photonMom = photon1Mom;
            showerGap = dist1.Mag();
            allPhotonGapVsVertexE -> Fill(showerGap,vertexEnergy);
          }
          else if (utils.IsActive(iDet,photon1Pos,5) &&
                   miscPhotonConversionMom->at(i_gamma).E() > egammaThreshold )
          {
            totalActiveGammas ++;
          }        
        }
      }
      if(totalGammas == 1 && !isCC){
        singlePhotonNCEnuVsPhotE->Fill(photonE,enugen);
        singlePhotonGapVsVertexE -> Fill(showerGap,vertexEnergy);
        vertexEvsGenereratedEPi0 -> Fill(vertexEnergy, enugen);
        VertexEnergyPhot -> Fill(vertexEnergy);
        ConversionDistPhot -> Fill(showerGap);
      }

      vertexEvsGenereratedE -> Fill(vertexEnergy, enugen);

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


      // Figure out how far the muon traveled;
      double muon_ContainedLength = 0;
      if (isCC && abs(inno) == 14) 
        muon_ContainedLength = (*vertex - MuonExitPos).Mag();


      // if (totalGammas == 1){
      //   std::cout << "\n\nConversion Distance is: " << showerGap
      //             << " and energy is " << vertexEnergy << "\n" 
      //             << " Run is " << run << " and event is " << event << std::endl;
      // }


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
        
          if (totalGammas > 0) {
            // Then this event already has a shower in the event.
            // std::cout << "Skipping another event for having a photon "
                      // << "and electron. Total: " << totalSkipped << "\n";
            totalSkipped += wgt;
            continue;
          }

          if (smearing) 
            ElepSmeared = utils.GetLeptonEnergy(Elep,true,11);
          else 
            ElepSmeared = Elep;


          electron_cand_energy = ElepSmeared;
          electron_cand_angle = ThetaLep;
          enuccqe = utils.NuEnergyCCQE( 1000*electron_cand_energy, 
                    sqrt(pow(1000*electron_cand_energy,2) - pow(0.511,2)), 
                    electron_cand_angle, 0.511, iflux )/1000.0;

          enucalo1 = utils.NuEnergyCalo(GeniePDG, GenieMomentum, 
                                        electron_cand_energy, 
                                        smearing, true, true ) 
                                      + vertexEnergy;
          enucalo2 = utils.NuEnergyCalo(GeniePDG, GenieMomentum, 
                                        electron_cand_energy, smearing,
                                        false, false, prot_thresh)
                                      + photon_energy 
                                      + vertexEnergy;

            // check on the smearing stats - if the signal is nue intrisic, repeat this 
            // calculation to improve the stats.
            if (smearingStats != 1.0  && smearing == true){
              if (smearingStatsIndex == 0){ // if the index is maxed out, keep going
                smearingStatsIndex = smearingStats;
                continue;
              }
              else{
                jentry --;  // decrement the index to repeat this event.
                wgt *= (1.0/smearingStats);
                smearingStatsIndex --;
              }

            }

          showerGap = 0.0;

          if (ndecay < 5 && ndecay > 0){  // K0
            ibkg = kNueFromNueCC_neutKaon;
            nueFromK0Decay->Fill( enugen, wgt );
            nueFromK0DecayLepE->Fill( electron_cand_energy, wgt );
            nueFromK0DecayCCQE->Fill( enuccqe, wgt );
            nueFromK0DecayCalo->Fill( enucalo2, wgt );
            if( enucalo2 > 0.2 && enucalo2 < 0.475 ) NnueFromK0Decay_LE += wgt;
          }
          else if (ndecay > 4 && ndecay < 11){  // K+
            ibkg = kNueFromNueCC_chargeKaon;
            nueFromKPlusDecay->Fill( enugen, wgt );
            nueFromKPlusDecayLepE->Fill( electron_cand_energy, wgt );
            nueFromKPlusDecayCCQE->Fill( enuccqe, wgt );
            nueFromKPlusDecayCalo->Fill( enucalo2, wgt );
            if( enucalo2 > 0.2 && enucalo2 < 0.475 ) NnueFromKPlusDecay_LE += wgt;
          }
          else if (ndecay == 11 || ndecay == 12){  // mu
            ibkg = kNueFromNueCC_muon;
            nueFromMuonDecay->Fill( enugen, wgt ); 
            nueFromMuonDecayLepE->Fill( electron_cand_energy, wgt ); 
            nueFromMuonDecayCCQE->Fill( enuccqe, wgt );
            nueFromMuonDecayCalo->Fill( enucalo2, wgt );
            if( enucalo2 > 0.2 && enucalo2 < 0.475 ) NnueFromMuonDecay_LE += wgt;
          }
          ShowerContainedDistance = utils.GetContainedLength(*vertex, lepDir, iDet);
          ShowerDistanceToStart   = utils.GetLengthToStart(  *vertex, lepDir, iDet);
          ShowerDistanceToStartYZ = utils.GetYZLengthToStart(*vertex, lepDir, iDet);



        } // end of intrinsic electrons

        // #############################
        // Look at misID from numu CC:
        // #############################
        if (isCC && abs(inno) == 14){

          // continue;

/*
          // ############################################################
          // Start of block of old code:
          // ############################################################
          if (!isFid) continue;


          if (leptonMom->at(0).E() < egammaThreshold ) continue;

          if (smearing) 
            ElepSmeared = utils.GetLeptonEnergy(Elep,true,11);
          else
            ElepSmeared = Elep;

          ShowerContainedDistance = utils.GetContainedLength(*vertex, lepDir, iDet);
          ShowerDistanceToStart   = utils.GetLengthToStart(  *vertex, lepDir, iDet);
          ShowerDistanceToStartYZ = utils.GetYZLengthToStart(*vertex, lepDir, iDet);
          showerGap = 0.0;
          ibkg = 7;
          efficiency = muonCCMisID;
          wgt = fluxweight*efficiency;
          electron_cand_energy = ElepSmeared;
          electron_cand_angle = ThetaLep;
           enuccqe = utils.NuEnergyCCQE( 1000*electron_cand_energy, 
                                         sqrt(pow(1000*electron_cand_energy,2) 
                                       - pow(0.511,2)), 
                                         electron_cand_angle, 0.511, iflux )/1000.0;
           enucalo1 = utils.NuEnergyCalo(GeniePDG, GenieMomentum, 
                                         electron_cand_energy, smearing, 
                                         true, true )
                                       + vertexEnergy;
           enucalo2 = utils.NuEnergyCalo(GeniePDG, GenieMomentum, 
                                         electron_cand_energy, smearing,
                                         false, false, prot_thresh)
                                       + photon_energy
                                       + vertexEnergy;
          // ############################################################
          // End of block of old code.
          // ############################################################
*/

          // In this case, looking for single photons again
          // but making sure we don't see the muon.
          
          if (muon_ContainedLength > 100) continue;

          // if (contained) continue;

          // are there photons?
          // if there is not exactly one single fiducial photon, continue
          if (totalGammas != 1 ) continue;

          // looking for active photons that can be rejected
          if (totalActiveGammas != 0 ){
            // std::cout << "Skipping this event because there is another active volume.\n";
            continue;
          }

          // At this point, there is a muon that was missed and exactly one fiducial photon
          // that was found without any active volume friends.
          // from here, continue like in the NC photon case
          
          if (photonE < egammaThreshold ){
            continue;
          }

          if (smearing){
            ElepSmeared = utils.GetLeptonEnergy(photonE,true,11);
            Elep = photonE;
          }
          else {
            ElepSmeared = photonE;
            Elep = photonE;
          }

          ShowerContainedDistance = utils.GetContainedLength(photonPos, photonMom, iDet);
          ShowerDistanceToStart   = utils.GetLengthToStart(  photonPos, photonMom, iDet);
          ShowerDistanceToStartYZ = utils.GetYZLengthToStart(photonPos, photonMom, iDet);
          electron_cand_energy = ElepSmeared;
          electron_cand_angle = photonTheta;
          


          enuccqe = utils.NuEnergyCCQE( 1000*electron_cand_energy, 
                                        sqrt(pow(1000*electron_cand_energy,2) 
                                      - pow(0.511,2)), 
                                        electron_cand_angle, 0.511, iflux )/1000.0;
          enucalo1 = utils.NuEnergyCalo( GeniePDG, GenieMomentum, 
                                       electron_cand_energy, smearing,
                                       true, true ) + vertexEnergy;
          // Since in this case, the photon energy is the electron
          // candidate energy, *don't* add back on the photon energy
          // Further, 
          enucalo2 = utils.NuEnergyCalo( GeniePDG, GenieMomentum, 
                                         electron_cand_energy, smearing,
                                         false, false, 
                                         prot_thresh ) + vertexEnergy;
          

          efficiency = photonMisID;
          wgt = fluxweight*efficiency;
          // check on the smearing stats - if the signal is nue intrisic or a photon bkg, repeat this 
          // calculation to improve the stats.
          if (smearingStats != 1.0  && smearing == true){
            if (smearingStatsIndex == 0){ // if the index is maxed out, keep going
              smearingStatsIndex = smearingStats;
              continue;
            }
            else{
              jentry --;  // decrement the index to repeat this event.
              wgt *= (1.0/smearingStats);
              smearingStatsIndex --;
            }
  
          }

          // In principle, can reject photons with a gap.
          // So, reject photons with vertex energy above the cut
          // AND a gap longer than the cut
          // Really this happens at the next stage/
          if ( photonConvDist > convDistCut && vertexEnergy > vtxEcut){
            std::cout << "Continuing because of cuts.....\n\n\n";
            std::cout << "  photonConvDist is " << photonConvDist << "\n"
                      << "  vertexEnergy is " << vertexEnergy << "\n";
            continue;

          }

          //end of new code


          ibkg = kNueFromNumuCC;
          
          numuCC->Fill( enugen, wgt );
          numuCCLepE->Fill( electron_cand_energy, wgt );
          numuCCCCQE->Fill( enuccqe, wgt );
          numuCCCalo->Fill( enucalo2, wgt );
          if( enucalo2 > 0.2 && enucalo2 < 0.475 ) NnumuCC_LE += wgt;

          nNumuMisID_total += wgt;




        } // end of numu CC misid as nue CC


        // #############################
        // Look at misID from NC:
        // #############################
        if (!isCC && totalGammas == 1 && 
             photonE > egammaThreshold)
        {


          if (totalActiveGammas != 0 ){
            // std::cout << "Skipping this event because there is another active volume.\n";
            continue;
          }

          if (smearing){
            ElepSmeared = utils.GetLeptonEnergy(photonE,true,11);
            Elep = photonE;
          }
          else {
            ElepSmeared = photonE;
            Elep = photonE;
          }

          ShowerContainedDistance = utils.GetContainedLength(photonPos, photonMom, iDet);
          ShowerDistanceToStart   = utils.GetLengthToStart(  photonPos, photonMom, iDet);
          ShowerDistanceToStartYZ = utils.GetYZLengthToStart(photonPos, photonMom, iDet);
          electron_cand_energy = ElepSmeared;
          electron_cand_angle = photonTheta;
          


          enuccqe = utils.NuEnergyCCQE( 1000*electron_cand_energy, 
                                        sqrt(pow(1000*electron_cand_energy,2) 
                                      - pow(0.511,2)), 
                                        electron_cand_angle, 0.511, iflux )/1000.0;
          enucalo1 = utils.NuEnergyCalo( GeniePDG, GenieMomentum, 
                                       electron_cand_energy, smearing,
                                       true, true ) + vertexEnergy;
          // Since in this case, the photon energy is the electron
          // candidate energy, *don't* add back on the photon energy
          // Further, 
          enucalo2 = utils.NuEnergyCalo( GeniePDG, GenieMomentum, 
                                         electron_cand_energy, smearing,
                                         false, false, 
                                         prot_thresh ) + vertexEnergy;
          


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

          // check on the smearing stats - if the signal is nue intrisic or a photon bkg, repeat this 
          // calculation to improve the stats.
          if (smearingStats != 1.0  && smearing == true){
            if (smearingStatsIndex == 0){ // if the index is maxed out, keep going
              smearingStatsIndex = smearingStats;
              continue;
            }
            else{
              jentry --;  // decrement the index to repeat this event.
              wgt *= (1.0/smearingStats);
              smearingStatsIndex --;
            }
  
          }

          if(NPi0FinalState > 0 ){
            ibkg = kNueFromNC_pi0;
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
            ibkg = kNueFromNC_singlePhot;
            
            singlePhotonNC -> Fill(enugen, wgt);
            singlePhotonNCPhoE -> Fill(photonE, wgt);
            singlePhotonNCCalo -> Fill(enucalo2, wgt);
            singlePhotonNCCCQE -> Fill(enuccqe, wgt);
            NCsinglePhoton += wgt;
            if( enucalo2 > 0.2 && enucalo2 < 0.475 ) NsinglePhotonNC_LE += wgt;
          }

          if (photonE > 0.200){
            photonConv->Fill( photonPos.X(), photonPos.Y(), photonPos.Z() );
            photonConvX->Fill( photonPos.X() );
            photonConvY->Fill( photonPos.Y() );
            photonConvZ->Fill( photonPos.Z() );
            photonCCQE->Fill( enuccqe, photonE, wgt );
          }

          // NCsinglePhoton += wgt;
          // singlePhotonE.push_back( photonE );
          // singlePhotonTheta.push_back( photonTheta );


        }
        // #############################
        // Add misID from nu - e scattering:
        // #############################
        if( isFid  && nuchan == 98 ){
          iTop ++;  //Should be an electron in these events
          iChan = 2000; //This is an other case!

          ibkg = kNueFromEScatter;
          efficiency = electronIDeff;
          wgt = fluxweight*efficiency;
          showerGap = 0.0;
        
          if (smearing) Elep = utils.GetLeptonEnergy(Elep,true,11);

          // std::cout << "This is numu electron scattering, elep is: " << Elep;

          electron_cand_energy = Elep;           // !!! don't have electron energy for these events!!
          electron_cand_angle = ThetaLep;               // !!! don't have electron angle, assume small
          enuccqe = utils.NuEnergyCCQE( 1000*electron_cand_energy, 
                                        sqrt(pow(1000*electron_cand_energy,2) 
                                        - pow(0.511,2)), 
                                        electron_cand_angle, 0.511, iflux )/1000.0;

          enucalo1 = utils.NuEnergyCalo( GeniePDG, GenieMomentum, 
                                         Elep, smearing,
                                         true, true )
                                       + vertexEnergy;
          enucalo2 = utils.NuEnergyCalo( GeniePDG, GenieMomentum, 
                                         Elep, smearing,
                                         false, false, 
                                         prot_thresh ) + photon_energy
                                       + vertexEnergy;


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
          elecAngleVsEnergy      -> Fill( Elep, ThetaLep*(180.0/3.14159), wgt);
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

          // if (contained)
          //   std::cout << "contained length is " << muon_ContainedLength << std::endl;
          // else
          //   std::cout << "non-contained length is " << muon_ContainedLength << std::endl;


          if (!contained || muon_ContainedLength < 100) {
            // std::cout << "continuing!" << std::endl;
            continue;
          }

          
          if (smearing) 
            ElepSmeared = utils.GetLeptonEnergy(Elep,
                                         true,13,contained,
                                         muon_ContainedLength);
          else
            ElepSmeared = Elep;

          // std::cout << "  Elep = " << Elep
          //           << ", ElepSmeared = " << ElepSmeared 
          //           << ", " << (Elep - ElepSmeared)/Elep << "%"
          //           << std::endl;


          efficiency = muonIDeff;
          wgt = fluxweight*efficiency;

          ibkg = kNumuFromNumuCC;

          electron_cand_energy = ElepSmeared;
          electron_cand_angle = ThetaLep;

          enuccqe = utils.NuEnergyCCQE( 1000*electron_cand_energy,
                                        sqrt(pow(1000*electron_cand_energy,2)
                                           - pow(105.7,2)),
                                        electron_cand_angle,
                                        105.7,
                                        iflux )/1000.0;
          enucalo1 = utils.NuEnergyCalo( GeniePDG,
                                         GenieMomentum,
                                         Elep,
                                         smearing,
                                         true,
                                         true )
                                       + vertexEnergy;
          enucalo2 = utils.NuEnergyCalo( GeniePDG,
                                         GenieMomentum,
                                         Elep,
                                         smearing,
                                         false,
                                         false,
                                         prot_thresh ) + photon_energy
                                       + vertexEnergy;
          numuCC->Fill(enugen, wgt);
          numuCCLepE->Fill( leptonMom->at(0).E(), wgt );
        
        }

        // Here is where to implement numu background from pi+/-

      }
      if (signal == "fosc"){
        if ( isFid && isCC && abs(inno) == 12 && Elep > egammaThreshold ){

          if (smearing) 
            ElepSmeared = utils.GetLeptonEnergy(Elep,true,11);
          else 
            ElepSmeared = Elep;

          efficiency = electronIDeff;
          wgt = fluxweight*efficiency;

          ShowerContainedDistance = utils.GetContainedLength(*vertex, lepDir, iDet);
          ShowerDistanceToStart   = utils.GetLengthToStart(*vertex, lepDir, iDet);
          ShowerDistanceToStartYZ = utils.GetYZLengthToStart(*vertex, lepDir, iDet);
          electron_cand_energy    = ElepSmeared;

          nueTotal->Fill(enugen,wgt);
    
         
          electron_cand_angle = ThetaLep;
          
          enuccqe = utils.NuEnergyCCQE( 1000*electron_cand_energy,
                                        sqrt(pow(1000*electron_cand_energy,2)
                                           - pow(105.7,2)),
                                        electron_cand_angle,
                                        105.7,
                                        iflux )/1000.0;
          enucalo1 = utils.NuEnergyCalo( GeniePDG,
                                         GenieMomentum,
                                         Elep,
                                         smearing,
                                         true,
                                         true )
                                       + vertexEnergy;
          enucalo2 = utils.NuEnergyCalo( GeniePDG,
                                         GenieMomentum,
                                         Elep,
                                         smearing,
                                         false,
                                         false,
                                         prot_thresh ) + photon_energy
                                       + vertexEnergy;


          CcqeVsTrueE->Fill( enugen, enuccqe, wgt );
          Calo1VsTrueE->Fill( enugen, enucalo1, wgt );
          Calo2VsTrueE->Fill( enugen, enucalo2, wgt );

        }
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
                                         true )
                                       + vertexEnergy;

          enucalo2 = utils.NuEnergyCalo( GeniePDG,
                                         GenieMomentum,
                                         false,
                                         false,
                                         prot_thresh ) + photon_energy
                                       + vertexEnergy;

          numuCC->Fill(enugen, wgt);
          numuCCLepE->Fill( Elep, wgt );
        }
      }
        
      if (smearing){
        LeptonSmearing -> Fill(Elep, ElepSmeared);
      }

      if (wgt != 0){
        CcqeVsTrueE            -> Fill( enugen, enuccqe, wgt );
        Calo1VsTrueE           -> Fill( enugen, enucalo1, wgt );
        Calo2VsTrueE           -> Fill( enugen, enucalo2, wgt );
        nueTotal               -> Fill( enugen,wgt);
      }      

      if (verbose){
        std::cout << "  For this interaction,\n" 
                  << "    etrue = " << enugen << "\n"
                  << "    eccqe = " << enuccqe << "\n"
                  << "    electron_cand_energy = "  << electron_cand_energy << "\n"
                  << "    ecalo1 = " << enucalo1 << "\n"
                  << "    ecalo2 = " << enucalo2 << "\n"
                  << "  wgt = " << wgt << "\n";
      } 


      if (isActive) newt->Fill();

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
    std::cout << "Fiducial volume = " << utils.GetFidMass( iDet,signal ) << " tons" << std::endl;
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
    
    std::cout << "Total (integral) numu events as nue misID: " << nNumuMisID_total << std::endl;
    
    std::cout << "==================================================" << std::endl;
    std::cout << "Output in " << outfile << std::endl;
    std::cout << "Had to bail on : " << "\n"
              << "N_continue_Pi0mismatch    : " << N_continue_Pi0mismatch << "\n"
              << "N_continue_Gamma_mismatch : " << N_continue_Gamma_mismatch << "\n"
              << "N_continue_foundPhotons   : " << N_continue_foundPhotons << "\n"
              << "N_continue_CC_muon        : " << N_continue_CC_muon << "\n";


    std::cout << "nFoscEvents: " << nFoscEvents << std::endl;
    std::cout << "nNumuCCEvents: " << nNumuCCEvents << std::endl;

}

