#include "EffCalc.hh"


namespace lar1{

  EffCalc::EffCalc(){

    legend.reserve(13);
    legend.push_back("low");
    legend.push_back("high");
    legend.push_back("mu");
    legend.push_back("k+");
    legend.push_back("k0");
    legend.push_back("nuscat");
    legend.push_back("pi0");
    legend.push_back("1gam");
    legend.push_back("numucc");
    legend.push_back("dirt");
    legend.push_back("cosmic");
    legend.push_back("other");
    legend.push_back("signal");

    nominalData.resize(legend.size());

    fileStart = "nue_appearance_ecalo2_";
    fileEnd   = "100m_nu_globBF.txt";

  }

  EffCalc::~EffCalc(){}

  void EffCalc::getNominalData()
  {

    TString fileName = path;
    fileName += fileStart;
    fileName += fileEnd;

    if (!sensUtils.fileExists(fileName)){
      std::cerr << "File " << fileName << " doesn't exist!\n";
      exit(-1);
    }

    int n_bins = parseData(fileName, nominalData);

    // Set up the bins here now that there is nominal data:
    bins.reserve(n_bins+1);
    for (auto & vec : nominalData) 
      bins.push_back(vec.front());

    // get the last bin ...
    bins.push_back(nominalData.back().at(1));

    return;


  }

  int EffCalc::readData(  std::vector<std::vector<float> > & data,
                          double photonGap, 
                          double minVertexEnergyPhoton, 
                          double minVertexEnergySignal)
  {
   
    TString fileName = path;
    fileName += fileStart;

    char tempstring[100];

    if (minVertexEnergyPhoton != 10000 ) {
      sprintf(tempstring, "vePhot%g_",minVertexEnergyPhoton );
      fileName += tempstring;
    }
    if (photonGap != 10000 )  { 
      sprintf(tempstring, "gap%g_",photonGap );
      fileName += tempstring;
    }
    if (minVertexEnergySignal != 0 ) {
      sprintf(tempstring, "veSig%g_",minVertexEnergySignal );
      fileName += tempstring;
    }

    fileName += fileEnd;

    if (!sensUtils.fileExists(fileName)){
      std::cerr << "File " <<fileName<< " doesn't exist!\n";
      exit(-1);
    }

    return parseData(fileName, data);

  }

  int EffCalc::parseData(TString fileName, std::vector<std::vector<float> > & data)
  {
    // open the file:
    ifstream myfile (fileName.Data());
    std::string line;
    int nBins = -1;

    // properly format the output:
    data.clear();
    std::vector<float> templateRow;
    templateRow.resize(legend.size());
    data.reserve(20);

    if (myfile.is_open())
    {
      while ( getline (myfile,line) )
      {
        if (nBins == -1){
          nBins ++;
          continue;
        }
        else nBins ++;

        data.push_back(templateRow);

        std::vector<std::string> tempStrings;
        plotUtils.split(tempStrings,line,'\t');
        for (unsigned int s =0; s< tempStrings.size(); s++) {
          std::stringstream s_stream(tempStrings.at(s));
          s_stream >> data.back().at(s);
        }

      }
      myfile.close();
    }
  
    else std::cout << "Unable to open file"; 

    return nBins;
  }


  TH1F * EffCalc::makeEfficiency( int channel,
                                  double photonGap, 
                                  double minVertexEnergyPhoton, 
                                  double minVertexEnergySignal){

    // get a vector to hold the data that's coming:
    std::vector<std::vector<float> > data;
    int nbins = readData(data,photonGap,minVertexEnergyPhoton,minVertexEnergySignal);

    if (nbins != data.size() && nominalData.size() != data.size()){
      std::cerr << "error, the bins are mismatched.  jgkldhfajkdsf\n";
      std::cerr << "Nbins: " << nbins << std::endl;
      std::cerr << "Nbins data: " << data.size() << std::endl;
      exit(-1);
    }

    static int fuckRoot = 0;
    fuckRoot ++;
    char temp[20];
    sprintf(temp,"eff_%d",fuckRoot);

    TH1F * eff = new TH1F(temp,"",nbins,&(bins[0]));

    for (unsigned int i = 0; i < nominalData.size(); i++){
      eff -> SetBinContent(i+1, data.at(i).at(channel)/nominalData.at(i).at(channel));
    }

    eff -> SetTitle(legend[channel]);
    eff -> GetYaxis() -> SetTitle("Efficiency");
    eff -> GetXaxis() -> SetTitle("Energy (GeV)");

    eff -> SetMaximum(1);
    eff -> SetMinimum(0);
    // eff -> Draw();

    std::cout << "Trying to retunr ... \n";

    return eff;
  }



}