/**
 * \file ShowerQuality.hh
 *
 * \ingroup ShowerReco3D
 * 
 * \brief Class def header for a class ShowerQuality
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerReco3D

    @{*/

#ifndef SHOWERQUALITY_HH
#define SHOWERQUALITY_HH

#include "ana_base.hh"

namespace larlight {
  /**
     \class ShowerQuality
     User custom analysis class made by kazuhiro
   */
  class ShowerQuality : public ana_base{
  
  public:

    /// Default constructor
    ShowerQuality()
    { 
      _name="ShowerQuality"; 
      _fout=0; 
      event_clear();
      plane_clear();
    };

    /// Default destructor
    virtual ~ShowerQuality(){};

    /** IMPLEMENT in ShowerQuality.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in ShowerQuality.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in ShowerQuality.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:

    void event_clear() {
      _dist = 0;
      _angle_diff = -1;
      _mc_energy = -1;
      _energy_containment = -1;
      _best_plane = 4;
      _num_mcs = 0;
    }

    void plane_clear() {
      _plane = 4;
      _dEdX = -1;
      _energy = -1;
      _mip_energy = -1;
    }

    double _dist;
    double _angle_diff;
    double _energy_containment;
    unsigned int _plane;
    unsigned int _best_plane;
    unsigned int _num_mcs;
    double _dEdX;
    double _energy;
    double _mip_energy;
    double _mc_energy;

    TTree *fTree;

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
