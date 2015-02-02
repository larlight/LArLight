
#include <iostream>
#include <map>
#include "TChain.h"
#include "TString.h"
#include "TSystem.h"

void TopologyCalc(TString target){

  gSystem->Load("liblar1Osc.so");

  Int_t nbytes = 0,nb = 0;
  Int_t evtcounter =0;
  //Int_t i,j,k;
  //Int_t ibin,jbin;
  Int_t ievt,ientry;

  TString dummytarget;
  TChain *c = new TChain("EventsTot");
  Double_t energy,wgt;
  Int_t inno,isCC,mode, ibkg;
  Double_t fluxweight;
  Double_t Elep;
  Double_t ElecCandEnergy;
  Double_t Eccqe;
  Int_t iTop = 0;
  Int_t iChan = 0;
  Int_t nuchan = 0;
  bool isFid= false;
  bool isAct= false;
  Int_t NPi0;

  bool useMultiWeights = false;


  c->Reset();
  c->Add(target);
  if (c==0) return;
  
  const Int_t nentries1 = Int_t(c->GetEntries());

  std::cout<<"Input filename:\t" << dummytarget << std::endl;
  std::cout << "Number of entries :\t" << nentries1 << std::endl;
  c->SetBranchAddress("enugen",&energy);
  c->SetBranchAddress("inno",&inno);
  c->SetBranchAddress("mode",&mode);
  c->SetBranchAddress("isCC",&isCC);
  c->SetBranchAddress("wgt",&wgt);
  c->SetBranchAddress("FluxWgt",&fluxweight);
  c->SetBranchAddress("ibkg",&ibkg);
  c->SetBranchAddress("LepE", &Elep);
  c->SetBranchAddress("ElecCandEnergy", &ElecCandEnergy);
  c->SetBranchAddress("Eccqe", & Eccqe);
  c->SetBranchAddress("Topology", &iTop);
  c->SetBranchAddress("Channel", &iChan);
  c->SetBranchAddress("nuchan", &nuchan);
  c->SetBranchAddress("IsFiducial", &isFid);
  c->SetBranchAddress("IsActive", &isAct);
  c->SetBranchAddress("NPi0FinalState", &NPi0);

  std::vector<std::vector<float> > *MultiWeight=0;
  unsigned int nWeights = 0;

  if (useMultiWeights)
    c->SetBranchAddress("MultiWeight",&MultiWeight);

  std::map < int, double> eventRatesNumuMap;
  std::map < int, double> eventRatesNueMap;
  std::map < int, double> showerMap;
  std::map < int, double> nuchanNueMap;
  std::map < int, double> nuchanNumuMap;
  std::map < int, double> pi0Map;

  std::vector< std::map < int, double> > eventRatesNumuMapMultiWeight;
  std::vector< std::map < int, double> > eventRatesNueMapMultiWeight;
  std::vector< std::map < int, double> > showerMapMultiWeight;
  std::vector< std::map < int, double> > nuchanMapNumuMultiWeight;
  std::vector< std::map < int, double> > nuchanMapNueMultiWeight;
  std::vector< std::map < int, double> > pi0MapMultiWeight;



  std::map < int, std::string> legendMap;
  std::map < int, std::string> nuchanLegendMap;
  std::map < int, std::string> pi0LegendMap;

  legendMap[0]  = "Events with 0 EM Showers";
  legendMap[1]  = "Events with 1 EM Showers";
  legendMap[2]  = "Events with 2 EM Showers";
  legendMap[3]  = "Events with 3 EM Showers";
  legendMap[4]  = "Events with 4 EM Showers";
  legendMap[5]  = "Events with 5 EM Showers";
  legendMap[6]  = "Events with 6 EM Showers";
  legendMap[7]  = "Events with 7 EM Showers";
  legendMap[8]  = "Events with 8 EM Showers";
  legendMap[9]  = "Events with 9 EM Showers";
  legendMap[10] = "Events with 10 EM Showers";
  legendMap[11] = "Events with 11 EM Showers";

  legendMap[100] = "CC with 0 pions, 0 protons, 0 neutrons";
  legendMap[101] = "CC with 0 pions, 0 protons, 1+ neutrons";
  legendMap[110] = "CC with 0 pions, 1 proton,  0 neutrons";
  legendMap[111] = "CC with 0 pions, 1 proton,  1+ neutrons";
  legendMap[120] = "CC with 0 pions, 2 protons, 0 neutrons";
  legendMap[121] = "CC with 0 pions, 2 protons, 1+ neutrons";
  legendMap[130] = "CC with 0 pions, 3+ protons, 0 neutrons";
  legendMap[131] = "CC with 0 pions, 3+ protons, 1+ neutrons ";

  legendMap[10001] = "CC Inclusive";
  legendMap[10002] = "NC Inclusive";
  legendMap[10003] = "CC 0 pi inclusive";
  legendMap[10004] = "CC 0 pi 0 p";
  legendMap[10005] = "CC 0 pi 1 p";
  legendMap[10006] = "CC 0 pi 2 p";
  legendMap[10007] = "CC 0 pi 3+ p";
  legendMap[10008] = "CC 1 pi+";
  legendMap[10009] = "CC 2+ pi+";
  legendMap[10010] = "CC 1+ pi0";
  legendMap[10011] = "NC 1 pi+";
  legendMap[10012] = "NC 2+ pi+";
  legendMap[10013] = "NC 1+ pi0";
  legendMap[10014] = "nu - e scattering";
  legendMap[10015] = ">= 1 charged Kaon";
  legendMap[10016] = "CCQE";
  legendMap[10017] = "CCRes";
  legendMap[10018] = "CCDIS";
  legendMap[10019] = "CCCoh";
  legendMap[10020] = "NC with 0 pions, any number of protons";


  legendMap[1000] = "CC with 0 charged pions, 0 protons, 0 neutrons";
  legendMap[1001] = "CC with 0 charged pions, 0 protons, 1+ neutrons";
  legendMap[1010] = "CC with 0 charged pions, 1 proton,  0 neutrons";
  legendMap[1011] = "CC with 0 charged pions, 1 proton,  1+ neutrons";
  legendMap[1020] = "CC with 0 charged pions, 2 protons, 0 neutrons";
  legendMap[1021] = "CC with 0 charged pions, 2 protons, 1+ neutrons";
  legendMap[1030] = "CC with 0 charged pions, 3+ protons, 0 neutrons";
  legendMap[1031] = "CC with 0 charged pions, 3+ protons, 1+ neutrons ";
  legendMap[1100] = "CC with 1 charged pions, 0 protons, 0 neutrons";
  legendMap[1101] = "CC with 1 charged pions, 0 protons, 1+ neutrons";
  legendMap[1110] = "CC with 1 charged pions, 1 proton,  0 neutrons";
  legendMap[1111] = "CC with 1 charged pions, 1 proton,  1+ neutrons";
  legendMap[1120] = "CC with 1 charged pions, 2+ protons, 0 neutrons";
  legendMap[1121] = "CC with 1 charged pions, 2+ protons, 1+ neutrons ";
  legendMap[1130] = "CC with 1 charged pions, 3+ protons, 0 neutrons";
  legendMap[1131] = "CC with 1 charged pions, 3+ protons, 1+ neutrons ";  
  legendMap[1200] = "CC with 2+ charged pions, 0 protons, 0 neutrons";
  legendMap[1201] = "CC with 2+ charged pions, 0 protons, 1+ neutrons";
  legendMap[1210] = "CC with 2+ charged pions, 1 proton,  0 neutrons";
  legendMap[1211] = "CC with 2+ charged pions, 1 proton,  1+ neutrons";
  legendMap[1220] = "CC with 2+ charged pions, 2+ protons, 0 neutrons";
  legendMap[1221] = "CC with 2+ charged pions, 2+ protons, 1+ neutrons";
  legendMap[1230] = "CC with 2+ charged pions, 3+ protons, 0 neutrons";
  legendMap[1231] = "CC with 2+ charged pions, 3+ protons, 1+ neutrons ";  
  legendMap[2000] = "NC with 0 charged pions, 0 protons, 0 neutrons";
  legendMap[2001] = "NC with 0 charged pions, 0 protons, 1+ neutrons";
  legendMap[2010] = "NC with 0 charged pions, 1 proton,  0 neutrons";
  legendMap[2011] = "NC with 0 charged pions, 1 proton,  1+ neutrons";
  legendMap[2020] = "NC with 0 charged pions, 2 protons, 0 neutrons";
  legendMap[2021] = "NC with 0 charged pions, 2 protons, 1+ neutrons";
  legendMap[2030] = "NC with 0 charged pions, 3+ protons, 0 neutrons";
  legendMap[2031] = "NC with 0 charged pions, 3+ protons, 1+ neutrons";
  legendMap[2100] = "NC with 1 charged pions, 0 protons, 0 neutrons";
  legendMap[2101] = "NC with 1 charged pions, 0 protons, 1+ neutrons";
  legendMap[2110] = "NC with 1 charged pions, 1 proton,  0 neutrons";
  legendMap[2111] = "NC with 1 charged pions, 1 proton,  1+ neutrons";
  legendMap[2120] = "NC with 1 charged pions, 2+ protons, 0 neutrons";
  legendMap[2121] = "NC with 1 charged pions, 2+ protons, 1+ neutrons";
  legendMap[2130] = "NC with 1 charged pions, 3+ protons, 0 neutrons";
  legendMap[2131] = "NC with 1 charged pions, 3+ protons, 1+ neutrons";
  legendMap[2200] = "NC with 2+ charged pions, 0 protons, 0 neutrons";
  legendMap[2201] = "NC with 2+ charged pions, 0 protons, 1+ neutrons";
  legendMap[2210] = "NC with 2+ charged pions, 1 proton,  0 neutrons";
  legendMap[2211] = "NC with 2+ charged pions, 1 proton,  1+ neutrons";
  legendMap[2220] = "NC with 2+ charged pions, 2+ protons, 0 neutrons";
  legendMap[2221] = "NC with 2+ charged pions, 2+ protons, 1+ neutrons";
  legendMap[2230] = "NC with 2+ charged pions, 3+ protons, 0 neutrons";
  legendMap[2231] = "NC with 2+ charged pions, 3+ protons, 1+ neutrons";
  
  nuchanLegendMap[1]  = "kCCQE";                        ///< charged current quasi-elastic                      
  nuchanLegendMap[2]  = "kNCQE";                        ///< neutral current quasi-elastic                      	
  nuchanLegendMap[3]  = "kResCCNuProtonPiPlus";         ///< resonant charged current, nu p -> l- p pi+
  nuchanLegendMap[4]  = "kResCCNuNeutronPi0";           ///< resonant charged current, nu n -> l- n pi0
  nuchanLegendMap[5]  = "kResCCNuNeutronPiPlus";        ///< resonant charged current, nu n -> l- n pi+
  nuchanLegendMap[6]  = "kResNCNuProtonPi0";            ///< resonant neutral current, nu p -> nu p pi0
  nuchanLegendMap[7]  = "kResNCNuProtonPiPlus";         ///< resonant neutral current, nu p -> nu p pi+
  nuchanLegendMap[8]  = "kResNCNuNeutronPi0";           ///< resonant neutral current, nu n -> nu n pi0
  nuchanLegendMap[9]  = "kResNCNuNeutronPiMinus";       ///< resonant neutral current, nu n -> nu p pi-
  nuchanLegendMap[10] = "kResCCNuBarNeutronPiMinus";   ///< resonant charged current, nubar n -> l+ n pi-
  nuchanLegendMap[11] = "kResCCNuBarProtonPi0";        ///< resonant charged current, nubar p -> l+ n pi0
  nuchanLegendMap[12] = "kResCCNuBarProtonPiMinus";    ///< resonant charged current, nubar p -> l+ p pi-
  nuchanLegendMap[13] = "kResNCNuBarProtonPi0";        ///< resonant charged current, nubar p -> nubar p pi0
  nuchanLegendMap[14] = "kResNCNuBarProtonPiPlus";     ///< resonant charged current, nubar p -> nubar n pi+
  nuchanLegendMap[15] = "kResNCNuBarNeutronPi0";       ///< resonant charged current, nubar n -> nubar n pi0
  nuchanLegendMap[16] = "kResNCNuBarNeutronPiMinus";   ///< resonant charged current, nubar n -> nubar p pi-  
  nuchanLegendMap[17] = "kResCCNuDeltaPlusPiPlus";   
  nuchanLegendMap[21] = "kResCCNuDelta2PlusPiMinus"; 
  nuchanLegendMap[28] = "kResCCNuBarDelta0PiMinus";  
  nuchanLegendMap[32] = "kResCCNuBarDeltaMinusPiPlu";
  nuchanLegendMap[39] = "kResCCNuProtonRhoPlus";     
  nuchanLegendMap[41] = "kResCCNuNeutronRhoPlus";    
  nuchanLegendMap[46] = "kResCCNuBarNeutronRhoMinus";
  nuchanLegendMap[48] = "kResCCNuBarNeutronRho0";    
  nuchanLegendMap[53] = "kResCCNuSigmaPlusKaonPlus"; 
  nuchanLegendMap[55] = "kResCCNuSigmaPlusKaon0";    
  nuchanLegendMap[60] = "kResCCNuBarSigmaMinusKaon0";
  nuchanLegendMap[62] = "kResCCNuBarSigma0Kaon0";    
  nuchanLegendMap[67] = "kResCCNuProtonEta";         
  nuchanLegendMap[70] = "kResCCNuBarNeutronEta";     
  nuchanLegendMap[73] = "kResCCNuKaonPlusLambda0";   
  nuchanLegendMap[76] = "kResCCNuBarKaon0Lambda0";   
  nuchanLegendMap[79] = "kResCCNuProtonPiPlusPiMinu";
  nuchanLegendMap[80] = "kResCCNuProtonPi0Pi0";      
  nuchanLegendMap[85] = "kResCCNuBarNeutronPiPlusPiMinus";
  nuchanLegendMap[86] = "kResCCNuBarNeutronPi0Pi0";  
  nuchanLegendMap[90] = "kResCCNuBarProtonPi0Pi0";   
  nuchanLegendMap[91] = "kCCDIS";                      ///< charged current deep inelastic scatter
  nuchanLegendMap[92] = "kNCDIS";                      ///< charged current deep inelastic scatter
  nuchanLegendMap[93] = "kUnUsed1";                  
  nuchanLegendMap[94] = "kUnUsed2";                  
  nuchanLegendMap[95] = "kCCQEHyperon";              
  nuchanLegendMap[96] = "kNCCOH";                    
  nuchanLegendMap[97] = "kCCCOH";                      ///< charged current coherent pion         
  nuchanLegendMap[98] = "kNuElectronElastic";          ///< neutrino electron elastic scatter        
  nuchanLegendMap[99] = "kInverseMuDecay";             ///< inverse muon decay                          

  int NCoffset = 20;

  pi0LegendMap[0] = "CC events with 0 pi0";
  pi0LegendMap[1] = "CC events with 1 pi0";
  pi0LegendMap[2] = "CC events with 2 pi0";
  pi0LegendMap[3] = "CC events with 3 pi0";
  pi0LegendMap[4] = "CC events with 4 pi0";
  pi0LegendMap[5] = "CC events with 5 pi0"; 
  pi0LegendMap[6] = "CC events with 6 pi0";
  pi0LegendMap[7] = "CC events with 7 pi0";

  pi0LegendMap[0 + NCoffset] = "NC events with 0 pi0";
  pi0LegendMap[1 + NCoffset] = "NC events with 1 pi0";
  pi0LegendMap[2 + NCoffset] = "NC events with 2 pi0";
  pi0LegendMap[3 + NCoffset] = "NC events with 3 pi0";
  pi0LegendMap[4 + NCoffset] = "NC events with 4 pi0";
  pi0LegendMap[5 + NCoffset] = "NC events with 5 pi0";
  pi0LegendMap[6 + NCoffset] = "NC events with 6 pi0";
  pi0LegendMap[7 + NCoffset] = "NC events with 7 pi0";



  for (ievt=0; ievt<nentries1; ievt++)
  {
    ientry = c->LoadTree(ievt); if (ientry < 0) break; nb = c->GetEntry(ievt); nbytes += nb;
    evtcounter++;
    if (ievt == 0){
      if (useMultiWeights) nWeights = MultiWeight->back().size();
      else nWeights = 1;
      eventRatesNumuMapMultiWeight.resize(nWeights);
      eventRatesNueMapMultiWeight.resize(nWeights);
      showerMapMultiWeight.resize(nWeights);
      nuchanMapNumuMultiWeight.resize(nWeights);
      nuchanMapNueMultiWeight.resize(nWeights);
      pi0MapMultiWeight.resize(nWeights);
    }

    if (ievt % 100000 == 0){ 
      std::cout << "On event " << ievt << " of " << nentries1 <<"\n";

    }


    //Really simple to do this with maps
    if (isAct){
      // if (useMultiWeights){
      //   std::cout << "  Printing out the multiweights for this event ...\n";
      //   for (unsigned i = 0; i < MultiWeight->back().size();i++){
      //     std::cout << "Weight " << i << " of " << nWeights 
      //               << ": " << MultiWeight->back().at(i) << std::endl; 
      //   }
      // }
      // fluxweight *= 0.333333;  
      for (unsigned int N_weight = 0; N_weight < nWeights; N_weight ++){
        double multiWeight = 0.0;
        if (useMultiWeights){
          multiWeight = fluxweight*MultiWeight->back().at(N_weight);
        } 

        //get the numu events:
        if (abs(inno) == 14 ) {
          if (N_weight==0){ 
            eventRatesNumuMap[iChan] += fluxweight;
            nuchanNumuMap[nuchan] += fluxweight;
            // 10001 is CC inclusive, 10002 is NC inclusive
            if (isCC) eventRatesNumuMap[10001] += fluxweight;
            if (!isCC) eventRatesNumuMap[10002] += fluxweight;
            if (nuchan == 98) eventRatesNumuMap[10014] += fluxweight;
            if (nuchan == 1 ) eventRatesNumuMap[10016] += fluxweight;
            if (nuchan == 3 || nuchan == 4 || nuchan == 5) 
                              eventRatesNumuMap[10017] += fluxweight;
            if (nuchan == 91) eventRatesNumuMap[10018] += fluxweight;
            if (nuchan == 97) eventRatesNumuMap[10019] += fluxweight;

          }
          eventRatesNumuMapMultiWeight[N_weight][iChan] += multiWeight;
          nuchanMapNumuMultiWeight[N_weight][nuchan] += multiWeight;
          if (isCC) eventRatesNumuMapMultiWeight[N_weight][10001] += multiWeight;
          if (!isCC) eventRatesNumuMapMultiWeight[N_weight][10002] += multiWeight;
          if (nuchan == 98) eventRatesNumuMapMultiWeight[N_weight][10014] += multiWeight;
          if (nuchan == 1 ) eventRatesNumuMapMultiWeight[N_weight][10016] += multiWeight;
          if (nuchan == 3 || nuchan == 4 || nuchan == 5) 
                            eventRatesNumuMapMultiWeight[N_weight][10017] += multiWeight;
          if (nuchan == 91) eventRatesNumuMapMultiWeight[N_weight][10018] += multiWeight;
          if (nuchan == 97) eventRatesNumuMapMultiWeight[N_weight][10019] += multiWeight;
        }
        // and the nue cc events
        else if (abs(inno) == 12 ) {
          if (N_weight==0){
            eventRatesNueMap[iChan] += fluxweight;
            nuchanNueMap[nuchan] += fluxweight;
            if (isCC) eventRatesNueMap[10001]  += fluxweight;
            if (!isCC) eventRatesNueMap[10002] += fluxweight;
            if (nuchan == 98) eventRatesNueMap[10014] += fluxweight;
            if (nuchan == 1 ) eventRatesNueMap[10016] += fluxweight;
            if (nuchan == 3 || nuchan == 4 || nuchan == 5) 
                              eventRatesNueMap[10017] += fluxweight;
            if (nuchan == 91) eventRatesNueMap[10018] += fluxweight;
            if (nuchan == 97) eventRatesNueMap[10019] += fluxweight;          }
          eventRatesNueMapMultiWeight[N_weight][iChan] += multiWeight;
          nuchanMapNueMultiWeight[N_weight][nuchan]    += multiWeight;
          if (isCC) eventRatesNueMapMultiWeight[N_weight][10001]  += multiWeight;
          if (!isCC) eventRatesNueMapMultiWeight[N_weight][10002] += multiWeight;
          if (nuchan == 98) eventRatesNueMapMultiWeight[N_weight][10014] += multiWeight;
          if (nuchan == 1 ) eventRatesNueMapMultiWeight[N_weight][10016] += multiWeight;
          if (nuchan == 3 || nuchan == 4 || nuchan == 5) 
                            eventRatesNueMapMultiWeight[N_weight][10017] += multiWeight;
          if (nuchan == 91) eventRatesNueMapMultiWeight[N_weight][10018] += multiWeight;
          if (nuchan == 97) eventRatesNueMapMultiWeight[N_weight][10019] += multiWeight;
        }

        //this is the zero charged pion category
        if (iChan < 1100 && iChan >= 1000) { 
          if (NPi0 == 0) { // make sure there are no neutral pions either.

            int tempChan = 100 + (iChan % 1000);
            if (abs(inno) == 14){
              if (N_weight == 0){
                eventRatesNumuMap[tempChan] += fluxweight;
                eventRatesNumuMap[10003]    += fluxweight;
              }
              eventRatesNumuMapMultiWeight[N_weight][tempChan] += multiWeight;
              eventRatesNumuMapMultiWeight[N_weight][10003]    += multiWeight;
            }
            if (abs(inno) == 12){
              if (N_weight == 0){
                eventRatesNueMap[tempChan] += fluxweight;
                eventRatesNueMap[10003]    += fluxweight;
              }
              eventRatesNueMapMultiWeight[N_weight][tempChan] += multiWeight;
              eventRatesNueMapMultiWeight[N_weight][10003]    += multiWeight;
            }

            // Break these events down by number of protons
            if (iChan == 1000 || iChan == 1001){
              if (abs(inno) == 14){ 
                if (N_weight == 0){
                  eventRatesNumuMap[10004] += fluxweight;
                }
                eventRatesNumuMapMultiWeight[N_weight][10004] += multiWeight;
              }
              if (abs(inno) == 12){ 
                if (N_weight == 0){
                  eventRatesNueMap[10004] += fluxweight;
                }
                eventRatesNueMapMultiWeight[N_weight][10004] += multiWeight;
              }
            }
            if (iChan == 1010 || iChan == 1011){
              if (abs(inno) == 14){ 
                if (N_weight == 0){
                  eventRatesNumuMap[10005] += fluxweight;
                }
                eventRatesNumuMapMultiWeight[N_weight][10005] += multiWeight;
              }
              if (abs(inno) == 12){ 
                if (N_weight == 0){
                  eventRatesNueMap[10005] += fluxweight;
                }
                eventRatesNueMapMultiWeight[N_weight][10005] += multiWeight;
              }
            }
            if (iChan == 1020 || iChan == 1021){
              if (abs(inno) == 14){ 
                if (N_weight == 0){
                  eventRatesNumuMap[10006] += fluxweight;
                }
                eventRatesNumuMapMultiWeight[N_weight][10006] += multiWeight;
              }
              if (abs(inno) == 12){ 
                if (N_weight == 0){
                  eventRatesNueMap[10006] += fluxweight;
                }
                eventRatesNueMapMultiWeight[N_weight][10006] += multiWeight;
              }
            }
            if (iChan == 1030 || iChan == 1031){
              if (abs(inno) == 14){ 
                if (N_weight == 0){
                  eventRatesNumuMap[10007] += fluxweight;
                }
                eventRatesNumuMapMultiWeight[N_weight][10007] += multiWeight;
              }
              if (abs(inno) == 12){ 
                if (N_weight == 0){
                  eventRatesNueMap[10007] += fluxweight;
                }
                eventRatesNueMapMultiWeight[N_weight][10007] += multiWeight;
              }
            }

          } // if NPi0 == 0

        } // ichan < 1100 && ichan > 1000

        if (NPi0 > 0){
          if (isCC){
            if (abs(inno) == 14){ 
              if (N_weight == 0){
                eventRatesNumuMap[10010] += fluxweight;
              }
              eventRatesNumuMapMultiWeight[N_weight][10010] += multiWeight;
            }
            if (abs(inno) == 12){ 
              if (N_weight == 0){
                eventRatesNueMap[10010] += fluxweight;
              }
              eventRatesNueMapMultiWeight[N_weight][10010] += multiWeight;
            }
          }
          else{
            if (abs(inno) == 14){ 
              if (N_weight == 0){
                eventRatesNumuMap[10013] += fluxweight;
              }
              eventRatesNumuMapMultiWeight[N_weight][10013] += multiWeight;
            }
            if (abs(inno) == 12){ 
              if (N_weight == 0){
                eventRatesNueMap[10013] += fluxweight;
              }
              eventRatesNueMapMultiWeight[N_weight][10013] += multiWeight;
            }
          }
        } // end NPi0 > 0

         // These are the NC events with no pions:
         if (iChan >= 2000 && iChan < 2100 && NPi0 == 0){
           //have NC with no pions of anytype:
           if (abs(inno) == 14){
             if (N_weight == 0){
               eventRatesNumuMap[10020] += fluxweight;
             }
             eventRatesNumuMapMultiWeight[N_weight][10020] += multiWeight;
           }
           if (abs(inno) == 12){
             if (N_weight == 0){
               eventRatesNueMap[10020] += fluxweight;
             }
             eventRatesNueMapMultiWeight[N_weight][10020] += multiWeight;
           }
         }
        // Fill out the rest of the CC entries

        // if (NPi0 >= 1){
        //   eventRatesNumuMap[10009] += fluxweight;
        // }
  

        // CC 1 pi+
        if (iChan >= 1100 && iChan < 1200){
          if (abs(inno) == 14){ 
            if (N_weight == 0){
              eventRatesNumuMap[10008] += fluxweight;
            }
            eventRatesNumuMapMultiWeight[N_weight][10008] += multiWeight;
          }
          if (abs(inno) == 12){ 
            if (N_weight == 0){
              eventRatesNueMap[10008] += fluxweight;
            }
            eventRatesNueMapMultiWeight[N_weight][10008] += multiWeight;
          }
        }
        // CC 2+ pi+
        if (iChan >= 1200 && iChan < 1300){
          if (abs(inno) == 14){ 
            if (N_weight == 0){
              eventRatesNumuMap[10009] += fluxweight;
            }
            eventRatesNumuMapMultiWeight[N_weight][10009] += multiWeight;
          }
          if (abs(inno) == 12){ 
            if (N_weight == 0){
              eventRatesNueMap[10009] += fluxweight;
            }
            eventRatesNueMapMultiWeight[N_weight][10009] += multiWeight;
          }
        }        
        if (iChan >= 2100 && iChan < 2200){
          if (abs(inno) == 14){ 
            if (N_weight == 0){
              eventRatesNumuMap[10011] += fluxweight;
            }
            eventRatesNumuMapMultiWeight[N_weight][10011] += multiWeight;
          }
          if (abs(inno) == 12){ 
            if (N_weight == 0){
              eventRatesNueMap[10011] += fluxweight;
            }
            eventRatesNueMapMultiWeight[N_weight][10011] += multiWeight;
          }
        }
        if (iChan >= 2200 && iChan < 2300){
          if (abs(inno) == 14){ 
            if (N_weight == 0){
              eventRatesNumuMap[10012] += fluxweight;
            }
            eventRatesNumuMapMultiWeight[N_weight][10012] += multiWeight;
          }
          if (abs(inno) == 12){ 
            if (N_weight == 0){
              eventRatesNueMap[10012] += fluxweight;
            }
            eventRatesNueMapMultiWeight[N_weight][10012] += multiWeight;
          }
        }

        // This block sorts events purely by the number of EM showers
        if (N_weight == 0){
          showerMap[iTop] += fluxweight;
        }
        showerMapMultiWeight[N_weight][iTop] 
          += multiWeight;

        // This section figures out the number of NC pi0 
        if (!isCC){
          if (N_weight == 0){
            pi0Map[NPi0 + NCoffset] += fluxweight;
          }
          pi0MapMultiWeight[N_weight][NPi0 + NCoffset] += multiWeight;
        }
        else{
          if (N_weight == 0){
            pi0Map[NPi0] += fluxweight;
          }
          pi0MapMultiWeight[N_weight][NPi0] += multiWeight;
        }    


      } // end loop over weights
      



    } // end (isAct)

  }

  std::map < int, double> eventRatesNumuMapSystematicUncert;
  std::map < int, double> eventRatesNueMapSystematicUncert;
  std::map < int, double> showerMapSystematicUncert;
  std::map < int, double> nuchanMapNumuSystematicUncert;
  std::map < int, double> nuchanMapNueSystematicUncert;
  std::map < int, double> pi0MapSystematicUncert;

  std::map < int, double> eventRatesNumuMapStatisticalUncert;
  std::map < int, double> eventRatesNueMapStatisticalUncert;
  std::map < int, double> showerMapStatisticalUncert;
  std::map < int, double> nuchanMapNumuStatisticalUncert;
  std::map < int, double> nuchanMapNueStatisticalUncert;
  std::map < int, double> pi0MapStatisticalUncert;


  // Compute the errors on the rates
  if (useMultiWeights){
    // Total event rates:
    for ( std::map<int,double>::iterator centralValuePair = eventRatesNumuMap.begin();
          centralValuePair != eventRatesNumuMap.end(); centralValuePair ++)
    {
      int key = centralValuePair->first;
      double tempRMS = 0;
      for (unsigned int N_weight = 0; N_weight < nWeights; N_weight ++){
        // if (key == 10001){
        //   std::cout << "centralValuePair->second - eventRatesNumuMapMultiWeight[N_weight][key]:"
        //             << centralValuePair->second <<  " - " << eventRatesNumuMapMultiWeight[N_weight][key]
        //             << " = " << centralValuePair->second - eventRatesNumuMapMultiWeight[N_weight][key]
        //             << std::endl;
        // }
        tempRMS += pow((centralValuePair->second - eventRatesNumuMapMultiWeight[N_weight][key]),2);
      }
      tempRMS = sqrt(tempRMS/nWeights);
      eventRatesNumuMapSystematicUncert[key] = tempRMS;
      eventRatesNumuMapStatisticalUncert[key] = sqrt(centralValuePair->second);
    }
    for ( std::map<int,double>::iterator centralValuePair = eventRatesNueMap.begin();
          centralValuePair != eventRatesNueMap.end(); centralValuePair ++)
    {
      int key = centralValuePair->first;
      double tempRMS = 0;
      for (unsigned int N_weight = 0; N_weight < nWeights; N_weight ++){
        tempRMS += pow((centralValuePair->second - eventRatesNueMapMultiWeight[N_weight][key]),2);
      }
      tempRMS = sqrt(tempRMS/nWeights);
      eventRatesNueMapSystematicUncert[key] = tempRMS;
      eventRatesNueMapStatisticalUncert[key] = sqrt(centralValuePair->second);
    }
    for ( std::map<int,double>::iterator centralValuePair = showerMap.begin();
          centralValuePair != showerMap.end(); centralValuePair ++)
    {
      int key = centralValuePair->first;
      double tempRMS = 0;
      for (unsigned int N_weight = 0; N_weight < nWeights; N_weight ++){
        tempRMS += pow((centralValuePair->second - showerMapMultiWeight[N_weight][key]),2);
      }
      tempRMS = sqrt(tempRMS/nWeights);
      showerMapSystematicUncert[key] = tempRMS;
      showerMapStatisticalUncert[key] = sqrt(centralValuePair->second);
    }
    for ( std::map<int,double>::iterator centralValuePair = nuchanNumuMap.begin();
          centralValuePair != nuchanNumuMap.end(); centralValuePair ++)
    {
      int key = centralValuePair->first;
      double tempRMS = 0;
      for (unsigned int N_weight = 0; N_weight < nWeights; N_weight ++){
        tempRMS += pow((centralValuePair->second - nuchanMapNumuMultiWeight[N_weight][key]),2);
      }
      tempRMS = sqrt(tempRMS/nWeights);
      nuchanMapNumuSystematicUncert[key] = tempRMS;
      nuchanMapNumuStatisticalUncert[key] = sqrt(centralValuePair->second);
    }
    for ( std::map<int,double>::iterator centralValuePair = nuchanNueMap.begin();
          centralValuePair != nuchanNueMap.end(); centralValuePair ++)
    {
      int key = centralValuePair->first;
      double tempRMS = 0;
      for (unsigned int N_weight = 0; N_weight < nWeights; N_weight ++){
        tempRMS += pow((centralValuePair->second - nuchanMapNueMultiWeight[N_weight][key]),2);
      }
      tempRMS = sqrt(tempRMS/nWeights);
      nuchanMapNueSystematicUncert[key] = tempRMS;
      nuchanMapNueStatisticalUncert[key] = sqrt(centralValuePair->second);
    }
    for ( std::map<int,double>::iterator centralValuePair = pi0Map.begin();
          centralValuePair != pi0Map.end(); centralValuePair ++)
    {
      int key = centralValuePair->first;
      double tempRMS = 0;
      for (unsigned int N_weight = 0; N_weight < nWeights; N_weight ++){
        tempRMS += pow((centralValuePair->second - pi0MapMultiWeight[N_weight][key]),2);
      }
      tempRMS = sqrt(tempRMS/nWeights);
      pi0MapSystematicUncert[key] = tempRMS;
      pi0MapStatisticalUncert[key] = sqrt(centralValuePair->second);
    }
  }



  // print out the results:
  std::cout << "\nNumu Results:\n\n";
  for (std::map<int, double>::iterator i = eventRatesNumuMap.begin(); i != eventRatesNumuMap.end(); ++i)
  {
    int key = i->first;
    if (key < 10000) continue;
    std::cout << legendMap[key] << ": \t" << i->second 
              << "\t" << eventRatesNumuMapSystematicUncert[key]
              << "\t" << eventRatesNumuMapStatisticalUncert[key]
              << std::endl;
  }
  std::cout << "\n Nuchan interaction Results:\n";
  for (std::map<int, double>::iterator i = nuchanNumuMap.begin(); i != nuchanNumuMap.end(); ++i)
  {
    std::cout << nuchanLegendMap[i->first] << ": \t" << i->second 
              << "\t" << nuchanMapNumuSystematicUncert[i->first]
              << "\t" << nuchanMapNumuStatisticalUncert[i->first]
              << std::endl;
  }  

  std::cout << "\nNue Results:\n\n";
  for (std::map<int, double>::iterator i = eventRatesNueMap.begin(); i != eventRatesNueMap.end(); ++i)
  {
    int key = i->first;
    if (key < 10000) continue;
    std::cout << legendMap[key] << ": \t" << i->second 
              << "\t" << eventRatesNueMapSystematicUncert[key]
              << "\t" << eventRatesNueMapStatisticalUncert[key]
              << std::endl;
  }
  for (std::map<int, double>::iterator i = nuchanNueMap.begin(); i != nuchanNueMap.end(); ++i)
  {
    std::cout << nuchanLegendMap[i->first] << ": \t" << i->second 
              << "\t" << nuchanMapNueSystematicUncert[i->first]
              << "\t" << nuchanMapNueStatisticalUncert[i->first]
              << std::endl;
  }  

  // std::cout << "\nNumber of Showers Results:\n\n";
  // for (std::map<int, double>::iterator i = showerMap.begin(); i != showerMap.end(); ++i)
  // {
  //   std::cout << legendMap[i->first] << ": \t" << i->second 
  //             << "\t" << showerMapSystematicUncert[i->first]
  //             << "\t" << showerMapStatisticalUncert[i->first]
  //             << std::endl;
  // }



  
  // std::cout << "\nNumber of pi0 Results:\n\n";
  // for (std::map<int, double>::iterator i = pi0Map.begin(); i != pi0Map.end(); ++i)
  // {
  //   std::cout << pi0LegendMap[i->first] << ": \t" << i->second 
  //             << "\t" << pi0MapSystematicUncert[i->first]
  //             << "\t" << pi0MapStatisticalUncert[i->first]
  //             << std::endl;
  // }  

}




