

#include "NueAppearanceFitter.hh"

namespace lar1{

  NueAppearanceFitter::NueAppearanceFitter(){
    //---------------Start of section you can alter to change the baselines:

    //Self explanatory, this is where the ntuples you want to use are located:
    fileSource="/Users/cja33/Desktop/LAr1ND/lar1Repos/lar1/MC2_proposal_ntuples/";
    //Note: parts of this routine will write to that same folder, 
    //make sure its writable!

    //Set up the bins for the vector:
    //coming soon...


    //verbose causes print outs of information that is good for checking
    verbose = false;  
    //debug prints out info to make sure root is behaving...
    debug   = false;  

    specialNameText = "";
    specialNameTextOsc = "";
    specialNameText_far = "";
    specialNameTextOsc_far = "";

    useHighDm=false;
    useGlobBF=false;

    fileNameRoot = fileSource;
    fileNameRoot += "nue_appearance_";
    flatSystematicError = 0.20;  // Only used if nearDetStats = false.

    mode = "nu";  //beam mode to run in
    use100m = true;      //Include the detector at 100m?
    use100mLong=false;
    use470m = false;      //Include the detector at 470m?
    use700m = false;     //Include the detector at 700m?
    useT600 = false;
     
    forceRemake = false;

    //Note: there is no infrastructure to handle more than 3 baselines.
    //So, try at your own risk!
    
    ubooneScale = 1.0;       //Scale the event rates (uniformly across all events), uboone
    LAr1NDScale = (1.0); //Scale the event rates (uniformly across all events), near det
    LAr1FDScale = (1.0);     //Scale the event rates (uniformly across all events), far det
    
    energyType = "ecalo1";
    fileNameRoot += energyType;
    fileNameRoot += "_";
    // Options are etrue, eccqe, ecalo1, ecalo2;

    //How many points in the final graph do you want?  (symmetric in x and y)
    //More points means longer run time but better graphs
    //Note: most of the run time is in looping over ntuples, which only takes awhile
    //on the very first pass!  (subsequent runs are much faster)
    npoints = 250;
    
    //grid boundaries
    
    useNearDetStats = true;           // Only matters if the covariance matrix vector is empty.
    shapeOnlyFit = false;              // Only matters with near detd stats = true
    nearDetSystematicError = 0.20;  // Only matters if userNearDetStats = true
    // std::vector<std::string>  cov_max_name;
    //cov_max_name.push_back(mode+"_covariance_matrix/output_fraccovmx_xsec.out");
  }

