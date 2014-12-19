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
#include "TStyle.h" //for gStyle
#include "TLegend.h"
#include "TMarker.h"
#include "TPad.h"
#include "TColor.h"
#include "TFile.h"
#include "TGaxis.h"

namespace lar1{

  class PlotUtils
  {
  public:
    PlotUtils(){};
    ~PlotUtils(){};
    
    std::string GetEnv( const std::string & var );

    TLegend * getLSNDLegend();

    TH2D * getEmptySensPlot(double sin22thmin=0.0001,
                            double sin22thmax=1.0,
                            double dm2min=0.01,
                            double dm2max=100.0);


    // Add the lsnd_data to a plot
    void lsnd_plot (TPad* c);
    // void lsnd_plot (TCanvas* c);

    void miniBoone_plot(TPad* c);

    // Add the region from Giuni et al
    void giunti_plot(TPad* c);

    void add_plot_label(char* label, double x, double y,
                        double size = 0.05, int color = 1,
                        int font = 72, int align = 22 );

    void set_plot_style();
    
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
    int plotGFData(TPad * c);
    // int plotGFData(TCanvas * c);

    //  reads in the LSND data and then bins it onto the grid
    //  of dm2 points and sin22th points.  It then finds the
    //  minimum point in sin22th for each dm2 point, and returns 
    //  that point as a vector (function of dm2)
    std::vector<int> Bin_LSND_Data(int npoints,
                                     std::vector<float> dm2points,
                                     std::vector<float> sin22thpoints);

    // This function reads in the appropriate TH2D and then calls the plotting 
    // function
    void plot_Matrix( TString matrixFileName,
                      TString matrixName);

    // This function makes a plot of a matrix but requires the TH2D histogram
    // to do it.
    TCanvas * plot_Matrix( TH2D * matrix, TString matrixName,
                      bool use100m, bool use470m, bool use600m, 
                      int nBins_nue,
                      int nBins_numu = 0);

    std::vector<TCanvas *> plotRatesWithErrors(TH2D * matrix,
                                               std::vector<TH1F*> nueRates,
                                               std::vector<TH1F*> numuRates);

    std::vector<TCanvas *> plotFractionalUncerts(TH2D * matrix,
                                                 std::vector<TH1F*> nueRates,
                                                 std::vector<TH1F*> numuRates,
                                                 TString matrixFileName);

  protected:
    

    /* data */

  };
}

#endif
