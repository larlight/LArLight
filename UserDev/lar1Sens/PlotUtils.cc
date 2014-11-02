


#include "PlotUtils.hh"

namespace lar1{

  // PlotUtils::PlotUtils(){}

  void PlotUtils::lsnd_plot(TPad * c){
    c->cd();

    std::string path = GetEnv("MAKE_TOP_DIR");
    path.append("/UserDev/lar1Sens/");
    std::string data_dir= path;
    data_dir.append("lsnd_data/");

    // const char* data_dir = "lsnd_data/";
    Double_t  dm2BF[] = {1.2};
    Double_t sin22thBF[] = {0.003};

    const Int_t NDATAFILES = 11;
    const char * file_list[NDATAFILES] = {"llreg_608_1.vec",
                 "llreg_608_2.vec",
                 "llreg_608_3.vec",
                 "llreg_607_1.vec",
                 "llreg_607_2.vec",
                 "llreg_607_3.vec",
                 "llreg_607_4.vec",
                 "llreg_607_5.vec",
                 "llreg_607_6.vec",
                 "llreg_607_7.vec",
                 "llreg_607_8.vec"};

    Int_t graph_color[NDATAFILES] = {29, 29, 29, 38, 38, 38, 38, 38, 38, 38, 38};

    Int_t    nlines;
    Double_t x[500],y[500];
    Double_t dummy, dummy_old;
    TGraph* gr[NDATAFILES];
    for (Int_t ifile = 0; ifile<NDATAFILES; ifile++) {
      nlines = 0;
      for (Int_t i=0;i<500;i++){x[i]=0.0;y[i]=0.0;}
      char  filename[200];
      strcpy(filename, data_dir.c_str());
      //printf("%s\n",filename);
      strcat(filename, file_list[ifile]);
      //printf("%s\n",filename);
      ifstream datafile;
      datafile.open(filename, std::ios_base::in);
      //check if the file is open: 
      if (!datafile.is_open() ) {std::cerr << "lsnd_plot.C: file not opened" <<std::endl; return;}
      //else {std::cout << "Successfully opened " << filename << std::endl;}
      while (!datafile.eof()) {
        datafile >> dummy; 
        datafile >> dummy; 
        datafile >> x[nlines]; 
        datafile >> y[nlines];
        nlines++;
        if (dummy == dummy_old) nlines--; //if last row was empty
        dummy_old = dummy;
      }

      gr[ifile] = new TGraph(nlines,x,y);
      datafile.close();
    }
    std::cout << "Finished reading data files" << std::endl;
    for (Int_t ifile = 0; ifile<NDATAFILES; ifile++) {
      // Int_t graph_color[NDATAFILES] = {29, 29, 29, 38, 38, 38, 38, 38, 38, 38, 38};
      gr[ifile]->SetFillColor(graph_color[ifile]);
      gr[ifile]->Draw("LF");
    }
    //Add the best fit point;
    TGraph * bfPoint = new TGraph(1, sin22thBF, dm2BF);
    bfPoint -> SetLineColor(2);
    bfPoint -> SetMarkerStyle(3);
    bfPoint -> SetMarkerColor(1);
    bfPoint -> Draw("LP");

    plotGFData(c);

    return;
  }

  // adds a plot label to the current canvas
  void PlotUtils::add_plot_label(char* label, double x, double y,
                                 double size , int color,
                                 int font, int align){

    TLatex *latex = new TLatex( x, y, label );
    latex->SetNDC();
    latex->SetTextSize(size);
    latex->SetTextColor(color);
    latex->SetTextFont(font);
    latex->SetTextAlign(align);
    latex->Draw();
  }

  TLegend * PlotUtils::getLSNDLegend(){

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

    return leg3;

  }

  TH2D* PlotUtils::getEmptySensPlot(double sin22thmin,
                                    double sin22thmax,
                                    double dm2min,
                                    double dm2max)
  {
    gStyle->SetOptStat(0000);
    gStyle->SetOptFit(0000);
    // gStyle->SetPadBorderMode(0);
    gStyle->SetPadBottomMargin(0.16);
    gStyle->SetPadLeftMargin(0.16);
    gStyle->SetPadRightMargin(0.05);
    gStyle->SetFrameBorderMode(0);
    // gStyle->SetCanvasBorderMode(0);
    gStyle->SetPalette(0);
    gStyle->SetCanvasColor(0);
    gStyle->SetPadColor(0);

    TH2D* hr1=new TH2D("hr1","hr1",500,sin22thmin,sin22thmax,500,dm2min,dm2max);
    hr1->Reset();
    hr1->SetFillColor(0);
    hr1->SetTitle(";sin#lower[-.6]{#scale[.6]{2}}2#theta#lower[.4]{#scale[.6]{#mue}};#Deltam#lower[-.6]{#scale[.6]{2}}#lower[.4]{#scale[.6]{41}} (eV#lower[-.7]{#scale[.6]{2}})");
    hr1->GetXaxis()->SetTitleOffset(1.1);
    hr1->GetYaxis()->SetTitleOffset(1.2);
    hr1->GetXaxis()->SetTitleSize(0.05);
    hr1->GetYaxis()->SetTitleSize(0.05);
    hr1->GetXaxis()->CenterTitle();
    hr1->GetYaxis()->CenterTitle();
    hr1->SetStats(kFALSE);
    return hr1;
  }

