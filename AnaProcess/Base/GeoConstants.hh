/**
 * \file GeoConstants.hh
 *
 * \ingroup Base
 * 
 * \brief defines geometry related constants
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Base

    @{*/

#ifndef GEOCONSTANTS_HH
#define GEOCONSTANTS_HH
#include <string>

/// Namespace of everything in this framework
namespace larlight{
  
  namespace GEO {

    /// Enumerate the possible plane projections
    enum View_t {
      kU,       ///< planes which measure U
      kV,       ///< planes which measure V
      kW,       ///< soon to be deprecated, planes which measure W (third view for Bo, MicroBooNE, etc)
      kZ=kW,    ///< planes which measure Z direction (ie wires are vertical)
      k3D,      ///< 3 dimensional objects, potentially hits, clusters, prongs, etc
      kUnknown  ///< unknown view
    };
    
    /// Enumerate the possible signal types
    enum SigType_t {
      kInduction,   ///< signal from induction planes
      kCollection,  ///< signal from collection planes
      kMysteryType  ///< who knows?
    };
    
  }
}
#endif
/** @} */ // end of doxygen group
