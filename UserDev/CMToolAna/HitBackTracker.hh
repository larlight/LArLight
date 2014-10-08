/**
 * \file HitBackTracker.hh
 *
 * \ingroup CMToolAna
 * 
 * \brief Class def header for a class HitBackTracker
 *
 * @author david
 */

/** \addtogroup CMToolAna

    @{*/

#ifndef HITBACKTRACKER_HH
#define HITBACKTRACKER_HH

#include "ana_base.hh"

namespace larlight {
  /**
     \class HitBackTracker
     User custom analysis class made by david
   */
  class HitBackTracker {
  
  public:

    /// Default constructor
    HitBackTracker(){ };

    /// Default destructor
    virtual ~HitBackTracker(){ _verbosity=false; };

    /// Function to Find MC electrons associated with hit
    /// Second arg (optional): look for e- n-sigma away from PeakTime 
    double FindElectrons( larlight::hit h, double sigma = 1 );

    /// Function to Find MC IDEs associated with hit
    /// Second arg (optional): look for e- n-sigma away from PeakTime 
    std::vector<larlight::ide> FindIDEs( larlight::hit h, double sigma = 1 );

    /// Make SimchMap
    void MakeSimchMap( larlight::event_simch *evt_simch );

    /// Set Verbosity
    void SetVerbosity(bool on) { _verbosity = on; }


    protected:

    std::map<UShort_t, larlight::simch> _simch_map;

    bool _verbosity;

  };
}
#endif

//**************************************************************************
// 
// For Analysis framework documentation, read Manual.pdf here:
//
// http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
//
//**************************************************************************

/** @} */ // end of doxygen group 
