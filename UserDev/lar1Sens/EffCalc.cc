#include "EffCalc.hh"


namespace lar1{

  EffCalc::EffCalc(){

    legend.reserve(13);
    legend.push_back("low bin");
    legend.push_back("high bin");
    legend.push_back("#nu_{e} from  #mu");
    legend.push_back("#nu_{e} from K^{ +}");
    legend.push_back("#nu_{e} from K^{ 0}");
    legend.push_back("#nu - e scattering");
    legend.push_back("#pi^{0} misID");
    legend.push_back("NC #gamma MisID");
    legend.push_back("#nu_{#mu} CC MisID");
    legend.push_back("Dirt");
    legend.push_back("Cosmic");
    legend.push_back("Other");
    legend.push_back("Signal");

    nominalData.resize(legend.size());

    fileStart = "nue_appearance_ecalo2_nu_";
    detector  = "100m";
    fileEnd   = "_nu_globBF.txt";

  }

  EffCalc::~EffCalc(){}

  void EffCalc::getNominalData()
  {

    TString fileName = path;
    fileName += fileStart;
    fileName += detector;
    fileName += fileEnd;

    if (!sensUtils.fileExists(fileName)){
      std::cerr << "File " << fileName << " doesn't exist!\n";
      exit(-1);
    }

    parseData(fileName, nominalData);

    // Set up the bins here now that there is nominal data:
    bins = nominalData.front();

    // get the last bin ...
    bins.push_back(nominalData.at(1).back());

    // Set up the nominal bin and bkg integrals:
    nominalBkgIntegral.resize(nominalData.size());
    nominalBinIntegral.resize(nominalData.front().size());

    for (unsigned int i_bkg = 2; i_bkg < nominalData.size(); i_bkg++){
      for (unsigned int i_bin = 0; i_bin < nominalData.front().size(); i_bin++){
        nominalBinIntegral.at(i_bin) += nominalData.at(i_bkg).at(i_bin)
                                      / (bins.at(i_bin+1) - bins.at(i_bin));
        nominalBkgIntegral.at(i_bkg) += nominalData.at(i_bkg).at(i_bin)
                                      / (bins.at(i_bin+1) - bins.at(i_bin));
      }

    }

    // std::cout << "Nominal Data:\n";
    // // Just for debugging, print out the data:
    // for (unsigned int i_bkg = 2; i_bkg < nominalData.size(); ++i_bkg)
    // {
    //   std::cout << legend.at(i_bkg) << ": \t";
    //   for (auto item : nominalData.at(i_bkg)){
    //     std::cout << item << "\t";
    //   }
    //   std::cout << std::endl;
    // }
    // std::cout << "\n";


    return;


  }



  unsigned int EffCalc::readData( std::vector<std::vector<float> > & data,
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

    fileName += detector;
    fileName += fileEnd;

    if (!sensUtils.fileExists(fileName)){
      std::cerr << "File " <<fileName<< " doesn't exist!\n";
      exit(-1);
    }

    return parseData(fileName, data);

  }

  unsigned int EffCalc::parseData(TString fileName, std::vector<std::vector<float> > & data)
  {
    // open the file:
    ifstream myfile (fileName.Data());
    std::string line;
    unsigned int nBins = 0;

    // properly format the output:
    data.clear();
    std::vector<float> templateRow;
    templateRow.resize(legend.size());
    data.reserve(20);

    std::vector< std::vector<float> > tempData;

    bool isFirst = true;

    if (myfile.is_open())
    {
      while ( getline (myfile,line) )
      {
        if (isFirst){
          isFirst = false;
          continue;
        }
        else nBins ++;

        tempData.push_back(templateRow);

        std::vector<std::string> tempStrings;
        plotUtils.split(tempStrings,line,'\t');
        for (unsigned int s =0; s< tempStrings.size(); s++) {
          std::stringstream s_stream(tempStrings.at(s));
          s_stream >> tempData.back().at(s);
        }

      }
      myfile.close();
    }
    else std::cout << "Unable to open file"; 
  
    // tempData is right now tempData[bin][background]
    // I want to transpose it:
    data.resize(tempData.front().size());
    for (unsigned int temp_y = 0; temp_y < tempData.front().size(); temp_y++){
      for (unsigned int temp_x = 0; temp_x < tempData.size(); temp_x ++){
        data.at(temp_y).push_back(tempData.at(temp_x).at(temp_y));
      }
    }



    return nBins;
  }


  TH1F * EffCalc::makeEfficiency( int channel,
                                  double photonGap, 
                                  double minVertexEnergyPhoton, 
                                  double minVertexEnergySignal){

    // get a vector to hold the data that's coming:
    std::vector<std::vector<float> > data;
    unsigned int nbins = readData(data,photonGap,minVertexEnergyPhoton,minVertexEnergySignal);

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
      eff -> SetBinContent(i+1, data.at(channel).at(i)/nominalData.at(channel).at(i));
    }

    eff -> SetTitle(legend[channel]);
    eff -> GetYaxis() -> SetTitle("Efficiency");
    eff -> GetXaxis() -> SetTitle("Energy (GeV)");

