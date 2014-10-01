
#include <iostream>
#include <map>
#include "TChain.h"
#include "TString.h"

void TopologyCalc(TString target){


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
  Int_t NPi0;

  bool useMultiWeights = true;


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
  c->SetBranchAddress("NPi0FinalState", &NPi0);

  std::vector<std::vector<float> > *MultiWeight=0;
  unsigned int nWeights = 250;

  if (useMultiWeights)
    c->SetBranchAddress("MultiWeight",&MultiWeight);

  std::map < int, double> eventRatesMap;
  std::map < int, double> showerMap;
  std::map < int, double> nuchanMap;
  std::map < int, double> pi0Map;
  double nue_CC = 0;
  double nue_NC = 0;

  std::vector< std::map < int, double> > eventRatesMapMultiWeight;
  std::vector< std::map < int, double> > showerMapMultiWeight;
  std::vector< std::map < int, double> > nuchanMapMultiWeight;
  std::vector< std::map < int, double> > pi0MapMultiWeight;
  std::vector<double> nue_CC_MultiWeight;
  std::vector<double> nue_NC_MultiWeight;

  if (useMultiWeights){
    eventRatesMapMultiWeight.resize(nWeights);
    showerMapMultiWeight.resize(nWeights);
    nuchanMapMultiWeight.resize(nWeights);
    pi0MapMultiWeight.resize(nWeights);
    nue_CC_MultiWeight.resize(nWeights);
    nue_NC_MultiWeight.resize(nWeights);
  }

  std::map < int, std::string> legendMap;
  std::map < int, std::string> nuchanLegendMap;
  std::map < int, std::string> pi0LegendMap;

  legendMap[0] = "Events with 0 EM Showers";
  legendMap[1] = "Events with 1 EM Showers";
  legendMap[2] = "Events with 2 EM Showers";
  legendMap[3] = "Events with 3 EM Showers";
  legendMap[4] = "Events with 4 EM Showers";
  legendMap[5] = "Events with 5 EM Showers";
  legendMap[6] = "Events with 6 EM Showers";
  legendMap[7] = "Events with 7 EM Showers";
  legendMap[8] = "Events with 8 EM Showers";
  legendMap[9] = "Events with 9 EM Showers";
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

  legendMap[10000] = "NC with 0 pions, any number of protons";
  legendMap[10001] = "CC Inclusive";
  legendMap[10002] = "NC Inclusive";
  legendMap[10003] = "CC 0 pi 0 p";
  legendMap[10004] = "CC 0 pi 1 p";
  legendMap[10005] = "CC 0 pi 2 p";
  legendMap[10006] = "CC 0 pi 3+ p";
  legendMap[10007] = "CC 1 pi+";
  legendMap[10008] = "CC 2+ pi+";
  legendMap[10009] = "CC 1+ pi0";
  legendMap[10010] = "NC 1 pi+";
  legendMap[10011] = "NC 2+ pi+";
  legendMap[10012] = "NC 1+ pi0";



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

    if (ievt % 100000 == 0) 
      std::cout << "On event " << ievt << " of " << nentries1 <<"\n";

    //Really simple to do this with maps, it takes care of everything!
    if (isFid){
      //get the numu cc events:
      if (abs(inno) == 14 ) {
        eventRatesMap[iChan] += fluxweight;
        if (useMultiWeights){
          for (unsigned int N_weight = 0; N_weight < nWeights; N_weight ++){
            eventRatesMapMultiWeight[N_weight][iChan] += fluxweight*MultiWeight->back().at(N_weight);
          } 
        }
        //get the 0 pion break down (no neutral or charged pions)
        if (iChan < 1100 && iChan >= 1000) { //this is the zero charged pion category
          if (NPi0 == 0) { // make sure there are no neutral pions either.
            int tempChan = 100 + (iChan % 1000);
            eventRatesMap[tempChan] += fluxweight;
            if (useMultiWeights){
              for (unsigned int N_weight = 0; N_weight < nWeights; N_weight ++){
                eventRatesMapMultiWeight[N_weight][tempChan] += fluxweight*MultiWeight->back().at(N_weight);
              }
            }
          }
        }
      }
      //get the EM showers:
      showerMap[iTop] += fluxweight;

      if (abs(inno) == 14 ) nuchanMap[nuchan] += fluxweight;
      if (isCC && abs(inno) == 14 ) pi0Map[NPi0] += fluxweight;
      else if (!isCC && abs(inno) == 14) {
        pi0Map[NPi0 + NCoffset] += fluxweight;
        if (useMultiWeights){
          for (unsigned int N_weight = 0; N_weight < nWeights; N_weight ++){
            pi0MapMultiWeight[N_weight][NPi0 + NCoffset] += fluxweight*MultiWeight->back().at(N_weight);
          }         
        }
        if (NPi0 == 0){
          if (iChan >= 2000 && iChan < 2100){
            //have NC with no pions of anytype:
            eventRatesMap[10000] += fluxweight;
            if (useMultiWeights){
              for (unsigned int N_weight = 0; N_weight < nWeights; N_weight ++){
                eventRatesMapMultiWeight[N_weight][10000] += fluxweight*MultiWeight->back().at(N_weight);
              }
            }
          }
        }
      }

      if (abs(inno) == 12 && isCC) nue_CC += fluxweight;
      if (abs(inno) == 12 && !isCC) nue_NC += fluxweight;


      if (useMultiWeights){
        for (unsigned int N_weight = 0; N_weight < nWeights; N_weight ++){
          showerMapMultiWeight[N_weight][iTop] 
            += fluxweight*MultiWeight->back().at(N_weight);
          if (abs(inno) == 12 && isCC){
            nue_CC_MultiWeight[N_weight] 
              += fluxweight*MultiWeight->back().at(N_weight);
          }
          if (abs(inno) == 12 && !isCC) 
            nue_NC_MultiWeight[N_weight] 
              += fluxweight*MultiWeight->back().at(N_weight);
          if (abs(inno) == 14 ) 
            nuchanMapMultiWeight[N_weight][nuchan] 
              += fluxweight*MultiWeight->back().at(N_weight);
          if (isCC && abs(inno) == 14 ) 
            pi0MapMultiWeight[N_weight][NPi0] 
              += fluxweight*MultiWeight->back().at(N_weight);
        }
      }

      // Get the final state topologies that go into the table with errors
  // legendMap[10000] = "NC with 0 pions, any number of protons";
  // legendMap[10001] = "CC Inclusive"
  // legendMap[10002] = "NC Inclusive"
  // legendMap[10003] = "CC 0 pi 0 p"
  // legendMap[10004] = "CC 0 pi 1 p"
  // legendMap[10005] = "CC 0 pi 2 p"
  // legendMap[10006] = "CC 0 pi 3+ p"
  // legendMap[10007] = "CC 1 pi+"
  // legendMap[10008] = "CC 2+ pi+"
  // legendMap[10009] = "CC 1+ pi0"
  // legendMap[10010] = "NC 1 pi+"
  // legendMap[10011] = "NC 2+ pi+"
  // legendMap[10012] = "NC 1+ pi0"  


      if (iChan >= 1000 && iChan < 2000){
        eventRatesMap[10001] += fluxweight;
        if (iChan == 1000 || iChan == 1001){
          eventRatesMap[10003] += fluxweight;
        }
        else if (iChan == 1010 || iChan == 1011){
          eventRatesMap[10004] += fluxweight;
        }
        else if (iChan == 1020 || iChan == 1021){
          eventRatesMap[10005] += fluxweight;
        }
        else if (iChan == 1030 || iChan == 1031){
          eventRatesMap[10006] += fluxweight;
        }
        else if (iChan >= 1100 && iChan < 1200){
          eventRatesMap[10007] += fluxweight;
        }
        else if (iChan >= 1200 && iChan < 1300){
          eventRatesMap[10008] += fluxweight;
        }
        if (NPi0 >= 1){
          eventRatesMap[10009] += fluxweight;
        }
      }
      else{
        eventRatesMap[10002] += fluxweight;
        if (iChan >= 2100 && iChan < 2200){
          eventRatesMap[10010] += fluxweight;
        }
        if (iChan >= 2200 && iChan < 2300){
          eventRatesMap[10011] += fluxweight;
        }
        if (NPi0 >= 1){
          eventRatesMap[10012] += fluxweight;
        }
      }

      if (useMultiWeights){
        for (unsigned int N_weight = 0; N_weight < nWeights; N_weight ++){
          if (iChan >= 1000 && iChan < 2000){
            eventRatesMapMultiWeight[N_weight][10001] 
              += fluxweight*MultiWeight->back().at(N_weight);
            if (iChan == 1000 || iChan == 1001){
              eventRatesMapMultiWeight[N_weight][10003] 
                += fluxweight*MultiWeight->back().at(N_weight);
            }
            else if (iChan == 1010 || iChan == 1011){
              eventRatesMapMultiWeight[N_weight][10004] 
                += fluxweight*MultiWeight->back().at(N_weight);
            }
            else if (iChan == 1020 || iChan == 1021){
              eventRatesMapMultiWeight[N_weight][10005] 
                += fluxweight*MultiWeight->back().at(N_weight);
            }
            else if (iChan == 1030 || iChan == 1031){
              eventRatesMapMultiWeight[N_weight][10006] 
                += fluxweight*MultiWeight->back().at(N_weight);
            }
            else if (iChan >= 1100 && iChan < 1200){
              eventRatesMapMultiWeight[N_weight][10007] 
                += fluxweight*MultiWeight->back().at(N_weight);
            }
            else if (iChan >= 1200 && iChan < 1300){
              eventRatesMapMultiWeight[N_weight][10008] 
                += fluxweight*MultiWeight->back().at(N_weight);
            }
            if (NPi0 >= 1){
              eventRatesMapMultiWeight[N_weight][10009] 
                += fluxweight*MultiWeight->back().at(N_weight);
            }
          }
          else{
            eventRatesMapMultiWeight[N_weight][10002] 
              += fluxweight*MultiWeight->back().at(N_weight);
            if (iChan >= 2100 && iChan < 2200){
              eventRatesMapMultiWeight[N_weight][10010] 
                += fluxweight*MultiWeight->back().at(N_weight);
            }
            if (iChan >= 2200 && iChan < 2300){
              eventRatesMapMultiWeight[N_weight][10011] 
                += fluxweight*MultiWeight->back().at(N_weight);
            }
            if (NPi0 >= 1){
              eventRatesMapMultiWeight[N_weight][10012] 
                += fluxweight*MultiWeight->back().at(N_weight);
            }
          }
        }
      }



    }

  }

  std::map < int, double> eventRatesMapSystematicUncert;
  std::map < int, double> showerMapSystematicUncert;
  std::map < int, double> nuchanMapSystematicUncert;
  std::map < int, double> pi0MapSystematicUncert;

  std::map < int, double> eventRatesMapStatisticalUncert;
  std::map < int, double> showerMapStatisticalUncert;
  std::map < int, double> nuchanMapStatisticalUncert;
  std::map < int, double> pi0MapStatisticalUncert;

  double nue_CC_SystematicUncert;
  double nue_CC_StatisticalUncert;
  double nue_NC_SystematicUncert;
  double nue_NC_StatisticalUncert;

  // Compute the errors on the rates
  if (useMultiWeights){
    // Total event rates:
    for ( auto centralValuePair = eventRatesMap.begin();
          centralValuePair != eventRatesMap.end(); centralValuePair ++)
    {
      int key = centralValuePair->first;
      double tempRMS = 0;
      for (unsigned int N_weight = 0; N_weight < nWeights; N_weight ++){
        tempRMS += pow((centralValuePair->second - eventRatesMapMultiWeight[N_weight][key]),2);
      }
      tempRMS = sqrt(tempRMS/nWeights);
      eventRatesMapSystematicUncert[key] = tempRMS;
      eventRatesMapStatisticalUncert[key] = sqrt(centralValuePair->second);
    }

    for ( auto centralValuePair = showerMap.begin();
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
    for ( auto centralValuePair = nuchanMap.begin();
          centralValuePair != nuchanMap.end(); centralValuePair ++)
    {
      int key = centralValuePair->first;
      double tempRMS = 0;
      for (unsigned int N_weight = 0; N_weight < nWeights; N_weight ++){
        tempRMS += pow((centralValuePair->second - nuchanMapMultiWeight[N_weight][key]),2);
      }
      tempRMS = sqrt(tempRMS/nWeights);
      nuchanMapSystematicUncert[key] = tempRMS;
      nuchanMapStatisticalUncert[key] = sqrt(centralValuePair->second);
    }
    for ( auto centralValuePair = pi0Map.begin();
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

    for (unsigned int N_weight = 0; N_weight < nWeights; N_weight ++){
      std::cout << "nue_CC - nue_CC_MultiWeight[N_weight] :"
                << nue_CC << " - " << nue_CC_MultiWeight[N_weight]
                << " = " << nue_CC - nue_CC_MultiWeight[N_weight] << std::endl;
      nue_CC_SystematicUncert += pow(nue_CC - nue_CC_MultiWeight[N_weight],2);
      nue_NC_SystematicUncert += pow(nue_NC - nue_NC_MultiWeight[N_weight],2);
    }
    std::cout << "nue_CC_SystematicUncert is " << nue_CC_SystematicUncert << std::endl;
    nue_CC_SystematicUncert = sqrt(nue_CC_SystematicUncert/nWeights);
    nue_NC_SystematicUncert = sqrt(nue_NC_SystematicUncert/nWeights);
    nue_CC_StatisticalUncert = sqrt(nue_CC);
    nue_NC_StatisticalUncert = sqrt(nue_NC);
  }



  // print out the results:

  for (std::map<int, double>::iterator i = eventRatesMap.begin(); i != eventRatesMap.end(); ++i)
  {
    int key = i->first;
    std::cout << legendMap[i->first] << ": \t" << i->second 
              << "\t" << eventRatesMapSystematicUncert[i->first]
              << "\t" << eventRatesMapStatisticalUncert[i->first]
              << std::endl;
  }
  
  for (std::map<int, double>::iterator i = showerMap.begin(); i != showerMap.end(); ++i)
  {
    std::cout << legendMap[i->first] << ": \t" << i->second 
              << "\t" << showerMapSystematicUncert[i->first]
              << "\t" << showerMapStatisticalUncert[i->first]
              << std::endl;
  }

  for (std::map<int, double>::iterator i = nuchanMap.begin(); i != nuchanMap.end(); ++i)
  {
    std::cout << nuchanLegendMap[i->first] << ": \t" << i->second 
              << "\t" << nuchanMapSystematicUncert[i->first]
              << "\t" << nuchanMapStatisticalUncert[i->first]
              << std::endl;
  }  
  
  for (std::map<int, double>::iterator i = pi0Map.begin(); i != pi0Map.end(); ++i)
  {
    std::cout << pi0LegendMap[i->first] << ": \t" << i->second 
              << "\t" << pi0MapSystematicUncert[i->first]
              << "\t" << pi0MapStatisticalUncert[i->first]
              << std::endl;
  }  

  std::cout << "Nue CC events:\t" << nue_CC 
            << "\t" << nue_CC_SystematicUncert
            << "\t" << nue_CC_StatisticalUncert
            << std::endl;
  std::cout << "Nue NC events:\t" << nue_NC
            << "\t" << nue_NC_SystematicUncert
            << "\t" << nue_NC_StatisticalUncert
            << std::endl;
}




