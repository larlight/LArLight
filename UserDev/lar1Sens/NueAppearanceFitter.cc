
#include "NueAppearanceFitter.hh"

namespace lar1{

  NueAppearanceFitter::NueAppearanceFitter(){
    //---------------Start of section you can alter to change the baselines:

    //Self explanatory, this is where the ntuples you want to use are located:
    //  fileSource="/Users/cja33/Desktop/LAr1ND/lar1Repos/lar1/MC2_proposal_ntuples/";
    //Note: parts of this routine will write to that same folder, 
    //make sure its writable!

    //Set up the bins for the vector:
    //coming soon...


    //verbose causes print outs of information that is good fsior checking
    verbose = false;  
    //debug prints out info to make sure root is behaving...
    debug   = false;  

    specialNameText = "";
    specialNameTextOsc = "";
    specialNameText_far = "";
    specialNameTextOsc_far = "";

    useHighDm=false;
    useGlobBF=true;
    absolute_MWSource=false; 

    flatSystematicError = 0.20;  // Only used if nearDetStats = false.

    mode = "nu";  //beam mode to run in
    use100m = true;      //Include the detector at 100m?
    use150m = false;
    use200m = false;
    use100mLong=false;
    use470m = false;      //Include the detector at 470m?
    use700m = false;     //Include the detector at 700m?
    useT600_onaxis = true;
    useT600_offaxis = false;
    

    useInfiniteStatistics = false;

    forceRemake = false;

    //Note: there is no infrastructure to handle more than 3 baselines.
    //So, try at your own risk!
    
    ubooneScale = 1.0;       //Scale the event rates (uniformly across all events), uboone
    LAr1NDScale = (1.0); //Scale the event rates (uniformly across all events), near det
    LAr1FDScale = (1.0);     //Scale the event rates (uniformly across all events), far det
    
    energyType = "ecalo1";

    // Options are etrue, eccqe, ecalo1, ecalo2;

    //How many points in the final graph do you want?  (symmetric in x and y)
    //More points means longer run time but better graphs
    //Note: most of the run time is in looping over ntuples, which only takes awhile
    //on the very first pass!  (subsequent runs are much faster)
    npoints = 250;
    // nWeights = 1000;
    dm2FittingPoint = 0.9*npoints/2;
    sin22thFittingPoint = 0.25*npoints/2;

    ElectContainedDist=-999;

    systematicInflationAmount = 0.0;
    inflateSystematics = false;

    //grid boundaries
    
    useNearDetStats = true;           // Only matters if the covariance matrix vector is empty.
    shapeOnlyFit = false;              // Only matters with near detd stats = true
    nearDetSystematicError = 0.20;  // Only matters if userNearDetStats = true
    std::vector<std::string>  cov_max_name;
    //    if(useNearDetStats == false){    
    // cov_max_name.push_back("matrices/s_no_osc_matrix.out");

      //}
  }

  int NueAppearanceFitter::Prepare(){

    if (useT600_onaxis && useT600_offaxis){
        std::cout << "Error: can't use both T600 locations." <<std::endl;
        exit(-1);
    }
    if (use100m && use100mLong){
        std::cout << "Error: can't use both LAr1-ND and 2*LAr1-ND." << std::endl;
        exit(-1);
    }


    // char  label1[200], label2[200];

    if (mode == "nu"){
        if (energyType == "etrue")  sprintf(label1, "#nu mode, CC Events");
        if (energyType == "eccqe")  sprintf(label1, "#nu mode, CCQE Events");
        if (energyType == "ecalo1") sprintf(label1, "#nu mode, CC Events");
        if (energyType == "ecalo2") sprintf(label1, "#nu mode, CC Events");
    }
    else if (mode == "nubar"){
        if (energyType == "etrue")  sprintf(label1, "#bar{#nu} mode, CC Events");
        if (energyType == "eccqe")  sprintf(label1, "#bar{#nu} mode, CCQE Events");
        if (energyType == "ecalo1") sprintf(label1, "#bar{#nu} mode, CC Events");
        if (energyType == "ecalo2") sprintf(label1, "#bar{#nu} mode, CC Events");
    }
    else {
      std::cerr << "NO mode selected!!!\n";
      return -1;
    }

    if (energyType == "etrue")  sprintf(label2, "True Energy");
    if (energyType == "eccqe")  sprintf(label2, "CCQE Energy");
    if (energyType == "ecalo1") sprintf(label2, "Reconstructed Energy");
    if (energyType == "ecalo2") sprintf(label2, "Reconstructed Energy");

    fileNameRoot = fileSource;
    fileNameRoot += "nue_appearance_";
    fileNameRoot += energyType;
    fileNameRoot += "_";
    // if(useCovarianceMatrix)
    //   fileNameRoot+="covMat_";

    // std::vector<int> baselines;
    // std::vector<double> scales;
    // std::vector<std::string> names;
    // std::vector<double> volume;
    // std::vector<NtupleReader> readerNue;
    // std::vector<NtupleReader> readerNumu;
  
    //initialize all the baselines we'll use:
    if (use100m) baselines.push_back("100m");
    if (use100mLong) baselines.push_back("100m");
    if (use150m) baselines.push_back("150m");
    if (use200m) baselines.push_back("200m");
    if (use470m) baselines.push_back("470m");
    if (useT600_onaxis) baselines.push_back("600m_onaxis");
    if (useT600_offaxis) baselines.push_back("600m_offaxis");
    if (use100m) baselinesFancy.push_back("100m");
    if (use100mLong) baselinesFancy.push_back("100m");
    if (use150m) baselinesFancy.push_back("150m");
    if (use200m) baselinesFancy.push_back("200m");
    if (use470m) baselinesFancy.push_back("470m");
    if (useT600_onaxis) baselinesFancy.push_back("600m, on axis");
    if (useT600_offaxis) baselinesFancy.push_back("600m, off axis");
    // if (useT600){
    //   if (specialNameText_far == "600") baselines.push_back("600");
    //   else if (specialNameText_far == "800") baselines.push_back("800");
    //   else baselines.push_back("700");
    // }
    // and they're corresponding scaling factors:
    if (use100m || use150m || use200m) scales.push_back(LAr1NDScale);
    if (use100mLong) scales.push_back(LAr1NDScale);
    if (use470m) scales.push_back(ubooneScale);
    if (use700m) scales.push_back(LAr1FDScale);
    if (useT600_onaxis || useT600_offaxis) scales.push_back(LAr1FDScale);
    //some detector names just for fun:
    if (use100m || use150m || use200m) names.push_back("LAr1-ND");
    if (use100mLong) names.push_back("2*LAr1-ND");
    if (use470m) names.push_back("MicroBooNE");
    if (use700m) names.push_back("LAr1-FD");
    if (useT600_onaxis) names.push_back("T600");
    if (useT600_offaxis) names.push_back("T600");
    //fiducial volume FROM MC, also just for fun: (scale to get target volume)
    if (use100m || use150m || use200m) volume.push_back(40);
    if (use470m) volume.push_back(61.4);
    if (use700m) volume.push_back(1000);
    if (useT600_onaxis || useT600_offaxis) volume.push_back(476);
    if (use100mLong) volume.push_back(80);
    if (use100m) detNamesString += "ND_100m_";
    if (use150m) detNamesString += "ND_150m_";
    if (use200m) detNamesString += "ND_200m_";
    if (use100mLong) detNamesString += "2ND_";
    if (use470m) detNamesString += "uB_";
    if (use700m) detNamesString += "FD_";
    if (useT600_onaxis ) detNamesString += "T600_onaxis";
    if (useT600_offaxis) detNamesString += "T600_offaxis";
    if (ElectContainedDist != -999) {
        char tempstring[100];
        sprintf(tempstring,"cont%g_",ElectContainedDist);
        fileNameRoot += tempstring;
    }

    if ( (use100m && use150m) ||
         (use100m && use200m) ||
         (use150m && use200m) ){
      std::cout << "Error, can only pick one near detector location!" << std::endl;
      exit(-1);
    }

    if (use100m){
      NtupleReader a("nue",fileSource, "100m", mode, energyType, npoints, forceRemake);
      a.setContainedShowers(ElectContainedDist);
      a.setSpecialNameText(specialNameText);
      a.setSpecialNameTextOsc(specialNameTextOsc);
      if (useCovarianceMatrix){
        a.useMultiWeights(useCovarianceMatrix,useSignalCovarianceMatrix,multiWeightSource);
        a.setNWeights(nWeights);
        if (absolute_MWSource)
          a.setAbsolute_MWSource(absolute_MWSource);
      }
      readerNue.push_back(a);
      a.setSignal("numu");
      a.setSpecialNameText(specialNameText);
      // a.setSpecialNameText(specialNameTextOsc);
      readerNumu.push_back(a);
    }
    if (use150m){
      NtupleReader a("nue",fileSource, "150m", mode, energyType, npoints, forceRemake);
      a.setContainedShowers(ElectContainedDist);
      a.setSpecialNameText(specialNameText);
      a.setSpecialNameTextOsc(specialNameTextOsc);
      if (useCovarianceMatrix){
        a.useMultiWeights(useCovarianceMatrix,useSignalCovarianceMatrix,multiWeightSource);
        a.setNWeights(nWeights);
        if (absolute_MWSource)
          a.setAbsolute_MWSource(absolute_MWSource);
      }
      readerNue.push_back(a);
      a.setSignal("numu");
      a.setSpecialNameText(specialNameText);
      // a.setSpecialNameText(specialNameTextOsc);
      readerNumu.push_back(a);
    }
    if (use200m){
      NtupleReader a("nue",fileSource, "200m", mode, energyType, npoints, forceRemake);
      a.setContainedShowers(ElectContainedDist);
      a.setSpecialNameText(specialNameText);
      a.setSpecialNameTextOsc(specialNameTextOsc);
      if (useCovarianceMatrix){
        a.useMultiWeights(useCovarianceMatrix,useSignalCovarianceMatrix,multiWeightSource);
        a.setNWeights(nWeights);
        if (absolute_MWSource)
          a.setAbsolute_MWSource(absolute_MWSource);
      }
      readerNue.push_back(a);
      a.setSignal("numu");
      a.setSpecialNameText(specialNameText);
      // a.setSpecialNameText(specialNameTextOsc);
      readerNumu.push_back(a);
    }
    if (use100mLong){
      NtupleReader a("nue",fileSource, "100m", mode, energyType, npoints, forceRemake);
      a.setContainedShowers(ElectContainedDist);
      a.setSpecialNameText("long");
      a.setSpecialNameTextOsc("long");
      if (useCovarianceMatrix){
        a.useMultiWeights(useCovarianceMatrix,useSignalCovarianceMatrix,multiWeightSource);
        a.setNWeights(nWeights);
        if (absolute_MWSource)
          a.setAbsolute_MWSource(absolute_MWSource);
      }
      readerNue.push_back(a);
      a.setSignal("numu");
      // a.setSpecialNameText("");
      a.setSpecialNameTextOsc("long");
      // a.setSpecialNameText(specialNameTextOsc);
      readerNumu.push_back(a);
    }
    if (use470m){
      NtupleReader a("nue",fileSource, "470m", mode, energyType, npoints, forceRemake);
      a.setContainedShowers(ElectContainedDist);
      // a.setSpecialNameText(specialNameText);
      a.setSpecialNameTextOsc(specialNameTextOsc);
      if (useCovarianceMatrix){
        a.useMultiWeights(useCovarianceMatrix,useSignalCovarianceMatrix,multiWeightSource);
        a.setNWeights(nWeights);
        if (absolute_MWSource)
          a.setAbsolute_MWSource(absolute_MWSource);
      }
      readerNue.push_back(a);
      a.setSignal("numu");
      // a.setSpecialNameText("");
      a.setSpecialNameTextOsc("");
      // a.setSpecialNameText(specialNameTextOsc);
      readerNumu.push_back(a);
    }
    // if (use700m) {
    //   NtupleReader a("nue",fileSource, 700, mode, energyType, npoints, forceRemake);
    //   a.setContainedShowers(ElectContainedDist);
    //   a.setSpecialNameText(specialNameText);
    //   a.setSpecialNameTextOsc(specialNameTextOsc);
    //   readerNue.push_back(a);
    //   a.setSignal("numu");
    //   a.setSpecialNameText("");
    //   a.setSpecialNameTextOsc("");
    //   // a.setSpecialNameText(specialNameTextOsc);
    //   readerNumu.push_back(a);
    // }
    if (useT600_onaxis) {
      NtupleReader a("nue",fileSource, "600m_onaxis", mode, energyType, npoints, forceRemake);
      a.setContainedShowers(ElectContainedDist);
      a.setSpecialNameText(specialNameText);
      a.setSpecialNameTextOsc(specialNameTextOsc);
      if (useCovarianceMatrix){
        a.useMultiWeights(useCovarianceMatrix,useSignalCovarianceMatrix,multiWeightSource);
        a.setNWeights(nWeights);
        if (absolute_MWSource)
          a.setAbsolute_MWSource(absolute_MWSource);
      }
      readerNue.push_back(a);
      a.setSignal("numu");
      a.setSpecialNameText("");
      a.setSpecialNameTextOsc("");
      // a.setSpecialNameText(specialNameTextOsc);
      readerNumu.push_back(a);
    }
    if (useT600_offaxis) {
      NtupleReader a("nue",fileSource,  "600m_offaxis", mode, energyType, npoints, forceRemake);
      a.setContainedShowers(ElectContainedDist);
      a.setSpecialNameText(specialNameText);
      a.setSpecialNameTextOsc(specialNameTextOsc);
      if (useCovarianceMatrix){
        a.useMultiWeights(useCovarianceMatrix,useSignalCovarianceMatrix,multiWeightSource);
        a.setNWeights(nWeights);
        if (absolute_MWSource)
          a.setAbsolute_MWSource(absolute_MWSource);
      }
      readerNue.push_back(a);
      a.setSignal("numu");
      a.setSpecialNameText("");
      a.setSpecialNameTextOsc("");
      // a.setSpecialNameText(specialNameTextOsc);
      readerNumu.push_back(a);
    }
    // if (useT600) {
    //   NtupleReader a("nue",fileSource, 700, mode, energyType, npoints, forceRemake);
    //   a.setContainedShowers(ElectContainedDist);
    //   std::cout << "before: " << specialNameText_far << std::endl;
    //   if (specialNameText_far == "")
    //     specialNameText_far = "IC";
    //   else
    //     specialNameText_far.insert(0,"IC_");
    //   std::cout << "after: " << specialNameText_far << std::endl;

    //   if (specialNameTextOsc_far == "")
    //     specialNameTextOsc_far = "IC";
    //   else
    //     specialNameTextOsc_far.insert(0,"IC_");


    //   a.setSpecialNameText(specialNameText_far);
    //   a.setSpecialNameTextOsc(specialNameTextOsc_far);
    //   // a.setSpecialNameText("IC");
    //   // a.setSpecialNameTextOsc("IC");
    //   readerNue.push_back(a);
    //   a.setSignal("numu");
    //   a.setSpecialNameText("");
    //   a.setSpecialNameTextOsc("");
    //   // a.setSpecialNameText(specialNameTextOsc);
    //   readerNumu.push_back(a);
    // }
    //This value is the number of baselines:
    nL = baselines.size();
    nbins = nL*nbinsE*3;    //total number of energy bins for side-by-side distributions:
    //just in case, if the number of baselines is 1 switch off nearDetStats:
    if (nL == 1) {
        useNearDetStats=false;
        shapeOnlyFit = false;
        //flatSystematicError = nearDetSystematicError; 
    }
    // if (!useNearDetStats && !useCovarianceMatrix) shapeOnlyFit = false;

    // if (shapeOnlyFit)

    chi2FileName = fileNameRoot+mode;
    if(shapeOnlyFit){
        if (useNearDetStats){
            chi2FileName = chi2FileName+detNamesString+"_nearDetStats_shapeOnly_chi2.root";
        }
        else if (useCovarianceMatrix) {
            chi2FileName = chi2FileName+detNamesString+"_covMat_shapeOnly_chi2.root";
        }
        else{
            chi2FileName = chi2FileName+detNamesString+"_flatStats_shapeOnly_chi2.root";
        }
    }
    else{
        if (useNearDetStats){
            chi2FileName = chi2FileName+detNamesString+"_nearDetStats_chi2.root";
        }
        else if (useCovarianceMatrix){
            chi2FileName = chi2FileName+detNamesString+"_covMat_chi2.root";
        }
        else {
          chi2FileName = chi2FileName+detNamesString+"_flatStats_chi2.root";
        }
    }



    //The following section is purely informational, to let the user see what's happening...

    for (int i = 0; i < 10; i++) std::cout << "\n";
    std::cout << "Beginning sensitivity analysis...\n";
    std::cout << "Running in " << mode << " mode with the following detectors:\n";
    for (int i = 0; i < nL; i++){
        std::cout << "\t" << names[i] << " at " << baselines[i] << "m with " << volume[i]*scales[i] << " ton fiducial volume.\n";
    }
    std::cout << "\nLooking for input ntuples in this folder:\n";
    std::cout << "\t" << fileSource << std::endl;
    std::cout << "\nDebug is set to " << debug << std::endl;
    std::cout << "Verbose is set to " << verbose << std::endl;
    
    for (int i = 0; i < 10; i++) std::cout << "\n";
    
    //Make sure there is at least one detector...
    if (nL == 0){
        std::cout << "\nNo baselines selected!  What did you expect? Exiting.\n" << std::endl;
        return 2;
    }
    
    return 0;
  }

