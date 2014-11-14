
#include "NtupleReader.hh"

namespace lar1{

  NtupleReader::NtupleReader(){

    //See up all the default arguments:
    signal = "nue";
    path = "../ntuples/";
    baseline = "470";
    beammode = "nu";
    forceRemake = false;
    energy = "true";
    npoints = 10;

    isScaling = false;
    scale = 1;
    specialNameText = "";

    usingMultiWeights = false;
    absolute_MWSource = false;
    nWeights = 1000;
    includeOsc=true;

    showerContainmentDist = -999;
    minDistanceToStart = 0;
    minVertexEnergySignal=0;
    minVertexEnergyPhoton=10000;
    minShowerGap=10000;

    std::vector<float> defaultBins;

    // Default to 10 equal length bins from 0.2 GeV to 3 GeV:
    double emin = 0.0;
    double emax = 3.0;
    double nbins = 120.0;
    double binSize = (emax - emin)/nbins;
    defaultBins.resize(nbins+1);
    for (double i = 0; i <= nbins; i++) {
      defaultBins.at(i) = emin + i*binSize;
    }

    setBins(defaultBins);

  }

  NtupleReader::NtupleReader( std::string input_signal,
                              std::string input_path,
                              std::string input_baseline,
                              std::string input_beammode,
                              std::string input_energy,
                              int input_npoints,
                              bool input_forceRemake){

    // Old and not useful default path
    if (path == "") path = "/uboone/data/users/LAr1/Combined_ntuples_weight/";

    signal = input_signal;
    path = input_path;
    baseline = input_baseline;
    beammode = input_beammode;
    energy = input_energy;
    forceRemake = input_forceRemake;
    npoints = input_npoints;

    isScaling = false;
    scale = 1;
    specialNameText = "";

    usingMultiWeights = false;
    absolute_MWSource = false;
    nWeights = 1000;
    includeOsc=true;

    showerContainmentDist = -999;
    minDistanceToStart = 0;
    minVertexEnergySignal=0;
    minVertexEnergyPhoton=10000;
    minShowerGap=10000;

    std::vector<float> defaultBins;
    // Default to 10 equal length bins from 0.2 GeV to 3 GeV:
    double emin = 0.0;
    double emax = 3.0;
    double nbins = 120.0;
    double binSize = (emax - emin)/nbins;
    defaultBins.resize(nbins+1);
    for (double i = 0; i <= nbins; i++) {
      defaultBins.at(i) = emin + i*binSize;
    }
    setBins(defaultBins);

  }
  void lar1::NtupleReader::useMultiWeights(bool b, int this_multiWeightSource)
  {
    usingMultiWeights = b;
    multiWeightSource = this_multiWeightSource;
    multiWeightData.resize(nWeights);
    for (int i = 0; i < nWeights; ++i)
    {
      multiWeightData[i].resize(bins.size()-1);
    }
    return;
  }

  void NtupleReader::setScale(double input_scale){
    isScaling = true;
    scale = input_scale;
  }

  void NtupleReader::setSpecialNameText(std::string s){
    specialNameText = s;
  }
  void NtupleReader::setSpecialNameTextOsc(std::string s){
    specialNameTextOsc = s;
  }

  void NtupleReader::setContainedShowers(double d){
    showerContainmentDist = d;
  }
  void NtupleReader::setMinDistanceToStart(double d){
    minDistanceToStart = d;
  }

  void NtupleReader::setTopologyCut(double MinVertexEnergy, double MinShowerGap){
    minVertexEnergyPhoton = MinVertexEnergy;
    minShowerGap = MinShowerGap;
  }

  void NtupleReader::setMinVertexEnergySignal(double MinVertexEnergySignal){
    minVertexEnergySignal = MinVertexEnergySignal;
  }

  //Not passing references here, since it's small and called infrequently
  std::vector<float> NtupleReader::GetData() const{

    if ( data.size() != 0 ) return data;
    else{
      std::vector<float> fakeData(bins.size(),0);
      return fakeData;
    }

  }
  std::vector<std::vector<float> > NtupleReader::GetDataOsc() const{

    if ( dataOsc.size() != 0 ) return dataOsc;
    else{
      std::vector<std::vector<float> > fakeData(npoints,std::vector<float>(bins.size()-1, 0));
      return fakeData;
    }

  }

  std::vector<std::vector< float> > NtupleReader::GetMultiWeightData() const{
    return multiWeightData;
  }



  bool NtupleReader::fileExists(const char *filename) const{
    ifstream ifile(filename);
    return ifile.is_open();
  }




  void NtupleReader::buildFileNameSource() {

    //Need to make the names of the files, nue and numu:
    TString Name  = "combined_ntuple_";
    Name += baseline;
    Name += "_";     //"combined_ntuple_470m_"
    Name += beammode; //"combined_ntuple_470m_nu"
    fileNameSource = Name;
    fileNameSourceOsc = Name;
    if (signal == "nue") {
      fileNameSourceOsc += "_fosc";
    }
    fileNameSource += "_processed_";
    if (specialNameText != "") fileNameSource += specialNameText + "_";
    if (isScaling && signal == "nue")
    {
      fileNameSource += "scale_";
      fileNameSource += scale;
      fileNameSource += "_";
    }
    fileNameSource += signal;
    fileNameSource += ".root";
    fileNameSourceOsc += "_processed_";
    if (specialNameTextOsc != "" ) fileNameSourceOsc += specialNameTextOsc + "_";
    if (signal == "nue" ) fileNameSourceOsc += "fosc";
    else if (signal == "numu") fileNameSourceOsc += "numu";
    fileNameSourceOsc += ".root";

    return;

  }

  void NtupleReader::buildFileNameHists() {

    //Template name: "sourceFile_emin_emax_energy_hists.root"
    //Template name: "sourceFile_emin_emax_npoints_energy_hists.root"

    // Variables used in naming:
    // double emin = bins.front();
    // double emax = bins.back();
    // int nbins = bins.size() - 1;
    char tempstring[100];
    //path to the file that holds histos:
    fileNameHists = "hists/";
    fileNameHists += fileNameSource;
    fileNameHistsOsc = "hists_osc/";
    fileNameHistsOsc += fileNameSourceOsc;
    fileNameHists += "_";
    if (signal == "nue"){
      if(showerContainmentDist != -999){
        sprintf(tempstring, "cont%g",showerContainmentDist );
        fileNameHists += tempstring;
        fileNameHists += "_";
        fileNameHistsOsc += "_";
        fileNameHistsOsc += tempstring;
      }
      if(minDistanceToStart != -999){
        sprintf(tempstring, "dist%g",minDistanceToStart );
        fileNameHists += tempstring;
        fileNameHists += "_";
        fileNameHistsOsc += "_";
        fileNameHistsOsc += tempstring;
      }
      if (minVertexEnergyPhoton != 10000){
        sprintf(tempstring, "vePhot%g",minVertexEnergyPhoton );
        fileNameHists += tempstring;
        fileNameHists += "_";
        // fileNameHistsOsc += "_";
        // fileNameHistsOsc += tempstring;
      }
      if (minShowerGap != 10000){
        sprintf(tempstring, "sg%g",minShowerGap );
        fileNameHists += tempstring;
        fileNameHists += "_";
        // fileNameHistsOsc += "_";
        // fileNameHistsOsc += tempstring;
      }
    }
    if (minVertexEnergySignal != 0){
      sprintf(tempstring, "veSig%g",minVertexEnergySignal );
      fileNameHists += tempstring;
      fileNameHists += "_";
      fileNameHistsOsc += "_";
      fileNameHistsOsc += tempstring;
    }
    // sprintf(tempstring, "%g", emin);
    // fileNameHists += tempstring;
    // fileNameHists += "_";
    // sprintf(tempstring, "%g_", emax);
    // fileNameHists += tempstring;
    if (usingMultiWeights){
      sprintf(tempstring, "%dw_%d", nWeights,multiWeightSource);
      fileNameHists += tempstring;
      if (absolute_MWSource)
        fileNameHists += "_abs_";
    }
    fileNameHists += energy;
    fileNameHists += "_hists.root";


    // fileNameHistsOsc += "_";
    // sprintf(tempstring, "%g", emin);
    // fileNameHistsOsc += tempstring;
    // fileNameHistsOsc += "_";
    // sprintf(tempstring, "%g", emax);
    // fileNameHistsOsc += tempstring;
    sprintf(tempstring, "_%dp_", npoints);
    fileNameHistsOsc += tempstring;

    fileNameHistsOsc += energy;
    fileNameHistsOsc += "_hists.root";



  }


