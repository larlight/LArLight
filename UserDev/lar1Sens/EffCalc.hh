#ifndef EFFCALC_HH
#define EFFCALC_HH


#include <iostream>
#include <fstream>
#include <sstream>

#include "TString.h"
#include "THStack.h"
#include "TPad.h"

#include "PlotUtils.hh"
#include "SensUtils.hh"


namespace lar1{

  class EffCalc
  {
  public:
    EffCalc();
    ~EffCalc();
  
    void getNominalData();

    unsigned int readData(std::vector<std::vector<float> > & data,
                          double photonGap = 10000, 
                          double minVertexEnergyPhoton = 10000, 
                          double minVertexEnergySignal = 0);

    unsigned int readData(std::vector<std::vector<float> > & data, TString filename);


    void setDetector(TString d){detector = d;}

    void setPath(TString Path){path=Path;}

    TH1F * makeNueRatio( TString file_denominator, TString file_numerator,
                         std::vector<int>, TString title = "", double max = 1.1);


    TH1F * makeEfficiency( int channel,
                           double photonGap = 10000, 
                           double minVertexEnergyPhoton = 10000, 
                           double minVertexEnergySignal = 0);

    TH1F* makeNueEfficiency(double photonGap = 10000, 
                            double minVertexEnergyPhoton = 10000, 
                            double minVertexEnergySignal = 0);

    TH1F* makePhotonEfficiency( double photonGap = 10000, 
                                double minVertexEnergyPhoton = 10000, 
                                double minVertexEnergySignal = 0);

    TCanvas * createBackground( double photonGap = 10000, 
                                double minVertexEnergyPhoton = 10000, 
                                double minVertexEnergySignal = 0,
                                bool normToNominal = true);

    /* data */

  private:

    // parseData opens a file and reads it into the vector.
    // It checks that the file has the correct number of entries, 
    // and the return value is the number of bins used (though
    // some of the entries of "result" are the bin values)
    unsigned int parseData(TString fileName, std::vector<std::vector<float> > & results);


    TString path;
    TString fileEnd, fileStart;
    TString detector;

    std::vector<float> bins;

    PlotUtils plotUtils;
    SensUtils sensUtils;

    std::vector<TString> legend;
    std::vector<std::vector<float> > nominalData;

    std::vector<float> nominalBinIntegral;
    std::vector<float> nominalBkgIntegral;

  };



}

#endif