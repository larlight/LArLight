/*
* This class is a helper class for the chi2 calculation done in the 
* other files.
* 
*

 */

#ifndef SENSUTILS_HH
#define SENSUTILS_HH

#include "TH1F.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "TLatex.h"

namespace lar1{
  class SensUtils
  {
  public:
    SensUtils();
    ~SensUtils();
  
    TH1F* AppendHistograms(TH1F * hist1, TH1F* hist2, TH1F* hist3);
    TH1F* AppendHistograms(TH1F * hist1, TH1F* hist2);

    Float_t * CollapseMatrix(std::vector<std::vector<float> > entries,
                             int nbinsE, int nL);

    bool fileExists(const char *filename);

    std::vector<float> appendVectors(std::vector<float> , std::vector<float> );
    std::vector<float> appendVectors(std::vector<float> , std::vector<float> ,
                                     std::vector<float>);
    int buildCovarianceMatrix(std::vector<std::vector<float> > &,
                              std::vector<std::string> & , int );
    TH1F* makeHistogram(std::vector<float> & input, 
                        double lowbin, double highbin);
    std::vector<float> collapseVector(std::vector<float> input,
                                      int nbinsE, int nL);

    void add_plot_label(char* label, double x, double y,
                        double size = 0.05, int color = 1,
                        int font = 62, int align = 22 );

  private:


  };

}

#endif