  int NtupleReader::processData(){

    TString filePath=path;
    filePath += "/"; //just in case...
    // bool nueHistsFound  = false;
    // bool numuHistsFound = false;
    // bool foscHistsFound = false;

    if (bins.size() == 0 || bins.size() == 1){
      std::cout << "NtupleReader: Error: not enough bins." <<std::endl;
      exit(6);
    }

    buildFileNameSource();
    bool SourceFound = fileExists(filePath+fileNameSource);


    if (! (SourceFound) ){
      std::cout << "NtupleReader: Error: No input file found.  Searched for: " <<std::endl;
      std::cout << "\t" << fileNameSource <<std::endl;
      std::cout << "\tIn: " << filePath << std::endl;
      exit(2);
    }
    if (includeOsc){
      SourceFound = fileExists(filePath+fileNameSourceOsc);
      if (! (SourceFound) ){
        std::cout << "NtupleReader: Error: No input file found.  Searched for: " <<std::endl;
        std::cout << "\t" << fileNameSourceOsc <<std::endl;
        std::cout << "\tIn: " << filePath << std::endl;
        exit(2);
      }
    }

    //If the code gets to this point, there is an input!
    std::cout << "Good input files exist!" << std::endl;

    // Now see if the already read file exists.  If so, just read in that.
    // If not, make it.
    
    std::cout << "NtupleReader: Looking for the " << signal << " histogram files at L = " << baseline << "..." << std::endl;
    buildFileNameHists();

    // Handle the background histograms first:
    if (fileExists(filePath+fileNameHists) && !forceRemake){ //A file exists, make sure it's up to date
      std::cout << "\tFile " << fileNameHists << " found." << std::endl;
      // Check that the hists file is newer than the source file:
      if (!UpToDate(filePath+fileNameSource,filePath+fileNameHists)){
        std::cout << "\tThe hist file exists but isn't up to date.\n";
        int val(0);
        if (signal == "nue") val = LoopNue();
        if (signal == "numu") val = LoopNumu();
        if (val != 0) return val;
      }
      else{ // Everything is good, just read in the files:
        std::cout << "\tThe hist file exists and is up to date.\n";
        int val(0);
        if (signal == "nue") val = ReadNue();
        if (signal == "numu") val = ReadNumu();
        if (val != 0) return val;
      }
    }
    else { // No hists file exists, make it.
      std::cout << "\tNo hist file exists.\n";
      int val(0);
      if (signal == "nue") val = LoopNue();
      if (signal == "numu") val = LoopNumu();
      if (val != 0) return val;
    }

    // Now do the osc histograms
    if (fileExists(filePath+fileNameHistsOsc) && !forceRemake ){ //A file exists, make sure it's up to date
      // Check that the hists file is newer than the source file:
      if (!UpToDate(filePath+fileNameSourceOsc,filePath+fileNameHistsOsc)){
        int val(0);
        if (signal == "nue" && includeOsc) val = LoopNueOsc();
        // if (signal == "numu") val = LoopNumuOsc();
        if (val != 0) return val;
      }
      else{ // Everything is good, just read in the files:
        int val(0);
        if (signal == "nue"&& includeOsc) val = ReadNueOsc();
        // if (signal == "numu") val = ReadNumuOsc();
        if (val != 0) return val;
      }
    }
    else { // No hists file exists, make it.
      int val(0);
      if (signal == "nue"&& includeOsc) val = LoopNueOsc();
      // if (signal == "numu") val = LoopNumuOsc();
      if (val != 0) return val;
    }




      
    return 0;

  }

  bool NtupleReader::UpToDate(const char * sourceFile, const char * testFile) const{
    if (fileExists(sourceFile)){
      std::cout << "\tFound source file.  Verifying up to date." << std::endl;
      if (fileExists(testFile)){

        struct stat attrib;
        stat(testFile, &attrib);
        int timediff = attrib.st_mtime;
        stat(sourceFile, &attrib);
        timediff -= attrib.st_mtime;
        if (timediff > 0) {
          std::cout << "\tFile " << testFile << " is up to date." << std::endl;
          return true;
        }
        else {
          std::cout << "\tFile" << testFile << " is out of date, need to remake." << std::endl;
          return false;
        }
      }
      else {
        std::cout << "\tFile " << testFile << " not found." << std::endl;
        return false;
      }
    }
    else {
      std::cout << "Didn't find source file in UpToDate, return false." << std::endl;
      return false;
    }
  }

  // return true to (re) make the hists.  return false to reuse the ones that exist (faster)
  bool NtupleReader::remakeHists() const{

    if (forceRemake){
      std::cout << "\tForce remake set to true, rebuilding the hist file for " << signal << std::endl;
      return true;
    }
    else 
      std::cout << "\tLooking for file " <<  fileNameHists << std::endl;
    


    if (fileExists(path+fileNameHists)) {
      std::cout << "\tFound histogram file.  Verifying up to date." << std::endl;
      //Know that the file exists.  Check it against the dependancies:

        
      //check the time of the creation of the ntuple file:
      struct stat attrib;
      stat(path+fileNameHists, &attrib);
      int timediff = attrib.st_mtime;
        
      stat(path + fileNameSource, &attrib);
        
      timediff -= attrib.st_mtime;
        
      //timediff should now be the time that the histogram file was created minus the 
      // time that the ntuple containing file was created.
      //if the ntuple is newer than the histograms, then timediff<0 and the
      // histograms are out of date.
      if (timediff > 0) {
        std::cout << "\tFile is up to date." << std::endl;
        return false;
        
      }
      else std::cout << "\tHistogram files are out of date, need to remake." << std::endl;
      return true;
    }
    else {
      std::cout << "\tHistogram file does not exist, making it..." << std::endl;
      return true;
    }
  }