  int NueAppearanceFitter::ReadData(){

    //This loop calls the routine to read all of the ntuples.  We then read them in after.
    //You could consider changing the oscillation model by making cuts in the program that
    //reads the ntuples, which is read_ntuple.C and read_ntuple_fosc.C.
    //It might be easier to reprocess the histograms though.
    for (int i = 0; i < nL; i++){
        std::string L = baselines[i];
        Int_t returnVal;
        returnVal = readerNue[i].processData();
        if (returnVal) {  //only returns 0 on successful completion
            std::cout << "Error: failed to read the ntuple at " << L << " with error " << returnVal;
            std::cout << std::endl;
            return 1;
        }
        returnVal = readerNumu[i].processData();
//Joseph? if (useT600){continue;}
        if (returnVal) {  //only returns 0 on successful completion
            std::cout << "Error: failed to read the ntuple at " << L << " with error " << returnVal;
            std::cout << std::endl;
            return 1;
        }
    }

    //Make sure the vectors are the right size!
    eventsnueVec.resize(nL);
    eventsnumuVec.resize(nL);
    eventsnueoscVec.resize(nL);
    eventsnuefoscVec.resize(nL);
    eventsSignalBestFitNuVec.resize(nL);
    eventsSignalBestFitNubarVec.resize(nL);

    //  Event rate background vecs
    NueFromNueCC_muonVec.resize(nL);
    NueFromNueCC_chargeKaonVec.resize(nL);
    NueFromNueCC_neutKaonVec.resize(nL);
    NueFromEScatterVec.resize(nL);
    NueFromNC_pi0Vec.resize(nL);
    NueFromNC_delta0Vec.resize(nL);
    NueFromNumuCCVec.resize(nL);
    eventsNueMCStats.resize(nL);
    eventsNumuMCStats.resize(nL);
    DirtVec.resize(nL);
    OtherVec.resize(nL);

    if (useCovarianceMatrix){
      eventsNullVecMultiWeight.resize(nWeights);
      if (useSignalCovarianceMatrix) signalMultiWeight.resize(nWeights);
      for (int N_weight = 0; N_weight < nWeights; ++N_weight)
      {
        eventsNullVecMultiWeight[N_weight].resize(nL*nbinsE*3);
        if (useSignalCovarianceMatrix){
          signalMultiWeight[N_weight].resize(npoints+1);
          for (int point = 0; point <= npoints; ++point)
          {
            signalMultiWeight[N_weight][point].resize(nL*nbinsE*3);
          }
        }
      }
    }

      //     Double_t dm2,sin22th;
    Double_t sin22thBF=0.003; //The LSND best fit values of the parameters (dm2 = 1.2)
    Double_t sin22thBF_glob=0.013; // (dm = 0.43)

    if (useGlobBF)
      sin22thBF = sin22thBF_glob;
    if (useHighDm)
      sin22thBF = 50;

    //Now actually read in the histograms, which are in files generated by read_ntuple.C and _fosc.C
    //Sure, you could include this loop above, but it is easier logically to read all ntuples 
    //and then import all the results.  And that's what we're going for here.
    //You shouldn't have to change anything in this loop if you don't edit read_ntuple.C (or fosc)
    for(int b_line = 0; b_line < nL; b_line++){
      //For each detector, we read in 3 files of histograms, nuebackground, numubkg, fosc
      // eventsnueoscVec[b_line].resize(npoints+1); // make room for the osc vectors

      eventsnueVec[b_line]     = readerNue[b_line].GetData();
      eventsnueoscVec[b_line]  = readerNue[b_line].GetDataOsc();
      eventsnumuVec[b_line]    = readerNumu[b_line].GetData();
      eventsnuefoscVec[b_line] = readerNumu[b_line].GetData();
      std::transform( eventsnuefoscVec[b_line].begin(), eventsnuefoscVec[b_line].end(),
          eventsnuefoscVec[b_line].begin(), 
          std::bind2nd(std::multiplies<float>(), 0.003));
      // That last line is getting the nue background again but it should really be getting
      // the numu background!!
      // A lot of the items need to be grabbed a la carte:
      if (useCovarianceMatrix){
        eventsNueMCStats[b_line]           = readerNue[b_line].GetVectorFromTree
                                               ( (char*) "eventsNueMCVec");
        eventsNumuMCStats[b_line]          = readerNumu[b_line].GetVectorFromTree
                                               ( (char*) "eventsNumuMC");
      }
      NueFromNueCC_muonVec[b_line]       = readerNue[b_line].GetVectorFromTree
                                             ( (char*) "NueFromNueCC_muonVec");
      NueFromNueCC_chargeKaonVec[b_line] = readerNue[b_line].GetVectorFromTree
                                             ( (char*) "NueFromNueCC_chargeKaonVec");
      NueFromNueCC_neutKaonVec[b_line]   = readerNue[b_line].GetVectorFromTree
                                             ( (char*) "NueFromNueCC_neutKaonVec");
      NueFromEScatterVec[b_line]         = readerNue[b_line].GetVectorFromTree
                                             ( (char*) "NueFromEScatterVec");
      NueFromNC_pi0Vec[b_line]           = readerNue[b_line].GetVectorFromTree
                                             ( (char*) "NueFromNC_pi0Vec");
      NueFromNC_delta0Vec[b_line]        = readerNue[b_line].GetVectorFromTree
                                             ( (char*) "NueFromNC_delta0Vec");
      NueFromNumuCCVec[b_line]           = readerNue[b_line].GetVectorFromTree
                                             ( (char*) "NueFromNumuCCVec");
      DirtVec[b_line]                    = readerNue[b_line].GetVectorFromTree
                                             ( (char*) "DirtVec");
      OtherVec[b_line]                   = readerNue[b_line].GetVectorFromTree
                                             ( (char*) "OtherVec");
      
      if (useHighDm){
        eventsSignalBestFitNuVec[b_line]    = readerNue[b_line].GetVectorFromTree
                  ( (char*) "edistrnueHighDmNuVec",(char*)"tvecfosc",true);
        eventsSignalBestFitNubarVec[b_line] = readerNue[b_line].GetVectorFromTree
                  ( (char*) "edistrnueHighDmNubarVec",(char*)"tvecfosc",true);
      }
      else if (useGlobBF){
        eventsSignalBestFitNuVec[b_line]    = readerNue[b_line].GetVectorFromTree
                  ( (char*) "edistrnueBestFit_globNuVec",(char*)"tvecfosc",true);
        eventsSignalBestFitNubarVec[b_line] = readerNue[b_line].GetVectorFromTree
                  ( (char*) "edistrnueBestFit_globNubarVec",(char*)"tvecfosc",true);
      }
      else {
        eventsSignalBestFitNuVec[b_line]    = readerNue[b_line].GetVectorFromTree
                  ( (char*) "edistrnueBestFitNuVec",(char*)"tvecfosc",true);
        eventsSignalBestFitNubarVec[b_line] = readerNue[b_line].GetVectorFromTree
                  ( (char*) "edistrnueBestFitNubarVec",(char*)"tvecfosc",true);
      }

      // now deal with the multiweight BS:
      if (useCovarianceMatrix){
        auto tempMultiWeightInfoNue = readerNue[b_line].GetMultiWeightData();
        auto tempMultiWeightInfoNueOsc = readerNue[b_line].GetMultiWeightDataOsc();
        auto tempMultiWeightInfoNumu = readerNumu[b_line].GetMultiWeightData();
        std::vector<float> blankVector(nbinsE, 0); 

        // append the info into the proper places, then we'll scale it as we copy it into place.
        std::vector<float> tempVector;
        tempVector.reserve(nbinsE*3);
        std::vector<float> tempVector2;
        tempVector2.reserve(nbinsE*3);
        for (int N_weight = 0; N_weight < nWeights; ++N_weight)
        {
          tempVector = utils.appendVectors(blankVector,tempMultiWeightInfoNue[N_weight],tempMultiWeightInfoNumu[N_weight]);
          for (int point = 0; point <= npoints; ++point)
          {
            if (useSignalCovarianceMatrix) 
              tempVector2 = utils.appendVectors(tempMultiWeightInfoNueOsc[N_weight][point],blankVector,blankVector);
            for (int this_bin = 0; this_bin < nbinsE*3; this_bin++)
            {
              // std::cout << "On N_weight = " << N_weight 
              //           << ", point = " << point 
              //           << ", this_bin = " << this_bin << std::endl;
              // std::cout << "    signal size: "
              //           << "["<<signalMultiWeight.size()<<"]"
              //           << "["<<signalMultiWeight.front().size()<<"]"
              //           << "["<<signalMultiWeight.front().front().size()<<"]"
              //           << ", data size: "
              //           << "["<<eventsNullVecMultiWeight.size()<<"]"
              //           << "["<<eventsNullVecMultiWeight.front().size()<<"]"
              //           << std::endl;

              if (point == 0) 
                eventsNullVecMultiWeight[N_weight][b_line*nbinsE*3 + this_bin] = scales[b_line]*tempVector[this_bin];
              if (useSignalCovarianceMatrix) 
                signalMultiWeight[N_weight][point][b_line*nbinsE*3+this_bin] = scales[b_line]*tempVector2[this_bin];
            }
          }
        }
      }


    }//end loop over baselines


    //Everything is read, now scale the vectors:
    for (int b_line = 0; b_line < nL; b_line ++){
      //using the stl to do the scaling.  
      //transform works on each element from the first arg to the second (takes iterators)
      //and stores the result in the range starting at 3rd arg.  In this case, it's an overwrite
      //bind2nd takes a binary operation (multiplies, in this case) and creates a unary op
      //by always using the same value as the second operand (scales[b_line] in this case)
      //If you're unsure, check out:
      //http://www.cplusplus.com/reference/algorithm/transform/
      //http://www.cplusplus.com/reference/functional/bind2nd/
      std::transform( eventsnueVec[b_line].begin(), eventsnueVec[b_line].end(),
                      eventsnueVec[b_line].begin(), 
                      std::bind2nd(std::multiplies<float>(), scales[b_line]));
      std::transform( eventsnumuVec[b_line].begin(), eventsnumuVec[b_line].end(),
                      eventsnumuVec[b_line].begin(), 
                      std::bind2nd(std::multiplies<float>(), scales[b_line]));
      std::transform( eventsnumuVec[b_line].begin(), eventsnumuVec[b_line].end(),
                      eventsnumuVec[b_line].begin(), 
                      std::bind2nd(std::plus<float>(), 100.0));
      std::transform( eventsnuefoscVec[b_line].begin(), eventsnuefoscVec[b_line].end(),
                      eventsnuefoscVec[b_line].begin(), 
                      std::bind2nd(std::multiplies<float>(), scales[b_line]));
      std::transform( eventsSignalBestFitNuVec[b_line].begin(), eventsSignalBestFitNuVec[b_line].end(),
                      eventsSignalBestFitNuVec[b_line].begin(), 
                      std::bind2nd(std::multiplies<float>(), scales[b_line]));
      std::transform( eventsSignalBestFitNubarVec[b_line].begin(), eventsSignalBestFitNubarVec[b_line].end(),
                      eventsSignalBestFitNubarVec[b_line].begin(), 
                      std::bind2nd(std::multiplies<float>(), scales[b_line]));
      //Scale the various background correctly:
      std::transform( NueFromNueCC_muonVec[b_line].begin(), NueFromNueCC_muonVec[b_line].end(),
                      NueFromNueCC_muonVec[b_line].begin(), 
                      std::bind2nd(std::multiplies<float>(), scales[b_line]));
      std::transform( NueFromNueCC_chargeKaonVec[b_line].begin(), NueFromNueCC_chargeKaonVec[b_line].end(),
                      NueFromNueCC_chargeKaonVec[b_line].begin(), 
                      std::bind2nd(std::multiplies<float>(), scales[b_line]));
      std::transform( NueFromNueCC_neutKaonVec[b_line].begin(), NueFromNueCC_neutKaonVec[b_line].end(),
                      NueFromNueCC_neutKaonVec[b_line].begin(), 
                      std::bind2nd(std::multiplies<float>(), scales[b_line]));
      std::transform( NueFromEScatterVec[b_line].begin(), NueFromEScatterVec[b_line].end(),
                      NueFromEScatterVec[b_line].begin(), 
                      std::bind2nd(std::multiplies<float>(), scales[b_line]));
      std::transform( NueFromNC_pi0Vec[b_line].begin(), NueFromNC_pi0Vec[b_line].end(),
                      NueFromNC_pi0Vec[b_line].begin(), 
                      std::bind2nd(std::multiplies<float>(), scales[b_line]));
      std::transform( NueFromNC_delta0Vec[b_line].begin(), NueFromNC_delta0Vec[b_line].end(),
                      NueFromNC_delta0Vec[b_line].begin(), 
                      std::bind2nd(std::multiplies<float>(), scales[b_line]));
      std::transform( NueFromNumuCCVec[b_line].begin(), NueFromNumuCCVec[b_line].end(),
                      NueFromNumuCCVec[b_line].begin(), 
                      std::bind2nd(std::multiplies<float>(), scales[b_line]));
      std::transform( DirtVec[b_line].begin(), DirtVec[b_line].end(),
                      DirtVec[b_line].begin(), 
                      std::bind2nd(std::multiplies<float>(), scales[b_line]));
      std::transform( OtherVec[b_line].begin(), OtherVec[b_line].end(),
                      OtherVec[b_line].begin(), 
                      std::bind2nd(std::multiplies<float>(), scales[b_line]));

      //Scale the best fit signals:
      std::cout << "Using scale factor of " << sin22thBF << std::endl;
      std::transform(  eventsSignalBestFitNuVec[b_line].begin(), eventsSignalBestFitNuVec[b_line].end(),
                       eventsSignalBestFitNuVec[b_line].begin(), 
                       std::bind2nd(std::multiplies<float>(), scales[b_line]*sin22thBF));
      std::transform(  eventsSignalBestFitNubarVec[b_line].begin(), eventsSignalBestFitNubarVec[b_line].end(),
                       eventsSignalBestFitNubarVec[b_line].begin(), 
                       std::bind2nd(std::multiplies<float>(), scales[b_line]*sin22thBF));


      for (int point = 0; point <= npoints; point ++){
          std::transform( eventsnueoscVec[b_line][point].begin(),
                          eventsnueoscVec[b_line][point].end(),
                          eventsnueoscVec[b_line][point].begin(), 
                          std::bind2nd(std::multiplies<float>(), scales[b_line]));    
      } //loop over points
    } //loop over baselines, ends scaling

    eventsnLVec.resize(npoints+1);
    eventsnLfitVec.resize(npoints+1);


    //Stitch together the histograms into the side by side versions (over npoints):
    //loop over npoints for eventsnL and eventsnLfit

    //=============================================
    //this histogram is as a blank histogram
    std::vector<float> blankVector(nbinsE, 0); 


    //Fill the combined histograms: 
    for (int point = 0; point <= npoints; point++){
      //this isn't elegant but it gets the job done:
      if (nL == 1){ //only one baseline
        eventsnLVec[point]    = utils.appendVectors(eventsnueoscVec[0][point],
                                              eventsnueVec[0], eventsnumuVec[0]);
        eventsnLfitVec[point] = eventsnLVec[point];
        // eventsnL[point]       = utils.makeHistogram(eventsnLVec[point]    , 0 , nbins);
        // eventsnLfit[point]    = utils.makeHistogram(eventsnLfitVec[point] , 0 , nbins);
        if (point == 0){
          eventsnLcvVec   = utils.appendVectors(eventsnuefoscVec[0], 
                                          eventsnueVec[0], eventsnumuVec[0]);
          eventsnLnullVec = utils.appendVectors(blankVector     , eventsnueVec[0], eventsnumuVec[0]);
          // eventsnLcv      = utils.makeHistogram(eventsnLcvVec   , 0 , nbins);
          // eventsnLnull    = utils.makeHistogram(eventsnLnullVec , 0 , nbins);
        }
      }
      else if (nL == 2) { //two baselines
        //append the vectors:
        eventsnLVec[point]    = utils.appendVectors( utils.appendVectors(eventsnueoscVec[0][point], eventsnueVec[0], eventsnumuVec[0]),
                                                     utils.appendVectors(eventsnueoscVec[1][point], eventsnueVec[1], eventsnumuVec[1]));
        eventsnLfitVec[point] = eventsnLVec[point];
        //make them into histograms, just for display:
        // eventsnL[point]   = utils.makeHistogram(eventsnLVec[point]    , 0 , nbins);
        // eventsnLfit[point]  = utils.makeHistogram(eventsnLfitVec[point] , 0 , nbins);
        if (point == 0){
          eventsnLcvVec       = utils.appendVectors( utils.appendVectors(eventsnuefoscVec[0] , eventsnueVec[0], eventsnumuVec[0]),
                                                     utils.appendVectors(eventsnuefoscVec[1] , eventsnueVec[1], eventsnumuVec[1]));
          eventsnLnullVec     = utils.appendVectors( utils.appendVectors(blankVector     , eventsnueVec[0], eventsnumuVec[0]),
                                                     utils.appendVectors(blankVector     , eventsnueVec[1], eventsnumuVec[1]));
          // eventsnLcv    = utils.makeHistogram(eventsnLcvVec   , 0 , nbins);
          // eventsnLnull  = utils.makeHistogram(eventsnLnullVec   , 0 , nbins);
        }
      }
      else if (nL == 3) { //three baselines
        //append the vectors:
        eventsnLVec[point] = utils.appendVectors( utils.appendVectors(eventsnueoscVec[0][point], eventsnueVec[0], eventsnumuVec[0]),
                                                  utils.appendVectors(eventsnueoscVec[1][point], eventsnueVec[1], eventsnumuVec[1]),
                                                  utils.appendVectors(eventsnueoscVec[2][point], eventsnueVec[2], eventsnumuVec[2]));
        eventsnLfitVec[point] = eventsnLVec[point];
        //make them into histograms, just for display:
        // eventsnL[point]   = utils.makeHistogram(eventsnLVec[point]    , 0 , nbins);
        // eventsnLfit[point]  = utils.makeHistogram(eventsnLfitVec[point] , 0 , nbins);
        if (point == 0){
          eventsnLcvVec   = utils.appendVectors(utils.appendVectors(eventsnuefoscVec[0] , eventsnueVec[0], eventsnumuVec[0]),
                                                  utils.appendVectors(eventsnuefoscVec[1] , eventsnueVec[1], eventsnumuVec[1]),
                                                  utils.appendVectors(eventsnuefoscVec[2] , eventsnueVec[2], eventsnumuVec[2]));
          eventsnLnullVec = utils.appendVectors(utils.appendVectors(blankVector     , eventsnueVec[0], eventsnumuVec[0]),
                                                  utils.appendVectors(blankVector     , eventsnueVec[1], eventsnumuVec[1]),
                                                  utils.appendVectors(blankVector     , eventsnueVec[2], eventsnumuVec[2]));
          // eventsnLcv    = utils.makeHistogram(eventsnLcvVec   , 0 , nbins);
          // eventsnLnull  = utils.makeHistogram(eventsnLnullVec   , 0 , nbins);
        } 
      }
      else { //4+ baselines.  No way!
        std::cout << "This macro doesn't handle more than 3 baselines." << std::endl;
      }
    }



    if (verbose){
      std::cout << "\n------------------------\nPrinting out P=0.3% values and P=0.0% values:\n";
      std::cout << "\tP=0.3%\t\tP=0\t\tosc"<<npoints/2<<"\n";
      for (unsigned int i = 0; i < eventsnLcvVec.size(); i++){
        std::cout << "\t"<<eventsnLcvVec[i] << "\t\t" << eventsnLnullVec[i] << "\t\t";
        std::cout << eventsnLfitVec[0][i] << "\n";
      }
      std::cout << "------------------------" << std::endl;
    }


    
    //Input fractional systematics covariance matrix
    //This is going to come from a txt file, from Georgia
    // fracentries = std::vector<float>( nbins, std::vector<float> ( nbins, 0.0 ) );
    fracentries.resize(nbins);
    //    for(int i = 0; i < nbins; i++){
    //  fracentries[i].resize(nbins);  
    // }
    for (auto & row : fracentries) row.resize(nbins);
    //initialized to zero
    //Try it here I guess
    
    // cov_max_name.push_back("matrices/s_no_osc_matrix.out");


    //if there is nothing in the covariance matrix vector, going to use near detector
    //statistics as the systematics downstream.  This involves going and finding
    //the near detector statistics in each case, and then populating the covariance matrix
    //Since we can't do this until we're reading in the event rates, there's no point to do
    //it here.  But we can read in the near detector stats:
    //Only have useful near det stats for nue and numu, nothing on signal.  However,
    //going to make an array 3*nbinsE long and just leave the signal entries at zero
    // std::vector<float> nearDetStats;


    // For shape only fits, the near det stats get recalculated each time chi2 gets
    // calculate.  But we do them once out here too, for the shape+rate fit.
    if (!useCovarianceMatrix){
      //by design, the nearest detector to the source is the first in the vector
      //of baselines.
      nearDetStats.resize(nbinsE*3);
      for (unsigned int i = nbinsE; i < nearDetStats.size(); i++){
        //leaving the first nbinsE entries at 0 for the signal bins
        //for bins nbinsE -> 2*nbinsE-1, use 1/sqrt(nue events in that bin)
        if (i < 2*nbinsE) nearDetStats[i] = 1/sqrt(eventsnueVec[0][i-nbinsE]);
        else nearDetStats[i] = 1/sqrt(eventsnumuVec[0][i-2*nbinsE]);
      }
      if(verbose){
        std::cout << "\nNo covariance matrix entries, calculating near det errors.\n";
        std::cout << "The fractional errors are: " << std::endl;
        for (unsigned int i = 0; i < nearDetStats.size(); i++){
          std::cout << "\tBin " << i << ":\t" << nearDetStats[i] << "\n";
        }
      }


  // }


    // std::cout << std::endl;
    //now either the covariance matrix is set, or the frac entries is filled.
    //nearDetStats is only nonezero size if the cov_max_name is empty
    
    //This vector holds the null oscillation collapsed signal (in other words,
    //it's the nue and numu appended).  It's used in the chisq calc.
    nullVec.resize(nbins-nL*nbinsE);
    // nullVec = utils.collapseVector(eventsnLnullVec, nbinsE, nL);

    
    }
    return 0;
  }

