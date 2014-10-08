/**
 * \file ArianaPi0Test.hh
 *
 * \ingroup ArianaPi0Test
 * 
 * \brief Class def header for a class ArianaPi0Test
 *
 * @author ah673
 */

/** \addtogroup ArianaPi0Test

    @{*/

#ifndef ARIANAPI0TEST_HH
#define ARIANAPI0TEST_HH

#include "ana_base.hh"

namespace larlight {
  /**
     \class ArianaPi0Test
     User custom analysis class made by ah673
   */
  class ArianaPi0Test : public ana_base{
  
  public:

    /// Default constructor
    ArianaPi0Test() : ana_base() { _name="ArianaPi0Test"; }

    /// Default destructor
    virtual ~ArianaPi0Test(){};

    /** IMPLEMENT in ArianaPi0Test.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in ArianaPi0Test.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in ArianaPi0Test.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    protected:

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