  int NtupleReader::LoopNue(){

    //Input ntuple variables

    TString dummytarget = path+fileNameSource;
    TChain *c = new TChain("EventsTot");
    TChain *d = new TChain("EventsTot");
    Double_t fill_energy,wgt;
    Double_t checking_wgt;
    Int_t inno,isCC,mode, ibkg;
    Double_t weight;
    Double_t Elep;
    Double_t ElecCandEnergy;
    Double_t Eccqe;
    Double_t ecalo1, ecalo2;
    Double_t ShowerContainedDist;
    Double_t ShowerDistToStart;
    Double_t showerGap;
    Double_t vertexEnergy;
    int nuchan;



    Int_t nbytes = 0,nb = 0;
    Int_t evtcounter;
    //Int_t i,j,k;
    //Int_t ibin,jbin;
    Int_t ievt,ientry;

    c->Reset();
    c->Add(dummytarget);
    if (c==0) return 5;
    
    d->Reset();
    d->Add(dummytarget);
    if (d==0) return 5;
    

    const Int_t nentries1 = Int_t(c->GetEntries());
    // const Int_t nentries1 = 50000;

    std::cout<<"Input filename:\t" << dummytarget << std::endl;
    std::cout << "Number of entries at L= " << baseline << ":\t" << nentries1 << std::endl;
    c->SetBranchAddress("enugen",&fill_energy);
    c->SetBranchAddress("inno",&inno);
    c->SetBranchAddress("nuchan", &nuchan);         // NUANCE interaction codes
    c->SetBranchAddress("mode",&mode);
    c->SetBranchAddress("isCC",&isCC);
    c->SetBranchAddress("wgt",&wgt);
    c->SetBranchAddress("ibkg",&ibkg);
    c->SetBranchAddress("LepE", &Elep);
    c->SetBranchAddress("ElecCandEnergy", &ElecCandEnergy);
    c->SetBranchAddress("Eccqe", &Eccqe);
    c->SetBranchAddress("Ecalo1", &ecalo1);
    c->SetBranchAddress("Ecalo2", &ecalo2);
    c->SetBranchAddress("ShowerContainedDistance",&ShowerContainedDist);
    c->SetBranchAddress("ShowerDistanceToStart"  ,&ShowerDistToStart);
    c->SetBranchAddress("VertexEnergy"  ,&vertexEnergy);
    c->SetBranchAddress("showerGap"  ,&showerGap);


    std::vector<std::vector<float> > *MultiWeight;
    MultiWeight = 0;
    if (usingMultiWeights) 
      c->SetBranchAddress("MultiWeight",&MultiWeight);

    d->SetBranchAddress("wgt",&checking_wgt);


    // These backgrounds are nue specific but really they're just an enumeration of ibkgs
    // Can be reused and recycled at will...
    int kNueFromNueCC_muon        = 1;  // ibkg == 1
    int kNueFromNueCC_chargeKaon  = 2;  // ibkg == 2
    int kNueFromNueCC_neutKaon    = 3;  // ibkg == 3
    int kNueFromEScatter          = 4;  // ibkg == 4
    int kNueFromNC_pi0            = 5;  // ibkg == 5
    int kNueFromNC_delta0         = 6;  // ibkg == 6
    int kNueFromNumuCC            = 7;  // ibkg == 7
    int kDirt                     = 8;  // ibkg == 8
    int kOther                    = 9;  // ibkg == 9

    // Need these for the histograms:
    double emin = bins.front();
    double emax = bins.back();
    int nbins = bins.size() - 1;
    static int iHateROOTitSucksSoMuch=0;
    iHateROOTitSucksSoMuch++;
    //the various backgrounds:
    TH1F * nue_kNueFromNueCC_muon       = new TH1F(Form("kNueFromNueCC_muon%d",iHateROOTitSucksSoMuch),
                                                   "NueFromNueCC_muon",
                                                    nbins,emin, emax);
    TH1F * nue_kNueFromNueCC_chargeKaon = new TH1F(Form("kNueFromNueCC_chargeKaon%d",iHateROOTitSucksSoMuch),
                                                   "NueFromNueCC_chargeKaon",
                                                    nbins,emin, emax);
    TH1F * nue_kNueFromNueCC_neutKaon   = new TH1F(Form("kNueFromNueCC_neutKaon%d",iHateROOTitSucksSoMuch),
                                                   "NueFromNueCC_neutKaon",
                                                    nbins,emin, emax);
    TH1F * nue_kNueFromEScatter         = new TH1F(Form("kNueFromEScatter%d",iHateROOTitSucksSoMuch),
                                                   "NueFromEScatter",
                                                    nbins,emin, emax);
    TH1F * nue_kNueFromNC_pi0           = new TH1F(Form("kNueFromNC_pi0%d",iHateROOTitSucksSoMuch),
                                                   "NueFromNC_pi0",
                                                    nbins,emin, emax);
    TH1F * nue_kNueFromNC_delta0        = new TH1F(Form("kNueFromNC_delta0%d",iHateROOTitSucksSoMuch),
                                                   "NueFromNC_delta0",
                                                    nbins,emin, emax);
    TH1F * nue_kNueFromNumuCC           = new TH1F(Form("kNueFromNumuCC%d",iHateROOTitSucksSoMuch),
                                                   "NueFromNumuCC",
                                                    nbins,emin, emax);
    TH1F * nue_kDirt                    = new TH1F(Form("kDirt%d",iHateROOTitSucksSoMuch),
                                                   "Dirt",
                                                    nbins,emin, emax);
    TH1F * nue_kOther                   = new TH1F(Form("kOther%d",iHateROOTitSucksSoMuch),
                                                   "Other",
                                                    nbins,emin, emax);
    // This histogram holds the total nue hist.
    char edistName[100];
    sprintf(edistName,"Background %s event distribution",signal.c_str() );
    TH1F * edistrnue     = new TH1F(Form("edistrnue%d",iHateROOTitSucksSoMuch),edistName,nbins,emin,emax);
    TH1F * eLeptonEnergy   = new TH1F(Form("eLeptonDist%d",iHateROOTitSucksSoMuch), "Distribution of lepton candidate energies.", nbins, 0.0, emax);

    TH1F * eventsNueMC = new TH1F(Form("eventsNueMC%d",iHateROOTitSucksSoMuch),"MC Statistics by bin",nbins,emin,emax);

    TH1F ** edistrnueMultiWeight;
    if (usingMultiWeights)
      edistrnueMultiWeight = new TH1F*[nWeights];
    std::vector<std::vector<float > > edistrnueMultiWeightVec;
    if (usingMultiWeights)
      edistrnueMultiWeightVec.resize(nWeights);



    // Have the capacity to change the binning, not that I've ever used it:
    double binsArray[nbins+1];
    for (int i = 0; i < nbins+1; i ++){
      binsArray[i] = bins[i];
    }

    //Set the bin boundaries:
    nue_kNueFromNueCC_muon       -> SetBins(nbins, binsArray);
    nue_kNueFromNueCC_chargeKaon -> SetBins(nbins, binsArray);
    nue_kNueFromNueCC_neutKaon   -> SetBins(nbins, binsArray);
    nue_kNueFromEScatter         -> SetBins(nbins, binsArray);
    nue_kNueFromNC_pi0           -> SetBins(nbins, binsArray);
    nue_kNueFromNC_delta0        -> SetBins(nbins, binsArray);
    nue_kNueFromNumuCC           -> SetBins(nbins, binsArray);
    nue_kDirt                    -> SetBins(nbins, binsArray);
    nue_kOther                   -> SetBins(nbins, binsArray);
    edistrnue                    -> SetBins(nbins, binsArray);
    eLeptonEnergy                -> SetBins(nbins, binsArray);
    eventsNueMC                  -> SetBins(nbins, binsArray);

    if (usingMultiWeights){
      for (int i = 0; i < nWeights; ++i)
      {
        edistrnueMultiWeight[i] = (TH1F *)(edistrnue -> Clone());
        edistrnueMultiWeightVec[i].resize(nbins);
      }
    }

    std::vector<float> * NueFromNueCC_muonVec = new std::vector<float>(nbins,0);
    std::vector<float> * NueFromNueCC_chargeKaonVec = new std::vector<float>(nbins,0);
    std::vector<float> * NueFromNueCC_neutKaonVec = new std::vector<float>(nbins,0);
    std::vector<float> * NueFromEScatterVec = new std::vector<float>(nbins,0);
    std::vector<float> * NueFromNC_pi0Vec = new std::vector<float>(nbins,0);
    std::vector<float> * NueFromNC_delta0Vec = new std::vector<float>(nbins,0);
    std::vector<float> * NueFromNumuCCVec = new std::vector<float>(nbins,0);
    std::vector<float> * DirtVec = new std::vector<float>(nbins,0);
    std::vector<float> * OtherVec = new std::vector<float>(nbins,0);
    std::vector<float> * edistrnueVec = new std::vector<float>(nbins,0);
    std::vector<float> * eLeptonEnergyVec = new std::vector<float>(nbins,0);
    std::vector<float> * eventsNueMCVec = new std::vector<float>(nbins,0);


    data.resize(nbins);

    //loop through events once, fill in events[iL][idm2] arrays
    std::cout<<"Evaluating histograms... Progress...\n";

    edistrnue->Reset();   //one has the option of oscillating bkgd events too

    evtcounter = 0;


    for (ievt=0; ievt<nentries1; ievt++)
    // for (ievt=0; ievt<50000; ievt++)
    {
      evtcounter++;
      ientry = d->LoadTree(ievt); if (ientry < 0) break; nb = d->GetEntry(ievt);
      if (checking_wgt == 0){
        // std::cout << "Skipping this event because the weight is zero." << std::endl;
        continue;
      }

      ientry = c->LoadTree(ievt); if (ientry < 0) break; nb = c->GetEntry(ievt); nbytes += nb;

      // if (ievt == 0) {
      //   std::cout << "This is the first event, multiweight information:\n";
      //   std::cout << "  mutliweight.size(): " << MultiWeight -> size() << std::endl;
      //   std::cout << "  mutliweight[0].size(): " << (*MultiWeight)[0].size()<< std::endl;
      // }

      if (wgt == 0) continue;
      if ( (ievt%100000) == 0) {
        std::cout << "On event " << ievt << " of " << nentries1 << std::endl;
      }

      if (energy == "elecCand") fill_energy = ElecCandEnergy;
      if (energy == "eccqe") fill_energy = Eccqe;
      if (energy == "ecalo1") fill_energy = ecalo1;
      if (energy == "ecalo2") fill_energy = ecalo2;
      if (energy == "elep") fill_energy = Elep;

      if ( (ibkg == kNueFromNueCC_muon       || 
            ibkg == kNueFromNueCC_chargeKaon || 
            ibkg == kNueFromNueCC_neutKaon)  &&
           nuchan != 1 &&
           energy == "eccqe")
      {
        if (inno == 12 || inno == -12)
          continue;
      }

      if (ibkg == kNueFromNueCC_muon       || 
          ibkg == kNueFromNueCC_chargeKaon || 
          ibkg == kNueFromNueCC_neutKaon   || 
          ibkg == kNueFromEScatter         || 
          ibkg == kNueFromNC_pi0           || 
          ibkg == kNueFromNC_delta0        
         ){
        if(ShowerContainedDist < showerContainmentDist){
          continue;
        }
      }
      if(ShowerDistToStart < minDistanceToStart){
          continue;
      }  
      if (vertexEnergy < minVertexEnergySignal){
        continue;
      }
      if( showerGap    > minShowerGap && 
          vertexEnergy > minVertexEnergyPhoton){
        continue;
      }


      // // Reject all events with less than a certain amount of vertex activity:
      // if (vertexEnergy < minVertexEnergy){
      //   continue;
      // }

      //use all the events because the weight, eff is already calculated
      weight=wgt; 
      // if (!absolute_MWSource) edistrnue->Fill(fill_energy,weight);
      if (inno == 12 || inno == -12) eventsNueMC->Fill(fill_energy);


      // Now do the multiweight part
      if (usingMultiWeights){
        for (int i = 0; i < nWeights; ++i)
        {
          double mult_wgt = (*MultiWeight)[multiWeightSource][i];
          if (absolute_MWSource){
            if (mult_wgt != 1.0){
              edistrnueMultiWeight[i]->Fill(fill_energy,wgt*mult_wgt);           
              if (i == 1)
                edistrnue->Fill(fill_energy,weight);
            }
          }
          else{
            if (i == 0) edistrnue->Fill(fill_energy,weight);
            edistrnueMultiWeight[i]->Fill(fill_energy,wgt*mult_wgt);  
          }
        }
      }
      else edistrnue->Fill(fill_energy,weight);


      if (ibkg == kNueFromNueCC_muon      ) nue_kNueFromNueCC_muon       -> Fill(fill_energy, weight);
      if (ibkg == kNueFromNueCC_chargeKaon) nue_kNueFromNueCC_chargeKaon -> Fill(fill_energy, weight);
      if (ibkg == kNueFromNueCC_neutKaon  ) nue_kNueFromNueCC_neutKaon   -> Fill(fill_energy, weight);
      if (ibkg == kNueFromEScatter        ) nue_kNueFromEScatter         -> Fill(fill_energy, weight);
      if (ibkg == kNueFromNC_pi0          ) nue_kNueFromNC_pi0           -> Fill(fill_energy, weight);
      if (ibkg == kNueFromNC_delta0       ) nue_kNueFromNC_delta0        -> Fill(fill_energy, weight);
      if (ibkg == kNueFromNumuCC          ) nue_kNueFromNumuCC           -> Fill(fill_energy, weight);
      if (ibkg == kDirt                   ) nue_kDirt                    -> Fill(fill_energy, weight);
      if (ibkg == kOther                  ) nue_kOther                   -> Fill(fill_energy, weight);
        
      eLeptonEnergy -> Fill(Elep, weight);
      

    }//end loop over events

    if (evtcounter!=nentries1){ printf("Events loop error! Exiting...\n"); return 4; }
    
    
    //fill the vectors with the results from filling the histograms;
    for (int i = 0; i < nbins;  i++){
      (* NueFromNueCC_muonVec)[i]       = nue_kNueFromNueCC_muon    -> GetBinContent(i+1);
      (* NueFromNueCC_chargeKaonVec)[i] = nue_kNueFromNueCC_chargeKaon  -> GetBinContent(i+1);
      (* NueFromNueCC_neutKaonVec)[i]   = nue_kNueFromNueCC_neutKaon  -> GetBinContent(i+1);
      (* NueFromEScatterVec)[i]         = nue_kNueFromEScatter      -> GetBinContent(i+1);
      (* NueFromNC_pi0Vec)[i]           = nue_kNueFromNC_pi0      -> GetBinContent(i+1);
      (* NueFromNC_delta0Vec)[i]        = nue_kNueFromNC_delta0     -> GetBinContent(i+1);
      (* NueFromNumuCCVec)[i]           = nue_kNueFromNumuCC      -> GetBinContent(i+1);
      (* DirtVec)[i]                    = nue_kDirt           -> GetBinContent(i+1);
      (* OtherVec)[i]                   = nue_kOther          -> GetBinContent(i+1);
      (* edistrnueVec)[i]               = edistrnue           -> GetBinContent(i+1);
      (* eLeptonEnergyVec)[i]           = eLeptonEnergy       -> GetBinContent(i+1);
      data[i]                           = edistrnue           -> GetBinContent(i+1);
      (*eventsNueMCVec)[i]              = eventsNueMC         -> GetBinContent(i+1);
    }

    if (usingMultiWeights){
      for (int i = 0; i < nWeights; ++i)
      {
        for (int bin = 0; bin < nbins; ++bin)
        {
          edistrnueMultiWeightVec[i][bin] = edistrnueMultiWeight[i]->GetBinContent(bin+1);
          multiWeightData[i][bin] = edistrnueMultiWeight[i]->GetBinContent(bin+1);
        }
      }
    }

    //Write these histograms to a file!
    
    TFile * f = TFile::Open(path+fileNameHists, "RECREATE");
    if (!f) {
      std::cout << "Could not open file to write." << std::endl;
      return -1;
    }

    TTree *t = new TTree("tvecnue", "Tree with vectors");
    t-> Branch("NueFromNueCC_muonVec", &NueFromNueCC_muonVec);
    t-> Branch("NueFromNueCC_chargeKaonVec", &NueFromNueCC_chargeKaonVec);
    t-> Branch("NueFromNueCC_neutKaonVec", &NueFromNueCC_neutKaonVec);
    t-> Branch("NueFromEScatterVec", &NueFromEScatterVec);
    t-> Branch("NueFromNC_pi0Vec", &NueFromNC_pi0Vec);
    t-> Branch("NueFromNC_delta0Vec", &NueFromNC_delta0Vec);
    t-> Branch("NueFromNumuCCVec", &NueFromNumuCCVec);
    t-> Branch("DirtVec", &DirtVec);
    t-> Branch("OtherVec", &OtherVec);
    t-> Branch("edistrnueVec", &edistrnueVec);
    t-> Branch("eLeptonEnergyVec", &eLeptonEnergyVec);
    t-> Branch("eventsNueMCVec",&eventsNueMCVec);
    if (usingMultiWeights){
      for (int i = 0; i < nWeights; i ++){
        char  name[100];
        sprintf(name, "edistrnueMultiWeightVec_%i",i);
        t -> Branch(name, &(edistrnueMultiWeightVec[i]));
      }
    }


    t->Fill();
    if (usingMultiWeights){
      for (int i = 0; i < nWeights; ++i)
      {
        edistrnueMultiWeight[i]->Write();
      }
    }
    edistrnue                     -> Write();
    eLeptonEnergy                 -> Write();
    nue_kNueFromNueCC_muon        -> Write();
    nue_kNueFromNueCC_chargeKaon  -> Write();
    nue_kNueFromNueCC_neutKaon    -> Write();
    nue_kNueFromEScatter          -> Write();
    nue_kNueFromNC_pi0            -> Write();
    nue_kNueFromNC_delta0         -> Write();
    nue_kNueFromNumuCC            -> Write();
    nue_kDirt                     -> Write();
    nue_kOther                    -> Write();
    eventsNueMC                   -> Write();
    t                             -> Write("tvecnue");
    delete t;
    f -> Close();

    //cleaning up:''
    // delete edistrnue;           
    // delete eLeptonEnergy;         
    // delete nue_kNueFromNueCC_muon;      
    // delete nue_kNueFromNueCC_chargeKaon;
    // delete nue_kNueFromNueCC_neutKaon;  
    // delete nue_kNueFromEScatter;        
    // delete nue_kNueFromNC_pi0;          
    // delete nue_kNueFromNC_delta0;       
    // delete nue_kNueFromNumuCC;          
    // delete nue_kDirt;                   
    // delete nue_kOther;                  
    // delete t;             
    // 
    // delete c;

    return 0;

  }

