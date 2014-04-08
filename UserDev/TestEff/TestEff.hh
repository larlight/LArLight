/**
 * \file TestEff.hh
 *
 * \ingroup TestEff
 * 
 * \brief Class def header for a class TestEff
 *
 * @author andrzej
 */

/** \addtogroup TestEff

    @{*/

#ifndef TESTEFF_HH
#define TESTEFF_HH

#include "ana_base.hh"
#include "ClusterParamsAlgNew.hh"


namespace larlight {
  /**
     \class TestEff
     User custom analysis class made by andrzej
   */
  class TestEff : public ana_base{
  
  public:

    /// Default constructor
    TestEff() : ana_base() { _name="TestEff"; }

    /// Default destructor
    virtual ~TestEff(){};

    /** IMPLEMENT in TestEff.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in TestEff.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in TestEff.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    protected:

    ::cluster::ClusterParamsAlgNew  fCPAlg; 
      
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
