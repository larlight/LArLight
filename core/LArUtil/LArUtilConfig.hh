/**
 * \file LArUtilConfig.hh
 *
 * \ingroup LArUtil
 * 
 * \brief Class def header for a class LArUtilConfig
 *
 * @author kazuhiro
 */

/** \addtogroup LArUtil

    @{*/
#ifndef LARUTILCONFIG_HH
#define LARUTILCONFIG_HH

#include <iostream>
#include "Base-TypeDef.hh"

namespace larutil {
  /**
     \class LArUtilConfig
     User defined class LArUtilConfig ... these comments are used to generate
     doxygen documentation!
  */
  class LArUtilConfig : public larlight::larlight_base {
    
  private:
    
    /// Default constructor
    LArUtilConfig(){};
    
    /// Default destructor
    virtual ~LArUtilConfig(){};
    
    static larlight::GEO::DetId_t _detector;
    
  public:
    
    static bool SetDetector(larlight::GEO::DetId_t type);
    
    static const larlight::GEO::DetId_t Detector() { return _detector;}
    
  };
}

#endif
/** @} */ // end of doxygen group 

