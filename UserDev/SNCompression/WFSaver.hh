/**
 * \File WFSaver.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class WFSaver
 *
 * @author David Caratelli
 */

/** \addtogroup Analysis

    @{*/

#ifndef WFSAVER_HH
#define WFSAVER_HH

#include "ana_base.hh"
#include "GeometryUtilities.hh"
#include <math.h>
#include <fstream>

namespace larlight {
  /**
     \class WFSaver
     User custom analysis class made by SHELL_USER_NAME
  */
  class WFSaver : public ana_base{

  public:
    
    /// Default constructor
    WFSaver();
    
    /// Default destructor
    virtual ~WFSaver(){};

    /** IMPLEMENT in WFSaver.cc!
        Initialization method to be called before the analysis event loop.
    */ 

    virtual bool initialize(){}

    /** IMPLEMENT in WFSaver.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in WFSaver.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize(){}

    /// decide what units to use for plot coordinates
    void setCoordinatesInCm(bool on) { _useCmCm = on; }
    
    
  protected:
    
    /// Event Number
    int _evtNum;

    /// Boolean to decide coordinates (cm x cm) or ( w x T)
    bool _useCmCm;

    ofstream _f;
    
    GEO::View_t iview;
    std::vector<hit> ihit_v;
    
    double _w2cm;
    double _t2cm;

    int _baseline;

  };
}
#endif

/** @} */ // end of doxygen group 