  int NueAppearanceFitter::BuildCovarianceMatrix(int sin22thpoint, int dm2point){
    // if sin22thpoint and dm2point are -1 (either of them) 
    // if means to compute the matrix for no signal.
    fractionalErrorMatrix.Clear();
    fractionalErrorMatrix.ResizeTo(nL*3*nbinsE,nL*3*nbinsE);
    correlationMatrix.Clear();
    correlationMatrix.ResizeTo(nL*3*nbinsE,nL*3*nbinsE);
    covarianceMatrix.Clear();
    covarianceMatrix.ResizeTo(nL*3*nbinsE,nL*3*nbinsE);

    Shape_covarianceMatrix.Clear();
    Shape_covarianceMatrix.ResizeTo(nL*3*nbinsE,nL*3*nbinsE);
    fractional_Shape_covarianceMatrix.Clear();
    fractional_Shape_covarianceMatrix.ResizeTo(nL*3*nbinsE,nL*3*nbinsE);
    Mixed_covarianceMatrix.Clear();
    Mixed_covarianceMatrix.ResizeTo(nL*3*nbinsE,nL*3*nbinsE);
    Norm_covarianceMatrix.Clear();
    Norm_covarianceMatrix.ResizeTo(nL*3*nbinsE,nL*3*nbinsE);


    double n_total =0.0;


    float sin22th = 1;
    if (sin22thpoint != -1)
        sin22th = pow(10.,(TMath::Log10(sin22thmin)+(sin22thpoint*1./npoints)*TMath::Log10(sin22thmax/sin22thmin)));

    TH2D * covarianceMatrixHist 
         = new TH2D("covMatHist","Covariance Matrix",
                    nL*nbinsE*3,0,nL*nbinsE*3-1,
                    nL*nbinsE*3,0,nL*nbinsE*3-1);
    

    TH2D * fractionalMatrixHist 
         = new TH2D("fracMatHist","Fractional Error Matrix",
                    nL*nbinsE*3,0,nL*nbinsE*3-1,
                    nL*nbinsE*3,0,nL*nbinsE*3-1);
    TH2D * correlationMatrixHist 
         = new TH2D("corrMatHist","Correlation Matrix",
                    nL*nbinsE*3,0,nL*nbinsE*3-1,
                    nL*nbinsE*3,0,nL*nbinsE*3-1);

    TH2D * collapsed_covarianceMatrixHist 
         = new TH2D("collapsed_covMatHist","Collapsed Covariance Matrix",
                    nL*nbinsE*2,0,nL*nbinsE*2-1,
                    nL*nbinsE*2,0,nL*nbinsE*2-1);
    

    TH2D * collapsed_fractionalMatrixHist 
         = new TH2D("collapsed_fracMatHist","Collapsed Fractional Error Matrix",
                    nL*nbinsE*2,0,nL*nbinsE*2-1,
                    nL*nbinsE*2,0,nL*nbinsE*2-1);
    TH2D * collapsed_correlationMatrixHist 
         = new TH2D("collapsed_corrMatHist","Collapsed Correlation Matrix",
                    nL*nbinsE*2,0,nL*nbinsE*2-1,
                    nL*nbinsE*2,0,nL*nbinsE*2-1);

    // Here's the method.  The nominal, no signal sample is in
    // eventsnLnullVec.  It's a vector of length nL*nbinsE*3.  It 
    // contains NO signal, and we shouldn't change that.
    // The multiweight, signalless vectors are in eventsNullVecMultiWeight
    // and each entry eventsNullVecMultiWeight[N_weight] is of the same
    // type and length as eventsnLnullVec.
    // 
    // Loop over each weight and compute the covariance between the nominal
    // and the multiweight vectors.
    // 
    // I'm going to copy the eventsnLnullVec so that it isn't changed,
    // but also because then we can add in signal to it.
    // 
    // If using a signal, it will come from eventsnLVec[point]
    
    // It's also going to be useful to copy each signal as we go through the
    // multiweights so that signal can be added there, too.
    // That signal will come from signalMultiWeight[N_weight][dm2point]
    // which will have the same format and lenght as the above vectors.
    // It will need to be modulated by sin22th.

    std::vector<float> events_nominal_COPY;
    std::vector<float> signal_nominal_COPY;

    events_nominal_COPY = eventsnLnullVec;
    signal_nominal_COPY.resize(events_nominal_COPY.size());

    if (useSignalCovarianceMatrix){
      if(dm2point != -1 && dm2point <= npoints){
        for (int bin = 0; bin < nbinsE; ++bin){
          for (int b_line =0;b_line<nL;++b_line){
            signal_nominal_COPY[b_line*nbinsE*3 + bin] = eventsnLVec[dm2point][b_line*nbinsE*3 + bin];
          } 
        }
      }
    }

    // these vectors are to hold the multiweight versions of above 
    // for each iteration
    std::vector<float> temp_events_MW_COPY;
    std::vector<float> temp_signal_MW_COPY;
    std::vector<float> saved_events_MW_COPY;
    std::vector<float> saved_signal_MW_COPY;

    std::cout << "Computing the covariance matrix...." << std::endl;
    for (int N_weight = 0; N_weight < nWeights; ++N_weight)
    {
      temp_events_MW_COPY = eventsNullVecMultiWeight[N_weight];
      temp_signal_MW_COPY.resize(temp_events_MW_COPY.size());
      
      if (useSignalCovarianceMatrix){
        if (dm2point != -1 && dm2point <= npoints){
          for (int bin = 0; bin < nbinsE; ++bin){
            for (int b_line =0;b_line<nL;++b_line){
              temp_signal_MW_COPY[b_line*nbinsE*3 + bin] = signalMultiWeight[N_weight][dm2point][b_line*nbinsE*3 + bin];
            } 
          }
        }
      }
      if (N_weight == 500){
        saved_signal_MW_COPY = temp_signal_MW_COPY;
        saved_events_MW_COPY = temp_events_MW_COPY;
      }

      for (int ibin = 0; ibin < nL*nbinsE*3; ++ibin)
      {
        // This is for computing the total number of events and is used in
        // factoring the covariance matrix into shape, mixed, and norm parts
        n_total += events_nominal_COPY[ibin] + sin22th*signal_nominal_COPY[ibin];

        for (int jbin = 0; jbin < nL*nbinsE*3; ++jbin)
        {
          if (debug && ibin == 53 && jbin == 20){
            std::cout << "This is the debug point!!\n";
            std::cout << "  nominal, i: " << events_nominal_COPY[ibin] + signal_nominal_COPY[ibin]<<std::endl;
            std::cout << "  weights, i: " << temp_events_MW_COPY[ibin] + temp_signal_MW_COPY[ibin]<<std::endl;
            std::cout << "  nominal, j: " << events_nominal_COPY[jbin] + signal_nominal_COPY[jbin]<<std::endl;
            std::cout << "  weights, j: " << temp_events_MW_COPY[jbin] + temp_signal_MW_COPY[jbin]<<std::endl;
          }
          float part1 = events_nominal_COPY[ibin] + sin22th*signal_nominal_COPY[ibin];
          part1 -= temp_events_MW_COPY[ibin] + sin22th*temp_signal_MW_COPY[ibin];
          float part2 = events_nominal_COPY[jbin] + sin22th*signal_nominal_COPY[jbin];
          part2 -= temp_events_MW_COPY[jbin] + sin22th*temp_signal_MW_COPY[jbin];
          if (ibin == 25 && jbin == 25) {
            std::cout << "ibin, jbin: ("<<ibin<<","<<jbin<<"), weight: " << N_weight <<std::endl;
            std::cout << "\tpart1: " << part1 << "\t" << "part2: " << part2 << "\n";
          }
          covarianceMatrix[ibin][jbin] += (1.0/nWeights)*(part1*part2);
        }
      }
    }


    // if (debug){
      std::cout << "Printing out the nominal and last used weight vectors:\n";
      for (int ibin = 0; ibin< nL*nbinsE*3; ++ibin){
        std::cout << events_nominal_COPY[ibin]  << "+" << signal_nominal_COPY[ibin]  << "\t\t";
        std::cout << saved_events_MW_COPY[ibin] << "+" << saved_signal_MW_COPY[ibin] << "\n";
      }
    // }

    // std::cout << "Printing out fractional covariance matrix:"<<std::endl;
    // This loop takes the error matrix and computes subsequent matrices from it:
    // correlation matrix
    // fractional error matrix
    // 
    // 

    double covMatSum = 0.0;

    for (int ibin = 0; ibin < nL*nbinsE*3; ++ibin)
    {
      for (int jbin = 0; jbin < nL*nbinsE*3; ++jbin)
      {
        covMatSum += covarianceMatrix[ibin][jbin];
        float norm = 1;
        if (covarianceMatrix[ibin][jbin] != 0)
        {
          norm = events_nominal_COPY[ibin] + sin22th*signal_nominal_COPY[ibin];
          norm *= (events_nominal_COPY[jbin] + sin22th*signal_nominal_COPY[jbin]);
          if (norm != 0)
            fractionalErrorMatrix[ibin][jbin] = covarianceMatrix[ibin][jbin]/(norm);
          else 
            fractionalErrorMatrix[ibin][jbin] = 0.0;

          if (covarianceMatrix[ibin][ibin] != 0 &&
              covarianceMatrix[jbin][jbin] != 0 )
          {
            correlationMatrix[ibin][jbin] =  covarianceMatrix[ibin][jbin];
            correlationMatrix[ibin][jbin] /= sqrt(covarianceMatrix[ibin][ibin]);
            correlationMatrix[ibin][jbin] /= sqrt(covarianceMatrix[jbin][jbin]);
          }
          else{
            correlationMatrix[ibin][jbin] = 0.0;
          }
          // std::cout << "Correlation at ["<<ibin<<"]["<<jbin<<"] is "<< correlationMatrix[ibin][jbin] << std::endl;
          covarianceMatrixHist  -> SetBinContent(ibin+1,jbin+1,covarianceMatrix[ibin][jbin]);
          fractionalMatrixHist  -> SetBinContent(ibin+1,jbin+1,fractionalErrorMatrix[ibin][jbin]);
          correlationMatrixHist -> SetBinContent(ibin+1,jbin+1,correlationMatrix[ibin][jbin]);
        }
        // std::cout << "Matrix["<<ibin<<"]["<<jbin<<"] = " 
                  // << covarianceMatrix[ibin][jbin] << std::endl;
      }

    }

    // Compute the following:
    // shape only matrix
    // fractional shape only matrix
    // mixed matrix
    // normalization matrix
    // 
    // Need to compute a few sums first:
    for (int ibin = 0; ibin < nbinsE*nL*3; ++ibin)
    {
      for (int jbin = 0; jbin < nbinsE*nL*3; ++jbin)
      {
        Shape_covarianceMatrix[ibin][jbin] = covarianceMatrix[ibin][ibin];
        double temp = events_nominal_COPY[ibin] + sin22th*signal_nominal_COPY[ibin];
        temp *= events_nominal_COPY[jbin] + sin22th*signal_nominal_COPY[jbin];
        temp /= n_total*n_total;
        temp *= covMatSum;
        Shape_covarianceMatrix[ibin][jbin] -= temp;
        temp = events_nominal_COPY[ibin] + sin22th*signal_nominal_COPY[ibin];
        temp *= events_nominal_COPY[jbin] + sin22th*signal_nominal_COPY[jbin];
        if (temp != 0)
          fractional_Shape_covarianceMatrix[ibin][jbin] = Shape_covarianceMatrix[ibin][jbin]/temp;
        else
          fractional_Shape_covarianceMatrix[ibin][jbin] = 0.0;
      }
    }

    // for (int bin = 0; bin < nbinsE*3*nL; ++bin)
    // {
    //   std::cout << fractional_Shape_covarianceMatrix[bin][bin] << "\t"
    //   std::cout << fractional_Shape_covarianceMatrix[bin][bin] << "\t"
    //   std::cout << Shape_covarianceMatrix[bin][bin] << "\t"
    //   std::cout << fractional_Shape_covarianceMatrix[bin][bin] << "\t"
    // }

// Shape_covarianceMatrix
// fractional_Shape_covarianceMatrix
// Mixed_covarianceMatrix
// Norm_covarianceMatrix

    // std::cout << "Making plots of the covariance matrix...." << std::endl;
    // TCanvas * covCanv = new TCanvas("covCanv","Covariance Matrix",700,700);
    // TCanvas * fracCanv = new TCanvas("fracCanv","Fractional Matrix",700,700);

    // gStyle->SetOptStat(0);

    // TCanvas * corrCanv = new TCanvas("corrCanv","Correlation Matrix",700,700);
    // covCanv -> cd();
    // // covarianceMatrixHist->SetDrawOption();
    // // covarianceMatrixHist -> SetOptStat(0);
    // covarianceMatrixHist -> Draw("colz");
    // fracCanv -> cd();
    // // fractionalMatrixHist->SetDrawOption();
    // // fractionalMatrixHist -> SetOptStat(0);
    // fractionalMatrixHist -> Draw("colz");
    // corrCanv->cd();
    // // fractionalMatrixHist -> SetOptStat(0);
    // correlationMatrixHist->Draw("colz");
    
    // gStyle->SetPalette(56,0);
    // correlationMatrix.SetTitle("Correlation Matrix");
    // std::cout << "Saving the covariance matrix to file...." << std::endl;
    // std::cout << "  file is: "<< fileNameRoot << "_cov_matrix.root" << std::endl;
    // gStyle->SetPalette(56,0);
    // TMatrixFBase->SetContour(999);
    // TMatrixFBase->GetZaxis()->SetRangeUser(-0.05,0.4);
    // TMatrixFBase->GetZaxis()->SetTitleFont(62);
    // TMatrixFBase->GetZaxis()->SetLabelFont(62);
    // TMatrixFBase->GetZaxis()->SetTitleSize(0.045);
    // TMatrixFBase->GetZaxis()->SetTitle("Fractional Error Matrix");
    // TMatrixFBase->GetZaxis()->SetTitleOffset(1.5);
    // TMatrixFBase->GetXaxis()->SetTitle("");
    // TMatrixFBase->GetXaxis()->SetLabelSize(0);
    // TMatrixFBase->GetXaxis()->SetTitleOffset(1.5);
    // TMatrixFBase->GetYaxis()->SetTitle("");
    // TMatrixFBase->GetYaxis()->SetTitleOffset(1.5);
    // TMatrixFBase->GetYaxis()->SetLabelSize(0);
    // TMatrixFBase->SetStats(0);
    // 
    // 
    if (savePlots){
      // Need to get the name of the matrix right:
      int systematicInt = multiWeightSource;
      TString matrixFileName = fileNameRoot + "matrixFile_";
      if (absolute_MWSource) matrixFileName += "abs_";
      matrixFileName += std::to_string(systematicInt) + "_" + detNamesString + ".root";
      TFile * fileOut = new TFile(matrixFileName,"RECREATE");
      covarianceMatrixHist -> Write();
      fractionalMatrixHist -> Write();
      correlationMatrixHist -> Write();

      // Collapse these matrices to remove the signal region for the write up.
      // Gotta write out the matrices into vector of vectors, collapse them, and 
      // then make them into new vectors:
      std::vector<std::vector<float> > covarianceMatrixVec(nL*nbinsE*3,std::vector<float>(nbinsE*nL*3, 0.0));
      std::vector<std::vector<float> > fractionalMatrixVec(nL*nbinsE*3,std::vector<float>(nbinsE*nL*3, 0.0));
      std::vector<std::vector<float> > correlationMatrixVec(nL*nbinsE*3,std::vector<float>(nbinsE*nL*3, 0.0));
      for (int ibin = 0; ibin < nL*nbinsE*3; ++ibin)
      {
        for (int jbin = 0; jbin < nL*nbinsE*3; ++jbin)
        {
          covarianceMatrixVec[ibin][jbin] = covarianceMatrix[ibin][jbin];
          fractionalMatrixVec[ibin][jbin] = fractionalErrorMatrix[ibin][jbin];
          correlationMatrixVec[ibin][jbin] = correlationMatrix[ibin][jbin];
        }
      }

      Float_t * collapsed_covnMat_array = utils.CollapseMatrix(covarianceMatrixVec,nbinsE,nL);
      Float_t * collapsed_fracMat_array = utils.CollapseMatrix(fractionalMatrixVec,nbinsE,nL);
      Float_t * collapsed_corrMat_array = utils.CollapseMatrix(correlationMatrixVec,nbinsE,nL);

      TMatrix *collapsed_covnMat = new TMatrix(nbinsE*nL*2,nbinsE*nL*2,collapsed_covnMat_array);
      TMatrix *collapsed_fracMat = new TMatrix(nbinsE*nL*2,nbinsE*nL*2,collapsed_fracMat_array);
      TMatrix *collapsed_corrMat = new TMatrix(nbinsE*nL*2,nbinsE*nL*2,collapsed_corrMat_array);

      covarianceMatrix.Write();
      fractionalErrorMatrix.Write();
      correlationMatrix.Write();

      collapsed_covnMat -> Write();
      collapsed_fracMat -> Write();
      collapsed_corrMat -> Write();

      for (int ibin = 0; ibin < nL*nbinsE*2; ++ibin)
      {
        for (int jbin = 0; jbin < nL*nbinsE*2; ++jbin)
        {
          collapsed_covarianceMatrixHist  -> SetBinContent(ibin+1, jbin+1, (*collapsed_covnMat)[ibin][jbin]);
          collapsed_fractionalMatrixHist  -> SetBinContent(ibin+1, jbin+1, (*collapsed_fracMat)[ibin][jbin]);
          collapsed_correlationMatrixHist -> SetBinContent(ibin+1, jbin+1, (*collapsed_corrMat)[ibin][jbin]);
        }
      }
      collapsed_covarianceMatrixHist  -> Write();
      collapsed_fractionalMatrixHist  -> Write();
      collapsed_correlationMatrixHist -> Write();

      // Dig the event rates out of the vectors and put them in the file
      // just for a cross check and to be sure what the matrices are

      std::vector<TH1F *>  nueEventRates;
      std::vector<TH1F *>  numuEventRates;
      nueEventRates.resize(nL);
      numuEventRates.resize(nL);
      for (int b_line = 0; b_line < nL; ++b_line)
      {
        char temp[100];
        sprintf(temp,"nueEventRates_%s",baselines[b_line].c_str());
        nueEventRates[b_line] = new TH1F(temp,"Nue Event Rates",nbinsE,emin,emax);
        sprintf(temp,"numuEventRates_%s",baselines[b_line].c_str());
        numuEventRates[b_line] = new TH1F(temp,"Numu Event Rates",nbinsE,emin,emax);
        for (int bin = 0; bin < nbinsE; ++bin)
        {
          nueEventRates[b_line]  -> SetBinContent(bin+1,events_nominal_COPY[b_line*nbinsE*3+nbinsE+bin]);
          numuEventRates[b_line] -> SetBinContent(bin+1,events_nominal_COPY[b_line*nbinsE*3+2*nbinsE+bin]);
        }

        nueEventRates[b_line] -> Write();
        numuEventRates[b_line] -> Write();

      }



      fileOut -> Close();
    }

    return 0;

  }

