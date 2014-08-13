/**
 * \file MCinfo.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class MCinfo
 *
 * @author davidc1
 */

/** \addtogroup Analysis

    @{*/

#ifndef MCINFO_HH
#define MCINFO_HH

#include "ana_base.hh"
#include "Geometry.hh"
#include <map>
#include <vector>
#include <TH1D.h>
#include <TH2D.h>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <deque>
#include <fstream>
#include <TVector3.h>
#include <TLorentzVector.h>


namespace larlight {
  /**
     \class MCinfo
     User custom analysis class made by davidc1
   */
  class MCinfo : public ana_base{
  
  public:

    /// Default constructor
    MCinfo(){ _name="MCinfo"; _fout=0;};

    /// Default destructor
    virtual ~MCinfo(){};

    /** IMPLEMENT in MCinfo.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in MCinfo.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in MCinfo.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void SetVerbose(bool on) { _verbose = on; }

    protected:

    bool _verbose;
    int _evtN;
    ofstream outFile;

    TH1D * eEnergy;
    TH1D * eAngle;
    TH1D * gEnergypre;
    TH1D * gEnergypost;
    TH1D * gAngle;
    TH2D * eEvseAngle;
    TH2D * eEvsgAngle;
    TH2D * gEvsgAngle;

  };
}
#endif

/** @} */ // end of doxygen group 
