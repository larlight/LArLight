/**
 * \file LArUtilManager.hh
 *
 * \ingroup LArUtil
 * 
 * \brief Class def header for a class LArUtilManager
 *
 * @author kazuhiro
 */

/** \addtogroup LArUtil

    @{*/
#ifndef LARUTILMANAGER_HH
#define LARUTILMANAGER_HH

#include <iostream>
#include "Geometry.hh"
#include "LArProperties.hh"
#include "DetectorProperties.hh"
#include "GeometryUtilities.hh"

namespace larutil {
  /**
     \class LArUtilManager
     User defined class LArUtilManager ... these comments are used to generate
     doxygen documentation!
  */
  class LArUtilManager : public larlight::larlight_base {
    
  private:
    
    /// Default constructor
    LArUtilManager(){};
    
    /// Default destructor
    virtual ~LArUtilManager(){};

    /// Method to execute reconfigure for utilitites
    static bool ReconfigureUtilities();
    
  public:

    /// Method to reconfigure utiities for the provided detector type
    static bool Reconfigure(larlight::GEO::DetId_t type);
    
  };
}

#endif
/** @} */ // end of doxygen group 

