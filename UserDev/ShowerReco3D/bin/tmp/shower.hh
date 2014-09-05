/**
 * \file Alg_Class_Name.hh
 *
 * \ingroup ShowerReco3D
 * 
 * \brief Class def header for a class Alg_Class_Name
 *
 * @author SHELL_USER_NAME
 */

/** \addtogroup ShowerReco3D

    @{*/
#ifndef ALG_CLASS_NAME_HH
#define ALG_CLASS_NAME_HH

#include "ShowerRecoAlgBase.hh"
namespace showerreco {
  
  /**
     \class Alg_Class_Name
     User defined class Alg_Class_Name ... these comments are used to generate
     doxygen documentation!
  */
  class Alg_Class_Name : public ShowerRecoAlgBase{
    
  public:
    
    /// Default constructor
    Alg_Class_Name();
    
    /// Default destructor
    virtual ~Alg_Class_Name(){}

    /// Function to reset algorithm, to be called @ beginning of each event
    virtual void Reset(){}

    /// Function to reconstruct a shower
    virtual ::larlight::shower Reconstruct(const std::vector< ::cluster::ClusterParamsAlg>& )
    { return ::larlight::shower(); }

  };
}

#endif
/** @} */ // end of doxygen group 