    eff -> SetMaximum(1);
    eff -> SetMinimum(0);

    return eff;
  }

  TH1F* EffCalc::makeNueEfficiency( double photonGap, 
                                    double minVertexEnergyPhoton, 
                                    double minVertexEnergySignal)
  {

    // This function digs out the nues in the samples and treats them all as one

    // get a vector to hold the data that's coming:
    std::vector<std::vector<float> > data;
    unsigned int nbins = readData(data,photonGap,minVertexEnergyPhoton,minVertexEnergySignal);

    if (nbins != data.size() && nominalData.size() != data.size()){
      std::cerr << "error, the bins are mismatched.  jgkldhfajkdsf\n";
      std::cerr << "Nbins: " << nbins << std::endl;
      std::cerr << "Nbins data: " << data.size() << std::endl;
      exit(-1);
    }
    

    static int fuckRoot = 0;
    fuckRoot ++;
    char temp[20];
    sprintf(temp,"nue_eff_%d",fuckRoot);

    TH1F * eff = new TH1F(temp,"",nbins,&(bins[0]));

    for (unsigned int i = 0; i < nominalData.front().size(); i++){
      double tempData = data.at(3).at(i) 
                      + data.at(4).at(i) 
                      + data.at(5).at(i);
      double tempNom  = nominalData.at(3).at(i)
                      + nominalData.at(4).at(i)
                      + nominalData.at(5).at(i);
      eff -> SetBinContent(i+1, tempData / tempNom);
    }

    eff -> SetTitle("Total #nu_{e} Efficiency");
    eff -> GetYaxis() -> SetTitle("Efficiency");
    eff -> GetXaxis() -> SetTitle("Energy (GeV)");

    eff -> SetMaximum(1);
    eff -> SetMinimum(0);

    return eff;
  }

  TH1F* EffCalc::makePhotonEfficiency(double photonGap, 
                                      double minVertexEnergyPhoton, 
                                      double minVertexEnergySignal)
  {
    // This function digs out the nues in the samples and treats them all as one

    // get a vector to hold the data that's coming:
    std::vector<std::vector<float> > data;
    unsigned int nbins = readData(data,photonGap,minVertexEnergyPhoton,minVertexEnergySignal);

    if (nbins != data.size() && nominalData.size() != data.size()){
      std::cerr << "error, the bins are mismatched.  jgkldhfajkdsf\n";
      std::cerr << "Nbins: " << nbins << std::endl;
      std::cerr << "Nbins data: " << data.size() << std::endl;
      exit(-1);
    }

    static int fuckRoot = 0;
    fuckRoot ++;
    char temp[20];
    sprintf(temp,"phot_eff_%d",fuckRoot);

    TH1F * eff = new TH1F(temp,"",nbins,&(bins[0]));

    for (unsigned int i = 0; i < nominalData.front().size(); i++){
      double tempData = data.at(7).at(i) 
                      + data.at(8).at(i);
      double tempNom  = nominalData.at(7).at(i)
                      + nominalData.at(8).at(i);
      eff -> SetBinContent(i+1, tempData / tempNom);
    }

    eff -> SetTitle("Total Photon Efficiency");
    eff -> GetYaxis() -> SetTitle("Efficiency");
    eff -> GetXaxis() -> SetTitle("Energy (GeV)");

    eff -> SetMaximum(1);
    eff -> SetMinimum(0);

    return eff;
  }


  TCanvas * EffCalc::createBackground(double photonGap, 
                                      double minVertexEnergyPhoton, 
                                      double minVertexEnergySignal,
                                      bool normToNominal)
  {
    // Utilize the functions to read in the data to make nice looking histograms
    // of the backgrounds.
    
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

    // TCanvas *c1 = new TCanvas("c1","Example 2 pads (20,80)",200,10,300,500)

    


    TCanvas * c1 = new TCanvas(Form("sc_%s",detector.Data()), "Stacked event rates", 600, 800);
    TPad *pad1 = new TPad("pad1", "The pad 80% of the height",0.0,0.2,1.0,1.0);
    TPad *pad2 = new TPad("pad2", "The pad 20% of the height",0.0,0.0,1.0,0.2);
    pad1->Draw();
    pad2->Draw();
    
    pad1 -> cd();

    // c1 -> cd(1,2);

    TString nueTitle = "#nu_{e} Events at L = ";
    nueTitle += detector;
    THStack * stack = new THStack("nue",nueTitle);

    TLegend * plotLegend = new TLegend(0.7,0.45,.85,0.85);
    plotLegend -> SetTextFont(72);

    // Need to hard code colors for the histograms:
    std::vector<int> colors;
    colors.push_back(0);  // low bins
    colors.push_back(0);  // high bins
    colors.push_back(kGreen+3);
    colors.push_back(kGreen+2);
    colors.push_back(kGreen-6);
    colors.push_back(kOrange-8);
    colors.push_back(kOrange-8);
    colors.push_back(kBlue);
    colors.push_back(kBlue-9);
    colors.push_back(15);
    colors.push_back(8);
    colors.push_back(kPink);
    colors.push_back(0); //  signal

    // Going to use the nominal data to set the scale of the histogram:
    
    if (nominalData.size() == 0) getNominalData();

    // Loop over the different backgrounds and get the data for each background:

    std::vector < std::vector<float> > totalData;
    totalData.resize(legend.size());

    std::vector < bool > hasContent(legend.size(), false);

    // std::vector< std::vector<float> > tempData;
    readData( totalData, photonGap,
              minVertexEnergyPhoton,
              minVertexEnergySignal);

    // for (auto bin : bins) std::cout << "Bin is " << bin << std::endl;

    std::vector<float> sig(totalData.front().size(),0);
    std::vector<float> bkg(totalData.front().size(),0);

    for (unsigned int i_bkg = 2; i_bkg < totalData.size(); i_bkg ++ ){
      for (unsigned int i_bin = 0; i_bin < totalData.front().size(); i_bin++){
        if (i_bkg != totalData.size()-1){
          bkg[i_bin] += totalData.at(i_bkg).at(i_bin);
        }
        else{
          sig[i_bin] += totalData.at(i_bkg).at(i_bin);
        }
      }
    }

    // Normalize all of the events by bin width:
    for (unsigned int i_bkg = 2; i_bkg < totalData.size(); ++i_bkg)
    {
      for (unsigned int i_bin = 0; i_bin < totalData.front().size();i_bin ++){
        double temp = totalData.at(i_bkg).at(i_bin);
        if (temp == 0) continue;
        else{
          if (!hasContent.at(i_bkg)) hasContent.at(i_bkg) = true;
          totalData.at(i_bkg).at(i_bin) = temp / (bins.at(i_bin+1) - bins.at(i_bin));
        }

      }
    }

    // Just for debugging, print out the data:
    for (unsigned int i_bkg = 2; i_bkg < totalData.size(); ++i_bkg)
    {
      std::cout << legend.at(i_bkg) << ": \t";
      for (auto item : totalData.at(i_bkg)){
        std::cout << item << "\t";
      }
      std::cout << std::endl;
    }


    std::vector<TH1F *> histograms;
    for (unsigned int i_bkg = 2; i_bkg < totalData.size(); ++i_bkg)
    {
      histograms.push_back(sensUtils.makeHistogram(totalData.at(i_bkg),bins));
      histograms.back() -> SetFillColor(colors[i_bkg]);
      if (hasContent.at(i_bkg)){
        if (i_bkg == 7) continue;
        stack -> Add(histograms.back());
        plotLegend -> AddEntry(histograms.back(), legend.at(i_bkg));
      }
    }

    plotLegend -> SetTextSize(0.04);
    plotLegend -> SetFillStyle(0);
    plotLegend -> SetFillColor(0);
    plotLegend -> SetBorderSize(0);


    stack -> Draw();



    // Find the maximum:
    float max(0.0);
    if (normToNominal) max = sensUtils.getMaximum(nominalBinIntegral);
    else max = 1.4* stack ->GetMaximum();

    stack-> SetMaximum(1.4*max);
    stack->GetHistogram() -> GetYaxis()->SetTitle("Events / MeV");
    stack->GetHistogram() -> GetYaxis()->SetTitleSize(0.06);
    stack->GetHistogram() -> GetYaxis()->SetTitleOffset(0.9);
    stack->GetHistogram() -> GetYaxis()->CenterTitle();
    stack->GetHistogram() -> GetXaxis()->CenterTitle();
    stack->GetHistogram() -> GetXaxis()->SetLabelSize(0.05);
    stack->GetHistogram() -> GetXaxis()->SetTitle("Reconstructed Energy (GeV)");
    stack->GetHistogram() -> GetXaxis()->SetTitleOffset(0.95);
    stack->GetHistogram() -> GetXaxis()->SetTitleSize(0.07);
    stack->GetHistogram() -> GetXaxis()->SetLimits(bins.front()-0.01,bins.back());
    stack->Draw("hist");


    plotUtils.add_plot_label((char*)"Statistical Uncertainty Only",0.2,0.76, 0.04,1,62,12);
    plotUtils.add_plot_label((char*)"PRELIMINARY",0.2,0.71, 0.04, kRed-3,62,12);

    plotLegend -> Draw();

    pad2 -> cd();

    // On this pad I'll plot signal / sqrt(B)
    // Divide signal by background:
    for (unsigned int i_bin = 0; i_bin < sig.size(); i_bin ++){
      if (bkg.at(i_bin)!= 0)
        sig.at(i_bin) = sig.at(i_bin) / sqrt(bkg.at(i_bin));
      else
        sig.at(i_bin) = 0;
    }

    TH1F * sigHist = sensUtils.makeHistogram(sig,bins);
    sigHist -> SetLineColor(1);
    sigHist -> SetTitle("");
    sigHist -> GetYaxis() -> SetTitle("#sigma_{stat.}");
    sigHist -> GetYaxis() -> SetTitleSize(0.2);
    sigHist -> GetYaxis() -> SetTitleOffset(0.2);
    sigHist -> GetYaxis() -> CenterTitle();
    sigHist -> GetYaxis() -> SetLabelSize(0.1);
    sigHist -> Draw();



    return c1;
    
  }

}