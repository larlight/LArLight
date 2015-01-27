#ifndef UTILS_HH
#define UTILS_HH

#include "FluxRW.hh"
#include "FluxRWnubar.hh"
#include "Rtypes.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include <vector>
#include "TDecompLU.h"
#include "TRandom.h"
#include "horn2boost.hh"

class TH1D;

namespace lar1{

  enum beamMode {kNu, kNu_Fosc, kNubar, kNubar_Fosc };
  enum detectorGeom {kND, kUB, kIC, kND_long, kMB};
  enum detectorLoc  {k100m, k150m, k175m, k200m, k470m, k600m_onaxis, k600m_offaxis};


  class Utils {
    
  public:
    
    Utils();
    ~Utils();

    void setMC_Generation(int g);
    void reconfigure();
    
    Double_t GetFluxWeight( Double_t energy, bool isFosc, Int_t inno, Int_t ndecay );

    Double_t GetTwoHornWeight( Double_t energy, bool isFosc, Int_t inno, Int_t ndecay, Int_t iLoc );

    Double_t GetTheta( const TVector3 & mom, 
                       const TVector3 & ref = TVector3(0,0,1) ) const;
    Double_t GetTheta( Double_t px, Double_t py, 
                       Double_t pz, const TVector3 & ref = TVector3(0,0,1) ) const;
    Double_t GetPhi(   TVector3 mom, 
                       TVector3 ref = TVector3(0,0,1) ) const;
    Double_t GetPhi(   Double_t px, Double_t py ) const;

    Double_t NuEnergyCCQE( Double_t lep_energy, Double_t lep_p, 
                           Double_t lep_theta, Double_t mass, 
                           Int_t mode, bool verbose = false ) const;

    Double_t NuEnergyCalo( std::vector<Int_t> *pdg, std::vector<TLorentzVector> *energy, 
                           double lepEnergy,
                           bool smearing = true,
                           Bool_t include_neutrons = false, 
                           Bool_t include_pizeros = false, 
                           Double_t prot_thresh = 0.02, 
                           Double_t pion_thresh = 0.02, 
                           Double_t kaon_thresh = 0.02,
                           bool verbose = false );   

   
    Double_t VertexEnergy( std::vector<Int_t> *pdg, std::vector<TLorentzVector> *energy, 
                           bool smearing = true,
                           Double_t prot_thresh = 0.02, 
                           Double_t pion_thresh = 0.02, 
                           Double_t kaon_threshold = 0.02,
                           bool verbose = false);

    Double_t TotalPhotonEnergy( Int_t idet, 
                                std::vector<TLorentzVector> *p1pos,
                                std::vector<TLorentzVector> *p1mom,
                                std::vector<TLorentzVector> *p2pos,
                                std::vector<TLorentzVector> *p2mom,
                                std::vector<TLorentzVector> *miscpos,
                                std::vector<TLorentzVector> *miscmom ) const;

    Double_t GetLeptonEnergy(   Double_t energy,
                                bool smearing,
                                int PDG, bool contained=true,
                                double containedLength=0);


    // Detector property functions:
    Bool_t   IsFiducial(     Int_t idet, const TVector3 & vtx,
                             std::string signal = "" ) const;
    Bool_t   IsActive(       Int_t idet, TVector3 vtx, double cut = 0 ) const;
    Bool_t   IsFiducialMB(   Int_t idet, TVector3 vtx) const;
    Double_t GetFidMass(     Int_t idet, std::string signal = "" ) const;
    Double_t GetActiveMass(  Int_t idet ) const;
    void     GetDetBoundary( Int_t idet, Double_t &xmin, Double_t &xmax, 
  			                     Double_t &ymin, Double_t &ymax, 
                             Double_t &zmin, Double_t &zmax ) const; 
    
    Double_t GetPOTNorm(     Int_t iflux, Int_t iLoc ) const;
    Double_t GetPOTNormNuMI( Int_t iflux, Int_t iLoc ) const;

    double   GetContainedLength(const TVector3 & startPoint, 
                                const TVector3 & startDir, int idet,
                                double precision = 0.01) const;
    double   GetLengthToStart(  const TVector3 & startPoint, 
                                const TVector3 & startDir, int idet) const;
    double   GetYZLengthToStart(const TVector3 & startPoint, 
                                const TVector3 & startDir, int idet) const;


    //This function returns true if the line along direction startDir that starts at startPos
    //intersects the parallelogram defined by the 3 points given.
    //NB a parallelogram needs 4 points.  The last point is defined as p1 + a +b, where a and b
    //are the vectors from p1 to p2, p1 to p3.  
    Bool_t   IntersectsPlane(const TVector3 & startPos, const TVector3 & startDir,
                             const TVector3 & planeCorner1,
                             const TVector3 & planeCorner2,
                             const TVector3 & planeCorner3,
                             const bool verbose = false) const;

   private:

    FluxRW      fluxrw_nu;
    FluxRWnubar fluxrw_nubar;

    horn2boost  fHorn2Boost;

    double PotNormNubar;
    double PotNormNu;

    // const static Int_t kNu         = 0;
    // const static Int_t kNu_Fosc    = 1;
    // const static Int_t kNubar 	   = 2;
    // const static Int_t kNubar_Fosc = 3; 


    // Enumerate the detectors
    // const static Int_t kND = 0;
    // const static Int_t kUB = 1;
    // const static Int_t kFD = 2; 
    // const static Int_t kMB = 3;
    // const static Int_t kIC = 4;
    // const static Int_t kND_long = 5;
    // const static Int_t kIC_600 = 6;
    // const static Int_t kIC_800 = 7;
    // const static Int_t kND150 = 8;
    // const static Int_t kND175 = 9;
    // const static Int_t kND200 = 10;

    int mc_generation;
    
    TRandom randService;

    double nue_fidCut_x,  nue_fidCut_y,  nue_fidCut_zUp,  nue_fidCut_zDown;
    double numu_fidCut_x, numu_fidCut_y, numu_fidCut_zUp, numu_fidCut_zDown;
    double cathode_cut;
    double nd_xmin, nd_xmax, nd_ymin, nd_ymax, nd_zmin, nd_zmax;
    double nd_long_xmin, nd_long_xmax, nd_long_ymin, 
           nd_long_ymax, nd_long_zmin, nd_long_zmax;
    double ub_xmin, ub_xmax, ub_ymin, ub_ymax, ub_zmin, ub_zmax;
    double fd_xmin, fd_xmax, fd_ymin, fd_ymax, fd_zmin, fd_zmax;
    double ic_xmin, ic_xmax, ic_ymin, ic_ymax, ic_zmin, ic_zmax;
    TVector3 kMB_center;
    double kMB_radius, kMB_fidcut;
    
  };

} //namespace lar1


#endif