  int NtupleReader::ReadNue(){
    TChain *c = new TChain("tvecnue");
    c -> Add(path+fileNameHists);
    std::vector <float>   * readInVector = 0;
    std::vector<std::vector<float> * > multiWeightReadInVector;
    if (usingMultiWeights) multiWeightReadInVector.resize(nWeights);
    c->SetBranchAddress("edistrnueVec",&readInVector);
    if (usingMultiWeights){
      for (int N_weight = 0; N_weight < nWeights; ++N_weight)
      {
        multiWeightReadInVector[N_weight] = 0;
        char  name[100];
        sprintf(name, "edistrnueMultiWeightVec_%i",N_weight);
        c->SetBranchAddress(name,&(multiWeightReadInVector[N_weight]));
      }
    }
    std::cout << "Reading in nue data, this may take a minute or two...." << std::endl;
    c-> GetEntry(0);
    data = *(readInVector);
    if (usingMultiWeights){
      for (int N_weight = 0; N_weight < nWeights; ++N_weight)
      {
        multiWeightData[N_weight] = *(multiWeightReadInVector[N_weight]);
        delete multiWeightReadInVector[N_weight];
      }
    }
    delete readInVector;
    delete c;
    return 0;

  }

  int NtupleReader::LoopNueOsc(){
      

    //Input ntuple variables

    TString dummytarget = path+fileNameSourceOsc;
    TChain *c = new TChain("EventsTot");
    TChain *d = new TChain("EventsTot");
    Double_t fill_energy,wgt,enugen,nuleng;
    Double_t checking_wgt;
    Int_t inno,isCC,mode;
    Double_t weight;
    // Double_t Elep;
    Double_t ElecCandEnergy;
    Double_t Eccqe;
    Double_t ecalo1, ecalo2;
    Double_t ElectContainedDist;
    Double_t ElectDistToStart;
    double vertexEnergy;
    int nuchan;

    Int_t nbytes = 0,nb = 0;
    Int_t evtcounter;
    //Int_t i,j,k;
    //Int_t ibin,jbin;
    Int_t ievt,ientry;

    Double_t dm2BF=1.2;
    Double_t dm2_high=50.0;
    Double_t dm2min = 0.01, dm2max = 100;
    Double_t dm2BF_glob = 0.43;
    
    //inno ID
    Int_t nue  = 12;
    Int_t nuebar = -12;

    c->Reset();
    c->Add(dummytarget);
    if (c==0) return 5;
    
    d->Reset();
    d->Add(dummytarget);
    if (d==0) return 5;
    
    const Int_t nentries2 = Int_t(c->GetEntries());
    // const Int_t nentries2 = 50000;

    std::cout<<"Fosc Input filename:\t" << dummytarget << std::endl;
    std::cout<<"Number of entries L="<<baseline<<"m: "<< nentries2<< std::endl;
    c->SetBranchAddress("enugen",&fill_energy);
    c->SetBranchAddress("inno",&inno);
    c->SetBranchAddress("mode",&mode);
    c->SetBranchAddress("nuchan", &nuchan);         // NUANCE interaction codes
    c->SetBranchAddress("isCC",&isCC);
    c->SetBranchAddress("wgt",&wgt);
    c->SetBranchAddress("enugen",&enugen);
    c->SetBranchAddress("nuleng",&nuleng);
    c->SetBranchAddress("ElecCandEnergy", &ElecCandEnergy);
    c->SetBranchAddress("Eccqe", &Eccqe);
    c->SetBranchAddress("Ecalo1", &ecalo1);
    c->SetBranchAddress("Ecalo2", &ecalo2);
    c->SetBranchAddress("ShowerContainedDistance",&ElectContainedDist);
    c->SetBranchAddress("ShowerDistanceToStart"  ,&ElectDistToStart);
    c->SetBranchAddress("VertexEnergy"  ,&vertexEnergy);

    // std::vector<std::vector<float> > *MultiWeight;
    // MultiWeight = 0;


    d->SetBranchAddress("wgt",&checking_wgt);



      
    TH1F **eventsnueosc = new TH1F*[npoints+1]; //fullosc weighted by Posc=1*sin2(1.27dm2L/E), at each dm2 point
    TH1F *eventsnuefosc;                        //fullosc (Posc=1)
    TH1F *eventsSignalBestFitNu;                //Number of events expected at the best fit.
    TH1F *eventsSignalBestFitNubar;             //Number of events expected at the best fit.
    TH1F *eventsSignalBestFit_globNu;           //Number of events expected at the best fit.
    TH1F *eventsSignalBestFit_globNubar;        //Number of events expected at the best fit.
    TH1F *eventsSignalHighDmNu;                 //Number of events expected at the best fit.
    TH1F *eventsSignalHighDmNubar;              //Number of events expected at the best fit.
    
    int nbins = bins.size() - 1;
    double emin = bins.front();
    double emax = bins.back();
    static int iHateROOTitSucksSoMuch = 0;
    iHateROOTitSucksSoMuch ++;
    TH1F *edistrnueosc      = new TH1F(Form("edistrnueosc%d",iHateROOTitSucksSoMuch),
                                      "Oscillated #nu_{e} event distribution",
                                      nbins,emin,emax);
    TH1F *edistrnuefosc     = new TH1F(Form("edistrnuefosc%d",iHateROOTitSucksSoMuch),
                                      "Fully-oscillated #nu_{e} event distribution",
                                      nbins,emin,emax);
    TH1F *edistrnueBestFitNu  = new TH1F(Form("edistrnueBestFitNu%d",iHateROOTitSucksSoMuch),
                                     "Signal values of nue appearance at LSND best fit", 
                                     nbins, emin, emax);
    TH1F *edistrnueBestFitNubar = new TH1F(Form("edistrnueBestFitNubar%d",iHateROOTitSucksSoMuch),
                                     "Signal values of nue appearance at LSND best fit", 
                                     nbins, emin, emax);
    TH1F *edistrnueHighDmNu  = new TH1F(Form("edistrnueHighDmNu%d",iHateROOTitSucksSoMuch),
                                     "Signal values of nue appearance at high dm2", 
                                     nbins, emin, emax);
    TH1F *edistrnueHighDmNubar = new TH1F(Form("edistrnueHighDmNubar%d",iHateROOTitSucksSoMuch),
                                     "Signal values of nue appearance at high dm2", 
                                     nbins, emin, emax);
    TH1F *edistrnueBestFit_globNu  = new TH1F(Form("edistrnueBestFit_globNu%d",iHateROOTitSucksSoMuch),
                                     "Signal values of nue appearance at high dm2", 
                                     nbins, emin, emax);
    TH1F *edistrnueBestFit_globNubar = new TH1F(Form("edistrnueBestFit_globNubar%d",iHateROOTitSucksSoMuch),
                                     "Signal values of nue appearance at high dm2", 
                                     nbins, emin, emax);



    //Need vectors to save out the important information:
    std::vector<float> edistrnuefoscVec(nbins,0);
    std::vector<float> edistrnueBestFitNuVec(nbins,0);
    std::vector<float> edistrnueBestFitNubarVec(nbins,0);
    std::vector<float> edistrnueHighDmNuVec(nbins,0);
    std::vector<float> edistrnueHighDmNubarVec(nbins,0);
    std::vector<float> edistrnueBestFit_globNuVec(nbins,0);
    std::vector<float> edistrnueBestFit_globNubarVec(nbins,0);
    std::vector<std::vector<float> > edistrnueoscVec(npoints+1, std::vector<float>(nbins,0) );
     


    dataOsc.resize(npoints+1);
    for (unsigned int i = 0; i < dataOsc.size(); i++) dataOsc[i].resize(nbins);

    //Make sure the bins are OK:
    double binsArray[nbins+1];
    for (int i = 0; i < nbins+1; i ++){
      binsArray[i] = bins[i];
    }
    edistrnueosc                -> SetBins(nbins, binsArray);
    edistrnuefosc               -> SetBins(nbins, binsArray);
    edistrnueBestFitNu          -> SetBins(nbins, binsArray);
    edistrnueBestFitNubar       -> SetBins(nbins, binsArray);
    edistrnueHighDmNu           -> SetBins(nbins, binsArray);
    edistrnueHighDmNubar        -> SetBins(nbins, binsArray);
    edistrnueBestFit_globNu     -> SetBins(nbins, binsArray);
    edistrnueBestFit_globNubar  -> SetBins(nbins, binsArray);
   

    
    Double_t dm2Array[npoints+1];
    //Fill the array of dm2 points:
    

    for (int i=0; i<=npoints; i++)  //loop over dm2 points
    {
      dm2Array[i] = pow(10., (TMath::Log10(dm2min)+ (i*1./npoints)*TMath::Log10(dm2max/dm2min)) );
      //While looping, may as well set up the histograms:
      eventsnueosc[i] = (TH1F*) edistrnueosc -> Clone();
    }

    //Now loop over events in the tree and fill each histogram for dm2 at each point.
    //loop through events once, fill in events[iL][idm2] arrays
    std::cout<<"Evaluating histograms... Progress...\n";
    evtcounter = 0;
    for (ievt=0; ievt<nentries2; ievt++)
    {
      if ( (ievt%100000) == 0) {
        std::cout << "On event " << ievt << " of " << nentries2 << std::endl;
      }

      evtcounter++;
      ientry = d->LoadTree(ievt); if (ientry < 0) break; nb = d->GetEntry(ievt);
      if (checking_wgt == 0){
        // std::cout << "Skipping this event because the weight is zero." << std::endl;
        continue;
      }

      ientry = c->LoadTree(ievt); if (ientry < 0) break; nb = c->GetEntry(ievt); nbytes += nb;




      if (energy == "elecCand") fill_energy = ElecCandEnergy;
      if (energy == "eccqe") fill_energy = Eccqe;
      if (energy == "ecalo1") fill_energy = ecalo1;
      if (energy == "ecalo2") fill_energy = ecalo2;

      if (energy == "eccqe" && nuchan != 1){
        if (inno == 12 || inno == -12)
          continue;
      }

      if(ElectContainedDist < showerContainmentDist){
        // std::cout << "Skipping this fosc event in the containment cut.\n";
        continue;
      }
      if(ElectDistToStart < minDistanceToStart){
        continue;
      }

      if (vertexEnergy < minVertexEnergySignal){
        continue;
      }


      //Need to loop over dm2 points...
      for (int i=0; i<=npoints; i++){
        weight=wgt*pow((TMath::Sin(1.27*dm2Array[i]*(nuleng/100.)/(enugen*1000.))),2);
        eventsnueosc[i]->Fill(fill_energy,weight);
      }

      //Fill the scaled probability thinger:

      weight=wgt*0.003;
      edistrnuefosc->Fill(fill_energy,weight);
      
      //Fill the bestfit plot:
      weight=wgt*pow((TMath::Sin(1.27*dm2BF*(nuleng/100.)/(enugen*1000.))),2);
      if (inno == nue) edistrnueBestFitNu->Fill(fill_energy,weight);
      if (inno == nuebar) edistrnueBestFitNubar->Fill(fill_energy,weight);
      // Fill the high dm2 point:
      weight=wgt*pow((TMath::Sin(1.27*dm2_high*(nuleng/100.)/(enugen*1000.))),2);
      if (inno == nue) edistrnueHighDmNu->Fill(fill_energy,weight);
      if (inno == nuebar) edistrnueHighDmNubar->Fill(fill_energy,weight);
      // Fill the global BF dm2 point:
      weight=wgt*pow((TMath::Sin(1.27*dm2BF_glob*(nuleng/100.)/(enugen*1000.))),2);
      if (inno == nue) edistrnueBestFit_globNu->Fill(fill_energy,weight);
      if (inno == nuebar) edistrnueBestFit_globNubar->Fill(fill_energy,weight);
    }//end loop over events

    if (evtcounter!=nentries2){ printf("Events loop error! Exiting...\n"); return 4; }
    //populate nueosc, and fullosc only on the first go.
    
    eventsSignalBestFitNu           = (TH1F*)edistrnueBestFitNu->Clone();
    eventsSignalBestFitNubar        = (TH1F*)edistrnueBestFitNubar->Clone();
    eventsSignalHighDmNu            = (TH1F*)edistrnueHighDmNu->Clone();
    eventsSignalHighDmNubar         = (TH1F*)edistrnueHighDmNubar->Clone();
    eventsSignalBestFit_globNu      = (TH1F*)edistrnueBestFit_globNu->Clone();
    eventsSignalBestFit_globNubar   = (TH1F*)edistrnueBestFit_globNubar->Clone();
    
    eventsnuefosc = (TH1F*)edistrnuefosc->Clone();



    //end loop over dm2 points
    
    //Now move the histograms into the vectors:
    for (int i = 0; i < nbins;  i++){
      edistrnuefoscVec[i]     = eventsnuefosc     -> GetBinContent(i+1);
      edistrnueBestFitNuVec[i]  = eventsSignalBestFitNu   -> GetBinContent(i+1);
      edistrnueBestFitNubarVec[i] = eventsSignalBestFitNubar  -> GetBinContent(i+1);
      edistrnueHighDmNuVec[i]  = eventsSignalHighDmNu   -> GetBinContent(i+1);
      edistrnueHighDmNubarVec[i] = eventsSignalHighDmNubar  -> GetBinContent(i+1);
      edistrnueBestFit_globNuVec[i]  = eventsSignalBestFit_globNu   -> GetBinContent(i+1);
      edistrnueBestFit_globNubarVec[i] = eventsSignalBestFit_globNubar  -> GetBinContent(i+1);
    }
    for (int point =0; point <= npoints; point ++){
      for (int i = 0; i < nbins; i++){
        edistrnueoscVec[point][i] = eventsnueosc[point]   -> GetBinContent(i+1);
        dataOsc[point][i] = eventsnueosc[point]   -> GetBinContent(i+1);
      }
    }
    

    //write to file...
    TFile f(path+fileNameHistsOsc, "RECREATE");
    eventsnuefosc ->Write();
    eventsSignalBestFitNu ->Write();
    eventsSignalBestFitNubar ->Write();
    eventsSignalBestFit_globNu ->Write();
    eventsSignalBestFit_globNubar ->Write();
    eventsSignalHighDmNu ->Write();
    eventsSignalHighDmNubar ->Write();
    for (int i=0; i<=npoints; i++)  //loop over dm2 points
    {
      eventsnueosc[i] -> Write();
      //std::cout << "Just wrote nueosc " << j << ", " << i << std::endl;
    }
    TTree *t = new TTree("tvecfosc", "Tree with vectors");
    t-> Branch("edistrnuefoscVec", &edistrnuefoscVec);
    t-> Branch("edistrnueBestFitNuVec", &edistrnueBestFitNuVec);
    t-> Branch("edistrnueBestFitNubarVec", &edistrnueBestFitNubarVec);
    t-> Branch("edistrnueHighDmNuVec", &edistrnueHighDmNuVec);
    t-> Branch("edistrnueHighDmNubarVec", &edistrnueHighDmNubarVec);
    t-> Branch("edistrnueBestFit_globNuVec", &edistrnueBestFit_globNuVec);
    t-> Branch("edistrnueBestFit_globNubarVec", &edistrnueBestFit_globNubarVec);
    t->Fill();
    t-> Write();
    //Dealing with all the oscillated vectors can be tricker:
    TTree *t1 = new TTree("tvecosc", "Tree with oscillation signals");
    for (int point = 0; point <= npoints; point ++){
      char  name[100];
      sprintf(name, "edistrnueoscVec_%i",point);
      t1 -> Branch(name, &(edistrnueoscVec[point]));
    }

    t1 -> Fill();
    t1 -> Write();

    std::cout << "Successfully wrote histograms to file." << std::endl;
    
    f.Close();

    // delete t;
    // delete c;
    // delete t1; 

    return 0;
  }

