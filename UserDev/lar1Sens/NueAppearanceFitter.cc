
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

    ElectContainedDist = -999;
    minDistanceToStart = -999;
    minShowerGap = 10000;
    minVertexEnergyPhoton = 10000;
    minVertexEnergySignal = 0;

    systematicInflationAmount = 0.0;
    inflateSystematics = false;
    
    shapeOnlyFit = false;              // Only matters with near detd stats = true

    // set up the default bins
    for(int i = 0; i <= 14; i++){
      nueBins.push_back(0.2 + i*0.2); // default is 200Mev bins from 200 to 3000
      numuBins.push_back(0.2 + i*0.2); // default is 200Mev bins from 200 to 3000
    }
    nbins_nue = nueBins.size()-1;
    nbins_numu = numuBins.size()-1;
    
    covMatrixList.clear();
    covMatrixListSource.clear();

  }

  int NueAppearanceFitter::Run(){
    // This function configures all the functions and checks that boolean
    // logic works out between all the functions
    // 
    
    // Do all of the checks to make sure the functions will run
    
    if (fLoop) includeFosc = true;
    if ( fBuildCovarianceMatrix || fMakeRatioPlots){
      if (!useMultiWeights){
        std::cout << " WARNING: switch useMultiWeights to true"
                  << " since it's needed in a function you requested.\n";
       useMultiWeights = true;
      }  
    }

    if (fLoop){
      if (fBuildCovarianceMatrix) {
        std::cout << "Problem because covariance matrices need to be built before looping.\n";
        exit(-1);
      }
      useMultiWeights =false;
    }

    Prepare();
    ReadData();
    if (fBuildCovarianceMatrix) {
      if (useFluxWeights && useXSecWeights){
        std::cerr << "ERROR: You have requested to build a covariance"
                  << " matrix but only one uncertainty is available to build"
                  << " at a time.  Please run the uncertainties separately.\n";
        return -1;
      }
      if( !useFluxWeights && !useXSecWeights){
        std::cerr << "ERROR: You have requested to build a covariance"
                  << " matrix but have not selected an uncertainty to use.\n";
        return -1;        
      }
      int returnVal = BuildCovarianceMatrix();
      if (returnVal != 0) return returnVal;
    }
    if (fMakeRatioPlots) {
      int returnVal = MakeRatioPlots();
      if (returnVal != 0) return returnVal;
    }
    if (fLoop) {
      int returnVal = Loop();
      if (returnVal != 0) return returnVal;
    }
    if (fMakeSimplePlot) {
      int returnVal = MakeSimplePlot();
      if (returnVal != 0) return returnVal;
    }
    if (fMakeEventRatePlots) {
      int returnVal = MakeEventRatePlots();
      if (returnVal != 0) return returnVal;
    }
    if (fMakeAltSensPlot) {
      int returnVal = MakeAltSensPlot();
      if (returnVal != 0) return returnVal;
    }

    return 0;
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

    // This variable gets used in writing output to file
    // notice that it goes to the same directory as the input files
    // fileNameRoot = fileSource;
    fileNameRoot = "nue_appearance_";
    fileNameRoot += energyType;
    fileNameRoot += "_";
    fileNameRoot += mode;
    fileNameRoot += "_";

    if (!includeCosmics) cosmicsFile = "";

    emin = nueBins.front();
    emax = nueBins.back();
  
    // this block of if statements is controlling all of the vectors
    // that keep track of which detectors get used, their names,
    // what they are scaled by, etc.
    if (use100m) {
      baselines.push_back("100m");
      baselinesFancy.push_back("100m");
      scales.push_back(LAr1NDScale);
      names.push_back("LAr1-ND");
      detNamesString += "ND_100m_";
    }
    if (use100mLong) {
      baselines.push_back("100m");
      baselinesFancy.push_back("100m");
      scales.push_back(LAr1NDScale);
      names.push_back("2*LAr1-ND");
      detNamesString += "2ND_";
    }
    if (use150m) {
      baselines.push_back("150m");
      baselinesFancy.push_back("150m");
      scales.push_back(LAr1NDScale);
      names.push_back("LAr1-ND");
      detNamesString += "ND_150m_";
    }
    if (use200m) {
      baselines.push_back("200m");
      baselinesFancy.push_back("200m");
      scales.push_back(LAr1NDScale);
      names.push_back("LAr1-ND");
      detNamesString += "ND_200m_";
    }
    if (use470m) {
      baselines.push_back("470m");
      baselinesFancy.push_back("470m");
      scales.push_back(ubooneScale);
      names.push_back("MicroBooNE");
      detNamesString += "uB_";
    }
    if (useT600_onaxis){
      baselines.push_back("600m_onaxis");
      baselinesFancy.push_back("600m, on axis");
      scales.push_back(LAr1FDScale);
      names.push_back("T600");
      detNamesString += "T600_onaxis_";
    }
    if (useT600_offaxis) {
      baselines.push_back("600m_offaxis");
      baselinesFancy.push_back("600m, off axis");
      scales.push_back(LAr1FDScale);
      names.push_back("T600");
      detNamesString += "T600_offaxis_";
    }

    char tempstring[100];
    if (ElectContainedDist != -999) {
      sprintf(tempstring,"cont%g_",ElectContainedDist);
      fileNameRoot += tempstring;
    }
    if (minDistanceToStart != -999) {
      sprintf(tempstring,"dist%g_",minDistanceToStart);
      fileNameRoot += tempstring;
    }
    if (minVertexEnergyPhoton != 10000 ) {
      sprintf(tempstring, "vePhot%g_",minVertexEnergyPhoton );
      fileNameRoot += tempstring;
    }
    if (minShowerGap != 10000 )  { 
      sprintf(tempstring, "gap%g_",minShowerGap );
      fileNameRoot += tempstring;
    }
    if (minVertexEnergySignal != 0 ) {
      sprintf(tempstring, "veSig%g_",minVertexEnergySignal );
      fileNameRoot += tempstring;
    }

    if ( (use100m && use150m) ||
         (use100m && use200m) ||
         (use150m && use200m) ){
      std::cout << "Error, can only pick one near detector location!" << std::endl;
      exit(-1);
    }


    for (auto name : baselines){
      NtupleReader a("nue",fileSource, name, mode, 
                     energyType, npoints, forceRemake);

      a.setContainedShowers(ElectContainedDist);
      a.setMinDistanceToStart(minDistanceToStart);
      a.setTopologyCut(minVertexEnergyPhoton,minShowerGap);
      a.setMinVertexEnergySignal(minVertexEnergySignal);
      a.setSpecialNameText(specialNameText);
      a.setIncludeOsc(includeFosc);
      a.setSpecialNameTextOsc(specialNameTextOsc);
      if (useMultiWeights){
        a.useMultiWeights(useMultiWeights,multiWeightSource);
        a.setNWeights(nWeights);
        if (absolute_MWSource)
          a.setAbsolute_MWSource(absolute_MWSource);
      }
      readerNue.push_back(a);
      if (includeNumus){
        a.setSignal("numu");
        a.setSpecialNameText(specialNameText);
        readerNumu.push_back(a);
      }      
    }



    //This value is the number of baselines:
    nL = baselines.size();
    //total number of energy bins for side-by-side distributions:
    nbins = nbins_nue;
    nbins_null = nbins_nue;
    if (includeFosc) nbins += nbins_nue;
    if (includeNumus) {
      nbins += nbins_numu;
      nbins_null += nbins_numu;
    }
    else {
      nbins_numu = 0;
    }
    //just in case, if the number of baselines is 1 switch off shapeOnlyFit:
    if (nL == 1) {
        shapeOnlyFit = false;
        //flatSystematicError = nearDetSystematicError; 
    }



    //The following section is purely informational, to let the user see what's happening...

    std::cout << "\nBeginning sensitivity analysis...\n";
    std::cout << "Running in " << mode << " mode with the following detectors:\n";
    for (int i = 0; i < nL; i++){
        std::cout << "\t" << names[i] << " at " << baselines[i] << "m with " << 6.6e20*scales[i] << " POT.\n";
    }
    std::cout << "\nLooking for input ntuples in this folder:\n";
    std::cout << "\t" << fileSource << std::endl;
    std::cout << "\nDebug is set to " << debug << std::endl;
    std::cout << "Verbose is set to " << verbose << std::endl;
    std::cout << "\n";
    std::cout << "\n";
    
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
      Int_t returnVal(-1);
      returnVal = readerNue[i].processData();
      if (returnVal) {  //only returns 0 on successful completion
          std::cout << "Error: failed to read the ntuple at " << L << " with error " << returnVal;
          std::cout << std::endl;
          return 1;
      }
      if (includeNumus) returnVal = readerNumu[i].processData();
      if (returnVal) {  //only returns 0 on successful completion
          std::cout << "Error: failed to read the ntuple at " << L << " with error " << returnVal;
          std::cout << std::endl;
          return 1;
      }
    }

    //Make sure the vectors are the right size!
    eventsnueVec.resize(nL);
    if (includeNumus) eventsnumuVec.resize(nL);
    if (includeFosc) eventsnueoscVec.resize(nL);
    if (includeFosc) eventsnuefoscVec.resize(nL);
    if (includeFosc) eventsSignalBestFitNuVec.resize(nL);
    if (includeFosc) eventsSignalBestFitNubarVec.resize(nL);

    //  Event rate background vecs
    NueFromNueCC_muonVec.resize(nL);
    NueFromNueCC_chargeKaonVec.resize(nL);
    NueFromNueCC_neutKaonVec.resize(nL);
    NueFromEScatterVec.resize(nL);
    NueFromNC_pi0Vec.resize(nL);
    NueFromNC_delta0Vec.resize(nL);
    NueFromNumuCCVec.resize(nL);
    eventsNueMCStats.resize(nL);
    if (includeNumus) eventsNumuMCStats.resize(nL);
    DirtVec.resize(nL);
    OtherVec.resize(nL);
    NueFromCosmicsVec.resize(nL);

    if (useMultiWeights){
      eventsNullVecMultiWeight.resize(nWeights);
      for (int N_weight = 0; N_weight < nWeights; ++N_weight)
      {
        eventsNullVecMultiWeight[N_weight].resize(nL*nbins);
      }
    }

      //     Double_t dm2,sin22th;
    Double_t sin22thBF=0.003; //The LSND best fit values of the parameters (dm2 = 1.2)
    Double_t sin22thBF_glob=0.013; // (dm = 0.43)

    if (useGlobBF)
      sin22thBF = sin22thBF_glob;
    if (useHighDm)
      sin22thBF = 0.50;

    //Now actually read in the histograms, which are in files generated by read_ntuple.C and _fosc.C
    //Sure, you could include this loop above, but it is easier logically to read all ntuples 
    //and then import all the results.  And that's what we're going for here.
    //You shouldn't have to change anything in this loop if you don't edit read_ntuple.C (or fosc)
    for(int b_line = 0; b_line < nL; b_line++){
      //For each detector, we read in 3 files of histograms, nuebackground, numubkg, fosc
      // eventsnueoscVec[b_line].resize(npoints+1); // make room for the osc vectors

      eventsnueVec[b_line]     = utils.rebinVector(readerNue[b_line].GetData(),nueBins);
      if (includeNumus)
        eventsnumuVec[b_line]    = utils.rebinVector(readerNumu[b_line].GetData(),numuBins);
      if (includeFosc){
        eventsnuefoscVec[b_line] = utils.rebinVector(readerNue[b_line].GetData(),nueBins);

        // Getting the osc vectors and rebinning them:
        eventsnueoscVec[b_line]  = readerNue[b_line].GetDataOsc();
        for (auto & vec : eventsnueoscVec[b_line]){
          vec = utils.rebinVector(vec, nueBins);
        }
        for (auto & bin : eventsnuefoscVec[b_line]) bin*= 0.003;
      }

      // A lot of the items need to be grabbed a la carte:

      eventsNueMCStats[b_line]           
        = utils.rebinVector(readerNue[b_line].GetVectorFromTree(
                 (char*) "eventsNueMCVec"), nueBins);
      if (includeNumus)
        eventsNumuMCStats[b_line]        
          = utils.rebinVector(readerNumu[b_line].GetVectorFromTree(
                              (char*) "eventsNumuMC"),
                              numuBins);
      NueFromNueCC_muonVec[b_line]       
        = utils.rebinVector(readerNue[b_line].GetVectorFromTree(
                 (char*) "NueFromNueCC_muonVec"),nueBins);
      NueFromNueCC_chargeKaonVec[b_line] 
        = utils.rebinVector(readerNue[b_line].GetVectorFromTree(
                 (char*) "NueFromNueCC_chargeKaonVec"),nueBins);
      NueFromNueCC_neutKaonVec[b_line]   
        = utils.rebinVector(readerNue[b_line].GetVectorFromTree(
                 (char*) "NueFromNueCC_neutKaonVec"),nueBins);
      NueFromEScatterVec[b_line]         
        = utils.rebinVector(readerNue[b_line].GetVectorFromTree(
                 (char*) "NueFromEScatterVec"),nueBins);
      NueFromNC_pi0Vec[b_line]           
        = utils.rebinVector(readerNue[b_line].GetVectorFromTree(
                 (char*) "NueFromNC_pi0Vec"),nueBins);
      NueFromNC_delta0Vec[b_line]        
        = utils.rebinVector(readerNue[b_line].GetVectorFromTree(
                 (char*) "NueFromNC_delta0Vec"),nueBins);
      NueFromNumuCCVec[b_line]           
        = utils.rebinVector(readerNue[b_line].GetVectorFromTree(
                 (char*) "NueFromNumuCCVec"),nueBins);
      DirtVec[b_line]                    
        = utils.rebinVector(readerNue[b_line].GetVectorFromTree(
                 (char*) "DirtVec"),nueBins);
      OtherVec[b_line]                   
        = utils.rebinVector(readerNue[b_line].GetVectorFromTree(
                 (char*) "OtherVec"),nueBins);
      // if (includeCosmics)
      NueFromCosmicsVec[b_line]          
        = utils.rebinVector(readerNue[b_line].GetComptonBackgroundFromFile(cosmicsFile,
                 minDistanceToStart),nueBins);
      if (includeFosc){
        if (useHighDm){
          eventsSignalBestFitNuVec[b_line]    
            = utils.rebinVector(readerNue[b_line].GetVectorFromTree( 
                (char*) "edistrnueHighDmNuVec",
                (char*)"tvecfosc",true),nueBins);
          eventsSignalBestFitNubarVec[b_line] 
            = utils.rebinVector(readerNue[b_line].GetVectorFromTree( 
                (char*) "edistrnueHighDmNubarVec",
                (char*)"tvecfosc",true),nueBins);
        }
        else if (useGlobBF){
          eventsSignalBestFitNuVec[b_line]    
            = utils.rebinVector(readerNue[b_line].GetVectorFromTree( 
                (char*) "edistrnueBestFit_globNuVec",
                (char*)"tvecfosc",true),nueBins);
          eventsSignalBestFitNubarVec[b_line] 
            = utils.rebinVector(readerNue[b_line].GetVectorFromTree( 
                (char*) "edistrnueBestFit_globNubarVec",
                (char*)"tvecfosc",true),nueBins);
        }
        else {
          eventsSignalBestFitNuVec[b_line]    
            = utils.rebinVector(readerNue[b_line].GetVectorFromTree( 
                (char*) "edistrnueBestFitNuVec",
                (char*)"tvecfosc",true),nueBins);
          eventsSignalBestFitNubarVec[b_line] 
            = utils.rebinVector(readerNue[b_line].GetVectorFromTree( 
                (char*) "edistrnueBestFitNubarVec",
                (char*)"tvecfosc",true),nueBins);
        }
      }
      // now deal with the multiweight stuff:
      if (useMultiWeights){
        auto tempMultiWeightInfoNue    = readerNue[b_line].GetMultiWeightData();
        std::vector<std::vector< float> > tempMultiWeightInfoNumu;
        if (includeNumus)
          tempMultiWeightInfoNumu   = readerNumu[b_line].GetMultiWeightData();
        std::vector<float> blankVectorNue(nbins_nue, 0);
        std::vector<float> blankVectorNumu(nbins_numu, 0);

        // append the info into the proper places, 
        // then we'll scale it as we copy it into place.
        std::vector<float> tempVector;
        tempVector.reserve(nbins);
        for (int N_weight = 0; N_weight < nWeights; ++N_weight)
        {
          // this is just attaching nue_osc to nue to numu into the 
          // total, combined vector
          if (includeFosc && includeNumus){
            tempVector = utils.appendVectors(blankVectorNue,
                         utils.rebinVector(tempMultiWeightInfoNue[N_weight],nueBins),
                         utils.rebinVector(tempMultiWeightInfoNumu[N_weight],numuBins));
          }
          else if (includeFosc){
            tempVector = utils.appendVectors(blankVectorNue,
                         utils.rebinVector(tempMultiWeightInfoNue[N_weight],nueBins));
          }
          else if (includeNumus){
            tempVector = utils.appendVectors(
                         utils.rebinVector(tempMultiWeightInfoNue[N_weight],nueBins),
                         utils.rebinVector(tempMultiWeightInfoNumu[N_weight],numuBins));
          }
          else{
            tempVector = utils.rebinVector(tempMultiWeightInfoNue[N_weight],nueBins);
          }
          for (int this_bin = 0; this_bin < nbins; this_bin++)
          {
              eventsNullVecMultiWeight[N_weight][b_line*nbins + this_bin]
                 = scales[b_line]*tempVector[this_bin];
          }
        }

      }


    }//end loop over baselines

    //Everything is read, now scale the vectors.
    //this is to correct the pot to whatever we'd like it to
    //be for this calculation
    for (int b_line = 0; b_line < nL; b_line ++){
      for(int bin = 0; bin < nbins_nue; bin++){
        eventsnueVec[b_line][bin]                 *= scales[b_line];
        NueFromNueCC_muonVec[b_line][bin]         *= scales[b_line];
        NueFromNueCC_chargeKaonVec[b_line][bin]   *= scales[b_line];
        NueFromNueCC_neutKaonVec[b_line][bin]     *= scales[b_line];
        NueFromEScatterVec[b_line][bin]           *= scales[b_line];
        NueFromNC_pi0Vec[b_line][bin]             *= scales[b_line];
        NueFromNC_delta0Vec[b_line][bin]          *= scales[b_line];
        NueFromNumuCCVec[b_line][bin]             *= scales[b_line];
        NueFromCosmicsVec[b_line][bin]            *= scales[b_line];
        DirtVec[b_line][bin]                      *= scales[b_line];
        OtherVec[b_line][bin]                     *= scales[b_line];
        if (includeFosc){
          eventsnuefoscVec[b_line][bin]             *= scales[b_line];
          eventsSignalBestFitNuVec[b_line][bin]     *= scales[b_line]*sin22thBF;
          eventsSignalBestFitNubarVec[b_line][bin]  *= scales[b_line]*sin22thBF;
        }
        if (includeCosmics)
          eventsnueVec[b_line][bin] += NueFromCosmicsVec[b_line][bin];
      }
      if (includeNumus){
        for(int bin = 0; bin < nbins_numu; bin++){
          eventsnumuVec[b_line][bin] *= scales[b_line];
        }
      }


      if (includeFosc){
        //Scale the best fit signals:
        std::cout << "Using scale factor of " << sin22thBF << std::endl;

        for (int point = 0; point <= npoints; point ++){
          for(int bin = 0; bin < nbins_nue; bin++){
            eventsnueoscVec[b_line][point][bin] *= scales[b_line];
          }    
        } //loop over points
      }

    } //loop over baselines, ends scaling

    oscVector.resize(npoints+1);


    //Stitch together the vectors into the side by side versions (over npoints):
    //loop over npoints for eventsnL and eventsnLfit

    //=============================================
    //this histogram is a blank histogram
    std::vector<float> blankVector(nbins_nue, 0); 

    // Need to combine the vectors. Combine each baseline first, then all
    // baselines.  It's not quite the most efficient route but I don't
    // think it matters.
    std::vector< std::vector<float> > combinedByBaseline;
    combinedByBaseline.resize(nL);
    for (int b_line = 0; b_line < nL; ++b_line)
    {
      if (includeFosc && includeNumus){
        combinedByBaseline[b_line] = utils.appendVectors( blankVector,
                                                  eventsnueVec[b_line],
                                                  eventsnumuVec[b_line]);
      }
      else if (includeNumus){
        combinedByBaseline[b_line] = utils.appendVectors( eventsnueVec[b_line],
                                                  eventsnumuVec[b_line]);
      }
      else if (includeFosc){
        combinedByBaseline[b_line] = utils.appendVectors( blankVector,
                                                  eventsnueVec[b_line]);
      }
      else{
        combinedByBaseline[b_line] = eventsnueVec[b_line];
      }
    }

    // Now combine into the total vectors
    if (nL == 1) 
      nullBackgrounds = combinedByBaseline.front();
    if (nL == 2) 
      nullBackgrounds = utils.appendVectors(combinedByBaseline.at(0), 
                                            combinedByBaseline.at(1));
    if (nL == 3) 
      nullBackgrounds = utils.appendVectors(combinedByBaseline.at(0), 
                                            combinedByBaseline.at(1),
                                            combinedByBaseline.at(2));

    //Fill the combined vectors: 
    for (int point = 0; point <= npoints; point++){
      for (int b_line = 0; b_line < nL; ++b_line)
      {
        if (includeFosc && includeNumus){
          combinedByBaseline[b_line] = utils.appendVectors( eventsnueoscVec[b_line][point],
                                                    eventsnueVec[b_line],
                                                    eventsnumuVec[b_line]);
        }
        else if (includeNumus){
          combinedByBaseline[b_line] = utils.appendVectors( eventsnueVec[b_line],
                                                    eventsnumuVec[b_line]);
        }
        else if (includeFosc){
          combinedByBaseline[b_line] = utils.appendVectors( eventsnueoscVec[b_line][point],
                                                    eventsnueVec[b_line]);
        }
        else{
          combinedByBaseline[b_line] = eventsnueVec[b_line];
        }
      }
      // Now combine into the total vectors
      if (nL == 1) 
        oscVector[point] = combinedByBaseline.front();
      if (nL == 2) 
        oscVector[point] = utils.appendVectors(combinedByBaseline.at(0), 
                                        combinedByBaseline.at(1));
      if (nL == 3) 
        oscVector[point] = utils.appendVectors(combinedByBaseline.at(0), 
                                        combinedByBaseline.at(1),
                                        combinedByBaseline.at(2));
    }



    if (verbose){
      std::cout << "\n------------------------\nPrinting out P=0.3% values and P=0.0% values:\n";
      std::cout << "\tP=0\t\tosc"<<npoints/2;
      if (useMultiWeights){
        std::cout << "\t\tMWF\t\tMWB";
      }
      std::cout<<"\n";
      for (unsigned int i = 0; i < nullBackgrounds.size(); i++){
        std::cout << "\t" << nullBackgrounds[i] << "\t\t";
        std::cout << oscVector[0][i];
        if (useMultiWeights){
          std::cout << "\t\t" << eventsNullVecMultiWeight.front().at(i);
          std::cout << "\t\t" << eventsNullVecMultiWeight.back().at(i);
        }      
        std::cout << "\n";
      }
      std::cout << "------------------------" << std::endl;
    }



    //if not using the covariance matrix, going to use near detector
    //statistics as the systematics downstream.  This involves going and finding
    //the near detector statistics in each case, and then populating the covariance matrix
    //Since we can't do this until we're reading in the event rates, there's no point to do
    //it here.  But we can read in the near detector stats:
    //Only have useful near det stats for nue and numu, nothing on signal.  However,
    //going to make an array nbins long and just leave the signal entries at zero



    return 0;
  }

  int NueAppearanceFitter::BuildCovarianceMatrix(){
    
    TMatrix covarianceMatrix;
    TMatrix fractionalErrorMatrix;
    TMatrix correlationMatrix;

    TMatrix Shape_covarianceMatrix;
    TMatrix fractional_Shape_covarianceMatrix;
    TMatrix Mixed_covarianceMatrix;
    TMatrix Norm_covarianceMatrix;

    // if means to compute the matrix for no signal.
    fractionalErrorMatrix.Clear();
    fractionalErrorMatrix.ResizeTo(nL*nbins_null,nL*nbins_null);
    correlationMatrix.Clear();
    correlationMatrix.ResizeTo(nL*nbins_null,nL*nbins_null);
    covarianceMatrix.Clear();
    covarianceMatrix.ResizeTo(nL*nbins_null,nL*nbins_null);

    Shape_covarianceMatrix.Clear();
    Shape_covarianceMatrix.ResizeTo(nL*nbins_null,nL*nbins_null);
    fractional_Shape_covarianceMatrix.Clear();
    fractional_Shape_covarianceMatrix.ResizeTo(nL*nbins_null,nL*nbins_null);
    Mixed_covarianceMatrix.Clear();
    Mixed_covarianceMatrix.ResizeTo(nL*nbins_null,nL*nbins_null);
    Norm_covarianceMatrix.Clear();
    Norm_covarianceMatrix.ResizeTo(nL*nbins_null,nL*nbins_null);


    double n_total =0.0;


    TH2D * covarianceMatrixHist 
         = new TH2D("covMatHist","Covariance Matrix",
                    nL*nbins_null,0,nL*nbins_null-1,
                    nL*nbins_null,0,nL*nbins_null-1);
    TH2D * fractionalMatrixHist 
         = new TH2D("fracMatHist","Fractional Covariance Matrix",
                    nL*nbins_null,0,nL*nbins_null-1,
                    nL*nbins_null,0,nL*nbins_null-1);
    TH2D * correlationMatrixHist 
         = new TH2D("corrMatHist","Correlation Matrix",
                    nL*nbins_null,0,nL*nbins_null-1,
                    nL*nbins_null,0,nL*nbins_null-1);
    TH2D * shapeFract_MatrixHist 
         = new TH2D("shapeMatHist","Fractional Shape Matrix",
                    nL*nbins_null,0,nL*nbins_null-1,
                    nL*nbins_null,0,nL*nbins_null-1);

    // Here's the method.  The nominal, no signal sample is in
    // nullBackgrounds.  It's a vector of length nL*nbins.  It 
    // contains NO signal, and we shouldn't change that.
    // The multiweight, signalless vectors are in eventsNullVecMultiWeight
    // and each entry eventsNullVecMultiWeight[N_weight] is of the same
    // type and length as nullBackgrounds.
    // 
    // Loop over each weight and compute the covariance between the nominal
    // and the multiweight vectors.
    // 
    // I'm going to copy the nullBackgrounds so that it isn't changed

    std::vector<float> events_nominal_COPY;

    // The covariance matrix doesn't care about signal, so make sure 
    // that the null vector doesn't have signal either
    
    if (!includeFosc)
      events_nominal_COPY = nullBackgrounds;
    else{
      // events_nominal_COPY must be collapsed to remove the empty spaces
      events_nominal_COPY = utils.collapseVector(nullBackgrounds,
                                                 nbins_nue,
                                                 nbins_numu,
                                                 nL);
    }

    // // Don't want to include any cosmics in the matrix calculation, so:
    // if (includeCosmics){
    //   for (int b_line = 0; b_line < nL; ++b_line)
    //   {
    //     for (int bin = 0; bin < nbins_nue; ++bin)
    //     {
    //       events_nominal_COPY[b_line*(nbins)+nbins_nue+bin] -= NueFromCosmicsVec[b_line][bin];
    //     }
    //   }
    // }


    // these vectors are to hold the multiweight versions of above 
    // for each iteration
    std::vector<float> temp_events_MW_COPY;
    std::vector<float> saved_events_MW_COPY;

    std::cout << "Computing the covariance matrix...." << std::endl;
    for (int N_weight = 0; N_weight < nWeights; ++N_weight)
    {
      temp_events_MW_COPY = eventsNullVecMultiWeight[N_weight];
      if (events_nominal_COPY.size() != temp_events_MW_COPY.size()){
        std::cerr << "Problem with vector sizes in the covariance Matrix function.\n";
        exit(-2);
      }

      if (N_weight == nWeights-1){
        saved_events_MW_COPY = temp_events_MW_COPY;
      }

      for (unsigned int ibin = 0; ibin < events_nominal_COPY.size(); ++ibin)
      {
        // This is for computing the total number of events and is used in
        // factoring the covariance matrix into shape, mixed, and norm parts
        n_total += events_nominal_COPY[ibin];

        for (unsigned int jbin = 0; jbin < events_nominal_COPY.size(); ++jbin)
        {

          float part1 = events_nominal_COPY[ibin]-temp_events_MW_COPY[ibin];
          float part2 = events_nominal_COPY[jbin]-temp_events_MW_COPY[jbin];
          covarianceMatrix[ibin][jbin] += (1.0/nWeights)*(part1*part2);
          if (debug && 
              ibin == 0 && 
              jbin == 0)
              // ibin == events_nominal_COPY.size()/2 && 
              // jbin == events_nominal_COPY.size()/2)
          {
            std::cout << "This is the debug point!!\n";
            std::cout << "  nominal, i: " << events_nominal_COPY[ibin]<<std::endl;
            std::cout << "  weights, i: " << temp_events_MW_COPY[ibin]<<std::endl;
            std::cout << "  nominal, j: " << events_nominal_COPY[jbin]<<std::endl;
            std::cout << "  weights, j: " << temp_events_MW_COPY[jbin]<<std::endl;
            std::cout << "Adding " << (1.0/nWeights)*(part1*part2) << std::endl; 
          }
        }
      }

    }


    if (debug){
      std::cout << "Printing out the nominal and last used weight vectors:\n";
      for (int ibin = 0; ibin< nL*nbins; ++ibin){
        std::cout << events_nominal_COPY[ibin]  << "\t\t";
        std::cout << saved_events_MW_COPY[ibin] << "\n";
      }
    }



    // This loop takes the error matrix and computes subsequent matrices from it:
    // correlation matrix
    // fractional error matrix
    // 
    // 

    double covMatSum = 0.0;

    for (unsigned int ibin = 0; ibin <events_nominal_COPY.size(); ++ibin)
    {
      for (unsigned int jbin = 0; jbin <events_nominal_COPY.size(); ++jbin)
      {
        covMatSum += covarianceMatrix[ibin][jbin];
        float norm = 1;
        if (covarianceMatrix[ibin][jbin] != 0)
        {
          norm = events_nominal_COPY[ibin];
          norm *= events_nominal_COPY[jbin];
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


    // std::cout << "Printing out the fractional uncertainties along the diagonal:\n";
    // for (unsigned int ibin = 0; ibin <events_nominal_COPY.size(); ++ibin)
    // {
    //   std::cout << "Bin " << ibin << ": " 
    //             << covarianceMatrix[ibin][ibin] << std::endl;
    // }

    // std::cout << "Printing out fractional covariance matrix:"<<std::endl;
    // for (int ibin = 0; ibin < nL*nbins; ++ibin){
    //   for (int jbin = 0; jbin < nL*nbins; ++jbin){
    //     std::cout << fractionalErrorMatrix[ibin][jbin] << "\t";
    //   }      
    //   std::cout << "\n";
    // }


    // Compute the following:
    // shape only matrix
    // fractional shape only matrix
    // mixed matrix
    // normalization matrix
    // 
    // Need to compute a few sums first:
    for (unsigned int ibin = 0; ibin < events_nominal_COPY.size(); ++ibin)
    {
      for (unsigned int jbin = 0; jbin < events_nominal_COPY.size(); ++jbin)
      {
        Shape_covarianceMatrix[ibin][jbin] = covarianceMatrix[ibin][jbin];
        // std::cout << "Shape matrix entry is: " << Shape_covarianceMatrix[ibin][jbin] << "\n";
        double temp = events_nominal_COPY[ibin];
        // std::cout << "temp is " << temp << " * " 
                  // << events_nominal_COPY[jbin] + sin22th*signal_nominal_COPY[jbin]
                  // << " = ";
        temp *= events_nominal_COPY[jbin];
        // std::cout << temp << "\n";
        temp /= n_total*n_total;
        temp *= covMatSum;
        // std::cout << "temp gets divided by " 
                  // << n_total << " * " << n_total << " / " << covMatSum
                  // << " to be " << temp << "\n";
        Shape_covarianceMatrix[ibin][jbin] -= temp;
        // std::cout << "Shape matrix entry is now: " << Shape_covarianceMatrix[ibin][jbin] << "\n";
        temp = events_nominal_COPY[ibin];
        temp *= events_nominal_COPY[jbin];
        if (temp != 0)
          fractional_Shape_covarianceMatrix[ibin][jbin] = Shape_covarianceMatrix[ibin][jbin]/temp;
        else
          fractional_Shape_covarianceMatrix[ibin][jbin] = 0.0;
        // std::cout << "Frac Shape matrix entry is: " << fractional_Shape_covarianceMatrix[ibin][jbin] << "\n\n";

        shapeFract_MatrixHist -> SetBinContent(ibin+1,jbin+1,fractional_Shape_covarianceMatrix[ibin][jbin]);
      }
    }

    // std::cout << "Printing out fractional shape covariance matrix:"<<std::endl;
    // for (int ibin = 0; ibin < nL*nbins; ++ibin){
    //   for (int jbin = 0; jbin < nL*nbins; ++jbin){
    //     std::cout << fractional_Shape_covarianceMatrix[ibin][jbin] << "\t";
    //   }      
    //   std::cout << "\n";
    // }

    // for (int bin = 0; bin < events_nominal_COPY.size(); ++bin)
    // {
    //   std::cout << fractional_Shape_covarianceMatrix[bin][bin] << "\t"
    //   std::cout << fractional_Shape_covarianceMatrix[bin][bin] << "\t"
    //   std::cout << Shape_covarianceMatrix[bin][bin] << "\t"
    //   std::cout << fractional_Shape_covarianceMatrix[bin][bin] << "\t"
    // }

    if (savePlots){
      // Need to get the name of the matrix right:
      
      // save to the folder "matrices":
      TString path = fileSource + "/matrices/";

      TString matrixFileName = utils.GetMatrixFileName( path,
                                                        detNamesString,
                                                        fileNameRoot,
                                                        includeNumus,
                                                        useXSecWeights,
                                                        useFluxWeights,
                                                        multiWeightSource,
                                                        absolute_MWSource);

      TFile * fileOut = new TFile(matrixFileName,"RECREATE");
      covarianceMatrixHist  -> Write();
      fractionalMatrixHist  -> Write();
      correlationMatrixHist -> Write();
      shapeFract_MatrixHist ->Write();

      // Collapse these matrices to remove the signal region for the write up.

      // std::cout << "events nominal copy: \n";
      // for (unsigned int i = 0; i < events_nominal_COPY.size(); i++){
      //   std::cout << events_nominal_COPY.at(i) << "\n";
      // }

      covarianceMatrix.Write("covarianceMatrix");
      fractionalErrorMatrix.Write("fractionalCovarianceMatrix");
      correlationMatrix.Write("correlationMatrix");

      // Dig the event rates out of the vectors and put them in the file
      // just for a cross check and to be sure what the matrices are

      std::vector<TH1F *>  nueEventRates;
      std::vector<TH1F *>  numuEventRates;
      nueEventRates.resize(nL);
      if (includeNumus) numuEventRates.resize(nL);
      for (int b_line = 0; b_line < nL; ++b_line)
      {
        char temp[100];
        sprintf(temp,"nueEventRates_%s",baselines[b_line].c_str());
        nueEventRates[b_line] = new TH1F(temp,"Nue Event Rates",nbins_nue,&(nueBins[0]));
        for (int bin = 0; bin < nbins_nue; ++bin)
        {
          nueEventRates[b_line]  -> SetBinContent(bin+1,events_nominal_COPY[b_line*nbins_null+bin]);
        }
        nueEventRates[b_line] -> Write();

        if (includeNumus){
          sprintf(temp,"numuEventRates_%s",baselines[b_line].c_str());
          numuEventRates[b_line] = new TH1F(temp,"Numu Event Rates",nbins_numu,&(numuBins[0]));
          for (int bin = 0; bin < nbins_numu; ++bin)
          {
            numuEventRates[b_line] -> SetBinContent(bin+1,events_nominal_COPY[b_line*nbins+2*nbins_nue+bin]);
          }
          numuEventRates[b_line] -> Write();
        }
      }

      fileOut -> Close();
    }

    return 0;

  }

  int NueAppearanceFitter::MakeRatioPlots(){

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

    // Do both signals (nue,numu) in separate vectors
    // though computed in parallel.  And bin by baseline to make it easier.
    std::vector<std::vector<float> > events_nue_nominal_COPY;
    std::vector<std::vector<float> > events_numu_nominal_COPY;
    events_nue_nominal_COPY.resize(nL);
    if(includeNumus) events_numu_nominal_COPY.resize(nL);

    for (int b_line = 0; b_line < nL; ++b_line)
    {
      events_nue_nominal_COPY[b_line].resize(nbins_nue);
      for (int bin = 0; bin < nbins_nue; ++bin)
      {
        if (!includeFosc)
          events_nue_nominal_COPY[b_line][bin] 
            = nullBackgrounds[b_line*nbins_null+bin];
        else
          events_nue_nominal_COPY[b_line][bin]
            = nullBackgrounds[b_line*nbins+nbins_nue+bin];
      }
      if(includeNumus){
        events_numu_nominal_COPY[b_line].resize(nbins_numu);
        for (int bin = 0; bin < nbins_numu; ++bin)
        {
        if (!includeFosc)
          events_numu_nominal_COPY[b_line][bin] 
            = nullBackgrounds[b_line*nbins_null+bin + nbins_nue];
        else
          events_numu_nominal_COPY[b_line][bin]
            = nullBackgrounds[b_line*nbins+nbins_nue+bin];        
        }
      }
    }



    // Now compute the nominal ratio
    std::vector<std::vector<float>> nominal_nue_Ratio;
    std::vector<std::vector<float>> nominal_numu_Ratio;

    nominal_nue_Ratio.resize(nL-1);
    nominal_numu_Ratio.resize(nL-1);

    if (debug){
      std::cout << "Printing nominal values:\n";
      for (int b_line = 0; b_line < nL; b_line ++){
        std::cout << "  Baseline: " << baselinesFancy[b_line] << std::endl;
        std::cout << "    nue   \tnumu" << std::endl;
        for (unsigned int entry = 0; entry < events_numu_nominal_COPY.at(b_line).size(); entry ++)
        {
          if (entry < events_nue_nominal_COPY.at(b_line).size()){
            std::cout << "    " << events_nue_nominal_COPY.at(b_line).at(entry);
          }
          else{
            std::cout << "      -    ";
          }
            std::cout << "\t" << events_numu_nominal_COPY.at(b_line).at(entry) << std::endl;
        }
        std::cout << std::endl;
      }
    }




    // The ratio is the far detector divided by the near detector, always.
    for (int b_line = 1; b_line < nL; ++b_line)
    {
      nominal_nue_Ratio[b_line-1].resize(nbins_nue);
      for (int bin = 0; bin < nbins_nue; ++bin)
      {
        nominal_nue_Ratio [b_line-1][bin] = events_nue_nominal_COPY [b_line][bin]/events_nue_nominal_COPY [0][bin];
      }
      if (includeNumus){
        nominal_numu_Ratio[b_line-1].resize(nbins_numu);
        for (int bin = 0; bin < nbins_numu; ++bin)
        {
          nominal_numu_Ratio[b_line-1][bin] = events_numu_nominal_COPY[b_line][bin]/events_numu_nominal_COPY[0][bin];
        }
      }
    }


    // these vectors are to hold the multiweight versions of above 
    // for each iteration
    std::vector<std::vector<float> > temp_events_nue_MW_COPY;
    std::vector<std::vector<float> > temp_events_numu_MW_COPY;

    std::vector<std::vector<float>> temp_nue_Ratio;
    std::vector<std::vector<float>> temp_numu_Ratio;

    // Need a spot to hold the variance of these ratios...
    std::vector<std::vector<float> > error_events_nue_Ratio;
    std::vector<std::vector<float> > error_events_numu_Ratio;

    // And a spot to hold the ratios themselves
    std::vector<std::vector<std::vector<float> > > nue_multiWeightRatio;
    std::vector<std::vector<std::vector<float> > > numu_multiWeightRatio;

    nue_multiWeightRatio.resize(nL-1);
    error_events_nue_Ratio.resize(nL-1);

    if (includeNumus){
      numu_multiWeightRatio.resize(nL-1);
      error_events_numu_Ratio.resize(nL-1);    
    }

    for (int i = 0; i < nL-1; ++i)
    {
      error_events_nue_Ratio[i].resize(nbins_nue);
      nue_multiWeightRatio[i].resize(nWeights);
      if (includeNumus){
        error_events_numu_Ratio[i].resize(nbins_numu);
        numu_multiWeightRatio[i].resize(nWeights);
      }
    }


    std::cout << "Computing the ratio errors...." << std::endl;
    for (int N_weight = 0; N_weight < nWeights; ++N_weight)
    {

      // First step, get the ratio for this particular weight.
      // Zeroth step, get the event rates for this weight!


      temp_events_nue_MW_COPY.clear();

      temp_nue_Ratio.clear();
      
      temp_nue_Ratio.resize(nL-1);

      temp_events_nue_MW_COPY.resize(nL);

      if (includeNumus){
        temp_events_numu_MW_COPY.clear();
        temp_numu_Ratio.clear();
        temp_numu_Ratio.resize(nL-1);
        temp_events_numu_MW_COPY.resize(nL);
      }

      for (int b_line = 0; b_line < nL; ++b_line)
      {

        temp_events_nue_MW_COPY[b_line].resize(nbins_nue);
        // std::cout << "eventsNullVecMultiWeight.size()" << eventsNullVecMultiWeight.size() << std::endl;

        for (int bin = 0; bin < nbins_nue; ++bin)
        {
          // std::cout << "index is " << b_line*nbins_null + bin 
          //           << ", value is " << eventsNullVecMultiWeight[N_weight][b_line*nbins_null + bin]
          //           << std::endl;
          if (!includeFosc)
            temp_events_nue_MW_COPY[b_line][bin]  
              = eventsNullVecMultiWeight[N_weight][b_line*nbins_null + bin];
          else
            temp_events_nue_MW_COPY[b_line][bin]  
              = eventsNullVecMultiWeight[N_weight][b_line*nbins+nbins_nue+bin];

        }
 

        if (includeNumus){
          temp_events_numu_MW_COPY[b_line].resize(nbins_numu);
          for (int bin = 0; bin < nbins_numu; ++bin)
          {
            temp_events_numu_MW_COPY[b_line][bin] 
              = eventsNullVecMultiWeight[N_weight][b_line*nbins_null + bin + nbins_nue];
          }
        }

      }
      // std::cout << "This set of event rates (N = " << N_weight <<") is:\n";
      // for (unsigned int i =0; i < eventsNullVecMultiWeight[N_weight].size();i++)
      // {
      //   std::cout << "  " << temp_events_nue_MW_COPY[N_weight][i]
      //             << " (" << eventsNullVecMultiWeight[N_weight][i] 
      //             << ")"  << std::endl;
      // }

      // Ok, got all the data, compute the ratio for this universe:
      for (int b_line = 1; b_line < nL; ++b_line)
      {
        temp_nue_Ratio[b_line-1].resize(nbins_nue);
        for (int bin = 0; bin < nbins_nue; ++bin)
        {
          temp_nue_Ratio[b_line-1][bin]  = temp_events_nue_MW_COPY[b_line][bin] / temp_events_nue_MW_COPY[0][bin];
        }
        nue_multiWeightRatio[b_line-1][N_weight] = temp_nue_Ratio[b_line-1];
        if (includeNumus){
          temp_numu_Ratio[b_line-1].resize(nbins_numu);
          for (int bin = 0; bin < nbins_numu; ++bin)
          {
            temp_numu_Ratio[b_line-1][bin] = temp_events_numu_MW_COPY[b_line][bin] / temp_events_numu_MW_COPY[0][bin];
          }
          numu_multiWeightRatio[b_line-1][N_weight] = temp_numu_Ratio[b_line-1];
        }
      }

      // std::cout <<"N_weight, temp_nue_Ratio ND/600 0: " << N_weight << " "
      //           << temp_nue_Ratio[0][0]<<std::endl;

      // Now we have the nominal event rates and the event rates in this universe.
      // So, find the diffence, square it, divide by nWeights, and add it to the errors:
      for (int b_line = 0; b_line < nL-1; ++b_line)
      {
        for (int bin = 0; bin < nbins_nue; ++bin)
        {
          error_events_nue_Ratio[b_line][bin]  
            += (1.0/nWeights)
               *(temp_nue_Ratio[b_line][bin]  - nominal_nue_Ratio[b_line][bin])
               *(temp_nue_Ratio[b_line][bin]  - nominal_nue_Ratio[b_line][bin]);
        }
        if (includeNumus){
          for (int bin = 0; bin < nbins_numu; ++bin)
          {
            error_events_numu_Ratio[b_line][bin] 
              += (1.0/nWeights)
                 *(temp_numu_Ratio[b_line][bin] - nominal_numu_Ratio[b_line][bin])
                 *(temp_numu_Ratio[b_line][bin] - nominal_numu_Ratio[b_line][bin]);     
          }
        }
      }

    }

    if (debug){
      std::cout << "Printing the nominal and last used event rates and ratios." << std::endl;
      for (int b_line = 0; b_line < nL; ++b_line)
      { 
        std::cout << "------nominal-------------" << std::endl;
        std::cout << "Nom\tLast" << std::endl;
        for (int bin = 0; bin < nbins_nue; ++bin)
        {
          std::cout << events_nue_nominal_COPY[b_line][bin] << "\t";
          std::cout << temp_events_nue_MW_COPY[b_line][bin] << "\n";
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
    nue_errorPlots.resize(nL-1);
    nue_stat_errorPlots.resize(nL-1);
    nue_MCstat_errorPlots.resize(nL-1);

    if (includeNumus){
      numu_ratioPlots.resize(nL-1);
      numu_errorPlots.resize(nL-1);
      numu_stat_errorPlots.resize(nL-1);
      numu_MCstat_errorPlots.resize(nL-1);
    }

    for (int i = 0; i < nL-1; i++){
      char temp[100];
      sprintf(temp,"nue_ratioplot_%i",i);
      nue_ratioPlots[i] = new TH1F(temp,"Ratio Plot",nbins_nue,&(nueBins[0]));
      sprintf(temp,"#nu_{e} Ratio of %s to %s",baselinesFancy[0].c_str(),baselinesFancy[i+1].c_str());
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
      sprintf(temp,"%s / %s", baselinesFancy[i+1].c_str(),baselinesFancy[0].c_str());
      nue_ratioPlots[i] -> GetYaxis() -> SetTitle(temp);
      nue_ratioPlots[i] -> GetYaxis() -> SetTitleSize(.045);
      nue_ratioPlots[i] -> GetYaxis() -> SetTitleOffset(1.6);

      sprintf(temp,"nue_errorplot_%i",i);
      nue_errorPlots[i] = new TH1F(temp,"Error Plot",nbins_nue,&(nueBins[0]));
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


      sprintf(temp,"nue_stat_errorplot_%i",i);
      nue_stat_errorPlots[i] = new TH1F(temp,"Statistical Uncert. Plot",nbins_nue,&(nueBins[0]));
      sprintf(temp,"#nu_{e} Statistical Uncert. of %s to %s",baselines[0].c_str(),baselines[i+1].c_str());
      nue_stat_errorPlots[i] -> SetTitle(temp);
      nue_stat_errorPlots[i] -> SetMinimum(0);
      nue_stat_errorPlots[i] -> SetMaximum(25);
      nue_stat_errorPlots[i] -> GetXaxis() -> SetTitle("Reconstructed Energy (GeV)");
      nue_stat_errorPlots[i] -> GetYaxis() -> SetTitle("Percent Uncert. [\%]");
      nue_stat_errorPlots[i] -> SetLineWidth(2);

      sprintf(temp,"nue_MCstat_errorplot_%i",i);
      nue_MCstat_errorPlots[i] = new TH1F(temp,"MC Statistical Uncert. Plot",nbins_nue,&(nueBins[0]));
      sprintf(temp,"#nu_{e} MC Statistical Uncert. of %s to %s",baselines[0].c_str(),baselines[i+1].c_str());
      nue_MCstat_errorPlots[i] -> SetTitle(temp);
      nue_MCstat_errorPlots[i] -> SetMinimum(0);
      nue_MCstat_errorPlots[i] -> SetMaximum(25);
      nue_MCstat_errorPlots[i] -> SetLineColor(11);
      nue_MCstat_errorPlots[i] -> GetXaxis() -> SetTitle("Reconstructed Energy (GeV)");
      nue_MCstat_errorPlots[i] -> GetYaxis() -> SetTitle("Percent Uncert. [\%]");
      
      for (int bin = 0; bin < nbins_nue; ++bin)
      {
        
        nue_ratioPlots[i] -> SetBinContent(bin+1, nominal_nue_Ratio[i][bin]);
        nue_errorPlots[i] -> SetBinContent(bin+1, 100*sqrt(error_events_nue_Ratio[i][bin])
                                                           /nominal_nue_Ratio[i][bin]);
        nue_stat_errorPlots[i]    -> SetBinContent(bin+1, 100*sqrt(1.0/events_nue_nominal_COPY[0][bin]
                                                                 + 1.0/events_nue_nominal_COPY[i+1][bin]));
        nue_MCstat_errorPlots[i]  -> SetBinContent(bin+1, 100*sqrt(1.0/eventsNueMCStats[0][bin]
                                                                 + 1.0/eventsNueMCStats[i+1][bin]));
        // std::cout << "eventsNumuMCStats[0][bin]"   << eventsNumuMCStats[0][bin]   << "\t";
        // std::cout << "eventsNumuMCStats[i+1][bin]" << eventsNumuMCStats[i+1][bin] << std::endl;
      }

      if (includeNumus){
        sprintf(temp,"numu_ratioplot_%i",i);
        numu_ratioPlots[i] = new TH1F(temp,"Ratio Plot",nbins_numu,&(numuBins[0]));
        sprintf(temp,"#nu_{#mu} Ratio of %s to %s",baselines[0].c_str(),baselines[i+1].c_str());
        numu_ratioPlots[i] -> SetTitle(temp);
        numu_ratioPlots[i] -> SetMinimum(0);
        numu_ratioPlots[i] -> SetMaximum(1);
        numu_ratioPlots[i] -> GetXaxis() -> SetTitle("Reconstructed Energy (GeV)");
        numu_ratioPlots[i] -> GetYaxis() -> SetTitle("Event Ratio");
        
        sprintf(temp,"numu_errorplot_%i",i);
        numu_errorPlots[i] = new TH1F(temp,"Uncert. Plot",nbins_numu,&(numuBins[0]));
        sprintf(temp,"#nu_{#mu} Uncert. of %s to %s",baselines[0].c_str(),baselines[i+1].c_str());
        numu_errorPlots[i] -> SetTitle(temp);
        numu_errorPlots[i] -> SetMinimum(0);
        numu_errorPlots[i] -> SetMaximum(10);
        numu_errorPlots[i] -> GetXaxis() -> SetTitle("Reconstructed Energy (GeV)");
        numu_errorPlots[i] -> GetYaxis() -> SetTitle("Percent Uncert. [\%]");

        sprintf(temp,"numu_stat_errorplot_%i",i);
        numu_stat_errorPlots[i] = new TH1F(temp,"Statistical Uncert. Plot",nbins_numu,&(numuBins[0]));
        sprintf(temp,"#nu_{#mu} Statistical Uncert. of %s to %s",baselines[0].c_str(),baselines[i+1].c_str());
        numu_stat_errorPlots[i] -> SetTitle(temp);
        numu_stat_errorPlots[i] -> SetMinimum(0);
        numu_stat_errorPlots[i] -> SetMaximum(10);
        numu_stat_errorPlots[i] -> GetXaxis() -> SetTitle("Reconstructed Energy (GeV)");
        numu_stat_errorPlots[i] -> GetYaxis() -> SetTitle("Percent Uncert. [\%]");

        sprintf(temp,"numu_MCstat_errorplot_%i",i);
        numu_MCstat_errorPlots[i] = new TH1F(temp,"MC Statistical Uncert. Plot",nbins_numu,&(numuBins[0]));
        sprintf(temp,"#nu_{#mu} MC Statistical Uncert. of %s to %s",baselines[0].c_str(),baselines[i+1].c_str());
        numu_MCstat_errorPlots[i] -> SetTitle(temp);
        numu_MCstat_errorPlots[i] -> SetMinimum(0);
        numu_MCstat_errorPlots[i] -> SetMaximum(10);
        numu_MCstat_errorPlots[i] -> SetLineColor(11);
        numu_MCstat_errorPlots[i] -> GetXaxis() -> SetTitle("Reconstructed Energy (GeV)");
        numu_MCstat_errorPlots[i] -> GetYaxis() -> SetTitle("Percent Uncert. [\%]");


        for (int bin = 0; bin < nbins_numu; ++bin)
        {
          numu_ratioPlots[i] -> SetBinContent(bin+1, nominal_numu_Ratio[i][bin]);
          numu_errorPlots[i] -> SetBinContent(bin+1, 100*sqrt(error_events_numu_Ratio[i][bin])
                                                              /nominal_numu_Ratio[i][bin]);
          numu_stat_errorPlots[i]   -> SetBinContent(bin+1, 100*sqrt(1.0/events_numu_nominal_COPY[0][bin]
                                                                   + 1.0/events_numu_nominal_COPY[i+1][bin]));
          numu_MCstat_errorPlots[i] -> SetBinContent(bin+1, 100*sqrt(1.0/eventsNumuMCStats[0][bin]
                                                                   + 1.0/eventsNumuMCStats[i+1][bin]));
          
        }
      }

    }

    gStyle->SetOptStat(0);
    leg_ratio -> AddEntry(nue_ratioPlots[0],"Nominal  #nu_{e} event ratio","l");
    // leg_ratio -> AddEntry();
    leg_error -> AddEntry(nue_stat_errorPlots[0],"Data Statistical Uncert.","l");
    leg_error -> AddEntry(nue_MCstat_errorPlots[0],"MC Statistical Uncert.","l");
    leg_error -> AddEntry(nue_errorPlots[0],"RMS for Ratio");
    leg_error -> SetTextSize(0.035);
    // TH1F *chr = stackedCanvas[j]->DrawFrame(emin-0.01,-0.01*(SignalNu->GetMaximum()),emax,1.0*max);

    TCanvas * canv_nue = new TCanvas("ratioplots_nue","ratioplots_nue",800,500*(nL-1));
    canv_nue -> Divide(2,nL-1);

    TCanvas * canv_numu;
    if (includeNumus){
      canv_numu = new TCanvas("ratioplots_numu","ratioplots_numu",800,500*(nL-1));
      canv_numu -> Divide(2,nL-1);
    }
    for (int b_line = 0; b_line < nL-1; ++b_line)
    {
      canv_nue -> cd(2*b_line + 1);
      nue_ratioPlots[b_line] -> Draw("H");
      for (int N_weight = 0; N_weight < nWeights; ++N_weight)
      {
        TH1F * temp_hist = utils.makeHistogram(nue_multiWeightRatio[b_line][N_weight],nueBins);
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

      if (includeNumus){
        canv_numu -> cd(2*b_line + 1);
        numu_ratioPlots[b_line] -> Draw("H");
        for (int N_weight = 0; N_weight < nWeights; ++N_weight)
        {
          TH1F * temp_hist = utils.makeHistogram(numu_multiWeightRatio[b_line][N_weight],numuBins);
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


    // First things first, get the complete, non scaled covariance matrix.
    nullVec = utils.collapseVector(nullBackgrounds,
                                   nbins_nue,
                                   nbins_numu,
                                   nL);

    TString path = fileSource + "/matrices/";

    std::cout << "covMatrixList.size() is " << covMatrixList.size() << std::endl;

    TMatrix * fullCovarianceMatrix 
      = utils.assembleCovarianceMatrix( path,
                                        detNamesString,
                                        includeNumus,
                                        nullVec,
                                        covMatrixList,
                                        covMatrixListSource,
                                        absolute_MWSource);

    std::cout << "Covariance matrix (0,0) is " << (*fullCovarianceMatrix)[0][0] << std::endl;
    // return 1;


    // We now have the full covariance matrix for the background!  
    // We can add the statistics and any inflation amount along the diagonal, then 
    // invert it and do the raster scan.
    // 
    // Since shape only fit is complicated, I have not really supported it yet.

    for (unsigned int bin = 0; bin < nullVec.size(); bin++)
    {
      // add the statistical uncertainty:
      (*fullCovarianceMatrix)[bin][bin] += nullVec.at(bin);
      if (inflateSystematics)
        (*fullCovarianceMatrix)[bin][bin] += nullVec.at(bin)*systematicInflationAmount;
    }

    // Now invert the matrix
    TMatrix invertedMatrix = fullCovarianceMatrix -> Invert();

    // This loop is actually doing the chisq calculation
    // It loops over npoints twice.  First over dm2, and second over sin22th
    // for (int dm2point = 450; dm2point <= npoints; dm2point ++){
    for (int dm2point = 0; dm2point <= npoints; dm2point ++){
      if (dm2point % 25 == 0) 
        std::cout << "dm^2: " << dm2point << " of " << npoints << std::endl;      
      // for (int sin22thpoint = 250; sin22thpoint <= npoints; sin22thpoint++){
      for (int sin22thpoint = 0; sin22thpoint <= npoints; sin22thpoint++){
        // std::cout << "sin22th: " << sin22thpoint << " of " << npoints << std::endl;      
        chisq = 0.0; //reset the chisq!


        //First, figure out what dm2 and sin22th are for this iteration:
        dm2 = pow(10.,(TMath::Log10(dm2min)+(dm2point*1./npoints)*TMath::Log10(dm2max/dm2min)));
        sin22th = pow(10.,(TMath::Log10(sin22thmin)+(sin22thpoint*1./npoints)*TMath::Log10(sin22thmax/sin22thmin)));
        if (debug) std::cout << "---------------------------Begin dm2: " << dm2 << ",\tsin22th: " << sin22th << std::endl;


        //Now build the covariance matrix for this point on (dm2, sin22th)
        //The current matrix is just the fractional systematics.
        //This means scaling each entry by the central values
        //And also means adding statistical errors on the diagonal.
        

        // At the end of the day, we need one vector that is the "predicted
        // background" and one that is "background plus signal."
        // background plus signal is easy to compute, it is just the collapsed
        // version of the vector that contains MC backgrounds and simulate
        // signal.  Always - whether it's shape only or shape plus rate, the 
        // prediction vector is always this.  The background vector can be more 
        // difficult in the shape only analysis.
        

        // Set up the predictive vectors:

        //Collapsed spectrum(dm2,s22th)
        //PredictionVec holds background + signal
        std::vector<float> predictionVec(nL*(nbins_nue + nbins_numu),0);
        // Null vec holds the rates with no oscillation signal
        // But in the shape only analysis, we normalize this to account
        // for high dm2 signal affecting the "null" background
        
        //before collapsing the prediction, need to scale the oscillated signal prediction:
        std::vector<float> eventsFitVecTemp = oscVector[dm2point]; //don't want to overwrite...
        for (int i = 0; i < nL; i ++){
            std::transform( eventsFitVecTemp.begin() + i*nbins, //start here
                            eventsFitVecTemp.begin() + i*nbins + nbins_nue, //end here
                            eventsFitVecTemp.begin() + i*nbins, //put it here (replace)
                            std::bind2nd(std::multiplies<double>(), sin22th)); //use this unary operation
            //starts at eventsnLfit
        }



        predictionVec = utils.collapseVector(eventsFitVecTemp,
                                             nbins_nue,
                                             nbins_numu,
                                             nL);
        // Now predictionVec and holds the right stuff
        

        // In the case of the shape only fit,
        // Copy the unscaled vector that will be used to build error matrix
        // Probably need to scale it as well
/*
        // Here's where the shape only stuff comes into play
        double intSignalND = 0.;    double intNooscND = 0.;
        std::vector<double> scalefac;
        if(shapeOnlyFit){
          // Loop over the near det and determine the ratio between 
          // background and background + signal
          for(int i = 0; i < nbins_nue; i ++){
              intSignalND = predictionVec[i];
              intNooscND  = nullVec[i];
              scalefac.push_back(intSignalND / intNooscND);
          }
          // if (verbose) for (auto & point : scalefac) std::cout << " Scalefac: " << point << "\n";
          
          // next scale the nue background for each det
          for(int b_line = 0; b_line < nL; b_line++)
          {
            for (int i = 0; i < nbins_nue; ++ i)
            {
                if (debug) std::cout << "\nScaled from: " << nullVec[b_line*(nbins_nue+nbins_numu) + i] << std::endl;
                nullVec[b_line*(nbins_nue+nbins_numu) + i] *= scalefac[i];
                // eventsFitVecTemp[nbinsE + n*nbinsE*3 + i] *= scalefac[i];
                if (debug) std::cout << "Scaled to: " << nullVec[b_line*(nbins_nue+nbins_numu) + i] << std::endl;
            }
          }
        }
*/
        // Ok, prediction vec and null vec are now the correct vectors
        // with scaling if necessary
        // 

        // Now make sure the statistical and systematic errors are correct:
        // These aren't used in the covariance matrix analysis.       







        


        //Checking collapsing:
        if (debug){
          std::cout << "Pre\t\tPost\n";
          for (unsigned int i = 0; i < oscVector[dm2point].size(); i++){
            std::cout << oscVector[dm2point][i] << "\t\t";
            if (i < predictionVec.size())  std::cout << predictionVec[i];
            std::cout << "\n";
          }
        }
        //This is the actual chi2 calculation
        //Energy spectrum fit
        //loop over collapsed bins
        for (int ibin=0; ibin < nL*(nbins_nue+nbins_numu); ibin++)
        {
            //loop over collapsed bins
            for (int jbin=0; jbin < nL*(nbins_nue+nbins_numu); jbin++)
            {

                float cvi = nullVec[ibin];
                float cvj = nullVec[jbin];

                float predictioni = predictionVec[ibin];
                float predictionj = predictionVec[jbin];
                
                if (debug){
                    if (ibin == jbin){
                        std::cout << "ibin: "<< ibin << " Prediction: " << predictioni << " cvi: " << cvi;
                        std::cout << " M^-1: " << (invertedMatrix)(ibin,jbin);
                        std::cout << " chi2: " << (predictioni-cvi)*(predictionj-cvj)* (invertedMatrix)(ibin,jbin) << std::endl;
                    }
                }
                // if (sin22thpoint == sin22thFittingPoint && dm2point == dm2FittingPoint){
                //   if (jbin == ibin){
                //     std::cout << "On bin " << ibin 
                //               << ", adding chi2 = " 
                //               << (predictioni-cvi)*(predictionj-cvj)
                //                 *(invertedMatrix)(ibin,jbin) << std::endl;
                //     if (useNearDetStats) 
                //       std::cout << "  nearDetStats error on this bin is "
                //                 << nearDetStats[ibin%(nbins_nue+nbins_numu)] 
                //                 << std::endl;
                //     std::cout << "  ibin: "<< ibin 
                //               << " Prediction: " << predictioni 
                //               << " cvi: " << cvi << std::endl;
                //     std::cout << "  jbin: "<< jbin 
                //               << " Prediction: " << predictionj 
                //               << " cvj: " << cvj;
                //     std::cout << "  M^-1: " << (*cov)(ibin,jbin) << std::endl;
                //   }
                // }
                chisq += (predictioni-cvi)*(predictionj-cvj)* (invertedMatrix)(ibin,jbin);

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
    

    path = fileSource + "/chi2/";
    TString chi2FileName = utils.GetChi2FileName( path,
                                                  fileNameRoot,
                                                  includeNumus,
                                                  covMatrixList,
                                                  covMatrixListSource,
                                                  absolute_MWSource);

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


  int NueAppearanceFitter::MakeSimplePlot(){

    // gStyle->SetOptStat(0000);
    // gStyle->SetOptFit(0000);
    // gStyle->SetPadBorderMode(0);
    // gStyle->SetPadBottomMargin(0.15);
    // gStyle->SetPadLeftMargin(0.15);
    // gStyle->SetPadRightMargin(0.05);
    // gStyle->SetFrameBorderMode(0);
    // gStyle->SetCanvasBorderMode(0);
    // gStyle->SetPalette(0);
    // gStyle->SetCanvasColor(0);
    // gStyle->SetPadColor(0);
    
    TGraph *sens90 = new TGraph(npoints+1,x90,y90); 
    TGraph *sens3s = new TGraph(npoints+1,x3s,y3s); 
    TGraph *sens5s = new TGraph(npoints+1,x5s,y5s);

    //Plot Results:
    sens90->SetLineColor(1); sens90->SetLineWidth(2);
    sens3s->SetLineColor(9); sens3s->SetLineWidth(2);
    sens5s->SetLineColor(9); sens5s->SetLineStyle(2); sens5s->SetLineWidth(1);   



    // TH2D* hr1=new TH2D("hr1","hr1",500,sin22thmin,sin22thmax,500,dm2min,dm2max);
    // hr1->Reset();
    // hr1->SetFillColor(0);
    // hr1->SetTitle(";sin^{2}2#theta_{#mue};#Deltam^{2}_{41} (eV^{2})");
    // hr1->GetXaxis()->SetTitleOffset(1.1);
    // hr1->GetYaxis()->SetTitleOffset(1.2);
    // hr1->GetXaxis()->SetTitleSize(0.05);
    // hr1->GetYaxis()->SetTitleSize(0.05);
    // hr1->GetXaxis()->CenterTitle();
    // hr1->GetYaxis()->CenterTitle();
    // hr1->SetStats(kFALSE);
    // hr1->Draw();


    TH2D * hr1 = plotUtils.getEmptySensPlot();


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
    tempCanv->cd();
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

    TString basename = fileSource + fileNameRoot;
/*
    if (savePlots){
      if(shapeOnlyFit){
          if (useNearDetStats){
              tempCanv -> Print(basename+"_nearDetStats_shapeOnly_megaPlot.pdf", "pdf");
              tempCanv -> Print(basename+"_nearDetStats_shapeOnly_megaPlot.eps", "eps");
          }
          // else if (useCovarianceMatrix) {
              // tempCanv -> Print(basename+"_covMat_shapeOnly_megaPlot.pdf", "pdf");
              // tempCanv -> Print(basename+"_covMat_shapeOnly_megaPlot.eps", "eps");
          // }
          else{
              tempCanv -> Print(basename+"_flatStats_shapeOnly_megaPlot.pdf", "pdf");
              tempCanv -> Print(basename+"_flatStats_shapeOnly_megaPlot.eps", "eps");
          }
      }
      else{
        if (useNearDetStats){
              tempCanv -> Print(basename+"_nearDetStats_megaPlot.pdf", "pdf");
              tempCanv -> Print(basename+"_nearDetStats_megaPlot.eps", "eps");
          }
          // else if (useCovarianceMatrix){
              // tempCanv -> Print(basename+"_covMat_megaPlot.pdf", "pdf");
              // tempCanv -> Print(basename+"_covMat_megaPlot.eps", "eps");
          // }
          else {
              tempCanv -> Print(basename+"_flatStats_megaPlot.pdf", "pdf");
              tempCanv -> Print(basename+"_flatStats_megaPlot.eps", "eps");
          }
      }
    }
    */


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

      
      TH1F * NueFromNueCC_muon = utils.makeHistogram(NueFromNueCC_muonVec[j],nueBins);
      TH1F * NueFromNueCC_chargeKaon = utils.makeHistogram(NueFromNueCC_chargeKaonVec[j],nueBins);
      TH1F * NueFromNueCC_neutKaon = utils.makeHistogram(NueFromNueCC_neutKaonVec[j],nueBins);
      TH1F * NueFromEScatter = utils.makeHistogram(NueFromEScatterVec[j],nueBins);
      TH1F * NueFromNC_pi0 = utils.makeHistogram(NueFromNC_pi0Vec[j],nueBins);
      TH1F * NueFromNC_delta0 = utils.makeHistogram(NueFromNC_delta0Vec[j],nueBins);
      TH1F * NueFromNumuCC = utils.makeHistogram(NueFromNumuCCVec[j],nueBins);
      TH1F * Dirt = utils.makeHistogram(DirtVec[j],nueBins);
      TH1F * Other = utils.makeHistogram(OtherVec[j],nueBins);
      TH1F * NueFromCosmics = utils.makeHistogram(NueFromCosmicsVec[j],nueBins);

      // pi0 and gamma are combined:
      // NueFromNC_pi0 -> Add(NueFromNC_delta0);

      TH1F * SignalNu;
      TH1F * SignalNubar;
      if (includeFosc){
        SignalNu = utils.makeHistogram(eventsSignalBestFitNuVec[j],nueBins);
        SignalNubar = utils.makeHistogram(eventsSignalBestFitNubarVec[j],nueBins);
      // TH1F* SignalNu = new TH1F("signal","signal",nbins_nue,&(nueBins[0]));
        SignalNu -> Add(SignalNubar);
      }
      // SignalNu->SetBinContent(1,77.47);
      // SignalNu->SetBinContent(2,22.65);
      // SignalNu->SetBinContent(3,3.57);
      // SignalNu->SetBinContent(4,1.37);
      // SignalNu->SetBinContent(5,0.54);
      // SignalNu->SetBinContent(6,0.13);
      // SignalNu->SetBinContent(7,0.1);
      // SignalNu->SetBinContent(8,0.07);
      // SignalNu->SetBinContent(9,0.07);
      // SignalNu->SetBinContent(10,0.0);



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
      NueFromCosmics          -> SetFillColor(kPink);

      std::vector< float > totalEvents, signalEvents;
      totalEvents.resize(nbins_nue);
      signalEvents.resize(nbins_nue);

      std::cout << "Integral backgrounds by type: " << std::endl;
      std::cout << "  NueFromNueCC_muon: ........" << NueFromNueCC_muon -> Integral() << "\n";
      std::cout << "  NueFromNueCC_chargeKaon: .." << NueFromNueCC_chargeKaon -> Integral() << "\n";
      std::cout << "  NueFromNueCC_neutKaon: ...." << NueFromNueCC_neutKaon -> Integral() << "\n";
      std::cout << "  NueFromEScatter: .........." << NueFromEScatter -> Integral() << "\n";
      std::cout << "  NueFromNC_pi0: ............" << NueFromNC_pi0 -> Integral() << "\n";
      std::cout << "  NueFromNC_delta0: ........." << NueFromNC_delta0 -> Integral() << "\n";
      std::cout << "  NueFromNumuCC: ............" << NueFromNumuCC -> Integral() << "\n";
      std::cout << "  Dirt: ....................." << Dirt -> Integral() << "\n";
      std::cout << "  Other: ...................." << Other -> Integral() << "\n";
      std::cout << "  NueFromCosmics: ..........." << NueFromCosmics -> Integral() << "\n";
      if (includeFosc){
      std::cout << "  SignalNu: ................." << SignalNu -> Integral() << "\n";
      }

      // if needed, print out the backgrounds here:
      bool printData = true;
      if (printData){
        TString fileNamePrint = fileSource;
        fileNamePrint += "/text/";
        fileNamePrint += fileNameRoot;

        char fileName[200];
        if (useHighDm) 
          sprintf(fileName, "%s%s_%s_highdm2.txt", fileNamePrint.Data(), baselines[j].c_str(), mode.c_str());
        else if (useGlobBF) 
          sprintf(fileName, "%s%s_%s_globBF.txt", fileNamePrint.Data(), baselines[j].c_str(), mode.c_str());
        else sprintf(fileName, "%s%s_%s.txt", fileNamePrint.Data(), baselines[j].c_str(), mode.c_str());
        std::cout << "nueBins.size(): " << nueBins.size() << std::endl;
        
        std::ofstream dataFile;
        dataFile.open(fileName);
        dataFile << std::setprecision(5) << "low\thigh\tmu\tk+\tk0\tnuscat\tpi0\t1gam"
                 << "\tnumucc\tdirt\tcosmic\tother\tsignal\n";
        for (int bin = 1; bin <= nbins_nue; bin++){
          dataFile  << std::setprecision(4)
                    << nueBins.at(bin - 1) << "\t" << nueBins.at(bin) << "\t"
                    << NueFromNueCC_muon       -> GetBinContent(bin) << "\t"
                    << NueFromNueCC_chargeKaon -> GetBinContent(bin) << "\t"
                    << NueFromNueCC_neutKaon   -> GetBinContent(bin) << "\t"
                    << NueFromEScatter         -> GetBinContent(bin) << "\t"
                    << NueFromNC_pi0           -> GetBinContent(bin) << "\t"
                    << NueFromNC_delta0        -> GetBinContent(bin) << "\t"
                    << NueFromNumuCC           -> GetBinContent(bin) << "\t"
                    << Dirt                    -> GetBinContent(bin) << "\t"
                    << NueFromCosmics          -> GetBinContent(bin) << "\t"
                    << Other                   -> GetBinContent(bin) << "\t";
          if (includeFosc)
            dataFile << SignalNu               -> GetBinContent(bin) << "\n";
          else
            dataFile << "0.0\n";
        }
        std::cout << "nueBins.size(): " << nueBins.size() << std::endl;

      }


      for(int bin = 0; bin < nbins_nue; bin++)
      {
        NueFromNueCC_muon       -> SetBinContent(bin+1,
                                      NueFromNueCC_muon->GetBinContent(bin+1)
                                      /NueFromNueCC_muon->GetBinWidth(bin+1));
        NueFromNueCC_chargeKaon -> SetBinContent(bin+1,
                                      NueFromNueCC_chargeKaon->GetBinContent(bin+1)
                                      /NueFromNueCC_chargeKaon->GetBinWidth(bin+1));
        NueFromNueCC_neutKaon   -> SetBinContent(bin+1,
                                      NueFromNueCC_neutKaon->GetBinContent(bin+1)
                                      /NueFromNueCC_neutKaon->GetBinWidth(bin+1));
        NueFromEScatter         -> SetBinContent(bin+1,
                                      NueFromEScatter->GetBinContent(bin+1)
                                      /NueFromEScatter->GetBinWidth(bin+1));
        NueFromNC_pi0           -> SetBinContent(bin+1,
                                      NueFromNC_pi0->GetBinContent(bin+1)
                                      /NueFromNC_pi0->GetBinWidth(bin+1));
        NueFromNC_delta0        -> SetBinContent(bin+1,
                                      NueFromNC_delta0->GetBinContent(bin+1)
                                      /NueFromNC_delta0->GetBinWidth(bin+1));
        NueFromNumuCC           -> SetBinContent(bin+1,
                                      NueFromNumuCC->GetBinContent(bin+1)
                                      /NueFromNumuCC->GetBinWidth(bin+1));
        Dirt                    -> SetBinContent(bin+1,
                                      Dirt->GetBinContent(bin+1)
                                      /Dirt->GetBinWidth(bin+1));
        Other                   -> SetBinContent(bin+1,
                                      Other->GetBinContent(bin+1)
                                      /Other->GetBinWidth(bin+1));
        if (includeFosc){
          SignalNu              -> SetBinContent(bin+1,
                                      SignalNu->GetBinContent(bin+1)
                                      /SignalNu->GetBinWidth(bin+1));
        }
        if (includeCosmics){
          NueFromCosmics        -> SetBinContent(bin+1,
                                      NueFromCosmics->GetBinContent(bin+1)
                                      /NueFromCosmics->GetBinWidth(bin+1));
        }
      }



      // Set the bin errors to zero except for the very last bin:
      for (int i = 0; i < nbins_nue; ++i)
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
        NueFromCosmics      -> SetBinError(i+1, 0.0);
        if (includeCosmics)
          totalEvents[i]      += NueFromCosmics -> GetBinContent(i+1);
        if (includeFosc)
          signalEvents[i]     = SignalNu -> GetBinContent(i+1);
      }



      std::cout << "Number of bins is " << nbins_nue << std::endl;
      //Now set the errors on the last hist going in:
      for (int i = 0; i < nbins_nue; ++i)
      {
        // double error = sqrt(totalEvents[i]);
        double error = sqrt(totalEvents[i] / (nueBins[i+1] - nueBins[i]));
        std::cout << "error " << i << " is " << error << std::endl;
        if (includeCosmics) NueFromCosmics -> SetBinError(i+1,error);
        else
          NueFromNumuCC -> SetBinError(i+1, error);
        if (includeFosc){
          SignalNu->SetBinContent(i+1, totalEvents[i]+signalEvents[i]);
          SignalNu->SetBinError(i+1, sqrt(signalEvents[i]/ (nueBins[i+1] - nueBins[i])));
        }
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
      if (includeCosmics) stack -> Add(NueFromCosmics);

      if (includeFosc){
        SignalNu -> SetLineStyle(0);
        SignalNu -> SetLineColor(1);
        SignalNu -> SetLineWidth(2);
      }

      double integral = 0;
      integral += NueFromNueCC_muon->Integral();
      integral += NueFromNueCC_chargeKaon->Integral();
      integral += NueFromNueCC_neutKaon->Integral();
      integral += NueFromNC_pi0->Integral();
      integral += NueFromNumuCC->Integral();
      integral += NueFromCosmics -> Integral();

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
      leg->AddEntry(NueFromNC_pi0, "NC Single  #gamma");
      // leg->AddEntry(NueFromNC_delta0, "#Delta #rightarrow N#gamma");
      leg->AddEntry(NueFromNumuCC, "#nu_{#mu} CC");
      if (includeCosmics) {
        leg->AddEntry(NueFromCosmics, "Cosmics");
        std::cout << "For some reason, include cosmics was true!  hard stop.\n";
        exit(-1);
      }
      // leg->AddEntry(Dirt, "Dirt");
      // leg->AddEntry(Other, "Other");
      if (includeFosc)
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
                                  0.0,emax,1.4*max);
                                  // 0.0,emax,1200);
                                  // 0.0,emax,25000);
      
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
      
      chr->GetYaxis()->SetTitle("Events / MeV");
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
      if(includeFosc){
        if (useHighDm) 
          plotUtils.add_plot_label( (char*)"Signal: ( #Deltam^{2} = 50 eV^{  2}, sin^{2} 2#theta_{#mue} = 0.003)", 0.2, 0.81, 0.04, 1,62,12);
        else if (useGlobBF)
          plotUtils.add_plot_label( (char*)"Signal: ( #Deltam^{2} = 0.43 eV^{  2}, sin^{2} 2#theta_{#mue} = 0.013)", 0.2, 0.81, 0.04, 1,62,12);
          // plotUtils.add_plot_label( (char*)"Signal: MiniBooNE Excess", 0.2, 0.81, 0.04, 1,62,12);
        else  plotUtils.add_plot_label( (char*)"Signal: ( #Deltam^{2} = 1.2 eV^{  2}, sin^{2} 2#theta_{#mue} = 0.003)", 0.2, 0.81, 0.04, 1,62,12);
      }
      plotUtils.add_plot_label((char*)"Statistical Uncertainty Only",0.2,0.76, 0.04,1,62,12);
      plotUtils.add_plot_label((char*)"PRELIMINARY",0.2,0.71, 0.04, kRed-3,62,12);


      // this is temporary:
      char temp[100];
      sprintf(temp,"Vertex Activity: %g MeV",1000*minVertexEnergyPhoton);
      plotUtils.add_plot_label(temp,.65,0.45,0.04,1,62,12);
      sprintf(temp,"Shower Gap: %g cm",minShowerGap);
      plotUtils.add_plot_label(temp,.65,0.4,0.04,1,62,12);

      stack -> Draw("E0 hist same");
      if (includeFosc){
        SignalNu -> Draw("E0 hist same");
        // SignalNu -> Draw("E0 same");
      }
      stack -> Draw("E0 same ");




      // stack -> Draw(" same ");
      leg->Draw();

    }
    
    TString plotName = fileSource;
    plotName += "/event_rates/";
    plotName += fileNameRoot;


    if (specialNameText != "") plotName = plotName + specialNameText + "_";
    if (specialNameTextOsc != "") plotName = plotName + specialNameTextOsc + "_";


    for (int i = 0; i < nL; i++)
    {
      char fileName[200];
      if (useHighDm) 
        sprintf(fileName, "%s%s_%s_highdm2.pdf", plotName.Data(), baselines[i].c_str(), mode.c_str());
      else if (useGlobBF) 
        sprintf(fileName, "%s%s_%s_globBF.pdf", plotName.Data(), baselines[i].c_str(), mode.c_str());
      else sprintf(fileName, "%s%s_%s.pdf", plotName.Data(), baselines[i].c_str(), mode.c_str());
      if (savePlots) stackedCanvas[i] -> Print(fileName, "pdf");
      if (useHighDm) 
        sprintf(fileName, "%s%s_%s_highdm2.png", plotName.Data(), baselines[i].c_str(), mode.c_str());
      else if (useGlobBF) 
        sprintf(fileName, "%s%s_%s_globBF.png", plotName.Data(), baselines[i].c_str(), mode.c_str());
      else sprintf(fileName, "%s%s_%s.png", plotName.Data(), baselines[i].c_str(), mode.c_str());
      if (savePlots) stackedCanvas[i] -> Print(fileName, "png");
      // stackedCanvas[i] -> Print(fileName, "eps");
    }
   
    return 0;

  }

  int NueAppearanceFitter::MakeAltSensPlot()
  {

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

    TString path = fileSource + "/chi2/";
    TString chi2FileName = utils.GetChi2FileName( path,
                                                  detNamesString,
                                                  includeNumus,
                                                  covMatrixList,
                                                  covMatrixListSource,
                                                  absolute_MWSource);

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