  int NueAppearanceFitter::MakeRatioPlots(int sin22thpoint, int dm2point){

    gStyle->SetOptStat(0000);
    gStyle->SetOptFit(0000);
    gStyle->SetPadBorderMode(0);
    gStyle->SetPadBottomMargin(0.15);
    gStyle->SetPadLeftMargin(0.15);
    gStyle->SetPadRightMargin(0.05);
    gStyle->SetFrameBorderMode(0);
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetPalette(0);
    gStyle->SetCanvasColor(0);
    gStyle->SetPadColor(0);


    if (nL < 2) {
      std::cout << "Can't make a ratio plot with only 1 detector!" << std::endl;
      return -1;
    }

    // This starts out the same as above, so getting the nominal distributions
    // and also the multiweight distributions.
    // Then, extract the near detector part and the far detector parts.
    // If there are 3 detectors, this can do both ratios at once I hope.

    // I'm going to leverage this function to also compute the systematic
    // uncertainty on the event rates themselves from the multiweights

    // Do all three signals (fosc,nue,numu) in separate vectors
    // though computed in parallel.  And bin by baseline to make it easier.
    std::vector<std::vector<float> > events_nue_nominal_COPY;
    std::vector<std::vector<float> > events_numu_nominal_COPY;
    std::vector<std::vector<float> > signal_nominal_COPY;
    events_nue_nominal_COPY.resize(nL);
    events_numu_nominal_COPY.resize(nL);
    signal_nominal_COPY.resize(nL);

    for (int b_line = 0; b_line < nL; ++b_line)
    {
      events_nue_nominal_COPY[b_line].resize(nbinsE);
      events_numu_nominal_COPY[b_line].resize(nbinsE);
      signal_nominal_COPY[b_line].resize(nbinsE);
      for (int bin = 0; bin < nbinsE; ++bin)
      {
        events_nue_nominal_COPY[b_line][bin]  = eventsnLnullVec[b_line*nbinsE*3 + bin + nbinsE];
        events_numu_nominal_COPY[b_line][bin] = eventsnLnullVec[b_line*nbinsE*3 + bin + 2*nbinsE];
        if (dm2point != -1) 
          signal_nominal_COPY[b_line] = eventsnueoscVec[dm2point][b_line*nbinsE*3 + bin];
      }
    }


    // THIS IS WHERE TO ADD SIGNAL TO THE NUE NOMINAL BACKGROUND


    // Now compute the nominal ratio
    std::vector<std::vector<float>> nominal_nue_Ratio;
    std::vector<std::vector<float>> nominal_numu_Ratio;

    nominal_nue_Ratio.resize(nL-1);
    nominal_numu_Ratio.resize(nL-1);





    // The ratio is the far detector divided by the near detector, always.
    for (int b_line = 1; b_line < nL; ++b_line)
    {
      nominal_nue_Ratio[b_line-1].resize(nbinsE);
      nominal_numu_Ratio[b_line-1].resize(nbinsE);
      for (int bin = 0; bin < nbinsE; ++bin)
      {
        nominal_nue_Ratio [b_line-1][bin] = events_nue_nominal_COPY [b_line][bin]/events_nue_nominal_COPY [0][bin];
        nominal_numu_Ratio[b_line-1][bin] = events_numu_nominal_COPY[b_line][bin]/events_numu_nominal_COPY[0][bin];
      }
    }


    // these vectors are to hold the multiweight versions of above 
    // for each iteration
    std::vector<std::vector<float> > temp_events_nue_MW_COPY;
    std::vector<std::vector<float> > temp_events_numu_MW_COPY;
    std::vector<std::vector<float> > temp_signal_MW_COPY;

    std::vector<std::vector<float>> temp_nue_Ratio;
    std::vector<std::vector<float>> temp_numu_Ratio;

    // Need a spot to hold the variance of these ratios...
    std::vector<std::vector<float> > error_events_nue_Ratio;
    std::vector<std::vector<float> > error_events_numu_Ratio;

    // And a spot to hold the ratios themselves
    std::vector<std::vector<std::vector<float> > > nue_multiWeightRatio;
    std::vector<std::vector<std::vector<float> > > numu_multiWeightRatio;

    nue_multiWeightRatio.resize(nL-1);
    numu_multiWeightRatio.resize(nL-1);

    error_events_nue_Ratio.resize(nL-1);
    error_events_numu_Ratio.resize(nL-1);    

    for (int i = 0; i < nL-1; ++i)
    {
      error_events_nue_Ratio[i].resize(nbinsE);
      error_events_numu_Ratio[i].resize(nbinsE);
      nue_multiWeightRatio[i].resize(nWeights);
      numu_multiWeightRatio[i].resize(nWeights);
    }



    std::cout << "Computing the ratio errors...." << std::endl;
    for (int N_weight = 0; N_weight < nWeights; ++N_weight)
    {

      // First step, get the ratio for this particular weight.
      // Zeroth step, get the event rates for this weight!


      temp_events_nue_MW_COPY.clear();
      temp_events_numu_MW_COPY.clear();
      temp_signal_MW_COPY.clear();

      temp_nue_Ratio.clear();
      temp_numu_Ratio.clear();
      
      temp_nue_Ratio.resize(nL-1);
      temp_numu_Ratio.resize(nL-1);

      temp_events_nue_MW_COPY.resize(nL);
      temp_events_numu_MW_COPY.resize(nL);
      temp_signal_MW_COPY.resize(nL);


      for (int b_line = 0; b_line < nL; ++b_line)
      {

        temp_events_nue_MW_COPY[b_line].resize(nbinsE);
        temp_events_numu_MW_COPY[b_line].resize(nbinsE);
        temp_signal_MW_COPY[b_line].resize(nbinsE);

        for (int bin = 0; bin < nbinsE; ++bin)
        {
          temp_events_nue_MW_COPY[b_line][bin]  = eventsNullVecMultiWeight[N_weight][b_line*nbinsE*3 + bin + nbinsE];
          temp_events_numu_MW_COPY[b_line][bin] = eventsNullVecMultiWeight[N_weight][b_line*nbinsE*3 + bin + 2*nbinsE];
          if (dm2point != -1)
            temp_signal_MW_COPY[b_line][bin]    = signalMultiWeight[N_weight][dm2point][b_line*nbinsE*3 + bin];
        }
      }



      // Ok, got all the data, compute the ratio for this universe:
      for (int b_line = 1; b_line < nL; ++b_line)
      {
        temp_nue_Ratio[b_line-1].resize(nbinsE);
        temp_numu_Ratio[b_line-1].resize(nbinsE);
        for (int bin = 0; bin < nbinsE; ++bin)
        {
          temp_nue_Ratio[b_line-1][bin]  = temp_events_nue_MW_COPY[b_line][bin] / temp_events_nue_MW_COPY[0][bin];
          temp_numu_Ratio[b_line-1][bin] = temp_events_numu_MW_COPY[b_line][bin] / temp_events_numu_MW_COPY[0][bin];
        }
        nue_multiWeightRatio[b_line-1][N_weight] = temp_nue_Ratio[b_line-1];
        numu_multiWeightRatio[b_line-1][N_weight] = temp_numu_Ratio[b_line-1];
      }

      // std::cout <<"N_weight, temp_nue_Ratio ND/600 0: " << N_weight << " "
      //           << temp_nue_Ratio[0][0]<<std::endl;

      // Now we have the nominal event rates and the event rates in this universe.
      // So, find the diffence, square it, divide by nWeights, and add it to the errors:
      for (int b_line = 0; b_line < nL-1; ++b_line)
      {
        for (int bin = 0; bin < nbinsE; ++bin)
        {
        error_events_nue_Ratio[b_line][bin]  += (1.0/nWeights)*(temp_nue_Ratio[b_line][bin]  - nominal_nue_Ratio[b_line][bin])
                                                            *(temp_nue_Ratio[b_line][bin]  - nominal_nue_Ratio[b_line][bin]);
        error_events_numu_Ratio[b_line][bin] += (1.0/nWeights)*(temp_numu_Ratio[b_line][bin] - nominal_numu_Ratio[b_line][bin])
                                                            *(temp_numu_Ratio[b_line][bin] - nominal_numu_Ratio[b_line][bin]);     
        }
      }
    }

    if (debug){
      std::cout << "Printing the nominal and last used event rates and ratios." << std::endl;
      for (int b_line = 0; b_line < nL; ++b_line)
      { 
        std::cout << "------nominal-------------" << std::endl;
        std::cout << "sig\tnue\tnumu\tsig\tnue\tnumu" << std::endl;
        for (int bin = 0; bin < nbinsE; ++bin)
        {
          std::cout << signal_nominal_COPY[b_line][bin] << "\t";
          std::cout << events_nue_nominal_COPY[b_line][bin] << "\t";
          std::cout << events_numu_nominal_COPY[b_line][bin] << "\t";
          std::cout << temp_signal_MW_COPY[b_line][bin] << "\t";
          std::cout << temp_events_nue_MW_COPY[b_line][bin] << "\t";
          std::cout << temp_events_numu_MW_COPY[b_line][bin] << "\n";
        }
        std::cout << std::endl;
      }
    }

    //Now go and plot the ratios
    std::vector<TH1F *> nue_ratioPlots;
    std::vector<TH1F *> numu_ratioPlots;
    std::vector<TH1F *> nue_errorPlots;
    std::vector<TH1F *> numu_errorPlots;
    std::vector<TH1F *> nue_stat_errorPlots;
    std::vector<TH1F *> numu_stat_errorPlots;
    std::vector<TH1F *> nue_MCstat_errorPlots;
    std::vector<TH1F *> numu_MCstat_errorPlots;

    TLegend * leg_ratio = new TLegend(0.2,0.75,.8,0.9);
    TLegend * leg_error = new TLegend(0.2,0.75,.8,0.9);
    
    leg_ratio->SetFillStyle(0);
    leg_ratio->SetFillColor(0);
    leg_ratio->SetBorderSize(0);
    leg_ratio->SetTextSize(0.045);

    leg_error->SetFillStyle(0);
    leg_error->SetFillColor(0);
    leg_error->SetBorderSize(0);
    leg_error->SetTextSize(0.045);

    nue_ratioPlots.resize(nL-1);
    numu_ratioPlots.resize(nL-1);
    nue_errorPlots.resize(nL-1);
    numu_errorPlots.resize(nL-1);
    nue_stat_errorPlots.resize(nL-1);
    numu_stat_errorPlots.resize(nL-1);
    nue_MCstat_errorPlots.resize(nL-1);
    numu_MCstat_errorPlots.resize(nL-1);

    for (int i = 0; i < nL-1; i++){
      char temp[100];
      sprintf(temp,"nue_ratioplot_%i",i);
      nue_ratioPlots[i] = new TH1F(temp,"Ratio Plot",nbinsE,emin,emax);
      sprintf(temp,"#nu_{e} Ratio of %s to %s",baselines[0].c_str(),baselines[i+1].c_str());
      nue_ratioPlots[i] -> SetTitle("");
      nue_ratioPlots[i] -> SetMinimum(0);
      nue_ratioPlots[i] -> SetMaximum(0.5);
      // nue_ratioPlots[i] -> GetXaxis() -> CenterTitle();
      nue_ratioPlots[i] -> GetXaxis() -> SetNdivisions(506);
      nue_ratioPlots[i] -> GetXaxis() -> SetLabelSize(0.05);
      nue_ratioPlots[i] -> GetYaxis() -> SetNdivisions(506);
      nue_ratioPlots[i] -> GetYaxis() -> SetLabelSize(0.05);
      nue_ratioPlots[i] -> GetXaxis() -> SetTitle("Reconstructed Neutrino Energy (GeV)");
      nue_ratioPlots[i] -> GetXaxis() -> CenterTitle();
      // nue_ratioPlots[i] -> GetYaxis() -> CenterTitle();
      nue_ratioPlots[i] -> GetXaxis() -> SetTitleSize(.05);
      nue_ratioPlots[i] -> GetYaxis() -> SetTitle("T600 (600m, on axis) / LAr1-ND (100m)");
      nue_ratioPlots[i] -> GetYaxis() -> SetTitleSize(.045);
      nue_ratioPlots[i] -> GetYaxis() -> SetTitleOffset(1.6);


      sprintf(temp,"numu_ratioplot_%i",i);
      numu_ratioPlots[i] = new TH1F(temp,"Ratio Plot",nbinsE,emin,emax);
      sprintf(temp,"#nu_{#mu} Ratio of %s to %s",baselines[0].c_str(),baselines[i+1].c_str());
      numu_ratioPlots[i] -> SetTitle(temp);
      numu_ratioPlots[i] -> SetMinimum(0);
      numu_ratioPlots[i] -> SetMaximum(1);
      numu_ratioPlots[i] -> GetXaxis() -> SetTitle("Reconstructed Energy (GeV)");
      numu_ratioPlots[i] -> GetYaxis() -> SetTitle("Event Ratio");

      sprintf(temp,"nue_errorplot_%i",i);
      nue_errorPlots[i] = new TH1F(temp,"Error Plot",nbinsE,emin,emax);
      sprintf(temp,"#nu_{e} error of %s to %s",baselines[0].c_str(),baselines[i+1].c_str());
      nue_errorPlots[i] -> SetTitle("");
      nue_errorPlots[i] -> SetMinimum(0);
      nue_errorPlots[i] -> SetMaximum(25);
      nue_errorPlots[i] -> GetXaxis() -> SetNdivisions(506);
      nue_errorPlots[i] -> GetXaxis() -> SetLabelSize(0.05);
      nue_errorPlots[i] -> GetYaxis() -> SetNdivisions(506);
      nue_errorPlots[i] -> GetYaxis() -> SetLabelSize(0.05);
      nue_errorPlots[i] -> GetXaxis() -> CenterTitle();
      nue_errorPlots[i] -> GetXaxis() -> SetTitleSize(.05);
      nue_errorPlots[i] -> GetXaxis() -> SetTitle("Reconstructed Neutrino Energy (GeV)");
      nue_errorPlots[i] -> GetYaxis() -> SetTitle("Percent Uncert. [\%]");
      nue_errorPlots[i] -> GetYaxis() -> SetTitleSize(.05);
      nue_errorPlots[i] -> GetYaxis() -> SetTitleOffset(1.2);
      nue_errorPlots[i] -> SetLineWidth(2);

      sprintf(temp,"numu_errorplot_%i",i);
      numu_errorPlots[i] = new TH1F(temp,"Uncert. Plot",nbinsE,emin,emax);
      sprintf(temp,"#nu_{#mu} Uncert. of %s to %s",baselines[0].c_str(),baselines[i+1].c_str());
      numu_errorPlots[i] -> SetTitle(temp);
      numu_errorPlots[i] -> SetMinimum(0);
      numu_errorPlots[i] -> SetMaximum(10);
      numu_errorPlots[i] -> GetXaxis() -> SetTitle("Reconstructed Energy (GeV)");
      numu_errorPlots[i] -> GetYaxis() -> SetTitle("Percent Uncert. [\%]");

      sprintf(temp,"nue_stat_errorplot_%i",i);
      nue_stat_errorPlots[i] = new TH1F(temp,"Statistical Uncert. Plot",nbinsE,emin,emax);
      sprintf(temp,"#nu_{e} Statistical Uncert. of %s to %s",baselines[0].c_str(),baselines[i+1].c_str());
      nue_stat_errorPlots[i] -> SetTitle(temp);
      nue_stat_errorPlots[i] -> SetMinimum(0);
      nue_stat_errorPlots[i] -> SetMaximum(25);
      nue_stat_errorPlots[i] -> GetXaxis() -> SetTitle("Reconstructed Energy (GeV)");
      nue_stat_errorPlots[i] -> GetYaxis() -> SetTitle("Percent Uncert. [\%]");
      nue_stat_errorPlots[i] -> SetLineWidth(2);

      sprintf(temp,"numu_stat_errorplot_%i",i);
      numu_stat_errorPlots[i] = new TH1F(temp,"Statistical Uncert. Plot",nbinsE,emin,emax);
      sprintf(temp,"#nu_{#mu} Statistical Uncert. of %s to %s",baselines[0].c_str(),baselines[i+1].c_str());
      numu_stat_errorPlots[i] -> SetTitle(temp);
      numu_stat_errorPlots[i] -> SetMinimum(0);
      numu_stat_errorPlots[i] -> SetMaximum(10);
      numu_stat_errorPlots[i] -> GetXaxis() -> SetTitle("Reconstructed Energy (GeV)");
      numu_stat_errorPlots[i] -> GetYaxis() -> SetTitle("Percent Uncert. [\%]");

      sprintf(temp,"nue_MCstat_errorplot_%i",i);
      nue_MCstat_errorPlots[i] = new TH1F(temp,"MC Statistical Uncert. Plot",nbinsE,emin,emax);
      sprintf(temp,"#nu_{e} MC Statistical Uncert. of %s to %s",baselines[0].c_str(),baselines[i+1].c_str());
      nue_MCstat_errorPlots[i] -> SetTitle(temp);
      nue_MCstat_errorPlots[i] -> SetMinimum(0);
      nue_MCstat_errorPlots[i] -> SetMaximum(25);
      nue_MCstat_errorPlots[i] -> SetLineColor(11);
      nue_MCstat_errorPlots[i] -> GetXaxis() -> SetTitle("Reconstructed Energy (GeV)");
      nue_MCstat_errorPlots[i] -> GetYaxis() -> SetTitle("Percent Uncert. [\%]");

      sprintf(temp,"numu_MCstat_errorplot_%i",i);
      numu_MCstat_errorPlots[i] = new TH1F(temp,"MC Statistical Uncert. Plot",nbinsE,emin,emax);
      sprintf(temp,"#nu_{#mu} MC Statistical Uncert. of %s to %s",baselines[0].c_str(),baselines[i+1].c_str());
      numu_MCstat_errorPlots[i] -> SetTitle(temp);
      numu_MCstat_errorPlots[i] -> SetMinimum(0);
      numu_MCstat_errorPlots[i] -> SetMaximum(10);
      numu_MCstat_errorPlots[i] -> SetLineColor(11);
      numu_MCstat_errorPlots[i] -> GetXaxis() -> SetTitle("Reconstructed Energy (GeV)");
      numu_MCstat_errorPlots[i] -> GetYaxis() -> SetTitle("Percent Uncert. [\%]");

      for (int bin = 0; bin < nbinsE; ++bin)
      {
        
        nue_ratioPlots[i]  ->SetBinContent(bin+1, nominal_nue_Ratio[i][bin]);
        numu_ratioPlots[i] ->SetBinContent(bin+1, nominal_numu_Ratio[i][bin]);
        nue_errorPlots[i]  ->SetBinContent(bin+1, 100*sqrt(error_events_nue_Ratio[i][bin])/nominal_nue_Ratio[i][bin]);
        numu_errorPlots[i] ->SetBinContent(bin+1, 100*sqrt(error_events_numu_Ratio[i][bin])/nominal_numu_Ratio[i][bin]);
        nue_stat_errorPlots[i]    -> SetBinContent(bin+1, 100*sqrt(1.0/events_nue_nominal_COPY[0][bin]
                                                                 + 1.0/events_nue_nominal_COPY[i+1][bin]));
        numu_stat_errorPlots[i]   -> SetBinContent(bin+1, 100*sqrt(1.0/events_numu_nominal_COPY[0][bin]
                                                                 + 1.0/events_numu_nominal_COPY[i+1][bin]));
        nue_MCstat_errorPlots[i]  -> SetBinContent(bin+1, 100*sqrt(1.0/eventsNueMCStats[0][bin]
                                                                 + 1.0/eventsNueMCStats[i+1][bin]));
        numu_MCstat_errorPlots[i] -> SetBinContent(bin+1, 100*sqrt(1.0/eventsNumuMCStats[0][bin]
                                                                 + 1.0/eventsNumuMCStats[i+1][bin]));
        // std::cout << "eventsNumuMCStats[0][bin]"   << eventsNumuMCStats[0][bin]   << "\t";
        // std::cout << "eventsNumuMCStats[i+1][bin]" << eventsNumuMCStats[i+1][bin] << std::endl;
      }

    }

    gStyle->SetOptStat(0);
    leg_ratio -> AddEntry(nue_ratioPlots[0],"Nominal #nu_{e} event ratio","l");
    // leg_ratio -> AddEntry();
    leg_error -> AddEntry(nue_stat_errorPlots[0],"Data Statistical Uncert.","l");
    leg_error -> AddEntry(numu_MCstat_errorPlots[0],"MC Statistical Uncert.","l");
    leg_error -> AddEntry(nue_errorPlots[0],"RMS for Ratio");
    leg_error -> SetTextSize(0.035);
    // TH1F *chr = stackedCanvas[j]->DrawFrame(emin-0.01,-0.01*(SignalNu->GetMaximum()),emax,1.0*max);

    TCanvas * canv_nue = new TCanvas("ratioplots_nue","ratioplots_nue",800,500*(nL-1));
    TCanvas * canv_numu = new TCanvas("ratioplots_numu","ratioplots_numu",800,500*(nL-1));
    canv_nue -> Divide(2,nL-1);
    canv_numu -> Divide(2,nL-1);
    for (int b_line = 0; b_line < nL-1; ++b_line)
    {
      canv_nue -> cd(2*b_line + 1);
      nue_ratioPlots[b_line] -> Draw("H");
      for (int N_weight = 0; N_weight < nWeights; ++N_weight)
      {
        TH1F * temp_hist = utils.makeHistogram(nue_multiWeightRatio[b_line][N_weight],emin,emax);
        temp_hist -> SetLineColor(11);
        temp_hist -> Draw("H same");
      }
      
      nue_ratioPlots[b_line] -> Draw("H same");
      
      leg_ratio->Draw("same");

      canv_nue -> cd(2*b_line + 2);
      nue_errorPlots[b_line] -> Draw("H");
      nue_stat_errorPlots[b_line] -> SetLineStyle(2);
      nue_stat_errorPlots[b_line] -> Draw("H same");
      nue_MCstat_errorPlots[b_line] -> SetLineStyle(2);
      nue_MCstat_errorPlots[b_line] -> Draw("H same");
      leg_error->Draw("same");

      canv_numu -> cd(2*b_line + 1);
      numu_ratioPlots[b_line] -> Draw("H");
      for (int N_weight = 0; N_weight < nWeights; ++N_weight)
      {
        TH1F * temp_hist = utils.makeHistogram(numu_multiWeightRatio[b_line][N_weight],emin,emax);
        temp_hist -> SetLineColor(11);
        temp_hist -> Draw("H same");
      }
      numu_ratioPlots[b_line] -> Draw("H same");

      leg_ratio->Draw("same");

      canv_numu -> cd(2*b_line + 2);
      numu_errorPlots[b_line] -> Draw("H");
      numu_stat_errorPlots[b_line] -> SetLineStyle(2);
      numu_stat_errorPlots[b_line] -> Draw("H same");
      numu_MCstat_errorPlots[b_line] -> SetLineStyle(2);
      numu_MCstat_errorPlots[b_line] -> Draw("H same");
      leg_error->Draw("same");

    }


    return 0;
  }

