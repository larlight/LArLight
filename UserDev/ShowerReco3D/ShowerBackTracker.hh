/**
 * \file ShowerBackTracker.hh
 *
 * \ingroup ShowerReco3D
 * 
 * \brief Class def header for a class ShowerBackTracker
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerReco3D

    @{*/

#ifndef SHOWERBACKTRACKER_HH
#define SHOWERBACKTRACKER_HH

#include "McshowerLookback.hh"
#include <vector>
namespace larlight {
  /**
     \class ShowerBackTracker
     User custom analysis class made by kazuhiro
   */
  class ShowerBackTracker {
  
  public:

    /// Default constructor
    ShowerBackTracker();

    /// Default destructor
    virtual ~ShowerBackTracker(){}

    bool Prepare(storage_manager* storage);

    std::vector<UInt_t> ShowerIDs() const { return fUniqueShowerID; }

    std::vector<float> MCShowerQ(const std::vector<const larlight::hit*> &hits)
    { return fBTAlg.MatchHitsAll(hits, fSimChMap, fShowerIDMap, false); }

  protected:

    std::vector<UInt_t> fUniqueShowerID;

    std::map<UInt_t,UInt_t> fShowerIDMap;

    std::map<UShort_t, larlight::simch> fSimChMap;

    McshowerLookback fBTAlg;
    
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
