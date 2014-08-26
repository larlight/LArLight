/**
 * \file ShowerRecoAlgBase.hh
 *
 * \ingroup ShowerReco3D
 * 
 * \brief Class def header for a class ShowerRecoAlgBase
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerReco3D

    @{*/
#ifndef SHOWERRECOALGBASE_HH
#define SHOWERRECOALGBASE_HH

#include <iostream>
#include "shower.hh"
#include "ClusterParamsAlg.hh"
namespace showerreco {
  
  /**
     \class ShowerRecoAlgBase
     User defined class ShowerRecoAlgBase ... these comments are used to generate
     doxygen documentation!
  */
  class ShowerRecoAlgBase{
    
  public:
    
    /// Default constructor
    ShowerRecoAlgBase();
    
    /// Default destructor
    virtual ~ShowerRecoAlgBase(){}

    /// Function to reset algorithm, to be called @ beginning of each event
    virtual void Reset() = 0;

    /// Function to reconstruct a shower
    virtual ::larlight::shower Reconstruct(const std::vector< ::cluster::ClusterParamsAlg>& ) = 0;
    
    /// Verbosity switch
    void Verbose(bool on=true) { fVerbosity=on; }
    
  protected:
    
    /// Verbosity flag
    bool fVerbosity;
    
  };
}

#endif
/** @} */ // end of doxygen group 

