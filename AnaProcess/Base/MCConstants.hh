/**
 * \file MCConstants.hh
 *
 * \ingroup Base
 * 
 * \brief defines constants used for MC
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Base

    @{*/

#ifndef MCCONSTANTS_HH
#define MCCONSTANTS_HH
#include <string>

/// Namespace of everything in this framework
namespace larlight{
  
  /// Defines constants for MC truth information
  namespace MC{
    
    /// MC generator type
    enum Origin_t{
      kUnknown,           ///< ???
      kBeamNeutrino,      ///< Beam neutrinos
      kCosmicRay,         ///< Cosmic rays
      kSuperNovaNeutrino, ///< Supernova neutrinos
      kSingleParticle     ///< single particles thrown at the detector
    };
  }
  
}
#endif
/** @} */ // end of doxygen group
