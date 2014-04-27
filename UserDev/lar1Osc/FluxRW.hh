#ifndef FLUXRW_HH
#define FLUXRW_HH

#include "Rtypes.h"
#include <vector>

class TH1D;
namespace lar1{
  class FluxRW {

   public:
    FluxRW();
    ~FluxRW();

    Double_t GetWeight(Double_t energy, Int_t ntype, Int_t ptype);
    TH1D*    GetWeightHist(Int_t ntype, Int_t ptype);

    std::string GetEnv( const std::string & var );


   private:

    Double_t fBinSize;
    std::vector<Double_t> fGS[4][4]; //fGS[ntype][ptype]
    std::vector<Double_t> fMB[4][4]; //fMB[ntype][ptype]

  };
}
#endif
