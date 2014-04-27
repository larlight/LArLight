/*
* This class is a helper class for the chi2 calculation done in the 
* other files.
* 
*

 */

#ifndef SENSUTILS_HH
#define SENSUTILS_HH

#include "TH1F.h"

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


    /* data */
  };

}

#endif