  int NtupleReader::ReadNueOsc(){

    dataOsc.resize(npoints+1);
    // for (unsigned int i = 0; i < dataOsc.size(); i++) dataOsc[i].resize(bins.size()-1);

    std::vector<std::vector<float> *> dataOscReadInVector;
    dataOscReadInVector.resize(npoints+1);

    TChain *c = new TChain("tvecosc");
    c -> Add(path+fileNameHistsOsc);



    
    for (int ii = 0; ii <= npoints; ii++){
      dataOscReadInVector[ii] = 0;
      char name[100];
      sprintf(name, "edistrnueoscVec_%i",ii);
      //Set up the tree and branch each time:
      c -> SetBranchAddress(name, &dataOscReadInVector[ii]);


    }
    std::cout << "Reading in nue osc data, this may take a minute or two...." << std::endl;
    c -> GetEntry(0);


    for (int ii = 0; ii <= npoints; ++ii)
    {
      dataOsc[ii] = *(dataOscReadInVector[ii]);
      delete dataOscReadInVector[ii];
    }


    // if (c) delete c;
    // if (d) delete d;
    // delete readInVector;
    

    return 0;
  }

  int NtupleReader::LoopNumu(){
    
    TString dummytarget = path+fileNameSource;
    TChain *c = new TChain("EventsTot");
    TChain *d = new TChain("EventsTot");
    Double_t fill_energy,wgt;
    Double_t checking_wgt;
    Int_t inno,isCC,mode, ibkg;
    Double_t weight;
    Double_t Elep;
    Double_t ElecCandEnergy;
    Double_t Eccqe;
    Double_t ecalo1, ecalo2;

    Int_t nbytes = 0,nb = 0;
    Int_t evtcounter;
    //Int_t i,j,k;
    //Int_t ibin,jbin;
    Int_t ievt,ientry;
      
    c->Reset();
    c->Add(dummytarget);
    d->Reset();
    d->Add(dummytarget);
    nbytes = 0;
    evtcounter = 0;
    if (c==0) return 5;
    if (d==0) return 5;

    const Int_t nentries1 = Int_t(c->GetEntries());
    // const Int_t nentries1 = 50000;

    std::cout<<"Input filename:\t" << dummytarget << std::endl;
    std::cout << "Number of entries at L= " << baseline << ":\t" << nentries1 << std::endl;
    c->SetBranchAddress("enugen",&fill_energy);
    c->SetBranchAddress("inno",&inno);
    c->SetBranchAddress("mode",&mode);
    c->SetBranchAddress("isCC",&isCC);
    c->SetBranchAddress("wgt",&wgt);
    c->SetBranchAddress("ibkg",&ibkg);
    c->SetBranchAddress("LepE", &Elep);
    c->SetBranchAddress("ElecCandEnergy", &ElecCandEnergy);
    c->SetBranchAddress("Eccqe", &Eccqe);
    c->SetBranchAddress("Ecalo1", &ecalo1);
    c->SetBranchAddress("Ecalo2", &ecalo2);
    std::vector<std::vector<float> > * MultiWeight;
    MultiWeight = 0;
    if (usingMultiWeights)
      c->SetBranchAddress("MultiWeight",&MultiWeight);

    d->SetBranchAddress("wgt",&checking_wgt);



    // Need these for the histograms:
    double emin = bins.front();
    double emax = bins.back();
    int nbins = bins.size() - 1;
    static int iHateROOTitSucksSoMuch = 0;
    iHateROOTitSucksSoMuch ++;
    // This histogram holds the total nue hist.
    TH1F * edistrnumu       = new TH1F(Form("edistrnumu%d",iHateROOTitSucksSoMuch),"Background #nu_{\{mu}} event distribution",nbins,emin,emax);
    TH1F * eventsNumuMC     = new TH1F(Form("eventsNumuMC%d",iHateROOTitSucksSoMuch),"MC event distribution for numu",nbins,emin,emax);
    TH1F * muLeptonEnergy   = new TH1F(Form("muLeptonDist%d",iHateROOTitSucksSoMuch), "Distribution of muon energies.", nbins, 0.0, emax);

    TH1F ** edistnumuMultiWeight;
    if(usingMultiWeights) 
      edistnumuMultiWeight = new TH1F*[nWeights];

    std::vector<float> edistrnumuVec(nbins,0);
    std::vector<float> eventsNumuMCVec(nbins,0);
    std::vector<float> muLeptonEnergyVec(nbins, 0);
    std::vector<std::vector<float> > edistnumuMultiWeightVec;
    if (usingMultiWeights) 
      edistnumuMultiWeightVec.resize(nWeights);
    
    data.resize(nbins);

    double binsArray[nbins+1];
    for (int i = 0; i < nbins+1; i ++){
      binsArray[i] = bins[i];
    }

    eventsNumuMC -> SetBins(nbins, binsArray);
    edistrnumu -> SetBins(nbins, binsArray);
    muLeptonEnergy -> SetBins(nbins, binsArray);

    if (usingMultiWeights){
      for (int N_weight = 0; N_weight < nWeights; ++N_weight)
      {
        edistnumuMultiWeight[N_weight] = (TH1F*) edistrnumu->Clone();
        edistnumuMultiWeightVec[N_weight].resize(nbins);
      }
    }

    for (ievt=0; ievt<nentries1; ievt++)
    {
      evtcounter++;

      ientry = d->LoadTree(ievt); if (ientry < 0) break; nb = d->GetEntry(ievt);
      if (checking_wgt == 0){
        // std::cout << "Skipping this event because the weight is zero." << std::endl;
        continue;
      }

      ientry = c->LoadTree(ievt); if (ientry < 0) break; nb = c->GetEntry(ievt); nbytes += nb;
      
      if ( (ievt%100000) == 0) {
        std::cout << "On event " << ievt << " of " << nentries1 << std::endl;
      }

      if (energy == "elecCand") fill_energy = ElecCandEnergy;
      if (energy == "eccqe")    fill_energy = Eccqe;
      if (energy == "ecalo1")   fill_energy = ecalo1;
      if (energy == "ecalo2")   fill_energy = ecalo2;

      //use all the events because the weight, eff is already calculated
      weight=wgt; 
      muLeptonEnergy  -> Fill(Elep, weight);
      eventsNumuMC -> Fill(fill_energy);

      // Now do the multiweight part
      // if (usingMultiWeights){
      //   for (int N_weight = 0; N_weight < nWeights; ++N_weight)
      //   {
      //     // double mutl_wgt = (*MultiWeight)[multiWeightSource][N_weight];
      //     double mutl_wgt = (*MultiWeight)[6][N_weight];
      //     edistnumuMultiWeight[N_weight]->Fill(fill_energy,weight*mutl_wgt);
      //   }
      // }
      if (usingMultiWeights){
        for (int i = 0; i < nWeights; ++i)
        {
          double mult_wgt = (*MultiWeight)[multiWeightSource][i];
          if (absolute_MWSource){
            if (mult_wgt != 1.0){
              edistnumuMultiWeight[i]->Fill(fill_energy,wgt*mult_wgt);           
              if (i == 0)
                edistrnumu->Fill(fill_energy,weight);
            }
          }
          else{
            if (i == 0) edistrnumu->Fill(fill_energy,weight);
            edistnumuMultiWeight[i]->Fill(fill_energy,wgt*mult_wgt);  
          }
        }
      }
      else edistrnumu -> Fill(fill_energy,weight);


    }//end loop over events

    if (evtcounter!=nentries1){ printf("Events loop error! Exiting...\n"); return 4; }
    
    
    //fill the vectors with the results from filling the histograms;
    for (int i = 0; i < nbins;  i++){
      edistrnumuVec[i]     = edistrnumu     -> GetBinContent(i+1);
      muLeptonEnergyVec[i] = muLeptonEnergy -> GetBinContent(i+1);
      eventsNumuMCVec[i]   = eventsNumuMC   -> GetBinContent(i+1);
      data[i]              = edistrnumu     -> GetBinContent(i+1);
    }
    if (usingMultiWeights){
      for (int N_weight = 0; N_weight < nWeights; ++N_weight)
      {
        for (int bin = 0; bin < nbins; ++bin)
        {
          multiWeightData[N_weight][bin] = edistnumuMultiWeight[N_weight] -> GetBinContent(bin+1);
          edistnumuMultiWeightVec[N_weight][bin] = edistnumuMultiWeight[N_weight] -> GetBinContent(bin+1);
        }
      }
    }

    //Write these histograms to a file!
    
    TFile * f = TFile::Open(path+fileNameHists, "RECREATE");
    if (!f || f->IsZombie() ) {
      std::cout << "Could not open file to write." << std::endl;
      return -1;
    }

    TTree *t = new TTree("tvecnumu", "Tree with vectors");
    t-> Branch("edistrnumuVec",&edistrnumuVec);
    t-> Branch("muLeptonEnergy",&muLeptonEnergyVec);
    t-> Branch("eventsNumuMC",&eventsNumuMCVec);

    if (usingMultiWeights){
      for (int N_weight= 0; N_weight < nWeights; N_weight++){
        char  name[100];
        sprintf(name, "edistnumuMultiWeightVec_%i",N_weight);
        t -> Branch(name, &(edistnumuMultiWeightVec[N_weight]));
      }
    }

    edistrnumu     -> Write();
    muLeptonEnergy -> Write();
    eventsNumuMC   -> Write();
    t -> Fill();
    t -> Write();
    
    f -> Close();
    //cleaning up:
    // delete edistrnumu;          
    // delete muLeptonEnergy;        

    // delete t;  
    // delete f;
    return 0;





  }