  int NueAppearanceFitter::Prepare(){

    if (use700m && useT600){
        std::cout << "Error: can't use both LAr1-FD and T600.";
        return -1;
    }
    if (use100m && use100mLong){
        std::cout << "Error: can't use both LAr1-ND and 2*LAr1-ND.";
        return -1;
    }


    // char  label1[200], label2[200];

    if (mode == "nu"){
        if (energyType == "etrue")  sprintf(label1, "#nu mode, CC events");
        if (energyType == "eccqe")  sprintf(label1, "#nu mode, CCQE events");
        if (energyType == "ecalo1") sprintf(label1, "#nu mode, CC events");
        if (energyType == "ecalo2") sprintf(label1, "#nu mode, CC events");
    }
    else if (mode == "nubar"){
        if (energyType == "etrue")  sprintf(label1, "#bar{#nu} mode, CC events");
        if (energyType == "eccqe")  sprintf(label1, "#bar{#nu} mode, CCQE events");
        if (energyType == "ecalo1") sprintf(label1, "#bar{#nu} mode, CC events");
        if (energyType == "ecalo2") sprintf(label1, "#bar{#nu} mode, CC events");
    }
    else {
      std::cerr << "NO mode selected!!!\n";
      return -1;
    }

    if (energyType == "etrue")  sprintf(label2, "True Energy");
    if (energyType == "eccqe")  sprintf(label2, "CCQE Energy");
    if (energyType == "ecalo1") sprintf(label2, "Calo Energy (no neutrons)");
    if (energyType == "ecalo2") sprintf(label2, "Calo Energy (neutrons)");

    // std::vector<int> baselines;
    // std::vector<double> scales;
    // std::vector<std::string> names;
    // std::vector<double> volume;
    // std::vector<NtupleReader> readerNue;
    // std::vector<NtupleReader> readerNumu;
  
    //initialize all the baselines we'll use:
    if (use100m) baselines.push_back(100);
    if (use100mLong) baselines.push_back(100);
    if (use470m) baselines.push_back(470);
    if (use700m || useT600) baselines.push_back(700);
    //and they're corresponding scaling factors:
    if (use100m) scales.push_back(LAr1NDScale);
    if (use100mLong) scales.push_back(LAr1NDScale);
    if (use470m) scales.push_back(ubooneScale);
    if (use700m) scales.push_back(LAr1FDScale);
    if (useT600) scales.push_back(1.0);
    //some detector names just for fun:
    if (use100m) names.push_back("LAr1-ND");
    if (use100mLong) names.push_back("2*LAr1-ND");
    if (use470m) names.push_back("MicroBooNE");
    if (use700m) names.push_back("LAr1-FD");
    if (useT600) names.push_back("ICARUS");
    //fiducial volume FROM MC, also just for fun: (scale to get target volume)
    if (use100m) volume.push_back(40);
    if (use470m) volume.push_back(61.4);
    if (use700m) volume.push_back(1000);
    if (useT600) volume.push_back(476);
    if (use100mLong) volume.push_back(80);
    if (use100m) fileNameRoot += "ND_";
    if (use100mLong) fileNameRoot += "2ND_";
    if (use470m) fileNameRoot += "uB_";
    if (use700m) fileNameRoot += "FD_";
    if (useT600) fileNameRoot += "IC_";
    
    if (use100m){
      NtupleReader a("nue",fileSource, 100, mode, energyType, npoints, forceRemake);
      a.setSpecialNameText(specialNameText);
      a.setSpecialNameTextOsc(specialNameTextOsc);
      readerNue.push_back(a);
      a.setSignal("numu");
      a.setSpecialNameText(specialNameText);
      // a.setSpecialNameText(specialNameTextOsc);
      readerNumu.push_back(a);
    }
    if (use100mLong){
      NtupleReader a("nue",fileSource, 100, mode, energyType, npoints, forceRemake);
      a.setSpecialNameText("long");
      a.setSpecialNameTextOsc("long");
      readerNue.push_back(a);
      a.setSignal("numu");
      // a.setSpecialNameText("");
      a.setSpecialNameTextOsc("long");
      // a.setSpecialNameText(specialNameTextOsc);
      readerNumu.push_back(a);
    }
    if (use470m){
      NtupleReader a("nue",fileSource, 470, mode, energyType, npoints, forceRemake);
      // a.setSpecialNameText(specialNameText);
      a.setSpecialNameTextOsc(specialNameTextOsc);
      readerNue.push_back(a);
      a.setSignal("numu");
      // a.setSpecialNameText("");
      a.setSpecialNameTextOsc("");
      // a.setSpecialNameText(specialNameTextOsc);
      readerNumu.push_back(a);
    }
    if (use700m) {
      NtupleReader a("nue",fileSource, 700, mode, energyType, npoints, forceRemake);
      a.setSpecialNameText(specialNameText);
      a.setSpecialNameTextOsc(specialNameTextOsc);
      readerNue.push_back(a);
      a.setSignal("numu");
      a.setSpecialNameText("");
      a.setSpecialNameTextOsc("");
      // a.setSpecialNameText(specialNameTextOsc);
      readerNumu.push_back(a);
    }
    if (useT600) {
      NtupleReader a("nue",fileSource, 700, mode, energyType, npoints, forceRemake);
      specialNameText_far.append("IC");
      specialNameTextOsc_far.append("IC");
      a.setSpecialNameText(specialNameText_far);
      a.setSpecialNameTextOsc(specialNameTextOsc_far);
      readerNue.push_back(a);
      a.setSignal("numu");
      a.setSpecialNameText("");
      a.setSpecialNameTextOsc("");
      // a.setSpecialNameText(specialNameTextOsc);
      readerNumu.push_back(a);
    }
    //This value is the number of baselines:
    nL = baselines.size();
    nbins = nL*nbinsE*3;    //total number of energy bins for side-by-side distributions:
    //just in case, if the number of baselines is 1 switch off nearDetStats:
    if (nL == 1) {
        useNearDetStats=false;
        shapeOnlyFit = false;
        //flatSystematicError = nearDetSystematicError; 
    }
    if (!useNearDetStats) shapeOnlyFit = false;

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
        Int_t L = baselines[i];
        Int_t returnVal;
        returnVal = readerNue[i].processData();
        if (returnVal) {  //only returns 0 on successful completion
            std::cout << "Error: failed to read the ntuple at " << L << " with error " << returnVal;
            std::cout << std::endl;
            return 1;
        }
        returnVal = readerNumu[i].processData();
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
    DirtVec.resize(nL);
    OtherVec.resize(nL);

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
      // Since this is the event rate calc function, gotta get all the goodies:
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
        eventsnLVec[point] = utils.appendVectors( utils.appendVectors(eventsnueoscVec[0][point], eventsnueVec[0], eventsnumuVec[0]),
                          utils.appendVectors(eventsnueoscVec[1][point], eventsnueVec[1], eventsnumuVec[1]));
        eventsnLfitVec[point] = eventsnLVec[point];
        //make them into histograms, just for display:
        // eventsnL[point]   = utils.makeHistogram(eventsnLVec[point]    , 0 , nbins);
        // eventsnLfit[point]  = utils.makeHistogram(eventsnLfitVec[point] , 0 , nbins);
        if (point == 0){
          eventsnLcvVec   = utils.appendVectors(utils.appendVectors(eventsnuefoscVec[0] , eventsnueVec[0], eventsnumuVec[0]),
                          utils.appendVectors(eventsnuefoscVec[1] , eventsnueVec[1], eventsnumuVec[1]));
          eventsnLnullVec = utils.appendVectors(utils.appendVectors(blankVector     , eventsnueVec[0], eventsnumuVec[0]),
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
    for (auto & row : fracentries) row.resize(nbins);
    //initialized to zero
  
    if (cov_max_name.size()) utils.buildCovarianceMatrix(fracentries, cov_max_name, nbins);
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
    if (!cov_max_name.size()){
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


    std::cout << std::endl;
    //now either the covariance matrix is set, or the frac entries is filled.
    //nearDetStats is only nonezero size if the cov_max_name is empty
    
    //This vector holds the null oscillation collapsed signal (in other words,
    //it's the nue and numu appended).  It's used in the chisq calc.
    nullVec.resize(nbins-nL*nbinsE);
    // nullVec = utils.collapseVector(eventsnLnullVec, nbinsE, nL);

    
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


    //This loop is actually doing the chisq calculation
    //It loops over npoints twice.  First over dm2, and second over sin22th
    for (int dm2point = 0; dm2point <= npoints; dm2point ++){
      std::cout << "dm^2: " << dm2point << " of " << npoints << std::endl;      
      for (int sin22thpoint = 0; sin22thpoint <= npoints; sin22thpoint++){
        chisq = 0.0; //reset the chisq!

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
        std::vector< float > eventsnLVecTemp = eventsnLVec[dm2point];

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

        // Here's where the shape only stuff comes into play
        double intSignalND = 0.;    double intNooscND = 0.;
        std::vector<double> scalefac;

        predictionVec = utils.collapseVector(eventsFitVecTemp, nbinsE, nL);
        // Now predictionVec and nullVec hold 
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
        if (shapeOnlyFit)
        {
            for (int i = 0; i < nbinsE*2; ++i)
            {
                nearDetStats[i] = 1/sqrt( nullVec.at(i+nbinsE) );
            }
        }

        //initialize the covariance matrix array:
        std::vector< std::vector<float> > entries( nbins, std::vector<float> ( nbins, 0.0 ) );

        
        if (cov_max_name.size() == 0){
          //Only dealing with diagonal entries, just loop over them:
          for (int ibin = 0; ibin < nbins; ibin++){
            //start by getting the central value for this entry
            // getting the central value depends on whether or not shape only
            // if not shape only, it's easy:
            double cvi = eventsnLVecTemp[ibin];
            if (shapeOnlyFit)
            {
              if (ibin%(3*nbinsE) < nbinsE) //on a signal bin
              {
                int index = ibin - ((int) ibin/(3*nbinsE))*nbinsE;
                // std::cout << "ibin is: " << ibin << " and accessing nullvec at "
                // << index;
                cvi = abs( predictionVec.at(index) - nullVec.at(index) );
                // std::cout << ". cvi is prediction - nullvec: " << cvi << std::endl;
                if (cvi > 10e10){
                    std::cout << "\tprediction is " << predictionVec[ibin] << std::endl;
                    std::cout << "\tnullVec is " << nullVec.at(ibin - ((int) ibin/(3*nbinsE))*nbinsE) << std::endl;                                
                }
                 // Make the cvi the difference between prediction and null
              }
              else
              {
                // std::cout << "ibin is: " << ibin << " and accessing nullvec at "
                  // << ibin - nbinsE - ((int) ibin/(3*nbinsE))*nbinsE;
                cvi = nullVec.at(ibin - nbinsE - ((int) ibin/(3*nbinsE))*nbinsE);
                // std::cout << ". cvi is nullvec only: " << cvi << std::endl;
                // else using the null results.
              }
            }
            if (debug) std::cout << "ibin: " << ibin << ", cvi: "<< cvi << std::endl;
            //looking at the simulated signal for this if:
            if ( (ibin%(nbinsE*3)) < nbinsE ) cvi *= sin22th; //scale the oscillated signal
            
            if (useNearDetStats){ //using near detector stats
              if (ibin < 3*nbinsE){ //on the near detector
                entries[ibin][ibin] = cvi + pow(nearDetSystematicError*cvi, 2); 
                //statistical^2 + systematic^2 errors
              } // end if on "is near det"
              
              else{ //on far detectors
                entries[ibin][ibin] = cvi + pow(nearDetStats[ibin % (3*nbinsE)]*cvi, 2);
              } // end else, end section on far dets
            } //end if on useNearDetStats
            
            else{ //using flat systematics on everything
              entries[ibin][ibin] = cvi + pow(flatSystematicError*cvi, 2);
            } //end else on 5% flat stats
          
            if (isinf(entries[ibin][ibin])) {
                std::cout << "ibin is " << ibin << std::endl;
                debug = true;
                // return -1;
            }

          } //end loop over bins, is really loop over diagonal entries
        } //end if on "cov_max_name.size() == 0"      
        else{ //use the full blown covariance matrix.  Loop over every entry.
          //Can't forget to add in the statistical errors on the diagonal!
          for (int ibin = 0; ibin < nbins; ibin++){
            for (int jbin = 0; jbin < nbins; jbin ++){
              double cvi = eventsnLVecTemp[ibin];
              double cvj = eventsnLVecTemp[jbin];
              if ( (ibin%(nbinsE*3)) <nbinsE ) cvi *= sin22th; //scale the oscillated signal
              if ( (jbin%(nbinsE*3)) <nbinsE ) cvj *= sin22th; //scale the oscillated signal

              //Now scale the entries:
              entries[ibin][jbin] = fracentries[ibin][jbin]*cvi*cvj;
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




              // Make some plots:
              if (shapeOnlyFit && dm2point == (int) npoints*0.9 )
              {
                  // if (sin22thpoint == (int) npoints/2)
                  // {
                  //     std::cout << "Using dm2: " << dm2 << std::endl;
                  //     std::cout << "Using sin22th: " << sin22th << std::endl;
                  //     // std::cout << "Scalefac: " << scalefac << std::endl;
                  //     std::vector<float> tempVec = utils.collapseVector(eventsnLnullVec, nbinsE, nL);
                  //     // TH1F * unscaled = makeHistogram(tempVec, 0, nbinsE);
                  //     // TH1F * background = makeHistogram(nullVec, 0, nbinsE);
                  //     // TH1F * sig = makeHistogram(predictionVec, 0, nbinsE);
                  //     unscaled -> SetLineColor(1);
                  //     background -> SetLineColor(2);
                  //     sig -> SetLineColor(4);

                  //     TLegend * scaleLegend = new TLegend(0.65,0.55,0.82,0.75);
                  //     scaleLegend->SetFillStyle(0);
                  //     scaleLegend->SetFillColor(0);
                  //     scaleLegend->SetBorderSize(0);
                  //     scaleLegend->SetTextSize(0.025);
                  
                  //     // scaleLegend -> AddEntry(unscaled, "Unscaled events");
                  //     // scaleLegend -> AddEntry(background, "background events");
                  //     // scaleLegend -> AddEntry(sig, "scaled events");

                  //     // TCanvas * c65 = new TCanvas("c65","Stupid",800,400);
                  //     // c65 -> cd();
                  //     // unscaled ->Draw();
                  //     // background ->Draw("same");
                  //     // sig -> Draw("same");
                  //     // scaleLegend -> Draw();
                  // }
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
                              std::cout << " M^-1: " << (*cov)(ibin-1,jbin-1) << std::endl;
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
              
          if (verbose) std::cout << "dm2: " << dm2 << ",\tsin22th: " << sin22th << ",\tchisq: " << chisq << std::endl;    
          if (debug) std::cout << "\n\n";
          if (debug) std::cout << "---------------------------End dm2: " << dm2 << ",\tsin22th: " << sin22th << std::endl;
          }// end loop over sin22thpoints
      } // end loop over dm2points
    
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
    
    TH1::AddDirectory(kFALSE);
    TGraph *sens90 = new TGraph(npoints+1,x90,y90); 
    TGraph *sens3s = new TGraph(npoints+1,x3s,y3s); 
    TGraph *sens5s = new TGraph(npoints+1,x5s,y5s);

    //Plot Results:
    sens90->SetLineColor(1); sens90->SetLineWidth(2);
    sens3s->SetLineColor(9); sens3s->SetLineWidth(2);
    sens5s->SetLineColor(9); sens5s->SetLineStyle(2); sens5s->SetLineWidth(1);
    
    TString chi2FileName = fileNameRoot+mode;
    if(shapeOnlyFit){
        if (useNearDetStats){
            chi2FileName = chi2FileName+"_nearDetStats_shapeOnly_chi2.root";
        }
        else{
            chi2FileName = chi2FileName+"_flatStats_shapeOnly_chi2.root";
        }
    }
    else{
        if (useNearDetStats){
            chi2FileName = chi2FileName+"_nearDetStats_chi2.root";
        }
        else{
            chi2FileName = chi2FileName+"_flatStats_chi2.root";
        }
    }

    std::cout << "Writing chi2 to " << chi2FileName << std::endl;

    //write the results to file:
    TFile *file1 = new TFile(chi2FileName,"RECREATE");
    chi2->Write();
    sens90->Write();
    // sens99->Write();
    sens3s->Write();
    sens5s->Write();
    file1->Close();
    
        
    //======================================================
    printf("\nDrawing sensitivity curves...\n");

    std::cout<<"Drawing LSND intervals...\n\n";

    TCanvas* c3 = new TCanvas("c3","Sensitivity",700,700);
    c3->SetLogx();
    c3->SetLogy();

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
    plotUtils.lsnd_plot(c3);

    //======================================================

    TLegend* legt=new TLegend(0.68,0.55,0.86,0.72);
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
        if (baselines[j] == 100){
            if (use100m)
                sprintf(name[j], "LAr1-ND (%im)", baselines[j]);
            else if (use100mLong)
                sprintf(name[j], "2*LAr1-ND (%im)", baselines[j]);
        }
        else if (baselines[j] == 470){
            sprintf(name[j], "MicroBooNE (%im)", baselines[j]);
        }
        else if (baselines[j] == 700){
            if (use700m) sprintf(name[j], "LAr1-FD (%im)", baselines[j]);
            else if (useT600) sprintf(name[j], "T600 (%im)", baselines[j]);
        }
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
    leg3->AddEntry(gdummy0,"Global Fit 90% CL (J. Kopp et al. arXiv:1303.3011)");   
    leg3->Draw();
    
    TImage *img = TImage::Create();
    img -> FromPad(c3);
    if (specialNameText_far != "") fileNameRoot = fileNameRoot + specialNameText_far + "_";
    if (specialNameTextOsc_far != "") fileNameRoot = fileNameRoot + specialNameTextOsc_far + "_";
    if(shapeOnlyFit){
        if (useNearDetStats){
            c3 -> Print(fileNameRoot+mode+"_nearDetStats_shapeOnly.pdf", "pdf");
            c3 -> Print(fileNameRoot+mode+"_nearDetStats_shapeOnly.png", "png");
        }
        else{
            c3 -> Print(fileNameRoot+mode+"_flatStats_shapeOnly.pdf", "pdf");
            c3 -> Print(fileNameRoot+mode+"_flatStats_shapeOnly.png", "png");
        }
    }
    else{
      if (useNearDetStats){
            c3 -> Print(fileNameRoot+mode+"_nearDetStats.pdf", "pdf");
            c3 -> Print(fileNameRoot+mode+"_nearDetStats.png", "png");
        }
        else{
            c3 -> Print(fileNameRoot+mode+"_flatStats.pdf", "pdf");
            c3 -> Print(fileNameRoot+mode+"_flatStats.png", "png");
        }
    }
    std::cout<<"\nEnd of routine.\n";
    
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
      

      std::cout << " NueFromNueCC_muonVec[j].front() is " << NueFromNueCC_muonVec[j].front() << std::endl;
      std::cout << " NueFromNueCC_chargeKaonVec[j].front() is " << NueFromNueCC_chargeKaonVec[j].front() << std::endl;
      std::cout << " NueFromNueCC_neutKaonVec[j].front() is " << NueFromNueCC_neutKaonVec[j].front() << std::endl;
      std::cout << " NueFromEScatterVec[j].front() is " << NueFromEScatterVec[j].front() << std::endl;
      std::cout << " NueFromNC_pi0Vec[j].front() is " << NueFromNC_pi0Vec[j].front() << std::endl;
      std::cout << " NueFromNC_delta0Vec[j].front() is " << NueFromNC_delta0Vec[j].front() << std::endl;
      std::cout << " NueFromNumuCCVec[j].front() is " << NueFromNumuCCVec[j].front() << std::endl;
      std::cout << " DirtVec[j].front() is " << DirtVec[j].front() << std::endl;
      std::cout << " OtherVec[j].front() is " << OtherVec[j].front() << std::endl;

      
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


      NueFromNueCC_muon   -> SetFillColor(30);
      NueFromNueCC_chargeKaon -> SetFillColor(31);
      NueFromNueCC_neutKaon -> SetFillColor(32);
      NueFromEScatter     -> SetFillColor(33);
      NueFromNC_pi0     -> SetFillColor(41);
      NueFromNC_delta0    -> SetFillStyle(41);
      NueFromNumuCC     -> SetFillColor(40);
      Dirt          -> SetFillColor(15);
      Other         -> SetFillColor(8);

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
      //stack -> Add(NueFromEScatter);
      stack -> Add(NueFromNC_pi0);
      // stack -> Add(NueFromNC_delta0);
      stack -> Add(NueFromNumuCC);
      // stack -> Add(Dirt);
      // stack -> Add(Other);
      // stack ->Add(MBPhotExcess);


      // stack -> Add(SignalNu);

      //set up the legend
      leg = new TLegend(0.75,0.5,.95,0.9);
      leg->AddEntry(NueFromNueCC_muon, "#mu #rightarrow #nu_{e}");
      leg->AddEntry(NueFromNueCC_chargeKaon, "k^{+} #rightarrow #nu_{e}");
      leg->AddEntry(NueFromNueCC_neutKaon, "k^{0} #rightarrow #nu_{e}");
      //leg->AddEntry(NueFromEScatter, "#nu - e^{-}");
      leg->AddEntry(NueFromNC_pi0, "NC #pi^{0}");
      // leg->AddEntry(NueFromNC_delta0, "#Delta #rightarrow N#gamma");
      leg->AddEntry(NueFromNumuCC, "#nu_{#mu} CC");
      // leg->AddEntry(Dirt, "Dirt");
      // leg->AddEntry(Other, "Other");
      leg->AddEntry(SignalNu,"Signal");

      leg -> SetTextSize(0.04);



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



      TH1F *chr = stackedCanvas[j]->DrawFrame(emin-0.01,-0.01*(SignalNu->GetMaximum()),emax,1.0*max);
      
      //chr->GetYaxis()->SetLabelOffset(999);
      chr->GetYaxis()->SetTitle("Events");
      chr->GetYaxis()->SetTitleSize(0.06);
      chr->GetYaxis()->SetTitleOffset(.7);
      char name[200];
      if (baselines[j] == 100){
        sprintf(name, "LAr1-ND (%im)", baselines[j]);
      }
      else if (baselines[j] == 470){
        sprintf(name, "MicroBooNE (%im)", baselines[j]);
      }
      else if (baselines[j] == 700){
        if (use700m) sprintf(name, "LAr1-FD (%im)", baselines[j]);
        if (useT600) sprintf(name, "ICARUS (%im)", baselines[j]);
      }

      
      chr->GetXaxis()->CenterTitle();
      chr->GetXaxis()->SetLabelSize(0.05);
      TString forComparison = fileSource;
      if (forComparison == "/Users/cja33/Desktop/lar1/CCQE/"){
        chr->GetXaxis()->SetTitle("E_{#nu}^{QE} (GeV)");
      }
      else chr->GetXaxis()->SetTitle("Energy (GeV)");


      chr->GetXaxis()->SetTitle("Reconstructed Neutrino Energy (GeV)");
      chr->GetXaxis()->SetTitleOffset(1);
      chr->GetXaxis()->SetTitleSize(0.07);
      chr->GetXaxis()->SetLimits(emin-0.01,emax);
      chr->Draw();
      SignalNu -> SetLineStyle(2);

      plotUtils.add_plot_label( name , 0.5, 0.85, 0.05, 1 );
      if (useHighDm) 
        plotUtils.add_plot_label( (char*)"signal: (#Deltam^{2} = 50 eV^{2}, sin^{2}2#theta_{#mue} = 0.003)", 0.5, 0.775, 0.04, 1);
      else if (useGlobBF)
        plotUtils.add_plot_label( (char*)"signal: (#Deltam^{2} = 0.43 eV^{2}, sin^{2}2#theta_{#mue} = 0.013)", 0.5, 0.775, 0.04, 1);
      else  plotUtils.add_plot_label( (char*)"signal: (#Deltam^{2} = 1.2 eV^{2}, sin^{2}2#theta_{#mue} = 0.003)", 0.5, 0.775, 0.04, 1);

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
        sprintf(fileName, "%s%i_%s_%s_highdm2.pdf", fileNameRoot.Data(), baselines[i], mode.c_str(),energyType.c_str());
      else if (useGlobBF) 
        sprintf(fileName, "%s%i_%s_%s_globBF.pdf", fileNameRoot.Data(), baselines[i], mode.c_str(),energyType.c_str());
      else sprintf(fileName, "%s%i_%s_%s.pdf", fileNameRoot.Data(), baselines[i], mode.c_str(),energyType.c_str());
      stackedCanvas[i] -> Print(fileName, "pdf");
      stackedCanvas[i] -> Print(fileName, "png");
    }
   
    return 0;

  }

}






