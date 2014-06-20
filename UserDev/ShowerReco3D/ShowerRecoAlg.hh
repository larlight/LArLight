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

#include <iostream>
#include "shower.hh"
#include "CMatchManager.hh"
#include "CalorimetryAlg.hh"

namespace shower {
  
  /**
     \class ShowerRecoAlg
     User defined class ShowerRecoAlg ... these comments are used to generate
     doxygen documentation!
  */
  class ShowerRecoAlg{
    
  public:
    
    /// Default constructor
    ShowerRecoAlg();
    
    /// Default destructor
    virtual ~ShowerRecoAlg(){}

    /// Function to reset algorithm, to be called @ beginning of each event
    void Reset(){}

    /// Function to reconstruct a shower
    ::larlight::shower Reconstruct(const std::vector< ::cluster::ClusterParamsAlgNew>& );
    
  protected:

    ::calo::CalorimetryAlg fCaloAlg;

  };
}

#endif
/** @} */ // end of doxygen group 

