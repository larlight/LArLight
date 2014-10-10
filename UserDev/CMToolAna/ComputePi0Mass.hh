/**
 * \file ComputePi0Mass.hh
 *
 * \ingroup CMergePerformance
 * 
 * \brief This class is to run matching on pi0 files and calculate a pi0 mass peak from their output.
 *
 * @author davidkaleko
 */

/** \addtogroup CMergePerformance

    @{*/

#ifndef COMPUTEPI0MASS_HH
#define COMPUTEPI0MASS_HH

#include "ana_base.hh"
#include "CMatchManager.hh"
#include "CRUHelper.hh"
#include "ShowerRecoAlg.hh"
#include "ShowerBackTracker.hh"
#include "ShowerReco3D/ShowerCalo.hh"

namespace larlight {
  /**
     \class ComputePi0Mass
     User custom analysis class made by davidkaleko
   */
  class ComputePi0Mass : public ana_base{
  
  public:

    /// Default constructor
    ComputePi0Mass(){ 
      _name="ComputePi0Mass"; 
      _fout=0; 
      _debug=false; 
      _applyEnergyCorrection=false;
      _shower_type = DATA::Shower;
      _cluster_type = DATA::Cluster;
    };

    /// Default destructor
    virtual ~ComputePi0Mass(){};

    /** IMPLEMENT in ComputePi0Mass.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in ComputePi0Mass.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in ComputePi0Mass.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void SetClusterType(DATA::DATA_TYPE type) {_cluster_type = type;}

    void SetShowerType(DATA::DATA_TYPE type) {_shower_type = type; }

    void SetDebug(bool flag) { _debug = flag; }

    float Pi0MassFormula3D(  float Energy1, float Energy2, TVector3 Direction3D_1, TVector3 Direction3D_2);

    void ComputeEnergyCorrection(storage_manager* storage);

    void SetApplyEnergyCorrection(bool flag){ _applyEnergyCorrection = flag; };

    
    static ComputePi0Mass* GetInstance() { 
      if(!me) me = new ComputePi0Mass;
      return me; 
    }
    
    float GetMass(){ return _mass; };

  protected:

    ::showerreco::ShowerCalo fSECaloAlg;

    DATA::DATA_TYPE _shower_type;
    DATA::DATA_TYPE _cluster_type;
    
    TH1D* hPi0MassPeak;

    TH1D* hEnergyCorr_MomToDaughter;
    TH1D* hElectronCorr_DepToDet;

    bool _debug;
    
    float _mass;

    std::vector<double> fEnergyCorr_MomToDaughter;
    std::vector<double> fElectronCorr_DepToDet;
    std::vector<double> fChargeCorr_DetToPeak;
    ShowerBackTracker fBTAlg;
    bool _applyEnergyCorrection;

    
  private:
    
    static ComputePi0Mass *me; // attempt at a shared object instance ptr


  };
}
#endif

/** @} */ // end of doxygen group 
