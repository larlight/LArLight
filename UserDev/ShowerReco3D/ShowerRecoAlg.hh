/**
 * \file ShowerRecoAlg.hh
 *
 * \ingroup ShowerReco3D
 * 
 * \brief Class def header for a class ShowerRecoAlg
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerReco3D

    @{*/
#ifndef SHOWERRECOALG_HH
#define SHOWERRECOALG_HH

#include "ShowerRecoAlgBase.hh"
#include "CalorimetryAlg.hh"
namespace showerreco {
  
  /**
     \class ShowerRecoAlg
     User defined class ShowerRecoAlg ... these comments are used to generate
     doxygen documentation!
  */
  class ShowerRecoAlg : public ShowerRecoAlgBase{
    
  public:
    
    /// Default constructor
    ShowerRecoAlg();
    
    /// Default destructor
    virtual ~ShowerRecoAlg(){}

    /// Function to reset algorithm, to be called @ beginning of each event
    virtual void Reset() { ShowerRecoAlgBase::Reset(); }

  protected:

    /// Function to reconstruct a shower
    virtual ::larlight::shower RecoOneShower(const std::vector< ::showerreco::ShowerCluster_t>& );

  protected:

   ::calo::CalorimetryAlg fCaloAlg;

    larutil::GeometryUtilities  *fGSer;
   
  private:
    
   double fcalodEdxlength;
   double fdEdxlength;
   bool fUseArea;
   
   
  };
}

#endif
/** @} */ // end of doxygen group 

