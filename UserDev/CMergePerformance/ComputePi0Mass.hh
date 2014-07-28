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

namespace larlight {
  /**
     \class ComputePi0Mass
     User custom analysis class made by davidkaleko
   */
  class ComputePi0Mass : public ana_base{
  
  public:

    /// Default constructor
    ComputePi0Mass(){ _name="ComputePi0Mass"; _fout=0; _debug=false; };

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

    void SetDebug(bool flag) { _debug = flag; }

    float Pi0MassFormula3D(  float Energy1, float Energy2, TVector3 Direction3D_1, TVector3 Direction3D_2);

    void ComputeEnergyCorrection(storage_manager* storage);
  protected:

    DATA::DATA_TYPE _shower_type;
    
    TH1D* hPi0MassPeak;

    bool _debug;

    std::vector<double> fEnergyCorr_MomToDaughter;
    std::vector<double> fElectronCorr_DepToDet;
    std::vector<double> fChargeCorr_DetToPeak;
    ShowerBackTracker fBTAlg;
    bool _applyEnergyCorrection;
  };
}
#endif

/** @} */ // end of doxygen group 