  int NueAppearanceFitter::Loop(){

    // output ntuple with chi2 values and sensitivity contour
    chi2 = new TNtuple("chi2","chi2","chisq:dm2:sin22th");
    //sensitivity contours
    x90 = new double[npoints+1]; y90 = new double[npoints+1];
    x3s = new double[npoints+1]; y3s = new double[npoints+1];
    x5s = new double[npoints+1]; y5s = new double[npoints+1];



    // // populate the vectors that hold the fractional errors
    // // for plotting purposes.
    // // statistical errors doesn't include any signal
    // // systematic error assumes no shape only fit
    // for (int i = 0; i < nL; i++){
    //   systematicErrors[i].resize(nbinsE);
    //   statisticalErrors[i].resize(nbinsE);
    //   for (int bin = 0; bin < nbinsE; bin++ ){
    //     if (useNearDetStats){
    //       if (i == 0)
    //         systematicErrors[i].at(bin) = nearDetSystematicError;
    //       else{
    //         systematicErrors[i].at(bin) = 1/sqrt(eventsnueVec[0].at(bin));
    //         if (inflateSystematics) 
    //           systematicErrors[i].at(bin) = sqrt(pow(systematicInflationAmount,2)
    //                                             +pow(systematicErrors[i].at(bin),2));
    //       }
    //     }
    //     else{
    //       systematicErrors[i].at(bin) = flatSystematicError;
    //     }
    //     if (!useInfiniteStatistics){
    //       statisticalErrors[i].at(bin) =  1/sqrt(eventsnueVec[i].at(bin));
    //       if (inflateSystematics) 
    //         systematicErrors[i].at(bin) = sqrt(pow(systematicInflationAmount,2)
    //                                          + pow(systematicErrors[i].at(bin),2));
    //     }
    //     else
    //       if (i == nL-1)
    //         statisticalErrors[i].at(bin) = 0.0;
    //       else {
    //         statisticalErrors[i].at(bin) =  1/sqrt(eventsnueVec[i].at(bin));
    //         if (inflateSystematics) 
    //           systematicErrors[i].at(bin) = sqrt(pow(systematicInflationAmount,2)
    //                                            + pow(systematicErrors[i].at(bin),2));          }

    //   }
    // }

    fittingSignal.resize(nL);
    fittingBackgr.resize(nL);
    fittingErrors.resize(nL);


    //This loop is actually doing the chisq calculation
    //It loops over npoints twice.  First over dm2, and second over sin22th
    for (int dm2point = 0; dm2point <= npoints; dm2point ++){
      std::cout << "dm^2: " << dm2point << " of " << npoints << std::endl;      
      for (int sin22thpoint = 0; sin22thpoint <= npoints; sin22thpoint++){
        chisq = 0.0; //reset the chisq!
        systematicErrors.clear();
        statisticalErrors.clear();
        systematicErrors.resize(nL);
        statisticalErrors.resize(nL);

        //First, figure out what dm2 and sin22th are for this iteration:
        dm2 = pow(10.,(TMath::Log10(dm2min)+(dm2point*1./npoints)*TMath::Log10(dm2max/dm2min)));
        sin22th = pow(10.,(TMath::Log10(sin22thmin)+(sin22thpoint*1./npoints)*TMath::Log10(sin22thmax/sin22thmin)));
        if (debug) std::cout << "---------------------------Begin dm2: " << dm2 << ",\tsin22th: " << sin22th << std::endl;

        //Now build the covariance matrix for this point on (dm2, sin22th)
        //The current matrix is just the fractional systematics.
        //This means scaling each entry by the central values
        //And also means adding statistical errors on the diagonal.
        //If cov_max_name.size() is zero, the use either flat systematics or nearDetStats
        

        
        // In the case of the shape only fit,
        // Copy the unscaled vector that will be used to build error matrix
        // Probably need to scale it as well
        // std::vector< float > eventsnLVecTemp = eventsnLVec[dm2point];

        // Set up the predictive vectors:

        //Collapsed spectrum(dm2,s22th)
        //PredictionVec holds background + signal
        std::vector<float> predictionVec(nbins-nL*nbinsE,0);
        // Null vec holds the rates with no oscillation signal
        // But in the shape only analysis, we normalize this to account
        // for high dm2 signal affecting the "null" background
        nullVec = utils.collapseVector(eventsnLnullVec, nbinsE, nL);
        
        //before collapsing the prediction, need to scale the oscillated signal prediction:
        std::vector<float> eventsFitVecTemp = eventsnLfitVec[dm2point]; //don't want to overwrite...
        for (int i = 0; i < nL; i ++){
            std::transform( eventsFitVecTemp.begin() + i*nbinsE*3, //start here
                            eventsFitVecTemp.begin() + i*nbinsE*3 + nbinsE, //end here
                            eventsFitVecTemp.begin() + i*nbinsE*3, //put it here (replace)
                            std::bind2nd(std::multiplies<double>(), sin22th)); //use this unary operation
            //starts at eventsnLfit
        }



        predictionVec = utils.collapseVector(eventsFitVecTemp, nbinsE, nL);
        // Now predictionVec and nullVec hold the right stuff, uncorrected
        // for high dm2
        
        // Here's where the shape only stuff comes into play
        double intSignalND = 0.;    double intNooscND = 0.;
        std::vector<double> scalefac;
        if(shapeOnlyFit){
          // Loop over the near det and determine the ratio between 
          // background and background + signal
          for(int i = 0; i < nbinsE; i ++){
              intSignalND = predictionVec[i];
              intNooscND  = nullVec[i];
              scalefac.push_back(intSignalND / intNooscND);
          }
          // if (verbose) for (auto & point : scalefac) std::cout << " Scalefac: " << point << "\n";
          
          // next scale the nue background for each det
          for(int n = 0; n < nL; n++)
          {
            for (int i = 0; i < nbinsE; ++ i)
            {
                if (debug) std::cout << "\nScaled from: " << nullVec[2*n*nbinsE + i] << std::endl;
                nullVec[2*n*nbinsE + i] *= scalefac[i];
                eventsFitVecTemp[nbinsE + n*nbinsE*3 + i] *= scalefac[i];
                if (debug) std::cout << "Scaled to: " << nullVec[2*n*nbinsE + i] << std::endl;
            }
          }
        }

        // At this point, prediction vec is the 2*nbinsE*nL long vector that holds
        // the null+signal
        // nullVec is the (corrected, if necessary) background
        // That means that the central values need to be read out from nullVec
        // It also means that the nearDetStats need to come from nullVec
        // Therefore, dig those out:
        // if (shapeOnlyFit)
        // {
        //   for (int i = 0; i < nbinsE*2; ++i)
        //   {
        //       nearDetStats[i+nbinsE] = 1/sqrt( nullVec.at(i+nbinsE) );
        //   }
        // }
        // if (shapeOnlyFit && inflateSystematics){
        //     for (int i = 0; i < nbinsE*2; ++i)
        //     {
        //         nearDetStats[i+nbinsE] = sqrt(pow(systematicInflationAmount,2)
        //                                     + pow(nearDetStats[i+nbinsE],2));
        //     }
        // }        

        // Now make sure the statistical and systematic errors are correct:
        systematicErrors.clear();
        statisticalErrors.clear();
        systematicErrors.resize(nL);
        statisticalErrors.resize(nL);       

        for (int b_line = 0; b_line < nL; b_line++){
          systematicErrors.at(b_line).clear();
          statisticalErrors.at(b_line).clear();
          systematicErrors.at(b_line).resize(nbinsE);
          statisticalErrors.at(b_line).resize(nbinsE);
          for ( int i = 0; i < nbinsE; i++){
            if (b_line == 0){
              statisticalErrors[b_line][i] = 1/sqrt(nullVec.at(i));
              systematicErrors[b_line][i] = nearDetSystematicError;
            } 
            else{
              if (useNearDetStats){
                statisticalErrors[b_line][i] = 1/sqrt(nullVec.at(i+b_line*nbinsE*2));
                systematicErrors[b_line][i] = 1/sqrt(nullVec.at(i));
              }
              else{ // no near det stats
                statisticalErrors[b_line][i] = 1/sqrt(nullVec.at(i+b_line*nbinsE*2));
                systematicErrors[b_line][i] = flatSystematicError;
              }
              if (inflateSystematics){
                // std::cout << "Testing inflateSystematics, before: " << statisticalErrors[b_line][i];
                systematicErrors[b_line][i] = sqrt(pow(systematicErrors[b_line][i],2)
                                                 + pow(systematicInflationAmount,2));
                // std::cout << "After: " << statisticalErrors[b_line][i] << std::endl;
              }

              if (useInfiniteStatistics && b_line == nL - 1){
                // zero out the statistical error:
                statisticalErrors[b_line][i] = 0;
              }

            } // else on if (b_line == 0)          
          }
        }

        if (dm2point == dm2FittingPoint && sin22thpoint == sin22thFittingPoint){
          statisticalErrorsPlotting.resize(nL);
          systematicErrorsPlotting.resize(nL);
          for (int i = 0; i < nL; i++){
            statisticalErrorsPlotting.at(i).resize(nbinsE);
            systematicErrorsPlotting.at(i).resize(nbinsE);
            for (int bin = 0; bin < nbinsE; ++ bin)
            {
              fittingSignal[i].push_back(predictionVec.at(bin + 2*i*nbinsE));
              fittingBackgr[i].push_back(nullVec.at(bin + 2*i*nbinsE));
              double error = 0;
              error += pow(nullVec.at(bin + 2*i*nbinsE)*statisticalErrors[i][bin],2);
              error += pow(nullVec.at(bin + 2*i*nbinsE)*systematicErrors[i][bin],2);
              fittingErrors[i].push_back(sqrt(error));
              statisticalErrorsPlotting[i][bin] = statisticalErrors[i][bin];
              systematicErrorsPlotting[i][bin]  = systematicErrors[i][bin];
            }
          }
        }
        //initialize the covariance matrix array:
        std::vector< std::vector<float> > entries( nbins, std::vector<float> ( nbins, 0.0 ) );


        
        if (!useCovarianceMatrix){

          for (int b_line = 0; b_line < nL; b_line ++){
            for (int ibin = 0; ibin < 2*nbinsE; ibin++){ // looping over the C.M.
              // Really, this just goes down the diagonal since there's no covariance
              // We don't use fractional errors here but full errors
              // So, pick the central value (nullVec)
              int thisBin = 3*nbinsE*b_line + ibin + nbinsE ; // + nbinsE is to skip signal sections
              if (ibin < nbinsE)
                entries[thisBin][thisBin] 
                    = pow(nullVec[2*nbinsE*b_line+ibin]*statisticalErrors[b_line][ibin],2)
                    + pow(nullVec[2*nbinsE*b_line+ibin]*systematicErrors[b_line][ibin] ,2);
              else
                entries[thisBin][thisBin] = nullVec[2*nbinsE*b_line+ibin];

              // std::cout << "On thisBin = " << thisBin << ", added " 
              //           << entries[thisBin][thisBin] << ", nullVec is "
              //           << nullVec[2*nbinsE*b_line+ibin]
              //           << std::endl;

            }
          }
        } //end if on "cov_max_name.size() == 0"      


	// Joseph Joseph Joseph Joseph Joseph Joseph

        else{
          //use the full blown covariance matrix.  Loop over every entry.
          //
          //But first, call the method to build the covariance matrix!
          
          // if (dm2point == 0 && sin22thpoint == 0) BuildCovarianceMatrix();
          if (useSignalCovarianceMatrix)
            BuildCovarianceMatrix(sin22thpoint, dm2point);
          else if (sin22thpoint == 0 && dm2point == 0)
            BuildCovarianceMatrix();

          //Can't forget to add in the statistical errors on the diagonal!
          for (int ibin = 0; ibin < nbins; ibin++){
            for (int jbin = 0; jbin < nbins; jbin ++){
              double cvi = eventsFitVecTemp[ibin];
              double cvj = eventsFitVecTemp[jbin];
      	      // if ( (ibin%(nbinsE*3)) <nbinsE ) cvi *= sin22th; //scale the oscillated signal
              // if ( (jbin%(nbinsE*3)) <nbinsE ) cvj *= sin22th; //scale the oscillated signal

              //Now scale the entries:
              if (shapeOnlyFit)
                entries[ibin][jbin] = fractional_Shape_covarianceMatrix[ibin][jbin]*cvi*cvj;
              else
                entries[ibin][jbin] = fractionalErrorMatrix[ibin][jbin]*cvi*cvj;
              //add statistical errors on the diagonal:
              if (ibin == jbin){
                entries[ibin][jbin] += cvi; //cvi should equal cvj here
              }
            } //end loop on jbin
          } //end loop on ibin
        
        } //end section that is using full covariance matrix
        
        //At this point, the covariance matrix is built in full in "entries"
        //We need to collapse everything, though.
        
        //for debugging, can print out covariance matrix:
        if (debug){
          for (unsigned int i = 0; i < entries.size(); i ++){
            std::cout << i << "\t";
            for (unsigned int j = 0; j < entries[i].size(); j++){
              std::cout << entries[i][j] << " ";
            }
            std::cout << std::endl;
          }
        }

        Float_t * entriescollapsed = (utils.CollapseMatrix(entries,nbinsE, nL));
        //collapsed!
        //if debug is on, print out the collapsed matrix:
        if (debug){
          //the matrix should be nbins- nL*nbinsE on each side.  Print:
          std::cout << "\nPrinting collapsed covariance matrix:\n";
          unsigned int side = nbins-nL*nbinsE;
          for (unsigned int i = 0; i < side; i ++){
            std::cout << i << "\t";
            for (unsigned int j = 0; j < side; j++){
              std::cout << entriescollapsed[i*side + j] << " ";
            }
            std::cout << std::endl;
          }
        }
        



        //Now create the inverse covariance matrix.  Root can invert for us:
        TMatrix *M = new TMatrix(nbins-nL*nbinsE,nbins-nL*nbinsE,entriescollapsed);
        // if (dm2point == npoints && sin22thpoint == npoints){
        //   TCanvas * collapsedCanvas = new TCanvas("cc","cc",700,700);
        //   M->Draw("surf");
        //   TCanvas * uncollapsedCanvas = new TCanvas("cc2","cc2",700,700);
        //   covarianceMatrix.Draw("surf");
        // }
        //inverse cov matrix, M^{-1}, used in chi2 calculation
        TMatrix *cov = new TMatrix(nbins-nL*nbinsE,nbins-nL*nbinsE);
        cov = &(M->Invert()); //this is used in chi2 calculation


        //Checking collapsing:
        if (debug){
          std::cout << "Pre\t\tPost\n";
          for (unsigned int i = 0; i < eventsnLfitVec[dm2point].size(); i++){
            std::cout << eventsnLfitVec[dm2point][i] << "\t\t";
            if (i < predictionVec.size())  std::cout << predictionVec[i];
            std::cout << "\n";
          }
        }

        //This is the actual chi2 calculation
        //Energy spectrum fit
        //loop over collapsed bins
        for (int ibin=1; ibin<=nbins-nL*nbinsE; ibin++)
        {
            //loop over collapsed bins
            for (int jbin=1; jbin<=nbins-nL*nbinsE; jbin++)
            {

                float cvi = nullVec[ibin-1];
                float cvj = nullVec[jbin-1];

                // The prediction doesn't change
                float predictioni = predictionVec[ibin-1];
                float predictionj = predictionVec[jbin-1];
                
                if (debug){
                    if (ibin == jbin){
                        std::cout << "ibin: "<< ibin << " Prediction: " << predictioni << " cvi: " << cvi;
                        std::cout << " M^-1: " << (*cov)(ibin-1,jbin-1);
                        std::cout << " chi2: " << (predictioni-cvi)*(predictionj-cvj)* (*cov)(ibin-1,jbin-1) << std::endl;
                    }
                }
                if (sin22thpoint == sin22thFittingPoint && dm2point == dm2FittingPoint){
                  if (ibin == jbin){
                    std::cout << "On bin " << ibin << ", adding chi2 = " << (predictioni-cvi)*(predictionj-cvj)* (*cov)(ibin-1,jbin-1) << std::endl;
                    if (useNearDetStats) std::cout << "  nearDetStats error on this bin is "<< nearDetStats[ibin%(2*nbinsE)] << std::endl;
                    std::cout << "  ibin: "<< ibin << " Prediction: " << predictioni << " cvi: " << cvi;
                    std::cout << "  M^-1: " << (*cov)(ibin-1,jbin-1) << std::endl;
                  }
                }
                chisq += (predictioni-cvi)*(predictionj-cvj)* (*cov)(ibin-1,jbin-1);

            }
        }
        
        // return 0;
        //write model in file
        chi2->Fill(chisq,dm2,sin22th);

        if (chisq<=deltachisq5s)
        {
            x5s[dm2point] = sin22th;
            y5s[dm2point] = dm2;
        }
        if (chisq<=deltachisq3s)
        {
            x3s[dm2point] = sin22th;
            y3s[dm2point] = dm2;
        }
        if (chisq<=deltachisq90)
        {
            x90[dm2point] = sin22th;
            y90[dm2point] = dm2;
        }
        // if (chisq<=deltachisq99)
        // {
        //     x99[dm2point] = sin22th;
        //     y99[dm2point] = dm2;
        // }
        
          if (debug) std::cout << "dm2: " << dm2 << ",\tsin22th: " << sin22th << ",\tchisq: " << chisq << std::endl;    
          if (debug) std::cout << "\n\n";
          if (debug) std::cout << "---------------------------End dm2: " << dm2 << ",\tsin22th: " << sin22th << std::endl;
          }// end loop over sin22thpoints
      } // end loop over dm2points
    
    std::cout << "Writing chi2 to " << chi2FileName << std::endl;


    TH1::AddDirectory(kFALSE);
    TGraph *sens90 = new TGraph(npoints+1,x90,y90); 
    TGraph *sens3s = new TGraph(npoints+1,x3s,y3s); 
    TGraph *sens5s = new TGraph(npoints+1,x5s,y5s);

    //Plot Results:
    sens90->SetLineColor(1); sens90->SetLineWidth(2);
    sens3s->SetLineColor(9); sens3s->SetLineWidth(2);
    sens5s->SetLineColor(9); sens5s->SetLineStyle(2); sens5s->SetLineWidth(1);
    //write the results to file:
    TFile *file1 = new TFile(chi2FileName,"RECREATE");
    chi2->Write();
    sens90->Write();
    // sens99->Write();
    sens3s->Write();
    sens5s->Write();
    file1->Close();    
    return 0;
  }

