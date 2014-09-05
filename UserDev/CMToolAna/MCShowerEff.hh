/**
 * \file MCShowerEff.hh
 *
 * \ingroup ShowerReco3D
 * 
 * \brief Class def header for a class MCShowerEff
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerReco3D

    @{*/

#ifndef MCSHOWEREFF_HH
#define MCSHOWEREFF_HH

#include "ana_base.hh"
#include "Geometry.hh"
#include "DetectorProperties.hh"
#include "ShowerBackTracker.hh"

namespace larlight {
  /**
     \class MCShowerEff
     User custom analysis class made by kazuhiro
   */
  class MCShowerEff : public ana_base{
  
  public:

    /// Default constructor
    MCShowerEff(){ _name="MCShowerEff"; _fout=0;};

    /// Default destructor
    virtual ~MCShowerEff(){};

    /** IMPLEMENT in MCShowerEff.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in MCShowerEff.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in MCShowerEff.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:

    std::vector<TH1D*> vMergeQEff;
    std::vector<TH1D*> vMergeQPur;

    ShowerBackTracker fBTAlg;

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