  int PlotUtils::plotGFData(TPad * c ){
    std::vector<std::vector<double> > data = readGFData();

    // std::cout << "Printing out the chi2 map:\n";
    // for (int dm2point = 0; dm2point < data.size(); dm2point ++){
    //   for (int sin22thpoint = 0; sin22thpoint < data[0].size(); sin22thpoint ++){
    //     std::cout << "data["<<dm2point<<"]["<<sin22thpoint<<"]: " 
    //               << data[dm2point][sin22thpoint] << "\n";
    //   }
    //   std::cout << std::endl;
    // }

    int npoints = 250;

    double min = getMinimum2D(data);
    double bound90 = min + 4.605;
    double xbins[npoints+1], ybins[npoints+1];

    for (int x_index = 0; x_index <= npoints; x_index ++ )
    {
      xbins[x_index] = pow(10.0,(x_index*0.016 - 4.01));
      // std::cout << xbins[x_index] << "\n";
    }
    std::cout << "\n" << std::endl;
    for (int y_index = 0; y_index <= npoints; y_index ++ )
    {
      ybins[y_index] = pow(10.0,(y_index*0.0112 - 1.4));
      // std::cout << ybins[y_index] << "\n";

    }

    // Want to find the bottom and top edge of these regions.
    std::vector<double> bottomEdge;
    std::vector<double> topEdge;

    // Want to loop over dm2 points and get the edge points there
    // that's the first index in the data
    std::cout << "Looking for bound90 = " << bound90 << " and min = " << min << std::endl;
    for (unsigned int dm2index = 0; dm2index < data.size() ; dm2index ++){
      bottomEdge.push_back(0.0);
      topEdge.push_back(0.0);    
      
      // now loop over the data inside this row of dm2 data
      for (unsigned int sin22thindex = 1;
           sin22thindex < data[dm2index].size() -1;
           sin22thindex ++)
      {
        double thisData = data[dm2index][sin22thindex];
        // double nextData = data[dm2index][sin22thindex+1];
        // double prevData = data[dm2index][sin22thindex-1];

        if ( thisData != 0 && thisData < bound90)
        {
          bottomEdge.back() = sin22thindex-1;
          // double dm2 = pow(10.0,(dm2index*0.05 - 1.4));
          // double sin22th = pow(10.0,(sin22thindex*0.1 - 4.01));
          // std::cout << "Found Crossing 1 at (" << dm2index 
          //           << ", " << sin22thindex << ") = (" << dm2 
          //           << ", " << sin22th << "): "  << thisData << "\n";
          break;
        }
        
      }
      for (unsigned int sin22thindex = data[dm2index].size() -1;
           sin22thindex > 0;
           sin22thindex --)
      {
        double thisData = data[dm2index][sin22thindex];
        // double nextData = data[dm2index][sin22thindex+1];
        // double prevData = data[dm2index][sin22thindex+1];
        if ( thisData != 0 && thisData < bound90) 
        {
          // double dm2 = pow(10.0,(dm2index*0.05 - 1.4));
          // double sin22th = pow(10.0,(sin22thindex*0.1 - 4.01));
          // std::cout << "Found Crossing 2 at (" << dm2index 
          //           << ", " << sin22thindex << ") = (" << dm2 
          //           << ", " << sin22th << "): "  << thisData << "\n";
          topEdge.back() = sin22thindex+1;
          break;
        }
      }

    }

    double xpoints[2*bottomEdge.size()], ypoints[2*bottomEdge.size()];
    // double xpoints_top[bottomEdge.size()], ypoints_top[bottomEdge.size()];
    int counter = 0;
    // int bottom_counter = 0;
    for(unsigned int dm2index = 0; dm2index < bottomEdge.size() ; dm2index ++)
    {
      if (bottomEdge.at(dm2index) != 0.0){
        // std::cout << "Found nonzero value 1: " << bottomEdge.at(dm2index) << std::endl;
        ypoints[counter] = pow(10.0,(dm2index*0.05 - 1.4));
        xpoints[counter] = pow(10.0,(bottomEdge[dm2index]*0.1 - 4.01));
        counter ++;
      }
    }
    for(unsigned int dm2index = bottomEdge.size() -1; dm2index > 0; dm2index --)
    {
      if (topEdge.at(dm2index) != 0.0) {
        // std::cout << "Found nonzero value 2: " << topEdge.at(dm2index) << std::endl;
        ypoints[counter] = pow(10.0,(dm2index*0.05 - 1.4));
        xpoints[counter] = pow(10.0,(topEdge[dm2index]*0.1 - 4.01));
        counter ++;
      }
    }

    // Smooth out the y variations in the plot.
    // Only change values if they are above or below both of their neighbors
    // Alost add a point at the end, in the next dm2 spot, to help the smoothness
    double smoothedCurve_x[counter+1], smoothedCurve_y[counter+1];
    smoothedCurve_x[0] = xpoints[0]; smoothedCurve_y[0] = ypoints[0];
    for (int index = 1; index < counter; index++){
      // see if the y on both sides is higher
      if (xpoints[index] <= xpoints[index+1] &&
          xpoints[index] <= xpoints[index-1])
      {
        // Average the two points on either side
        // std::cout << "Averaging at " << 
        smoothedCurve_x[index] = 0.5*(xpoints[index-1] + xpoints[index+1]);
      }
      // See if both sides are lower
      else if (xpoints[index] >= xpoints[index+1] &&
               xpoints[index] >= xpoints[index-1])
      {
        smoothedCurve_x[index] = 0.5*(xpoints[index-1] + xpoints[index+1]);
      } 
      else { // Don't do any smoothing in this case.
        smoothedCurve_x[index] = xpoints[index];
      }
      // in all cases, set the x point to the same value
      smoothedCurve_y[index] = ypoints[index];
    }

    // for (int index = 0; index < counter; index++){
    //   std::cout << "x: " << xpoints[index] 
    //             << ", y: " << ypoints[index] << std::endl;
    // }
    // for (int index = 0; index < bottom_counter; index++){
    //   std::cout << "Bottom x: " << xpoints_bottom[index] 
    //             << ", bottom y: " << ypoints_bottom[index] << std::endl;
    // }

    TGraph * bottom = new TGraph(counter, smoothedCurve_x, smoothedCurve_y);
    // TGraph * top = new TGraph(counter, xpoints, ypoints);

    // bottom -> Merge(top);

    TH2D * chi2hist = new TH2D("chi2","chi2", 250, xbins, 250,ybins);


    // for (int i = 0; i <= 40; i++){
    //   std::cout << "data[5]["<<i<<"]: " << data[5][i] << std::endl;
    // }




    // std::cout << "\t";
    // for (int j = 0; j <= 41; ++j) std::cout << std::fixed << (j*0.1 - 4.01) << " \t";
    // std::cout << "\n";
    for (int i = 0; i <= 56; i++){
      // std::cout << std::fixed << (i*0.05 - 1.4);
      for (int j = 0; j <= 40; ++j){
        // if (data[i][j] >= min && data[i][j] < bound90)
        // {
          double sin22th = pow(10.0,(j*0.1 - 4.01));
          double dm2 = pow(10.0,(i*0.05 - 1.4));
          chi2hist -> Fill(sin22th,dm2,data[i][j]);
          // std::cout << "Filling " << data[i][j] << " at x = " << sin22th 
          // << ", y = " << dm2 << std::endl;
        // }
      }
      // std::cout << "\n";
    }

    std::cout << "Max of plot: " << chi2hist -> GetMaximum() << std::endl;
    std::cout << "Min of plot: " << chi2hist -> GetMinimum() << std::endl;

    // TCanvas * d = new TCanvas("dumb","dumber", 500, 500);

    bottom -> SetFillColor(1);
    bottom -> SetFillStyle(3254);
    // bottom -> SetLineColor(46);
    // bottom -> GetFillColor() -> SetAlpha(0.5);

    // auto arr = chi2hist -> GetArray();
    // chi2hist -> Smooth();
    // chi2hist -> Smooth();
    // chi2hist -> Draw("cont list same");
    bottom -> Draw("CF same");
    double sin22thBF[1] = {0.013};
    double dm2BF[1] = {0.42};
    TGraph * bfPoint = new TGraph(1, sin22thBF, dm2BF);
    bfPoint -> SetLineColor(2);
    bfPoint -> SetMarkerStyle(34);
    bfPoint -> SetMarkerColor(1);
    bfPoint -> Draw("LP");
    // top -> Draw("L same");

    return 0;

  }