  int NueAppearanceFitter::MakePlots(){
    gStyle->SetOptStat(0000);
    gStyle->SetOptFit(0000);
    gStyle->SetPadBorderMode(0);
    gStyle->SetPadBottomMargin(0.15);
    gStyle->SetPadLeftMargin(0.15);
    gStyle->SetPadRightMargin(0.05);
    gStyle->SetFrameBorderMode(0);
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetPalette(0);
    gStyle->SetCanvasColor(0);
    gStyle->SetPadColor(0);
    
    TGraph *sens90 = new TGraph(npoints+1,x90,y90); 
    TGraph *sens3s = new TGraph(npoints+1,x3s,y3s); 
    TGraph *sens5s = new TGraph(npoints+1,x5s,y5s);

    //Plot Results:
    sens90->SetLineColor(1); sens90->SetLineWidth(2);
    sens3s->SetLineColor(9); sens3s->SetLineWidth(2);
    sens5s->SetLineColor(9); sens5s->SetLineStyle(2); sens5s->SetLineWidth(1);   


    

    //======================================================
    printf("\nDrawing sensitivity curves...\n");

    std::cout<<"Drawing LSND intervals...\n\n";

    TCanvas* c3 = new TCanvas("c3","Sensitivity",700,700);
    // c3->Divide(1,2);
    TPad * pad1 = new TPad("pad1","pad1",0,0.25,0.75,1);
    TPad * pad2 = new TPad("pad2","pad2",0,0,0.75,0.25);
    TPad * pad3 = new TPad("pad3","pad3",0.75,0,1,1);

    pad1 -> Draw();
    pad2 -> Draw();
    pad3 -> Draw();

    pad1 -> cd();
    // pad2 -> cd();

    // pad1->cd(1);
    pad1->SetLogx();
    pad1->SetLogy();

    TH2D* hr1=new TH2D("hr1","hr1",500,sin22thmin,sin22thmax,500,dm2min,dm2max);
    hr1->Reset();
    hr1->SetFillColor(0);
    hr1->SetTitle(";sin^{2}2#theta_{#mue};#Deltam^{2} (eV^{2})");
    hr1->GetXaxis()->SetTitleOffset(1.1);
    hr1->GetYaxis()->SetTitleOffset(1.2);
    hr1->GetXaxis()->SetTitleSize(0.05);
    hr1->GetYaxis()->SetTitleSize(0.05);
    hr1->SetStats(kFALSE);
    hr1->Draw();
    plotUtils.lsnd_plot(pad1);

    // Draw the systematic and statistical errors
 
    std::cout << "systematicErrorsPlotting  ... " << systematicErrorsPlotting [nL-1].at(0)  << std::endl;
    std::cout << "systematicErrorsPlotting  ... " << systematicErrorsPlotting [nL-1].at(1)  << std::endl;
    std::cout << "systematicErrorsPlotting  ... " << systematicErrorsPlotting [nL-1].at(2)  << std::endl;
    std::cout << "systematicErrorsPlotting  ... " << systematicErrorsPlotting [nL-1].at(3)  << std::endl;
    std::cout << "statisticalErrorsPlotting ... " << statisticalErrorsPlotting[nL-1].at(0) << std::endl;
    std::cout << "statisticalErrorsPlotting ... " << statisticalErrorsPlotting[nL-1].at(1) << std::endl;
    std::cout << "statisticalErrorsPlotting ... " << statisticalErrorsPlotting[nL-1].at(2) << std::endl;
    std::cout << "statisticalErrorsPlotting ... " << statisticalErrorsPlotting[nL-1].at(3) << std::endl;
    std::vector<TH1F *> systematicHist;
    std::vector<TH1F *> statisticalHist;

    systematicHist.resize(nL);
    statisticalHist.resize(nL);


    std::cout << "fittingSignal ... " << fittingSignal[nL-1].at(0)  << std::endl;
    std::cout << "fittingSignal ... " << fittingSignal[nL-1].at(1)  << std::endl;
    std::cout << "fittingSignal ... " << fittingSignal[nL-1].at(2)  << std::endl;
    std::cout << "fittingSignal ... " << fittingSignal[nL-1].at(3)  << std::endl;
    std::cout << "fittingBackgr ... " << fittingBackgr[nL-1].at(0) << std::endl;
    std::cout << "fittingBackgr ... " << fittingBackgr[nL-1].at(1) << std::endl;
    std::cout << "fittingBackgr ... " << fittingBackgr[nL-1].at(2) << std::endl;
    std::cout << "fittingBackgr ... " << fittingBackgr[nL-1].at(3) << std::endl;
    std::cout << "fittingErrors ... " << fittingErrors[nL-1].at(0) << std::endl;
    std::cout << "fittingErrors ... " << fittingErrors[nL-1].at(1) << std::endl;
    std::cout << "fittingErrors ... " << fittingErrors[nL-1].at(2) << std::endl;
    std::cout << "fittingErrors ... " << fittingErrors[nL-1].at(3) << std::endl;

    std::vector<TH1F *> fittingSignalHist;
    std::vector<TH1F *> fittingBackgrHist;

    fittingSignalHist.resize(nL);
    fittingBackgrHist.resize(nL);
    // for (int i = 0; i < nL; i++){
    //   systematicHist[i] = utils.makeHistogram(systematicErrors[i],0.2,3.0);
    // }

    // TH1F * dummy = new TH1F("dummy", "dummy", nbinsE,0.2, 3.0);



    // for (int i = 0; i < nL; i++){
    //   dummy -> GetXaxis()->SetBinLabel(i*nbinsE + 1, "0.2");
    //   dummy -> GetXaxis()->SetBinLabel(i*nbinsE + 10, "3.0");
    // }

    // dummy->SetLabelSize(0.2);
    // dummy->SetTitle("Fractional Errors");
    // pad2->cd();
    // dummy-> Draw();
    for (int i = 0; i < nL; i++){
      pad2 -> cd();
      char name[100];
      sprintf(name,"padtemp%d",i);
      TPad * padtemp = new TPad(name,name,i*(1.0/nL)+0.01,0.05,(i+1)*(1.0/nL)-0.01,1);
      std::cout << "1/nL is " << 1.0/nL << std::endl;
      padtemp->Draw();
      padtemp -> cd();
      systematicHist[i]  = utils.makeHistogram(systematicErrorsPlotting[i],0.2,3.0);
      statisticalHist[i] = utils.makeHistogram(statisticalErrorsPlotting[i],0.2,3.0);
      systematicHist[i] -> SetTitle(Form("%s Fractional Errors",names[i].c_str()));
      systematicHist[i] -> SetTitleSize(12);
      // systematicHist[i] -> GetYaxis()->SetTitle("");
      if (i == 0){
        systematicHist[i] -> GetYaxis()->SetLabelSize(0.08);
        systematicHist[i] -> GetYaxis()->SetTitle("Uncert. [\%]");
      }
      else{
        systematicHist[i] -> GetYaxis()->SetLabelSize(0.0);
      }
      systematicHist[i] -> GetYaxis()->SetTitleSize(0.08);
      systematicHist[i] -> GetXaxis()->SetTitleSize(0.08);
      systematicHist[i] -> GetXaxis()->SetLabelSize(0.08);
      systematicHist[i] -> GetXaxis()->SetTitleOffset(0.9);
      systematicHist[i] -> GetXaxis()->SetTitle("Energy (GeV)");
      systematicHist[i] -> SetMaximum(0.4);
      systematicHist[i] -> SetMinimum(0);
      systematicHist[i] -> SetLineColor(2);
      statisticalHist[i]-> SetLineColor(1);
      systematicHist[i] -> SetLineWidth(2);
      statisticalHist[i]-> SetLineWidth(2);      
      systematicHist[i] ->Draw("L");
      statisticalHist[i] ->Draw("L same");

    }

     for (int i = 0; i < nL; i++){
      pad3 -> cd();
      char name[100];
      sprintf(name,"pad3temp%d",i);
      TPad * padtemp = new TPad(name,name,0,(i)*(1.0/nL),1,(i+1)*(1.0/nL));
      std::cout << "1/nL is " << 1.0/nL << std::endl;
      padtemp->Draw();
      padtemp -> cd();
      fittingSignalHist[i]  = utils.makeHistogram(fittingSignal[i],0.2,3.0);
      fittingBackgrHist[i] = utils.makeHistogram(fittingBackgr[i],0.2,3.0);
      fittingSignalHist[i] -> SetTitle(Form("%s Signal and Background",names[i].c_str()));
      fittingSignalHist[i] -> SetTitleSize(12);
      // fittingSignalHist[i] -> GetYaxis()->SetTitle("");
      if (i == 0){
        // fittingSignalHist[i] -> GetYaxis()->SetLabelSize(0.08);
        // fittingSignalHist[i] -> GetYaxis()->SetTitle("Uncert. [\%]");
      }
      else{
        // fittingSignalHist[i] -> GetYaxis()->SetLabelSize(0.0);
      }
      fittingSignalHist[i] -> GetYaxis()->SetTitle("Events");
      // fittingSignalHist[i] -> GetXaxis()->SetTitleSize(0.08);
      // fittingSignalHist[i] -> GetXaxis()->SetLabelSize(0.08);
      // fittingSignalHist[i] -> GetXaxis()->SetTitleOffset(0.9);
      fittingSignalHist[i] -> GetXaxis()->SetTitle("Energy (GeV)");
      // fittingSignalHist[i] -> SetMaximum(0.4);
      // fittingSignalHist[i] -> SetMinimum(0);
      fittingSignalHist[i] -> SetLineColor(2);
      fittingBackgrHist[i] -> SetLineColor(1);
      fittingSignalHist[i] -> SetLineWidth(2);
      fittingBackgrHist[i] -> SetLineWidth(2);
      for (int bin = 0; bin < nbinsE; bin++)
        fittingBackgrHist[i]->SetBinError(bin+1,fittingErrors[i][bin]); 
      fittingSignalHist[i] -> Draw("hist");
      fittingBackgrHist[i] -> Draw("E hist same");
      if (i == nL-1){
        TLegend * legSig = new TLegend(0.5,0.7,0.9,0.85);
        legSig->SetFillColor(0);
        legSig->SetFillStyle(0);
        legSig->SetBorderSize(0);
        legSig->AddEntry(fittingBackgrHist[i],"Background");
        legSig->AddEntry(fittingSignalHist[i],"Signal");
        dm2 = pow(10.,(TMath::Log10(dm2min)+(dm2FittingPoint*1./npoints)*
                       TMath::Log10(dm2max/dm2min)));
        sin22th = pow(10.,(TMath::Log10(sin22thmin)+(sin22thFittingPoint*1./npoints)*
                           TMath::Log10(sin22thmax/sin22thmin)));        
        plotUtils.add_plot_label(Form("(%.2geV^{2},%.1g)",dm2,sin22th),0.7,0.6);
        legSig->Draw();
        // fittingSignalHist[i] -> GetYaxis()->SetTitle("Uncert. [\%]");
      }

    }   

    // systematicHist->Draw("same");
    // statisticalHist->Draw("same");

    pad1->cd();

    //======================================================

    TLegend* legt=new TLegend(0.68,0.50,0.86,0.62);
    legt->SetFillStyle(0);
    legt->SetFillColor(0);
    legt->SetBorderSize(0);
    legt->SetTextSize(0.025);
    //     legt->AddEntry("",running,"");
    //     legt->AddEntry("",datapot,"");
    //     
    //Determine the lines in the legend.  Print the detectors first:
    // TString detectors.
    // plotUtils.add_plot_label()
    //if (mode == "nu") legt ->AddEntry("", "#nu mode", "");
    //else if (mode == "nubar") legt ->AddEntry("", "#nubar mode", "");
    
    // char name[200];
    // char name2[200];
    // char name3[200];

    char name[3][200];

    for (int j=0; j<nL; j++) {
        if (baselines[j] == "100m" || baselines[j] == "150m" || baselines[j] == "200m"){
            if (use100m || use150m || use200m)
                sprintf(name[j], "LAr1-ND (%s)", baselinesFancy[j].c_str());
            else if (use100mLong)
                sprintf(name[j], "2*LAr1-ND (%s)", baselinesFancy[j].c_str());
        }
        else if (baselines[j] == "470m"){
            sprintf(name[j], "MicroBooNE (%s)", baselinesFancy[j].c_str());
        }
        else if (baselines[j] == "700m"){
            if (use700m) sprintf(name[j], "LAr1-FD (%s)", baselinesFancy[j].c_str());
        }
        else if (baselines[j] == "600m_onaxis" || baselines[j] == "600m_offaxis"){
            sprintf(name[j], "T600 (%s)", baselinesFancy[j].c_str());
        }
        // else if (baselines[j] == 800){
        //     sprintf(name[j], "T600 (%sm)", baselines[j].c_str());
        // }
    }


    //legt->AddEntry("",name,"");
    
    for (int i = 0; i < nL;i++){
        std::cout << "Name is " << name[i] << std::endl;
        plotUtils.add_plot_label(name[i],  0.77, 0.87-i*0.04, 0.026, 1, 62);
    }  

    plotUtils.add_plot_label(label2, 0.77, 0.76, 0.023, 1, 62);
    plotUtils.add_plot_label(label1, 0.77, 0.73, 0.023, 1, 62);
    // legt->AddEntry("",label,"");

    sens90->Draw("l same");
    legt->AddEntry(sens90,"90\% CL","l");
    // sens99->Draw("l same");
    // legt->AddEntry(sens99,"99\% CL","l");
    sens3s->Draw("l same");
    legt->AddEntry(sens3s,"3#sigma CL","l");
    sens5s->Draw("l same");
    legt->AddEntry(sens5s,"5#sigma CL","l");

    legt->Draw();

    // if (specialNameText != ""){
    //     char tempChar[20];
    //     sprintf(tempChar, "%s",specialNameText.c_str());
    //     plotUtils.add_plot_label(tempChar, 0.75, 0.25);
    // }
    // if (specialNameTextOsc != "") {
    //     char tempChar[20];
    //     sprintf(tempChar, "%s",specialNameTextOsc.c_str());
    //     plotUtils.add_plot_label(tempChar, 0.75, 0.3);
    // }
    //plotUtils.add_plot_label(label, label_x, label_y, 0.05, 1, 62, 22 );

    TLegend* leg3=new TLegend(0.2,0.2,0.4,0.35);
    leg3->SetFillStyle(0);
    leg3->SetFillColor(0);
    leg3->SetBorderSize(0);
    leg3->SetTextSize(0.03);
    TGraph *gdummy1 = new TGraph();
    gdummy1->SetFillColor(29);
    TGraph *gdummy2 = new TGraph();
    gdummy2->SetFillColor(38);
    TMarker *gdummy3 = new TMarker();
    gdummy3 -> SetMarkerStyle(3);
    gdummy3 -> SetMarkerColor(1);
    TMarker *gdummy4 = new TMarker();
    gdummy4 -> SetMarkerStyle(34);
    gdummy4 -> SetMarkerColor(1);
    TGraph *gdummy0 = new TGraph();
    gdummy0 -> SetFillColor(1);
    gdummy0 -> SetFillStyle(3254);
    leg3->AddEntry(gdummy2,"LSND 90% CL","F");
    leg3->AddEntry(gdummy1,"LSND 99% CL","F");
    leg3->AddEntry(gdummy3,"LSND Best Fit","P*");
    leg3->AddEntry(gdummy4,"Global Best Fit","P*");
    leg3->AddEntry(gdummy0,"Global Fit 90% CL (arXiv:1303.3011)");   
    leg3->Draw();
    
    TImage *img = TImage::Create();
    img -> FromPad(c3);
    // if (specialNameText_far != "") fileNameRoot = fileNameRoot + specialNameText_far + "_";
    // if (specialNameTextOsc_far != "") fileNameRoot = fileNameRoot + specialNameTextOsc_far + "_";
    if (savePlots){
      if(shapeOnlyFit){
          if (useNearDetStats){
              c3 -> Print(fileNameRoot+mode+"_nearDetStats_shapeOnly_megaPlot.pdf", "pdf");
              c3 -> Print(fileNameRoot+mode+"_nearDetStats_shapeOnly_megaPlot.eps", "eps");
          }
          else if (useCovarianceMatrix) {
              c3 -> Print(fileNameRoot+mode+"_covMat_shapeOnly_megaPlot.pdf", "pdf");
              c3 -> Print(fileNameRoot+mode+"_covMat_shapeOnly_megaPlot.eps", "eps");
          }
          else{
              c3 -> Print(fileNameRoot+mode+"_flatStats_shapeOnly_megaPlot.pdf", "pdf");
              c3 -> Print(fileNameRoot+mode+"_flatStats_shapeOnly_megaPlot.eps", "eps");
          }
      }
      else{
        if (useNearDetStats){
              c3 -> Print(fileNameRoot+mode+"_nearDetStats_megaPlot.pdf", "pdf");
              c3 -> Print(fileNameRoot+mode+"_nearDetStats_megaPlot.eps", "eps");
          }
          else if (useCovarianceMatrix){
              c3 -> Print(fileNameRoot+mode+"_covMat_megaPlot.pdf", "pdf");
              c3 -> Print(fileNameRoot+mode+"_covMat_megaPlot.eps", "eps");
          }
          else {
              c3 -> Print(fileNameRoot+mode+"_flatStats_megaPlot.pdf", "pdf");
              c3 -> Print(fileNameRoot+mode+"_flatStats_megaPlot.eps", "eps");
          }
      }
    }

    TCanvas * tempCanv = new TCanvas("tempCanv","temp canvas",650,650);
    TPad * padTemp = new TPad("padTemp","padTemp",0,0,1,1);
    padTemp->Draw();

    padTemp->cd();

    padTemp->SetLogx();
    padTemp->SetLogy();
    hr1->Draw();
    plotUtils.lsnd_plot(padTemp);
    sens90->Draw("l same");
    // legt->AddEntry(sens90,"90\% CL","l");
    sens3s->Draw("l same");
    // legt->AddEntry(sens3s,"3#sigma CL","l");
    sens5s->Draw("l same");
    // legt->AddEntry(sens5s,"5#sigma CL","l");
    legt->Draw();
    leg3->Draw();
    for (int i = 0; i < nL;i++){
        std::cout << "Name is " << name[i] << std::endl;
        plotUtils.add_plot_label(name[i],  0.77, 0.87-i*0.04, 0.026, 1, 62);
    }  
    plotUtils.add_plot_label((char*)"PRELIMINARY",0.77,0.79,0.023,2,62);
    plotUtils.add_plot_label(label1, 0.77, 0.76, 0.023, 1, 62);
    plotUtils.add_plot_label(label2, 0.77, 0.73, 0.023, 1, 62);
    plotUtils.add_plot_label((char*)"80\% #nu_{e} efficiency", 0.77, 0.70, 0.023, 1, 62);
    plotUtils.add_plot_label((char*)"Statistical and flux uncert. only", 0.77, 0.67, 0.023, 1, 62);



    std::cout<<"\nEnd of routine.\n";

    return 0;
  }

