
#include "NtupleReader.hh"

namespace lar1{

  NtupleReader::NtupleReader(){

    //See up all the default arguments:
    signal = "nue";
    path = "../ntuples/";
    baseline = 470;
    beammode = "nu";
    forceRemake = false;
    energy = "true";
    npoints = 10;

    isScaling = false;
    scale = 1;
    specialNameText = "";

    std::vector<float> defaultBins;

    // Default to 10 equal length bins from 0.2 GeV to 3 GeV:
    double emin = 0.2;
    double emax = 3.0;
    double nbins = 10.0;
    double binSize = (emax - emin)/nbins;
    defaultBins.resize(nbins+1);
    for (double i = 0; i <= nbins; i++) {
      defaultBins.at(i) = emin + i*binSize;
    }

    setBins(defaultBins);

  }

  NtupleReader::NtupleReader( std::string input_signal,
                              std::string input_path,
                              int input_baseline,
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

    std::vector<float> defaultBins;
    // Default to 10 equal length bins from 0.2 GeV to 3 GeV:
    double emin = 0.2;
    double emax = 3.0;
    double nbins = 10.0;
    double binSize = (emax - emin)/nbins;
    defaultBins.resize(nbins+1);
    for (double i = 0; i <= nbins; i++) {
      defaultBins.at(i) = emin + i*binSize;
    }
    setBins(defaultBins);

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

  bool NtupleReader::fileExists(const char *filename) const{
    ifstream ifile(filename);
    return ifile.is_open();
  }

  void NtupleReader::buildFileNameSource() {

    //Need to make the names of the files, nue and numu:
    TString Name  = "combined_ntuple_";
    Name += baseline;
    Name += "m_";     //"combined_ntuple_470m_"
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
    double emin = bins.front();
    double emax = bins.back();
    // int nbins = bins.size() - 1;
    char tempstring[100];
    //path to the file that holds histos:
    fileNameHists = fileNameSource;
    fileNameHistsOsc = fileNameSourceOsc;
    fileNameHists += "_";
    sprintf(tempstring, "%g", emin);
    fileNameHists += tempstring;
    fileNameHists += "_";
    sprintf(tempstring, "%g_", emax);
    fileNameHists += tempstring;
    fileNameHists += energy;
    fileNameHists += "_hists.root";
    fileNameHistsOsc += "_";
    sprintf(tempstring, "%g", emin);
    fileNameHistsOsc += tempstring;
    fileNameHistsOsc += "_";
    sprintf(tempstring, "%g", emax);
    fileNameHistsOsc += tempstring;
    sprintf(tempstring, "_%d_", npoints);
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
      return 6;
    }

    buildFileNameSource();
    bool SourceFound = fileExists(filePath+fileNameSource);


    if (! (SourceFound) ){
      std::cout << "NtupleReader: Error: No input file found.  Searched for: " <<std::endl;
      std::cout << "\t" << fileNameSource <<std::endl;
      std::cout << "\tIn: " << filePath << std::endl;
      return 2;
    }
    SourceFound = fileExists(filePath+fileNameSourceOsc);

    if (! (SourceFound) ){
      std::cout << "NtupleReader: Error: No input file found.  Searched for: " <<std::endl;
      std::cout << "\t" << fileNameSourceOsc <<std::endl;
      std::cout << "\tIn: " << filePath << std::endl;
      return 2;
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
        int val(0);
        if (signal == "nue") val = LoopNue();
        if (signal == "numu") val = LoopNumu();
        if (val != 0) return val;
      }
      else{ // Everything is good, just read in the files:
        int val(0);
        if (signal == "nue") val = ReadNue();
        if (signal == "numu") val = ReadNumu();
        if (val != 0) return val;
      }
    }
    else { // No hists file exists, make it.
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
        if (signal == "nue") val = LoopNue();
        if (signal == "numu") val = LoopNumu();
        if (val != 0) return val;
      }
      else{ // Everything is good, just read in the files:
        int val(0);
        if (signal == "nue") val = ReadNueOsc();
        if (signal == "numu") val = ReadNumuOsc();
        if (val != 0) return val;
      }
    }
    else { // No hists file exists, make it.
      int val(0);
      if (signal == "nue") val = LoopNueOsc();
      if (signal == "numu") val = LoopNumuOsc();
      if (val != 0) return val;
    }




      
    return 0;

  }

  bool NtupleReader::UpToDate(const char * sourceFile, const char * testFile) const{
    if (fileExists(sourceFile)){
      if (fileExists(testFile)){
        std::cout << "\tFound source file.  Verifying up to date." << std::endl;

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
    Double_t fill_energy,wgt;
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
    if (c==0) return 5;
    
    const Int_t nentries1 = Int_t(c->GetEntries());

    std::cout<<"Input filename:\t" << dummytarget << std::endl;
    std::cout << "Number of entries at L= " << baseline << ":\t" << nentries1 << std::endl;
    c->SetBranchAddress("energy",&fill_energy);
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

    //the various backgrounds:
    TH1F * nue_kNueFromNueCC_muon       = new TH1F("kNueFromNueCC_muon      ", "NueFromNueCC_muon      ", nbins,emin, emax);
    TH1F * nue_kNueFromNueCC_chargeKaon = new TH1F("kNueFromNueCC_chargeKaon", "NueFromNueCC_chargeKaon", nbins,emin, emax);
    TH1F * nue_kNueFromNueCC_neutKaon   = new TH1F("kNueFromNueCC_neutKaon  ", "NueFromNueCC_neutKaon  ", nbins,emin, emax);
    TH1F * nue_kNueFromEScatter         = new TH1F("kNueFromEScatter        ", "NueFromEScatter        ", nbins,emin, emax);
    TH1F * nue_kNueFromNC_pi0           = new TH1F("kNueFromNC_pi0          ", "NueFromNC_pi0          ", nbins,emin, emax);
    TH1F * nue_kNueFromNC_delta0        = new TH1F("kNueFromNC_delta0       ", "NueFromNC_delta0       ", nbins,emin, emax);
    TH1F * nue_kNueFromNumuCC           = new TH1F("kNueFromNumuCC          ", "NueFromNumuCC          ", nbins,emin, emax);
    TH1F * nue_kDirt                    = new TH1F("kDirt                   ", "Dirt                   ", nbins,emin, emax);
    TH1F * nue_kOther                   = new TH1F("kOther                  ", "Other                  ", nbins,emin, emax);
   
    // This histogram holds the total nue hist.
    char edistName[100];
    sprintf(edistName,"Background %s event distribution",signal.c_str() );
    TH1F * edistrnue     = new TH1F("edistrnue",edistName,nbins,emin,emax);
    TH1F * eLeptonEnergy   = new TH1F("eLeptonDist", "Distribution of lepton candidate energies.", nbins, 0.0, emax);

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


    data.resize(nbins);

    //loop through events once, fill in events[iL][idm2] arrays
    std::cout<<"Evaluating histograms... Progress...\n";

    edistrnue->Reset();   //one has the option of oscillating bkgd events too

    evtcounter = 0;

    for (ievt=0; ievt<nentries1; ievt++)
    {
      ientry = c->LoadTree(ievt); if (ientry < 0) break; nb = c->GetEntry(ievt); nbytes += nb;
      evtcounter++;

      if (energy == "elecCand") fill_energy = ElecCandEnergy;
      if (energy == "eccqe") fill_energy = Eccqe;
      if (energy == "ecalo1") fill_energy = ecalo1;
      if (energy == "ecalo2") fill_energy = ecalo2;


      
      //use all the events because the weight, eff is already calculated
      weight=wgt; 
      edistrnue->Fill(fill_energy,weight);

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
      data[i]                       = edistrnue           -> GetBinContent(i+1);
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

    t->Fill();
    edistrnue             -> Write();
    eLeptonEnergy           -> Write();
    nue_kNueFromNueCC_muon        -> Write();
    nue_kNueFromNueCC_chargeKaon  -> Write();
    nue_kNueFromNueCC_neutKaon    -> Write();
    nue_kNueFromEScatter          -> Write();
    nue_kNueFromNC_pi0            -> Write();
    nue_kNueFromNC_delta0         -> Write();
    nue_kNueFromNumuCC            -> Write();
    nue_kDirt                     -> Write();
    nue_kOther                    -> Write();
    t               -> Write("tvecnue");
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
    c->SetBranchAddress("edistrnueVec",&readInVector);
    c-> GetEntry(0);
    data = *(readInVector);
    delete readInVector;
    delete c;
    return 0;
  }

  int NtupleReader::LoopNueOsc(){
      

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
    Double_t dm2_high=50.0;
    Double_t dm2min = 0.01, dm2max = 100;
    Double_t dm2BF_glob = 0.43;
    
    //inno ID
    Int_t nue  = 12;
    Int_t nuebar = -12;

    c->Reset();
    c->Add(dummytarget);
    if (c==0) return 5;

    const Int_t nentries2 = Int_t(c->GetEntries());

    std::cout<<"Input filename:\t" << dummytarget << std::endl;
    printf("Number of entries L=%im:\t %i\n",baseline,nentries2);
    c->SetBranchAddress("energy",&fill_energy);
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
      
    TH1F *eventsnueosc[npoints+1];      //fullosc weighted by Posc=1*sin2(1.27dm2L/E), at each dm2 point
    TH1F *eventsnuefosc;          //fullosc (Posc=1)
    TH1F *eventsSignalBestFitNu;      //Number of events expected at the best fit.
    TH1F *eventsSignalBestFitNubar;     //Number of events expected at the best fit.
    TH1F *eventsSignalBestFit_globNu;      //Number of events expected at the best fit.
    TH1F *eventsSignalBestFit_globNubar;     //Number of events expected at the best fit.
    TH1F *eventsSignalHighDmNu;      //Number of events expected at the best fit.
    TH1F *eventsSignalHighDmNubar;     //Number of events expected at the best fit.
    
    int nbins = bins.size() - 1;
    double emin = bins.front();
    double emax = bins.back();

    TH1F *edistrnueosc      = new TH1F("edistrnueosc","Oscillated #nu_{e} event distribution",nbins,emin,emax);
    TH1F *edistrnuefosc     = new TH1F("edistrnuefosc","Fully-oscillated #nu_{e} event distribution",nbins,emin,emax);
    TH1F *edistrnueBestFitNu  = new TH1F("edistrnueBestFitNu", "Signal values of nue appearance at LSND best fit", nbins, emin, emax);
    TH1F *edistrnueBestFitNubar = new TH1F("edistrnueBestFitNubar", "Signal values of nue appearance at LSND best fit", nbins, emin, emax);
    TH1F *edistrnueHighDmNu  = new TH1F("edistrnueHighDmNu", "Signal values of nue appearance at high dm2", nbins, emin, emax);
    TH1F *edistrnueHighDmNubar = new TH1F("edistrnueHighDmNubar", "Signal values of nue appearance at high dm2", nbins, emin, emax);
    TH1F *edistrnueBestFit_globNu  = new TH1F("edistrnueBestFit_globNu", "Signal values of nue appearance at high dm2", nbins, emin, emax);
    TH1F *edistrnueBestFit_globNubar = new TH1F("edistrnueBestFit_globNubar", "Signal values of nue appearance at high dm2", nbins, emin, emax);


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
    edistrnueosc      -> SetBins(nbins, binsArray);
    edistrnuefosc       -> SetBins(nbins, binsArray);
    edistrnueBestFitNu    -> SetBins(nbins, binsArray);
    edistrnueBestFitNubar   -> SetBins(nbins, binsArray);
    edistrnueHighDmNu    -> SetBins(nbins, binsArray);
    edistrnueHighDmNubar   -> SetBins(nbins, binsArray);
    edistrnueBestFit_globNu    -> SetBins(nbins, binsArray);
    edistrnueBestFit_globNubar   -> SetBins(nbins, binsArray);

    Double_t dm2Array[npoints+1];
    //Fill the array of dm2 points:

    for (int i=0; i<=npoints; i++)  //loop over dm2 points
    {
      dm2Array[i] = pow(10., (TMath::Log10(dm2min)+ (i*1./npoints)*TMath::Log10(dm2max/dm2min)) );
      //While looping, may as well set up the histograms:
      eventsnueosc[i] = (TH1F*) edistrnueosc -> Clone();
     }


    //Now loop over events in the tree and fill each histogram for dm2 at each point.

    evtcounter = 0;
    for (ievt=0; ievt<nentries2; ievt++)
    {
      ientry = c->LoadTree(ievt); if (ientry < 0) break; nb = c->GetEntry(ievt); nbytes += nb;
      evtcounter++;

      if ( (ievt%100000) == 0) {
        std::cout << "On event " << ievt << " of " << nentries2 << std::endl;
      }

      if (energy == "elecCand") fill_energy = ElecCandEnergy;
      if (energy == "eccqe") fill_energy = Eccqe;
      if (energy == "ecalo1") fill_energy = ecalo1;
      if (energy == "ecalo2") fill_energy = ecalo2;

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
    
    eventsSignalBestFitNu     = (TH1F*)edistrnueBestFitNu->Clone();
    eventsSignalBestFitNubar  = (TH1F*)edistrnueBestFitNubar->Clone();
    eventsSignalHighDmNu      = (TH1F*)edistrnueHighDmNu->Clone();
    eventsSignalHighDmNubar   = (TH1F*)edistrnueHighDmNubar->Clone();
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
    t1-> Write();
    
    std::cout << "Successfully wrote histograms to file." << std::endl;
    
    f.Close();

    // delete t;
    // delete c;
    // delete t1; 

    return 0;
  }

  int NtupleReader::ReadNueOsc(){

    dataOsc.resize(npoints+1);
    for (unsigned int i = 0; i < dataOsc.size(); i++) dataOsc[i].resize(bins.size()-1);

    for (int ii = 0; ii <= npoints; ii++){
      std::vector <float>   * readInVector = 0;
      char name[100];
      sprintf(name, "edistrnueoscVec_%i",ii);
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
      delete c;
      delete readInVector;
    }
    // //get the other things for fullosc, that only happen once:
    // std::vector <float>   * readInVector1 = 0;
    // // std::vector <float>   * readInVector2 = 0;
    // // std::vector <float>   * readInVector3 = 0;
    // TChain *c = new TChain("tvecfosc");
    // c -> Add(fileNameHistsOsc);
    // // c->SetBranchAddress("edistrnuefoscVec",&readInVector1);
    // // c->SetBranchAddress("edistrnueBestFitNuVec",&readInVector2);
    // // c->SetBranchAddress("edistrnueBestFitNubarVec",&readInVector3);
    // c-> GetEntry(0);
    // eventsnuefoscVec[b_line]      = *(readInVector1);
    // eventsSignalBestFitNuVec[b_line]  = *(readInVector2);
    // eventsSignalBestFitNubarVec[b_line] = *(readInVector3);
    // delete c;
    // delete readInVector1;
    // delete readInVector2;
    // delete readInVector3;

    return 0;
  }

  int NtupleReader::LoopNumu(){
    
    TString dummytarget = path+fileNameSource;
    TChain *c = new TChain("EventsTot");
    Double_t fill_energy,wgt;
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
    nbytes = 0;
    evtcounter = 0;
    if (c==0) return 5;

    const Int_t nentries1 = Int_t(c->GetEntries());

    std::cout<<"Input filename:\t" << dummytarget << std::endl;
    std::cout << "Number of entries at L= " << baseline << ":\t" << nentries1 << std::endl;
    c->SetBranchAddress("energy",&fill_energy);
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

    // Need these for the histograms:
    double emin = bins.front();
    double emax = bins.back();
    int nbins = bins.size() - 1;

    // This histogram holds the total nue hist.
    TH1F * edistrnumu     = new TH1F("edistrnumu","Background #nu_{\{mu}} event distribution",nbins,emin,emax);
    TH1F * muLeptonEnergy   = new TH1F("muLeptonDist", "Distribution of muon energies.", nbins, 0.0, emax);

    std::vector<float> edistrnumuVec(nbins,0);
    std::vector<float> muLeptonEnergyVec(nbins, 0);
    data.resize(nbins);

    double binsArray[nbins+1];
    for (int i = 0; i < nbins+1; i ++){
      binsArray[i] = bins[i];
    }

    edistrnumu -> SetBins(nbins, binsArray);
    muLeptonEnergy -> SetBins(nbins, binsArray);

    for (ievt=0; ievt<nentries1; ievt++)
    {
      ientry = c->LoadTree(ievt); if (ientry < 0) break; nb = c->GetEntry(ievt); nbytes += nb;
      evtcounter++;

      if (energy == "elecCand") fill_energy = ElecCandEnergy;
      if (energy == "eccqe")    fill_energy = Eccqe;
      if (energy == "ecalo1")   fill_energy = ecalo1;
      if (energy == "ecalo2")   fill_energy = ecalo2;

      //use all the events because the weight, eff is already calculated
      weight=wgt; 
      edistrnumu    -> Fill(fill_energy,weight);
      muLeptonEnergy  -> Fill(Elep, weight);
      
    }//end loop over events

    if (evtcounter!=nentries1){ printf("Events loop error! Exiting...\n"); return 4; }
    
    
    //fill the vectors with the results from filling the histograms;
    for (int i = 0; i < nbins;  i++){
      edistrnumuVec[i]     = edistrnumu     -> GetBinContent(i+1);
      muLeptonEnergyVec[i] = muLeptonEnergy -> GetBinContent(i+1);
      data[i]              = edistrnumu     -> GetBinContent(i+1);
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



    t->Fill();
    edistrnumu    -> Write();
    muLeptonEnergy  -> Write();
    t       -> Write("tvecnumu");
    f -> Close();
    //cleaning up:
    delete edistrnumu;          
    delete muLeptonEnergy;        

    // delete t;  
    delete f;
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
    printf("Number of entries L=%im:\t %i\n",baseline,nentries2);
    c->SetBranchAddress("energy",&fill_energy);
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

    TH1F *edistrnumuosc      = new TH1F("edistrnumuosc","Oscillated #nu_{e} event distribution",nbins,emin,emax);
    TH1F *edistrnumufosc     = new TH1F("edistrnumufosc","Fully-oscillated #nu_{e} event distribution",nbins,emin,emax);
    TH1F *edistrnumuBestFitNu  = new TH1F("edistrnumuBestFitNu", "Signal values of numu appearance at LSND best fit", nbins, emin, emax);
    TH1F *edistrnumuBestFitNubar = new TH1F("edistrnumuBestFitNubar", "Signal values of numu appearance at LSND best fit", nbins, emin, emax);


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

      if ( (ievt%100000) == 0) std::cout << "On event " << ievt << " of " << nentries2 << std::endl;

      if (energy == "elecCand") fill_energy = ElecCandEnergy;
      if (energy == "eccqe")    fill_energy = Eccqe;
      if (energy == "ecalo1")   fill_energy = ecalo1;
      if (energy == "ecalo2")   fill_energy = ecalo2;

      ientry = c->LoadTree(ievt); if (ientry < 0) break; nb = c->GetEntry(ievt); nbytes += nb;
      evtcounter++;
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
    c-> GetEntry(0);
    data = *(readInVector);
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
      delete c;
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
    delete c;
    return *(readInVector);
  }

}


