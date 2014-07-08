#define NtupleReprocessing_cxx
#include "NtupleReprocessing.hh"
#include <TH2.h>
#include <TH3.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <THStack.h>

#define print(a) ( std::cout << a << std::endl)

namespace lar1{

  void NtupleReprocessing::Loop( std::string signal,
                                 int iDet,
                                 int iLoc,
                                 Long64_t max_entry,
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

    //---------------------------------------------
    // some things to be configured by the user:

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


    double electronIDeff = 0.8;
    double muonIDeff     = 0.8;
    double photonMisID   = 0.06;
    double muonCCMisID   = 0.001;

    // NC photon vertex energy cuts
    // double vtxEcut = 0.0;     // 0.025;   // GeV
    // double convDistCut = 0.0; // 5.0;     // cm

    double egammaThreshold = 0.0; // 0.140 // GeV

    bool ccqe_only = false;

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
    // TString sample = "ccinc";
    // if( ccqe_only ) sample = "ccqe";
    TString outfile = InFile().Remove(InFile().Length()-5) + "_processed_";
    // if (iDet == 5) outfile += "MB_";
    // if (iDet == 4) outfile += "IC_";
    // if (iDet == 6) outfile += "IC_600_";
    // if (iDet == 7) outfile += "IC_800_";
    if (scale != 1.0) {
      outfile += "scale_";
      outfile += scale;
      outfile += "_";
    }
    if (specialNameText != "") outfile = outfile + specialNameText + "_";
    outfile = outfile + signal + ".root";

    std::cout << "outfile file: " << outfile << std::endl;
    TFile *f = new TFile(outfile, "RECREATE");
    if (f->IsZombie()){
      std::cout << "Error, couldn't create output file." << std::endl;
      return;
    }  
    TTree *newt = new TTree("EventsTot", "Event info for ALL types");

    // newt -> SetDirectory(0);

    double xmin, xmax, ymin, ymax, zmin, zmax;

    utils.GetDetBoundary(iDet, xmin, xmax, ymin, ymax, zmin, zmax);

    double ElectContainedDist = 0;
    //===========================================================
    // This is all of the information that winds up in the tree, 
    // which is in turn what the sensitivity code gets.
    // If you make additions, let me know so I can update the 
    // sensitivity code accordingly. (Corey A.)

    //--------------------------------------------------
    // These are variables already in the input ntuple
    //
    newt->Branch("iflux", &iflux, "iflux/I");            
    /* iflux
       0 = neutrino mode normal MC
       1 = neutrino mode full osc
       2 = anti neutrino mode, normal mc
       3 = anti neutrino mode, full osc
    */
    newt->Branch("ibkg", &ibkg, "ibkg/I");          
    /* ibkg
       see above
    */
    newt->Branch("iDet", &iDet, "iDet/I");
    /** iDet
       0 = LAr1-ND
       1 = uboone
       2 = LAr1-FD
    **/
    newt->Branch("nuchan", &nuchan, "nuchan/I");         // NUANCE interaction codes
    newt->Branch("inno", &inno, "inno/I");               // Neutrino type +-12,+-14,
    newt->Branch("enugen", &enugen, "enugen/D");         // Neutrino energy
    newt->Branch("energy", &energy, "energy/D");         // Reconstructed neutrino energy
    newt->Branch("nuleng", &nuleng, "nuleng/D");         // True neutrino length
    newt->Branch("parid", &parid, "parid/I");            // Neutrino parent ID
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
    newt->Branch("Vx", &Vx, "Vx/D");             // Neutrino interaction vertex
    newt->Branch("Vy", &Vy, "Vy/D");
    newt->Branch("Vz", &Vz, "Vz/D"); 
    newt->Branch("ParVx",&ParVx,"ParVx/D");      // Neutrino creation vertex
    newt->Branch("ParVy",&ParVy,"ParVy/D");
    newt->Branch("ParVz",&ParVz,"ParVz/D");
    newt->Branch("ParPx",&ParPx,"ParPx/D");      // Neutrino parent momentum
    newt->Branch("ParPy",&ParPy,"ParPy/D");
    newt->Branch("ParPz",&ParPz,"ParPz/D");
    newt->Branch("LepTheta", &ThetaLep, "LepTheta/D");
    newt->Branch("LepPhi", &PhiLep, "LepPhi/D");
    newt->Branch("LepThetaSmeared", &ThetaLepSmeared, "LepThetaSmeared/D");
    newt->Branch("LepPhiSmeared", &PhiLepSmeared, "LepPhiSmeared/D");
    newt->Branch("LepE", &Elep, "LepE/D");
    newt->Branch("LepESmeared", &ElepSmeared, "LepESmeared/D");
    newt->Branch("ElectContainedDist",&ElectContainedDist,"ElectContainedDist/D");

    //------------------------------------------------------------------
    // These are new variables that get added at the reprocessing stage

    double efficiency = 0.0;
    double fluxweight = 1.0;
    double electron_cand_energy = 0.0;
    double electron_cand_angle = 0.0;
    double enuccqe = 0.0;
    double enucalo1 = 0.0;
    double enucalo2 = 0.0;
    std::vector<double> ConversionDist;

    // If necessary, scale the far det:
    if (scale != 1.0) utils.ScaleFarDet(scale/100.0); 

    //Get the POT weight.  Need to get iflux
    b_iflux->GetEntry(0); //iflux should now be filled
    double potweight;
    if (signal == "numi") potweight = utils.GetPOTNormNuMI( iflux, iDet );
    else potweight = utils.GetPOTNorm( iflux, iLoc );
    std::cout << "POT weight = " << potweight << std::endl;

   
    newt->Branch("Eff", &efficiency, "Efficiency/D");                                // Reconstruction efficiency
    newt->Branch("FluxWgt", &fluxweight, "FluxWeight/D");                            // Flux reweight
    newt->Branch("Potweight",&potweight, "Potweight/D");
    newt->Branch("ConversionDist", "ConversionDist", &ConversionDist, 32000, 0);     // Photon conversion distance in pizero events
    newt->Branch("LepPx",&LepPx,"LepPx/D");                                          // lepton kinematics
    newt->Branch("LepPy",&LepPy,"LepPy/D");
    newt->Branch("LepPz",&LepPz,"LepPz/D");
    newt->Branch("LepAngle",&LepAngle,"LepAngle/D");
    newt->Branch("ElecCandEnergy",&electron_cand_energy,"ElectronCandEnergy/D");     // True energy of electron/photon/muon that is taken for an electron
    newt->Branch("ElecCandAngle",&electron_cand_angle,"ElectronCandAngle/D");        // True angle of electron/photon/muon that is taken for an electron
    newt->Branch("Eccqe",&enuccqe,"Eccqe/D");                                        // Neutrino energy assuming CCQE kinematics
    newt->Branch("Ecalo1",&enucalo1,"Ecalo1/D");                                     // Neutrino energy using calorimetric reconstruction
    newt->Branch("Ecalo2",&enucalo2,"Ecalo2/D");
    double ncpi0en=0;
    newt->Branch("NCpi0En",&ncpi0en,"NCpi0En/D");                                     // pi0energy
    TLorentzVector shower1P, shower2P;
    newt->Branch("Pi0Shower1P",&shower1P);
    newt->Branch("Pi0Shower2P",&shower2P);
    TLorentzVector shower1X, shower2X;
    newt->Branch("Pi0Shower1X",&shower1X);
    newt->Branch("Pi0Shower2X",&shower2X);

    newt->Branch("MultiWeight",&MultiWeight);


    bool isFid, isActive;
    int iChan = 0, iTop = 0;
    newt->Branch("Topology",&iTop,"Topology/I");
    newt->Branch("Channel",&iChan,"Channel/I");
    newt->Branch("IsFiducial",&isFid);
    newt->Branch("IsActive",&isActive);

    // double muonExitVx=0, muonExitVy=0, muonExitVz=0, muonExitPx=0, muonExitPy=0, muonExitPz=0;
    TVector3 MuonExitPos(0,0,0), MuonExitMom(0,0,0);
    newt->Branch("MuonExitPos",&MuonExitPos);
    newt->Branch("MuonExitMom",&MuonExitMom);
    // std::vector <TVector3> chargedPionExitPos;
    // std::vector <TVector3> chargedPionExitMom;
    // newt->Branch("ChargedPionExitPos",&chargedPionExitPos);
    // newt->Branch("ChargedPionExitMom",&chargedPionExitMom);

    // newt->Branch("MuonExitVx",&muonExitVx);
    // newt->Branch("MuonExitVy",&muonExitVy);
    // newt->Branch("MuonExitVz",&muonExitVz);
    // newt->Branch("MuonExitPx",&muonExitPx);
    // newt->Branch("MuonExitPy",&muonExitPy);
    // newt->Branch("MuonExitPz",&muonExitPz);  

    //-----------------------------------------------------------------
    // These are variables that do event rates for the topology counters
    // Enumerating the channels and topologies.
    // Can have CC, NC, or other type events

    // Can further distinguish:
    // iChan = 1000 //NC with 0 charged pions, 0 protons, 0 neutrons
    // iChan = 1001 //NC with 0 charged pions, 0 protons, 1 or more neutrons
    // iChan = 1010 //NC with 0 charged pions, 1 proton,  0 neutrons
    // iChan = 1011 //NC with 0 charged pions, 1 proton,  1 or more neutrons
    // iChan = 1020 //NC with 0 charged pions, 2 protons, 0 neutrons
    // iChan = 1021 //NC with 0 charged pions, 2 protons, 1 or more neutrons
    // iChan = 1030 //NC with 0 charged pions, 3+ protons, 0 neutrons
    // iChan = 1031 //NC with 0 charged pions, 3+ protons, 1 or more neutrons
    // 
    // 
    // iChan = 1100 //NC with 1 charged pions, 0 protons, 0 neutrons
    // iChan = 1101 //NC with 1 charged pions, 0 protons, 1 or more neutrons
    // iChan = 1110 //NC with 1 charged pions, 1 proton,  0 neutrons
    // iChan = 1111 //NC with 1 charged pions, 1 proton,  1 or more neutrons
    // iChan = 1120 //NC with 1 charged pions, 2+ protons, 0 neutrons
    // iChan = 1121 //NC with 1 charged pions, 2+ protons, 1 or more neutrons
    // 
    // iChan = 1200 //NC with 2+ charged pions, 0 protons, 0 neutrons
    // iChan = 1201 //NC with 2+ charged pions, 0 protons, 1 or more neutrons
    // iChan = 1210 //NC with 2+ charged pions, 1 proton,  0 neutrons
    // iChan = 1211 //NC with 2+ charged pions, 1 proton,  1 or more neutrons
    // iChan = 1220 //NC with 2+ charged pions, 2+ protons, 0 neutrons
    // iChan = 1221 //NC with 2+ charged pions, 2+ protons, 1 or more neutrons
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
    std::vector<double> singlePhotonE, singlePhotonTheta;

    double NCpizeroForNCDis = 0;

    double emin = 0.0, emax = 3.0;
    int ebins = 15;

    // true neutrino energy
    TH1D *nueFromMuonDecay = new TH1D("NueFromMuonDecay","NueFromMuonDecay;Generated Neutrino Energy (GeV);Events",ebins,emin,emax);
    TH1D *nueFromK0Decay = new TH1D("NueFromK0Decay","NueFromK0Decay;Generated Neutrino Energy (GeV);Events",ebins,emin,emax);
    TH1D *nueFromKPlusDecay = new TH1D("NueFromKPlusDecay","NueFromKPlusDecay;Generated Neutrino Energy (GeV);Events",ebins,emin,emax);
    TH1D *singlePhotonPi0 = new TH1D("SinglePhotonPi0", "SinglePhotonPi0;Generated Neutrino Energy (GeV);Events",ebins,emin,emax);
    TH1D *singlePhotonNC = new TH1D("SinglePhotonNC", "SinglePhotonNC;Generated Neutrino Energy (GeV);Events",ebins,emin,emax);
    TH1D *numuCC = new TH1D("NumuCC","NumuCC;Generated Neutrino Energy (GeV);Events",ebins,emin,emax);
    TH1D *numuElec = new TH1D("NumuElec","NumuElec;Generated Neutrino Energy (GeV);Events",ebins,emin,emax);

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
    TH1D *nueFromMuonDecayLepE = new TH1D("NueFromMuonDecayLepE","NueFromMuonDecay;Electron Energy (GeV);Events",20*ebins,emin,emax);
    TH1D *nueFromK0DecayLepE = new TH1D("NueFromK0DecayLepE","NueFromK0Decay;Electron Energy (GeV);Events",20*ebins,emin,emax);
    TH1D *nueFromKPlusDecayLepE = new TH1D("NueFromKPlusDecayLepE","NueFromKPlusDecay;Electron Energy (GeV);Events",20*ebins,emin,emax);
    TH1D *singlePhotonPi0PhoE = new TH1D("SinglePhotonPi0PhoE", "SinglePhotonPi0;Photon Energy (GeV);Events",20*ebins,emin,emax);
    TH1D *singlePhotonNCPhoE = new TH1D("SinglePhotonNCPhoE", "SinglePhotonPi0;Photon Energy (GeV);Events",20*ebins,emin,emax);
    TH1D *numuCCLepE = new TH1D("NumuCCLepE", "NumuCCLepE;Muon Energy (GeV);Events",20*ebins,emin,emax);
    TH1D *numuElecLepE = new TH1D("NumuElecLepE","NumuElecLepE;Generated Neutrino Energy (GeV);Events",20*ebins,emin,emax);

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
    // parent pion kinematics
    TH2D * parentKinematics = new TH2D("parentKinematics", "Neutrino Parent p_{T} vs. p_{z};p_{z};p_{T}",
                                     100,0,10,100,0,3);
    // Flux Through Both Detectors
    TH2D * SharedFlux = new TH2D("SharedFlux","Amount of flux that passes through uB",250,xmin,xmax,250,ymin,ymax);
    TH2D * nearDetOnlyFlux = new TH2D("nearDetOnlyFlux","Amount of flux that passes through ND only",250,xmin,xmax,250,ymin,ymax);
    TH2D * FluxRatio = new TH2D("FluxRatio","Ratio of events in ND only to Total Events",250,xmin,xmax,250,ymin,ymax);


    // vertex energy in NC pizero single photon events
    TH1D *NCpizeroVtxKE = new TH1D("NCpizeroVtxKE","NCpizeroVtxKE;Vtx Kinetic Energy (GeV)",50,0,500);
    TH1D *NCpizeroConvD = new TH1D("NCpizeroConvD","NCpizeroConvD;Photon Conversion Distance (cm)",100,0,100);
    TH2D *NCpizeroVtxKEConvD = new TH2D("NCpizeroVtxKEConvD","NCpizeroVtxKEConvD;Vtx Kinetic Energy (GeV);Photon Conv. Dist. (cm)",50,0,500,100,0,100);


    // True Pion, Muon energy
    TH1D *ChargedPionEnergy = new TH1D("ChargedPionEnergy", "ChargedPionEnergy; Pion Energy (GeV);Events", 60, 0.0, 3);
    TH1D *MuonEnergy = new TH1D("MuonEnergy", "MuonEnergy; Muon Energy (GeV);Events", 60, 0.0, 3);
    ChargedPionEnergy -> SetFillColor(29);
    MuonEnergy -> SetFillColor(32);

    TH1D * ProtonMultiplicity = new TH1D("ProtonMultiplicity", "Proton Multiplicity (21MeV KE Threshold)", 5, 0, 5);
    TH1D * ProtonMomentum = new TH1D("ProtonMomentum", "Proton Momentum (21 MeV KE Threshold)",120,0,3);

    TH2D * elecAngleVsEnergy = new TH2D("ElectronAngleVsEnergy",
                                        "ElectronAngleVsEnergy;Electron Energy (GeV);Electron Angle (Rad)",
                                        ebins,emin,emax,30,0,180);

    TH2D *photonCCQE = new TH2D("PhotonCCQE","PhotonCCQE;CCQE Energy (GeV);Photon Energy (GeV)",ebins,emin,emax,20*ebins,emin,emax);
    TH1D *EMShowers = new TH1D("EMShowers","EMShowers;Number of Showers in Event;Number of Events",6,-1,5);
    TH2D *CcqeVsTrueE = new TH2D("CcqeVsTrueE","CcqeVsTrueE;Generated Neutrino Energy (GeV);CCQE Energy (GeV)",30,0,3,30,0,3);
    TH2D *Calo1VsTrueE = new TH2D("Calo1VsTrueE","Calo1VsTrueE;Generated Neutrino Energy (GeV);Calorimetric Energy - all (GeV)",30,0,3,30,0,3);
    TH2D *Calo2VsTrueE = new TH2D("Calo2VsTrueE","Calo2VsTrueE;Generated Neutrino Energy (GeV);Calorimetric Energy - no neutrals (GeV)",30,0,3,30,0,3);
    TH3D *photonConv = new TH3D("PhotonConv","PhotonConv;X;Y;Z",50,xmin,xmax,50,ymin,ymax,100,zmin,zmax);
    TH1D *photonConvX = new TH1D("PhotonConvX","PhotonConvX;X",50,xmin, xmax);
    TH1D *photonConvY = new TH1D("PhotonConvY","PhotonConvY;Y",50,ymin, ymax);
    TH1D *photonConvZ = new TH1D("PhotonConvZ","PhotonConvY;Z",50,zmin, zmax);
    TH3D *ncpi0bkgV = new TH3D("ncpi0bkgV","ncpi0bkgV;X;Y;Z",50,xmin,xmax,50,ymin,ymax,100,zmin,zmax);
    TH1D *ncpi0bkgX = new TH1D("ncpi0bkgX","ncpi0bkgX;X",50,xmin,xmax);
    TH1D *ncpi0bkgY = new TH1D("ncpi0bkgY","ncpi0bkgY;Y",50,ymin,ymax);
    TH1D *ncpi0bkgZ = new TH1D("ncpi0bkgZ","ncpi0bkgZ;Z",50,zmin,zmax);

    // Ints to keep track of continue bails
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

      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = GetEntry(jentry);   nbytes += nb;
      iTop = 0;
      iChan = 0;

      // verbose = false;
      // if (NPi0FinalState > 0) verbose = true;

      if (verbose || jentry % 50000 == 0) std::cout << "========== Event " << ientry << " of " << nentries << " ==========" << std::endl;
      
      // Clear ntuple variables
      ConversionDist.clear();
      efficiency = 0.0;
      wgt = 0.0;
      electron_cand_energy = 0.0;
      electron_cand_angle = 0.0;
      enuccqe = 0.0;
      enucalo1 = 0.0;
      enucalo2 = 0.0;
      ElectContainedDist = -1;
      
      // Calculate the FS lepton's 4-vector and return neutrino path length
      nuleng = CalcLepton( detect_dist );

      // Get flux weight from FluxRW utilities
      if ( signal != "numi" ) fluxweight = utils.GetFluxWeight( energy, iflux, inno, ndecay );
      else fluxweight = 1.0;

      // Lump in the Pot weight to flux weight so it isn't lost:
      fluxweight *= potweight;

      // Take care of sign selection right here:
      if (signal == "fosc"){
        if (!oscNeutrinos && inno > 0) fluxweight = 0.0;
        if (!oscAntiNeutrinos && inno < 0) fluxweight = 0.0;
      }


      // Is the interaction in the fiducial volume?
      TVector3 vtx(Vx, Vy, Vz);
      isFid = utils.IsFiducial( iDet, vtx );
      isActive = utils.IsActive( iDet, vtx );
      if (verbose) std::cout << std::setprecision(5) << "vertex = (" << Vx << ", " << Vy << ", " << Vz << ") cm" << std::endl;
      if( isActive && verbose) std::cout << "In active detector!" << std::endl;
      if( isFid && verbose) std::cout << "Fiducial event!" << std::endl;

      if( !isActive ) continue;


      beamSpot -> Fill(Vx,Vy,fluxweight);
      parentKinematics -> Fill(ParPz,sqrt(ParPy*ParPy + ParPx*ParPx),fluxweight);


/*
      // Now find out if this neutrino would have hit microboone
      // (but only if this is a near detector)
      // The function that does this is a ray tracing alg in utils
      
      if (iDet == 0 || iDet == 8 || iDet == 9 || iDet == 10){


        // Important: Putting everything in beam coordinates!

        // Need 5 vectors: the start position, the start direction
        // The corners of the parallelogram upstream to hit, in
        // the same coordinates.
        // The start point is the neutrino vertex, so just move to beam coords:
        TVector3 startPoint(Vx, Vy, Vz);
        // Find the start direction:  Make a vector of the neutrino path
        // from parent to vertex and then normalize.
        TVector3 startDir(neutMom->X(), neutMom->Y(), neutMom->Z());
        startDir*= 1.0/startDir.Mag();
        // The 3 corners are the bottom left, bottom right, and top left
        // corners of uboone, and then shifted forward in z by 470m - detector_dist
        // to account for the distance between detectors
        // TVector3 corner1(0.0 + 8  , -116.5 - 42, 47000-detect_dist);
        TVector3 corner1( 128, -116.5, 47000-detect_dist - Vz - 35000);
        TVector3 corner2(-128, -116.5, 47000-detect_dist - Vz - 35000);
        TVector3 corner3( 128,  116.5, 47000-detect_dist - Vz - 35000);



        // Now find out if the neutrino (which has interacted in ND)
        // would have passed through uB
        bool hitsuB = utils.IntersectsPlane(startPoint,startDir,corner1,corner2,corner3);


        if (verbose || jentry % 50000 == 0){
          std::cout << "Neutrino vertex and direction: ("
                    << vtx.X() << ", " << vtx.Y() << ", " << vtx.Z() << "), ("
                    << startDir.X() << ", " << startDir.Y() << ", " << startDir.Z() << ") " << std::endl;
          std::cout << "  MicroBooNE corners: " << "("
                    << corner1.X() << ", " << corner1.Y() << ", " << corner1.Z() << "), ("
                    << corner2.X() << ", " << corner2.Y() << ", " << corner2.Z() << "), ("
                    << corner3.X() << ", " << corner3.Y() << ", " << corner3.Z() << ")" << std::endl;

        }
        

        if (hitsuB){
          std::cout << "Found a neutrino that hits uB!"<< std::endl;
          SharedFlux -> Fill(Vx,Vy,fluxweight);
        }
        else
          nearDetOnlyFlux -> Fill(Vx,Vy,fluxweight);
      }
*/

      // Now figure out which direction the neutrino was going in

      Double_t photon_energy = utils.TotalPhotonEnergy( iDet, p1PhotonConversionPos, p1PhotonConversionMom, 
                    p2PhotonConversionPos, p2PhotonConversionMom );

      if ( verbose )std::cout << "total secondary photon energy: " << photon_energy << " GeV" << std::endl;

      //------------------------------------------
      // Analyze CC events
      //------------------------------------------
      if (verbose) std::cout << "interaction = ";
      if( isCC && verbose) std::cout << "CC" << std::endl;
      else if (verbose) std::cout << "NC" << std::endl;

      if (verbose){
        std::cout << "Debugging information ... \n"
                  << "\t inno: " << inno << "\n"
                  << "\t Elep: " << Elep << "\n"
                  << "\t MuonMom->size(): " << MuonMom->size() << "\n"
                  << "\t MuonPos->size(): " << MuonPos->size() << "\n"
                  << "\t p1PhotonConversionMom->size(): " << p1PhotonConversionMom->size() << "\n"
                  << "\t p2PhotonConversionMom->size(): " << p2PhotonConversionMom->size() << "\n"
                  << "\t NPi0FinalState: " << NPi0FinalState << "\n"
                  << "\t miscPhotonConversionPos->size(): " << miscPhotonConversionPos->size() << "\n"
                  << "\t NGamma: " << NGamma << "\n"
                  << "\t egammaThreshold " << egammaThreshold << "\n"
                  << "\t fluxweight: " << fluxweight << "\n";

      }

      bool pass = true;
      if( ccqe_only && nuchan != 1 ) pass = false;

      // electron CC events
      if( isFid && isCC && abs(inno) == 12 && pass && Elep > egammaThreshold ){

        iTop ++; //nueCC means there must be a shower!
        if ( signal == "fosc" ){
          efficiency = electronIDeff;
          wgt = fluxweight*efficiency;

          TVector3 vertex(Vx, Vy, Vz);
          TVector3 lepDir(LepPx, LepPy, LepPz);
          ElectContainedDist = utils.GetContainedLength(vertex, lepDir, iDet);

          electron_cand_energy = Elep;
          electron_cand_angle = ThetaLep;
          enuccqe = utils.NuEnergyCCQE( 1000*electron_cand_energy, sqrt(pow(1000*electron_cand_energy,2) - pow(0.511,2)), 
                electron_cand_angle, 0.511, iflux )/1000.0;
          enucalo1 = utils.NuEnergyCalo( geniePDG, genieE, true, true );
          enucalo2 = utils.NuEnergyCalo( geniePDG, genieE, false, false, prot_thresh ) + photon_energy;
          CcqeVsTrueE->Fill( energy, enuccqe, wgt );
          Calo1VsTrueE->Fill( energy, enucalo1, wgt );
          Calo2VsTrueE->Fill( energy, enucalo2, wgt );
        }
        if ( signal == "numi" ){
          efficiency = electronIDeff;
          wgt = fluxweight*efficiency;

          electron_cand_energy = Elep;
          electron_cand_angle = ThetaLep;
          enuccqe = utils.NuEnergyCCQE( 1000*electron_cand_energy, sqrt(pow(1000*electron_cand_energy,2) - pow(0.511,2)), 
                electron_cand_angle, 0.511, iflux )/1000.0;
          enucalo1 = utils.NuEnergyCalo( geniePDG, genieE, true, true );
          enucalo2 = utils.NuEnergyCalo( geniePDG, genieE, false, false, prot_thresh ) + photon_energy;
          CcqeVsTrueE->Fill( energy, enuccqe, wgt );
          Calo1VsTrueE->Fill( energy, enucalo1, wgt );
          Calo2VsTrueE->Fill( energy, enucalo2, wgt );      
        }
        if ( signal == "phot" ){
          efficiency = photonMisID;
          wgt = fluxweight*efficiency;

          electron_cand_energy = Elep;
          electron_cand_angle = ThetaLep;
          enuccqe = utils.NuEnergyCCQE( 1000*electron_cand_energy, sqrt(pow(1000*electron_cand_energy,2) - pow(0.511,2)), 
                electron_cand_angle, 0.511, iflux )/1000.0;
          enucalo1 = utils.NuEnergyCalo( geniePDG, genieE, true, true );
          enucalo2 = utils.NuEnergyCalo( geniePDG, genieE, false, false, prot_thresh ) + photon_energy;
          // CcqeVsTrueE->Fill( energy, enuccqe, wgt );
          // Calo1VsTrueE->Fill( energy, enucalo1, wgt );
          // Calo2VsTrueE->Fill( energy, enucalo2, wgt );
          if (ndecay < 5 && ndecay > 0){  // K0
            ibkg = 3;
          }
          else if (ndecay > 4 && ndecay < 11){  // K+
            ibkg = 2;
          }
          else if (ndecay == 11 || ndecay == 12){  // mu
            ibkg = 1;
          }
        }

        
        if( signal == "nue" ){
          efficiency = electronIDeff;
          wgt = fluxweight*efficiency;
          if (!detNeutrinos && inno > 0) wgt = 0.0;
          if (!detAntiNeutrinos && inno < 0) wgt = 0.0;
          electron_cand_energy = Elep;
          electron_cand_angle = ThetaLep;
          enuccqe = utils.NuEnergyCCQE( 1000*electron_cand_energy, sqrt(pow(1000*electron_cand_energy,2) - pow(0.511,2)), 
                electron_cand_angle, 0.511, iflux )/1000.0;
          enucalo1 = utils.NuEnergyCalo( geniePDG, genieE, true, true );
          enucalo2 = utils.NuEnergyCalo( geniePDG, genieE, false, false, prot_thresh ) + photon_energy;
          CcqeVsTrueE->Fill( energy, enuccqe, wgt );
          Calo1VsTrueE->Fill( energy, enucalo1, wgt );
          Calo2VsTrueE->Fill( energy, enucalo2, wgt );

          TVector3 vertex(Vx, Vy, Vz);
          TVector3 lepDir(LepPx, LepPy, LepPz);
          ElectContainedDist = utils.GetContainedLength(vertex, lepDir, iDet);

          // Calculate the angle to z direction for the electron
          double Theta= acos(LepPz/lepDir.Mag()); 
          Theta *= 180/3.14159;
          elecAngleVsEnergy->Fill(Elep, Theta, wgt);

          if (ndecay < 5 && ndecay > 0){  // K0
            ibkg = 3;
            nueFromK0Decay->Fill( energy, wgt );
            nueFromK0DecayLepE->Fill( Elep, wgt );
            nueFromK0DecayCCQE->Fill( enuccqe, wgt );
            nueFromK0DecayCalo->Fill( enucalo2, wgt );
            if( enucalo2 > 0.2 && enucalo2 < 0.475 ) NnueFromK0Decay_LE += wgt;
          }
          else if (ndecay > 4 && ndecay < 11){  // K+
            ibkg = 2;
            nueFromKPlusDecay->Fill( energy, wgt );
            nueFromKPlusDecayLepE->Fill( Elep, wgt );
            nueFromKPlusDecayCCQE->Fill( enuccqe, wgt );
            nueFromKPlusDecayCalo->Fill( enucalo2, wgt );
            if( enucalo2 > 0.2 && enucalo2 < 0.475 ) NnueFromKPlusDecay_LE += wgt;
          }
          else if (ndecay == 11 || ndecay == 12){  // mu
            ibkg = 1;
            nueFromMuonDecay->Fill( energy, wgt ); 
            nueFromMuonDecayLepE->Fill( Elep, wgt ); 
            nueFromMuonDecayCCQE->Fill( enuccqe, wgt );
            nueFromMuonDecayCalo->Fill( enucalo2, wgt );
            if( enucalo2 > 0.2 && enucalo2 < 0.475 ) NnueFromMuonDecay_LE += wgt;
          }
        }
        else if( signal == "numu" )
          efficiency = 0.0;

        electronCC += fluxweight*efficiency;

      }
      if (signal == "numi" && isFid && isCC && abs(inno) == 14){

          efficiency = muonIDeff;
          wgt = fluxweight*efficiency;
          numuCC->Fill(energy, wgt);
          numuCCLepE->Fill( Elep, wgt );
          electron_cand_energy = Elep;
          // Find the muon in the GeniePDG list:
          int muonPos = -1;
          int i = 0;
          while (muonPos == -1){
            if (abs(geniePDG->at(i)) == 13) muonPos = i;
            i++ ;
          }

          electron_cand_angle = utils.GetTheta( geniePx->at(muonPos), geniePy->at(muonPos), geniePz->at(muonPos) );

          enuccqe = utils.NuEnergyCCQE(  1000*electron_cand_energy,
                                         sqrt(pow(1000*electron_cand_energy,2) - pow(105.7,2)),
                                         electron_cand_angle,
                                         105.7,
                                         iflux )/1000.0;
          enucalo1 = utils.NuEnergyCalo( geniePDG,
                                         genieE,
                                         true,
                                         true );
          enucalo2 = utils.NuEnergyCalo( geniePDG,
                                         genieE,
                                         false,
                                         false,
                                         prot_thresh ) + photon_energy;
      }

      if (signal != "fosc" && isCC && abs(inno) == 14 && MuonMom -> size() == 0) {
        N_continue_CC_muon ++;
        continue;
      }
      // if (isCC && abs(inno) == 14 && MuonMom -> size() != 0) {
        // std::cout << "at least that worked..." <<std::endl;
        // for (auto point : * MuonMom)
        //    std::cout << point.E() << std::endl;
      // }

      if( signal != "fosc" && signal != "numi" && isFid && isCC && abs(inno) == 14 && MuonMom->at(0).E() > egammaThreshold ){


        if( signal == "numu" ){
          efficiency = muonIDeff;
          wgt = fluxweight*efficiency;
          numuCC->Fill(energy, wgt);
          numuCCLepE->Fill( MuonMom->at(0).E(), wgt );
          electron_cand_energy = MuonMom->at(0).E();
          electron_cand_angle = utils.GetTheta( MuonMom->at(0).X(), MuonMom->at(0).Y(), MuonMom->at(0).Z() );

          enuccqe = utils.NuEnergyCCQE( 1000*electron_cand_energy,
                                        sqrt(pow(1000*electron_cand_energy,2) - pow(105.7,2)),
                                        electron_cand_angle,
                                        105.7,
                                        iflux )/1000.0;
          enucalo1 = utils.NuEnergyCalo( geniePDG,
                                         genieE,
                                         true,
                                         true );
          enucalo2 = utils.NuEnergyCalo( geniePDG,
                                         genieE,
                                         false,
                                         false,
                                         prot_thresh ) + photon_energy;
        }
        else if( signal == "nue" ){
          // std::cout << "Should be making a muon entry in the nue table!\n";
          ibkg = 7;
          efficiency = muonCCMisID;
          wgt = fluxweight*efficiency;


          electron_cand_energy = MuonMom->at(0).E();
          electron_cand_angle = utils.GetTheta( MuonMom->at(0).X(), MuonMom->at(0).Y(), MuonMom->at(0).Z() );
          enuccqe = utils.NuEnergyCCQE( 1000*electron_cand_energy, sqrt(pow(1000*electron_cand_energy,2) - pow(0.511,2)), 
                electron_cand_angle, 0.511, iflux )/1000.0;
          enucalo1 = utils.NuEnergyCalo( geniePDG, genieE, true, true );
          enucalo2 = utils.NuEnergyCalo( geniePDG, genieE, false, false, prot_thresh ) + photon_energy;
          CcqeVsTrueE->Fill( energy, enuccqe, wgt );
          Calo1VsTrueE->Fill( energy, enucalo1, wgt );
          Calo2VsTrueE->Fill( energy, enucalo2, wgt );

          numuCC->Fill( energy, wgt );
          numuCCLepE->Fill( MuonMom->at(0).E(), wgt );
          numuCCCCQE->Fill( enuccqe, wgt );
          numuCCCalo->Fill( enucalo2, wgt );
          if( enucalo2 > 0.2 && enucalo2 < 0.475 ) NnumuCC_LE += wgt;
        }

        muonCC += fluxweight*efficiency;

      }


      //----------------------------------------------
      // Analyze numu-electron scattering
      //----------------------------------------------
      if( isFid && abs(inno) == 14 && nuchan == 98 ){

        iTop ++;  //Should be an electron in these events
        iChan = 2000; //This is an other case!



        ibkg = 4;
        efficiency = electronIDeff;
        wgt = fluxweight*efficiency;
        
        electron_cand_energy = energy;           // !!! don't have electron energy for these events!!
        electron_cand_angle = 0.0;               // !!! don't habe electron angle, assume small
        enuccqe = utils.NuEnergyCCQE( 1000*electron_cand_energy, sqrt(pow(1000*electron_cand_energy,2) - pow(0.511,2)), 
              electron_cand_angle, 0.511, iflux )/1000.0;
        enucalo1 = utils.NuEnergyCalo( geniePDG, genieE, true, true );
        enucalo2 = utils.NuEnergyCalo( geniePDG, genieE, false, false, prot_thresh ) + photon_energy;
        CcqeVsTrueE->Fill( energy, enuccqe, wgt );
        Calo1VsTrueE->Fill( energy, enucalo1, wgt );
        Calo2VsTrueE->Fill( energy, enucalo2, wgt );

        numuElec->Fill( energy, wgt );
        numuElecLepE->Fill( energy, wgt );       // !!! don't have electron energy for these events!!
        numuElecCCQE->Fill( enuccqe, wgt );
        numuElecCalo->Fill( enucalo2, wgt );
        if( enucalo2 > 0.2 && enucalo2 < 0.475 ) NnumuElec_LE += wgt;

      } 

      //----------------------------------------------
      // Analyze the charged pion events and event topologies
      //----------------------------------------------
      //Loop over genie final state particles and look for pions:
      int chargedPion = 0;
      int proton = 0;
      int neutron = 0;
      int neutralPion = 0;
      double protonMass = 0.9382; // GeV
      // double pionMass = 0.1395; // GeV
      std::vector<double> protonP (0,0);

      if (verbose){
        std::cout << "Genie vector sizes: \n"
                  << "\t PDG : " << geniePDG ->size() << "\n"
                  << "\t   E : " << genieE   ->size() << "\n"
                  << "\t  Px : " << geniePx  ->size() << "\n"
                  << "\t  Py : " << geniePy  ->size() << "\n"
                  << "\t  Pz : " << geniePz  ->size() << "\n";
      }
      //Loop over genie final state particles and look for pions:
      for (unsigned int i = 0; i < geniePDG->size(); ++i)
      {
        if (geniePDG->at(i) == 211 || geniePDG->at(i) == -211){
          double e = (geniePx->at(i)*geniePx->at(i) + geniePy->at(i)*geniePy->at(i) + geniePz->at(i)*geniePz->at(i));
          if (!isCC) ChargedPionEnergy -> Fill(e, fluxweight);
          chargedPion ++;
        }
        if (geniePDG->at(i) == 111 ){
          // double e = (geniePx->at(i)*geniePx->at(i) + geniePy->at(i)*geniePy->at(i) + geniePz->at(i)*geniePz->at(i));
          neutralPion ++;
        }


        if (geniePDG->at(i) == 13 || geniePDG->at(i) == -13){
          double e = (geniePx->at(i)*geniePx->at(i) + geniePy->at(i)*geniePy->at(i) + geniePz->at(i)*geniePz->at(i));
          MuonEnergy -> Fill(e, fluxweight);
        }
        if (geniePDG->at(i) == 2212){ //protons
          //apply a cut on protons:
          double e = genieE -> at(i) - protonMass;
          if (e > 0.021) {
            proton ++;
            protonP.push_back(geniePx->at(i)*geniePx->at(i) + geniePy->at(i)*geniePy->at(i) + geniePz->at(i)*geniePz->at(i));
          }
          if (e < 0) print("Error! Energy less than 0!");
        }
        if (geniePDG->at(i) == 2112){ //neutrons
          neutron ++;
        }
      }



      if (isCC && abs(inno) == 14 && chargedPion == 0 && neutralPion == 0){
        // Fill in the info about the proton multiplicity
        if (protonP.size() != 0) ProtonMultiplicity -> Fill(proton, fluxweight);
        else ProtonMultiplicity -> Fill(0.0,fluxweight);
        // Fill in the proton momentum:
        for (std::vector<double>::iterator i = protonP.begin(); i != protonP.end(); ++i)
        {
          ProtonMomentum -> Fill(sqrt(*i), fluxweight);
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
      // Analyze those pesky NC pi0 events
      //----------------------------------------------
      int totalGammas = 0;
      double photonE = 0;
      double photonTheta = 0;
      double photonConvDist = 0;
      TVector3 photonPos;
      TVector3 photonMom;

      // ncpi0 gets filled in here, so clear the info first
      ncpi0en = 0;
      shower1P.Clear();
      shower2P.Clear();
      shower1X.Clear();
      shower2X.Clear();



      if( signal != "fosc" && signal != "numi" && NPi0FinalState > 0 ){ //added catch to make sure fosc/numi doesn't do this

        // if (!isCC) print("NC event made it here!");

        if (!isCC && verbose) std::cout << "NC event with pizeros: " << NPi0FinalState << std::endl;
        if ( isCC && verbose) std::cout << "CC event with pizeros: " << NPi0FinalState << std::endl;
        if( NPi0FinalState > 1 && !isCC )  multiPizeroEvents += fluxweight;

        if (!FoundPhotons){ 
          std::cout << "Found photons is false, bailing." << std::endl;
          N_continue_foundPhotons++;
          continue;
        }
        if (NPi0FinalState > NPi0) {
          N_continue_Pi0mismatch++;
          continue;
        }
        // if (p1PhotonConversionPos -> size() != NPi0FinalState){
        //   std::cout << "p1PhotonConversionPos -> size(): "<< p1PhotonConversionPos -> size() << "\n"
        //             << "NPi0 :" << NPi0 << "\n"
        //             << "NPi0FinalState : "<< NPi0FinalState << std::endl;
        // }
        // //   continue;
        // if (p2PhotonConversionPos -> size() != NPi0FinalState){
        //   std::cout << "p2PhotonConversionPos -> size(): "<< p2PhotonConversionPos -> size() << "\n"
        //             << "NPi0 :" << NPi0 << "\n"
        //             << "NPi0FinalState : "<< NPi0FinalState << std::endl;
        // }
        // //   continue;

        for( int i = 0; i < NPi0FinalState; ++i ){
        
          if ( verbose ) std::cout << "pizero: " << i << std::endl;
          if( !isCC ) NCpizero += fluxweight;

          // Find the position and distance from the vertex for photon conversion:
          TVector3 photon1Pos((*p1PhotonConversionPos).at(i).X(), (*p1PhotonConversionPos).at(i).Y(), (*p1PhotonConversionPos).at(i).Z() );
          TVector3 photon1Mom((*p1PhotonConversionMom).at(i).X(), (*p1PhotonConversionMom).at(i).Y(), (*p1PhotonConversionMom).at(i).Z() );
          if ( verbose ) std::cout << std::setprecision(5) << "Photon 1 converted at (" << photon1Pos.X()
                    << ", " << photon1Pos.Y() << ", " << photon1Pos.Z() << ") cm" << std::endl;
          if ( verbose ) std::cout << "  in fiducial volume: " << utils.IsFiducial( iDet, photon1Pos ) << std::endl;
          TVector3 dist1 = photon1Pos - vtx;
          if ( verbose ) std::cout << "  converted " << dist1.Mag() << " cm from the vertex with energy " 
                    << (*p1PhotonConversionMom).at(i).E() << " MeV" << std::endl;

          TVector3 photon2Pos( (*p2PhotonConversionPos).at(i).X(), (*p2PhotonConversionPos).at(i).Y(), (*p2PhotonConversionPos).at(i).Z() );
          TVector3 photon2Mom( (*p2PhotonConversionMom).at(i).X(), (*p2PhotonConversionMom).at(i).Y(), (*p2PhotonConversionMom).at(i).Z() );
          if ( verbose ) std::cout << std::setprecision(5) << "Photon 2 converted at (" << photon2Pos.X() << ", " 
                    << photon2Pos.Y() << ", " << photon2Pos.Z() << ") cm" << std::endl;
          if ( verbose ) std::cout << "  in fiducial volume: " << utils.IsFiducial( iDet, photon2Pos ) << std::endl;
          TVector3 dist2 = photon2Pos - vtx;
          if ( verbose ) std::cout << "  converted " << dist2.Mag() << " cm from the vertex with energy " 
                    << (*p2PhotonConversionMom).at(i).E() << " MeV" << std::endl;
    
          if( utils.IsFiducial( iDet, photon1Pos ) ){ 
            ConversionDist.push_back( dist1.Mag() ); 
            totalGammas++; 
            photonE = (*p1PhotonConversionMom).at(i).E();
            photonTheta = utils.GetTheta( photon1Mom );
            photonConvDist = dist1.Mag();
            photonPos = photon1Pos;
            photonMom = photon1Mom;
            //photonConv->Fill( photon1Pos.X(), photon1Pos.Y(), photon1Pos.Z() );
            //photonConvX->Fill( photon1Pos.X() );
            //photonConvY->Fill( photon1Pos.Y() );
            //photonConvZ->Fill( photon1Pos.Z() );
          }
          if( utils.IsFiducial( iDet, photon2Pos ) ){ 
            ConversionDist.push_back( dist2.Mag() );
            totalGammas++;
            photonE = (*p2PhotonConversionMom).at(i).E();
            photonTheta = utils.GetTheta( photon2Mom );
            photonConvDist = dist2.Mag();
            photonPos = photon2Pos;
            photonMom = photon2Mom;
            //photonConv->Fill( photon2Pos.X(), photon2Pos.Y(), photon2Pos.Z() );
            //photonConvX->Fill( photon2Pos.X() );
            //photonConvY->Fill( photon2Pos.Y() );
            //photonConvZ->Fill( photon2Pos.Z() );
          }
          if ( signal == "ncpi0" &&
               NPi0FinalState == 1 &&
               utils.IsFiducial( iDet, photon1Pos ) &&
               utils.IsFiducial( iDet, photon2Pos ) &&
               isFid &&
               !isCC)
          {
            wgt = fluxweight;
            // std::cout << "wgt is " << wgt << std::endl;
            ncpi0en = (*PionMom).at(i).E();
            shower1P = TLorentzVector((*p1PhotonConversionMom).at(i).Px(),
                                     (*p1PhotonConversionMom).at(i).Py(),
                                     (*p1PhotonConversionMom).at(i).Pz(),
                                     (*p1PhotonConversionMom).at(i).E());
            shower2P = TLorentzVector((*p2PhotonConversionMom).at(i).Px(),
                                     (*p2PhotonConversionMom).at(i).Py(),
                                     (*p2PhotonConversionMom).at(i).Pz(),
                                     (*p2PhotonConversionMom).at(i).E());
            shower1X = TLorentzVector((*p1PhotonConversionPos).at(i).X(),
                                     (*p1PhotonConversionPos).at(i).Y(),
                                     (*p1PhotonConversionPos).at(i).Z(),
                                     (*p1PhotonConversionPos).at(i).T());
            shower2X = TLorentzVector((*p2PhotonConversionPos).at(i).X(),
                                     (*p2PhotonConversionPos).at(i).Y(),
                                     (*p2PhotonConversionPos).at(i).Z(),
                                     (*p2PhotonConversionPos).at(i).T());
            NCpizeroForNCDis += 1;
            // NCpizeroForNCDis += wgt;
          }
    
          if( !isCC ){
            if(  utils.IsFiducial( iDet, photon1Pos ) &&  utils.IsFiducial( iDet, photon2Pos ) ) foundBothPhotons+=fluxweight;
            if(  utils.IsFiducial( iDet, photon1Pos ) && !utils.IsFiducial( iDet, photon2Pos ) ) foundOnePhoton+=fluxweight;
            if( !utils.IsFiducial( iDet, photon1Pos ) &&  utils.IsFiducial( iDet, photon2Pos ) ) foundOnePhoton+=fluxweight;
            if( !utils.IsFiducial( iDet, photon1Pos ) && !utils.IsFiducial( iDet, photon2Pos ) ) foundNoPhotons+=fluxweight;
          }
        }
        // print ("Found both: " << foundBothPhotons);
      }

      iTop += totalGammas; //Now works for CC events too!
      
      if( totalGammas == 1 && photonE > egammaThreshold ){
      
        if( signal == "nue" && !isCC ){ // added catch to make sure NC here
          
          if( isFid ){
            NCpizeroVtxKE->Fill( 1000*utils.VertexEnergy( geniePDG, genieE ), wgt );
            NCpizeroConvD->Fill( photonConvDist, wgt );
            NCpizeroVtxKEConvD->Fill( 1000*utils.VertexEnergy( geniePDG, genieE ), photonConvDist, wgt );
          }

          // std::cout << "photonPos is (" << photonPos.X() << ", " << photonPos.Y() << ", " << photonPos.Z() << ")\n";
          // std::cout << "photonMom is (" << photonMom.X() << ", " << photonMom.Y() << ", " << photonMom.Z() << ")\n";
          ElectContainedDist = utils.GetContainedLength(photonPos, photonMom, iDet);
          if (verbose) std::cout << "The contained length of this shower is " << ElectContainedDist << std::endl;
          // if( !(isFid && utils.VertexEnergy( geniePDG, genieE ) > vtxEcut && photonConvDist > convDistCut) || !isFid ){  // >25 MeV vtx energy and 5cm separation

            ibkg = 5;
            efficiency = photonMisID;
            wgt = fluxweight*efficiency;
            
            electron_cand_energy = photonE;
            electron_cand_angle = photonTheta;
            enuccqe = utils.NuEnergyCCQE( 1000*electron_cand_energy, sqrt(pow(1000*electron_cand_energy,2) - pow(0.511,2)), 
                  electron_cand_angle, 0.511, iflux )/1000.0;
            enucalo1 = utils.NuEnergyCalo( geniePDG, genieE, true, true );
            enucalo2 = utils.NuEnergyCalo( geniePDG, genieE, false, false, prot_thresh ) + photon_energy;
            
            CcqeVsTrueE->Fill( energy, enuccqe, wgt );
            Calo1VsTrueE->Fill( energy, enucalo1, wgt );
            Calo2VsTrueE->Fill( energy, enucalo2, wgt );
            
            singlePhotonPi0->Fill( energy, wgt ); 
            singlePhotonPi0PhoE->Fill( photonE, wgt );
            singlePhotonPi0CCQE->Fill( enuccqe, wgt );
            singlePhotonPi0Calo->Fill( enucalo2, wgt );
    
            photonCCQE->Fill( enuccqe, photonE, wgt );
          
            if( enucalo2 > 0.2 && enucalo2 < 0.475 ){   // analyze LE region
              NsinglePhotonPi0_LE += wgt;
              ncpi0bkgV->Fill( Vx, Vy, Vz, wgt );
              ncpi0bkgX->Fill( Vx, wgt );
              ncpi0bkgY->Fill( Vy, wgt );
              ncpi0bkgZ->Fill( Vz, wgt );
        
              photonConv->Fill( photonPos.X(), photonPos.Y(), photonPos.Z() );
              photonConvX->Fill( photonPos.X() );
              photonConvY->Fill( photonPos.Y() );
              photonConvZ->Fill( photonPos.Z() );
            }
            
            NCsinglePhoton += wgt;
            singlePhotonE.push_back( photonE );
            singlePhotonTheta.push_back( photonTheta );
    
          // }
        }
        else if( signal == "numu" && !isCC ){
          efficiency = 0;
        }
        else if( signal == "phot" && !isCC){ //NC single photons
          efficiency = electronIDeff; //80% efficiency?
          wgt = fluxweight*efficiency;
          ibkg = 5;

          electron_cand_energy = photonE;
          electron_cand_angle = photonTheta;
          enuccqe = utils.NuEnergyCCQE( 1000*electron_cand_energy, sqrt(pow(1000*electron_cand_energy,2) - pow(0.511,2)), 
                electron_cand_angle, 0.511, iflux )/1000.0;
          enucalo1 = utils.NuEnergyCalo( geniePDG, genieE, true, true );
          enucalo2 = utils.NuEnergyCalo( geniePDG, genieE, false, false, prot_thresh ) + photon_energy;
          CcqeVsTrueE->Fill( energy, enuccqe, wgt );
          Calo1VsTrueE->Fill( energy, enucalo1, wgt );
          Calo2VsTrueE->Fill( energy, enucalo2, wgt );
        }
      }



      //----------------------------------------------
      // Track the muon till it exits the detector:
      //----------------------------------------------
      bool contained = true;
      unsigned int muonPosIndex = 0;
      // Make sure default values are 0:
      MuonExitPos *= 0.0;
      MuonExitMom *= 0.0;
      if (signal != "fosc" && signal != "numi" && isCC && abs(inno) == 14)  {
        while ( contained && muonPosIndex < MuonPos->size() ){
          TVector3 pos(MuonPos->at(muonPosIndex).X(), MuonPos->at(muonPosIndex).Y(), MuonPos->at(muonPosIndex).Z());
          contained = utils.IsActive(iDet,pos,5); //fiducial cut at 5cm
          muonPosIndex++;
        }

        // Take the data from the previous point.  It's now exiting the detector.
        // Or has stopped!
        MuonExitPos = TVector3(MuonPos->at(muonPosIndex-1).X(), MuonPos->at(muonPosIndex-1).Y(), MuonPos->at(muonPosIndex-1).Z());
        MuonExitMom = TVector3(MuonMom->at(muonPosIndex-1).X(), MuonMom->at(muonPosIndex-1).Y(), MuonMom->at(muonPosIndex-1).Z());

      }



      // //----------------------------------------------
      // // Track all charged pions till they exit the detector:
      // //----------------------------------------------
      // contained = true;
      // unsigned int pionPosIndex = 0;
      // // std::vector 
      // if (signal != "fosc" && signal != "numi" && chargedPionSign->size() != 0 ) 
      // {
      //   for (unsigned int i = 0; i < chargedPionSign->size(); i++ )
      //   {
      //     while (contained && pionPosIndex < chargedPionPos->at(i).size())
      //     {
      //       TVector3 pos(chargedPionPos->at(i).at(pionPosIndex).X(),
      //                    chargedPionPos->at(i).at(pionPosIndex).Y(),
      //                    chargedPionPos->at(i).at(pionPosIndex).Z());
      //       contained = utils.IsActive(iDet,pos,5); //fiducial cut at 5cm
      //       muonPosIndex++;
      //     }

      //     chargedPionExitPos.push_back(TVector3(MuonPos->at(muonPosIndex-1).X(), MuonPos->at(muonPosIndex-1).Y(), MuonPos->at(muonPosIndex-1).Z()));
      //     chargedPionExitMom.push_back(TVector3(MuonMom->at(muonPosIndex-1).X(), MuonMom->at(muonPosIndex-1).Y(), MuonMom->at(muonPosIndex-1).Z()));
      //   }

      // }

      //----------------------------------------------
      // Analyze single photon final state events
      //----------------------------------------------
      if (NGamma != miscPhotonConversionPos -> size()) {
        N_continue_Gamma_mismatch ++;
        continue;
      }
      if( ( signal == "nue" || signal == "phot" ) && !isCC && NGamma == 1 ){

        electron_cand_energy = (*miscPhotonConversionMom).at(0).E();

        if( electron_cand_energy  > egammaThreshold ){

          TVector3 photonSingPos((*miscPhotonConversionPos).at(0).X(), (*miscPhotonConversionPos).at(0).Y(), (*miscPhotonConversionPos).at(0).Z() );
        
          if( utils.IsFiducial( iDet, photonSingPos ) ){
        
            ibkg = 6;
            if (signal == "nue") efficiency = photonMisID;
            else efficiency = electronIDeff;
            wgt = fluxweight*efficiency;
            
            electron_cand_angle = utils.GetTheta( (*miscPhotonConversionMom).at(0).X(), 
                    (*miscPhotonConversionMom).at(0).Y(), 
                    (*miscPhotonConversionMom).at(0).Z() ); 
            enuccqe = utils.NuEnergyCCQE( 1000*electron_cand_energy, sqrt(pow(1000*electron_cand_energy,2) - pow(0.511,2)), 
                  electron_cand_angle, 0.511, iflux )/1000.0;
            enucalo1 = utils.NuEnergyCalo( geniePDG, genieE, true, true );
            enucalo2 = utils.NuEnergyCalo( geniePDG, genieE, false, false, prot_thresh ) + photon_energy;
            CcqeVsTrueE->Fill( energy, enuccqe, wgt );
            Calo1VsTrueE->Fill( energy, enucalo1, wgt );
            Calo2VsTrueE->Fill( energy, enucalo2, wgt );
            
            NCsinglePhoton += wgt;
            singlePhotonE.push_back( (*miscPhotonConversionMom).at(0).E() );
            
            singlePhotonNC->Fill( energy, wgt );
            singlePhotonNCPhoE->Fill( (*miscPhotonConversionMom).at(0).E(), wgt );
            singlePhotonNCCCQE->Fill( enuccqe, wgt );
            singlePhotonNCCalo->Fill( enucalo2, wgt );
            if( enucalo2 > 0.2 && enucalo2 < 0.475 ) NsinglePhotonNC_LE += wgt;
          }
        }
      }
      //Find out how many single gammas, if any, and if they are fiducial:
      if (NGamma > 0){

        for (int i = 0; i < NGamma; ++i)
        {
          TVector3 photonSingPos((*miscPhotonConversionPos).at(i).X(), (*miscPhotonConversionPos).at(i).Y(), (*miscPhotonConversionPos).at(0).Z() );
          if (utils.IsFiducial(iDet, photonSingPos)){
            iTop ++;
          }
        }
      }
      if (iTop > 1 && signal == "phot"){
        // For single photon signal, more than 1 EM shower can be zeroed out:
        efficiency = 0.0;
      }
      


      EMShowers -> Fill(iTop, fluxweight);

      if ( verbose ) std::cout << "weight = " << wgt << std::endl;
      
      newt->Fill();
      


    } // end loop over entries 



    TH1F * PionMuonRatio = new TH1F("PionMuonRatio", "Ratio of Pions per Muon;True Energy (GeV); Ratio", 60, 0, 3.0);
    TH1F * PionMuonRemainingRatio = new TH1F("PionMuonRemainingRatio","Ratio of Pions per Muon remaining above energy;True Energy (GeV); Ratio", 60, 0, 3.0);
    for (int i = 1; i <= 60; i++){
      double muons = MuonEnergy -> GetBinContent(i);
      double pions = ChargedPionEnergy -> GetBinContent(i);
      if (muons != 0) PionMuonRatio -> SetBinContent(i, pions/muons );
      else PionMuonRatio -> SetBinContent(i, 0);
      // Now do the integral:
      muons = MuonEnergy -> Integral(i, 60);
      pions = ChargedPionEnergy -> Integral(i, 60);
      if (muons != 0) PionMuonRemainingRatio -> SetBinContent(i, pions/muons );
      else PionMuonRemainingRatio -> SetBinContent(i, 0);
    }

    //Find out what energy max contains 90%, 95% and 99% of pions:
    double totalPions = ChargedPionEnergy -> Integral();
    double estep = 3.0/60.0;  //0 to 3 gev, steps of 60.
    //Don't complain to much about how shortsighted it is that I'm hardcoding all these numbers...
    double e90 = 0, e95 = 0, e99 = 0, e75 = 0;
    for (int i = 59; i != 0; i--){
      //Get the number up to this point:
      double tempPions = ChargedPionEnergy -> Integral(0.0, i);
      //std::cout << " tempPions is " << tempPions << " and totalPions is " << totalPions << std::endl;
      //std::cout << "At E = " << i*estep << " Ratio is " << tempPions/totalPions << std::endl;
      if (tempPions/totalPions > 0.75) e75 = i*estep;
      if (tempPions/totalPions > 0.9) e90 = i*estep;
      if (tempPions/totalPions > 0.95) e95 = i*estep;
      if (tempPions/totalPions > 0.99) e99 = i*estep;
    }
    std::cout << "99\% of pions contained by " << e99 << ", 95\% by " << e95 << ", 90% by "<< e90 << ", and 75% by "<<e75<< std::endl;
    
    //-- write histograms
    nueBackgrounds->Write();
    nueBackgroundsLepE->Write();
    nueBackgroundsCCQE->Write();
    nueBackgroundsCalo->Write();

    FluxRatio = (TH2D*) nearDetOnlyFlux->Clone();
    FluxRatio -> Divide(beamSpot);

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

    //for( unsigned long i = 0; i < singlePhotonE.size(); i++ ){
    //std::cout << " sinlgle photon: " << 1000*singlePhotonE[i] << " MeV, " << singlePhotonTheta[i] << " rad" << std::endl;
    //}
    std::cout << "==================================================" << std::endl;
    std::cout << "Output in " << outfile << std::endl;
    std::cout << "Had to bail on : " << "\n"
              << "N_continue_Pi0mismatch    : " << N_continue_Pi0mismatch << "\n"
              << "N_continue_Gamma_mismatch : " << N_continue_Gamma_mismatch << "\n"
              << "N_continue_foundPhotons   : " << N_continue_foundPhotons << "\n"
              << "N_continue_CC_muon        : " << N_continue_CC_muon << "\n";


  }

}// namespace lar1

