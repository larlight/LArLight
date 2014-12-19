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
#include <algorithm>
#include "TMatrix.h"
#include "TFile.h"

namespace lar1{
  class SensUtils
  {
  public:
    SensUtils();
    ~SensUtils();
  
    TH1F* AppendHistograms(TH1F * hist1, TH1F* hist2, TH1F* hist3);
    TH1F* AppendHistograms(TH1F * hist1, TH1F* hist2);

    std::vector<std::vector<float> > CollapseMatrix(
                              std::vector<std::vector<float> > entries,
                              int nbins_nue, int nbins_numu, int nL);
    TMatrix CollapseMatrix(TMatrix entries,
                              int nbins_nue, int nbins_numu, int nL);

    bool fileExists(const char *filename);

    std::vector<float> rebinVector(std::vector<float> inputVector,
                                   std::vector<float> bins);
    // std::vector<float> rebinVector(std::vector<float> & inputVector,
                                   // std::string binSchemeName);

    std::vector<float> appendVectors(std::vector<float> , std::vector<float> );
    std::vector<float> appendVectors(std::vector<float> , std::vector<float> ,
                                     std::vector<float>);
    
    TString GetChi2FileName  (TString fileSource,
                              TString fileNameRoot,
                              TString detNamesString,
                              TString fileNameExtenstion,
                              bool includeNumus,
                              bool includeDetSyst,
                              std::vector<std::string> covMatrixList,
                              std::vector<int> covMatrixListSource,
                              bool absolute_MWSource);

    TString GetMatrixFileName(TString fileSource,
                              TString detNamesString,
                              bool includeNumus,
                              bool useXSecWeights,
                              bool useFluxWeights,
                              int  multiWeightSource,
                              bool absolute_MWSource);
    
    TString GetMatrixFileName(TString fileSource,
                              TString detNamesString,
                              TString fileNameRoot,
                              bool includeNumus,
                              bool useXSecWeights,
                              bool useFluxWeights,
                              int  multiWeightSource,
                              bool absolute_MWSource);  
    
    TString GetMatrixFileName( TString fileSource,
                               TString detNamesString,
                               TString fileNameRoot,
                               bool includeNumus,
                               std::string uncert,
                               int  multiWeightSource,
                               bool absolute_MWSource);
    
    TMatrix * assembleCovarianceMatrix( TString fileSource,
                                        TString detNamesString,
                                        TString fileNameRoot,
                                        bool includeNumus,
                                        const std::vector<float> & nullVector,
                                        std::vector<std::string> covMatrixList,
                                        std::vector<int> covMatrixListSource,
                                        bool absolute_MWSource);

    TMatrix getDirtMatrix(const std::vector<float> & nullVector,
                            const std::vector<std::vector<float> > & Dirt);
    TMatrix getDetMatrix(const std::vector<float> & nullVec, int nL, int nbins_nue);

    TMatrix getCosmicMatrix(const std::vector<float> & nullVector,
                              const std::vector<std::vector<float> > & Cosmics);

    TH1F* makeHistogram(std::vector<float> & input, 
                        double lowbin, double highbin);
    TH1F* makeHistogram(std::vector<float> & input, std::vector<float> & bins);

    std::vector<float> collapseVector(std::vector<float> input,
                                      int nbins_nue, int nbins_numu, int nL);

    std::vector<float> smoothVector(const std::vector<float> & data, unsigned int range = 3);

    float getMaximum (const std::vector<float> &);

  private:

    std::vector<float> miniboone_bins;
    std::vector<float> nue_bins;
    std::vector<float> numu_bins;
    std::vector<float> default_bins;

  };

}

#endif