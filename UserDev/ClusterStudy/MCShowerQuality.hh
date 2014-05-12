/**
 * \file MCShowerQuality.hh
 *
 * \ingroup ClusterStudy
 * 
 * \brief Class def header for a class MCShowerQuality
 *
 * @author kazuhiro
 */

/** \addtogroup ClusterStudy

    @{*/

#ifndef MCSHOWERQUALITY_HH
#define MCSHOWERQUALITY_HH

#include <TH1D.h>
#include <TH2D.h>

#include "ana_base.hh"

namespace larlight {
  /**
     \class MCShowerQuality
     User custom analysis class made by kazuhiro
   */
  class MCShowerQuality : public ana_base{
  
  public:

    /// Default constructor
    MCShowerQuality();

    /// Default destructor
    virtual ~MCShowerQuality(){};

    void SetEnergyCut(double energy) {_energy_min = energy;}

    /** IMPLEMENT in MCShowerQuality.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in MCShowerQuality.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in MCShowerQuality.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    protected:

    double _energy_min;

    double _dx;

    TH1D* _hCount;

    TH2D* _hEnergy;

    TH2D* _hDEDX;

    TH1D* _hEnergyFrac;

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