  int NueAppearanceFitter::MakeSimplePlot(){

    gStyle->SetOptStat(0000);
    gStyle->SetOptFit(0000);
    gStyle->SetPadBorderMode(0);
    gStyle->SetPadBottomMargin(0.15);
    gStyle->SetPadLeftMargin(0.15);
    gStyle->SetPadRightMargin(0.05);
    gStyle->SetFrameBorderMode(0);
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetPalette(0);
    gStyle->SetCanvasColor(0);
    gStyle->SetPadColor(0);
    
    TGraph *sens90 = new TGraph(npoints+1,x90,y90); 
    TGraph *sens3s = new TGraph(npoints+1,x3s,y3s); 
    TGraph *sens5s = new TGraph(npoints+1,x5s,y5s);

    //Plot Results:
    sens90->SetLineColor(1); sens90->SetLineWidth(2);
    sens3s->SetLineColor(9); sens3s->SetLineWidth(2);
    sens5s->SetLineColor(9); sens5s->SetLineStyle(2); sens5s->SetLineWidth(1);   



    TH2D* hr1=new TH2D("hr1","hr1",500,sin22thmin,sin22thmax,500,dm2min,dm2max);
    hr1->Reset();
    hr1->SetFillColor(0);
    hr1->SetTitle(";sin^{2}2#theta_{#mue};#Deltam^{2}_{41} (eV^{2})");
    hr1->GetXaxis()->SetTitleOffset(1.1);
    hr1->GetYaxis()->SetTitleOffset(1.2);
    hr1->GetXaxis()->SetTitleSize(0.05);
    hr1->GetYaxis()->SetTitleSize(0.05);
    hr1->GetXaxis()->CenterTitle();
    hr1->GetYaxis()->CenterTitle();
    hr1->SetStats(kFALSE);
    // hr1->Draw();


    TLegend* legt=new TLegend(0.8,0.45,0.93,0.57);
    legt->SetFillStyle(0);
    legt->SetFillColor(0);
    legt->SetBorderSize(0);
    legt->SetTextSize(0.025);

    char name[3][200];

    for (int j=0; j<nL; j++) {
        if (baselines[j] == "100m" || baselines[j] == "150m" || baselines[j] == "200m"){
            if (use100m || use150m || use200m)
                sprintf(name[j], "LAr1-ND (%s)", baselinesFancy[j].c_str());
            else if (use100mLong)
                sprintf(name[j], "2*LAr1-ND (%s)", baselinesFancy[j].c_str());
        }
        else if (baselines[j] == "470m"){
            sprintf(name[j], "MicroBooNE (%s)", baselinesFancy[j].c_str());
        }
        else if (baselines[j] == "700m"){
            if (use700m) sprintf(name[j], "LAr1-FD (%s)", baselinesFancy[j].c_str());
        }
        else if (baselines[j] == "600m_onaxis" || baselines[j] == "600m_offaxis"){
            sprintf(name[j], "T600 (%s)", baselinesFancy[j].c_str());
        }
        // else if (baselines[j] == 800){
        //     sprintf(name[j], "T600 (%sm)", baselines[j].c_str());
        // }
    }



    TLegend * leg3 = plotUtils.getLSNDLegend();
    // leg3->Draw();

    TCanvas * tempCanv = new TCanvas("tempCanv","temp canvas",650,650);
    TPad * padTemp = new TPad("padTemp","padTemp",0,0,1,1);
    padTemp->Draw();

    padTemp->cd();

    padTemp->SetLogx();
    padTemp->SetLogy();
    hr1->Draw();
    plotUtils.lsnd_plot(padTemp);
    sens90->Draw("l same");
    legt->AddEntry(sens90,"90\% CL","l");
    sens3s->Draw("l same");
    legt->AddEntry(sens3s,"3#sigma CL","l");
    sens5s->Draw("l same");
    legt->AddEntry(sens5s,"5#sigma CL","l");
    legt->Draw();
    leg3->Draw();
    // TLegend * FinalLegend()
    for (int i = 0; i < nL;i++){
        std::cout << "Name is " << name[nL-i-1] << std::endl;
        plotUtils.add_plot_label(name[nL-i-1],  0.93, 0.78+i*0.04, 0.025, 1, 62,32);
    }  
    plotUtils.add_plot_label((char*)"PRELIMINARY",0.93,0.72,0.025,kRed-3,62,32);
    plotUtils.add_plot_label(label1, 0.93, 0.69, 0.025, 1, 62,32);
    plotUtils.add_plot_label(label2, 0.93, 0.66, 0.025, 1, 62,32);
    plotUtils.add_plot_label((char*)"80\% #nu_{e} Efficiency", 0.93, 0.63, 0.025, 1, 62,32);
    plotUtils.add_plot_label((char*)"Statistical and Flux Uncert. Only", 0.93, 0.60, 0.025, 1, 62,32);


    if (savePlots){
      if(shapeOnlyFit){
          if (useNearDetStats){
              tempCanv -> Print(fileNameRoot+mode+"_nearDetStats_shapeOnly_megaPlot.pdf", "pdf");
              tempCanv -> Print(fileNameRoot+mode+"_nearDetStats_shapeOnly_megaPlot.eps", "eps");
          }
          else if (useCovarianceMatrix) {
              tempCanv -> Print(fileNameRoot+mode+"_covMat_shapeOnly_megaPlot.pdf", "pdf");
              tempCanv -> Print(fileNameRoot+mode+"_covMat_shapeOnly_megaPlot.eps", "eps");
          }
          else{
              tempCanv -> Print(fileNameRoot+mode+"_flatStats_shapeOnly_megaPlot.pdf", "pdf");
              tempCanv -> Print(fileNameRoot+mode+"_flatStats_shapeOnly_megaPlot.eps", "eps");
          }
      }
      else{
        if (useNearDetStats){
              tempCanv -> Print(fileNameRoot+mode+"_nearDetStats_megaPlot.pdf", "pdf");
              tempCanv -> Print(fileNameRoot+mode+"_nearDetStats_megaPlot.eps", "eps");
          }
          else if (useCovarianceMatrix){
              tempCanv -> Print(fileNameRoot+mode+"_covMat_megaPlot.pdf", "pdf");
              tempCanv -> Print(fileNameRoot+mode+"_covMat_megaPlot.eps", "eps");
          }
          else {
              tempCanv -> Print(fileNameRoot+mode+"_flatStats_megaPlot.pdf", "pdf");
              tempCanv -> Print(fileNameRoot+mode+"_flatStats_megaPlot.eps", "eps");
          }
      }
    }


   return 0;
  }