  std::vector<std::vector<double > >  PlotUtils::readGFData(){
    std::vector<std::vector<double> > nullData;
    std::string path = GetEnv("MAKE_TOP_DIR");
    path.append("/UserDev/lar1Sens/");
    std::string data_file= path;
    data_file.append("lsnd_data/dm41th14th24-app.dat");
    // const char* data_file = "lsnd_data/dm41th14th24-app.dat";
    ifstream datafile;
    std::cout << "Trying to open Global Best Fit data from: \n"
              << "  " << data_file << std::endl;
    datafile.open(data_file.c_str(), std::ios_base::in);
    //check if the file is open: 
    if (!datafile.is_open() ) {std::cerr << "readGFData.C: file not opened" <<std::endl; return nullData;}
    //else {std::cout << "Successfully opened " << filename << std::endl;}

    int i = 0;

    std::vector<std::string> legend;
    legend.push_back("DM41");
    legend.push_back("s22thmue");
    legend.push_back("Um4");
    legend.push_back("chi2_NH");
    legend.push_back("chi2_IH");

    // Setting up a container to hold the chi2 values.
    // There are 57 dm14 points, from -1.4 to 1.4 in steps of 0.05 inclusive;
    // There are 41 sin22th points from -4.01 to -0.01 in steps of 0.1 inclusive;
    // at each point, we marginalize over any Um4 or chi2_IH/chi2_NH
    std::vector< std::vector<double > > chi2map(57, std::vector<double>(41, 0.0) );
    // to get indices correct, the index of dm14 is (dm14 + 1.4)/0.05

    std::string line;
    while (std::getline(datafile, line))
    {

      if (*line.begin() == '#') continue;
      if (*line.begin() == 'P') continue;
      // std::cout << "---------------\n";
      // std::istrinstream iss(line);
      std::vector < std::string > splitStream;
      reduce(line);
      split(splitStream, line);
      // if (splitStream.size() > 10) continue;
      // std::cout << "Length of line (split tokens): " << splitStream.size() << "\n";
      std::vector <double> data;
      for (int index = 0; index < 5; index++)
      {
        // std::cout << "data["<< index <<"] - " << legend[index] << ": " << splitStream[index] << std::endl;
        std::stringstream ss(splitStream[index]);
        double temp;
        ss >> temp;
        // std::cout << "pushing back " << temp << std::endl;
        data.push_back(temp);
      }
      // Now do the actual processing.
      

      unsigned int dm14index = ceil(((data[0]+ 1.4)/0.05));
      unsigned int sin22thindex = ceil(((data[1] + 4.01)/0.1));

      double chi2 = fmin(data[3], data[4]);
      if (chi2map[dm14index][sin22thindex] == 0.0) 
        chi2map[dm14index][sin22thindex] = chi2;
      else if (chi2map[dm14index][sin22thindex] > chi2) 
        chi2map[dm14index][sin22thindex] = chi2;

      i ++;

    }


    return chi2map;
  }


  void PlotUtils::reduce(std::string & s, char delim)
  {
    if (s.size() == 0)
      return;
    // Useful code to remove all consecutive duplicates:
    // for (auto ch = s.begin() + 1; ch != s.end(); ++ ch)
    // {
    //   if (*ch == *(ch -1)) s.erase(ch);
    // }
    for (auto ch = s.begin() + 1; ch != s.end(); ++ ch)
    {
      if (*ch == delim && *(ch -1) == delim){
        s.erase(ch);
        ch --;
      }
    }
    // std::cout << s << "\n";
  }