  int NtupleReader::LoopNumuOsc(){
       

    //Input ntuple variables

    TString dummytarget = path+fileNameSourceOsc;
    TChain *c = new TChain("EventsTot");
    Double_t fill_energy,wgt,enugen,nuleng;
    Int_t inno,isCC,mode;
    Double_t weight;
    // Double_t Elep;
    Double_t ElecCandEnergy;
    Double_t Eccqe;
    Double_t ecalo1, ecalo2;

    Int_t nbytes = 0,nb = 0;
    Int_t evtcounter;
    //Int_t i,j,k;
    //Int_t ibin,jbin;
    Int_t ievt,ientry;

    Double_t dm2BF=1.2;
    Double_t dm2min = 0.01, dm2max = 100;
    
    //inno ID
    Int_t numu  = 14;
    Int_t numubar = -14;

    c->Reset();
    c->Add(dummytarget);
    if (c==0) return 5;

    const Int_t nentries2 = Int_t(c->GetEntries());

    std::cout<<"Input filename:\t" << dummytarget << std::endl;
    std::cout<<"Number of entries L="<<baseline<<"m: "<< nentries2<< std::endl;
    c->SetBranchAddress("enugen",&fill_energy);
    c->SetBranchAddress("inno",&inno);
    c->SetBranchAddress("mode",&mode);
    c->SetBranchAddress("isCC",&isCC);
    c->SetBranchAddress("wgt",&wgt);
    c->SetBranchAddress("enugen",&enugen);
    c->SetBranchAddress("nuleng",&nuleng);
    c->SetBranchAddress("ElecCandEnergy", &ElecCandEnergy);
    c->SetBranchAddress("Eccqe", &Eccqe);
    c->SetBranchAddress("Ecalo1", &ecalo1);
    c->SetBranchAddress("Ecalo2", &ecalo2);

    //loop through events once, fill in events[iL][idm2] arrays
    std::cout<<"Evaluating histograms... Progress...\n";
      
    TH1F *eventsnumuosc[npoints+1];      //fullosc weighted by Posc=1*sin2(1.27dm2L/E), at each dm2 point
    TH1F *eventsnumufosc;          //fullosc (Posc=1)
    TH1F *eventsSignalBestFitNu;      //Number of events expected at the best fit.
    TH1F *eventsSignalBestFitNubar;     //Number of events expected at the best fit.
    
    int nbins = bins.size() - 1;
    double emin = bins.front();
    double emax = bins.back();

    static int iHateROOTitSucksSoMuch = 0;
    iHateROOTitSucksSoMuch++;

    TH1F *edistrnumuosc      = new TH1F(Form("edistrnumuosc%d",iHateROOTitSucksSoMuch),
                                  "Oscillated #nu_{e} event distribution",nbins,emin,emax);
    TH1F *edistrnumufosc     = new TH1F(Form("edistrnumufosc%d",iHateROOTitSucksSoMuch),
                                  "Fully-oscillated #nu_{e} event distribution",nbins,emin,emax);
    TH1F *edistrnumuBestFitNu  = new TH1F(Form("edistrnumuBestFitNu%d",iHateROOTitSucksSoMuch),
                                   "Signal values of numu appearance at LSND best fit", nbins, emin, emax);
    TH1F *edistrnumuBestFitNubar = new TH1F(Form("edistrnumuBestFitNubar%d",iHateROOTitSucksSoMuch),
                                   "Signal values of numu appearance at LSND best fit", nbins, emin, emax);


    //Need vectors to save out the important information:
    std::vector<float> edistrnumufoscVec(nbins,0);
    std::vector<float> edistrnumuBestFitNuVec(nbins,0);
    std::vector<float> edistrnumuBestFitNubarVec(nbins,0);
    std::vector<std::vector<float> > edistrnumuoscVec(npoints+1, std::vector<float>(nbins,0) );
    
    dataOsc.resize(npoints+1);
    for (unsigned int i = 0; i < dataOsc.size(); i++) dataOsc[i].resize(nbins);

    //Make sure the bins are OK:
    double binsArray[nbins+1];
    for (int i = 0; i < nbins+1; i ++){
      binsArray[i] = bins[i];
    }
    edistrnumuosc      -> SetBins(nbins, binsArray);
    edistrnumufosc       -> SetBins(nbins, binsArray);
    edistrnumuBestFitNu    -> SetBins(nbins, binsArray);
    edistrnumuBestFitNubar   -> SetBins(nbins, binsArray);

    Double_t dm2Array[npoints+1];
    //Fill the array of dm2 points:

    for (int i=0; i<=npoints; i++)  //loop over dm2 points
    {
      dm2Array[i] = pow(10., (TMath::Log10(dm2min)+ (i*1./npoints)*TMath::Log10(dm2max/dm2min)) );
      //While looping, may as well set up the histograms:
      eventsnumuosc[i] = (TH1F*) edistrnumuosc -> Clone();
    }


    //Now loop over events in the tree and fill each histogram for dm2 at each point.

    evtcounter = 0;
    for (ievt=0; ievt<nentries2; ievt++)
    {
      evtcounter++;

      if ( (ievt%100000) == 0) std::cout << "On event " << ievt << " of " << nentries2 << std::endl;

      if (energy == "elecCand") fill_energy = ElecCandEnergy;
      if (energy == "eccqe")    fill_energy = Eccqe;
      if (energy == "ecalo1")   fill_energy = ecalo1;
      if (energy == "ecalo2")   fill_energy = ecalo2;

      ientry = c->LoadTree(ievt); if (ientry < 0) break; nb = c->GetEntry(ievt); nbytes += nb;
      //Need to loop over dm2 points...
      for (int i=0; i<=npoints; i++){
        weight=wgt*pow((TMath::Sin(1.27*dm2Array[i]*(nuleng/100.)/(enugen*1000.))),2);
        eventsnumuosc[i]->Fill(fill_energy,weight);
      }

      //Fill the scaled probability thinger:

      weight=wgt*0.003;
      edistrnumufosc->Fill(fill_energy,weight);
      
      //Fill the bestfit plot:
      weight=wgt*pow((TMath::Sin(1.27*dm2BF*(nuleng/100.)/(enugen*1000.))),2);
      if (inno == numu) edistrnumuBestFitNu->Fill(fill_energy,weight);
      if (inno == numubar) edistrnumuBestFitNubar->Fill(fill_energy,weight);

    }//end loop over events

    if (evtcounter!=nentries2){ printf("Events loop error! Exiting...\n"); return 4; }
    //populate numuosc, and fullosc only on the first go.
    
    eventsSignalBestFitNu     = (TH1F*)edistrnumuBestFitNu->Clone();
    eventsSignalBestFitNubar  = (TH1F*)edistrnumuBestFitNubar->Clone();
    
    eventsnumufosc = (TH1F*)edistrnumufosc->Clone();


    //end loop over dm2 points
    
    //Now move the histograms into the vectors:
    for (int i = 0; i < nbins;  i++){
      edistrnumufoscVec[i]         = eventsnumufosc           -> GetBinContent(i+1);
      edistrnumuBestFitNuVec[i]    = eventsSignalBestFitNu    -> GetBinContent(i+1);
      edistrnumuBestFitNubarVec[i] = eventsSignalBestFitNubar -> GetBinContent(i+1);
    }
    for (int point =0; point <= npoints; point ++){
      for (int i = 0; i < nbins; i++){
        edistrnumuoscVec[point][i] = eventsnumuosc[point]   -> GetBinContent(i+1);
        dataOsc[point][i]        = eventsnumuosc[point]   -> GetBinContent(i+1);
      }
    }
    
    //write to file...
    TFile f(path+fileNameHistsOsc, "RECREATE");
    eventsnumufosc ->Write();
    eventsSignalBestFitNu ->Write();
    eventsSignalBestFitNubar ->Write();
    for (int i=0; i<=npoints; i++)  //loop over dm2 points
    {
      eventsnumuosc[i] -> Write();
      //std::cout << "Just wrote numuosc " << j << ", " << i << std::endl;
    }
    TTree *t = new TTree("tvecfosc", "Tree with vectors");
    t-> Branch("edistrnumufoscVec", &edistrnumufoscVec);
    t-> Branch("edistrnumuBestFitNuVec", &edistrnumuBestFitNuVec);
    t-> Branch("edistrnumuBestFitNubarVec", &edistrnumuBestFitNubarVec);
    t->Fill();
    t-> Write();
    //Dealing with all the oscillated vectors can be tricker:
    TTree *t1 = new TTree("tvecosc", "Tree with oscillation signals");
    for (int point = 0; point <= npoints; point ++){
      char  name[100];
      sprintf(name, "edistrnumuoscVec_%i",point);
      t1 -> Branch(name, &(edistrnumuoscVec[point]));
    }
    t1 -> Fill();
    t1-> Write();
    
    std::cout << "Successfully wrote histograms to file." << std::endl;
    
    f.Close();

    // delete t;
    // delete c;
    // delete t1; 

    return 0;
  }

