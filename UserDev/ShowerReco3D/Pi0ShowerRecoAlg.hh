/**
 * \file Pi0ShowerRecoAlg.hh
 *
 * \ingroup ShowerReco3D
 * 
 * \brief Class def header for a class Pi0ShowerRecoAlg
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerReco3D

    @{*/
#ifndef PI0SHOWERRECOALG_HH
#define PI0SHOWERRECOALG_HH

#include "ShowerRecoAlgBase.hh"
#include "CalorimetryAlg.hh"
namespace showerreco {
  
  /**
     \class Pi0ShowerRecoAlg
     User defined class Pi0ShowerRecoAlg ... these comments are used to generate
     doxygen documentation!
  */
  class Pi0ShowerRecoAlg : public ShowerRecoAlgBase{
    
  public:
    
    /// Default constructor
    Pi0ShowerRecoAlg();
    
    /// Default destructor
    virtual ~Pi0ShowerRecoAlg(){}

    /// Function to reset algorithm, to be called @ beginning of each event
    virtual void Reset(){}

    /// Function to reconstruct a shower
    virtual ::larlight::shower Reconstruct(const std::vector< ::cluster::ClusterParamsAlg>& );

  protected:

   ::calo::CalorimetryAlg fCaloAlg;

    larutil::GeometryUtilities  *fGSer;
   
  private:
    
   std::vector < larutil::PxPoint > fStartPoint;    // for each plane
   std::vector < larutil::PxPoint > fEndPoint;    // for each plane
   std::vector < double > fOmega2D;    // for each plane
   
   std::vector < double > fEnergy;    // for each plane
   std::vector < double > fMIPEnergy;    // for each plane
   std::vector < double > fdEdx;      
   std::vector <unsigned char> fPlaneID;
   
   double fcalodEdxlength;
   double fdEdxlength;
   bool fUseArea;
   
   
  };
}

#endif
/** @} */ // end of doxygen group 

