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
      _dist_max=10; 
      _angle_max=10;
      _energy_containment=0.9;
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

    void SetQualityCut(double dist, double angle, double ec)
    { _dist_max = dist; _angle_max = angle; _energy_containment=ec;}

  protected:

    double _dist_max, _angle_max, _energy_containment;
    
    std::vector<TH1D*> vdEdX;

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