  int NtupleReader::ReadNumu(){
    TChain *c = new TChain("tvecnumu");
    c -> Add(path+fileNameHists);
    std::vector <float>   * readInVector = 0;
    c->SetBranchAddress("edistrnumuVec",&readInVector);
    std::vector<std::vector<float> * > multiWeightReadInVector;
    if (usingMultiWeights) multiWeightReadInVector.resize(nWeights);
    if (usingMultiWeights){
      for (int N_weight = 0; N_weight < nWeights; ++N_weight)
      {
        multiWeightReadInVector[N_weight] = 0;
        char  name[100];
        sprintf(name, "edistnumuMultiWeightVec_%i",N_weight);
        c->SetBranchAddress(name,&multiWeightReadInVector[N_weight]);      
      }
    }
    c-> GetEntry(0);
    data = *(readInVector);
    if (usingMultiWeights){
      for (int N_weight = 0; N_weight < nWeights; ++N_weight)
      {
        multiWeightData[N_weight] = *(multiWeightReadInVector[N_weight]);
        delete multiWeightReadInVector[N_weight];
      }
    }
    delete readInVector;
    delete c;
    return 0;

  }

  int NtupleReader::ReadNumuOsc(){
    dataOsc.resize(npoints+1);
    for (unsigned int i = 0; i < dataOsc.size(); i++) dataOsc[i].resize(bins.size()-1);

    for (int ii = 0; ii <= npoints; ii++){
      std::vector <float>   * readInVector = 0;
      char name[100];
      sprintf(name, "edistrnumuoscVec_%i",ii);
      //Set up the tree and branch each time:
      TChain *c = new TChain("tvecosc");
      c -> Add(path+fileNameHistsOsc);
      c -> SetBranchAddress(name, &readInVector);
      c -> GetEntry(0);
      dataOsc[ii] = *(readInVector);
      // if (debug){
      //   std::cout << "Size is " << eventsnueoscVec[b_line][ii].size() << std::endl;
      //   for (unsigned int i = 0; i < eventsnueoscVec[b_line][ii].size(); i++){
      //     std::cout << "readInVector is " << eventsnueoscVec[b_line][ii][i] << std::endl;
      //   }
      // }
      // delete c;
      delete readInVector;
    }
    return 0;
  }