  int NueAppearanceFitter::MakeEventRatePlots(){

    //Stuff for root, drawing options and such.
    gStyle->SetOptStat(0000);
    gStyle->SetOptFit(0000);
    gStyle->SetPadBorderMode(0);
    gStyle->SetPadBottomMargin(0.15);
    gStyle->SetPadLeftMargin(0.15);
    gStyle->SetPadRightMargin(0.05);
    gStyle->SetFrameBorderMode(0);
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetPalette(0);
    gStyle->SetCanvasColor(0);
    gStyle->SetPadColor(0);

    TLegend *leg;
    
    std::vector< TCanvas * > stackedCanvas;
    stackedCanvas.resize(nL);
    for (int j = 0; j <nL; j++){
      // root makes me want to be eaten by a grue,  sometimes.
      // why do i have to do this stupid naming shit to keep track of my histograms?
      if (j == 0)     stackedCanvas[j] = new TCanvas("sc0", "Stacked event rates", 700, 500);
      else if (j == 1)  stackedCanvas[j] = new TCanvas("sc1", "Stacked event rates", 700, 500);
      else if (j == 2)  stackedCanvas[j] = new TCanvas("sc2", "Stacked event rates", 700, 500);
      else return -1;
      
      TString nueTitle = "Nue Events at L = ";
      nueTitle += baselines[j];
      THStack * stack = new THStack("nue",nueTitle);
      //add in all the plots    
      

      // std::cout << " NueFromNueCC_muonVec[j].front() is " << NueFromNueCC_muonVec[j].front() << std::endl;
      // std::cout << " NueFromNueCC_chargeKaonVec[j].front() is " << NueFromNueCC_chargeKaonVec[j].front() << std::endl;
      // std::cout << " NueFromNueCC_neutKaonVec[j].front() is " << NueFromNueCC_neutKaonVec[j].front() << std::endl;
      // std::cout << " NueFromEScatterVec[j].front() is " << NueFromEScatterVec[j].front() << std::endl;
      // std::cout << " NueFromNC_pi0Vec[j].front() is " << NueFromNC_pi0Vec[j].front() << std::endl;
      // std::cout << " NueFromNC_delta0Vec[j].front() is " << NueFromNC_delta0Vec[j].front() << std::endl;
      // std::cout << " NueFromNumuCCVec[j].front() is " << NueFromNumuCCVec[j].front() << std::endl;
      // std::cout << " DirtVec[j].front() is " << DirtVec[j].front() << std::endl;
      // std::cout << " OtherVec[j].front() is " << OtherVec[j].front() << std::endl;

      
      TH1F * NueFromNueCC_muon = utils.makeHistogram(NueFromNueCC_muonVec[j], emin, emax);
      TH1F * NueFromNueCC_chargeKaon = utils.makeHistogram(NueFromNueCC_chargeKaonVec[j], emin, emax);
      TH1F * NueFromNueCC_neutKaon = utils.makeHistogram(NueFromNueCC_neutKaonVec[j], emin, emax);
      TH1F * NueFromEScatter = utils.makeHistogram(NueFromEScatterVec[j], emin, emax);
      TH1F * NueFromNC_pi0 = utils.makeHistogram(NueFromNC_pi0Vec[j], emin, emax);
      TH1F * NueFromNC_delta0 = utils.makeHistogram(NueFromNC_delta0Vec[j], emin, emax);
      TH1F * NueFromNumuCC = utils.makeHistogram(NueFromNumuCCVec[j], emin, emax);
      TH1F * Dirt = utils.makeHistogram(DirtVec[j], emin, emax);
      TH1F * Other = utils.makeHistogram(OtherVec[j], emin, emax);


      TH1F * SignalNu = utils.makeHistogram(eventsSignalBestFitNuVec[j], emin, emax);
      TH1F * SignalNubar = utils.makeHistogram(eventsSignalBestFitNubarVec[j], emin, emax);

      SignalNu -> Add(SignalNubar);


      NueFromNueCC_muon       -> SetFillColor(kGreen+3);
      NueFromNueCC_chargeKaon -> SetFillColor(kGreen+2);
      NueFromNueCC_neutKaon   -> SetFillColor(kGreen-6);
      NueFromEScatter         -> SetFillColor(kOrange-8);
      // NueFromNC_pi0     -> SetFillColor(kRed-6);
      NueFromNC_pi0           -> SetFillColor(kOrange-8);
      NueFromNC_delta0        -> SetFillStyle(kBlack);
      NueFromNumuCC           -> SetFillColor(kBlue-9);
      Dirt                    -> SetFillColor(15);
      Other                   -> SetFillColor(8);

      std::vector< float > totalEvents, signalEvents;
      totalEvents.resize(nbins);
      signalEvents.resize(nbins);


      // Set the bin errors to zero except for the very last bin:
      for (int i = 0; i < nbinsE; ++i)
      {
        totalEvents[i] = 0;
        NueFromNueCC_muon   -> SetBinError(i+1, 0.0);
        totalEvents[i]      += NueFromNueCC_muon -> GetBinContent(i+1);
        NueFromNueCC_chargeKaon -> SetBinError(i+1, 0.0);
        totalEvents[i]      += NueFromNueCC_chargeKaon -> GetBinContent(i+1);
        NueFromNueCC_neutKaon -> SetBinError(i+1, 0.0);
        totalEvents[i]      += NueFromNueCC_neutKaon -> GetBinContent(i+1);
        NueFromEScatter     -> SetBinError(i+1, 0.0);
        totalEvents[i]      += NueFromEScatter -> GetBinContent(i+1);
        NueFromNC_pi0       -> SetBinError(i+1, 0.0);
        totalEvents[i]      += NueFromNC_pi0 -> GetBinContent(i+1);
        NueFromNC_delta0    -> SetBinError(i+1, 0.0);
        totalEvents[i]      += NueFromNC_delta0 -> GetBinContent(i+1);
        NueFromNumuCC       -> SetBinError(i+1, 0.0);
        totalEvents[i]      += NueFromNumuCC -> GetBinContent(i+1);
        Dirt                -> SetBinError(i+1, 0.0);
        totalEvents[i]      += Dirt -> GetBinContent(i+1);
        Other               -> SetBinError(i+1, 0.0);
        totalEvents[i]      += Other -> GetBinContent(i+1);
        signalEvents[i]     = SignalNu -> GetBinContent(i+1);
      }
      
      
      //Now set the errors on the last hist going in:
      for (int i = 0; i < nbinsE; ++i)
      {
        NueFromNumuCC -> SetBinError(i+1, sqrt(totalEvents[i]));
        SignalNu->SetBinContent(i+1, totalEvents[i]+SignalNu->GetBinContent(i+1));
        SignalNu->SetBinError(i+1, sqrt(signalEvents[i]));
      }
      
      
      stack -> Add(NueFromNueCC_muon);
      stack -> Add(NueFromNueCC_chargeKaon);
      stack -> Add(NueFromNueCC_neutKaon);
      // stack -> Add(NueFromEScatter);
      stack -> Add(NueFromNC_pi0);
      // stack -> Add(NueFromNC_delta0);
      stack -> Add(NueFromNumuCC);
      // stack -> Add(Dirt);
      // stack -> Add(Other);
      // stack ->Add(MBPhotExcess);

      SignalNu -> SetLineStyle(0);
      SignalNu -> SetLineColor(1);
      SignalNu -> SetLineWidth(2);

      double integral = 0;
      integral += NueFromNueCC_muon->Integral();
      integral += NueFromNueCC_chargeKaon->Integral();
      integral += NueFromNueCC_neutKaon->Integral();
      integral += NueFromNC_pi0->Integral();
      // integral += NueFromNumuCC->Integral();

      std::cout << "Total number of events in the background at L = " 
                << baselines[j] << ": " << integral << std::endl;

      // stack -> Add(SignalNu);

      //set up the legend
      leg = new TLegend(0.75,0.5,.95,0.9);
      leg -> SetTextFont(72);
      leg->AddEntry(NueFromNueCC_muon, "#mu #rightarrow #nu_{e}");
      leg->AddEntry(NueFromNueCC_chargeKaon, "K^{+} #rightarrow #nu_{e}");
      leg->AddEntry(NueFromNueCC_neutKaon, "K^{0} #rightarrow #nu_{e}");
      // leg->AddEntry(NueFromEScatter, "#nu - e^{-}");
      leg->AddEntry(NueFromNC_pi0, "NC #pi^{0}");
      // leg->AddEntry(NueFromNC_delta0, "#Delta #rightarrow N#gamma");
      leg->AddEntry(NueFromNumuCC, "#nu_{#mu} CC");
      // leg->AddEntry(Dirt, "Dirt");
      // leg->AddEntry(Other, "Other");
      leg->AddEntry(SignalNu,"Signal");

      leg -> SetTextSize(0.04);
      leg -> SetFillStyle(0);
      leg -> SetFillColor(0);
      leg -> SetBorderSize(0);
      // leg3->SetTextSize(0.03);

//Start here

    // TFile * f = new TFile("microBooNE_Event_Rates.root","RECREATE");
    // NueFromNueCC_muon -> Write();
    // NueFromNueCC_chargeKaon -> Write();
    // NueFromNueCC_neutKaon -> Write();
    // NueFromNC_pi0 -> Write();
    // NueFromNumuCC -> Write();
    // stack -> Write();
    // // stackedCanvas -> Write();
    // f->Close();
//end here
//
      // Get the value to be the maximum of the plot:
      double max = (stack -> GetMaximum());

      std::cout << "Max val: " << max << "\n";

      if (mode == "nubar"){
        if (j == 0) max *= 1.5;
        if (j == 1) max *= 2.5;
        if (j == 2) max *= 2.0;
      }
      if (mode == "nu"){
        if (j == 0) max *= 1.75;
        if (j == 1) max *= 2.0;
        if (j == 2) max *= 2.1;
      }
      std::cout << "Max val (scaled): " << max << "\n";



      TH1F *chr = stackedCanvas[j]->DrawFrame(emin-0.01,
                                  -0.01*(SignalNu->GetMaximum()),emax,1.0*max);
      
      //chr->GetYaxis()->SetLabelOffset(999);

      char name[200];
      if (baselines[j] == "100m" || baselines[j] == "150m" || baselines[j] == "200m"){
        if (use100mLong) sprintf(name, "2*LAr1-ND (%s)", baselinesFancy[j].c_str());
        else sprintf(name, "LAr1-ND (%s)", baselinesFancy[j].c_str());
      }
      else if (baselines[j] == "470m"){
        sprintf(name, "MicroBooNE (%s)", baselinesFancy[j].c_str());
      }
      else if (baselines[j] == "700m"){
        if (use700m) sprintf(name, "LAr1-FD (%s)", baselinesFancy[j].c_str());
        if (useT600_onaxis) sprintf(name, "T600 (%s)", baselinesFancy[j].c_str());
      }
      else if (baselines[j] == "600m_onaxis"){
        sprintf(name, "T600 (%s)", baselinesFancy[j].c_str());
      }
      else if (baselines[j] == "600m_offaxis"){
        sprintf(name, "T600 (%s)", baselinesFancy[j].c_str());
      }
      // else if (baselines[j] == "800m"){
      //   sprintf(name, "T600 (%sm)", baselines[j].c_str());
      // }
      
      chr->GetYaxis()->SetTitle("Events / 280 MeV");
      chr->GetYaxis()->SetTitleSize(0.06);
      chr->GetYaxis()->SetTitleOffset(0.9);
      chr->GetYaxis()->CenterTitle();
      chr->GetXaxis()->CenterTitle();
      chr->GetXaxis()->SetLabelSize(0.05);
      TString forComparison = fileSource;
      if (forComparison == "/Users/cja33/Desktop/lar1/CCQE/"){
        chr->GetXaxis()->SetTitle("E_{#nu}^{QE} (GeV)");
      }
      else chr->GetXaxis()->SetTitle("Energy (GeV)");

      TString EnergyLabel = label2;
      EnergyLabel += " (GeV)";
      chr->GetXaxis()->SetTitle(EnergyLabel);
      chr->GetXaxis()->SetTitleOffset(0.95);
      chr->GetXaxis()->SetTitleSize(0.07);
      chr->GetXaxis()->SetLimits(emin-0.01,emax);
      chr->Draw();
      

      plotUtils.add_plot_label( name , 0.2, 0.87, 0.05, 1,62,12);
      if (useHighDm) 
        plotUtils.add_plot_label( (char*)"Signal: (#Deltam^{2} = 50 eV^{2}, sin^{2}2#theta_{#mue} = 0.003)", 0.2, 0.81, 0.04, 1,62,12);
      else if (useGlobBF)
        plotUtils.add_plot_label( (char*)"Signal: (#Deltam^{2} = 0.43 eV^{2}, sin^{2}2#theta_{#mue} = 0.013)", 0.2, 0.81, 0.04, 1,62,12);
      else  plotUtils.add_plot_label( (char*)"Signal: (#Deltam^{2} = 1.2 eV^{2}, sin^{2}2#theta_{#mue} = 0.003)", 0.2, 0.81, 0.04, 1,62,12);

      plotUtils.add_plot_label((char*)"Statistical Uncertainty Only",0.2,0.76, 0.04,1,62,12);
      plotUtils.add_plot_label((char*)"PRELIMINARY",0.2,0.71, 0.04, kRed-3,62,12);

      stack -> Draw("E0 hist same");
      SignalNu -> Draw("E0 hist same");
      SignalNu -> Draw("E0 same");
      stack -> Draw("E0 hist  same");
      stack -> Draw("E0 same ");
      chr   -> Draw("same");

      // stack -> Draw(" same ");
      leg->Draw();

    }
    


    if (specialNameText != "") fileNameRoot = fileNameRoot + specialNameText + "_";
    if (specialNameTextOsc != "") fileNameRoot = fileNameRoot + specialNameTextOsc + "_";

    for (int i = 0; i < nL; i++)
    {
      char fileName[200];
      if (useHighDm) 
        sprintf(fileName, "%s%s_%s_highdm2.pdf", fileNameRoot.Data(), baselines[i].c_str(), mode.c_str());
      else if (useGlobBF) 
        sprintf(fileName, "%s%s_%s_globBF.pdf", fileNameRoot.Data(), baselines[i].c_str(), mode.c_str());
      else sprintf(fileName, "%s%s_%s.pdf", fileNameRoot.Data(), baselines[i].c_str(), mode.c_str());
      if (savePlots) stackedCanvas[i] -> Print(fileName, "pdf");
      if (useHighDm) 
        sprintf(fileName, "%s%s_%s_highdm2.eps", fileNameRoot.Data(), baselines[i].c_str(), mode.c_str());
      else if (useGlobBF) 
        sprintf(fileName, "%s%s_%s_globBF.eps", fileNameRoot.Data(), baselines[i].c_str(), mode.c_str());
      else sprintf(fileName, "%s%s_%s.eps", fileNameRoot.Data(), baselines[i].c_str(), mode.c_str());
      if (savePlots) stackedCanvas[i] -> Print(fileName, "eps");
      // stackedCanvas[i] -> Print(fileName, "eps");
    }
   

    return 0;

  }

  int NueAppearanceFitter::MakeAltSensPlot(){

    // contains the points at which chi2 is calc'd
    std::vector<float>  dm2Vec;
    std::vector<float>  sin22thVec;

    for (int i = 0; i <= npoints; i ++)
    {
      double dm2 = pow(10.,(TMath::Log10(dm2min)+(i*1./npoints)*TMath::Log10(dm2max/dm2min)));
      double sin22th = pow(10.,(TMath::Log10(sin22thmin)+(i*1./npoints)*TMath::Log10(sin22thmax/sin22thmin)));
      dm2Vec.push_back(dm2);
      sin22thVec.push_back(sin22th);
    }

      // set up the variables to be read in by the ntuple:
    Float_t dm2_temp;
    Float_t sin22th_temp;
    Float_t chi2_temp;

    // TString chi2FileName;

    TFile f(chi2FileName, "READ");

    TNtuple * thisNtuple;
    f.GetObject("chi2", thisNtuple);

    thisNtuple->SetBranchAddress("chisq", &chi2_temp);
    thisNtuple->SetBranchAddress("dm2", &dm2_temp);
    thisNtuple->SetBranchAddress("sin22th", &sin22th_temp);

    // build a vector for this ntuple, then push it back to the main set
    std::vector< std::vector< float> > thisChi2(npoints+1, std::vector<float>(npoints + 1, 0.0));
    int i_entry = 0;
    int i_dm2 = 0;
    int i_sin22th = 0;
    while (i_entry < thisNtuple->GetEntries())
    {
      thisNtuple->GetEntry(i_entry);
      thisChi2[i_dm2][i_sin22th] = chi2_temp;
      i_sin22th ++;
      if (i_sin22th % (npoints+1) == 0){
        i_sin22th = 0;
        i_dm2 ++;
      }
      i_entry ++;
    }
    std::cout << "Finished reading chi2 from: " << std::endl;
    std::cout << "  " << chi2FileName << std::endl;
  
    std::vector<float> lsnd_bottom_edge = plotUtils.Bin_LSND_Data(npoints, dm2Vec, sin22thVec);

    // Now make a plot of chi2 along that line lsnd_bins
    float chi2_at_bottom_edge[npoints+1];
    float dm2_points[npoints+1];

    for (int i = 0; i <= npoints; i++)
    {
      dm2_points[i] = dm2Vec.at(i);
      chi2_at_bottom_edge[i] = sqrt(thisChi2[i][lsnd_bottom_edge.at(i)]);
    }
    TGraph * lsnd_bottom_edge_graph = new TGraph(npoints+1, dm2_points, chi2_at_bottom_edge);

    lsnd_bottom_edge_graph -> GetHistogram()->SetMinimum(0.0);
    // lsnd_bottom_edge_graph -> GetHistogram()->SetMaximum(20);

    TCanvas * canv2 = new TCanvas("c2","Sensitivity along bottom of LSND region",1000,500);
    if (mode == "nu")
      lsnd_bottom_edge_graph -> SetTitle("Sensitivity to 3+1 #nu signal along the LSND edge.");
    else 
      lsnd_bottom_edge_graph -> SetTitle("Sensitivity to 3+1 #bar{#nu} signal along the LSND edge.");
    lsnd_bottom_edge_graph -> GetXaxis() -> SetTitle("#Delta m^{2} (eV^{2})");
    lsnd_bottom_edge_graph -> GetYaxis() -> SetTitle("Sensitivity (#sigma)");
    lsnd_bottom_edge_graph -> GetXaxis() -> SetTitleSize(.06);
    lsnd_bottom_edge_graph -> GetYaxis() -> SetTitleSize(.06);
    lsnd_bottom_edge_graph -> GetXaxis() -> CenterTitle();
    lsnd_bottom_edge_graph -> GetYaxis() -> CenterTitle();


    lsnd_bottom_edge_graph -> GetXaxis() -> SetTitleOffset(0.85);
    lsnd_bottom_edge_graph -> GetYaxis() -> SetTitleOffset(0.35);

    // Drawing attributes:
    lsnd_bottom_edge_graph -> SetLineWidth(2);
    lsnd_bottom_edge_graph -> SetLineColor(4);
    canv2 -> SetGrid();
    canv2 -> SetLogx();

      // sprintf(label,"Detector Mass (ton): %.0f", farDetMassVec[mass_i]);
    lsnd_bottom_edge_graph -> Draw("AC");

    // Make a plot to validate the LSND edge finding?
    if (false){

      // First, make an empty LSND plot:
      TH2D * hist = plotUtils.getEmptySensPlot();
      TCanvas * c = new TCanvas("canv","canv",800,800);
      c->SetLogy();
      c->SetLogx();
      hist->Draw();
      plotUtils.lsnd_plot(c);
      c->Update();

      float xpoints[npoints+1];
      float ypoints[npoints+1];
      for (int i = 0; i <= npoints; i++)
      {
        xpoints[i] = sin22thVec[lsnd_bottom_edge[i]];
        ypoints[i] = dm2Vec[i];
      }

    
      TGraph * line = new TGraph(npoints+1, xpoints, ypoints);
      line -> SetLineColor(4);
      line -> SetLineWidth(2);
      line -> Draw("same C");
    }


    return 0;
  }

}






