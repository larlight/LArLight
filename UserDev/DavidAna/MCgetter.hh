/**
 * \file MCgetter.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class MCgetter
 *
 * @author David Caratelli
 */

/** \addtogroup Analysis

    @{*/

#ifndef MCGETTER_HH
#define MCGETTER_HH

#include "ana_base.hh"

namespace larlight {
  /**
     \class MCgetter
     User custom analysis class made by SHELL_USER_NAME
   */
  class MCgetter{
  
  public:

    /// Default constructor
    MCgetter(){};

    /// Default destructor
    virtual ~MCgetter(){ _Ecut=0; };

    /// Print Daughter Information for user-selected particle
    void drawDaughters(const event_mcpart* event_part, mcpart paticle);

    /// Fill Particle Map
    void fillParticleMap(const event_mcpart *event_part);

    protected:

    // Map for particle element in list and trackID
    // MAP: <trackID, Position in event_part>
    std::map<int,int> _particleMap;
    
    /// Cut on Energy for particle info to be displayed
    double _Ecut;


  };
}
#endif

/** @} */ // end of doxygen group 