  std::vector<float> NtupleReader::GetVectorFromTree(char * branchName, char * treeName, bool osc) const{
    TChain * c;
    if (treeName == NULL ){
      if (signal == "nue") c = new TChain("tvecnue");
      if (signal == "numu") c = new TChain("tvecnumu");
    }
    else c = new TChain(treeName);
    if (osc) c -> Add(path + fileNameHistsOsc);
    else c -> Add(path + fileNameHists);
    std::vector <float>   * readInVector = 0;
    c->SetBranchAddress(branchName,&readInVector);
    c-> GetEntry(0);
    // if (c) delete c;
    return *(readInVector);
  }

  std::vector<float> NtupleReader::GetComptonBackgroundFromFile(std::string fileName, int cut){

    std::vector<float> photons200_nocutVec;
    std::vector<float> photons200_at50Vec;
    std::vector<float> photons200_at100Vec;
    photons200_nocutVec.resize(bins.size()-1);
    photons200_at50Vec.resize(bins.size()-1);
    photons200_at100Vec.resize(bins.size()-1);

    if (!fileExists(fileName.c_str())){
      std::cout << "WARNING: cosmics file "  << fileName << " does not exist,"
                << " returning an empty vector instead." << std::endl;
      return photons200_nocutVec;
    }

    TFile * file = new TFile((TString)fileName, "READ");
    TH1F * photons200_nocut = (TH1F*)file -> Get("photons200_nocut");
    TH1F * photons200_at50  = (TH1F*)file -> Get("photons200_at50");
    TH1F * photons200_at100 = (TH1F*)file -> Get("photons200_at100");
    
    // It's nasty but here is the spot to apply efficiency:
    for (unsigned int i = 4; i < bins.size()-1; i ++){
      photons200_nocutVec[i] = (0.8) * photons200_nocut -> GetBinContent(i+1);
      photons200_at50Vec[i]  = (0.8) * photons200_at50  -> GetBinContent(i+1);
      photons200_at100Vec[i] = (0.8) * photons200_at100 -> GetBinContent(i+1);
    }
    file->Close();

    // std::cout << "bin\tElow\tNull\t50\t100\n";
    // for (int i = 0; i < bins.size()-1; i ++){
    //   std::cout << i << "\t" << bins[i] << "\t"
    //             << photons200_nocutVec[i] << "\t"
    //             << photons200_at50Vec[i]  << "\t"
    //             << photons200_at100Vec[i] << "\n";
    // }
    std::cout << std::endl;

    if (cut == 50) return photons200_at50Vec;
    if (cut == 100) return photons200_at100Vec;
    else return photons200_nocutVec;


  }

}


