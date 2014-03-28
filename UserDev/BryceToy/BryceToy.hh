/**
 * \file BryceToy.hh
 *
 * \ingroup BryceToy
 * 
 * \brief Class def header for a class BryceToy
 *
 * @author guest38
 */

/** \addtogroup BryceToy

    @{*/

#ifndef BRYCETOY_HH
#define BRYCETOY_HH

#include<TH2D.h>
#include<vector>
#include "ana_base.hh"
#include "McshowerLookback.hh"

namespace larlight {
  /**
     \class BryceToy
     User custom analysis class made by guest38
   */
  //double cutoff = 0.005;

  class BryceToy : public ana_base{
  
  public:

    /// Default constructor
    BryceToy() : ana_base() { _name="BryceToy"; h=0; }

    /// Default destructor
    virtual ~BryceToy(){};

    /** IMPLEMENT in BryceToy.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in BryceToy.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in BryceToy.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    int totalmult = 0;
    double cutoff = 0.01;
    
    protected:

    TH2D* h;

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
