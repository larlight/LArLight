// A basic class to help with plotting for the nueappearance stuff

#ifndef PLOTUTILS_HH
#define PLOTUTILS_HH 

#include "TCanvas.h"
#include "TLatex.h"
#include "TGraph.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include "TH2D.h"

namespace lar1{

  class PlotUtils
  {
  public:
    PlotUtils(){};
    ~PlotUtils(){};
    
    std::string GetEnv( const std::string & var );

    // Add the lsnd_data to a plot
    void lsnd_plot (TCanvas* c);

    void add_plot_label(char* label, double x, double y,
                        double size = 0.05, int color = 1,
                        int font = 62, int align = 22 );
    
    //fields will contain the elements of the string s, split by the delimiter "delim"
    void split(std::vector<std::string> &fields, std::string s, char delim = ' ') ;
    
    // removes all consecutive instances of the char delim
    // so, if called with delim = 'a', s = "abaauragaal", returns with s = "abauragal"
    void reduce(std::string & s, char delim = ' ');
    
    // returns the minimum of a 2D data set
    double getMinimum2D(std::vector<std::vector<double> > &s);
    
    // reads in the Global Fit data from the file is lsnd_data/
    std::vector<std::vector<double > >  readGFData();

    // Plots the GF data on the Canvas given, like the lsnd_plot function above
    int plotGFData(TCanvas * c);


  protected:
    

    /* data */

  };
}

#endif