  void PlotUtils::split(std::vector<std::string> &fields, std::string s, char delim){
    if (fields.size() != 0) {
      fields.clear();
    }
    
    size_t found, previous = 0;
    
    // if the leading token is a delimeter, remove it:
    if (*s.begin() == delim) s.erase(s.begin());

    //find the first delimeter:
    found = s.find_first_of(delim);
    
    while (found!=std::string::npos)
    {
      
      //add the first instance into the vector:
      if (previous == 0) {
        fields.push_back(s.substr(previous, found - previous));
      }
      else fields.push_back(s.substr(previous + 1 , found  - previous - 1));
      //save where the previous delim was:
      previous = found;
      //search for another comma, starting just after where the previous left off.
      found=s.find_first_of(delim,found+1);
    }
    
    //get the last section of the line:
    fields.push_back(s.substr(previous + 1));
    
    return;
  }

  double PlotUtils::getMinimum2D(std::vector<std::vector<double> > & s)
  {
    double min = 1e20;
    for (auto & dim1 : s)
    {
      for (auto & dim2 : dim1){
        if (dim2 != 0 && dim2 < min) min = dim2;
      }
    }
    return min;
  }
  std::string PlotUtils::GetEnv( const std::string & var ){
    const char * val = std::getenv( var.c_str() );
    if ( val == 0 ) {
        return "";
    }
    else {
        return val;
    }
  }

std::vector<float> PlotUtils::Bin_LSND_Data( int npoints,
                    std::vector<float> dm2points,
                    std::vector<float> sin22thpoints){

  // Get at the underlying array from the vectors, to set up the bins:
  float * xbins = & sin22thpoints[0];
  float * ybins = & dm2points[0];

  // for (auto & x : sin22thpoints) std::cout << "x: " << x << std::endl;
  std::string path = GetEnv("MAKE_TOP_DIR");
  path.append("/UserDev/lar1Sens/");
  std::string data_dir= path;
  data_dir.append("lsnd_data/");

  TH2D * LSND_Data = new TH2D("data","data", npoints,xbins, npoints, ybins);

  const Int_t NDATAFILES = 11;
  const char * file_list[NDATAFILES] = {"llreg_608_1.vec",
               "llreg_608_2.vec",
               "llreg_608_3.vec",
               "llreg_607_1.vec",
               "llreg_607_2.vec",
               "llreg_607_3.vec",
               "llreg_607_4.vec",
               "llreg_607_5.vec",
               "llreg_607_6.vec",
               "llreg_607_7.vec",
               "llreg_607_8.vec"};

  Int_t    nlines;
  Double_t dummy, dummy_old;
  Double_t x(0), y(0);
  for (Int_t ifile = 0; ifile<NDATAFILES; ifile++) {
    nlines = 0;
    char  filename[100];
    strcpy(filename, data_dir.c_str());
    //printf("%s\n",filename);
    strcat(filename, file_list[ifile]);
    //printf("%s\n",filename);
    ifstream datafile;
    datafile.open(filename, std::ios_base::in);
    //check if the file is open: 
    if (!datafile.is_open() ) {std::cerr << "lsnd_plot.C: file not opened" <<std::endl;}
    //else {std::cout << "Successfully opened " << filename << std::endl;}
    while (!datafile.eof()) {
      datafile >> dummy; 
      datafile >> dummy; 
      datafile >> x; 
      datafile >> y;
      LSND_Data -> Fill(x,y);
      nlines++;
      if (dummy == dummy_old) nlines--; //if last row was empty
      dummy_old = dummy;
    }
    datafile.close();
  }

  // TStyle * style = new TStyle(*gStyle);

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


  // TCanvas * d = new TCanvas("LSND Region", "LSND Region", 700, 700);
  // d->SetLogx();
  // d->SetLogy();  

  // TH2D* hr1=new TH2D("hr1","hr1",500,0.0001,1.0,500,0.01,100.0);
  // hr1->Reset();
  // hr1->SetFillColor(0);
  // hr1->SetTitle(";sin^{2}2#theta_{#mue};#Deltam^{2} (eV^{2})");
  // hr1->GetXaxis()->SetTitleOffset(1.1);
  // hr1->GetYaxis()->SetTitleOffset(1.2);
  // hr1->GetXaxis()->SetTitleSize(0.05);
  // hr1->GetYaxis()->SetTitleSize(0.05);
  // hr1->SetStats(kFALSE);
  // hr1->Draw();
  // lsnd_plot(d);
  // // gROOT->ProcessLine(".x ./lsnd_plot.c+(d)");
  // // LSND_Data ->Draw("LF");

  // TLegend* leg3=new TLegend(0.2,0.2,0.4,0.35);
  // leg3->SetFillStyle(0);
  // leg3->SetFillColor(0);
  // leg3->SetBorderSize(0);
  // leg3->SetTextSize(0.03);
  // TGraph *gdummy1 = new TGraph();
  // gdummy1->SetFillColor(29);
  // TGraph *gdummy2 = new TGraph();
  // gdummy2->SetFillColor(38);
  // TMarker *gdummy3 = new TMarker();
  // gdummy3 -> SetMarkerStyle(3);
  // gdummy3 -> SetMarkerColor(1);
  // TGraph *gdummy0 = new TGraph();
  // gdummy0 -> SetFillColor(4);
  // gdummy0 -> SetFillStyle(3004);
  // leg3->AddEntry(gdummy2,"LSND 90% CL","F");
  // leg3->AddEntry(gdummy1,"LSND 99% CL","F");
  // leg3->AddEntry(gdummy3,"LSND Best Fit","P*");
  // leg3->AddEntry(gdummy0,"Global Fit 90% CL (J. Kopp et al. arXiv:1303.3011)");   
  // leg3->Draw("same");

  // gStyle -> cd();

  std::vector<float> sin22thresult(npoints+1, 0.0);
  std::cout << "Result size: " << sin22thresult.size() << std::endl;
  // Now loop over the hist in y points and find the first point where the bins are filled:
  for (int i_dm2 = 1; i_dm2 <= npoints+1; i_dm2++)
  {
    for (int i_sin22th = npoints+1; i_sin22th > 0; i_sin22th--)
    {
      if (LSND_Data -> GetBinContent(i_sin22th,i_dm2) > 0) sin22thresult.at(i_dm2-1) = i_sin22th;
    }
    if (sin22thresult.at(i_dm2 -1) == 0) {
      if (i_dm2 != 1 ) sin22thresult.at(i_dm2 - 1) = sin22thresult.at(i_dm2-2);
      else sin22thresult.at(i_dm2 -1) = npoints;
    }
  }

  // Refine the line to extrapolate between the points that have no fit
  // refine(sin22thresult);



  // Need to validate that this works:
  // float xpoints[npoints+1];
  // float ypoints[npoints+1];
  
  // Pass it to a function to interpolate between constant sections.
  // refine(sin22thresult);

  // for (int i = 0; i <= npoints; i++)
  // {
  //   xpoints[i] = sin22thpoints[sin22thresult[i]];
  //   ypoints[i] = dm2points[i];
  // }



  // for (int i = 0; i <= npoints; i++)
  //   std::cout << "Line along dm2 = " << ypoints[i] << ",\tsin22th = " << xpoints[i] << std::endl;



  // TGraph * line = new TGraph(npoints+1, xpoints, ypoints);
  // line -> SetLineColor(4);
  // line -> SetLineWidth(2);
  // line -> Draw("same C");

  // Returns the index of what points to use in sin22th space
  return sin22thresult;

}

void PlotUtils::plot_Matrix( TString matrixFileName,
                             TString matrixName)
{

  // Open the file, get the matrix.
  // Pass it to the real function]
  TFile * f = new TFile(matrixFileName);
  TH2D * matrix = (TH2D*) f -> Get(matrixName);

  bool use100m = matrixFileName.Contains("ND");
  bool use470m = matrixFileName.Contains("uB");
  bool use600m = matrixFileName.Contains("T600");


  // Get the nue rates to get the num bins:
  std::vector<TH1F *> nueEventRates;
  if (use100m) nueEventRates.push_back( (TH1F*) f-> Get("nueEventRates_100m") -> Clone());
  if (use470m) nueEventRates.push_back( (TH1F*) f-> Get("nueEventRates_470m")-> Clone());
  if (use600m) nueEventRates.push_back( (TH1F*) f-> Get("nueEventRates_600m_onaxis")-> Clone());
  if (!use100m && !use470m && !use600m) {
    std::cerr << "You seem to be providing a file that " 
              << "doesn't conform to standard format.\n";
    exit (-1);
  }

  int nBins_nue = nueEventRates.front() -> GetNbinsX();
  int nBins_numu = 0;

  // Find out if there are numu bins:
  // Get the nue rates to get the num bins:
  std::vector<TH1F *> numuEventRates;
  if (use100m) {
    TH1F* tempHist = (TH1F*) f-> Get("numuEventRates_100m");
    if (tempHist != 0)
      numuEventRates.push_back( (TH1F*) tempHist-> Clone());
  }
  if (use470m) {
    TH1F* tempHist = (TH1F*) f-> Get("numuEventRates_470m");
    if (tempHist != 0)
      numuEventRates.push_back( (TH1F*) tempHist-> Clone());
  }
  if (use600m) {
    TH1F* tempHist = (TH1F*) f-> Get("numuEventRates_600m_onaxis");
    if (tempHist != 0)
      numuEventRates.push_back( (TH1F*) tempHist-> Clone());
  }

  if (numuEventRates.size() != 0){
    nBins_numu = numuEventRates.front() -> GetNbinsX();
  }




  TCanvas * canv = plot_Matrix( matrix, matrixName, 
                                use100m, use470m, use600m, 
                                nBins_nue, nBins_numu);
  // for the name of the pdf file to print:
  matrixFileName.Remove(matrixFileName.Length()-5); // chop off ".root"
  TString outName = matrixFileName;
  outName += "_" + matrixName + ".pdf";
  canv -> Print(outName,"pdf");

  if (matrixName == "fracMatHist"){
    // std::vector<TCanvas*> canvases = plotRatesWithErrors(matrix,
    //                                                      nueEventRates,
    //                                                      numuEventRates);
    std::vector<TCanvas *> Uncerts = plotFractionalUncerts(matrix, 
                                                 nueEventRates,
                                                 numuEventRates,
                                                 matrixFileName);

    Uncerts.front() -> Print(matrixFileName+"_nue_uncerts.pdf","pdf");
    if (numuEventRates.size() != 0)
      Uncerts.back() -> Print(matrixFileName+"_numu_uncerts.pdf","pdf"); 

  }


  return;

}

std::vector<TCanvas *> PlotUtils::plotFractionalUncerts(TH2D * matrix,
                                          std::vector<TH1F*> nueRates,
                                          std::vector<TH1F*> numuRates,
                                          TString matrixFileName){

  // Set up the canvas and a background histogram to control the appearance

  std::vector<TCanvas *> uncerts;
  uncerts.push_back(new TCanvas("nueUncerts","",800,600));

  if (numuRates.size() != 0)
    uncerts.push_back(new TCanvas("numuUncerts","",800,600));

  
  std::vector<TH1F *> background;
  background.reserve(2);
  background.front() = (TH1F*) nueRates.front() -> Clone();

  for (int i = 1; i <= background.front()->GetNbinsX();i++) 
    background.front() -> SetBinContent(i,0);
  

  background.front() -> SetMinimum(0.0);
  background.front() -> SetMaximum(25);
  background.front() -> GetXaxis() -> SetTitle("Reconstructed Neutrino Energy [GeV]");
  background.front() -> GetYaxis() -> SetTitle("Uncert [\%]");
  background.front() -> GetXaxis() -> SetLabelSize(0.05);
  background.front() -> GetYaxis() -> SetLabelSize(0.05);
  background.front() -> GetXaxis() -> CenterTitle();
  background.front() -> GetXaxis() -> SetTitleSize(.05);
  background.front() -> GetYaxis() -> SetTitleSize(.045);
  background.front() -> GetYaxis() -> SetTitleOffset(1.6);
  
  background.back() = (TH1F*) background.front() -> Clone();

  if (matrixFileName.Contains("xsec")){
    background.front() -> SetTitle("#nu_{e} Cross Section Fractional Uncertaintes");
    background.back()  -> SetTitle("#nu_{#mu} Cross Section Fractional Uncertaintes");
  }
  else if (matrixFileName.Contains("flux")){
    background.front() -> SetTitle("#nu_{e} Flux Fractional Uncertaintes");
    background.back()  -> SetTitle("#nu_{#mu} Flux Fractional Uncertaintes");
  }
  else{
    background.front() -> SetTitle("#nu_{e} Fractional Uncertaintes");
    background.back()  -> SetTitle("#nu_{#mu} Fractional Uncertaintes");
  }  

  uncerts.front() -> cd();
  background.front() -> Draw();
  if (numuRates.size() != 0){
    uncerts.back() ->cd();
    background.back() -> Draw();
  }


  TLegend * leg = new TLegend(0.6,0.65,0.95,0.8);
  leg -> SetFillStyle(0);
  leg -> SetFillColor(0);
  leg -> SetBorderSize(0);
  leg -> SetTextSize(0.05);
  int curr_bin = 1;
  for (unsigned int iDet = 0; iDet < nueRates.size(); iDet ++)
  {

    // do nue first
    uncerts.front()->cd();

    TH1F * tempHist = (TH1F*) nueRates.at(iDet) -> Clone();
    // Set up the plot to look nice...
    for (int iBin = 1; iBin <= nueRates.at(iDet)->GetNbinsX(); iBin ++){
      tempHist -> SetBinContent(iBin, 100*sqrt(matrix->GetBinContent(curr_bin,curr_bin)));
      tempHist -> SetBinError(iBin, 0);
      curr_bin ++;
    }

    TString thisName = nueRates.at(iDet) -> GetName();
    if (thisName.Contains("100m")){
      // temphist -> SetLineWidth("#nu_{e} Rates at LAr1-ND");
      tempHist -> SetLineWidth(1);
      tempHist -> SetLineStyle(2);
      // tempHist -> SetMarkerStyle(8);
      // tempHist -> SetMarkerColor(1);
      tempHist -> Draw("same");
      leg -> AddEntry(tempHist,"LAr1-ND");
    }
    if (thisName.Contains("470m")){
      tempHist -> SetLineWidth(0);
      tempHist -> SetMarkerStyle(3);
      tempHist -> SetMarkerColor(2);
      // rates.at(iDet) -> SetTitle("#nu_{e} Rates at MicroBooNE");
      tempHist -> Draw("P same");
      leg -> AddEntry(tempHist,"uBooNE");
    }
    if (thisName.Contains("600m")) {
      tempHist -> SetLineWidth(0);
      tempHist -> SetMarkerStyle(5);
      tempHist -> SetMarkerColor(4);
      // rates.at(iDet) -> SetTitle("#nu_{e} Rates at T600");
      tempHist -> Draw("P same");
      leg -> AddEntry(tempHist,"T600");
    }

    leg -> Draw();

    // do numu if it exists:
    if (numuRates.size() != 0){
      uncerts.back()->cd();

      TH1F * tempHist = (TH1F*) numuRates.at(iDet) -> Clone();
      
      for (int iBin = 1; iBin <= numuRates.at(iDet)->GetNbinsX(); iBin ++){
        tempHist -> SetBinContent(iBin, 100*sqrt(matrix->GetBinContent(curr_bin,curr_bin)));
        tempHist -> SetBinError(iBin, 0);
        curr_bin ++;
      }
      
      // Set up the plot to look nice...
      TString thisName = numuRates.at(iDet) -> GetName();
      if (thisName.Contains("100m")){
        // temphist -> SetLineWidth("#nu_{e} Rates at LAr1-ND");
        tempHist -> SetLineWidth(1);
        tempHist -> SetLineStyle(2);
        // tempHist -> SetMarkerStyle(8);
        // tempHist -> SetMarkerColor(1);
        tempHist -> Draw("same");
        // leg -> AddEntry(tempHist,"LAr1-ND");
      }
      if (thisName.Contains("470m")){
        tempHist -> SetLineWidth(0);
        tempHist -> SetMarkerStyle(3);
        tempHist -> SetMarkerColor(2);
        // rates.at(iDet) -> SetTitle("#nu_{e} Rates at MicroBooNE");
        tempHist -> Draw("P same");
        // leg -> AddEntry(tempHist,"uBooNE");
      }
      if (thisName.Contains("600m")) {
        tempHist -> SetLineWidth(0);
        tempHist -> SetMarkerStyle(5);
        tempHist -> SetMarkerColor(4);
        // rates.at(iDet) -> SetTitle("#nu_{e} Rates at T600");
        tempHist -> Draw("P same");
        // leg -> AddEntry(tempHist,"T600");
      }

      leg -> Draw();
    }

  }


  return uncerts;

}
                                          

std::vector<TCanvas *> PlotUtils::plotRatesWithErrors(TH2D * matrix,
                                                      std::vector<TH1F*> nueRates,
                                                      std::vector<TH1F*> numuRates)
{
  std::vector <TCanvas *> canvases;
  int curr_bin = 1;

  for (unsigned int iDet = 0; iDet < nueRates.size(); iDet ++)
  {
    canvases.push_back(new TCanvas(Form("canv_nue_%u",iDet),"", 500,400));
    canvases.back() -> cd();

    // TH1F * tempHist = (TH1F*) nueRates.at(iDet) -> Clone();

    // Set the bin errors to be systematic uncertainties
    for (int iBin = 1; iBin <= nueRates.at(iDet)->GetNbinsX(); iBin ++){
      nueRates.at(iDet)->SetBinContent(iBin, nueRates.at(iDet)->GetBinContent(iBin) 
                                           / nueRates.at(iDet)->GetBinWidth(iBin) );
      nueRates.at(iDet)->SetBinError(iBin, sqrt(matrix->GetBinContent(curr_bin,curr_bin))
                                         * nueRates.at(iDet)->GetBinContent(iBin));
      // tempHist -> SetBinContent(iBin, matrix->GetBinContent(curr_bin,curr_bin));
      curr_bin ++;
    }

    // Set up the plot to look nice...
    TString thisName = nueRates.at(iDet) -> GetName();
    if (thisName.Contains("100m")) nueRates.at(iDet) -> SetTitle("#nu_{e} Rates at LAr1-ND");
    if (thisName.Contains("470m")) nueRates.at(iDet) -> SetTitle("#nu_{e} Rates at MicroBooNE");
    if (thisName.Contains("600m")) nueRates.at(iDet) -> SetTitle("#nu_{e} Rates at T600");

    nueRates.at(iDet) -> Draw("e hist");
    // nueUncerts -> cd();


    if (numuRates.size() != 0){
      canvases.push_back(new TCanvas("canv_nue_100m","", 400,300));
      canvases.back() -> cd();
      // Set the bin errors to be systematic uncertainties
      for (int iBin = 1; iBin <= numuRates.at(iDet)->GetNbinsX(); iBin ++){
        numuRates.at(iDet)->SetBinContent(iBin, numuRates.at(iDet)->GetBinContent(iBin) 
                                              / numuRates.at(iDet)->GetBinWidth(iBin) );
        numuRates.at(iDet)->SetBinError(iBin, sqrt(matrix->GetBinContent(curr_bin,curr_bin))
                                              * numuRates.at(iDet)->GetBinContent(iBin));
        curr_bin ++;
      }
        
      thisName = numuRates.at(iDet) -> GetName();
      if (thisName.Contains("100m")) numuRates.at(iDet) -> SetTitle("#nu_{#mu} Rates at LAr1-ND");
      if (thisName.Contains("470m")) numuRates.at(iDet) -> SetTitle("#nu_{#mu} Rates at MicroBooNE");
      if (thisName.Contains("600m")) numuRates.at(iDet) -> SetTitle("#nu_{#mu} Rates at T600");


      numuRates.at(iDet) -> Draw("e hist");
    }
  }


  return canvases;
}

TCanvas *  PlotUtils::plot_Matrix(TH2D * matrix, TString matrixName,
                                  bool use100m, bool use470m, bool use600m, 
                                  int nBins_nue,
                                  int nBins_numu)
{


  set_plot_style();

  static int nCanvas = 0;
  nCanvas ++;

  TString title;
  if (matrixName == "corrMatHist") title = "Correlation Matrix";
  else if (matrixName == "covMatHist") title = "Covariance Matrix";
  else if (matrixName == "fracMatHist") title = "Fractional Covariance Matrix";
  else title = matrixName;

  char name[100];
  sprintf(name,"canvas_%d", nCanvas);

  TCanvas * canv = new TCanvas(name, title, 750,750);
  canv -> cd();
  TPad *pad1 = new TPad("pad1","",0,0,1,1);
  
  pad1 -> Draw();
  pad1 -> cd();

  int total_bins = 0;
  if (use100m) total_bins += nBins_nue + nBins_numu;
  if (use470m) total_bins += nBins_nue + nBins_numu;
  if (use600m) total_bins += nBins_nue + nBins_numu;

  TH2D * blankHist = new TH2D("blank","",2*total_bins,0,2*total_bins-1,2*total_bins,0,2*total_bins-1);
  blankHist -> GetYaxis() -> SetBinLabel(1,"");
  // use a counter to keep track of which bin has been labeled:
  int curr_bin = 1;

  if (use100m) {
    matrix -> GetXaxis() -> SetBinLabel(curr_bin + nBins_nue/2, "ND #nu_{e}");
    matrix -> GetYaxis() -> SetBinLabel(curr_bin + nBins_nue/2, "ND #nu_{e}");
    blankHist -> GetXaxis() -> SetBinLabel(2*curr_bin, "200 MeV");
    blankHist -> GetXaxis() -> SetBinLabel(2*(curr_bin + nBins_nue)-3, "3 GeV");
    curr_bin += nBins_nue;
    if (nBins_numu != 0) {
      matrix -> GetXaxis() -> SetBinLabel(curr_bin + nBins_numu/2, "ND #nu_{#mu}");
      matrix -> GetYaxis() -> SetBinLabel(curr_bin + nBins_numu/2, "ND #nu_{#mu}");
      blankHist -> GetXaxis() -> SetBinLabel(2*curr_bin, "200 MeV");
      blankHist -> GetXaxis() -> SetBinLabel(2*(curr_bin + nBins_numu)-3, "3 GeV");
      curr_bin += nBins_numu;
    }
  }
  if (use470m) {
    matrix -> GetXaxis() -> SetBinLabel(curr_bin + nBins_nue/2, "uB #nu_{e}");
    matrix -> GetYaxis() -> SetBinLabel(curr_bin + nBins_nue/2, "uB #nu_{e}");
    blankHist -> GetXaxis() -> SetBinLabel(2*curr_bin, "200 MeV");
    blankHist -> GetXaxis() -> SetBinLabel(2*(curr_bin + nBins_nue)-3, "3 GeV");
    curr_bin += nBins_nue;    
    if (nBins_numu != 0) {
      matrix -> GetXaxis() -> SetBinLabel(curr_bin + nBins_numu/2, "uB #nu_{#mu}");
      matrix -> GetYaxis() -> SetBinLabel(curr_bin + nBins_numu/2, "uB #nu_{#mu}");
      blankHist -> GetXaxis() -> SetBinLabel(2*curr_bin, "200 MeV");
      blankHist -> GetXaxis() -> SetBinLabel(2*(curr_bin + nBins_numu)-3, "3 GeV");
      curr_bin += nBins_numu;
    }
  }
  if (use600m) {
    matrix -> GetXaxis() -> SetBinLabel(curr_bin + nBins_nue/2, "T600 #nu_{e}");
    matrix -> GetYaxis() -> SetBinLabel(curr_bin + nBins_nue/2, "T600 #nu_{e}");
    blankHist -> GetXaxis() -> SetBinLabel(2*curr_bin, "200 MeV");
    blankHist -> GetXaxis() -> SetBinLabel(2*(curr_bin + nBins_nue)-3, "3 GeV");
    curr_bin += nBins_nue;    
    if (nBins_numu != 0) {
      matrix -> GetXaxis() -> SetBinLabel(curr_bin + nBins_numu/2, "T600 #nu_{#mu}");
      matrix -> GetYaxis() -> SetBinLabel(curr_bin + nBins_numu/2, "T600 #nu_{#mu}");
      blankHist -> GetXaxis() -> SetBinLabel(2*curr_bin, "200 MeV");
      blankHist -> GetXaxis() -> SetBinLabel(2*(curr_bin + nBins_numu)-3, "3 GeV");
      curr_bin += nBins_numu;
    }
  }

  // Toggle the labels on the xaxis to draw horizontally:
  matrix -> GetXaxis() -> LabelsOption("v");
  matrix -> GetYaxis() -> SetLabelSize(0.055);
  matrix -> GetXaxis() -> SetLabelSize(0.055);

  // Remove the tick marks:
  matrix -> GetXaxis() -> SetTickSize(0);
  matrix -> GetYaxis() -> SetTickSize(0);
  blankHist -> GetXaxis() -> SetTickSize(0);
  blankHist -> GetYaxis() -> SetTickSize(0);



  // pad2 ->cd();
  // matrix -> Draw("colz");
  // blankHist -> Draw("same");
  matrix -> Draw("colz");
  TPad *overlay = new TPad("overlay","",0,0,1,1);
  overlay->SetFillStyle(0);
  overlay->SetFillColor(0);
  overlay->SetFrameFillStyle(0);
  overlay->Draw("FA");
  overlay->cd();

  blankHist -> LabelsOption("v");
  blankHist -> Draw("same x");


  // Draw some lines to separate the detectors:
  std::vector <TLine *> Lines;
  curr_bin = nBins_nue;
  if (use100m) {
    Lines.push_back(new TLine(2*curr_bin,0,2*curr_bin,2*total_bins-1));
    Lines.push_back(new TLine(0,2*curr_bin,2*total_bins-1,2*curr_bin));
    if (nBins_numu != 0){
      curr_bin += nBins_numu;
      Lines.push_back(new TLine(2*curr_bin,0,2*curr_bin,2*total_bins-1));
      Lines.push_back(new TLine(0,2*curr_bin,2*total_bins-1,2*curr_bin));
    }
    curr_bin += nBins_nue;
  }
  if (use470m){
    Lines.push_back(new TLine(2*curr_bin,0,2*curr_bin,2*total_bins-1));
    Lines.push_back(new TLine(0,2*curr_bin,2*total_bins-1,2*curr_bin));
    if (nBins_numu != 0){
      curr_bin += nBins_numu;
      Lines.push_back(new TLine(2*curr_bin,0,2*curr_bin,2*total_bins-1));
      Lines.push_back(new TLine(0,2*curr_bin,2*total_bins-1,2*curr_bin));
    }
    curr_bin += nBins_nue;
  }
  if (use600m){
    Lines.push_back(new TLine(2*curr_bin,0,2*curr_bin,2*total_bins-1));
    Lines.push_back(new TLine(0,2*curr_bin,2*total_bins-1,2*curr_bin));
    if (nBins_numu != 0){
      curr_bin += nBins_numu;
      Lines.push_back(new TLine(2*curr_bin,0,2*curr_bin,2*total_bins-1));
      Lines.push_back(new TLine(0,2*curr_bin,2*total_bins-1,2*curr_bin));
    }
    curr_bin += nBins_nue;
  }

  for (auto & line : Lines){
    if (line == *(Lines.end() - 2) || line == *(Lines.end() -1)) break;
    line->SetLineWidth(3);
    line->SetLineStyle(2);
    line->Draw("same");
  }

  // return the canvas so that it can be saved
  return canv;

}

void PlotUtils::set_plot_style()
{
  const Int_t NRGBs = 5;
  const Int_t NCont = 255;

  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);
  gStyle->SetOptStat(0);
  gStyle->SetPadRightMargin(0.15);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadTopMargin(0.15);
  gStyle->SetPadBottomMargin(0.15);
}


}