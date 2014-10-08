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
#include <limits>
#include <climits>
namespace showerreco {

  struct ShowerRecoInput_t {

    ::larutil::PxPoint start_point;
    ::larutil::PxPoint end_point;
    double             angle_2d;
    unsigned short     plane_id;
    std::vector<larutil::PxHit> hit_vector;

    ShowerRecoInput_t() : hit_vector()
    {}

  };
  
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
    virtual ::larlight::shower Reconstruct(const std::vector< ::showerreco::ShowerRecoInput_t>& clusters) = 0;
    
    /// Verbosity switch
    virtual void Verbose(bool on=true) { fVerbosity=on; }
    
  protected:
    
    /// Verbosity flag
    bool fVerbosity;
    
  };
}

#endif
/** @} */ // end of doxygen group 

