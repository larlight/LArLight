


#include "PlotUtils.hh"

namespace lar1{

  // PlotUtils::PlotUtils(){}

  void PlotUtils::lsnd_plot(TCanvas * c){
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
      std::cout << std::endl;
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

  int PlotUtils::plotGFData(TCanvas * c ){
    std::vector<std::vector<double> > data = readGFData();

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

    // std::cout << "\t";
    // for (int j = 0; j <= 41; ++j) std::cout << std::fixed << (j*0.1 - 4.01) << " \t";
    // std::cout << "\n";
    for (int i = 0; i <= 56; i++){
      // std::cout << std::fixed << (i*0.05 - 1.4);
      for (int j = 0; j <= 40; ++j){
        if (data[i][j] >= min && data[i][j] < bound90)
        {
          double sin22th = pow(10.0,(j*0.1 - 4.01));
          double dm2 = pow(10.0,(i*0.05 - 1.4));
          chi2hist -> Fill(sin22th,dm2,data[i][j]);
          // std::cout << "Filling " << data[i][j] << " at x = " << sin22th 
          // << ", y = " << dm2 << std::endl;
        }
      }
      // std::cout << "\n";
    }

    // std::cout << "Max of plot: " << chi2hist -> GetMaximum() << std::endl;

    // TCanvas * d = new TCanvas("dumb","dumber", 500, 500);

    bottom -> SetFillColor(1);
    bottom -> SetFillStyle(3254);
    // bottom -> SetLineColor(46);
    // bottom -> GetFillColor() -> SetAlpha(0.5);

    // auto arr = chi2hist -> GetArray();
    // chi2hist -> Smooth();
    // chi2hist -> Smooth();
    // chi2hist -> Draw("cont");
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
    // There are 57 dm14 points, from -1.4 to 1.4 in steps of 0.05;
    // There are 41 sin22th points from -4.01 to -0.01 in steps of 0.1;
    // at each point, we average over any Um4 or chi2_IH/chi2_NH
    std::vector< std::vector<double > > chi2map(57, std::vector<double>(41, 0.0) );
    // to get indices correct, the index of dm14 is (dm14 + 1.4)/0.05

    std::string line;
    while (std::getline(datafile, line))
    {
      if (*line.begin() == '#') continue;
      // std::cout << "---------------\n";
      // std::istrinstream iss(line);
      std::vector < std::string > splitStream;
      reduce(line);
      split(splitStream, line);
      if (splitStream.size() > 10) continue;
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
      

      double dm14index = ((data[0]+ 1.4)/0.05);
      double sin22thindex = ((data[1] + 4.01)/0.1);

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

}