/**
 * \file ComputePi0Mass_RG.hh
 *
 * \ingroup CMergePerformance
 * 
 * \brief This class is to run matching on pi0 files and calculate a pi0 mass peak from their output.
 *
 * @author davidkaleko
 */

/** \addtogroup CMergePerformance

    @{*/

#ifndef COMPUTEPI0MASS_RG_HH
#define COMPUTEPI0MASS_RG_HH

#include "ana_base.hh"
#include "CMatchManager.hh"
#include "CRUHelper.hh"
#include "ShowerRecoAlg.hh"
#include "ShowerBackTracker.hh"

namespace larlight {
  /**
     \class ComputePi0Mass_RG
     User custom analysis class made by davidkaleko
   */
  class ComputePi0Mass_RG : public ana_base{
  
  public:

    /// Default constructor
    ComputePi0Mass_RG(){ 
      _name="ComputePi0Mass_RG"; 
      _fout=0; 
      _debug=false; 
      _applyEnergyCorrection=false;
      _shower_type = DATA::Shower;
      _cluster_type = DATA::Cluster;
    };


    /// Default destructor
    virtual ~ComputePi0Mass_RG(){};

    /** IMPLEMENT in ComputePi0Mass_RG.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in ComputePi0Mass_RG.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in ComputePi0Mass_RG.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void SetClusterType(DATA::DATA_TYPE type) {_cluster_type = type;}

    void SetShowerType(DATA::DATA_TYPE type) {_shower_type = type; }

    void SetDebug(bool flag) { _debug = flag; }

    float Pi0MassFormula3D(  float Energy1, float Energy2, TVector3 Direction3D_1, TVector3 Direction3D_2);
    float Pi0CosCM(  float Energy1, float Energy2);

    void ComputeEnergyCorrection(storage_manager* storage);

    void SetApplyEnergyCorrection(bool flag){ _applyEnergyCorrection = flag; };

    
    static ComputePi0Mass_RG* GetInstance() { 
      if(!me) me = new ComputePi0Mass_RG;
      return me; 
    }
    
    float GetMass(){ return _mass; };

  protected:

    DATA::DATA_TYPE _shower_type;
    DATA::DATA_TYPE _cluster_type;    
    TH1D* hPi0MassPeak;
    TH1D* hPi0MassPeakPull;
    TH1D* hPi0CosCM;

    TH1D* hPi0MassPeak_GoodEnergy;
    TH1D* hPi0MassPeak_GoodAngle;
    TH1D* hPi0MassPeak_GoodAnglePull;
    TH1D* hPi0MassPeak_TrueDetector;

    TH2D* hPhotondos;
    TH2D* hOpeningAngle;
    TH1D* hPi0MassPeakdoscut;
    TH1D* hPi0MassPeakanglecut;
    TH1D* hPi0MassPeakdosanglecut;




    TH1D* hEnergyCorr_MomToDaughter;
    TH1D* hElectronCorr_DepToDet;

    bool _debug;
    
    float _mass;
    float _Pi0mass = 134.98;// in MeV
    float _dorient_EE_calb = 1.95;// in MeV
    float _mass_goodEnergy;
    float _mass_goodAngle;
    float _mass_detectorTrue;
    float _pi0_coscm;
    float _photon_dosReco;
    float _photon_dosTrue;

    std::vector<double> fEnergyCorr_MomToDaughter;
    std::vector<double> fElectronCorr_DepToDet;
    std::vector<double> fChargeCorr_DetToPeak;
    ShowerBackTracker fBTAlg;
    bool _applyEnergyCorrection;

    
  private:
    
    static ComputePi0Mass_RG *me; // attempt at a shared object instance ptr


  };
}
#endif

/** @} */ // end of doxygen group 
